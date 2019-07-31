#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"
#include <AsgTools/MsgStream.h>
#include <AsgTools/MsgStreamMacros.h>
#include "xAODCore/AuxInfoBase.h"
#include "xAODCore/ShallowAuxInfo.h"
#include "xAODCore/ShallowCopy.h"
#include <FourMuonAnalysis/XFourMuonAnalysis.h>
#include <FourMuonAnalysis/Utils.h>

// match Quad with two Pairs
OniaPairVec XFourMuonAnalysis :: matchOnia(std::string quadCombinationCode, std::vector<const xAOD::Vertex*> good_pairs) {

  // vector of matched onia pairs < (onia1, onia2), (onia3, onia4), ... >
  // all possible combinations included: DC1,DC2; DC2,DC1; AC1,AC2; AC2,AC1; ....
  OniaPairVec onia_pairs;

  //get track index from quad CombinationCode
  std::vector<int> trkIndices;
  for(int i=0; i<(int)quadCombinationCode.length(); i++) {
    trkIndices.push_back(int(quadCombinationCode[i]-'0'));
  }

  // do permutation for the 4 track index: "ijkl"
  do {
    // check if "ij" matches an Onia
    std::string pairCC1 = std::to_string(trkIndices[0]) + std::to_string(trkIndices[1]); 
    // check if "kl" matches an Onia
    std::string pairCC2 = std::to_string(trkIndices[2]) + std::to_string(trkIndices[3]); 

    // loop all Pairs to match the CombinationCode
    const xAOD::Vertex *pair1=0, *pair2=0;
    for(auto pair: good_pairs) {
      if(pair->auxdataConst<std::string>("CombinationCode")==pairCC1) {pair1=pair; break;}
    }
    for(auto pair: good_pairs) {
      if(pair->auxdataConst<std::string>("CombinationCode")==pairCC2) {pair2=pair; break;}
    }
    // found a pair of onias
    if(pair1 && pair2) {
      onia_pairs.push_back(std::make_pair(pair1, pair2));
    }
    // go to next permutation
  } while(std::next_permutation(trkIndices.begin(), trkIndices.end()));

  return onia_pairs;

}

// match four-muon with two Pairs
OniaPairVec XFourMuonAnalysis :: matchOnia(std::vector<const xAOD::Muon*> good_mus, std::vector<const xAOD::Vertex*> good_pairs) {

  // vector of matched onia pairs < (onia1, onia2), (onia3, onia4), ... >
  // all possible combinations included: DC1,DC2; DC2,DC1; AC1,AC2; AC2,AC1; ....
  OniaPairVec onia_pairs;

  // loop all pairs
  for(int ip=0; ip<(int)good_pairs.size(); ip++) {
    auto ipair=good_pairs.at(ip);
    const xAOD::Muon * Muon0 = NULL;
    const xAOD::Muon * Muon1 = NULL;
    // get the muons for this pair
    if(matchFourMuon(ipair, good_mus)) getConstiMuons(ipair, Muon0, Muon1);
    else continue;
    // find the other pair with another two muons
    for(int jp=ip+1; jp<(int)good_pairs.size(); jp++) {
      auto jpair=good_pairs.at(jp);
      const xAOD::Muon * Muon2 = NULL;
      const xAOD::Muon * Muon3 = NULL;
      if(matchFourMuon(jpair, good_mus)) getConstiMuons(jpair, Muon2, Muon3);
      else continue;
      // make sure the two muons in this pair are not the same as the first one
      if(Muon2==Muon0 || Muon2==Muon1) continue;
      if(Muon3==Muon0 || Muon3==Muon1) continue;
      // two distinct pairs found
      onia_pairs.push_back(std::make_pair(ipair, jpair));
    }
  }

  return onia_pairs;

}


