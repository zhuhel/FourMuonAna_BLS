#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#ifdef __llvm__
#pragma GCC diagnostic ignored "-Wunused-private-field"
#endif
// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIatlasdIdata18adIhelingzhdIworkplacedIsummer2016dIRun2dIFourMuonAnadIRootCoreBindIobjdIx86_64mIslc6mIgcc49mIoptdIMuonMomentumCorrectionsdIobjdIMuonMomentumCorrectionsCINT

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
#include "MuonMomentumCorrections/MuonResolutionAndMomentumScaleFactors.h"

// Header files passed via #pragma extra_include

namespace Analysis {
   namespace ROOT {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *Analysis_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("Analysis", 0 /*version*/, "MuonMomentumCorrections/MuonResolutionAndMomentumScaleFactors.h", 34,
                     ::ROOT::DefineBehavior((void*)0,(void*)0),
                     &Analysis_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *Analysis_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static TClass *AnalysiscLcLMuonResolutionAndMomentumScaleFactors_Dictionary();
   static void AnalysiscLcLMuonResolutionAndMomentumScaleFactors_TClassManip(TClass*);
   static void delete_AnalysiscLcLMuonResolutionAndMomentumScaleFactors(void *p);
   static void deleteArray_AnalysiscLcLMuonResolutionAndMomentumScaleFactors(void *p);
   static void destruct_AnalysiscLcLMuonResolutionAndMomentumScaleFactors(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Analysis::MuonResolutionAndMomentumScaleFactors*)
   {
      ::Analysis::MuonResolutionAndMomentumScaleFactors *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Analysis::MuonResolutionAndMomentumScaleFactors));
      static ::ROOT::TGenericClassInfo 
         instance("Analysis::MuonResolutionAndMomentumScaleFactors", "MuonMomentumCorrections/MuonResolutionAndMomentumScaleFactors.h", 36,
                  typeid(::Analysis::MuonResolutionAndMomentumScaleFactors), DefineBehavior(ptr, ptr),
                  &AnalysiscLcLMuonResolutionAndMomentumScaleFactors_Dictionary, isa_proxy, 4,
                  sizeof(::Analysis::MuonResolutionAndMomentumScaleFactors) );
      instance.SetDelete(&delete_AnalysiscLcLMuonResolutionAndMomentumScaleFactors);
      instance.SetDeleteArray(&deleteArray_AnalysiscLcLMuonResolutionAndMomentumScaleFactors);
      instance.SetDestructor(&destruct_AnalysiscLcLMuonResolutionAndMomentumScaleFactors);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Analysis::MuonResolutionAndMomentumScaleFactors*)
   {
      return GenerateInitInstanceLocal((::Analysis::MuonResolutionAndMomentumScaleFactors*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Analysis::MuonResolutionAndMomentumScaleFactors*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AnalysiscLcLMuonResolutionAndMomentumScaleFactors_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Analysis::MuonResolutionAndMomentumScaleFactors*)0x0)->GetClass();
      AnalysiscLcLMuonResolutionAndMomentumScaleFactors_TClassManip(theClass);
   return theClass;
   }

   static void AnalysiscLcLMuonResolutionAndMomentumScaleFactors_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_AnalysiscLcLMuonResolutionAndMomentumScaleFactors(void *p) {
      delete ((::Analysis::MuonResolutionAndMomentumScaleFactors*)p);
   }
   static void deleteArray_AnalysiscLcLMuonResolutionAndMomentumScaleFactors(void *p) {
      delete [] ((::Analysis::MuonResolutionAndMomentumScaleFactors*)p);
   }
   static void destruct_AnalysiscLcLMuonResolutionAndMomentumScaleFactors(void *p) {
      typedef ::Analysis::MuonResolutionAndMomentumScaleFactors current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Analysis::MuonResolutionAndMomentumScaleFactors

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 214,
                  typeid(vector<double>), DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace {
  void TriggerDictionaryInitialization_MuonMomentumCorrectionsCINT_Impl() {
    static const char* headers[] = {
"MuonMomentumCorrections/MuonResolutionAndMomentumScaleFactors.h",
0
    };
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
namespace Analysis{class __attribute__((annotate("$clingAutoload$MuonMomentumCorrections/MuonResolutionAndMomentumScaleFactors.h")))  MuonResolutionAndMomentumScaleFactors;}
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
#include "MuonMomentumCorrections/MuonResolutionAndMomentumScaleFactors.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Analysis::MuonResolutionAndMomentumScaleFactors", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("MuonMomentumCorrectionsCINT",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_MuonMomentumCorrectionsCINT_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_MuonMomentumCorrectionsCINT_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_MuonMomentumCorrectionsCINT() {
  TriggerDictionaryInitialization_MuonMomentumCorrectionsCINT_Impl();
}
