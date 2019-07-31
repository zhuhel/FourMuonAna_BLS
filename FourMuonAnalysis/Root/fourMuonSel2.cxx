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
                                                                     
//float XFourMuonAnalysis :: alphaCal(TVector3 v_plus, TVector3 v_minus){
//
//  float P1 = v_plus.Mag();
//  float P2 = v_minus.Mag();
//  float cosalpha1 = v_plus.Dot((v_plus+v_minus))/(v_plus.Mag()*(v_plus+v_minus).Mag());
//  float cosalpha2 = v_minus.Dot((v_plus+v_minus))/(v_minus.Mag()*(v_plus+v_minus).Mag()); 
//  float pL1 = v_plus.Mag()*cosalpha1; 
//  float pL2 = v_minus.Mag()*cosalpha2;
//  float Alpha = (pL1-pL2)/(pL1+pL2);
//  //float pT_rel = v_plus.Mag()*sqrt(1.-cosalpha1*cosalpha1);
//
//  return Alpha;
//
//} // alphaCal

EL::StatusCode XFourMuonAnalysis :: fourMuonSel2(){

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
      ATH_MSG_ERROR("dofourMuonSels() Quadruplet without 2 muon links! Something is wrong!");
      return StatusCode::FAILURE;
    }

    if( !doOSonly || getCharge( quad ) == 0 ) good_quads_os.push_back(quad);
  }

  if(m_msgLevel==MSG::DEBUG) ATH_MSG_INFO("dofourMuonSel2() Number of good quads = " << good_quads_os.size()  );
  HM->Var["nQuadsOS"]=good_quads_os.size();

  //std::cout<<__LINE__<<std::endl;

  m_good_qmus.clear();  // good four-muons
  // Loop through the quad
  double min_quad_chi2=1e9;
  for(auto quad: good_quads_os) {
    std::vector<const xAOD::Muon*> m_muon;
    m_muon.clear();

    HM->ClearCutflowFlag("Muon");

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
      HM->FillCutflow("Muon");
      continue;
    }
    m_muon.push_back(Muon0);
    m_muon.push_back(Muon1);
    m_muon.push_back(Muon2);
    m_muon.push_back(Muon3);

    std::vector<const xAOD::Muon*> mus_Pre, mus_CB, mus_nonCB, mus_ST;
    std::vector<const xAOD::Muon*> mus_CBxy, mus_STxy;
    std::vector<const xAOD::Muon*> mus_All, mus_Loose, mus_Medium, mus_Tight, mus_LowPt;
    m_good_mus.clear();

  //std::cout<<__LINE__<<std::endl;
    // Start the muon loop (4 muons)
    for(auto mu: m_muon){
       HM->Cutflow_flag_tmp["Muon"]["AllMu"]=1;
       if(mu->muonType()==1) {HM->FillCutflow("Muon"); continue;}
       //calibration
       xAOD::Muon* mu_corr=NULL;
       CP_CHECK("fourMuonSel()", m_muonCalibrationAndSmearingTool->correctedCopy(*mu, mu_corr));
       m_muonsMap[mu] = mu_corr;
       bool passedIDCuts = false;
       bool accepted_Loose = false, accepted_Medium = false, accepted_Tight = false, accepted_LowPt = false;
       xAOD::Muon::Quality muonQuality;
       if(m_muonSelectionToolLoose) {
         if(m_doMuonCorr) {
           passedIDCuts = m_muonSelectionToolLowPt->passedIDCuts(*mu_corr);//ID Cuts should be already included in next cut
           muonQuality  = m_muonSelectionToolLowPt->getQuality(*mu_corr);

           accepted_Loose = m_muonSelectionToolLoose->accept(*mu_corr);
           accepted_Medium = m_muonSelectionToolMedium->accept(*mu_corr);
           accepted_LowPt = m_muonSelectionToolLowPt->accept(*mu_corr);
         } else {
           passedIDCuts = m_muonSelectionToolLowPt->passedIDCuts(*mu);//ID Cuts should be already included in next cut
           muonQuality  = m_muonSelectionToolLowPt->getQuality(*mu);

           accepted_Loose = m_muonSelectionToolLoose->accept(*mu);
           accepted_Medium = m_muonSelectionToolMedium->accept(*mu);
           accepted_LowPt = m_muonSelectionToolLowPt->accept(*mu);
         }
       }
       // No ID cut at all
       //if(passedIDCuts) HM->Cutflow_flag_tmp["Muon"]["ID"]=1;
       HM->Cutflow_flag_tmp["Muon"]["ID"]=1;
       // track
       const xAOD::TrackParticle * trk = NULL;
       const TrkLink& Link = mu->inDetTrackParticleLink();

       if(Link.isValid()) trk = *Link;

  //std::cout<<__LINE__<<std::endl;
       if(trk != NULL) {

         if( trk->pt() > 2.5e3 && fabs(trk->eta()) < 2.5) mus_Pre.push_back(mu);

         // pt/eta cut
         if( trk->pt() > 3.e3) HM->Cutflow_flag_tmp["Muon"]["Pt"]=1;
         if( fabs(trk->eta()) < 2.5) HM->Cutflow_flag_tmp["Muon"]["Eta"]=1;

         // charge check
         if( trk->charge() == mu->charge() ) HM->Cutflow_flag_tmp["Muon"]["Charge"]=1;
         // impact parameters
         float z0SinTheta = (trk->z0()+trk->vz()-m_pv->z())*sin(trk->p4().Theta());
         if(fabs(z0SinTheta)<1) HM->Cutflow_flag_tmp["Muon"]["Z0"]=1;
         float d0Sig = xAOD::TrackingHelpers::d0significance(trk, m_eventInfo->beamPosSigmaX(), m_eventInfo->beamPosSigmaY(), m_eventInfo->beamPosSigmaXY() );
         if(fabs(d0Sig)<6) HM->Cutflow_flag_tmp["Muon"]["D0"]=1;
       }
       HM->FillCutflowFlag("Muon");

       if(HM->Cutflow_flag["Muon"]["D0"]==1) {
         mus_All.push_back(mu);
         if(accepted_Loose) mus_Loose.push_back(mu);
         if(accepted_Medium) mus_Medium.push_back(mu);
         if(accepted_Tight) mus_Tight.push_back(mu);
         if(accepted_LowPt) mus_LowPt.push_back(mu);

         // XY's muon definition
         if(mu->author()==xAOD::Muon::Author::MuidCo || mu->author()==xAOD::Muon::Author::STACO) mus_CBxy.push_back(mu);
         if(mu->author()==xAOD::Muon::Author::MuTagIMO || mu->author()==xAOD::Muon::Author::MuGirl) mus_STxy.push_back(mu);
       }
         
       HM->FillCutflow("Muon");
    }      
    std::sort(mus_All.begin(), mus_All.end(), sort_mu_trk_pt);
    std::sort(mus_Loose.begin(), mus_Loose.end(), sort_mu_trk_pt);
    std::sort(mus_Medium.begin(), mus_Medium.end(), sort_mu_trk_pt);
    std::sort(mus_LowPt.begin(), mus_LowPt.end(), sort_mu_trk_pt); 
    std::sort(mus_CB.begin(), mus_CB.end(), sort_mu_trk_pt); 
    std::sort(mus_ST.begin(), mus_ST.end(), sort_mu_trk_pt);
    std::sort(mus_CBxy.begin(), mus_CBxy.end(), sort_mu_trk_pt);
    std::sort(mus_STxy.begin(), mus_STxy.end(), sort_mu_trk_pt); 

    // select four good muons (baseline)
    //---------------------------------
    for(int im=0; im<(int)mus_All.size(); im++) {
      auto tmp_mu=mus_All.at(im);
      if(std::find(m_good_mus.begin(), m_good_mus.end(), tmp_mu) == m_good_mus.end()) m_good_mus.push_back(tmp_mu);
    }
    if(m_good_mus.size()>=2 && mus_Pre.size()>=4) HM->Cutflow_flag_tmp["Event"]["FourMuonPre"]=1;
    if(!(m_good_mus.size()>=4)) continue;

    // Muon quality counting
    // ------------------
    int nQLoose=0, nQMedium=0;
    int nQLowPt=0;
    int nQCB=0, nQST=0;
    int nQCBxy=0, nQSTxy=0;

    int nPt4=0;
    for(auto mu: m_good_mus) {
      // track pt cut
      const xAOD::TrackParticle * trk = NULL;
      const TrkLink& Link = mu->inDetTrackParticleLink();
      if(Link.isValid()) trk = *Link;
      if(trk != NULL && trk->pt()>4.e3) nPt4 +=1;

      if(mu->muonType()==0) nQCB ++;
      if(mu->muonType()==2) nQST ++;
      if(mu->author()==xAOD::Muon::Author::MuidCo || mu->author()==xAOD::Muon::Author::STACO) nQCBxy ++;
      if(mu->author()==xAOD::Muon::Author::MuTagIMO || mu->author()==xAOD::Muon::Author::MuGirl) nQSTxy ++;
      if(std::find(mus_Medium.begin(), mus_Medium.end(), mu) != mus_Medium.end()) nQMedium ++;
      if(std::find(mus_LowPt.begin(), mus_LowPt.end(), mu) != mus_LowPt.end()) nQLowPt ++;
      if(std::find(mus_Loose.begin(), mus_Loose.end(), mu) != mus_Loose.end()) nQLoose ++;
    }

    //if(!(m_good_mus.size()>=2 && mus_Pre.size()>=4)) continue;
    // two leading muons pt > 4 GeV
    //if(nPt4<2) continue;

    if(!(nQLowPt>=3)) continue;
    cand_quads_os_chi2.push_back(quad);
    cand_dionia_os_chi2[quad]=good_dionia_os[quad];


    //== select the quad with smallest chi2
    double tmp_chi2 = getChi2(quad);
    if(tmp_chi2 < min_quad_chi2) {
       min_quad_chi2=tmp_chi2;
       m_cand_quad=quad;

       HM->Var["nPt4"]=nPt4;
       HM->Var["nQMedium"]=nQMedium;
       HM->Var["nQLowPt"]=nQLowPt;
       HM->Var["nQLoose"]=nQLoose;
    }
  //std::cout<<__LINE__<<std::endl;
  }

  //std::cout<<__LINE__<<std::endl;
  if(m_cand_quad) {
  //std::cout<<__LINE__<<std::endl;
    HM->Cutflow_flag_tmp["Event"]["FourMuon"]=1;
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
      Warning("XFourMuonAnalysis::fourMuonSel2()", "Missing Four muons in CandQuad");
      return EL::StatusCode::SUCCESS;
    }
  //std::cout<<__LINE__<<std::endl;
    m_good_qmus.push_back(Muon0);
    m_good_qmus.push_back(Muon1);
    m_good_qmus.push_back(Muon2);
    m_good_qmus.push_back(Muon3);
  //std::cout<<__LINE__<<std::endl;
  }
  //std::cout<<__LINE__<<std::endl;

  return EL::StatusCode::SUCCESS;
}
