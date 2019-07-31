#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"
#include "xAODCore/AuxInfoBase.h"
#include "xAODCore/ShallowAuxInfo.h"
#include "xAODCore/ShallowCopy.h"
#include "xAODCutFlow/CutBookkeeper.h"
#include "xAODCutFlow/CutBookkeeperContainer.h"

#include <AsgTools/MsgStream.h>
#include <AsgTools/MsgStreamMacros.h>
#include "xAODTracking/TrackParticlexAODHelpers.h"


#include <FourMuonAnalysis/XFourMuonAnalysis.h>
#include <FourMuonAnalysis/Utils.h>

#include "TrigConfxAOD/xAODConfigTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "MuonMomentumCorrections/MuonCalibrationAndSmearingTool.h"
#include "MuonSelectorTools/MuonSelectionTool.h"
#include "MuonEfficiencyCorrections/MuonEfficiencyScaleFactors.h"
#include "xAODTracking/TrackParticlexAODHelpers.h"

#include "TSystem.h"

#define EL_RETURN_CHECK( CONTEXT, EXP )                              \
  if (EXP !=  EL::StatusCode::SUCCESS) {                      \
    Error( CONTEXT, "Failed to execute: %s. Exiting.", #EXP); \
    return EL::StatusCode::FAILURE;                           \
  }

// this is needed to distribute the algorithm to the workers
ClassImp(XFourMuonAnalysis)



XFourMuonAnalysis :: XFourMuonAnalysis () :
  m_eventInfo(nullptr),
  m_electrons(nullptr),
  m_muons(nullptr),
  m_pv(nullptr),
  m_muonsCorr(nullptr),
  m_cand_quad(nullptr),
  m_pairs(nullptr),
  m_quads(nullptr),
  m_debug(false),
  m_doxy(6), // 0: D2-D9, 1: D11, 2: D0, 3: D10 (Medium+All), 4: D12 (LowPt or Medium), 5: D13 (LowPt or Loose), 6: D14 (LowPt+All)
  m_dobphy(4), 
  m_msgLevel(MSG::WARNING),
  m_doMuonCorr(true),
  m_useQuad(false), // if false, for Bphy4 baseline; if true, for Eva's selection
  m_muonCalibrationAndSmearingTool(nullptr),
  m_muonSelectionToolLoose(nullptr),
  m_muonSelectionToolMedium(nullptr),
  //m_muonSelectionToolTight(nullptr),
  m_muonSelectionToolLowPt(nullptr),
  m_muonEfficiencyScaleFactorsTTVA(nullptr),
  m_muonEfficiencyScaleFactorsLoose(nullptr),
  m_muonEfficiencyScaleFactorsMedium(nullptr),
  m_muonEfficiencyScaleFactorsTight(nullptr)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().

  m_good_mus.clear(); m_good_qmus.clear(); m_good_pmus.clear();
  m_good_pairs.clear(); m_good_pairs_os.clear(); m_good_upsi.clear();
  m_onia_pairs.clear();

  m_grl = NULL;
  HM = new HistoManager();
}

XFourMuonAnalysis :: ~XFourMuonAnalysis () {
  if(HM) delete HM; 
  if(m_grl) delete m_grl;
  //if(m_trigConfigTool)                   delete m_trigConfigTool ;
  //if(m_trigDecTool)                      delete m_trigDecTool ;
  if(m_muonSelectionToolLoose)           delete m_muonSelectionToolLoose ;
  if(m_muonSelectionToolMedium)          delete m_muonSelectionToolMedium ;
  //if(m_muonSelectionToolTight)           delete m_muonSelectionToolTight ;
  if(m_muonSelectionToolLowPt)           delete m_muonSelectionToolLowPt ;
  if(m_muonCalibrationAndSmearingTool)   delete m_muonCalibrationAndSmearingTool ;
}