// check if the consistituent tracks of an Onia pass kinematic cuts
bool XFourMuonAnalysis :: passPairTrkPt(const xAOD::Vertex* pair, float ptCut, float etaCut) {

  bool pass=true;

  const MuonLinkVector MuonLinks = pair->auxdataConst<MuonLinkVector>("MuonLinks");
  const MuonLink& MuonLink0 = MuonLinks[0];
  const MuonLink& MuonLink1 = MuonLinks[1];

  const xAOD::Muon * Muon0 = NULL;
  const xAOD::Muon * Muon1 = NULL;
  if(MuonLink0.isValid()) Muon0 = *MuonLink0;
  if(MuonLink1.isValid()) Muon1 = *MuonLink1;
  if(Muon0 == NULL || Muon1 == NULL) return false;

  const xAOD::TrackParticle * Track0 = NULL;
  const xAOD::TrackParticle * Track1 = NULL;

  // avoid standalone muon
  if(Muon0->muonType()==1 || Muon1->muonType()==1) return false;

  //==== tracks
  const TrkLink& Link0 = Muon0->inDetTrackParticleLink();
  const TrkLink& Link1 = Muon1->inDetTrackParticleLink();

  if(Link0.isValid()) Track0 = *Link0;
  if(Link1.isValid()) Track1 = *Link1;

  if(Track0 == NULL || Track1 == NULL) return false;

  if(Track0->pt()<ptCut || Track1->pt()<ptCut) pass=false;
  if(fabs(Track0->eta())>etaCut || fabs(Track1->eta())>etaCut) pass=false;
  
  return pass;
}

// get constituent muons (pair only)
void XFourMuonAnalysis :: getConstiMuons(const xAOD::Vertex* pair, const xAOD::Muon*& mu0, const xAOD::Muon*& mu1) {

  const MuonLinkVector MuonLinks = pair->auxdataConst<MuonLinkVector>("MuonLinks");
  // pair
  if(MuonLinks.size()==2) {
    const MuonLink& MuonLink0 = MuonLinks[0];
    const MuonLink& MuonLink1 = MuonLinks[1];
    const xAOD::Muon * Muon0 = NULL;
    const xAOD::Muon * Muon1 = NULL;
    if(MuonLink0.isValid()) Muon0 = *MuonLink0;
    if(MuonLink1.isValid()) Muon1 = *MuonLink1;
    
    mu0 = Muon0; mu1 = Muon1;
  }
}

// get constituent muons
void XFourMuonAnalysis :: getConstiMuons(const xAOD::Vertex* quad, const xAOD::Muon*& mu0, const xAOD::Muon*& mu1, const xAOD::Muon*& mu2, const xAOD::Muon*& mu3) {

  const MuonLinkVector MuonLinks = quad->auxdataConst<MuonLinkVector>("MuonLinks");
  // pair
  if(MuonLinks.size()==2) {
    const MuonLink& MuonLink0 = MuonLinks[0];
    const MuonLink& MuonLink1 = MuonLinks[1];
    const xAOD::Muon * Muon0 = NULL;
    const xAOD::Muon * Muon1 = NULL;
    if(MuonLink0.isValid()) Muon0 = *MuonLink0;
    if(MuonLink1.isValid()) Muon1 = *MuonLink1;
    
    mu0 = Muon0; mu1 = Muon1;
    mu2 = NULL; mu3 = NULL;
  }
  // quad
  if(MuonLinks.size()==4) {
    const MuonLink& MuonLink0 = MuonLinks[0];
    const MuonLink& MuonLink1 = MuonLinks[1];
    const MuonLink& MuonLink2 = MuonLinks[2];
    const MuonLink& MuonLink3 = MuonLinks[3];

    const xAOD::Muon * Muon0 = NULL;
    const xAOD::Muon * Muon1 = NULL;
    const xAOD::Muon * Muon2 = NULL;
    const xAOD::Muon * Muon3 = NULL;
    if(MuonLink0.isValid()) Muon0 = *MuonLink0;
    if(MuonLink1.isValid()) Muon1 = *MuonLink1;
    if(MuonLink2.isValid()) Muon2 = *MuonLink2;
    if(MuonLink3.isValid()) Muon3 = *MuonLink3;

    mu0 = Muon0; mu1 = Muon1;
    mu2 = Muon2; mu3 = Muon3;
  }
}

