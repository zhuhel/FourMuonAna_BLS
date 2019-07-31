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

EL::StatusCode XFourMuonAnalysis :: doOniaSel () {

  // Onia selection
  //---------------------------------
  m_good_pairs.clear(); m_good_pairs_os.clear(); m_good_upsi.clear(); //!

  xAOD::VertexContainer::const_iterator pairItr = m_pairs->begin();
  for (; pairItr!=m_pairs->end(); ++pairItr) {
    auto pair = *pairItr;

    ATH_MSG_DEBUG("\t" << (*pairItr)->auxdataConst<std::string>("CombinationCode") <<
                 "\t" << (*pairItr)->auxdataConst<std::string>("ChargeCode") <<
                 "\t" << (*pairItr)->auxdataConst<float>("PAIR_mass") << " +/- " << (*pairItr)->auxdataConst<float>("PAIR_massErr"));
    if ((*pairItr)->auxdataConst<MuonLinkVector>("MuonLinks").size()!=2) {
      ATH_MSG_ERROR("doOniaSel() Pair without 2 muon links! Something is wrong!");
      return StatusCode::FAILURE;
    }

    //if(pair->auxdataConst<std::string>("ChargeCode") == "+-" || pair->auxdataConst<std::string>("ChargeCode") == "-+") 
    HM->ClearCutflowFlag("Onia");
    HM->Cutflow_flag_tmp["Onia"]["AllOnia"]=1;
    //if(pair->auxdataConst<float>("chiSquared") < 10000) {
      HM->Cutflow_flag_tmp["Onia"]["Chi2"]=1;
      if(pair->auxdataConst<float>("PAIR_mass") > 1e3 && pair->auxdataConst<float>("PAIR_mass") < 50e3) {
        HM->Cutflow_flag_tmp["Onia"]["Mass"]=1;
        if( passPairTrkPt(pair, 2.5e3) ) {
          HM->Cutflow_flag_tmp["Onia"]["OniaTrkKine"]=1;
          m_good_pairs.push_back(pair);
          if( getCharge( pair ) == 0 ) {
            HM->Cutflow_flag_tmp["Onia"]["OniaQ"]=1;
            m_good_pairs_os.push_back(pair);
          }
        }
      }
    //}
   
    HM->FillCutflow("Onia");
  }

  if(m_msgLevel==MSG::DEBUG) ATH_MSG_INFO("doOniaSel() Number of good pairs = " << m_good_pairs_os.size() );
  HM->Var["nPairsOS"]=m_good_pairs_os.size();

  int nGoodPair=0;
  //== make some plots
  for(auto pair: m_good_pairs_os) {
    HM->ClearCutflowFlag("Onia");
    bool pass_pt=false, pass_cb=false, pass_chi2=false, pass_mass=false;

    double mass = pair->auxdataConst<float>("PAIR_mass");
    double massErr = pair->auxdataConst<float>("PAIR_massErr");
    double chi2 = pair->auxdataConst<float>("chiSquared");

    if( passPairTrkPt(pair, 4e3) ) pass_pt=true;
    if( chi2 < 1 ) pass_chi2=true;
    if( mass>9.2e3 && mass<9.7e3) pass_mass=true;

    //==== muon kinematics
    const MuonLinkVector MuonLinks = pair->auxdataConst<MuonLinkVector>("MuonLinks");
    const MuonLink& MuonLink0 = MuonLinks[0];
    const MuonLink& MuonLink1 = MuonLinks[1];

    //std::cout << MuonLink0.isValid() << " " << MuonLink1.isValid() << std::endl;

    const xAOD::Muon * Muon0 = NULL;
    const xAOD::Muon * Muon1 = NULL;
    if(MuonLink0.isValid()) Muon0 = *MuonLink0;
    if(MuonLink1.isValid()) Muon1 = *MuonLink1;
    // std::cout << Muon0 << " " << Muon1 << std::endl;
    if(Muon0 == NULL || Muon1 == NULL) {
      HM->FillCutflow("Onia");
      continue;
    }
    HM->Cutflow_flag_tmp["Onia"]["MuonMatching"]=1;
    HM->Cutflow_flag["Onia"]["MuonMatching"]=1;

    const xAOD::TrackParticle * Track0 = NULL;
    const xAOD::TrackParticle * Track1 = NULL;

    //==== tracks
    const TrkLink& Link0 = Muon0->inDetTrackParticleLink();
    const TrkLink& Link1 = Muon1->inDetTrackParticleLink();

    if(Link0.isValid()) Track0 = *Link0;
    if(Link1.isValid()) Track1 = *Link1;

    if(Track0 == NULL || Track1 == NULL) {
      Warning("XFourMuonAnalysis::doOniaSel()", "Missing Track Pointers!");
      HM->FillCutflow("Onia");
      continue;
    }
    HM->Cutflow_flag_tmp["Onia"]["TrkMatching"]=1;


    int nCb=0;
    if(Muon0->muonType() == xAOD::Muon::Combined) nCb ++;
    if(Muon1->muonType() == xAOD::Muon::Combined) nCb ++;
    if(nCb==2) pass_cb=true;

    // match with the four good muons
    if(matchFourMuon(pair, m_good_qmus)) {
      HM->VVar["MatchQMu_Onia_PreSel"]->push_back(1);
      if(chi2 < 3) nGoodPair += 1;
    }
    else HM->VVar["MatchQMu_Onia_PreSel"]->push_back(0);

    // kinematics
    HM->VVar["MassO_Onia_PreSel"]->push_back(mass*1e-3);
    HM->VVar["MassErrO_Onia_PreSel"]->push_back(massErr*1e-3);
    HM->VVar["Chi2O_Onia_PreSel"]->push_back(log10(chi2));

    // muons
    TLorentzVector MuDiMuon1 = Muon0->p4() + Muon1->p4();
    HM->VVar["MuMO_Onia_PreSel"]->push_back(MuDiMuon1.M()*1e-3);
    HM->VVar["MuPtO_Onia_PreSel"]->push_back(MuDiMuon1.Pt()*1e-3);
    HM->VVar["MuPhiO_Onia_PreSel"]->push_back(MuDiMuon1.Phi());
    HM->VVar["MuYO_Onia_PreSel"]->push_back(MuDiMuon1.Rapidity());

    HM->VVar["L1IsCB_Onia_PreSel"]->push_back(Muon0->muonType()==0);
    HM->VVar["L1Pt_Onia_PreSel"]->push_back(Muon0->pt()*1e-3);
    HM->VVar["L1Eta_Onia_PreSel"]->push_back(Muon0->eta());
    HM->VVar["L1Phi_Onia_PreSel"]->push_back(Muon0->phi());

    HM->VVar["L2IsCB_Onia_PreSel"]->push_back(Muon1->muonType()==0);
    HM->VVar["L2Pt_Onia_PreSel"]->push_back(Muon1->pt()*1e-3);
    HM->VVar["L2Eta_Onia_PreSel"]->push_back(Muon1->eta());
    HM->VVar["L2Phi_Onia_PreSel"]->push_back(Muon1->phi());

    // tracks
    TLorentzVector TrkDiMuon1 = Track0->p4() + Track1->p4();
    HM->VVar["TrkMO_Onia_PreSel"]->push_back(TrkDiMuon1.M()*1e-3);
    HM->VVar["TrkPtO_Onia_PreSel"]->push_back(TrkDiMuon1.Pt()*1e-3);
    HM->VVar["TrkPhiO_Onia_PreSel"]->push_back(TrkDiMuon1.Phi());
    HM->VVar["TrkYO_Onia_PreSel"]->push_back(TrkDiMuon1.Rapidity());

    HM->VVar["L1TrkPt_Onia_PreSel"]->push_back(Track0->pt()*1e-3);
    HM->VVar["L1TrkEta_Onia_PreSel"]->push_back(Track0->eta());
    HM->VVar["L1TrkPhi_Onia_PreSel"]->push_back(Track0->phi());

    HM->VVar["L2TrkPt_Onia_PreSel"]->push_back(Track1->pt()*1e-3);
    HM->VVar["L2TrkEta_Onia_PreSel"]->push_back(Track1->eta());
    HM->VVar["L2TrkPhi_Onia_PreSel"]->push_back(Track1->phi());

    // Helper Tool
    xAOD::BPhysHelper bVtx(pair);
    TLorentzVector Muon0_RefTrack = bVtx.refTrk(0, M_MU);
    TLorentzVector Muon1_RefTrack = bVtx.refTrk(1, M_MU);

    TLorentzVector RefDiMuon = Muon0_RefTrack + Muon1_RefTrack;

    HM->VVar["MassORefTrk_Onia_PreSel"]->push_back(RefDiMuon.M()*1e-3);
    HM->VVar["PtORefTrk_Onia_PreSel"]->push_back(RefDiMuon.Pt()*1e-3);
    HM->VVar["YORefTrk_Onia_PreSel"]->push_back(RefDiMuon.Rapidity());

    if(pass_pt) HM->Cutflow_flag_tmp["Onia"]["Pt"]=1;
    if(pass_cb) HM->Cutflow_flag_tmp["Onia"]["CB"]=1;
    if(pass_chi2) HM->Cutflow_flag_tmp["Onia"]["Chi2Upsi"]=1;
    if(pass_mass) HM->Cutflow_flag_tmp["Onia"]["MassUpsi"]=1;
    if(pass_pt && pass_cb && pass_chi2 && pass_mass) m_good_upsi.push_back(pair);

    HM->FillCutflow("Onia");
  }

  if(nGoodPair>=1) HM->Cutflow_flag_tmp["Event"]["Onia"]=1;

  return EL::StatusCode::SUCCESS;
}
