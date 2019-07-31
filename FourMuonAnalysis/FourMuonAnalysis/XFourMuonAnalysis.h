#ifndef FourMuonAnalysis_XFourMuonAnalysis_H
#define FourMuonAnalysis_XFourMuonAnalysis_H

#include "TH1.h"
#include "TH2.h"

#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
// EDM includes (which rootcint doesn't like)
#ifndef __MAKECINT__

#ifndef XAODMUON_ELECTRONCONTAINER_H
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/ElectronAuxContainer.h"
#endif

#ifndef XAODMUON_PHOTONCONTAINER_H
#include "xAODEgamma/PhotonContainer.h"
#include "xAODEgamma/PhotonAuxContainer.h"
#endif

//#ifndef XAODMUON_JETCONTAINER_H
//#include "xAODJet/JetContainer.h"
//#include "xAODJet/JetAuxContainer.h"
//#endif

#ifndef XAODMUON_MUONCONTAINER_H
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"
#endif

//#ifndef XAODMUON_TAUCONTAINER_H
//#include "xAODTau/TauJetContainer.h"
//#include "xAODTau/TauJetAuxContainer.h"
//#endif

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"

#include "xAODEventInfo/EventInfo.h"

#include "xAODTruth/TruthEventContainer.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthVertexContainer.h"

#include "xAODTracking/VertexContainer.h"

// Tools
#include "GoodRunsLists/GoodRunsListSelectionTool.h"
#include "AssociationUtils/OverlapRemovalTool.h"

#include "xAODBPhys/BPhysHelper.h"

#endif // not __MAKECINT__

#include "FourMuonAnalysis/HistoManager.h"