// match vertex combinatin code (regardless of order)
bool XFourMuonAnalysis :: matchCC(const xAOD::Vertex* pair, std::string cc) {

  std::string v_cc = getCC(pair);
  // sort string
  std::sort(v_cc.begin(), v_cc.end());
  std::sort(cc.begin(), cc.end());

  return v_cc.compare(cc)==0; 
}

// match Quad with four good muons
bool XFourMuonAnalysis :: matchFourMuon(const xAOD::Vertex* quad, std::vector<const xAOD::Muon*> good_mus) {
  bool matched=false;

  std::vector<const xAOD::Muon*> mus;
  const xAOD::Muon * Muon0 = NULL;
  const xAOD::Muon * Muon1 = NULL;
  const xAOD::Muon * Muon2 = NULL;
  const xAOD::Muon * Muon3 = NULL;

  //==== muons
  const MuonLinkVector MuonLinks = quad->auxdataConst<MuonLinkVector>("MuonLinks");
  // pair
  if(MuonLinks.size()==2) {
    const MuonLink& MuonLink0 = MuonLinks[0];
    const MuonLink& MuonLink1 = MuonLinks[1];

    if(MuonLink0.isValid()) Muon0 = *MuonLink0;
    if(MuonLink1.isValid()) Muon1 = *MuonLink1;
  }
  // quad
  else if(MuonLinks.size()==4) {
    const MuonLink& MuonLink0 = MuonLinks[0];
    const MuonLink& MuonLink1 = MuonLinks[1];
    const MuonLink& MuonLink2 = MuonLinks[2];
    const MuonLink& MuonLink3 = MuonLinks[3];

    if(MuonLink0.isValid()) Muon0 = *MuonLink0;
    if(MuonLink1.isValid()) Muon1 = *MuonLink1;
    if(MuonLink2.isValid()) Muon2 = *MuonLink2;
    if(MuonLink3.isValid()) Muon3 = *MuonLink3;
  }

  if(Muon0) mus.push_back(Muon0);
  if(Muon1) mus.push_back(Muon1);
  if(Muon2) mus.push_back(Muon2);
  if(Muon3) mus.push_back(Muon3);
  if(MuonLinks.size() != mus.size()) return false;

  // sort by muon pt 
  std::sort(good_mus.begin(), good_mus.end(), [] (const xAOD::Muon* a, const xAOD::Muon* b)
     -> bool { return (a->pt() > b->pt());}
  );
  std::sort(mus.begin(), mus.end(), [] (const xAOD::Muon* a, const xAOD::Muon* b)
     -> bool { return (a->pt() > b->pt());}
  );

  // quad
  if(mus.size() == good_mus.size()) {
    if(mus == good_mus) matched=true;
  }
  else { // pair
    matched=true;
    // loop muons in pair to see if both of them can be found in good_mus
    for(auto mu: mus) {
      bool mu_matched=false;
      for(auto gmu: good_mus) {
        if(mu==gmu) {mu_matched=true; break;}
      }
      matched &= mu_matched;
      if(!matched) break;
    }
  }

  return matched;
}

// get trk/muon index
std::vector<int> XFourMuonAnalysis :: getTrkInd(const xAOD::Vertex* pair) {

  std::string cc="";
  cc=pair->auxdataConst<std::string>("CombinationCode");

  std::vector<int> trkIndices;
  for(int i=0; i<(int)cc.length(); i++) {
    trkIndices.push_back(int(cc[i]-'0'));
  }

  return trkIndices;
}

