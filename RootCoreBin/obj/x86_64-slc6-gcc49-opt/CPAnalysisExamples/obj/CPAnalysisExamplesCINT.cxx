#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#ifdef __llvm__
#pragma GCC diagnostic ignored "-Wunused-private-field"
#endif
// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIatlasdIdata18adIhelingzhdIworkplacedIsummer2016dIRun2dIFourMuonAnadIRootCoreBindIobjdIx86_64mIslc6mIgcc49mIoptdICPAnalysisExamplesdIobjdICPAnalysisExamplesCINT

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
#include "CPAnalysisExamples/xExample.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_xExample(void *p = 0);
   static void *newArray_xExample(Long_t size, void *p);
   static void delete_xExample(void *p);
   static void deleteArray_xExample(void *p);
   static void destruct_xExample(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::xExample*)
   {
      ::xExample *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::xExample >(0);
      static ::ROOT::TGenericClassInfo 
         instance("xExample", ::xExample::Class_Version(), "CPAnalysisExamples/xExample.h", 32,
                  typeid(::xExample), DefineBehavior(ptr, ptr),
                  &::xExample::Dictionary, isa_proxy, 4,
                  sizeof(::xExample) );
      instance.SetNew(&new_xExample);
      instance.SetNewArray(&newArray_xExample);
      instance.SetDelete(&delete_xExample);
      instance.SetDeleteArray(&deleteArray_xExample);
      instance.SetDestructor(&destruct_xExample);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::xExample*)
   {
      return GenerateInitInstanceLocal((::xExample*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::xExample*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr xExample::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *xExample::Class_Name()
{
   return "xExample";
}

//______________________________________________________________________________
const char *xExample::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::xExample*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int xExample::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::xExample*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *xExample::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::xExample*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *xExample::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::xExample*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void xExample::Streamer(TBuffer &R__b)
{
   // Stream an object of class xExample.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(xExample::Class(),this);
   } else {
      R__b.WriteClassBuffer(xExample::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_xExample(void *p) {
      return  p ? new(p) ::xExample : new ::xExample;
   }
   static void *newArray_xExample(Long_t nElements, void *p) {
      return p ? new(p) ::xExample[nElements] : new ::xExample[nElements];
   }
   // Wrapper around operator delete
   static void delete_xExample(void *p) {
      delete ((::xExample*)p);
   }
   static void deleteArray_xExample(void *p) {
      delete [] ((::xExample*)p);
   }
   static void destruct_xExample(void *p) {
      typedef ::xExample current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::xExample

namespace {
  void TriggerDictionaryInitialization_CPAnalysisExamplesCINT_Impl() {
    static const char* headers[] = {
"CPAnalysisExamples/xExample.h",
0
    };
    static const char* includePaths[] = {
"/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/CPAnalysisExamples/Root",
"/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/CPAnalysisExamples",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/atlas/data18a/helingzh/workplace/summer2016/Run2/FourMuonAna/RootCoreBin/include",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/CPAnalysisExamples/cmt/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/CPAnalysisExamples/Root/LinkDef.h")))  xExample;
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
  #define ROOTCORE_PACKAGE "CPAnalysisExamples"
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "CPAnalysisExamples/xExample.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"xExample", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("CPAnalysisExamplesCINT",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_CPAnalysisExamplesCINT_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_CPAnalysisExamplesCINT_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_CPAnalysisExamplesCINT() {
  TriggerDictionaryInitialization_CPAnalysisExamplesCINT_Impl();
}
