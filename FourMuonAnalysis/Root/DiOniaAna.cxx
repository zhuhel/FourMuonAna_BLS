#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"
#include <AsgTools/MsgStream.h>
#include <AsgTools/MsgStreamMacros.h>
#include "xAODCore/AuxInfoBase.h"
#include "xAODCore/ShallowAuxInfo.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODTracking/TrackParticlexAODHelpers.h"
#include "MuonMomentumCorrections/MuonCalibrationAndSmearingTool.h"
#include "MuonSelectorTools/MuonSelectionTool.h"
#include <FourMuonAnalysis/XFourMuonAnalysis.h>
#include <FourMuonAnalysis/Utils.h>

float XFourMuonAnalysis :: alphaCal(TVector3 v_plus, TVector3 v_minus){

  float P1 = v_plus.Mag();
  float P2 = v_minus.Mag();
  float cosalpha1 = v_plus.Dot((v_plus+v_minus))/(v_plus.Mag()*(v_plus+v_minus).Mag());
  float cosalpha2 = v_minus.Dot((v_plus+v_minus))/(v_minus.Mag()*(v_plus+v_minus).Mag()); 
  float pL1 = v_plus.Mag()*cosalpha1; 
  float pL2 = v_minus.Mag()*cosalpha2;
  float Alpha = (pL1-pL2)/(pL1+pL2);
  //float pT_rel = v_plus.Mag()*sqrt(1.-cosalpha1*cosalpha1);

  return Alpha;

} // alphaCal

