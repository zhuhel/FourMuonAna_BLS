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
#include <TLorentzVector.h>
                                                                     
EL::StatusCode XFourMuonAnalysis :: doEvaSel(){

/*
  // Onia selection
  //---------------------------------
  m_good_pairs.clear(); m_good_pairs_os.clear(); //!

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

    HM->ClearCutflowFlag("Onia");
    HM->Cutflow_flag_tmp["Onia"]["AllOnia"]=1;
    if(pair->auxdataConst<float>("chiSquared") < 100000) {
      HM->Cutflow_flag_tmp["Onia"]["Chi2"]=1;
      if(pair->auxdataConst<float>("PAIR_mass") > 0 ) {
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
    }
   
    HM->FillCutflow("Onia");
  }

  if(m_msgLevel==MSG::DEBUG) ATH_MSG_INFO("doEvaSel() Number of good pairs = " << m_good_pairs_os.size() );
  HM->Var["nPairsOS"]=m_good_pairs_os.size();
*/  

  // DiOnia selection
  //---------------------------------
  std::vector<const xAOD::Vertex*> good_quads_os, cand_quads_os, cand_quads_os_chi2;
  DiOnia good_dionia_os, cand_dionia_os_chi2;
  // reset !!
  m_onia_pairs.clear();
  m_cand_quad=0;

  bool doOSonly=true; // temp flag

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
    HM->Cutflow_flag_tmp["DiOnia"]["MatchQMu"]=1;

      //== should match two good pairs
      if( !doOSonly || getCharge( quad ) == 0 ) {
        HM->Cutflow_flag_tmp["DiOnia"]["DiOniaQ"]=1;
        OniaPairVec onia_pairs; // <o1, o2>, <o3, o4>, ...
        onia_pairs = matchOnia(quadCombinationCode, m_good_pairs);
        //if(onia_pairs.size()>0) {
          HM->Cutflow_flag_tmp["DiOnia"]["OniaMatching"]=1;
          good_quads_os.push_back(quad);
          if(onia_pairs.size()>0) good_dionia_os[quad]=onia_pairs;  // map: quad -- < <o1, o2> , <o3, o4>, ... >
          ATH_MSG_DEBUG("doDiOniaSel() Matched quad with two pairs: quad= " << quadCombinationCode << " pairs: ");
          for(auto onia_pair: onia_pairs) {
            ATH_MSG_DEBUG("        pairs1: " << onia_pair.first->auxdataConst<std::string>("CombinationCode") << " " << onia_pair.second->auxdataConst<std::string>("CombinationCode"));
          }
        //}
      }

    HM->FillCutflow("DiOnia");
  }

  if(m_msgLevel==MSG::DEBUG) ATH_MSG_INFO("doDiOniaSel() Number of good quads = " << good_quads_os.size()  );
  HM->Var["nQuadsOS"]=good_quads_os.size();

  // Loop through the quad
  double CandMassQRefTrk=-999.;
  double min_quad_chi2=1e9;
  for(auto quad: good_quads_os) {
    HM->ClearCutflowFlag("DiOnia");

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
    if(Muon0 == NULL || Muon1 == NULL || Muon2 == NULL || Muon3 == NULL){
      HM->FillCutflow("DiOnia");
      continue;
    }
    HM->VVar["Q_L1Pt"]->push_back(Muon0->pt()*1e-3);
    HM->VVar["Q_L2Pt"]->push_back(Muon1->pt()*1e-3);
    HM->VVar["Q_L3Pt"]->push_back(Muon2->pt()*1e-3);
    HM->VVar["Q_L4Pt"]->push_back(Muon3->pt()*1e-3);
    HM->VVar["Q_L1Eta"]->push_back(Muon0->eta());
    HM->VVar["Q_L2Eta"]->push_back(Muon1->eta());
    HM->VVar["Q_L3Eta"]->push_back(Muon2->eta());
    HM->VVar["Q_L4Eta"]->push_back(Muon3->eta());
    HM->VVar["Q_L1Phi"]->push_back(Muon0->phi());
    HM->VVar["Q_L2Phi"]->push_back(Muon1->phi());
    HM->VVar["Q_L3Phi"]->push_back(Muon2->phi());
    HM->VVar["Q_L4Phi"]->push_back(Muon3->phi());

    //== muon CB cuts
    int nCb=0;
    if(Muon0->muonType() == xAOD::Muon::Combined) nCb ++;
    if(Muon1->muonType() == xAOD::Muon::Combined) nCb ++;
    if(Muon2->muonType() == xAOD::Muon::Combined) nCb ++;
    if(Muon3->muonType() == xAOD::Muon::Combined) nCb ++;
    if(nCb<3){
      HM->FillCutflow("DiOnia");
      continue;
    }
    HM->Cutflow_flag_tmp["DiOnia"]["MuonCB"]=1;
    HM->Cutflow_flag["DiOnia"]["MuonCB"]=1;

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
      Warning("XFourMuonAnalysis::doEvaSel()", "Missing Track Pointers for QUAD!");
      HM->FillCutflow("DiOnia");
      continue;
    }

    
    //== track kinematics
    HM->VVar["Q_L1TrkPt"]->push_back(Track0->pt()*1e-3);
    HM->VVar["Q_L2TrkPt"]->push_back(Track1->pt()*1e-3);
    HM->VVar["Q_L3TrkPt"]->push_back(Track2->pt()*1e-3);
    HM->VVar["Q_L4TrkPt"]->push_back(Track3->pt()*1e-3);
    HM->VVar["Q_L1TrkEta"]->push_back(Track0->eta());
    HM->VVar["Q_L2TrkEta"]->push_back(Track1->eta());
    HM->VVar["Q_L3TrkEta"]->push_back(Track2->eta());
    HM->VVar["Q_L4TrkEta"]->push_back(Track3->eta());
    HM->VVar["Q_L1TrkPhi"]->push_back(Track0->phi());
    HM->VVar["Q_L2TrkPhi"]->push_back(Track1->phi());
    HM->VVar["Q_L3TrkPhi"]->push_back(Track2->phi());
    HM->VVar["Q_L4TrkPhi"]->push_back(Track3->phi());
    float Qchi2 = getChi2(quad);
    float Qcharge = getCharge( quad );
    HM->VVar["Q_chi2"]->push_back(Qchi2);
    HM->VVar["Q_charge"]->push_back(Qcharge);
    bool pass_trkkine=true;
    //if(Track0->pt()<3.e3 || Track1->pt()<3.e3 || Track2->pt()<3.e3 || Track3->pt()<3.e3) pass_trkkine=false;
    if(Track0->pt()<2.5e3 || Track1->pt()<2.5e3 || Track2->pt()<2.5e3 || Track3->pt()<2.5e3) pass_trkkine=false;
    if(fabs(Track0->eta())>2.5 || fabs(Track1->eta())>2.5 || fabs(Track2->eta())>2.5 || fabs(Track3->eta())>2.5)
      pass_trkkine=false;
    //if( ( int(Track0->pt()>4.e3) + int(Track1->pt()>4.e3) + int(Track2->pt()>4.e3) + int(Track3->pt()>4.e3) )<2) pass_trkkine=false;
    if( !pass_trkkine ){
      HM->FillCutflow("DiOnia");
      continue;
    }
    HM->Cutflow_flag["DiOnia"]["TrkKine"]=1;

    //---------------------------- James' ntuple selection done! -----------------------------------
    //---------------------------- Belows are Eva's selection! -------------------------------------

    //== get 4-track fit mass
    xAOD::BPhysHelper bVtx(quad);
    TLorentzVector Muon0_RefTrack = bVtx.refTrk(0, M_MU);
    TLorentzVector Muon1_RefTrack = bVtx.refTrk(1, M_MU);
    TLorentzVector Muon2_RefTrack = bVtx.refTrk(2, M_MU);
    TLorentzVector Muon3_RefTrack = bVtx.refTrk(3, M_MU);
    // refit track kinematic
    HM->VVar["Q_L1RefTrkPt"]->push_back(Muon0_RefTrack.Pt()*1e-3);
    HM->VVar["Q_L2RefTrkPt"]->push_back(Muon1_RefTrack.Pt()*1e-3);
    HM->VVar["Q_L3RefTrkPt"]->push_back(Muon2_RefTrack.Pt()*1e-3);
    HM->VVar["Q_L4RefTrkPt"]->push_back(Muon3_RefTrack.Pt()*1e-3);
    HM->VVar["Q_L1RefTrkEta"]->push_back(Muon0_RefTrack.Eta());
    HM->VVar["Q_L2RefTrkEta"]->push_back(Muon1_RefTrack.Eta());
    HM->VVar["Q_L3RefTrkEta"]->push_back(Muon2_RefTrack.Eta());
    HM->VVar["Q_L4RefTrkEta"]->push_back(Muon3_RefTrack.Eta());
    HM->VVar["Q_L1RefTrkPhi"]->push_back(Muon0_RefTrack.Phi());
    HM->VVar["Q_L2RefTrkPhi"]->push_back(Muon1_RefTrack.Phi());
    HM->VVar["Q_L3RefTrkPhi"]->push_back(Muon2_RefTrack.Phi());
    HM->VVar["Q_L4RefTrkPhi"]->push_back(Muon3_RefTrack.Phi());
    
    if(Muon0->charge()!=Track0->charge() || Muon1->charge()!=Track1->charge() || Muon2->charge()!= Track2->charge() || Muon3->charge()!=Track3->charge()){
      HM->FillCutflow("DiOnia");
      continue;
    }
    
    HM->Cutflow_flag_tmp["DiOnia"]["TrkCharge"]=1;
    HM->Cutflow_flag["DiOnia"]["TrkCharge"]=1;

    TLorentzVector RefDiMuon1 = Muon0_RefTrack + Muon1_RefTrack;
    TLorentzVector RefDiMuon2 = Muon2_RefTrack + Muon3_RefTrack;
    TLorentzVector RefQuad = RefDiMuon1 + RefDiMuon2;
    double MassQRefTrk = RefQuad.M()*1e-3;

    //== Either AC or DC exist with chi2<3 // don't have this selection now
    bool pass_two_o=true; // two good Onias: charge 0
    bool pass_two_o_chi2=true; // two good Onias: charge 0, chi2 < 3

    OniaPairVec onia_pairs = good_dionia_os[quad];
    // only look at unique pairs
    for(auto onia_pair: onia_pairs) {
      bool pass_tmp_q_o1 = false, pass_tmp_q_o2 = false, pass_tmp_chi2_o1 = false, pass_tmp_chi2_o2 = false;

      pass_tmp_q_o1 = getCharge(onia_pair.first) == 0;
      pass_tmp_q_o2 = getCharge(onia_pair.second) == 0;
      if(pass_tmp_q_o1 && pass_tmp_q_o2) pass_two_o=true;

      double chi2_o1 = getChi2(onia_pair.first);
      double chi2_o2 = getChi2(onia_pair.second);
      pass_tmp_chi2_o1 = chi2_o1 < 3;
      pass_tmp_chi2_o2 = chi2_o2 < 3;
      if(pass_tmp_chi2_o1 && pass_tmp_chi2_o2) pass_two_o_chi2=true;
    }
    if( !pass_two_o || !pass_two_o_chi2){
      HM->FillCutflow("DiOnia");
      continue;
    }
    HM->Cutflow_flag["DiOnia"]["OniaChi2"]=1;
    
    if( MassQRefTrk > 100.){
      HM->FillCutflow("DiOnia");
      continue;
    }
    HM->Cutflow_flag["DiOnia"]["Mass"]=1;
    HM->VVar["MassQRefTrk"]->push_back(MassQRefTrk);

    //== at least two muons with pt>4GeV
    bool pass_L2Pt4=true;
    if( ( int(Track0->pt()>4.e3) + int(Track1->pt()>4.e3) + int(Track2->pt()>4.e3) + int(Track3->pt()>4.e3) )<2) pass_L2Pt4=false;
    if( !pass_L2Pt4 ){
      HM->FillCutflow("DiOnia");
      continue;
    }
    HM->Cutflow_flag["DiOnia"]["L2Pt4"]=1;

    cand_quads_os_chi2.push_back(quad);
    cand_dionia_os_chi2[quad]=good_dionia_os[quad];
    HM->FillCutflow("DiOnia");

    //== select the quad with smallest chi2
    double tmp_chi2 = getChi2(quad);
    if(tmp_chi2 < min_quad_chi2) {
       min_quad_chi2=tmp_chi2;
       m_cand_quad=quad;
       CandMassQRefTrk = MassQRefTrk;
    }
  }

  //====== select Quad with smallest chi2
  if(m_cand_quad) {
    double mass_quad = getMass(m_cand_quad, 1);
    double chi2_quad = getChi2(m_cand_quad);

    HM->VVar["MassQ"]->push_back(mass_quad*1e-3);
    HM->VVar["Chi2Q"]->push_back(log10(chi2_quad));
    HM->VVar["ChargeQ"]->push_back(getCharge(m_cand_quad));

    HM->VVar["CandQ_MassRefTrk"]->push_back(CandMassQRefTrk);

    //== Muon quality counting
    const MuonLinkVector MuonLinks = m_cand_quad->auxdataConst<MuonLinkVector>("MuonLinks");
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
    if(Muon0 == NULL || Muon1 == NULL || Muon2 == NULL || Muon3 == NULL){
      Warning("XFourMuonAnalysis::doEvaSel()", "Missing Four muons in CandQuad");
      return EL::StatusCode::SUCCESS;
    }

    //== muon momentumBalanceSignificancei (??? don't know what it is)
    
    float tmpFloat=0.;
    HM->VVar["CandQ_momentumBalanceSignificance"]->push_back(Muon0->parameter(tmpFloat,xAOD::Muon::momentumBalanceSignificance)? tmpFloat : -999.);
    HM->VVar["CandQ_momentumBalanceSignificance"]->push_back(Muon1->parameter(tmpFloat,xAOD::Muon::momentumBalanceSignificance)? tmpFloat : -999.);
    HM->VVar["CandQ_momentumBalanceSignificance"]->push_back(Muon2->parameter(tmpFloat,xAOD::Muon::momentumBalanceSignificance)? tmpFloat : -999.);
    HM->VVar["CandQ_momentumBalanceSignificance"]->push_back(Muon3->parameter(tmpFloat,xAOD::Muon::momentumBalanceSignificance)? tmpFloat : -999.);
   
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
      Warning("XFourMuonAnalysis::doEvaSel()", "Missing Track Pointers for CandQUAD!");
      return EL::StatusCode::SUCCESS;
    }

    //== d0 and z0
    HM->VVar["CandQ_z0SinTheta"]->push_back( (Track0->z0()+Track0->vz()-m_pv->z())*sin(Track0->p4().Theta()) );
    HM->VVar["CandQ_z0SinTheta"]->push_back( (Track1->z0()+Track1->vz()-m_pv->z())*sin(Track1->p4().Theta()) );
    HM->VVar["CandQ_z0SinTheta"]->push_back( (Track2->z0()+Track2->vz()-m_pv->z())*sin(Track2->p4().Theta()) );
    HM->VVar["CandQ_z0SinTheta"]->push_back( (Track3->z0()+Track3->vz()-m_pv->z())*sin(Track3->p4().Theta()) );
    HM->VVar["CandQ_d0Sig"]->push_back( xAOD::TrackingHelpers::d0significance(Track0, m_eventInfo->beamPosSigmaX(), m_eventInfo->beamPosSigmaY(), m_eventInfo->beamPosSigmaXY() ) );
    HM->VVar["CandQ_d0Sig"]->push_back( xAOD::TrackingHelpers::d0significance(Track1, m_eventInfo->beamPosSigmaX(), m_eventInfo->beamPosSigmaY(), m_eventInfo->beamPosSigmaXY() ) );
    HM->VVar["CandQ_d0Sig"]->push_back( xAOD::TrackingHelpers::d0significance(Track2, m_eventInfo->beamPosSigmaX(), m_eventInfo->beamPosSigmaY(), m_eventInfo->beamPosSigmaXY() ) );
    HM->VVar["CandQ_d0Sig"]->push_back( xAOD::TrackingHelpers::d0significance(Track3, m_eventInfo->beamPosSigmaX(), m_eventInfo->beamPosSigmaY(), m_eventInfo->beamPosSigmaXY() ) );

    //== muon quality
    int nCb=0;
    if(Muon0->muonType() == xAOD::Muon::Combined) nCb ++;
    if(Muon1->muonType() == xAOD::Muon::Combined) nCb ++;
    if(Muon2->muonType() == xAOD::Muon::Combined) nCb ++;
    if(Muon3->muonType() == xAOD::Muon::Combined) nCb ++;
    HM->VVar["CandQ_nCB"]->push_back(nCb);

    //calibration
    // !! need to delete the pointers for the calibrated muons, to avoid memory blow up
    xAOD::Muon *mu_corr0=NULL, *mu_corr1=NULL, *mu_corr2=NULL, *mu_corr3=NULL;
    if(m_muonsMap.find(Muon0) !=m_muonsMap.end()) {
      mu_corr0 = m_muonsMap[Muon0];
    } else {
      CP_CHECK("doEvaSel()", m_muonCalibrationAndSmearingTool->correctedCopy(*Muon0, mu_corr0));
      m_muonsMap[Muon0]=mu_corr0;
    }
    if(m_muonsMap.find(Muon1) !=m_muonsMap.end()) {
      mu_corr1 = m_muonsMap[Muon1];
    } else {
      CP_CHECK("doEvaSel()", m_muonCalibrationAndSmearingTool->correctedCopy(*Muon1, mu_corr1));
      m_muonsMap[Muon1]=mu_corr1;
    }
    if(m_muonsMap.find(Muon2) !=m_muonsMap.end()) {
      mu_corr2 = m_muonsMap[Muon2];
    } else {
      CP_CHECK("doEvaSel()", m_muonCalibrationAndSmearingTool->correctedCopy(*Muon2, mu_corr2));
      m_muonsMap[Muon2]=mu_corr2;
    }
    if(m_muonsMap.find(Muon3) !=m_muonsMap.end()) {
      mu_corr3 = m_muonsMap[Muon3];
    } else {
      CP_CHECK("doEvaSel()", m_muonCalibrationAndSmearingTool->correctedCopy(*Muon3, mu_corr3));
      m_muonsMap[Muon3]=mu_corr3;
    }

    int nLoose=0, nMedium=0, nLowPt=0;
    if(m_doMuonCorr) {
      if(m_muonSelectionToolLoose->accept(*mu_corr0)) nLoose+=1;
      if(m_muonSelectionToolMedium->accept(*mu_corr0)) nMedium+=1;
      if(m_muonSelectionToolLowPt->accept(*mu_corr0)) nLowPt+=1;

      if(m_muonSelectionToolLoose->accept(*mu_corr1)) nLoose+=1;
      if(m_muonSelectionToolMedium->accept(*mu_corr1)) nMedium+=1;
      if(m_muonSelectionToolLowPt->accept(*mu_corr1)) nLowPt+=1;

      if(m_muonSelectionToolLoose->accept(*mu_corr2)) nLoose+=1;
      if(m_muonSelectionToolMedium->accept(*mu_corr2)) nMedium+=1;
      if(m_muonSelectionToolLowPt->accept(*mu_corr2)) nLowPt+=1;

      if(m_muonSelectionToolLoose->accept(*mu_corr3)) nLoose+=1;
      if(m_muonSelectionToolMedium->accept(*mu_corr3)) nMedium+=1;
      if(m_muonSelectionToolLowPt->accept(*mu_corr3)) nLowPt+=1;
    } else {
      if(m_muonSelectionToolLoose->accept(*Muon0)) nLoose+=1;
      if(m_muonSelectionToolMedium->accept(*Muon0)) nMedium+=1;
      if(m_muonSelectionToolLowPt->accept(*Muon0)) nLowPt+=1;

      if(m_muonSelectionToolLoose->accept(*Muon1)) nLoose+=1;
      if(m_muonSelectionToolMedium->accept(*Muon1)) nMedium+=1;
      if(m_muonSelectionToolLowPt->accept(*Muon1)) nLowPt+=1;

      if(m_muonSelectionToolLoose->accept(*Muon2)) nLoose+=1;
      if(m_muonSelectionToolMedium->accept(*Muon2)) nMedium+=1;
      if(m_muonSelectionToolLowPt->accept(*Muon2)) nLowPt+=1;

      if(m_muonSelectionToolLoose->accept(*Muon3)) nLoose+=1;
      if(m_muonSelectionToolMedium->accept(*Muon3)) nMedium+=1;
      if(m_muonSelectionToolLowPt->accept(*Muon3)) nLowPt+=1;
    }
    HM->VVar["CandQ_nLoose"]->push_back(nLoose);
    HM->VVar["CandQ_nMedium"]->push_back(nMedium);
    HM->VVar["CandQ_nLowPt"]->push_back(nLowPt);


  }

  //------------------------------------------------------------------------------------------
  // After preselection, then do event selections
  //------------------------------------------------------------------------------------------

  if(m_useQuad==1) {  // use Quad vertex fit
    //== at least one good Quad
    if(cand_quads_os_chi2.size()>0){
       HM->Cutflow_flag_tmp["Event"]["DiOnia"]=1;
    }
    else 
      return EL::StatusCode::SUCCESS;

    //== onia pairs matched to this quad
    //std::cout << "Check=> cand_quads_os_chi2: " << cand_quads_os_chi2.size() << std::endl;
    m_onia_pairs = cand_dionia_os_chi2[m_cand_quad];
    //std::cout << "Check=> m_cand_quad: " << cand_dionia_os_chi2[m_cand_quad].size() << std::endl;
    if(!m_cand_quad){
        //ATH_MSG_ERROR("doDiOniaSel() Could not find candidate quad");
        return EL::StatusCode::SUCCESS;
    }
    HM->Cutflow_flag_tmp["Event"]["Cand"]=1;
    // OS, SS
    if(getCharge( m_cand_quad ) == 0) HM->Cutflow_flag_tmp["EventOS"]["Cand"]=1;
    else if(fabs(getCharge( m_cand_quad )) == 2) HM->Cutflow_flag_tmp["EventSS"]["Cand"]=1;
             
    // should pass all previous common selections
    HM->FillCutflowFlag("Event");
    HM->Cutflow_flag_tmp["EventOS"]["Cand"] &= HM->Cutflow_flag["Event"]["Cand"];
    HM->Cutflow_flag_tmp["EventSS"]["Cand"] &= HM->Cutflow_flag["Event"]["Cand"];

  } 

  //== labeling of Onia1
  //std::cout << "Check=> m_onia_pairs: " << m_onia_pairs.size() << std::endl;
  const xAOD::Vertex *cand_o1=0, *cand_o2=0;
  for(auto onia_pair: m_onia_pairs) {
    auto o_first=onia_pair.first;
    auto o_second=onia_pair.second;

    bool pass_q_o1 = false, pass_q_o2 = false, pass_chi2_o1 = false, pass_chi2_o2 = false;
    bool pass_m_o1 = false, pass_m_o2 = false;

    pass_q_o1 = getCharge(o_first) == 0;
    pass_q_o2 = getCharge(o_second) == 0;
    //if(!pass_q_o1 || !pass_q_o2) std::cout<<"charge O1: "<<getCharge(o_first)<<"    charge O2: "<<getCharge(o_second)<<std::endl;

    double chi2_o1 = getChi2(o_first);
    double chi2_o2 = getChi2(o_second);
    pass_chi2_o1 = true;  //chi2_o1 < 3;
    pass_chi2_o2 = true;  //chi2_o2 < 3;

    double mass1 = getMass(o_first, 0);
    double mass2 = getMass(o_second, 0);
    pass_m_o1 = true; //mass1 > 2.e3 && mass1 < 50.e3;
    pass_m_o2 = true; //mass2 > 2.e3 && mass2 < 50.e3;

    // Calculate the rapidity of Onias
    xAOD::BPhysHelper bVtx1(o_first);
    TLorentzVector Muon0_pretrk = bVtx1.refTrkOriginP(0, M_MU);
    TLorentzVector Muon1_pretrk = bVtx1.refTrkOriginP(1, M_MU);
    TLorentzVector Ofirst_pretrk = Muon0_pretrk+Muon1_pretrk;
    xAOD::BPhysHelper bVtx2(o_second);
    TLorentzVector Muon2_pretrk = bVtx2.refTrkOriginP(0, M_MU);
    TLorentzVector Muon3_pretrk = bVtx2.refTrkOriginP(1, M_MU);
    TLorentzVector Osecond_pretrk = Muon2_pretrk+Muon3_pretrk;


    if(pass_chi2_o1 && pass_chi2_o2 && pass_m_o1 && pass_m_o2) {
      HM->VVar["CandQ_MassO"]->push_back(mass1*1e-3);
      HM->VVar["CandQ_MassO"]->push_back(mass2*1e-3);
      HM->VVar["CandQ_OY"]->push_back(Ofirst_pretrk.Rapidity());
      HM->VVar["CandQ_OY"]->push_back(Osecond_pretrk.Rapidity());
      if(mass1>9.2e3 && mass1<9.7e3 && pass_q_o1)  {
        if(!cand_o1 || getChi2(cand_o1) > chi2_o1) {
        cand_o1 = o_first; cand_o2 = o_second;}
      }
      if(mass2>9.2e3 && mass2<9.7e3 && pass_q_o2) {
        if(!cand_o1 || getChi2(cand_o1) > chi2_o2) {
        cand_o2 = o_first; cand_o1 = o_second;}
      }
    }
  }

  //============== fill some more variable to use
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

  HM->FillCutflowFlag("Event");
  return EL::StatusCode::SUCCESS;
}