EL::StatusCode XFourMuonAnalysis :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  Info("setupJob()", "Setting up job.");

  // setup for xAOD
  job.useXAOD();
  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init("AnalysisBase").ignore(); // call before opening first file

  // tell EventLoop about our output xAOD:
  EL::OutputStream out("outputLabel","xAODNoMeta");
  job.outputAdd (out);

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode XFourMuonAnalysis :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  Info("histInitialize()", "histInitialize.");

  // Cutflow
  HM->InitCutflow("Muon", "AllMu,Pt,Eta,ID,Charge,Z0,D0");
  if(m_dobphy==1) {
    HM->InitCutflow("B1Onia", "AllB1Onia,Chi2,Mass,B1OniaTrkKine,MuonMatching,TrkMatching");
    HM->InitCutflow("Event", "All,GRL,Onia");
  }
  else if(m_dobphy==4) {
    if(!m_useQuad){
       HM->InitCutflow("Onia", "AllOnia,Chi2,Mass,OniaTrkKine,OniaQ,MuonMatching,TrkMatching,CB,Pt,Chi2Upsi,MassUpsi");
       HM->InitCutflow("DiOnia", "AllDiOnia,MatchQMu,DiOniaQ,OniaMatching,MuonMatching,TrkMatching,OniaChi2,TrkKine,CB,Pt,QCheck");
       HM->InitCutflow("Event", "All,GRL,FourMuonPre,FourMuon,Onia,DiOnia,Cand,MassUp");
       HM->InitCutflow("EventOS", "DiOnia,Cand,MassUp");
       HM->InitCutflow("EventSS", "DiOnia,Cand,MassUp");
    }else{
       HM->InitCutflow("Onia", "AllOnia,Chi2,Mass,OniaTrkKine,OniaQ,MuonMatching,TrkMatching,CB,Pt,Chi2Upsi,MassUpsi");
       HM->InitCutflow("DiOnia", "AllDiOnia,MatchQMu,DiOniaQ,OniaMatching,MuonCB,TrkKine,TrkCharge,OniaChi2,Mass,L2Pt");
       HM->InitCutflow("Event", "All,GRL,DiOnia,Cand,MassUp");
       HM->InitCutflow("EventOS", "Cand,MassUp");
       HM->InitCutflow("EventSS", "Cand,MassUp");
    }     
  }

  // Event info
  HM->InitVar("Run,Event,Weight",1,0,1,0,1); 
  HM->InitVar("muNumber",100,0,100,0,1);

  // Onia kinematics
  if(m_dobphy==1) {
    HM->InitVar("Chi2O_B1Onia",2000,-10,10,1,0);
    HM->InitVar("B1Onia_d0Sig0,B1Onia_d0Sig1",1000,-50,50,1,0);
    HM->InitVar("B1Onia_z0SinTheta0,B1Onia_z0SinTheta1",1000,-5,5,1,0);
    HM->InitVar("MassO_B1Onia,MassO1,MassO2,MassErrO_B1Onia,MassORefTrk_B1Onia,MuMO_B1Onia,TrkMO_B1Onia",2000,0,100,1,0);
    HM->InitVar("PtO_B1Onia,PtORefTrk_B1Onia,MuPtO_B1Onia,TrkPtO_B1Onia",2000,0,100,1,0);
    HM->InitVar("PhiO_B1Onia,PhiORefTrk_B1Onia,MuPhiO_B1Onia,TrkPhiO_B1Onia",2000,-10,10,1,0);
    HM->InitVar("YO_B1Onia,YORefTrk_B1Onia,MuYO_B1Onia,TrkYO_B1Onia",2000,-10,10,1,0);

    //HM->InitVar("TrkPtQ,TrkMQ,MuPtQ,MuMQ",2000,0,100,"Event,EventOS,EventSS","Cand",1,0);

    HM->InitVar("MatchQMu_B1Onia,L1IsCB_B1Onia,L2IsCB_B1Onia",10,0,10,1,1);
    HM->InitVar("L1Pt_B1Onia,L2Pt_B1Onia,L1TrkPt_B1Onia,L2TrkPt_B1Onia,L1RefTrkPt_B1Onia,L2RefTrkPt_B1Onia",2000,0,100,1,0);
    HM->InitVar("L1Eta_B1Onia,L2Eta_B1Onia,L1TrkEta_B1Onia,L2TrkEta_B1Onia,L1RefTrkEta_B1Onia,L2RefTrkEta_B1Onia",600,-3,3,1,0);
    HM->InitVar("L1Phi_B1Onia,L2Phi_B1Onia,L1TrkPhi_B1Onia,L2TrkPhi_B1Onia",600,-3,3,1,0);

    HM->InitVar("L2Pt_B1Onia,L2Pt_B1Onia,L2TrkPt_B1Onia,L2TrkPt_B1Onia",2000,0,100,1,0);
    HM->InitVar("L2Eta_B1Onia,L2Eta_B1Onia,L2TrkEta_B1Onia,L2TrkEta_B1Onia",600,-3,3,1,0);
    HM->InitVar("L2Phi_B1Onia,L2Phi_B1Onia,L2TrkPhi_B1Onia,L2TrkPhi_B1Onia",600,-3,3,1,0);

    HM->InitVar("B1Onia_nCB,B1Onia_nLoose,B1Onia_nMedium,B1Onia_nLowPt",10,0,10,1,1);
    HM->InitVar("B1Onia_isL1Loose,B1Onia_isL1Medium,B1Onia_isL2Loose,B1Onia_isL2Medium",10,0,10,1,1);
    HM->InitVar("B1Onia_isL1LowPt,B1Onia_isL2LowPt",10,0,10,1,1);

    HM->InitVar("B1Oa0,B1Oa0xy,B1Oz0,B1Olxy",20000,-100,100,1,0);
    HM->InitVar("B1Oa0Err,B1Oa0xyErr,B1Oz0Err,B1OlxyErr",20000,-100,100,1,0);

    HM->InitVar("passJpsi_B1Onia,passUpsi_B1Onia",10,0,10,1,1);
  }

  if(m_dobphy==4) {
    // Onia kinematics
    HM->InitVar("Chi2O_Onia_PreSel",1000,0,10,1,0);
    HM->InitVar("MassO_Onia_PreSel,MassErrO_Onia_PreSel,MassORefTrk_Onia_PreSel,MuMO_Onia_PreSel,TrkMO_Onia_PreSel",2000,0,100,1,0);
    HM->InitVar("PtO_Onia_PreSel,PtORefTrk_Onia_PreSel,MuPtO_Onia_PreSel,TrkPtO_Onia_PreSel",2000,0,100,1,0);
    HM->InitVar("PhiO_Onia_PreSel,PhiORefTrk_Onia_PreSel,MuPhiO_Onia_PreSel,TrkPhiO_Onia_PreSel",2000,-10,10,1,0);
    HM->InitVar("YO_Onia_PreSel,YORefTrk_Onia_PreSel,MuYO_Onia_PreSel,TrkYO_Onia_PreSel",2000,-10,10,1,0);

    HM->InitVar("MatchQMu_Onia_PreSel,L1IsCB_Onia_PreSel,L2IsCB_Onia_PreSel",10,0,10,1,1);
    HM->InitVar("L1Pt_Onia_PreSel,L2Pt_Onia_PreSel,L1TrkPt_Onia_PreSel,L2TrkPt_Onia_PreSel",2000,0,100,1,0);
    HM->InitVar("L1Eta_Onia_PreSel,L2Eta_Onia_PreSel,L1TrkEta_Onia_PreSel,L2TrkEta_Onia_PreSel",600,-3,3,1,0);
    HM->InitVar("L1Phi_Onia_PreSel,L2Phi_Onia_PreSel,L1TrkPhi_Onia_PreSel,L2TrkPhi_Onia_PreSel",600,-3,3,1,0);

    HM->InitVar("L2Pt_Onia_PreSel,L2Pt_Onia_PreSel,L2TrkPt_Onia_PreSel,L2TrkPt_Onia_PreSel",2000,0,100,1,0);
    HM->InitVar("L2Eta_Onia_PreSel,L2Eta_Onia_PreSel,L2TrkEta_Onia_PreSel,L2TrkEta_Onia_PreSel",600,-3,3,1,0);
    HM->InitVar("L2Phi_Onia_PreSel,L2Phi_Onia_PreSel,L2TrkPhi_Onia_PreSel,L2TrkPhi_Onia_PreSel",600,-3,3,1,0);

    // DiOnia kinematics
    HM->InitVar("Chi2Q_DiOnia_PreSel",1000,0,10,1,0);
    HM->InitVar("MassQ_DiOnia_PreSel,MassQRefTrk_DiOnia_PreSel",2000,0,100,1,0);
    HM->InitVar("PtQ_DiOnia_PreSel,PtQRefTrk_DiOnia_PreSel",2000,0,100,1,0);
    HM->InitVar("YQ_DiOnia_PreSel,YQRefTrk_DiOnia_PreSel",2000,-10,10,1,0);

    HM->InitVar("Chi2O_DiOnia_PreSel",1000,0,10,1,0);
    HM->InitVar("MassO_DiOnia_PreSel,MassErrO_DiOnia_PreSel,MassORefTrk_DiOnia_PreSel",2000,0,100,1,0);
    HM->InitVar("PtO_DiOnia_PreSel,PtORefTrk_DiOnia_PreSel",2000,0,100,1,0);
    HM->InitVar("YO_DiOnia_PreSel,YORefTrk_DiOnia_PreSel",2000,-10,10,1,0);

    HM->InitVar("Chi2O1_DiOnia_PreSel",1000,0,10,1,0);
    HM->InitVar("MassO1_DiOnia_PreSel,MassO1RefTrk_DiOnia_PreSel",2000,0,100,1,0);
    HM->InitVar("PtO1_DiOnia_PreSel,PtO1RefTrk_DiOnia_PreSel",2000,0,100,1,0);
    HM->InitVar("YO1_DiOnia_PreSel,YO1RefTrk_DiOnia_PreSel",2000,-10,10,1,0);

    HM->InitVar("Chi2O2_DiOnia_PreSel",1000,0,10,1,0);
    HM->InitVar("MassO2_DiOnia_PreSel,MassO2RefTrk_DiOnia_PreSel",2000,0,100,1,0);
    HM->InitVar("PtO2_DiOnia_PreSel,PtO2RefTrk_DiOnia_PreSel",2000,0,100,1,0);
    HM->InitVar("YO2_DiOnia_PreSel,YO2RefTrk_DiOnia_PreSel",2000,-10,10,1,0);

    HM->InitVar("O1TrkPtMu1,O1TrkPtMu2,O2TrkPtMu1,O2TrkPtMu2",2000,0,100,"Event","Cand",1,0);
    HM->InitVar("O1TrkPt,O2TrkPt",2000,0,100,"Event","Cand",1,0);
    HM->InitVar("O1RefTrkPtMu1,O1RefTrkPtMu2,O2RefTrkPtMu1,O2RefTrkPtMu2",2000,0,100,"Event","Cand",1,0);
    HM->InitVar("O1RefTrkPt,O2RefTrkPt",2000,0,100,"Event","Cand",1,0);

    HM->InitVar("L1Pt,L2Pt,L1TrkPt,L2TrkPt",2000,0,100,"Event,EventOS,EventSS","Cand",1,0);
    HM->InitVar("L1Eta,L2Eta,L1TrkEta,L2TrkEta",600,-3,3,"Event,EventOS,EventSS","Cand",1,0);
    HM->InitVar("L1Phi,L2Phi,L1TrkPhi,L2TrkPhi",600,-3,3,"Event,EventOS,EventSS","Cand",1,0);

    HM->InitVar("L3Pt,L4Pt,L3TrkPt,L4TrkPt",2000,0,100,"Event,EventOS,EventSS","Cand",1,0);
    HM->InitVar("L3Eta,L4Eta,L3TrkEta,L4TrkEta",600,-3,3,"Event,EventOS,EventSS","Cand",1,0);
    HM->InitVar("L3Phi,L4Phi,L3TrkPhi,L4TrkPhi",600,-3,3,"Event,EventOS,EventSS","Cand",1,0);

    HM->InitVar("IndO1,IndAC1,IndAC2,IndDC1,IndDC2,IndSS1,IndSS2",10,0,10,1,1); // index of Onia1
    HM->InitVar("MassQ,MassQRefTrk,CandQ_MassRefTrk,MassO,MassErrO,MassCandO,MassO1,MassO2",2000,0,100,"Event,EventOS,EventSS","Cand",1,0);
    HM->InitVar("Chi2Q,Chi2O,Chi2CandO,Chi2O1,Chi2O2",1000,0,10,"Event,EventOS,EventSS","Cand",1,0);
    HM->InitVar("TrkMO",2000,0,100,"Event,EventOS,EventSS","Cand",1,1);
    HM->InitVar("TrkPtO,TrkPtCandO",1000,0,100,"Event,EventOS,EventSS","Cand",1,1);
    HM->InitVar("TrkYO,TrkYCandO,ChargeO,ChargeCandO",200,-10,10,"Event,EventOS,EventSS","Cand",1,1);
    HM->InitVar("TrkdRCandO",1000,0,10,"Event,EventOS,EventSS","Cand",1,1);
    HM->InitVar("CandO_trk_ind1,CandO_trk_ind2",100,0,100,"Event,EventOS,EventSS","Cand",1,1);

    HM->InitVar("Qa0,Qa0xy,Qz0,Qlxy",20000,-100,100,"Event,EventOS,EventSS","Cand",1,0);
    HM->InitVar("Qa0Err,Qa0xyErr,Qz0Err,QlxyErr",20000,-100,100,"Event,EventOS,EventSS","Cand",1,0);

    HM->InitVar("MassDC1,MassDC2,MassAC1,MassAC2,MassSS1,MassSS2",2000,0,100,"Event,EventOS,EventSS","Cand",1,0);
    HM->InitVar("Chi2DC1,Chi2DC2,Chi2AC1,Chi2AC2,Chi2SS1,Chi2SS2",1000,0,10,"Event,EventOS,EventSS","Cand",1,0);

    HM->Init2DHelper("MassQ",1000,0,50,"MassO1,MassDC1,MassAC1,MassSS1",1000,0,50,"Event,EventOS,EventSS","Cand",1,0);
    HM->Init2DHelper("MassQ",1000,0,50,"MassO2,MassDC2,MassAC2,MassSS2",1000,0,50,"Event,EventOS,EventSS","Cand",1,0);

    HM->InitVar("TrkPtQ,TrkMQ,MuPtQ,MuMQ",2000,0,100,"Event,EventOS,EventSS","Cand",1,0);

    HM->InitVar("DC1_trk_ind1,DC1_trk_ind2,DC2_trk_ind1,DC2_trk_ind2",10,0,10,"Event","Cand",1,1);
    HM->InitVar("AC1_trk_ind1,AC1_trk_ind2,AC2_trk_ind1,AC2_trk_ind2",10,0,10,"Event","Cand",1,1);
    HM->InitVar("SS1_trk_ind1,SS1_trk_ind2,SS2_trk_ind1,SS2_trk_ind2",10,0,10,"Event","Cand",1,1);
    HM->InitVar("TrkPt,MuPt",2000,0,100,"Event","Cand",1,1);
    HM->InitVar("TrkEta,MuEta",600,-3,3,"Event","Cand",1,1);
    HM->InitVar("TrkPhi,MuPhi",600,-3,3,"Event","Cand",1,1);
    HM->InitVar("IsCB,MuType,MuAuth",20,0,20,"Event","Cand",1,1);
    HM->InitVar("ChargeQ,ChargeAC1,ChargeAC2,ChargeDC1,ChargeDC2,ChargeSS1,ChargeSS2",20,-10,10,"Event","Cand",1,1);
    HM->InitVar("TrkZ0,TrkD0",20000,-100,100,"Event","Cand",1,1);

    HM->InitVar("O1Alpha,O2Alpha,XAlpha",1000,0,10,1,1);

    // for Eva's selection.
    if(m_useQuad){
      HM->InitVar("Q_L1Pt,Q_L2Pt,Q_L1TrkPt,Q_L2TrkPt,Q_L1RefTrkPt,Q_L2RefTrkPt",2000,0,100,1,1);
      HM->InitVar("Q_L1Eta,Q_L2Eta,Q_L1TrkEta,Q_L2TrkEta,Q_L1RefTrkEta,Q_L2RefTrkEta",600,-3,3,1,1);
      HM->InitVar("Q_L1Phi,Q_L2Phi,Q_L1TrkPhi,Q_L2TrkPhi,Q_L1RefTrkPhi,Q_L2RefTrkPhi",600,-3,3,1,1);
      HM->InitVar("Q_L3Pt,Q_L4Pt,Q_L3TrkPt,Q_L4TrkPt,Q_L3RefTrkPt,Q_L4RefTrkPt",2000,0,100,1,1);
      HM->InitVar("Q_L3Eta,Q_L4Eta,Q_L3TrkEta,Q_L4TrkEta,Q_L3RefTrkEta,Q_L4RefTrkEta",600,-3,3,1,1);
      HM->InitVar("Q_L3Phi,Q_L4Phi,Q_L3TrkPhi,Q_L4TrkPhi,Q_L3RefTrkPhi,Q_L4RefTrkPhi",600,-3,3,1,1);
      HM->InitVar("Q_chi2,Q_charge",1000,0,10,1,0);

      HM->InitVar("CandQ_nCB,CandQ_nLoose,CandQ_nMedium,CandQ_nLowPt",10,0,10,1,1);
      HM->InitVar("CandQ_momentumBalanceSignificance,CandQ_z0SinTheta,CandQ_d0Sig",1000,0,10,1,1);
      HM->InitVar("CandQ_MassO",2000,0,20,1,1);
      HM->InitVar("CandQ_OY",2000,-10,10,1,1);
    }
  }

  // counting
  HM->InitVar("nLooseMu,nMediumMu,nTightMu,nLowPtMu,nCBMu,nSTMu",100,0,100);
  HM->InitVar("nGoodMu,nGoodCBMu,nGoodCBMuGirl,nGoodSTMu,nGoodLooseMu,nGoodMediumMu,nGoodQMu",100,0,100);
  HM->InitVar("nAllMu,nPreMu,nnoIPMu",100,0,100);
  HM->InitVar("nQLoose,nQMedium,nQCBMuGirl,nQCB,nQST",100,0,100);
  HM->InitVar("nQLowPt",100,0,100);
  HM->InitVar("nGoodCBxyMu,nGoodSTxyMu,nQCBxy,nQSTxy",100,0,100);
  HM->InitVar("nPairsOS,nQuadsOS,nQuadCandsOS,nCandOniaPairs",100,0,100);
  HM->InitVar("good_AC,good_DC,good_SS",100,0,100,1);
  HM->InitVar("nPt4",100,0,100);
  HM->InitVar("isJJ,isUU,isUJ",1,0,1);


  // trigger
  // get list of triggers
  std::string triggerfile = gSystem->Getenv("ROOTCOREBIN");
  //triggerfile += "/data/FourMuonAnalysis/trigList_8tev";
  triggerfile += "/data/FourMuonAnalysis/trigList";
  Info("initializeTools()", "Open trigger file: %s",triggerfile.c_str());
  // open the file
  std::ifstream infile;
  infile.open(triggerfile);
  if ( infile.fail() ) {
    Error("initializeTools()","Trigger list file '%s' not found!", triggerfile.c_str());
    return EL::StatusCode::FAILURE;
  }
  std::string line;
  while (!infile.eof()) {
    getline(infile, line);
    // remove all spaces from line
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    // skip empty lines to avoid crash
    if (line.length() == 0) continue;
    // don't run over commented lines
    if (line.find("#") != std::string::npos) continue;
    m_triggerList.push_back(line);
  }

  HM->InitVar("trigList",1000,0,10,1,1,1); // in Tree only, vector of string
  HM->InitVar("trigPass",1000,0,10,1,1,0); // in Tree only, vector of int
  for(auto trig: m_triggerList) {
    HM->InitVar(trig,1000,0,10,0,1); // in Tree only
  }

  // creat histograms defined in InitVar
  HM->CreateHistoMap();

  // write variables defined in InitVar into tree
  HM->AddVarIntoTree();

  // write histograms to output
  for (auto h : HM->getHistos()) {
    wk()->addOutput(h);
  }
  wk()->addOutput(HM->getTree());

  // write cutflow to output
  wk()->addOutput(HM->getCutflow());

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode XFourMuonAnalysis :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  Info("fileExecute()", "fileExecute.");

  //EL_RETURN_CHECK("fileExecute()", initializeMetadata() );

  if(!m_event) m_event = wk()->xaodEvent();

  bool m_isDerivation= true;
  bool m_isMC=true;
  bool m_derivation20_7=true;
  std::string m_derivationName;

  // get the MetaData tree once a new file is opened, with
  TTree *MetaData = dynamic_cast<TTree*>(wk()->inputFile()->Get("MetaData"));
  if (!MetaData) {
    Error("fileExecute()", "MetaData not found! Exiting.");
    return EL::StatusCode::FAILURE;
  }
  MetaData->LoadTree(0);

  //retrieve DxAOD name 
  if(m_isDerivation){
    for (auto branch : *MetaData->GetListOfBranches()){
      std::string brName =  branch->GetName();
      if(brName.find("StreamDAOD") != std::string::npos){
        m_derivationName = brName.substr(brName.find("_")+1);
        break;
      }
    }
    if(m_derivationName == ""){
      Error("fileExecute()","Failed to retrieve derivation name from MetaData! Exiting.");
      return EL::StatusCode::FAILURE;
    }
  }

  m_isDerivation=0;
  if(m_isDerivation){
    // First, check that we have no incomplete bookkeepers.
    // This would indicate that an input file was not completely processed. 
    // -> should be disregarded, since it is not trustworthy!
       
    //don't do this check on data derivations for now!
         
    /*
    const xAOD::CutBookkeeperContainer* incompleteCBC = nullptr;
    TOOL_CHECK("fileExecute()",m_event->retrieveMetaInput(incompleteCBC, "IncompleteCutBookkeepers"));
    if ( incompleteCBC && incompleteCBC->size() != 0 ) {
      if (!m_isMC) {
        for(auto cbk : *incompleteCBC) {
          if (cbk->inputStream() != "unknownStream"){
            Error("fileExecute()","Found incomplete Bookkeepers in data! Check file for corruption. Exiting!");
            return EL::StatusCode::FAILURE;
          }
          else{
            Error("fileExecute()","Found incomplete Bookkeepers in data! Check file for corruption. NOT exiting coming from RAW->ESD step...");
          }
        }
      }else{
        if (!m_derivation20_7) {
          Error("fileExecute()","Found incomplete Bookkeepers in MC! Check file for corruption. Exiting!");
          return EL::StatusCode::FAILURE;
        }
        else {
          Warning("fileExecute()","Found incomplete Bookkeepers in MC! Just a print here as it can happen.");
        }
      }
    }
    */
    // Now, let's find the actual information
    const xAOD::CutBookkeeperContainer* completeCBC = 0;
    std::cout << "OK " << std::endl;
    TOOL_CHECK("fileExecute()",m_event->retrieveMetaInput(completeCBC, "CutBookkeepers") );
    std::cout << "OK 1" << std::endl;

    //AllExecutedEvents
    //number of events in DxAOD: HIGGxxKernel
    const xAOD::CutBookkeeper* DxAODEventsCBK=0;
    std::string kernelName = m_derivationName;
    kernelName+="Kernel";

    // Now, let's actually find the right one that contains all the needed info...
    // Directly from Karsten, see JIRA CXAOD-109
    const xAOD::CutBookkeeper* allEventsCBK = 0;
    int maxCycle = -1;
    for (const auto& cbk: *completeCBC) {
      if(!cbk) continue;
      if (cbk->cycle() > maxCycle && cbk->name() == "AllExecutedEvents" && cbk->inputStream() == "StreamAOD")
      { allEventsCBK = cbk; maxCycle = cbk->cycle(); }
      if ( cbk->name() == kernelName ){
        DxAODEventsCBK = cbk;
      }	
    }

    uint64_t nEventsProcessed  = 0; 
    //double sumOfWeightsSquared = 0; 
    double sumOfWeightsProcessed       = 0;

    uint64_t nEventsDxAOD           = 0;
    //double sumOfWeightsSquaredDxAOD = 0;
    double sumOfWeightsDxAOD        = 0;

    if(!allEventsCBK){
      Error("fileExecute()","Failed to retrieve CutBookkeeper AllExecutedEvents - needed for normalization! Exiting.");
      return EL::StatusCode::FAILURE;
    }
    else{
      nEventsProcessed      = allEventsCBK->nAcceptedEvents();
      //sumOfWeightsSquared   = allEventsCBK->sumOfEventWeightsSquared();
      sumOfWeightsProcessed = allEventsCBK->sumOfEventWeights();
    }

    if(!DxAODEventsCBK){
      Error("fileExecute()","Failed to retrieve CutBookkeeper %s! Keep going, not crucial...", m_derivationName.c_str());
    }
    else{
      nEventsDxAOD             = DxAODEventsCBK->nAcceptedEvents();
      //sumOfWeightsSquaredDxAOD = DxAODEventsCBK->sumOfEventWeightsSquared();
      sumOfWeightsDxAOD        = DxAODEventsCBK->sumOfEventWeights();
    }
        
    // read meta data
    Info("fileExecute()", "Meta data from this file:");
    Info("fileExecute()", "Initial  events         = %lu", nEventsProcessed);
    Info("fileExecute()", "Selected events         = %lu", nEventsDxAOD );
    Info("fileExecute()", "Initial  sum of weights = %.2f", sumOfWeightsProcessed);
    Info("fileExecute()", "Selected sum of weights = %.2f", sumOfWeightsDxAOD);
  }

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode XFourMuonAnalysis :: changeInput (bool firstFile)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  Info("changeInput()", "changeInput.");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode XFourMuonAnalysis :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initialize.");

  // initialize xAOD event
  EL_RETURN_CHECK("initialize()",initializeEvent()    );
  // initialize tools
  EL_RETURN_CHECK("initialize()",initializeTools()    );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode XFourMuonAnalysis::initializeEvent() {

  Info("initializeEvent()", "Initialize event.");

  // get TEvent
  m_event = wk()->xaodEvent();

  // sample name
  TString sampleName = wk()->metaData()->castString("sample_name");
  Info("initialize()", "Sample name = %s", sampleName.Data());
  // as a check, let's see the number of events in our xAOD (long long int)
  Info("initialize()", "Number of events in file   = %lli", m_event->getEntries());
  if(m_maxEvent < 0) m_maxEvent = m_event->getEntries() ;
  Info("initialize()", "Number of events to run on = %li", m_maxEvent);

  // count number of events
  m_eventCounter = 0;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode XFourMuonAnalysis::initializeTools() {

  Info("initializeTools()", "Initialize tools.");

  //===============================================================
  // GRL
  //===============================================================
  Info("initializeTools()", "Initialize GoodRunsListSelectionTool");

  m_grl = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");

  std::vector<std::string> vecStringGRL;
  //vecStringGRL.push_back("$ROOTCOREBIN/data/FourMuonAnalysis/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml");
  vecStringGRL.push_back("$ROOTCOREBIN/data/FourMuonAnalysis/grl_2012_v61.xml");
  vecStringGRL.push_back("$ROOTCOREBIN/data/FourMuonAnalysis/data15_13TeV.periodAllYear_DetStatus-v79-repro20-02_DQDefects-00-02-02_PHYS_StandardGRL_All_Good_25ns.xml");
  vecStringGRL.push_back("$ROOTCOREBIN/data/FourMuonAnalysis/data16_13TeV.periodAllYear_DetStatus-v83-pro20-15_DQDefects-00-02-04_PHYS_StandardGRL_All_Good_25ns.xml");
  vecStringGRL.push_back("$ROOTCOREBIN/data/FourMuonAnalysis/data17_13TeV.periodAllYear_DetStatus-v97-pro21-13_Unknown_PHYS_StandardGRL_All_Good_25ns_Triggerno17e33prim.xml");
  vecStringGRL.push_back("$ROOTCOREBIN/data/FourMuonAnalysis/data18_13TeV.periodAllYear_DetStatus-v102-pro22-04_Unknown_PHYS_StandardGRL_All_Good_25ns_Triggerno17e33prim.xml");
  if(!m_grl->setProperty( "GoodRunsListVec", vecStringGRL).isSuccess())
    Warning("initializeTools()","Problem with GoodRunsListSelectionTool!");

  // If true (default) will ignore result of GRL and will just pass all events
  if(!m_grl->setProperty("PassThrough",false)) Warning("initializeTools()","Problem with GoodRunsListSelectionTool!");

  if(!m_grl->initialize()) {
    Info("initializeTools()", "Problem in Initialize GoodRunsListSelectionTool!");
    return false;
  } else {
    Info("initializeTools()", "Initialize GoodRunsListSelectionTool - Done!");
  }

  //===============================================================
  // CP tools
  //===============================================================

  //std::cout<<__LINE__<<std::endl;

  //xAODConfigTool
  //MSG::Level m_msgLevel(MSG::WARNING);
  m_trigConfigTool = new TrigConf::xAODConfigTool ("xAODConfigTool");
  m_trigConfigTool->msg().setLevel( m_msgLevel );
  EL_RETURN_CHECK("initializeTools()",m_trigConfigTool->initialize());
  ToolHandle<TrigConf::ITrigConfigTool> handle(m_trigConfigTool);
  EL_RETURN_CHECK("initializeTools()",handle->initialize());
  //TrigDecisionTool
  m_trigDecTool = new Trig::TrigDecisionTool("TrigDecisionTool");
  m_trigDecTool->msg().setLevel( m_msgLevel );
  EL_RETURN_CHECK("initializeTools()",m_trigDecTool->setProperty("ConfigTool",handle));
  EL_RETURN_CHECK("initializeTools()",m_trigDecTool->setProperty("TrigDecisionKey","xTrigDecision"));
  EL_RETURN_CHECK("initializeTools()",m_trigDecTool->setProperty("OutputLevel", m_msgLevel));
  EL_RETURN_CHECK("initializeTools()",m_trigDecTool->initialize());

  // initialize the muon selector tool
  MSG::Level m_msgLevel_muSel(MSG::ERROR);
  //---------------------------------------------------
  m_muonSelectionToolLoose = new CP::MuonSelectionTool("MuonSelectionToolLoose");
  m_muonSelectionToolLoose->msg().setLevel( m_msgLevel_muSel );
  TOOL_CHECK("initializeTools()",m_muonSelectionToolLoose->setProperty("MaxEta", 2.7)); //2.7 should be default, but better to state it explicitly if later we want to change
  TOOL_CHECK("initializeTools()",m_muonSelectionToolLoose->setProperty("MuQuality", (int) xAOD::Muon::Quality::Loose));
  //TOOL_CHECK("initializeTools()",m_muonSelectionToolLoose->setProperty( "TurnOffMomCorr", true ));
  TOOL_CHECK("initializeTools()",m_muonSelectionToolLoose->setProperty( "TrtCutOff", true ));
  TOOL_CHECK("initializeTools()",m_muonSelectionToolLoose->initialize());

  m_muonSelectionToolMedium = new CP::MuonSelectionTool("MuonSelectionToolMedium");
  m_muonSelectionToolMedium->msg().setLevel( m_msgLevel_muSel );
  TOOL_CHECK("initializeTools()",m_muonSelectionToolMedium->setProperty("MaxEta", 2.7));
  TOOL_CHECK("initializeTools()",m_muonSelectionToolMedium->setProperty("MuQuality", (int) xAOD::Muon::Quality::Medium));
  //TOOL_CHECK("initializeTools()",m_muonSelectionToolMedium->setProperty( "TurnOffMomCorr", true ));
  TOOL_CHECK("initializeTools()",m_muonSelectionToolMedium->setProperty( "TrtCutOff", true ));
  TOOL_CHECK("initializeTools()",m_muonSelectionToolMedium->initialize());

  m_muonSelectionToolLowPt = new CP::MuonSelectionTool("MuonSelectionToolLowPt");
  m_muonSelectionToolLowPt->msg().setLevel( m_msgLevel_muSel );
  TOOL_CHECK("initializeTools()",m_muonSelectionToolLowPt->setProperty("MaxEta", 2.7));
  TOOL_CHECK("initializeTools()",m_muonSelectionToolLowPt->setProperty("MuQuality", 5));
  //TOOL_CHECK("initializeTools()",m_muonSelectionToolLowPt->setProperty( "TurnOffMomCorr", true ));
  TOOL_CHECK("initializeTools()",m_muonSelectionToolLowPt->setProperty( "TrtCutOff", true ));
  TOOL_CHECK("initializeTools()",m_muonSelectionToolLowPt->initialize());

  // initialize the muon calibration and smearing tool
  //---------------------------------------------------
  m_muonCalibrationAndSmearingTool = new CP::MuonCalibrationAndSmearingTool("MuonCalibrationTool");
  m_muonCalibrationAndSmearingTool->msg().setLevel( m_msgLevel );
  TOOL_CHECK("initializeTools()",m_muonCalibrationAndSmearingTool->initialize());

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode XFourMuonAnalysis :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if( (m_eventCounter % 10000) == 0) {
    Info("execute()", "Event number = %i", m_eventCounter );
  }
  m_eventCounter++;
  //if(m_eventCounter>10) return EL::StatusCode::SUCCESS;

  HM->ClearVariables();
  HM->ClearCutflowFlag();

  // clear map
  for(auto& mu : m_muonsMap) {if(mu.second) delete mu.second;}
  m_muonsMap.clear();

  HM->Cutflow_flag_tmp["Event"]["All"]=1;
  //HM->Cutflow_flag_tmp["UpsiMMEvent"]["All"]=1;

  // retrieve event info
  //---------------------------------
  const xAOD::EventInfo * eventInfoIn = 0;
  EL_RETURN_CHECK( "retrieve_m_eventInfo()", m_event->retrieve(eventInfoIn, "EventInfo"));

  std::pair< xAOD::EventInfo *, xAOD::ShallowAuxInfo *> sc_eventInfo = shallowCopyObject(*eventInfoIn);
  m_eventInfo = sc_eventInfo.first;

  // check if MC or data - agrees with information retrieved from MetaData?
  //------------------------
  //m_isMC is retrieved from the MetaData in AnalysisBase and passed to the EventInfoHandler
  int isMC = static_cast<int>(m_eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION));

  // GRL
  //------------------------
  bool byPassGRL=true;
  int passGRL = true;
  //int LATEST_RUN_GRL = ; // latest run in GRL
  if(!isMC) {
    if (!m_grl->passRunLB(*m_eventInfo)) passGRL = false;
  }
  if(passGRL || byPassGRL /*|| m_eventInfo->runNumber() > LATEST_RUN_GRL*/) {
    HM->Cutflow_flag_tmp["Event"]["GRL"]=1;
  }

  //double eventNumber=m_eventInfo->eventNumber();
  //if(eventNumber!=1146516081 && eventNumber!=1105396224 && eventNumber!=1673384311 && eventNumber!=1514723591 && eventNumber!=1309221731) return EL::StatusCode::SUCCESS;

  // check if the MC sample is interesting to us
  int mc_channel = -1;
  if(isMC) mc_channel = m_eventInfo->mcChannelNumber();

  // MC weight
  HM->Evt_weight = 1.;
  if(isMC) HM->Evt_weight = m_eventInfo->mcEventWeight(0);

  HM->Var["Run"]=m_eventInfo->runNumber();
  HM->Var["Event"]=m_eventInfo->eventNumber();
  HM->Var["Weight"]= HM->Evt_weight;
  HM->Var["muNumber"]=m_eventInfo->averageInteractionsPerCrossing();

  // for debug
  //if(m_debug && HM->Var["Event"] != 2166688009) return EL::StatusCode::SUCCESS;

  // find primary vertex
  //------------------------
  const xAOD::VertexContainer* vertices = 0;
  if( !m_event->retrieve( vertices, "PrimaryVertices" ).isSuccess() )
  {
    Error("execute ()", "Failed to retrieve verteices. Exiting." );
    return EL::StatusCode::FAILURE;
  }
  for ( const auto* const vtx_itr : *vertices )
  {
    if (vtx_itr->vertexType() != xAOD::VxType::VertexType::PriVtx) continue;
    else { m_pv = vtx_itr; break;}
  }

  // trigger
  //------------------------
  // check all triggers
  /*
  auto chainGroups = m_trigDecTool->getChainGroup("HLT.*");
  for(auto &trig : chainGroups->getListOfTriggers()) {
    cout << "  " << trig <<  "  " << m_trigDecTool->isPassed(trig) << endl;
  }
  */
  for(auto trig: m_triggerList) {
    //HM->Var[trig] = static_cast<int>(m_trigDecTool->isPassed(trig, TrigDefs::EF_passedRaw));
    HM->Var[trig] = static_cast<int>(m_trigDecTool->isPassed(trig));
    HM->VVStr["trigList"]->push_back(trig);
    HM->VVar["trigPass"]->push_back(HM->Var[trig]);
  }


  // retrieve objects
  //------------------------
  // Muons
  EL_RETURN_CHECK( "retrieve_Muons()", m_event->retrieve(m_muons, "Muons"));
  if(m_msgLevel==MSG::DEBUG) ATH_MSG_INFO("execute() Number of muons = " << m_muons->size()  );
  // Onia vertex
  if(m_dobphy==4) {
    EL_RETURN_CHECK( "retrieve_BPHY4Pairs()", m_event->retrieve(m_pairs, "BPHY4Pairs"));
    if(m_msgLevel==MSG::DEBUG) ATH_MSG_INFO("execute() Number of pairs = " << m_pairs->size()  );
    // Quad vertex
    EL_RETURN_CHECK( "retrieve_BPHY4Quads()", m_event->retrieve(m_quads, "BPHY4Quads"));
    if(m_msgLevel==MSG::DEBUG) ATH_MSG_INFO("execute() Number of quads = " << m_quads->size()  );
  }
  else if(m_dobphy==1) {
    // Bphy1 Onia vertex
    EL_RETURN_CHECK( "retrieve_BPHY1OniaCandidates()", m_event->retrieve(m_b1onias, "BPHY1OniaCandidates"));
    if(m_msgLevel==MSG::DEBUG) ATH_MSG_INFO("execute() Number of onias = " << m_b1onias->size()  );
  } 
  
  // event selection
  //------------------------
  // muon selection
  if(m_dobphy==4) {
    if (!m_useQuad) fourMuonSel2();
    // Onia selection (including Upsilon selection)
    doOniaSel(); 
    // DiOnia selection
    if (!m_useQuad) doDiOniaSel();
    if (m_useQuad) doEvaSel();
  }
  else if(m_dobphy==1) {
    fourMuonSel(); 
    // Onia selection (Jpsi, Upsi)
    doB1OniaSel(); 
  }

  // Fill histograms
  //------------------------
  HM->FillCutflow();
  HM->FillHistograms();
  if(m_dobphy==4 && !m_useQuad && HM->Cutflow_flag["Event"]["DiOnia"]) HM->FillTree();
  if(m_dobphy==4 && m_useQuad && HM->Cutflow_flag["Event"]["DiOnia"]) HM->FillTree();
  if(m_dobphy==1 && HM->Cutflow_flag["Event"]["Onia"]) HM->FillTree();

  // debug
  if(m_debug && HM->Cutflow_flag["EventOS"]["MassUp"])
    std::cout << int(HM->Var["Run"]) << " " << int(HM->Var["Event"]) << " " << HM->VVar["MassQ"]->at(0) << std::endl;

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode XFourMuonAnalysis :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode XFourMuonAnalysis :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  // write cutflow
  HM->PrintCutflow();
  
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode XFourMuonAnalysis :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode XFourMuonAnalysis :: fourMuonSel () {

  // muon selection
  //---------------------------------
  std::vector<const xAOD::Muon*> mus_Pre, mus_CB, mus_nonCB, mus_ST, mus_noIP;
  std::vector<const xAOD::Muon*> mus_CBxy, mus_STxy;
  std::vector<const xAOD::Muon*> mus_Loose, mus_Medium, mus_Tight, mus_LowPt, mus_All;
  m_good_mus.clear();
  m_good_qmus.clear();  // good four-muons
  m_good_pmus.clear();  // good two-muons

  // create shallow copy
  //std::pair< xAOD::MuonContainer*, xAOD::ShallowAuxContainer* > muons_shallowCopy = xAOD::shallowCopyContainer( *m_muons );
  //m_muonsCorr = muons_shallowCopy.first;

  HM->ClearCutflowFlag("Muon");
  xAOD::MuonContainer::const_iterator muItr = m_muons->begin();
  for (; muItr!=m_muons->end(); ++muItr) {
    auto mu = *muItr;

    HM->Cutflow_flag_tmp["Muon"]["AllMu"]=1; 

    // ignore standalone muons ( |eta|>2.5), since on ID track for SA muons
    if(mu->muonType()==1) {HM->FillCutflow("Muon"); continue;}

    //calibration
    //CP_CHECK("fourMuonSel()", m_muonCalibrationAndSmearingTool->applyCorrection(*mu), m_debug); 
    xAOD::Muon* mu_corr=NULL;
    CP_CHECK("fourMuonSel()", m_muonCalibrationAndSmearingTool->correctedCopy(*mu, mu_corr));
    // create a map: original muon -- corrected muon
    // needed for Quad matching
    m_muonsMap[mu] = mu_corr;

    // pt/eta cut
    //if( mu->pt() > 3.e3) HM->Cutflow_flag_tmp["Muon"]["Pt"]=1;
    //if( fabs(mu->eta()) < 2.5) HM->Cutflow_flag_tmp["Muon"]["Eta"]=1;

    // ID
    bool passedIDCuts = false;
    bool accepted_Loose = false, accepted_Medium = false, accepted_Tight = false, accepted_LowPt = false;
    xAOD::Muon::Quality muonQuality;
    if(m_muonSelectionToolLoose) {
      if(m_doMuonCorr) {
        passedIDCuts = m_muonSelectionToolLowPt->passedIDCuts(*mu_corr);//ID Cuts should be already included in next cut
        accepted_Loose = m_muonSelectionToolLoose->accept(*mu_corr);
        muonQuality = m_muonSelectionToolLoose->getQuality(*mu_corr);

        accepted_Medium = m_muonSelectionToolMedium->accept(*mu_corr);
        accepted_LowPt = m_muonSelectionToolLowPt->accept(*mu_corr);
      } else {
        passedIDCuts = m_muonSelectionToolLowPt->passedIDCuts(*mu);//ID Cuts should be already included in next cut
        accepted_Loose = m_muonSelectionToolLoose->accept(*mu);
        muonQuality = m_muonSelectionToolLoose->getQuality(*mu);

        accepted_Medium = m_muonSelectionToolMedium->accept(*mu);
        accepted_LowPt = m_muonSelectionToolLowPt->accept(*mu);
      }
    }
    // only requing hits
    //if(accepted_LowPt) HM->Cutflow_flag_tmp["Muon"]["ID"]=1;
    //if(accepted_Loose) HM->Cutflow_flag_tmp["Muon"]["ID"]=1;
    //if(passedIDCuts) HM->Cutflow_flag_tmp["Muon"]["ID"]=1;
    //  no ID cut at all !!
    HM->Cutflow_flag_tmp["Muon"]["ID"]=1;

    // track
    const xAOD::TrackParticle * trk = NULL;
    const TrkLink& Link = mu->inDetTrackParticleLink();

    if(Link.isValid()) trk = *Link;

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

    if(mu->muonType()==0 || mu->muonType()==2) HM->Cutflow_flag_tmp["Muon"]["CB"]=1;

    HM->FillCutflowFlag("Muon");
    
    if(HM->Cutflow_flag["Muon"]["Charge"]==1) mus_noIP.push_back(mu);
    if(HM->Cutflow_flag["Muon"]["D0"]==1) {
      mus_All.push_back(mu);
      if(accepted_Loose) mus_Loose.push_back(mu);
      if(accepted_Medium) mus_Medium.push_back(mu);
      if(accepted_Tight) mus_Tight.push_back(mu);
      if(accepted_LowPt) mus_LowPt.push_back(mu);

      //m_good_mus.push_back(mu);
      if(mu->muonType()==0){//Combined 
        mus_CB.push_back(mu);
      } else {
        mus_nonCB.push_back(mu);
      }
      if(mu->muonType()==2){// ST
        mus_ST.push_back(mu);
      }

      // XY's muon definition
      if(mu->author()==xAOD::Muon::Author::MuidCo || mu->author()==xAOD::Muon::Author::STACO) mus_CBxy.push_back(mu);
      if(mu->author()==xAOD::Muon::Author::MuTagIMO || mu->author()==xAOD::Muon::Author::MuGirl) mus_STxy.push_back(mu);
      
      if(m_debug) std::cout << " Muon pt= " << mu->pt() << " author= " << mu->author() << " type= " << mu->muonType() << " isMedium= " << accepted_Medium << std::endl;

       /*
       // test variables in SoftMuR21
       cout << "XXXXXXXXXXXXXXXXX Test variables XXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
       uint8_t value1(-9999), value2(-9999);
       cout << mu->floatParameter(xAOD::Muon::scatteringCurvatureSignificance) << endl; // SCS
       cout << mu->floatParameter(xAOD::Muon::scatteringNeighbourSignificance) << endl; // SNS
       cout << mu->floatParameter(xAOD::Muon::momentumBalanceSignificance) << endl; // momentum balance significance
       cout << mu->isAuthor(xAOD::Muon::MuTagIMO) << endl;
       mu->summaryValue(value1, xAOD::MuonSummaryType::combinedTrackOutBoundsPrecisionHits);
       cout << (int)value1 << endl;
       mu->summaryValue(value2, xAOD::SummaryType::numberOfPrecisionLayers);
       cout << (int)value2 << endl;

       //SNS = mu->floatParameter(xAOD::Muon::scatteringNeighbourSignificance);
       //SCS = mu->floatParameter(xAOD::Muon::scatteringCurvatureSignificance);
       
       std::cout<<xAOD::Muon::Author::MuidCo<<"\t"<<xAOD::Muon::Author::STACO<<"\t"<<xAOD::Muon::Author::MuTagIMO<<std::endl;
       //Sel_SoftMuR21 = ( mu->muonType==0  && passedIDCuts==1 && (fabs(SNS)<3.0 && fabs(SCS)<3.0 && value1=0 && value2>0 && ((mu->author()==1&&fabs(trk->eta())<1.2)||(mu->author==6&&isMuTagIMO==1&&fabs(trk->eta())<1.2))) || (SelectorQuality<2 && fabs(trk->eta())>1.2) )
       */
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
  if(m_doxy==1) {
    for(int im=0; im<(int)mus_CBxy.size(); im++) {
      auto tmp_mu=mus_CBxy.at(im);
      if(std::find(m_good_mus.begin(), m_good_mus.end(), tmp_mu) == m_good_mus.end()) m_good_mus.push_back(tmp_mu);
    }
    for(int im=0; im<(int)mus_STxy.size(); im++) {
      auto tmp_mu=mus_STxy.at(im);
      if(std::find(m_good_mus.begin(), m_good_mus.end(), tmp_mu) == m_good_mus.end()) m_good_mus.push_back(tmp_mu);
    }
  } else if(m_doxy==0) {
    for(int im=0; im<(int)mus_Medium.size(); im++) {
      auto tmp_mu=mus_Medium.at(im);
      if(std::find(m_good_mus.begin(), m_good_mus.end(), tmp_mu) == m_good_mus.end()) m_good_mus.push_back(tmp_mu);
    }
    for(int im=0; im<(int)mus_CB.size(); im++) {
      auto tmp_mu=mus_CB.at(im);
      if(std::find(m_good_mus.begin(), m_good_mus.end(), tmp_mu) == m_good_mus.end()) m_good_mus.push_back(tmp_mu);
    }
    for(int im=0; im<(int)mus_ST.size(); im++) {
      auto tmp_mu=mus_ST.at(im);
      if(std::find(m_good_mus.begin(), m_good_mus.end(), tmp_mu) == m_good_mus.end()) m_good_mus.push_back(tmp_mu);
    }
  } else if(m_doxy==2) {
    for(int im=0; im<(int)mus_Loose.size(); im++) {
      auto tmp_mu=mus_Loose.at(im);
      if(std::find(m_good_mus.begin(), m_good_mus.end(), tmp_mu) == m_good_mus.end()) m_good_mus.push_back(tmp_mu);
    }
  } else if(m_doxy==3) {
    for(int im=0; im<(int)mus_Medium.size(); im++) {
      auto tmp_mu=mus_Medium.at(im);
      if(std::find(m_good_mus.begin(), m_good_mus.end(), tmp_mu) == m_good_mus.end()) m_good_mus.push_back(tmp_mu);
    }
    for(int im=0; im<(int)mus_All.size(); im++) {
      auto tmp_mu=mus_All.at(im);
      if(std::find(m_good_mus.begin(), m_good_mus.end(), tmp_mu) == m_good_mus.end()) m_good_mus.push_back(tmp_mu);
    }
  } else if(m_doxy==6) {
    for(int im=0; im<(int)mus_LowPt.size(); im++) {
      auto tmp_mu=mus_LowPt.at(im);
      if(std::find(m_good_mus.begin(), m_good_mus.end(), tmp_mu) == m_good_mus.end()) m_good_mus.push_back(tmp_mu);
    }
    for(int im=0; im<(int)mus_All.size(); im++) {
      auto tmp_mu=mus_All.at(im);
      if(std::find(m_good_mus.begin(), m_good_mus.end(), tmp_mu) == m_good_mus.end()) m_good_mus.push_back(tmp_mu);
    }
  }

  for(int im=0; im<(int)m_good_mus.size(); im++) {
    if(im>3) break;
    auto mu = m_good_mus.at(im);
    m_good_qmus.push_back(mu);
  } 

  // Muon quality counting
  // ------------------
  int nQLoose=0, nLoose=0;
  int nQMedium=0, nMedium=0;
  int nQLowPt=0, nLowPt=0;
  int nQCB=0, nCB=0;
  int nQCBxy=0, nCBxy=0;
  int nQST=0, nST=0;
  int nQSTxy=0, nSTxy=0;
  int nQMuGirl=0, nMuGirl=0;
  // on good muons
  for(auto mu: m_good_mus) {
    if(mu->muonType()==0) nCB ++;
    if(mu->muonType()==0 && mu->author()==xAOD::Muon::Author::MuGirl) nMuGirl ++;
    if(mu->muonType()==2) nST ++;
    if(mu->author()==xAOD::Muon::Author::MuidCo || mu->author()==xAOD::Muon::Author::STACO) nCBxy ++;
    if(mu->author()==xAOD::Muon::Author::MuTagIMO || mu->author()==xAOD::Muon::Author::MuGirl) nSTxy ++;
    if(std::find(mus_Medium.begin(), mus_Medium.end(), mu) != mus_Medium.end()) nMedium ++;
    if(std::find(mus_LowPt.begin(), mus_LowPt.end(), mu) != mus_LowPt.end()) nLowPt ++;
    if(std::find(mus_Loose.begin(), mus_Loose.end(), mu) != mus_Loose.end()) nLoose ++;
  }
 
  // on good quad muons
  int nPt4=0;
  for(auto mu: m_good_qmus) {
    // track pt cut
    const xAOD::TrackParticle * trk = NULL;
    const TrkLink& Link = mu->inDetTrackParticleLink();
    if(Link.isValid()) trk = *Link;
    if(trk != NULL && trk->pt()>4.e3) nPt4 +=1;

    if(mu->muonType()==0) nQCB ++;
    if(mu->muonType()==0 && mu->author()==xAOD::Muon::Author::MuGirl) nQMuGirl ++;
    if(mu->muonType()==2) nQST ++;
    if(mu->author()==xAOD::Muon::Author::MuidCo || mu->author()==xAOD::Muon::Author::STACO) nQCBxy ++;
    if(mu->author()==xAOD::Muon::Author::MuTagIMO || mu->author()==xAOD::Muon::Author::MuGirl) nQSTxy ++;
    if(std::find(mus_Medium.begin(), mus_Medium.end(), mu) != mus_Medium.end()) nQMedium ++;
    if(std::find(mus_LowPt.begin(), mus_LowPt.end(), mu) != mus_LowPt.end()) nQLowPt ++;
    if(std::find(mus_Loose.begin(), mus_Loose.end(), mu) != mus_Loose.end()) nQLoose ++;
  }

  HM->Var["nPt4"]=nPt4;
  // pre selection: 4 muons (pt>2.5, eta<2.5)
  if(m_good_qmus.size()>=2 && mus_Pre.size()>=4) HM->Cutflow_flag_tmp["Event"]["FourMuonPre"]=1;
  // two leading muons pt > 4 GeV
  //if(nPt4>=2) HM->Cutflow_flag_tmp["Event"]["L2Pt"]=1;
  
  //if(((m_doxy==0 && nQCB>=3 ) || 
  //    (m_doxy==1 && nQCBxy>=3 && nQSTxy<=1) ||
  //    (m_doxy==2 && nQLoose>=4) || 
  //    (m_doxy==3 && nQMedium>=0) ||
  //    (m_doxy==6 && nQLowPt>=3)) && m_good_qmus.size()>=4)
  if(m_good_qmus.size()>=4)  
      HM->Cutflow_flag_tmp["Event"]["FourMuon"]=1;

  if(m_debug) std::cout << " nCB= " << nCB << " nST= " << nST << " nCBxy= " << nCBxy << " nSTxy= " << nSTxy << std::endl;
  if(m_debug) std::cout << " nQCB= " << nQCB << " nQST= " << nQST << " nQCBxy= " << nQCBxy << " nQSTxy= " << nQSTxy << std::endl;

  HM->Var["nLooseMu"]=(int)mus_Loose.size(); 
  HM->Var["nMediumMu"]=(int)mus_Medium.size(); 
  HM->Var["nLowPtMu"]=(int)mus_LowPt.size(); 
  HM->Var["nTightMu"]=(int)mus_Tight.size(); 
  HM->Var["nCBMu"]=(int)mus_CB.size();
  HM->Var["nSTMu"]=(int)mus_ST.size();
  HM->Var["nGoodMu"]=(int)m_good_mus.size(); 
  HM->Var["nAllMu"]=(int)mus_All.size(); 
  HM->Var["nPreMu"]=(int)mus_Pre.size(); 
  HM->Var["nnoIPMu"]=(int)mus_noIP.size(); 
  HM->Var["nGoodQMu"]=(int)m_good_qmus.size(); 
  HM->Var["nGoodCBMu"]=nCB; 
  HM->Var["nGoodCBxyMu"]=nCBxy; 
  HM->Var["nGoodCBMuGirl"]=nMuGirl; 
  HM->Var["nGoodSTMu"]=nST; 
  HM->Var["nGoodSTxyMu"]=nSTxy; 
  HM->Var["nGoodLooseMu"]=nLoose; 
  HM->Var["nGoodMediumMu"]=nMedium; 
  HM->Var["nQCB"]=nQCB;
  HM->Var["nQCBxy"]=nQCBxy;
  HM->Var["nQCBMuGirl"]=nQMuGirl; 
  HM->Var["nQST"]=nQST;
  HM->Var["nQSTxy"]=nQSTxy;
  HM->Var["nQMedium"]=nQMedium;
  HM->Var["nQLowPt"]=nQLowPt;
  HM->Var["nQLoose"]=nQLoose;

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode XFourMuonAnalysis :: doB1OniaSel () {

  // B1Onia selection
  //---------------------------------
  m_good_b1onias.clear();
  m_onia_pairs.clear();

  xAOD::VertexContainer::const_iterator pairItr = m_b1onias->begin();
  for (; pairItr!=m_b1onias->end(); ++pairItr) {
    auto pair = *pairItr;

    if ((*pairItr)->auxdataConst<MuonLinkVector>("MuonLinks").size()!=2) {
      ATH_MSG_ERROR("doB1OniaSel() Pair without 2 muon links! Something is wrong!");
      return StatusCode::FAILURE;
    }

    //if(pair->auxdataConst<std::string>("ChargeCode") == "+-" || pair->auxdataConst<std::string>("ChargeCode") == "-+") 
    HM->ClearCutflowFlag("B1Onia");
    HM->Cutflow_flag_tmp["B1Onia"]["AllB1Onia"]=1;
    if(pair->auxdataConst<float>("chiSquared") < 10000) {
      HM->Cutflow_flag_tmp["B1Onia"]["Chi2"]=1;
      if(pair->auxdataConst<float>("Jpsi_mass") > 2e3 && pair->auxdataConst<float>("Jpsi_mass") < 50e3) {
        HM->Cutflow_flag_tmp["B1Onia"]["Mass"]=1;
        if( passPairTrkPt(pair, 2.0e3) ) {
          HM->Cutflow_flag_tmp["B1Onia"]["B1OniaTrkKine"]=1;
          m_good_b1onias.push_back(pair);
        }
      }
    }
   
    HM->FillCutflow("B1Onia");
  }

  if(m_debug) ATH_MSG_INFO("doB1OniaSel() Number of good b1onias = " << m_good_b1onias.size() );
  HM->Var["nPairsOS"]=m_good_b1onias.size();

  int nGoodPair=0;
  //== make some plots
  for(auto pair: m_good_b1onias) {
    HM->ClearCutflowFlag("B1Onia");
    bool pass_pt=false, pass_cb=false, pass_chi2=false, pass_mass=false;

    double mass = pair->auxdataConst<float>("Jpsi_mass");
    double massErr = pair->auxdataConst<float>("Jpsi_massErr");
    double chi2 = pair->auxdataConst<float>("chiSquared");

    if( passPairTrkPt(pair, 4e3) ) pass_pt=true;
    if( chi2 < 3 ) pass_chi2=true;
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
      HM->FillCutflow("B1Onia");
      continue;
    }
    HM->Cutflow_flag_tmp["B1Onia"]["MuonMatching"]=1;
    HM->Cutflow_flag["B1Onia"]["MuonMatching"]=1;

    const xAOD::TrackParticle * Track0 = NULL;
    const xAOD::TrackParticle * Track1 = NULL;

    //==== tracks
    const TrkLink& Link0 = Muon0->inDetTrackParticleLink();
    const TrkLink& Link1 = Muon1->inDetTrackParticleLink();

    if(Link0.isValid()) Track0 = *Link0;
    if(Link1.isValid()) Track1 = *Link1;

    if(Track0 == NULL || Track1 == NULL) {
      Warning("XFourMuonAnalysis::doB1OniaSel()", "Missing Track Pointers!");
      HM->FillCutflow("B1Onia");
      continue;
    }
    float z0SinTheta0 = (Track0->z0()+Track0->vz()-m_pv->z())*sin(Track0->p4().Theta());
    float z0SinTheta1 = (Track1->z0()+Track1->vz()-m_pv->z())*sin(Track1->p4().Theta());
    if(fabs(z0SinTheta0)>1 || fabs(z0SinTheta1)>1) continue;
    float d0Sig0 = xAOD::TrackingHelpers::d0significance(Track0, m_eventInfo->beamPosSigmaX(), m_eventInfo->beamPosSigmaY(), m_eventInfo->beamPosSigmaXY() );
    float d0Sig1 = xAOD::TrackingHelpers::d0significance(Track1, m_eventInfo->beamPosSigmaX(), m_eventInfo->beamPosSigmaY(), m_eventInfo->beamPosSigmaXY() );
    if(fabs(d0Sig0)>6 || fabs(d0Sig1)>6) continue;
    HM->VVar["B1Onia_d0Sig0"]->push_back(d0Sig0);
    HM->VVar["B1Onia_d0Sig1"]->push_back(d0Sig1);
    HM->VVar["B1Onia_z0SinTheta0"]->push_back(z0SinTheta0);
    HM->VVar["B1Onia_z0SinTheta1"]->push_back(z0SinTheta1);

    HM->Cutflow_flag_tmp["B1Onia"]["TrkMatching"]=1;

    //==== ID hits cut
    bool passedIDCuts = true;
    passedIDCuts &= m_muonSelectionToolLowPt->passedIDCuts(*Muon0);//ID Cuts should be already included in next cut
    passedIDCuts &= m_muonSelectionToolLowPt->passedIDCuts(*Muon1);
    if(passedIDCuts) HM->Cutflow_flag_tmp["B1Onia"]["B1OniaID"]=1;
    else {
      HM->FillCutflow("B1Onia");
      continue;
    }

    //calibration
    // !! need to delete the pointers for the calibrated muons, to avoid memory blow up
    xAOD::Muon *mu_corr0=NULL, *mu_corr1=NULL;
    if(m_muonsMap.find(Muon0) !=m_muonsMap.end()) {
      mu_corr0 = m_muonsMap[Muon0];
    } else {
      CP_CHECK("doB1OniaSel()", m_muonCalibrationAndSmearingTool->correctedCopy(*Muon0, mu_corr0));
      m_muonsMap[Muon0]=mu_corr0; 
    }
    if(m_muonsMap.find(Muon1) !=m_muonsMap.end()) {
      mu_corr1 = m_muonsMap[Muon1];
    } else {
      CP_CHECK("doB1OniaSel()", m_muonCalibrationAndSmearingTool->correctedCopy(*Muon1, mu_corr1));
      m_muonsMap[Muon1]=mu_corr1; 
    }

    int nLoose=0, nMedium=0, nLowPt=0;
    int isL1Loose=0, isL2Loose=0;
    int isL1Medium=0, isL2Medium=0;
    int isL1LowPt=0, isL2LowPt=0;
    if(m_doMuonCorr) {
      if(m_muonSelectionToolLoose->accept(*mu_corr0)){ isL1Loose=1; nLoose+=1;}
      if(m_muonSelectionToolMedium->accept(*mu_corr0)){ isL1Medium=1; nMedium+=1;}
      if(m_muonSelectionToolLowPt->accept(*mu_corr0)) { isL1LowPt=1; nLowPt+=1;}
    
      if(m_muonSelectionToolLoose->accept(*mu_corr1)){ isL2Loose=1; nLoose+=1;}
      if(m_muonSelectionToolMedium->accept(*mu_corr1)){ isL2Medium=1; nMedium+=1;}
      if(m_muonSelectionToolLowPt->accept(*mu_corr1)) { isL2LowPt=1; nLowPt+=1;}
    } else {
      if(m_muonSelectionToolLoose->accept(*Muon0)){ isL1Loose=1; nLoose+=1;}
      if(m_muonSelectionToolMedium->accept(*Muon0)){ isL1Medium=1; nMedium+=1;}
      if(m_muonSelectionToolLowPt->accept(*Muon0)) { isL1LowPt=1; nLowPt+=1;}
    
      if(m_muonSelectionToolLoose->accept(*Muon1)){ isL2Loose=1; nLoose+=1;}
      if(m_muonSelectionToolMedium->accept(*Muon1)){ isL2Medium=1; nMedium+=1;}
      if(m_muonSelectionToolLowPt->accept(*Muon1)) { isL2LowPt=1; nLowPt+=1;}
    }

    HM->VVar["B1Onia_nLoose"]->push_back(nLoose);
    HM->VVar["B1Onia_nMedium"]->push_back(nMedium);
    HM->VVar["B1Onia_nLowPt"]->push_back(nLowPt);

    HM->VVar["B1Onia_isL1Loose"]->push_back(isL1Loose);
    HM->VVar["B1Onia_isL1Medium"]->push_back(isL1Medium);
    HM->VVar["B1Onia_isL1LowPt"]->push_back(isL1LowPt);
    HM->VVar["B1Onia_isL2Loose"]->push_back(isL2Loose);
    HM->VVar["B1Onia_isL2Medium"]->push_back(isL2Medium);
    HM->VVar["B1Onia_isL2LowPt"]->push_back(isL2LowPt);

    int nCb=0;
    if(Muon0->muonType() == xAOD::Muon::Combined) nCb ++;
    if(Muon1->muonType() == xAOD::Muon::Combined) nCb ++;
    if(nCb==2) pass_cb=true;
    HM->VVar["B1Onia_nCB"]->push_back(nCb);
    // match with the four good muons
    if(matchFourMuon(pair, m_good_qmus)) {
      HM->VVar["MatchQMu_B1Onia"]->push_back(1);
      if(chi2 < 3) nGoodPair += 1;
    }
    else HM->VVar["MatchQMu_B1Onia"]->push_back(0);

    // kinematics
    HM->VVar["MassO_B1Onia"]->push_back(mass*1e-3);
    HM->VVar["MassErrO_B1Onia"]->push_back(massErr*1e-3);
    HM->VVar["Chi2O_B1Onia"]->push_back(log10(chi2));

    // muons
    TLorentzVector MuDiMuon1 = Muon0->p4() + Muon1->p4();
    HM->VVar["MuMO_B1Onia"]->push_back(MuDiMuon1.M()*1e-3);
    HM->VVar["MuPtO_B1Onia"]->push_back(MuDiMuon1.Pt()*1e-3);
    HM->VVar["MuPhiO_B1Onia"]->push_back(MuDiMuon1.Phi());
    HM->VVar["MuYO_B1Onia"]->push_back(MuDiMuon1.Rapidity());

    HM->VVar["L1IsCB_B1Onia"]->push_back(Muon0->muonType()==0);
    HM->VVar["L1Pt_B1Onia"]->push_back(Muon0->pt()*1e-3);
    HM->VVar["L1Eta_B1Onia"]->push_back(Muon0->eta());
    HM->VVar["L1Phi_B1Onia"]->push_back(Muon0->phi());

    HM->VVar["L2IsCB_B1Onia"]->push_back(Muon1->muonType()==0);
    HM->VVar["L2Pt_B1Onia"]->push_back(Muon1->pt()*1e-3);
    HM->VVar["L2Eta_B1Onia"]->push_back(Muon1->eta());
    HM->VVar["L2Phi_B1Onia"]->push_back(Muon1->phi());

    // tracks
    TLorentzVector TrkDiMuon1 = Track0->p4() + Track1->p4();
    HM->VVar["TrkMO_B1Onia"]->push_back(TrkDiMuon1.M()*1e-3);
    HM->VVar["TrkPtO_B1Onia"]->push_back(TrkDiMuon1.Pt()*1e-3);
    HM->VVar["TrkPhiO_B1Onia"]->push_back(TrkDiMuon1.Phi());
    HM->VVar["TrkYO_B1Onia"]->push_back(TrkDiMuon1.Rapidity());

    HM->VVar["L1TrkPt_B1Onia"]->push_back(Track0->pt()*1e-3);
    HM->VVar["L1TrkEta_B1Onia"]->push_back(Track0->eta());
    HM->VVar["L1TrkPhi_B1Onia"]->push_back(Track0->phi());

    HM->VVar["L2TrkPt_B1Onia"]->push_back(Track1->pt()*1e-3);
    HM->VVar["L2TrkEta_B1Onia"]->push_back(Track1->eta());
    HM->VVar["L2TrkPhi_B1Onia"]->push_back(Track1->phi());

    // Helper Tool
    xAOD::BPhysHelper bVtx(pair);
    TLorentzVector Muon0_RefTrack = bVtx.refTrk(0, M_MU);
    TLorentzVector Muon1_RefTrack = bVtx.refTrk(1, M_MU);
    // vertex fit kine
    HM->VVar["B1Oa0"]->push_back(bVtx.a0());
    HM->VVar["B1Oa0Err"]->push_back(bVtx.a0Err());
    HM->VVar["B1Oz0"]->push_back(bVtx.z0());
    HM->VVar["B1Oz0Err"]->push_back(bVtx.z0Err());
    HM->VVar["B1Oa0xy"]->push_back(bVtx.a0xy());
    HM->VVar["B1Oa0xyErr"]->push_back(bVtx.a0xyErr());
    HM->VVar["B1Olxy"]->push_back(bVtx.lxy());
    HM->VVar["B1OlxyErr"]->push_back(bVtx.lxyErr());

    // refit trk
    TLorentzVector RefDiMuon = Muon0_RefTrack + Muon1_RefTrack;
    HM->VVar["MassORefTrk_B1Onia"]->push_back(RefDiMuon.M()*1e-3);
    HM->VVar["PtORefTrk_B1Onia"]->push_back(RefDiMuon.Pt()*1e-3);
    HM->VVar["YORefTrk_B1Onia"]->push_back(RefDiMuon.Rapidity());

    HM->VVar["L1RefTrkPt_B1Onia"]->push_back(Muon0_RefTrack.Pt()*1e-3);
    HM->VVar["L1RefTrkEta_B1Onia"]->push_back(Muon0_RefTrack.Eta());

    HM->VVar["L2RefTrkPt_B1Onia"]->push_back(Muon1_RefTrack.Pt()*1e-3);
    HM->VVar["L2RefTrkEta_B1Onia"]->push_back(Muon1_RefTrack.Eta());

    // passed Jpsi/Upsi
    HM->VVar["passJpsi_B1Onia"]->push_back((bool) pair->auxdata<char>("passed_Jpsi"));
    HM->VVar["passUpsi_B1Onia"]->push_back((bool) pair->auxdata<char>("passed_Upsi"));

    HM->FillCutflow("B1Onia");

  }

  if(nGoodPair>=0) HM->Cutflow_flag_tmp["Event"]["Onia"]=1;
/*
//---------------------------- DiOnia selection ----------------------------- 

   //== match pairs to the four good muons
    if((int)m_good_qmus.size()<4 || m_good_b1onias.size()==0) return EL::StatusCode::SUCCESS;
    m_onia_pairs = matchOnia(m_good_qmus, m_good_b1onias);
    

    //== Onia pair selection: pt>2, no chi2 cut
    //bool pass_two_o=false;
    bool pass_two_o_chi2=false;
    const xAOD::Vertex *o1=0, *o2=0;
    const xAOD::Vertex *cand_o1=0, *cand_o2=0;
//if (m_onia_pairs.size()!=0) std::cout<<"--------------m_onia_pairs.size():"<<m_onia_pairs.size()<<std::endl;
    int isJJ=0, isUU=0, isUJ=0;
    for(auto onia_pair: m_onia_pairs) {
      bool pass_tmp_chi2_o1 = false, pass_tmp_chi2_o2 = false, pass_tmp_leppt = false;

      o1=onia_pair.first; o2=onia_pair.second;
      double chi2_o1 = getChi2(o1); 
      double chi2_o2 = getChi2(o2);

      pass_tmp_chi2_o1 = chi2_o1 < 3;
      pass_tmp_chi2_o2 = chi2_o2 < 3;
      if(passPairTrkPt(o1, 2e3) && passPairTrkPt(o2, 2e3)) pass_tmp_leppt=true;

      //if(getCharge(o1)==0 && getCharge(o2)==0) pass_two_o |= pass_tmp_leppt;

      // !!! should not require OS, in order to look at SS
      pass_two_o_chi2 |= pass_tmp_chi2_o1 && pass_tmp_chi2_o2 && pass_tmp_leppt;

      double mass_o1 = o1->auxdataConst<float>("Jpsi_mass");
      double mass_o2 = o2->auxdataConst<float>("Jpsi_mass");
      if(mass_o1>8.e3 && mass_o1<12.e3 && mass_o2>8.e3 && mass_o2<12.e3) isUU=1;
      if(mass_o1>2.5e3 && mass_o1<3.6e3 && mass_o2>2.5e3 && mass_o2<3.6e3) isJJ=1;
      if((mass_o1>2.5e3 && mass_o1<3.6e3 && mass_o2>8.e3 && mass_o2<12.e3) || (mass_o1>8.e3 && mass_o1<12.e3 && mass_o2>2.5e3 && mass_o2<3.6e3)) isUJ=1;

      //Select the Upsilon candidate
      if(mass_o1>9.1e3 && mass_o1<9.8e3)  {
        if(!cand_o1 || getChi2(cand_o1) > chi2_o1) {
        cand_o1 = o1; cand_o2 = o2;}
      }
      if(mass_o2>9.1e3 && mass_o2<9.8e3) {
        if(!cand_o1 || getChi2(cand_o1) > chi2_o2) {
        cand_o2 = o1; cand_o1 = o2;}
      }

    }
    HM->Var["isJJ"]=isJJ;
    HM->Var["isUU"]=isUU;
    HM->Var["isUJ"]=isUJ;

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

    if(cand_o1 && cand_o2) {
      HM->Cutflow_flag_tmp["Event"]["MassUp"]=1;
      HM->Cutflow_flag_tmp["EventOS"]["MassUp"]=1;
      HM->Cutflow_flag_tmp["EventSS"]["MassUp"]=1;
      HM->VVar["MassO1"]->push_back(cand_o1->auxdataConst<float>("Jpsi_mass")*1e-3);
      HM->VVar["MassO2"]->push_back(cand_o2->auxdataConst<float>("Jpsi_mass")*1e-3);
    }

  //---------- calculate 4l mass -------------   
  const xAOD::Muon * Muon0 = NULL;
  const xAOD::Muon * Muon1 = NULL;
  const xAOD::Muon * Muon2 = NULL;
  const xAOD::Muon * Muon3 = NULL;

  if((int)m_good_qmus.size()>=4) {
    Muon0 = m_good_qmus.at(0); Muon1 = m_good_qmus.at(1);
    Muon2 = m_good_qmus.at(2); Muon3 = m_good_qmus.at(3);
  }
  TLorentzVector DiMuon1 = Muon0->p4() + Muon1->p4();
  TLorentzVector DiMuon2 = Muon2->p4() + Muon3->p4();
  TLorentzVector MuQuad = DiMuon1 + DiMuon2;

  HM->VVar["MuPtQ"]->push_back(MuQuad.Pt()*1e-3);
  HM->VVar["MuMQ"]->push_back(MuQuad.M()*1e-3);

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

  TLorentzVector TrkDiMuon1,TrkDiMuon2,TrkQuad;
  TLorentzVector Muon0_RefTrackOrig, Muon1_RefTrackOrig, Muon2_RefTrackOrig, Muon3_RefTrackOrig;

  Muon0_RefTrackOrig.SetPtEtaPhiM(Track0->pt(), Track0->eta(), Track0->phi(), M_MU);
  Muon1_RefTrackOrig.SetPtEtaPhiM(Track1->pt(), Track1->eta(), Track1->phi(), M_MU);
  Muon2_RefTrackOrig.SetPtEtaPhiM(Track2->pt(), Track2->eta(), Track2->phi(), M_MU);
  Muon3_RefTrackOrig.SetPtEtaPhiM(Track3->pt(), Track3->eta(), Track3->phi(), M_MU);

  TrkDiMuon1 = Muon0_RefTrackOrig + Muon1_RefTrackOrig;
  TrkDiMuon2 = Muon2_RefTrackOrig + Muon3_RefTrackOrig;
  TrkQuad = TrkDiMuon1 + TrkDiMuon2;

  HM->VVar["TrkPtQ"]->push_back(TrkQuad.Pt()*1e-3);
  HM->VVar["TrkMQ"]->push_back(TrkQuad.M()*1e-3);
*/

  return EL::StatusCode::SUCCESS;
}