// get trk/muon index
std::vector<int> XFourMuonAnalysis :: getTrkInd(const xAOD::Vertex* pair, std::vector<const xAOD::Muon*> good_mus) {

  std::vector<int> trkIndices;

  // get constituent muons
  const xAOD::Muon * Muon0 = NULL;
  const xAOD::Muon * Muon1 = NULL;
  const xAOD::Muon * Muon2 = NULL;
  const xAOD::Muon * Muon3 = NULL;
  getConstiMuons(pair, Muon0, Muon1, Muon2, Muon3);

  std::vector<const xAOD::Muon *> pair_mus;
  if(Muon0) pair_mus.push_back(Muon0);
  if(Muon1) pair_mus.push_back(Muon1);
  if(Muon2) pair_mus.push_back(Muon2);
  if(Muon3) pair_mus.push_back(Muon3);

  // loop all constituent muons
  for(int im=0; im<(int)pair_mus.size(); im++) {
    auto imu=pair_mus.at(im);
    // match with the ons from good_mus vector
    for(int jm=0; jm<(int)good_mus.size(); jm++) {
      auto jmu=good_mus.at(jm);
      if(imu==jmu) {// matched
        trkIndices.push_back(jm); break;
      }
    }
  }

  return trkIndices;
}


// only need to know relative index of the four muons
std::vector<int> XFourMuonAnalysis :: getTrkInd(const xAOD::Vertex* pair, const xAOD::Vertex* quad) {

  std::string cc_p="", cc_q="";
  cc_p=pair->auxdataConst<std::string>("CombinationCode");
  cc_q=quad->auxdataConst<std::string>("CombinationCode");

  std::vector<int> trkIndices;
  for(int i=0; i<(int)cc_p.length(); i++) {
    for(int j=0; j<(int)cc_q.length(); j++) {
      if(cc_q[j]==cc_p[i]) {
        trkIndices.push_back(j); break;
      }
    }
  }

  return trkIndices;
}

// get the Quad Combination code from Onia pairs
std::string XFourMuonAnalysis :: getCC_OniaPair(OniaPairVec onia_pairs) {
  // for a Quad (vertex), the combination code is just the index of the constitiuent muons
  // the index might be the orignal index of the muons in the muon container
  // but the indexing is consistent in Quad and Pair vertices
  // thus we can use the Pair combination code to get the Quad CC

  // only need to look at the first onia pairs
  std::string cc_q="";
  for(auto onia_pair: onia_pairs) {
    auto o1=onia_pair.first;
    auto o2=onia_pair.second;

    std::string cc_1=getCC(o1);
    std::string cc_2=getCC(o2);

    cc_q = cc_1 + cc_2;
    std::sort(cc_q.begin(), cc_q.end());

    break;
  } 

  return cc_q;
}

// get the Quad Charge code from Onia pairs
std::string XFourMuonAnalysis :: getQC_OniaPair(OniaPairVec onia_pairs) {

  // similar to getCC_OniaPair
  // firstly, find the combination code, then get the corresponding charge code
  
  std::string cc_q="", qc_q="";
  for(auto onia_pair: onia_pairs) {
    auto o1=onia_pair.first;
    auto o2=onia_pair.second;

    std::string cc_1=getCC(o1);
    std::string cc_2=getCC(o2);

    cc_q = cc_1 + cc_2;

    std::string qc_1=getQC(o1);
    std::string qc_2=getQC(o2);

    qc_q = qc_1 + qc_2;

    break;
  } 

  std::string qc_q_0="";
  std::string cc_q_0=getCC_OniaPair(onia_pairs); // CC, sorted already

  // cc_q is the Combination code of the Quad, and qc_q is the charge code; their indexing is consistent
  // cc_q_0 is the sorted combination code, need to get the corresponding charge code
   
  for(int i=0; i<(int)cc_q_0.length(); i++) {
    for(int j=0; j<(int)cc_q.length(); j++) {
      if(cc_q_0[i]==cc_q[j]) {// index matched 
        qc_q_0.push_back(qc_q[j]); 
        break;
      }
    }
  }

  return qc_q_0;
}
