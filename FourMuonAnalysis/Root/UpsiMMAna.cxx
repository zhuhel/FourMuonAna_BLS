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

EL::StatusCode XFourMuonAnalysis :: doUpsiMMSel () {

  // Upsilon+mm selection
  //---------------------------------
  // find Upsilon candidate: best chi2
  const xAOD::Vertex* cand_upsi=0;
  HM->Var["nUpsi"]=m_good_upsi.size();
  double min_chi2_upsi=9e9;
  for(auto pair: m_good_upsi) {
    double tmp_chi2=getChi2(pair);     
    if(tmp_chi2<min_chi2_upsi) {min_chi2_upsi=tmp_chi2; cand_upsi=pair;}
  } 
  if(!cand_upsi)
        return StatusCode::FAILURE;

  HM->Cutflow_flag_tmp["UpsiMMEvent"]["Upsi"]=1;

  // loop all quads and match with the Upsilon candidate
  const xAOD::VertexContainer* quads = 0;
  EL_RETURN_CHECK( "retrieve_BPHY4Quads()", m_event->retrieve(quads, "BPHY4Quads"));
  ATH_MSG_INFO("doDiOniaSel() Number of quads = " << quads->size()  );
  xAOD::VertexContainer::const_iterator quadItr = quads->begin();
  std::vector<const xAOD::Vertex*> good_upsimm;

  double mass_upsi = getMass(cand_upsi, 0);
  double chi2_upsi = getChi2(cand_upsi);

  HM->VVar["MassUpsi"]->push_back(mass_upsi*1e-3);
  HM->VVar["Chi2Upsi"]->push_back(log10(chi2_upsi));

  //==== muon & trk from Upsilon
  const MuonLinkVector MuonLinks_upsi = cand_upsi->auxdataConst<MuonLinkVector>("MuonLinks");
  const MuonLink& MuonLink0_upsi = MuonLinks_upsi[0];
  const MuonLink& MuonLink1_upsi = MuonLinks_upsi[1];

  const xAOD::Muon * Muon0_upsi = NULL;
  const xAOD::Muon * Muon1_upsi = NULL;
  if(MuonLink0_upsi.isValid()) Muon0_upsi = *MuonLink0_upsi;
  if(MuonLink1_upsi.isValid()) Muon1_upsi = *MuonLink1_upsi;

  const xAOD::TrackParticle * Track0_upsi = NULL;
  const xAOD::TrackParticle * Track1_upsi = NULL;
  const TrkLink& Link0_upsi = Muon0_upsi->inDetTrackParticleLink();
  const TrkLink& Link1_upsi = Muon1_upsi->inDetTrackParticleLink();
  if(Link0_upsi.isValid()) Track0_upsi = *Link0_upsi;
  if(Link1_upsi.isValid()) Track1_upsi = *Link1_upsi;

  // Helper Tool
  xAOD::BPhysHelper bVtx(cand_upsi);
  TLorentzVector Muon_upsi_0_RefTrack = bVtx.refTrk(0, M_MU);
  TLorentzVector Muon_upsi_1_RefTrack = bVtx.refTrk(1, M_MU);

  // loop quads, to find if any contains the Upsilon candidate
  for (; quadItr!=quads->end(); ++quadItr) {
    auto quad = *quadItr;

    bool pass_cb=false, pass_trkkine=false, pass_pt=false, pass_mass2=false;

    if ((*quadItr)->auxdataConst<MuonLinkVector>("MuonLinks").size()!=4) {
      ATH_MSG_ERROR("doDiOniaSel() Quadruplet without 2 muon links! Something is wrong!");
      return StatusCode::FAILURE;
    }

    HM->ClearCutflowFlag("UpsiMM");
    HM->Cutflow_flag_tmp["UpsiMM"]["AllUpsiMM"]=1;
    std::string quadCombinationCode=quad->auxdataConst<std::string>("CombinationCode");
    std::string upsiCC=getCC(cand_upsi);
    int upsi_trk_ind1=-1, upsi_trk_ind2=-1;
    //== should contain the Upsilon candidate
    if( getCharge( quad ) == 0 ) {
      HM->Cutflow_flag_tmp["UpsiMM"]["UpsiMMQ"]=1;
      if(matchUpsi(quadCombinationCode, upsiCC, upsi_trk_ind1, upsi_trk_ind2)) {
        HM->Cutflow_flag_tmp["UpsiMM"]["MatchUpsi"]=1;
        // find the other two muons&trks, other than Upsilon constituents
        const MuonLinkVector MuonLinks = quad->auxdataConst<MuonLinkVector>("MuonLinks");
        const xAOD::Muon * Muon_upsi_0 = NULL;
        const xAOD::Muon * Muon_upsi_1 = NULL;
        const xAOD::Muon * Muon_2 = NULL;
        const xAOD::Muon * Muon_3 = NULL;

        const xAOD::TrackParticle * Track_upsi_0 = NULL;
        const xAOD::TrackParticle * Track_upsi_1 = NULL;
        const xAOD::TrackParticle * Track_2 = NULL;
        const xAOD::TrackParticle * Track_3 = NULL;
        for(int ind=0; ind<MuonLinks.size(); ind++) {
          const MuonLink& MuonLink_tmp = MuonLinks[ind];
          const xAOD::Muon * Muon_tmp = NULL;
          if(MuonLink_tmp.isValid()) Muon_tmp = *MuonLink_tmp;
          const TrkLink& Link_tmp = Muon_tmp->inDetTrackParticleLink();
          const xAOD::TrackParticle * Track_tmp = NULL;
          if(Link_tmp.isValid()) Track_tmp = *Link_tmp;

          if(ind==upsi_trk_ind1)      { Muon_upsi_0 = Muon_tmp; Track_upsi_0 = Track_tmp;}
          else if(ind==upsi_trk_ind2) { Muon_upsi_1 = Muon_tmp; Track_upsi_1 = Track_tmp;}
          else {
            if(!Muon_2 && !Track_2) {
              Muon_2 = Muon_tmp; Track_2 = Track_tmp;
            } else {
              Muon_3 = Muon_tmp; Track_3 = Track_tmp;
            }
          }
        }
        if(Muon_upsi_0 == NULL || Muon_upsi_1 == NULL || Muon_2 == NULL || Muon_3 == NULL) {
          HM->FillCutflow("UpsiMM");
          continue;
        }
        HM->Cutflow_flag_tmp["UpsiMM"]["MuonMatching"]=1;

        if(Track_upsi_0 == NULL || Track_upsi_1 == NULL || Track_2 == NULL || Track_3 == NULL) {
          HM->FillCutflow("UpsiMM");
          continue;
        }
        HM->Cutflow_flag_tmp["UpsiMM"]["TrkMatching"]=1;

        //== track kinematics
        pass_trkkine=true;
        if(Track_upsi_0->pt()<2.5e3 || Track_upsi_1->pt()<2.5e3 || Track_2->pt()<2.5e3 || Track_3->pt()<2.5e3)
          pass_trkkine=false;
        if(fabs(Track_upsi_0->eta())>2.5 || fabs(Track_upsi_1->eta())>2.5 || fabs(Track_2->eta())>2.5 || fabs(Track_3->eta())>2.5)
          pass_trkkine=false;

        TLorentzVector tlv_ll34 = Track_2->p4() + Track_3->p4();

        // cut on the other muons
        if(Track_2->pt()>3.e3 && Track_3->pt()>3.e3) pass_pt=true;
        if(tlv_ll34.M()>2.e3) pass_mass2=true;
        
        if(pass_pt) HM->Cutflow_flag_tmp["UpsiMM"]["Pt"]=1;
        if(pass_mass2) HM->Cutflow_flag_tmp["UpsiMM"]["Mass2"]=1;
        if(pass_pt && pass_mass2) {good_upsimm.push_back(quad);}
      } // matchUpsi
    } // quad Charge

    HM->FillCutflow("UpsiMM");
  } // loop quads

  // if more than one upsimm, choose the one with smallest chi2
  const xAOD::Vertex* cand_upsimm=0;
  HM->Var["nUpsiMM"]=good_upsimm.size();
  double min_chi2_upsimm=9e9;
  float m_upsimm=-1;
  //for(auto quad: good_upsimm) 
  for(int ind=0; ind<good_upsimm.size(); ind++) {
    auto quad=good_upsimm.at(ind);
    double tmp_chi2=getChi2(quad);     
    if(tmp_chi2<min_chi2_upsimm) {min_chi2_upsimm=tmp_chi2; cand_upsimm=quad;}
  } 
  
  if(!cand_upsimm) 
    return EL::StatusCode::SUCCESS;

  HM->Cutflow_flag_tmp["UpsiMMEvent"]["Cand"]=1;
  double mass_upsimm = getMass(cand_upsimm, 1);
  double chi2_upsimm = getChi2(cand_upsimm);

  HM->VVar["MassUpsiMM"]->push_back(mass_upsimm*1e-3);
  HM->VVar["Chi2UpsiMM"]->push_back(log10(chi2_upsimm));

  // kinematic plots
  std::string quadCombinationCode=cand_upsimm->auxdataConst<std::string>("CombinationCode");
  std::string upsiCC=getCC(cand_upsi);
  int upsi_trk_ind1=-1, upsi_trk_ind2=-1;
  matchUpsi(quadCombinationCode, upsiCC, upsi_trk_ind1, upsi_trk_ind2);

  const MuonLinkVector MuonLinks = cand_upsimm->auxdataConst<MuonLinkVector>("MuonLinks");
  const xAOD::Muon * Muon_upsi_0 = NULL;
  const xAOD::Muon * Muon_upsi_1 = NULL;
  const xAOD::Muon * Muon_2 = NULL;
  const xAOD::Muon * Muon_3 = NULL;

  const xAOD::TrackParticle * Track_upsi_0 = NULL;
  const xAOD::TrackParticle * Track_upsi_1 = NULL;
  const xAOD::TrackParticle * Track_2 = NULL;
  const xAOD::TrackParticle * Track_3 = NULL;
  for(int ind=0; ind<MuonLinks.size(); ind++) {
    const MuonLink& MuonLink_tmp = MuonLinks[ind];
    const xAOD::Muon * Muon_tmp = NULL;
    if(MuonLink_tmp.isValid()) Muon_tmp = *MuonLink_tmp;
    const TrkLink& Link_tmp = Muon_tmp->inDetTrackParticleLink();
    const xAOD::TrackParticle * Track_tmp = NULL;
    if(Link_tmp.isValid()) Track_tmp = *Link_tmp;

    if(ind==upsi_trk_ind1)      { Muon_upsi_0 = Muon_tmp; Track_upsi_0 = Track_tmp;}
    else if(ind==upsi_trk_ind2) { Muon_upsi_1 = Muon_tmp; Track_upsi_1 = Track_tmp;}
    else {
      if(!Muon_2 && !Track_2) {
        Muon_2 = Muon_tmp; Track_2 = Track_tmp;
      } else {
        Muon_3 = Muon_tmp; Track_3 = Track_tmp;
      }
    }
  }

  if(Muon_upsi_0 == NULL || Muon_upsi_1 == NULL || Muon_2 == NULL || Muon_3 == NULL)
    ATH_MSG_ERROR("doUpsiMMSel() Could not find constituent muons");
  if(Track_upsi_0 == NULL || Track_upsi_1 == NULL || Track_2 == NULL || Track_3 == NULL)
    ATH_MSG_ERROR("doUpsiMMSel() Could not find constituent tracks");

  HM->VVar["Upsi1TrkPt"]->push_back(Track_upsi_0->pt()*1e-3);
  HM->VVar["Upsi1TrkEta"]->push_back(Track_upsi_0->eta());
  HM->VVar["Upsi1TrkPhi"]->push_back(Track_upsi_0->phi());

  HM->VVar["Upsi2TrkPt"]->push_back(Track_upsi_1->pt()*1e-3);
  HM->VVar["Upsi2TrkEta"]->push_back(Track_upsi_1->eta());
  HM->VVar["Upsi2TrkPhi"]->push_back(Track_upsi_1->phi());

  HM->VVar["M3TrkPt"]->push_back(Track_2->pt()*1e-3);
  HM->VVar["M3TrkEta"]->push_back(Track_2->eta());
  HM->VVar["M3TrkPhi"]->push_back(Track_2->phi());

  HM->VVar["M4TrkPt"]->push_back(Track_3->pt()*1e-3);
  HM->VVar["M4TrkEta"]->push_back(Track_3->eta());
  HM->VVar["M4TrkPhi"]->push_back(Track_3->phi());

  TLorentzVector tlv_ll34 = Track_2->p4() + Track_3->p4();
  TLorentzVector tlv_upsimm = Muon_upsi_0_RefTrack + Muon_upsi_1_RefTrack + tlv_ll34;
  TLorentzVector tlv_ll13, tlv_ll24;
  if(Track_upsi_0->charge() + Track_2->charge() ==0) {
    tlv_ll13 = Muon_upsi_0_RefTrack + Track_2->p4();
    tlv_ll24 = Muon_upsi_1_RefTrack + Track_3->p4();
  } else {
    tlv_ll13 = Muon_upsi_0_RefTrack + Track_3->p4();
    tlv_ll24 = Muon_upsi_1_RefTrack + Track_2->p4();
  }

  HM->VVar["MassMM"]->push_back(tlv_ll34.M()*1e-3);
  HM->VVar["MassUpsiMM0"]->push_back(tlv_upsimm.M()*1e-3);

  HM->VVar["MassMM13"]->push_back(tlv_ll13.M()*1e-3);
  HM->VVar["MassMM24"]->push_back(tlv_ll24.M()*1e-3);

  return EL::StatusCode::SUCCESS;
}