#define EL_RETURN_CHECK( CONTEXT, EXP )                              \
  if (EXP !=  EL::StatusCode::SUCCESS) {                      \
    Error( CONTEXT, "Failed to execute: %s. Exiting.", #EXP); \
    return EL::StatusCode::FAILURE;                           \
  }

#define TOOL_CHECK( CONTEXT, EXP )                            \
  if (!EXP.isSuccess()) {                                     \
    Error( CONTEXT, "Failed to execute: %s. Exiting.", #EXP); \
    return EL::StatusCode::FAILURE;                           \
  }

#define CP_CHECK( CONTEXT, EXP)                                               \
  if (EXP ==  CP::CorrectionCode::Error) {                                            \
    Error( CONTEXT, "Failed to execute: %s. Exiting.", #EXP);                         \
    return EL::StatusCode::FAILURE;                                                   \
  }

const float GeV = 0.001;
const float M_MU = 105.6583715;
const float M_EL = 0.510999; // 0.5109989461(31) MeV
const float M_UPSI = 9460.30;
const float M_UPSI2S = 10023.26;
const float M_UPSI3S = 10355.20;

// typedefs
typedef ElementLink<xAOD::MuonContainer> MuonLink;
typedef std::vector<MuonLink> MuonLinkVector;

typedef ElementLink<xAOD::TrackParticleContainer> TrkLink;
typedef ElementLink<xAOD::VertexContainer> VertexLink;

// dionia pair
typedef std::pair<const xAOD::Vertex*, const xAOD::Vertex*> OniaPair;
typedef std::vector<OniaPair> OniaPairVec;
typedef std::map<const xAOD::Vertex*, OniaPairVec> DiOnia;

// Forward declarations
namespace TrigConf {
  class xAODConfigTool;
}
namespace Trig {
  class TrigDecisionTool;
}
namespace CP {
  class MuonCalibrationAndSmearingTool;
  class MuonSelectionTool;
  class MuonEfficiencyScaleFactors;
  class IsolationSelectionTool;
}

class XFourMuonAnalysis : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;



  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!

  xAOD::TEvent * m_event; //!
  long m_maxEvent; //!
  long m_eventCounter; //!

  const xAOD::EventInfo* m_eventInfo ; //!
  const xAOD::ElectronContainer*  m_electrons; //!
  const xAOD::MuonContainer*      m_muons;     //!
  const xAOD::Vertex*             m_pv; //!
  // muons after pt correction
  xAOD::MuonContainer*      m_muonsCorr;     //!
  // map for corrected muon and original muon
  std::map<const xAOD::Muon*, xAOD::Muon* > m_muonsMap; //!

  // candidate quad
  const xAOD::Vertex *m_cand_quad; //!
  // vertex containers retrieved from TEvent
  const xAOD::VertexContainer *m_pairs, *m_quads, *m_b1onias; //!
  // good muons
  std::vector<const xAOD::Muon*> m_good_mus, m_good_qmus, m_good_pmus; //!
  // good vertices
  std::vector<const xAOD::Vertex*> m_good_pairs, m_good_pairs_os, m_good_upsi; //!
  std::vector<const xAOD::Vertex*> m_good_b1onias;//!
  // onia pairs
  OniaPairVec m_onia_pairs; //!

  // this is a standard constructor
  XFourMuonAnalysis ();
  ~XFourMuonAnalysis ();

  // histogram handler
  HistoManager *HM; //!
 
  // global settings
  bool m_debug; //!
  // using Xiangyang's muon defition
  // 0: D2-D10, 1: D11, 2: D0 (4Loose)
  int m_doxy; //!
  // using Bphy1 or Bphy4 DAOD
  int m_dobphy; //!
  MSG::Level m_msgLevel; //!
  bool m_doMuonCorr; //!
  bool m_useQuad; //!

  // CP tools
  GoodRunsListSelectionTool * m_grl; //!
  TrigConf::xAODConfigTool *m_trigConfigTool; //!
  Trig::TrigDecisionTool *m_trigDecTool; //!;
  std::vector<std::string> m_triggerList;

  // calibration tools
  CP::MuonCalibrationAndSmearingTool * m_muonCalibrationAndSmearingTool;
  CP::MuonSelectionTool  *m_muonSelectionToolLoose;
  CP::MuonSelectionTool  *m_muonSelectionToolMedium;
  //CP::MuonSelectionTool  *m_muonSelectionToolTight;
  CP::MuonSelectionTool  *m_muonSelectionToolLowPt;
  CP::MuonEfficiencyScaleFactors * m_muonEfficiencyScaleFactorsTTVA;
  CP::MuonEfficiencyScaleFactors * m_muonEfficiencyScaleFactorsLoose;
  CP::MuonEfficiencyScaleFactors * m_muonEfficiencyScaleFactorsMedium;
  CP::MuonEfficiencyScaleFactors * m_muonEfficiencyScaleFactorsTight;

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // initialze events
  EL::StatusCode initializeEvent();
  // initialze tools
  EL::StatusCode initializeTools();

  // Muon selection
  EL::StatusCode fourMuonSel();
  EL::StatusCode fourMuonSel2 ();

  // Onia selection
  EL::StatusCode doOniaSel ();

  // DiOnia selection
  EL::StatusCode doDiOniaSel ();
  EL::StatusCode doEvaSel ();

  // Upsi+mm selection
  EL::StatusCode doUpsiMMSel ();

  // Bphy1 Onia selection
  EL::StatusCode doB1OniaSel ();


  // calculate alpha for O1, O2, X->O1 O2, in Eva's selection
  float alphaCal (TVector3 v_plus, TVector3 v_minus);
  // match Quad with four good muons
  bool matchFourMuon(const xAOD::Vertex* quad, std::vector<const xAOD::Muon*> good_mus);
  // match DiOnia(vertex) with two Onias
  OniaPairVec matchOnia(std::string quadCombinationCode, std::vector<const xAOD::Vertex*> good_pairs);
  // match four-muon with two Onias
  OniaPairVec matchOnia(std::vector<const xAOD::Muon*> good_mus, std::vector<const xAOD::Vertex*> good_pairs);
  // apply pt cut for onia constituent tracks
  bool passPairTrkPt(const xAOD::Vertex* pair, float ptCut=0, float etaCut=2.5);
  // get constituent muons (pair)
  void getConstiMuons(const xAOD::Vertex* quad, const xAOD::Muon*& mu0, const xAOD::Muon*& mu1, const xAOD::Muon*& mu2, const xAOD::Muon*& mu3);
  // get constituent muons
  void getConstiMuons(const xAOD::Vertex* pair, const xAOD::Muon*& mu0, const xAOD::Muon*& mu1);
  // match pair/quad combination code
  bool matchCC(const xAOD::Vertex* pair, std::string cc);
  // get CC of Onia pairs
  std::string getCC_OniaPair(OniaPairVec onia_pairs);
  std::string getQC_OniaPair(OniaPairVec onia_pairs);
  // get trk index of the Onia
  std::vector<int> getTrkInd(const xAOD::Vertex* pair);
  // relative index of the muons inside the four muons from a Quad
  std::vector<int> getTrkInd(const xAOD::Vertex* pair, const xAOD::Vertex* quad);
  // relative index of the muons inside the four muons vector
  std::vector<int> getTrkInd(const xAOD::Vertex* pair, std::vector<const xAOD::Muon*> good_mus);

  // this is needed to distribute the algorithm to the workers
  ClassDef(XFourMuonAnalysis, 1);
};

// sort particle containter by pt
#ifndef __SORTPT__
#define __SORTPT__
static bool sort_par_pt(const xAOD::IParticle* parA, const xAOD::IParticle* parB) {
  return parA->pt() > parB->pt();
}
static bool sort_mu_trk_pt(const xAOD::Muon* parA, const xAOD::Muon* parB) {
  return (*(parA->inDetTrackParticleLink()))->pt() > (*(parB->inDetTrackParticleLink()))->pt();
}
#endif

#endif
