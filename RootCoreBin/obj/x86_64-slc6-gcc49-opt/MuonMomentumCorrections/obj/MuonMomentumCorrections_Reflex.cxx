// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIatlasdIdata18adIhelingzhdIworkplacedIsummer2016dIRun2dIFourMuonAnadIRootCoreBindIobjdIx86_64mIslc6mIgcc49mIoptdIMuonMomentumCorrectionsdIobjdIMuonMomentumCorrections_Reflex

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/MuonMomentumCorrections/MuonMomentumCorrections/MuonMomentumCorrectionsDict.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *CPcLcLMuonCalibrationAndSmearingTool_Dictionary();
   static void CPcLcLMuonCalibrationAndSmearingTool_TClassManip(TClass*);
   static void delete_CPcLcLMuonCalibrationAndSmearingTool(void *p);
   static void deleteArray_CPcLcLMuonCalibrationAndSmearingTool(void *p);
   static void destruct_CPcLcLMuonCalibrationAndSmearingTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CP::MuonCalibrationAndSmearingTool*)
   {
      ::CP::MuonCalibrationAndSmearingTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CP::MuonCalibrationAndSmearingTool));
      static ::ROOT::TGenericClassInfo 
         instance("CP::MuonCalibrationAndSmearingTool", "MuonMomentumCorrections/MuonCalibrationAndSmearingTool.h", 38,
                  typeid(::CP::MuonCalibrationAndSmearingTool), DefineBehavior(ptr, ptr),
                  &CPcLcLMuonCalibrationAndSmearingTool_Dictionary, isa_proxy, 0,
                  sizeof(::CP::MuonCalibrationAndSmearingTool) );
      instance.SetDelete(&delete_CPcLcLMuonCalibrationAndSmearingTool);
      instance.SetDeleteArray(&deleteArray_CPcLcLMuonCalibrationAndSmearingTool);
      instance.SetDestructor(&destruct_CPcLcLMuonCalibrationAndSmearingTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CP::MuonCalibrationAndSmearingTool*)
   {
      return GenerateInitInstanceLocal((::CP::MuonCalibrationAndSmearingTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::CP::MuonCalibrationAndSmearingTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CPcLcLMuonCalibrationAndSmearingTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CP::MuonCalibrationAndSmearingTool*)0x0)->GetClass();
      CPcLcLMuonCalibrationAndSmearingTool_TClassManip(theClass);
   return theClass;
   }

   static void CPcLcLMuonCalibrationAndSmearingTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CPcLcLMuonCalibrationAndSmearingTool(void *p) {
      delete ((::CP::MuonCalibrationAndSmearingTool*)p);
   }
   static void deleteArray_CPcLcLMuonCalibrationAndSmearingTool(void *p) {
      delete [] ((::CP::MuonCalibrationAndSmearingTool*)p);
   }
   static void destruct_CPcLcLMuonCalibrationAndSmearingTool(void *p) {
      typedef ::CP::MuonCalibrationAndSmearingTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CP::MuonCalibrationAndSmearingTool

namespace {
  void TriggerDictionaryInitialization_MuonMomentumCorrections_Reflex_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/MuonMomentumCorrections/Root",
"/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/MuonMomentumCorrections",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/atlas/data18a/helingzh/workplace/summer2016/Run2/FourMuonAna/RootCoreBin/include",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/MuonMomentumCorrections/cmt/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace CP{class __attribute__((annotate("$clingAutoload$MuonMomentumCorrections/MuonCalibrationAndSmearingTool.h")))  MuonCalibrationAndSmearingTool;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef ROOTCORE
  #define ROOTCORE 1
#endif
#ifndef ROOTCORE_RELEASE_SERIES
  #define ROOTCORE_RELEASE_SERIES 25
#endif
#ifndef ROOTCORE_TEST_FILE
  #define ROOTCORE_TEST_FILE "/afs/cern.ch/atlas/project/PAT/xAODs/r8565/valid2.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.recon.AOD.e4993_s2887_r8565/AOD.09534072._000257.pool.root.1"
#endif
#ifndef ROOTCORE_TEST_DATA
  #define ROOTCORE_TEST_DATA "/afs/cern.ch/atlas/project/PAT/xAODs/r8565"
#endif
#ifndef ASG_TEST_FILE_DATA
  #define ASG_TEST_FILE_DATA "/afs/cern.ch/atlas/project/PAT/xAODs/r8601/data15_13TeV.00284285.physics_Main.recon.AOD.r8601/AOD.09617210._008306.pool.root.1"
#endif
#ifndef ASG_TEST_FILE_MC
  #define ASG_TEST_FILE_MC "/afs/cern.ch/atlas/project/PAT/xAODs/r8565/valid2.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.recon.AOD.e4993_s2887_r8565/AOD.09534072._000257.pool.root.1"
#endif
#ifndef ASG_TEST_FILE_MCAFII
  #define ASG_TEST_FILE_MCAFII ""
#endif
#ifndef XAOD_STANDALONE
  #define XAOD_STANDALONE 1
#endif
#ifndef XAOD_ANALYSIS
  #define XAOD_ANALYSIS 1
#endif
#ifndef ASGTOOL_STANDALONE
  #define ASGTOOL_STANDALONE 1
#endif
#ifndef ROOTCORE_PACKAGE
  #define ROOTCORE_PACKAGE "MuonMomentumCorrections"
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#ifndef MUONMOMENTUMCORRECTIONS_MUONMOMENTUMCORRECTIONSDICT_H
#define MUONMOMENTUMCORRECTIONS_MUONMOMENTUMCORRECTIONSDICT_H

#if defined(__GCCXML__) and not defined(EIGEN_DONT_VECTORIZE)
#define EIGEN_DONT_VECTORIZE
#endif // __GCCXML__

#include "MuonMomentumCorrections/MuonCalibrationAndSmearingTool.h"

#endif // not MUONMOMENTUMCORRECTIONS_MUONMOMENTUMCORRECTIONSDICT_H

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"CP::MuonCalibrationAndSmearingTool", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("MuonMomentumCorrections_Reflex",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_MuonMomentumCorrections_Reflex_Impl, {{"namespace DataVector_detail { template <typename B1, typename B2, typename B3> class VirtBases; }", 1},{"template <typename T> class DataVectorBase;", 1},{"template <typename T, typename BASE> class DataVector;", 1},{"namespace DataVector_detail { template <typename T> class DVLEltBaseInit; }", 1}}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_MuonMomentumCorrections_Reflex_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_MuonMomentumCorrections_Reflex() {
  TriggerDictionaryInitialization_MuonMomentumCorrections_Reflex_Impl();
}
