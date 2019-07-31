// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIatlasdIdata18adIhelingzhdIworkplacedIsummer2016dIRun2dIFourMuonAnadIRootCoreBindIobjdIx86_64mIslc6mIgcc49mIoptdIMuonSelectorToolsdIobjdIMuonSelectorTools_Reflex

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
#include "/atlas/data18a/helingzh/workplace/summer2016/Run2/FourMuonAna/MuonSelectorTools/MuonSelectorTools/MuonSelectorToolsDict.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *CPcLcLIMuonSelectionTool_Dictionary();
   static void CPcLcLIMuonSelectionTool_TClassManip(TClass*);
   static void delete_CPcLcLIMuonSelectionTool(void *p);
   static void deleteArray_CPcLcLIMuonSelectionTool(void *p);
   static void destruct_CPcLcLIMuonSelectionTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CP::IMuonSelectionTool*)
   {
      ::CP::IMuonSelectionTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CP::IMuonSelectionTool));
      static ::ROOT::TGenericClassInfo 
         instance("CP::IMuonSelectionTool", "MuonSelectorTools/IMuonSelectionTool.h", 25,
                  typeid(::CP::IMuonSelectionTool), DefineBehavior(ptr, ptr),
                  &CPcLcLIMuonSelectionTool_Dictionary, isa_proxy, 0,
                  sizeof(::CP::IMuonSelectionTool) );
      instance.SetDelete(&delete_CPcLcLIMuonSelectionTool);
      instance.SetDeleteArray(&deleteArray_CPcLcLIMuonSelectionTool);
      instance.SetDestructor(&destruct_CPcLcLIMuonSelectionTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CP::IMuonSelectionTool*)
   {
      return GenerateInitInstanceLocal((::CP::IMuonSelectionTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::CP::IMuonSelectionTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CPcLcLIMuonSelectionTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CP::IMuonSelectionTool*)0x0)->GetClass();
      CPcLcLIMuonSelectionTool_TClassManip(theClass);
   return theClass;
   }

   static void CPcLcLIMuonSelectionTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CPcLcLMuonSelectionTool_Dictionary();
   static void CPcLcLMuonSelectionTool_TClassManip(TClass*);
   static void *new_CPcLcLMuonSelectionTool(void *p = 0);
   static void *newArray_CPcLcLMuonSelectionTool(Long_t size, void *p);
   static void delete_CPcLcLMuonSelectionTool(void *p);
   static void deleteArray_CPcLcLMuonSelectionTool(void *p);
   static void destruct_CPcLcLMuonSelectionTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CP::MuonSelectionTool*)
   {
      ::CP::MuonSelectionTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CP::MuonSelectionTool));
      static ::ROOT::TGenericClassInfo 
         instance("CP::MuonSelectionTool", "MuonSelectorTools/MuonSelectionTool.h", 28,
                  typeid(::CP::MuonSelectionTool), DefineBehavior(ptr, ptr),
                  &CPcLcLMuonSelectionTool_Dictionary, isa_proxy, 0,
                  sizeof(::CP::MuonSelectionTool) );
      instance.SetNew(&new_CPcLcLMuonSelectionTool);
      instance.SetNewArray(&newArray_CPcLcLMuonSelectionTool);
      instance.SetDelete(&delete_CPcLcLMuonSelectionTool);
      instance.SetDeleteArray(&deleteArray_CPcLcLMuonSelectionTool);
      instance.SetDestructor(&destruct_CPcLcLMuonSelectionTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CP::MuonSelectionTool*)
   {
      return GenerateInitInstanceLocal((::CP::MuonSelectionTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::CP::MuonSelectionTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CPcLcLMuonSelectionTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CP::MuonSelectionTool*)0x0)->GetClass();
      CPcLcLMuonSelectionTool_TClassManip(theClass);
   return theClass;
   }

   static void CPcLcLMuonSelectionTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CPcLcLIMuonSelectionTool(void *p) {
      delete ((::CP::IMuonSelectionTool*)p);
   }
   static void deleteArray_CPcLcLIMuonSelectionTool(void *p) {
      delete [] ((::CP::IMuonSelectionTool*)p);
   }
   static void destruct_CPcLcLIMuonSelectionTool(void *p) {
      typedef ::CP::IMuonSelectionTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CP::IMuonSelectionTool

namespace ROOT {
   // Wrappers around operator new
   static void *new_CPcLcLMuonSelectionTool(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) ::CP::MuonSelectionTool : new ::CP::MuonSelectionTool;
   }
   static void *newArray_CPcLcLMuonSelectionTool(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) ::CP::MuonSelectionTool[nElements] : new ::CP::MuonSelectionTool[nElements];
   }
   // Wrapper around operator delete
   static void delete_CPcLcLMuonSelectionTool(void *p) {
      delete ((::CP::MuonSelectionTool*)p);
   }
   static void deleteArray_CPcLcLMuonSelectionTool(void *p) {
      delete [] ((::CP::MuonSelectionTool*)p);
   }
   static void destruct_CPcLcLMuonSelectionTool(void *p) {
      typedef ::CP::MuonSelectionTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CP::MuonSelectionTool

namespace {
  void TriggerDictionaryInitialization_MuonSelectorTools_Reflex_Impl() {
    static const char* headers[] = {
0    };
    static const char* includePaths[] = {
"/atlas/data18a/helingzh/workplace/summer2016/Run2/FourMuonAna/MuonSelectorTools/Root",
"/atlas/data18a/helingzh/workplace/summer2016/Run2/FourMuonAna/MuonSelectorTools",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/atlas/data18a/helingzh/workplace/summer2016/Run2/FourMuonAna/RootCoreBin/include",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/atlas/data18a/helingzh/workplace/summer2016/Run2/FourMuonAna/MuonSelectorTools/cmt/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace CP{class __attribute__((annotate("$clingAutoload$MuonSelectorTools/IMuonSelectionTool.h")))  IMuonSelectionTool;}
namespace CP{class __attribute__((annotate("$clingAutoload$MuonSelectorTools/MuonSelectionTool.h")))  MuonSelectionTool;}
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
  #define ROOTCORE_PACKAGE "MuonSelectorTools"
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#ifndef MUONSELECTORTOOLS_MUONSELECTORTOOLSDICT_H
#define MUONSELECTORTOOLS_MUONSELECTORTOOLSDICT_H

#if defined(__GCCXML__) and not defined(EIGEN_DONT_VECTORIZE)
#define EIGEN_DONT_VECTORIZE
#endif // __GCCXML__

// Includes for the dictionary generation:
#include "MuonSelectorTools/IMuonSelectionTool.h"
#include "MuonSelectorTools/MuonSelectionTool.h"

#endif // MUONSELECTORTOOLS_MUONSELECTORTOOLSDICT_H

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"CP::IMuonSelectionTool", payloadCode, "@",
"CP::MuonSelectionTool", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("MuonSelectorTools_Reflex",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_MuonSelectorTools_Reflex_Impl, {{"namespace DataVector_detail { template <typename B1, typename B2, typename B3> class VirtBases; }", 1},{"template <typename T> class DataVectorBase;", 1},{"template <typename T, typename BASE> class DataVector;", 1},{"namespace DataVector_detail { template <typename T> class DVLEltBaseInit; }", 1}}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_MuonSelectorTools_Reflex_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_MuonSelectorTools_Reflex() {
  TriggerDictionaryInitialization_MuonSelectorTools_Reflex_Impl();
}