EL::StatusCode XFourMuonAnalysis :: doDiOniaSel () {

  // DiOnia selection
  //---------------------------------
  std::vector<const xAOD::Vertex*> good_quads_os, cand_quads_os, cand_quads_os_chi2;
  DiOnia good_dionia_os, cand_dionia_os_chi2;
  // reset !!
  m_onia_pairs.clear();
  m_cand_quad=0;

  bool doOSonly=false; // temp flag

  xAOD::VertexContainer::const_iterator quadItr = m_quads->begin();
  for (; quadItr!=m_quads->end(); ++quadItr) {
    auto quad = *quadItr;

    if(m_msgLevel==MSG::DEBUG)
      ATH_MSG_DEBUG("\t" << (*quadItr)->auxdataConst<std::string>("CombinationCode") <<
                 "\t" << (*quadItr)->auxdataConst<std::string>("ChargeCode") <<
                 "\t" << (*quadItr)->auxdataConst<float>("QUAD_mass") << " +/- " << (*quadItr)->auxdataConst<float>("QUAD_massErr") <<
                 "\t" << (*quadItr)->chiSquared() << "\t" << (*quadItr)->auxdataConst<double>("ACminusDC") <<
                 "\t" << (*quadItr)->auxdataConst<double>("SSminusDC"));
    if ((*quadItr)->auxdataConst<MuonLinkVector>("MuonLinks").size()!=4) {
      ATH_MSG_ERROR("doDiOniaSel() Quadruplet without 2 muon links! Something is wrong!");
      return StatusCode::FAILURE;
    }

    HM->ClearCutflowFlag("DiOnia");
    HM->Cutflow_flag_tmp["DiOnia"]["AllDiOnia"]=1;
    std::string quadCombinationCode=quad->auxdataConst<std::string>("CombinationCode");
    //== should match four good muons
    if(matchFourMuon(quad, m_good_qmus)) {
      HM->Cutflow_flag_tmp["DiOnia"]["MatchQMu"]=1;

      //== should match two good pairs
      if( !doOSonly || getCharge( quad ) == 0 ) {
        HM->Cutflow_flag_tmp["DiOnia"]["DiOniaQ"]=1;
        OniaPairVec onia_pairs; // <o1, o2>, <o3, o4>, ...
        onia_pairs = matchOnia(quadCombinationCode, m_good_pairs);
        if(onia_pairs.size()>0) {
          HM->Cutflow_flag_tmp["DiOnia"]["OniaMatching"]=1;
          good_quads_os.push_back(quad);
          good_dionia_os[quad]=onia_pairs;  // map: quad -- < <o1, o2> , <o3, o4>, ... >
          ATH_MSG_DEBUG("doDiOniaSel() Matched quad with two pairs: quad= " << quadCombinationCode << " pairs: ");
          for(auto onia_pair: onia_pairs) {
            ATH_MSG_DEBUG("        pairs1: " << onia_pair.first->auxdataConst<std::string>("CombinationCode") << " " << onia_pair.second->auxdataConst<std::string>("CombinationCode"));
          }
        }
      }
    }

    HM->FillCutflow("DiOnia");
  }
 
  if(m_msgLevel==MSG::DEBUG) ATH_MSG_INFO("doDiOniaSel() Number of good quads = " << good_quads_os.size()  );
  HM->Var["nQuadsOS"]=good_quads_os.size();

  //== make some plots
  for(auto quad: good_quads_os) {
    HM->ClearCutflowFlag("DiOnia");

    bool pass_two_o=false; // two good Onias: charge 0
    bool pass_two_o_chi2=false; // two good Onias: charge 0, chi2 < 3
    bool pass_cb=false;    // >=3 combined muon
    bool pass_leppt=false;   // pt cut: o1 (4, 4), o2 (3, 3)
    bool pass_trkkine=false; // track pt > 2.5, eta < 2.5
    bool pass_qcheck=false;  // muon charge consistent with track charge (TODO)

    double mass = quad->auxdataConst<float>("QUAD_mass");
    double chi2 = quad->auxdataConst<float>("chiSquared");

    HM->VVar["MassQ_DiOnia_PreSel"]->push_back(mass*1e-3);
    HM->VVar["Chi2Q_DiOnia_PreSel"]->push_back(log10(chi2));
  
    //== onia kinematics
    const xAOD::Vertex *o1=0, *o2=0;
    OniaPairVec onia_pairs = good_dionia_os[quad];
    // only look at unique pairs 
    std::vector<const xAOD::Vertex*> dionia_pairs, dionia_pairs_os;
    for(auto onia_pair: onia_pairs) {
      bool pass_tmp_q_o1 = false, pass_tmp_q_o2 = false, pass_tmp_chi2_o1 = false, pass_tmp_chi2_o2 = false;

      pass_tmp_q_o1 = getCharge(onia_pair.first) == 0;
      pass_tmp_q_o2 = getCharge(onia_pair.second) == 0;
      // two neutral onia
      if(pass_tmp_q_o1 && pass_tmp_q_o2) pass_two_o |= true;

      double chi2_o1 = getChi2(onia_pair.first); 
      double chi2_o2 = getChi2(onia_pair.second); 

      pass_tmp_chi2_o1 = chi2_o1 < 3;
      pass_tmp_chi2_o2 = chi2_o2 < 3;
      //if(pass_tmp_chi2_o1 && pass_tmp_chi2_o2 && pass_tmp_q_o1 && pass_tmp_q_o2) {o1=onia_pair.first; o2=onia_pair.second;}
      // no charge cut for now
      if(pass_tmp_chi2_o1 && pass_tmp_chi2_o2) {o1=onia_pair.first; o2=onia_pair.second;}

      if(std::find(dionia_pairs.begin(), dionia_pairs.end(), onia_pair.first) == dionia_pairs.end()) {
        dionia_pairs.push_back(onia_pair.first);
        if(pass_tmp_q_o1) dionia_pairs_os.push_back(onia_pair.first);
      }
      if(std::find(dionia_pairs.begin(), dionia_pairs.end(), onia_pair.second) == dionia_pairs.end()) {
        dionia_pairs.push_back(onia_pair.second);
        if(pass_tmp_q_o2) dionia_pairs_os.push_back(onia_pair.second);
      }
      // plot onia mass: charge 0 and chi2 < 3
      if(o1 && o2) {
        pass_two_o_chi2 |= true;
        double mass = o1->auxdataConst<float>("PAIR_mass");
        double chi2 = o1->auxdataConst<float>("chiSquared");
    
        HM->VVar["MassO1_DiOnia_PreSel"]->push_back(mass*1e-3);
        HM->VVar["Chi2O1_DiOnia_PreSel"]->push_back(log10(chi2));
    
        mass = o2->auxdataConst<float>("PAIR_mass");
        chi2 = o2->auxdataConst<float>("chiSquared");
    
        HM->VVar["MassO2_DiOnia_PreSel"]->push_back(mass*1e-3);
        HM->VVar["Chi2O2_DiOnia_PreSel"]->push_back(log10(chi2));
      }
    }   
    // plot all onias 
    for(auto pair: dionia_pairs) {
      if(m_debug) std::cout << " onia mass/chi2: " << getMass(pair) << " " << getChi2(pair) << " " << getCC(pair) << std::endl;

      double mass = pair->auxdataConst<float>("PAIR_mass");
      double massErr = pair->auxdataConst<float>("PAIR_massErr");
      double chi2 = pair->auxdataConst<float>("chiSquared");

      HM->VVar["MassO_DiOnia_PreSel"]->push_back(mass*1e-3);
      HM->VVar["MassErrO_DiOnia_PreSel"]->push_back(massErr*1e-3);
      HM->VVar["Chi2O_DiOnia_PreSel"]->push_back(log10(chi2));
    }

    //==== muon kinematics
    const MuonLinkVector MuonLinks = quad->auxdataConst<MuonLinkVector>("MuonLinks");
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
    if(Muon0 == NULL || Muon1 == NULL || Muon2 == NULL || Muon3 == NULL) {
      HM->FillCutflow("DiOnia");
      continue;
    }
    HM->Cutflow_flag_tmp["DiOnia"]["MuonMatching"]=1;
    // already passing all previous selections
    HM->Cutflow_flag["DiOnia"]["MuonMatching"]=1;

    //== muon cuts
    int nCb=0;
    if(Muon0->muonType() == xAOD::Muon::Combined) nCb ++;
    if(Muon1->muonType() == xAOD::Muon::Combined) nCb ++;
    if(Muon2->muonType() == xAOD::Muon::Combined) nCb ++;
    if(Muon3->muonType() == xAOD::Muon::Combined) nCb ++;
    if(nCb>=3) pass_cb=true;

    const xAOD::TrackParticle * Track0 = NULL;
    const xAOD::TrackParticle * Track1 = NULL;
    const xAOD::TrackParticle * Track2 = NULL;
    const xAOD::TrackParticle * Track3 = NULL;

    //==== tracks
    const TrkLink& Link0 = Muon0->inDetTrackParticleLink();
    const TrkLink& Link1 = Muon1->inDetTrackParticleLink();
    const TrkLink& Link2 = Muon2->inDetTrackParticleLink();
    const TrkLink& Link3 = Muon3->inDetTrackParticleLink();

    if(Link0.isValid()) Track0 = *Link0;
    if(Link1.isValid()) Track1 = *Link1;
    if(Link2.isValid()) Track2 = *Link2;
    if(Link3.isValid()) Track3 = *Link3;

    if(Track0 == NULL || Track1 == NULL || Track2 == NULL || Track3 == NULL) {
      Warning("XFourMuonAnalysis::doDiOniaSel()", "Missing Track Pointers for QUAD!");
      HM->FillCutflow("DiOnia");
      continue;
    }
    HM->Cutflow_flag_tmp["DiOnia"]["TrkMatching"]=1;

    //== track kinematics
    pass_trkkine=true;
    if(Track0->pt()<2.5e3 || Track1->pt()<2.5e3 || Track2->pt()<2.5e3 || Track3->pt()<2.5e3) pass_trkkine=false;
    if(fabs(Track0->eta())>2.5 || fabs(Track1->eta())>2.5 || fabs(Track2->eta())>2.5 || fabs(Track3->eta())>2.5)
      pass_trkkine=false;
    if(o1 && o2) {
      //if( (passPairTrkPt(o1, 4e3) && passPairTrkPt(o2, 3e3)) ||
          //(passPairTrkPt(o1, 3e3) && passPairTrkPt(o2, 4e3))) pass_leppt=true;
      if(passPairTrkPt(o1, 3e3) && passPairTrkPt(o2, 3e3)) pass_leppt=true;
      //if(passPairTrkPt(o1, 2.5e3) && passPairTrkPt(o2, 2.5e3)) pass_leppt=true;
    }
    pass_qcheck=true; // TODO: no cut on this for now

    // Helper Tool
    xAOD::BPhysHelper bVtx(quad);
    TLorentzVector Muon0_RefTrack = bVtx.refTrk(0, M_MU);
    TLorentzVector Muon1_RefTrack = bVtx.refTrk(1, M_MU);
    TLorentzVector Muon2_RefTrack = bVtx.refTrk(2, M_MU);
    TLorentzVector Muon3_RefTrack = bVtx.refTrk(3, M_MU);

    TLorentzVector RefDiMuon1 = Muon0_RefTrack + Muon1_RefTrack;
    TLorentzVector RefDiMuon2 = Muon2_RefTrack + Muon3_RefTrack;
    TLorentzVector RefQuad = RefDiMuon1 + RefDiMuon2;

    HM->VVar["MassO1RefTrk_DiOnia_PreSel"]->push_back(RefDiMuon1.M()*1e-3);
    HM->VVar["PtO1RefTrk_DiOnia_PreSel"]->push_back(RefDiMuon1.Pt()*1e-3);
    HM->VVar["YO1RefTrk_DiOnia_PreSel"]->push_back(RefDiMuon1.Rapidity());
    HM->VVar["MassO2RefTrk_DiOnia_PreSel"]->push_back(RefDiMuon2.M()*1e-3);
    HM->VVar["PtO2RefTrk_DiOnia_PreSel"]->push_back(RefDiMuon2.Pt()*1e-3);
    HM->VVar["YO2RefTrk_DiOnia_PreSel"]->push_back(RefDiMuon2.Rapidity());

    HM->VVar["MassQRefTrk_DiOnia_PreSel"]->push_back(RefQuad.M()*1e-3);
    HM->VVar["PtQRefTrk_DiOnia_PreSel"]->push_back(RefQuad.Pt()*1e-3);
    HM->VVar["YQRefTrk_DiOnia_PreSel"]->push_back(RefQuad.Rapidity());

    //== selections
    if(pass_two_o_chi2) HM->Cutflow_flag_tmp["DiOnia"]["OniaChi2"]=1; 
    if(pass_trkkine) HM->Cutflow_flag_tmp["DiOnia"]["TrkKine"]=1;
    if(pass_cb)      HM->Cutflow_flag_tmp["DiOnia"]["CB"]=1;
    if(pass_leppt)   HM->Cutflow_flag_tmp["DiOnia"]["Pt"]=1;
    if(pass_qcheck)  HM->Cutflow_flag_tmp["DiOnia"]["QCheck"]=1;

    if(pass_trkkine && pass_cb && pass_leppt && pass_qcheck) {
      cand_quads_os.push_back(quad);
    }
    if(pass_two_o_chi2 && pass_trkkine && pass_cb && pass_leppt && pass_qcheck) {
      cand_quads_os_chi2.push_back(quad);
      cand_dionia_os_chi2[quad]=good_dionia_os[quad];
    }

    HM->FillCutflow("DiOnia");
  }

  if(m_msgLevel==MSG::DEBUG) ATH_MSG_INFO("doDiOniaSel() Number of good canddiate quads = " << cand_quads_os_chi2.size()  );
  HM->Var["nQuadCandsOS"]=cand_quads_os_chi2.size();

  //== select Quad with smallest chi2
  if(cand_quads_os_chi2.size()>0) {
    double min_quad_chi2=1e9;
    for(auto quad: cand_quads_os_chi2) {
      double tmp_chi2 = getChi2(quad);
      if(tmp_chi2 < min_quad_chi2) { min_quad_chi2=tmp_chi2; m_cand_quad=quad;}
    }
  }
  if(m_cand_quad) {
    double mass_quad = getMass(m_cand_quad, 1);
    double chi2_quad = getChi2(m_cand_quad);

    HM->VVar["MassQ"]->push_back(mass_quad*1e-3);
    HM->VVar["Chi2Q"]->push_back(log10(chi2_quad));
    HM->VVar["ChargeQ"]->push_back(getCharge(m_cand_quad));
  }

  // Event selection
  //---------------------------------
  if(m_useQuad==1) {  // use Quad vertex fit
    //== at least one good Quad
    if(cand_quads_os.size()>0) HM->Cutflow_flag_tmp["Event"]["DiOnia"]=1;
    else 
      return EL::StatusCode::SUCCESS;

    //== onia pairs matched to this quad
    m_onia_pairs = cand_dionia_os_chi2[m_cand_quad];
    if(!m_cand_quad) ATH_MSG_ERROR("doDiOniaSel() Could not find candidate quad");
    HM->Cutflow_flag_tmp["Event"]["Cand"]=1;
   
    // OS, SS
    if(getCharge( m_cand_quad ) == 0) HM->Cutflow_flag_tmp["EventOS"]["Cand"]=1;
    else if(fabs(getCharge( m_cand_quad )) == 2) HM->Cutflow_flag_tmp["EventSS"]["Cand"]=1;

    // should pass all previous common selections
    HM->FillCutflowFlag("Event");
    HM->Cutflow_flag_tmp["EventOS"]["Cand"] &= HM->Cutflow_flag["Event"]["Cand"];
    HM->Cutflow_flag_tmp["EventSS"]["Cand"] &= HM->Cutflow_flag["Event"]["Cand"];

  } else {  // use four good muons directly, not based on Quad vertex fit
    //== match pairs to the four good muons
    if((int)m_good_qmus.size()<4) return EL::StatusCode::SUCCESS;
    m_onia_pairs = matchOnia(m_good_qmus, m_good_pairs);
    
    //== Onia pair selection: pt>3, chi2<3
    bool pass_two_o=false;
    bool pass_two_o_chi2=false;
    const xAOD::Vertex *o1=0, *o2=0;
    for(auto onia_pair: m_onia_pairs) {
      bool pass_tmp_chi2_o1 = false, pass_tmp_chi2_o2 = false, pass_tmp_leppt = false;

      o1=onia_pair.first; o2=onia_pair.second;
      double chi2_o1 = getChi2(o1); 
      double chi2_o2 = getChi2(o2);

      pass_tmp_chi2_o1 = chi2_o1 < 3;
      pass_tmp_chi2_o2 = chi2_o2 < 3;
      if(passPairTrkPt(o1, 3e3) && passPairTrkPt(o2, 3e3)) pass_tmp_leppt=true;
      //if(passPairTrkPt(o1, 2.5e3) && passPairTrkPt(o2, 2.5e3)) pass_tmp_leppt=true;

      if(getCharge(o1)==0 && getCharge(o2)==0) pass_two_o |= pass_tmp_leppt;

      // !!! should not require OS, in order to look at SS
      pass_two_o_chi2 |= pass_tmp_chi2_o1 && pass_tmp_chi2_o2 && pass_tmp_leppt;
    }

    // total charge of the four muons
    float charge_4mu=0;
    for(auto mu: m_good_qmus) charge_4mu += mu->charge();

    // two Onia
    if(m_onia_pairs.size()>0) {
      HM->Cutflow_flag_tmp["Event"]["DiOnia"]=1;
      // OS, SS
      if(charge_4mu == 0) HM->Cutflow_flag_tmp["EventOS"]["DiOnia"]=1;
      else if(fabs(charge_4mu) == 2) HM->Cutflow_flag_tmp["EventSS"]["DiOnia"]=1;
    } 

    // two Onia with chi2<3
    if(pass_two_o_chi2) {
      HM->Cutflow_flag_tmp["Event"]["Cand"]=1;
      // OS, SS
      if(charge_4mu == 0) HM->Cutflow_flag_tmp["EventOS"]["Cand"]=1;
      else if(fabs(charge_4mu) == 2) HM->Cutflow_flag_tmp["EventSS"]["Cand"]=1;
    }

    // should pass all previous common selections
    HM->FillCutflowFlag("Event");
    HM->Cutflow_flag_tmp["EventOS"]["DiOnia"] &= HM->Cutflow_flag["Event"]["DiOnia"];
    HM->Cutflow_flag_tmp["EventSS"]["DiOnia"] &= HM->Cutflow_flag["Event"]["DiOnia"];

  }

  // get constituent muons
  const xAOD::Muon * Muon0 = NULL;
  const xAOD::Muon * Muon1 = NULL;
  const xAOD::Muon * Muon2 = NULL;
  const xAOD::Muon * Muon3 = NULL;
  if(m_useQuad) getConstiMuons(m_cand_quad, Muon0, Muon1, Muon2, Muon3);
  else {
    if((int)m_good_qmus.size()>=4) {
      Muon0 = m_good_qmus.at(0); Muon1 = m_good_qmus.at(1); 
      Muon2 = m_good_qmus.at(2); Muon3 = m_good_qmus.at(3); 
    }
  }

  TLorentzVector DiMuon1 = Muon0->p4() + Muon1->p4();
  TLorentzVector DiMuon2 = Muon2->p4() + Muon3->p4();
  TLorentzVector MuQuad = DiMuon1 + DiMuon2;

  HM->VVar["MuPtQ"]->push_back(MuQuad.Pt()*1e-3);
  HM->VVar["MuMQ"]->push_back(MuQuad.M()*1e-3);
  
  HM->VVar["IsCB"]->push_back(Muon0->muonType()==0);
  HM->VVar["MuPt"]->push_back(Muon0->pt()*1e-3);
  HM->VVar["MuEta"]->push_back(Muon0->eta());
  HM->VVar["MuPhi"]->push_back(Muon0->phi());
  HM->VVar["MuType"]->push_back(Muon0->muonType());
  HM->VVar["MuAuth"]->push_back(Muon0->author());

  HM->VVar["IsCB"]->push_back(Muon1->muonType()==0);
  HM->VVar["MuPt"]->push_back(Muon1->pt()*1e-3);
  HM->VVar["MuEta"]->push_back(Muon1->eta());
  HM->VVar["MuPhi"]->push_back(Muon1->phi());
  HM->VVar["MuType"]->push_back(Muon1->muonType());
  HM->VVar["MuAuth"]->push_back(Muon1->author());

  HM->VVar["IsCB"]->push_back(Muon2->muonType()==0);
  HM->VVar["MuPt"]->push_back(Muon2->pt()*1e-3);
  HM->VVar["MuEta"]->push_back(Muon2->eta());
  HM->VVar["MuPhi"]->push_back(Muon2->phi());
  HM->VVar["MuType"]->push_back(Muon2->muonType());
  HM->VVar["MuAuth"]->push_back(Muon2->author());

  HM->VVar["IsCB"]->push_back(Muon3->muonType()==0);
  HM->VVar["MuPt"]->push_back(Muon3->pt()*1e-3);
  HM->VVar["MuEta"]->push_back(Muon3->eta());
  HM->VVar["MuPhi"]->push_back(Muon3->phi());
  HM->VVar["MuType"]->push_back(Muon3->muonType());
  HM->VVar["MuAuth"]->push_back(Muon3->author());

  HM->VVar["L1Pt"]->push_back(Muon0->pt()*1e-3);
  HM->VVar["L1Eta"]->push_back(Muon0->eta());
  HM->VVar["L1Phi"]->push_back(Muon0->phi());

  HM->VVar["L2Pt"]->push_back(Muon1->pt()*1e-3);
  HM->VVar["L2Eta"]->push_back(Muon1->eta());
  HM->VVar["L2Phi"]->push_back(Muon1->phi());

  HM->VVar["L3Pt"]->push_back(Muon2->pt()*1e-3);
  HM->VVar["L3Eta"]->push_back(Muon2->eta());
  HM->VVar["L3Phi"]->push_back(Muon2->phi());

  HM->VVar["L4Pt"]->push_back(Muon3->pt()*1e-3);
  HM->VVar["L4Eta"]->push_back(Muon3->eta());
  HM->VVar["L4Phi"]->push_back(Muon3->phi());

  // get constituent tracks
  const xAOD::TrackParticle * Track0 = NULL;
  const xAOD::TrackParticle * Track1 = NULL;
  const xAOD::TrackParticle * Track2 = NULL;
  const xAOD::TrackParticle * Track3 = NULL;

  const TrkLink& Link0 = Muon0->inDetTrackParticleLink();
  const TrkLink& Link1 = Muon1->inDetTrackParticleLink();
  const TrkLink& Link2 = Muon2->inDetTrackParticleLink();
  const TrkLink& Link3 = Muon3->inDetTrackParticleLink();

  if(Link0.isValid()) Track0 = *Link0;
  if(Link1.isValid()) Track1 = *Link1;
  if(Link2.isValid()) Track2 = *Link2;
  if(Link3.isValid()) Track3 = *Link3;

  float z0SinTheta1=-999, z0SinTheta2=-999, z0SinTheta3=-999, z0SinTheta0=-999;
  float d0Sig1=-999, d0Sig2=-999, d0Sig3=-999, d0Sig0=-999;

  z0SinTheta0 = (Track0->z0()+Track0->vz()-m_pv->z())*sin(Track0->p4().Theta());
  d0Sig0 = xAOD::TrackingHelpers::d0significance(Track0, m_eventInfo->beamPosSigmaX(), m_eventInfo->beamPosSigmaY(), m_eventInfo->beamPosSigmaXY() );
  z0SinTheta1 = (Track1->z0()+Track1->vz()-m_pv->z())*sin(Track1->p4().Theta());
  d0Sig1 = xAOD::TrackingHelpers::d0significance(Track1, m_eventInfo->beamPosSigmaX(), m_eventInfo->beamPosSigmaY(), m_eventInfo->beamPosSigmaXY() );
  z0SinTheta2 = (Track2->z0()+Track2->vz()-m_pv->z())*sin(Track2->p4().Theta());
  d0Sig2 = xAOD::TrackingHelpers::d0significance(Track2, m_eventInfo->beamPosSigmaX(), m_eventInfo->beamPosSigmaY(), m_eventInfo->beamPosSigmaXY() );
  z0SinTheta3 = (Track3->z0()+Track3->vz()-m_pv->z())*sin(Track3->p4().Theta());
  d0Sig3 = xAOD::TrackingHelpers::d0significance(Track3, m_eventInfo->beamPosSigmaX(), m_eventInfo->beamPosSigmaY(), m_eventInfo->beamPosSigmaXY() );

  TLorentzVector TrkDiMuon1,TrkDiMuon2,TrkQuad;
  TLorentzVector Muon0_RefTrackOrig, Muon1_RefTrackOrig, Muon2_RefTrackOrig, Muon3_RefTrackOrig;
  // Helper Tool
  if(m_cand_quad) { 
    xAOD::BPhysHelper bVtx(m_cand_quad);
    Muon0_RefTrackOrig = bVtx.refTrkOriginP(0, M_MU);
    Muon1_RefTrackOrig = bVtx.refTrkOriginP(1, M_MU);
    Muon2_RefTrackOrig = bVtx.refTrkOriginP(2, M_MU);
    Muon3_RefTrackOrig = bVtx.refTrkOriginP(3, M_MU);
  } else {
    Muon0_RefTrackOrig.SetPtEtaPhiM(Track0->pt(), Track0->eta(), Track0->phi(), M_MU); 
    Muon1_RefTrackOrig.SetPtEtaPhiM(Track1->pt(), Track1->eta(), Track1->phi(), M_MU); 
    Muon2_RefTrackOrig.SetPtEtaPhiM(Track2->pt(), Track2->eta(), Track2->phi(), M_MU); 
    Muon3_RefTrackOrig.SetPtEtaPhiM(Track3->pt(), Track3->eta(), Track3->phi(), M_MU); 
  }

  TrkDiMuon1 = Muon0_RefTrackOrig + Muon1_RefTrackOrig;
  TrkDiMuon2 = Muon2_RefTrackOrig + Muon3_RefTrackOrig;
  TrkQuad = TrkDiMuon1 + TrkDiMuon2;

  HM->VVar["TrkPtQ"]->push_back(TrkQuad.Pt()*1e-3);
  HM->VVar["TrkMQ"]->push_back(TrkQuad.M()*1e-3);

  HM->VVar["TrkPt"]->push_back(Track0->pt()*1e-3);
  HM->VVar["TrkEta"]->push_back(Track0->eta());
  HM->VVar["TrkPhi"]->push_back(Track0->phi());
  HM->VVar["TrkZ0"]->push_back(z0SinTheta0);
  HM->VVar["TrkD0"]->push_back(d0Sig0);

  HM->VVar["TrkPt"]->push_back(Track1->pt()*1e-3);
  HM->VVar["TrkEta"]->push_back(Track1->eta());
  HM->VVar["TrkPhi"]->push_back(Track1->phi());
  HM->VVar["TrkZ0"]->push_back(z0SinTheta1);
  HM->VVar["TrkD0"]->push_back(d0Sig1);

  HM->VVar["TrkPt"]->push_back(Track2->pt()*1e-3);
  HM->VVar["TrkEta"]->push_back(Track2->eta());
  HM->VVar["TrkPhi"]->push_back(Track2->phi());
  HM->VVar["TrkZ0"]->push_back(z0SinTheta2);
  HM->VVar["TrkD0"]->push_back(d0Sig2);

  HM->VVar["TrkPt"]->push_back(Track3->pt()*1e-3);
  HM->VVar["TrkEta"]->push_back(Track3->eta());
  HM->VVar["TrkPhi"]->push_back(Track3->phi());
  HM->VVar["TrkZ0"]->push_back(z0SinTheta3);
  HM->VVar["TrkD0"]->push_back(d0Sig3);

  HM->VVar["L1TrkPt"]->push_back(Track0->pt()*1e-3);
  HM->VVar["L1TrkEta"]->push_back(Track0->eta());
  HM->VVar["L1TrkPhi"]->push_back(Track0->phi());

  HM->VVar["L2TrkPt"]->push_back(Track1->pt()*1e-3);
  HM->VVar["L2TrkEta"]->push_back(Track1->eta());
  HM->VVar["L2TrkPhi"]->push_back(Track1->phi());

  HM->VVar["L3TrkPt"]->push_back(Track2->pt()*1e-3);
  HM->VVar["L3TrkEta"]->push_back(Track2->eta());
  HM->VVar["L3TrkPhi"]->push_back(Track2->phi());

  HM->VVar["L4TrkPt"]->push_back(Track3->pt()*1e-3);
  HM->VVar["L4TrkEta"]->push_back(Track3->eta());
  HM->VVar["L4TrkPhi"]->push_back(Track3->phi());

  /*
  int nPt4=0;
  if(Track0->pt()>4.e3) nPt4+=1;
  if(Track1->pt()>4.e3) nPt4+=1;
  if(Track2->pt()>4.e3) nPt4+=1;
  if(Track3->pt()>4.e3) nPt4+=1;
  HM->VVar["nPt4"]->push_back(nPt4);
  */
  // make sure at least one Onia pair
  if(m_onia_pairs.size()==0) return EL::StatusCode::SUCCESS;

  //== Upsilon content
  const xAOD::Vertex *cand_o1=0, *cand_o2=0;
  const xAOD::Vertex *cand_DC1=0, *cand_DC2=0;
  const xAOD::Vertex *cand_AC1=0, *cand_AC2=0;
  const xAOD::Vertex *cand_SS1=0, *cand_SS2=0;

  // only look at unique pairs 
  std::vector<const xAOD::Vertex*> dionia_pairs, dionia_pairs_os, dionia_good_pairs, dionia_good_pairs_os;
  // quad charge code
  std::string qc_cand_quad;
  // quad combination code: always ordered by track pt
  std::string cc_cand_quad;

  if(m_useQuad) {
    qc_cand_quad=getQC(m_cand_quad);
    cc_cand_quad=getCC(m_cand_quad);
  } else {
    qc_cand_quad=getQC_OniaPair(m_onia_pairs);
    cc_cand_quad=getCC_OniaPair(m_onia_pairs);
  }
  
  if(m_debug) std::cout << "m_good_qmus.size= " << m_good_qmus.size() << " m_good_pairs.size= " << m_good_pairs.size() << " m_onia_pairs.size= " << m_onia_pairs.size() << " qc_cand_quad=" << qc_cand_quad << " cc_cand_quad= "<< cc_cand_quad << std::endl;

  //== AC, DC combination
  // the two leading tracks are OS or SS
  bool l2os=false;
  std::string qc2=qc_cand_quad.substr(0,2);
  l2os = convCharge(qc2)==0;
  // decide the combination code for DC, AC, and SS
  std::string cc_dc1, cc_dc2, cc_ac1, cc_ac2, cc_ss1, cc_ss2;
  std::string qc_dc1, qc_dc2, qc_ac1, qc_ac2, qc_ss1, qc_ss2;
  // 1, 2 os?
  if(l2os) {
    cc_dc1 = cc_cand_quad.substr(0,2); qc_dc1 = qc_cand_quad.substr(0,2); // 12
    cc_dc2 = cc_cand_quad.substr(2,2); qc_dc2 = qc_cand_quad.substr(2,2); // 34
    std::string tmp_qc, tmp_cc;
    // 1, 3 os?
    tmp_cc = cc_cand_quad.substr(0,1) + cc_cand_quad.substr(2,1);
    tmp_qc = qc_cand_quad.substr(0,1) + qc_cand_quad.substr(2,1);
    if(convCharge(tmp_qc)==0) {
      cc_ac1 = tmp_cc; qc_ac1 = tmp_qc;                             // 13
      cc_ac2 = cc_cand_quad.substr(1,1) + cc_cand_quad.substr(3,1); // 24
      cc_ss1 = cc_cand_quad.substr(0,1) + cc_cand_quad.substr(3,1); // 14
      cc_ss2 = cc_cand_quad.substr(1,1) + cc_cand_quad.substr(2,1); // 23
      qc_ac2 = qc_cand_quad.substr(1,1) + qc_cand_quad.substr(3,1);
      qc_ss1 = qc_cand_quad.substr(0,1) + qc_cand_quad.substr(3,1);
      qc_ss2 = qc_cand_quad.substr(1,1) + qc_cand_quad.substr(2,1);
    } else {
    // 1, 3 ss?
      cc_ss1 = tmp_cc; qc_ss1 = tmp_qc;
      cc_ss2 = cc_cand_quad.substr(1,1) + cc_cand_quad.substr(3,1);
      cc_ac1 = cc_cand_quad.substr(0,1) + cc_cand_quad.substr(3,1);
      cc_ac2 = cc_cand_quad.substr(1,1) + cc_cand_quad.substr(2,1);
      qc_ss2 = qc_cand_quad.substr(1,1) + qc_cand_quad.substr(3,1);
      qc_ac1 = qc_cand_quad.substr(0,1) + qc_cand_quad.substr(3,1);
      qc_ac2 = qc_cand_quad.substr(1,1) + qc_cand_quad.substr(2,1);
    }

  } else {
  // 1, 2 ss? then 3, 3 is also ss
    cc_ss1 = cc_cand_quad.substr(0,2); qc_ss1 = qc_cand_quad.substr(0,2); // 12
    cc_ss2 = cc_cand_quad.substr(2,2); qc_ss2 = qc_cand_quad.substr(2,2); // 34
    std::string tmp_qc, tmp_cc;
    // 1, 3 must be os
    tmp_cc = cc_cand_quad.substr(0,1) + cc_cand_quad.substr(2,1);
    tmp_qc = qc_cand_quad.substr(0,1) + qc_cand_quad.substr(2,1);
    if(convCharge(tmp_qc)==0) {
      cc_dc1 = tmp_cc; qc_dc1 = tmp_qc;                             // 13
      cc_dc2 = cc_cand_quad.substr(1,1) + cc_cand_quad.substr(3,1); // 24
      cc_ac1 = cc_cand_quad.substr(0,1) + cc_cand_quad.substr(3,1); // 14
      cc_ac2 = cc_cand_quad.substr(1,1) + cc_cand_quad.substr(2,1); // 23
      qc_dc2 = qc_cand_quad.substr(1,1) + qc_cand_quad.substr(3,1); 
      qc_ac1 = qc_cand_quad.substr(0,1) + qc_cand_quad.substr(3,1);
      qc_ac2 = qc_cand_quad.substr(1,1) + qc_cand_quad.substr(2,1);
    } else {
      //ATH_MSG_ERROR("doDiOniaSel() if 1, 2 is SS, then 1, 3 must be OS, please check!");
      // since we also look at SS+OS quad, no need to check charge here
      // 14, 23
      cc_dc1 = cc_cand_quad.substr(0,1) + cc_cand_quad.substr(3,1); // 14
      cc_dc2 = cc_cand_quad.substr(1,1) + cc_cand_quad.substr(2,1); // 23
      cc_ac1 = cc_cand_quad.substr(0,1) + cc_cand_quad.substr(2,1); // 13
      cc_ac2 = cc_cand_quad.substr(1,1) + cc_cand_quad.substr(3,1); // 24
      qc_dc1 = qc_cand_quad.substr(0,1) + qc_cand_quad.substr(3,1);
      qc_dc2 = qc_cand_quad.substr(1,1) + qc_cand_quad.substr(2,1);
      qc_ac1 = qc_cand_quad.substr(0,1) + qc_cand_quad.substr(2,1);
      qc_ac2 = qc_cand_quad.substr(1,1) + qc_cand_quad.substr(3,1);
    }
  }

  int ind_o1=-1;
  bool good_AC=false, good_DC=false, good_SS=false;
  for(auto onia_pair: m_onia_pairs) {
    auto o_first=onia_pair.first;
    auto o_second=onia_pair.second;

    // find DC, AC and SS
    if(!cand_DC1 && matchCC(o_first, cc_dc1)) cand_DC1=o_first;
    if(!cand_DC1 && matchCC(o_second, cc_dc1)) cand_DC1=o_second;
    if(!cand_DC2 && matchCC(o_first, cc_dc2)) cand_DC2=o_first;
    if(!cand_DC2 && matchCC(o_second, cc_dc2)) cand_DC2=o_second;

    if(!cand_AC1 && matchCC(o_first, cc_ac1)) cand_AC1=o_first;
    if(!cand_AC1 && matchCC(o_second, cc_ac1)) cand_AC1=o_second;
    if(!cand_AC2 && matchCC(o_first, cc_ac2)) cand_AC2=o_first;
    if(!cand_AC2 && matchCC(o_second, cc_ac2)) cand_AC2=o_second;

    if(!cand_SS1 && matchCC(o_first, cc_ss1)) cand_SS1=o_first;
    if(!cand_SS1 && matchCC(o_second, cc_ss1)) cand_SS1=o_second;
    if(!cand_SS2 && matchCC(o_first, cc_ss2)) cand_SS2=o_first;
    if(!cand_SS2 && matchCC(o_second, cc_ss2)) cand_SS2=o_second;

    bool pass_q_o1 = false, pass_q_o2 = false, pass_chi2_o1 = false, pass_chi2_o2 = false;
    bool pass_m_o1 = false, pass_m_o2 = false;

    pass_q_o1 = getCharge(o_first) == 0;
    pass_q_o2 = getCharge(o_second) == 0;

    double chi2_o1 = getChi2(o_first); 
    double chi2_o2 = getChi2(o_second); 

    pass_chi2_o1 = chi2_o1 < 3;
    pass_chi2_o2 = chi2_o2 < 3;

    // onia mass
    double mass1 = getMass(o_first, 0);
    double mass2 = getMass(o_second, 0);
    //pass_m_o1 = mass1 > 2.e3 && mass1 < 50.e3;
    //pass_m_o2 = mass2 > 2.e3 && mass2 < 50.e3;
    pass_m_o1 = mass1 > 1.e3 && mass1 < 50.e3;
    pass_m_o2 = mass2 > 1.e3 && mass2 < 50.e3;

    bool goodPair=false;

    // at least one Onia consistent with Upsilon: mass, track pt > 4
    //if(pass_q_o1 && pass_q_o2 && pass_chi2_o1 && pass_chi2_o2 && pass_m_o1 && pass_m_o2) 
    if(pass_chi2_o1 && pass_chi2_o2 && pass_m_o1 && pass_m_o2) {
      goodPair=true;

      //if(mass1>9.2e3 && mass1<9.7e3 && pass_q_o1 && passPairTrkPt(o_first, 4e3))  {
      if(mass1>9.2e3 && mass1<9.7e3 && pass_q_o1)  {
        if(!cand_o1 || getChi2(cand_o1) > chi2_o1) {
        cand_o1 = o_first; cand_o2 = o_second;}
      }
      //if(mass2>9.2e3 && mass2<9.7e3 && pass_q_o2 && passPairTrkPt(o_second, 4e3)) {
      if(mass2>9.2e3 && mass2<9.7e3 && pass_q_o2) {
        if(!cand_o1 || getChi2(cand_o1) > chi2_o2) {
        cand_o2 = o_first; cand_o1 = o_second;}
      }
    }

    if(std::find(dionia_pairs.begin(), dionia_pairs.end(), o_first) == dionia_pairs.end()) {
      dionia_pairs.push_back(o_first);
      if(pass_q_o1) dionia_pairs_os.push_back(o_first);
      if(pass_chi2_o1 && pass_m_o1) dionia_good_pairs.push_back(o_first);
      if(pass_chi2_o1 && pass_m_o1 && pass_q_o1) dionia_good_pairs_os.push_back(o_first);
    }
    if(std::find(dionia_pairs.begin(), dionia_pairs.end(), o_second) == dionia_pairs.end()) {
      dionia_pairs.push_back(o_second);
      if(pass_q_o2) dionia_pairs_os.push_back(o_second);
      if(pass_chi2_o2 && pass_m_o2) dionia_good_pairs.push_back(o_second);
      if(pass_chi2_o2 && pass_m_o2 && pass_q_o2) dionia_good_pairs_os.push_back(o_second);
    }

    // check which pair is selected
    if(goodPair) {
      if(cand_DC1 && cand_DC2 && ( (cand_DC1==o_first && cand_DC2==o_second) || (cand_DC2==o_first && cand_DC1==o_second) ))
        good_DC=true;
      if(cand_AC1 && cand_AC2 && ( (cand_AC1==o_first && cand_AC2==o_second) || (cand_AC2==o_first && cand_AC1==o_second) ))
        good_AC=true;
      if(cand_SS1 && cand_SS2 && ( (cand_SS1==o_first && cand_SS2==o_second) || (cand_SS2==o_first && cand_SS1==o_second) ))
        good_SS=true;
    }
  }

  HM->VVar["good_AC"]->push_back(good_AC);
  HM->VVar["good_DC"]->push_back(good_DC);
  HM->VVar["good_SS"]->push_back(good_SS);
  // event categorization

  if(m_msgLevel==MSG::DEBUG) {
    ATH_MSG_INFO("doDiOniaSel() DiOna: " << cc_cand_quad << " " << qc_cand_quad);
    ATH_MSG_INFO("doDiOniaSel() DC1:   " << cc_dc1 << " " << qc_dc1 << " " << getMass(cand_DC1) << " " << getChi2(cand_DC1));
    ATH_MSG_INFO("doDiOniaSel() DC2:   " << cc_dc2 << " " << qc_dc2 << " " << getMass(cand_DC2) << " " << getChi2(cand_DC2));
    ATH_MSG_INFO("doDiOniaSel() AC1:   " << cc_ac1 << " " << qc_ac1 << " " << getMass(cand_AC1) << " " << getChi2(cand_AC1));
    ATH_MSG_INFO("doDiOniaSel() AC2:   " << cc_ac2 << " " << qc_ac2 << " " << getMass(cand_AC2) << " " << getChi2(cand_AC2));
    ATH_MSG_INFO("doDiOniaSel() SS1:   " << cc_ss1 << " " << qc_ss1 << " " << getMass(cand_SS1) << " " << getChi2(cand_SS1));
    ATH_MSG_INFO("doDiOniaSel() SS2:   " << cc_ss2 << " " << qc_ss2 << " " << getMass(cand_SS2) << " " << getChi2(cand_SS2));
  }

  HM->Var["nCandOniaPairs"]=dionia_pairs.size();
  //for(auto pair: dionia_pairs) {
  for(int ip=0; ip<(int)dionia_pairs.size(); ip++) {
    auto pair=dionia_pairs[ip];
    double mass = getMass(pair, 0);
    double chi2 = getChi2(pair);
    double massErr = pair->auxdataConst<float>("PAIR_massErr");

    // Helper Tool
    xAOD::BPhysHelper bVtx(pair);
    TLorentzVector Muon0_RefTrack = bVtx.refTrkOriginP(0, M_MU);
    TLorentzVector Muon1_RefTrack = bVtx.refTrkOriginP(1, M_MU);

    TLorentzVector RefDiMuon = Muon0_RefTrack + Muon1_RefTrack;

    HM->VVar["MassO"]->push_back(mass*1e-3);
    HM->VVar["MassErrO"]->push_back(massErr*1e-3);
    HM->VVar["ChargeO"]->push_back(getCharge(pair));
    HM->VVar["Chi2O"]->push_back(log10(chi2));
    HM->VVar["TrkMO"]->push_back(RefDiMuon.M()*1e-3);
    HM->VVar["TrkPtO"]->push_back(RefDiMuon.Pt()*1e-3);
    HM->VVar["TrkYO"]->push_back(RefDiMuon.Rapidity());

    std::vector<int> trk_inds;
    if(m_useQuad) trk_inds=getTrkInd(pair, m_cand_quad);
    else          trk_inds=getTrkInd(pair, m_good_qmus);
    if(int(trk_inds.size())==2) {
      HM->VVar["CandO_trk_ind1"]->push_back(trk_inds[0]);
      HM->VVar["CandO_trk_ind2"]->push_back(trk_inds[1]);
    }

    if(cand_o1 && cand_o1==pair) HM->VVar["IndO1"]->push_back(ip);
    if(cand_DC1 && cand_DC1==pair) HM->VVar["IndDC1"]->push_back(ip);
    if(cand_DC2 && cand_DC2==pair) HM->VVar["IndDC2"]->push_back(ip);
    if(cand_AC1 && cand_AC1==pair) HM->VVar["IndAC1"]->push_back(ip);
    if(cand_AC2 && cand_AC2==pair) HM->VVar["IndAC2"]->push_back(ip);
    if(cand_SS1 && cand_SS1==pair) HM->VVar["IndSS1"]->push_back(ip);
    if(cand_SS2 && cand_SS2==pair) HM->VVar["IndSS2"]->push_back(ip);

  }
  for(auto pair: dionia_good_pairs_os) {
    double mass = getMass(pair, 0);
    double chi2 = getChi2(pair);

    xAOD::BPhysHelper bVtx(pair);
    TLorentzVector Muon0_RefTrack = bVtx.refTrkOriginP(0, M_MU);
    TLorentzVector Muon1_RefTrack = bVtx.refTrkOriginP(1, M_MU);

    TLorentzVector RefDiMuon = Muon0_RefTrack + Muon1_RefTrack;

    HM->VVar["MassCandO"]->push_back(mass*1e-3);
    HM->VVar["ChargeCandO"]->push_back(getCharge(pair));
    HM->VVar["Chi2CandO"]->push_back(log10(chi2));
    HM->VVar["TrkPtCandO"]->push_back(RefDiMuon.Pt()*1e-3);
    HM->VVar["TrkYCandO"]->push_back(RefDiMuon.Rapidity());
    HM->VVar["TrkdRCandO"]->push_back(Muon0_RefTrack.DeltaR(Muon1_RefTrack));
  }
  if(cand_DC1 && cand_DC2) {
    double mass_1 = getMass(cand_DC1, 0);
    double chi2_1 = getChi2(cand_DC1);

    HM->VVar["MassDC1"]->push_back(mass_1*1e-3);
    HM->VVar["Chi2DC1"]->push_back(log10(chi2_1));
    HM->VVar["ChargeDC1"]->push_back(getCharge(cand_DC1));

    std::vector<int> trk_inds;
    if(m_useQuad) trk_inds=getTrkInd(cand_DC1, m_cand_quad);
    else          trk_inds=getTrkInd(cand_DC1, m_good_qmus);
    if(int(trk_inds.size())==2) {
      HM->VVar["DC1_trk_ind1"]->push_back(trk_inds[0]);
      HM->VVar["DC1_trk_ind2"]->push_back(trk_inds[1]);
    }

    double mass_2 = getMass(cand_DC2, 0);
    double chi2_2 = getChi2(cand_DC2);

    HM->VVar["MassDC2"]->push_back(mass_2*1e-3);
    HM->VVar["Chi2DC2"]->push_back(log10(chi2_2));
    HM->VVar["ChargeDC2"]->push_back(getCharge(cand_DC2));

    if(m_useQuad) trk_inds=getTrkInd(cand_DC2, m_cand_quad);
    else          trk_inds=getTrkInd(cand_DC2, m_good_qmus);
    if(int(trk_inds.size())==2) {
      HM->VVar["DC2_trk_ind1"]->push_back(trk_inds[0]);
      HM->VVar["DC2_trk_ind2"]->push_back(trk_inds[1]);
    }
  }
  if(cand_AC1 && cand_AC2) {
    double mass_1 = getMass(cand_AC1, 0);
    double chi2_1 = getChi2(cand_AC1);

    HM->VVar["MassAC1"]->push_back(mass_1*1e-3);
    HM->VVar["Chi2AC1"]->push_back(log10(chi2_1));
    HM->VVar["ChargeAC1"]->push_back(getCharge(cand_AC1));
     
    std::vector<int> trk_inds;
    if(m_useQuad) trk_inds=getTrkInd(cand_AC1, m_cand_quad);
    else          trk_inds=getTrkInd(cand_AC1, m_good_qmus);
    if(int(trk_inds.size())==2) {
      HM->VVar["AC1_trk_ind1"]->push_back(trk_inds[0]);
      HM->VVar["AC1_trk_ind2"]->push_back(trk_inds[1]);
    }

    double mass_2 = getMass(cand_AC2, 0);
    double chi2_2 = getChi2(cand_AC2);

    HM->VVar["MassAC2"]->push_back(mass_2*1e-3);
    HM->VVar["Chi2AC2"]->push_back(log10(chi2_2));
    HM->VVar["ChargeAC2"]->push_back(getCharge(cand_AC2));

    if(m_useQuad) trk_inds=getTrkInd(cand_AC2, m_cand_quad);
    else          trk_inds=getTrkInd(cand_AC2, m_good_qmus);
    if(int(trk_inds.size())==2) {
      HM->VVar["AC2_trk_ind1"]->push_back(trk_inds[0]);
      HM->VVar["AC2_trk_ind2"]->push_back(trk_inds[1]);
    }
  }
  if(cand_SS1 && cand_SS2) {
    double mass_1 = getMass(cand_SS1, 0);
    double chi2_1 = getChi2(cand_SS1);

    HM->VVar["MassSS1"]->push_back(mass_1*1e-3);
    HM->VVar["Chi2SS1"]->push_back(log10(chi2_1));
    HM->VVar["ChargeSS1"]->push_back(getCharge(cand_SS1));

    std::vector<int> trk_inds;
    if(m_useQuad) trk_inds=getTrkInd(cand_SS1, m_cand_quad);
    else          trk_inds=getTrkInd(cand_SS1, m_good_qmus);
    if(int(trk_inds.size())==2) {
      HM->VVar["SS1_trk_ind1"]->push_back(trk_inds[0]);
      HM->VVar["SS1_trk_ind2"]->push_back(trk_inds[1]);
    }

    double mass_2 = getMass(cand_SS2, 0);
    double chi2_2 = getChi2(cand_SS2);

    HM->VVar["MassSS2"]->push_back(mass_2*1e-3);
    HM->VVar["Chi2SS2"]->push_back(log10(chi2_2));
    HM->VVar["ChargeSS2"]->push_back(getCharge(cand_SS2));

    if(m_useQuad) trk_inds=getTrkInd(cand_SS2, m_cand_quad);
    else          trk_inds=getTrkInd(cand_SS2, m_good_qmus);
    if(int(trk_inds.size())==2) {
      HM->VVar["SS2_trk_ind1"]->push_back(trk_inds[0]);
      HM->VVar["SS2_trk_ind2"]->push_back(trk_inds[1]);
    }
  }
  if(cand_o1 && cand_o2) {
    HM->Cutflow_flag_tmp["Event"]["MassUp"]=1;
    HM->Cutflow_flag_tmp["EventOS"]["MassUp"]=1;
    HM->Cutflow_flag_tmp["EventSS"]["MassUp"]=1;

    double mass = getMass(cand_o1, 0);
    double chi2 = getChi2(cand_o1);

    HM->VVar["MassO1"]->push_back(mass*1e-3);
    HM->VVar["Chi2O1"]->push_back(log10(chi2));

    mass = getMass(cand_o2, 0);
    chi2 = getChi2(cand_o2);

    HM->VVar["MassO2"]->push_back(mass*1e-3);
    HM->VVar["Chi2O2"]->push_back(log10(chi2));

    xAOD::BPhysHelper bVtx1(cand_o1);
    TLorentzVector Muon0_Track = bVtx1.refTrkOriginP(0, M_MU);
    TLorentzVector Muon1_Track = bVtx1.refTrkOriginP(1, M_MU);
    TLorentzVector Muon0_RefTrack = bVtx1.refTrk(0, M_MU);
    TLorentzVector Muon1_RefTrack = bVtx1.refTrk(1, M_MU);
    TLorentzVector O1_Track = Muon0_Track+Muon1_Track;
    TLorentzVector O1_RefTrack = Muon0_RefTrack+Muon1_RefTrack;
    HM->VVar["O1TrkPtMu1"]->push_back(Muon0_Track.Pt()*1e-3);
    HM->VVar["O1TrkPtMu2"]->push_back(Muon1_Track.Pt()*1e-3);
    HM->VVar["O1TrkPt"]->push_back(O1_Track.Pt()*1e-3);
    HM->VVar["O1RefTrkPtMu1"]->push_back(Muon0_RefTrack.Pt()*1e-3);
    HM->VVar["O1RefTrkPtMu2"]->push_back(Muon1_RefTrack.Pt()*1e-3);
    HM->VVar["O1RefTrkPt"]->push_back(O1_RefTrack.Pt()*1e-3);

    //get alpha
    TVector3 v_plus1, v_minus1;
    v_plus1 .SetPtEtaPhi(Muon0_RefTrack.Pt()*1e-3, Muon0_RefTrack.Eta(), Muon0_RefTrack.Phi());
    v_minus1.SetPtEtaPhi(Muon1_RefTrack.Pt()*1e-3, Muon1_RefTrack.Eta(), Muon1_RefTrack.Phi());
    float Alpha_O1 = alphaCal(v_plus1, v_minus1);

    xAOD::BPhysHelper bVtx2(cand_o2);
    TLorentzVector Muon2_Track = bVtx2.refTrkOriginP(0, M_MU);
    TLorentzVector Muon3_Track = bVtx2.refTrkOriginP(1, M_MU);
    TLorentzVector O2_Track = Muon2_Track+Muon3_Track;
    TLorentzVector Muon2_RefTrack = bVtx2.refTrk(0, M_MU);
    TLorentzVector Muon3_RefTrack = bVtx2.refTrk(1, M_MU);
    TLorentzVector O2_RefTrack = Muon2_RefTrack+Muon3_RefTrack;
    HM->VVar["O2TrkPtMu1"]->push_back(Muon2_Track.Pt()*1e-3);
    HM->VVar["O2TrkPtMu2"]->push_back(Muon3_Track.Pt()*1e-3);
    HM->VVar["O2TrkPt"]->push_back(O2_Track.Pt()*1e-3);
    HM->VVar["O2RefTrkPtMu1"]->push_back(Muon2_RefTrack.Pt()*1e-3);
    HM->VVar["O2RefTrkPtMu2"]->push_back(Muon3_RefTrack.Pt()*1e-3);
    HM->VVar["O2RefTrkPt"]->push_back(O2_RefTrack.Pt()*1e-3);

    //get alpha
    TVector3 v_plus2, v_minus2;
    v_plus2 .SetPtEtaPhi(Muon2_RefTrack.Pt()*1e-3, Muon2_RefTrack.Eta(), Muon2_RefTrack.Phi());
    v_minus2.SetPtEtaPhi(Muon3_RefTrack.Pt()*1e-3, Muon3_RefTrack.Eta(), Muon3_RefTrack.Phi());
    float Alpha_O2 = alphaCal(v_plus2, v_minus2);

    //get alpha for X->O1 O2
    TVector3 v_plus0, v_minus0;
    v_plus0 .SetPtEtaPhi(O1_RefTrack.Pt()*1e-3, O1_RefTrack.Eta(), O1_RefTrack.Phi());
    v_minus0.SetPtEtaPhi(O2_RefTrack.Pt()*1e-3, O2_RefTrack.Eta(), O2_RefTrack.Phi());
    float Alpha_X = alphaCal(v_plus0, v_minus0);

    HM->VVar["O1Alpha"]->push_back(Alpha_O1);
    HM->VVar["O2Alpha"]->push_back(Alpha_O2);
    HM->VVar["XAlpha"]->push_back(Alpha_X);

  }

  //== vertex association
  bool pass_vertex=false;

  // Helper Tool
  const xAOD::Vertex *pv_quad=0, *pv_o1=0, *pv_o2=0;
  if(m_cand_quad) {
    xAOD::BPhysHelper bVtx(m_cand_quad);

    HM->VVar["Qa0"]->push_back(bVtx.a0());
    HM->VVar["Qa0Err"]->push_back(bVtx.a0Err());
    HM->VVar["Qz0"]->push_back(bVtx.z0());
    HM->VVar["Qz0Err"]->push_back(bVtx.z0Err());
    HM->VVar["Qa0xy"]->push_back(bVtx.a0xy());
    HM->VVar["Qa0xyErr"]->push_back(bVtx.a0xyErr());
    HM->VVar["Qlxy"]->push_back(bVtx.lxy());
    HM->VVar["QlxyErr"]->push_back(bVtx.lxyErr());

    pv_quad  = bVtx.pv(xAOD::BPhysHelper::PV_MIN_A0); // 0: PvMaxSumPt2Link, 1: PvMinA0Link, 2: PvMinZ0Link
    //if(!pv_quad) ATH_MSG_WARNING("doDiOniaSel () could not find PV_MIN_A0 for quad");
  }
  if(cand_o1 && cand_o2) {
    xAOD::BPhysHelper bVtx_o1(cand_o1);
    pv_o1 = bVtx_o1.pv(xAOD::BPhysHelper::PV_MIN_A0);
    //if(!pv_o1) ATH_MSG_WARNING("doDiOniaSel () could not find PV_MIN_A0 for onia1");

    // check pv of muons from Onia1
    const xAOD::Muon * Muon0_o1 = NULL;
    const xAOD::Muon * Muon1_o1 = NULL;
    getConstiMuons(cand_o1, Muon0_o1, Muon1_o1);
    // get pv of muons
    const xAOD::Vertex *pv0_o1=NULL, *pv1_o1=NULL;
    const xAOD::TrackParticle * IDtrkPart0 = Muon0_o1->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
    if(IDtrkPart0) pv0_o1 = IDtrkPart0->vertex();
    const xAOD::TrackParticle * IDtrkPart1 = Muon1_o1->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
    if(IDtrkPart1) pv1_o1 = IDtrkPart1->vertex();


    // check pv of muons from Onia2
    const xAOD::Muon * Muon0_o2 = NULL;
    const xAOD::Muon * Muon1_o2 = NULL;
    getConstiMuons(cand_o2, Muon0_o2, Muon1_o2);
    // get pv of muons
    const xAOD::Vertex *pv0_o2=NULL, *pv1_o2=NULL;
    const xAOD::TrackParticle * IDtrkPart2 = Muon0_o2->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
    if(IDtrkPart2) pv0_o2 = IDtrkPart2->vertex();
    const xAOD::TrackParticle * IDtrkPart3 = Muon1_o2->trackParticle(xAOD::Muon::InnerDetectorTrackParticle);
    if(IDtrkPart3) pv1_o2 = IDtrkPart3->vertex();

    //if(!pv0_o2) ATH_MSG_WARNING("doDiOniaSel () could not find vertex for mu0");
    //if(!pv1_o2) ATH_MSG_WARNING("doDiOniaSel () could not find vertex for mu1");

    // PV_MIN_A0 is not available
    /*
    if(pv_o1 && pv0_o2 && pv1_o2) {
      if(pv_o1==pv0_o2 || pv_o1==pv1_o2) pass_vertex=true;
    }
    */
    // require all four muons are from the PV
    if(m_pv && pv0_o1 && pv1_o1 && pv0_o2 && pv1_o2) {
      if(m_pv == pv0_o1 && m_pv == pv1_o1 && m_pv == pv0_o2 && m_pv == pv1_o2) pass_vertex=true;
    }
  }
  if(pass_vertex) HM->Cutflow_flag_tmp["Event"]["Vertex"]=1;

  return EL::StatusCode::SUCCESS;
}
