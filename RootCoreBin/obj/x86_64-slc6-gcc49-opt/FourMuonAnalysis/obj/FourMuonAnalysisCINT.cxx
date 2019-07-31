#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#ifdef __llvm__
#pragma GCC diagnostic ignored "-Wunused-private-field"
#endif
// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIatlasdIdata18adIhelingzhdIworkplacedIsummer2016dIRun2dIFourMuonAnadIRootCoreBindIobjdIx86_64mIslc6mIgcc49mIoptdIFourMuonAnalysisdIobjdIFourMuonAnalysisCINT

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
#include "FourMuonAnalysis/XFourMuonAnalysis.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_XFourMuonAnalysis(void *p = 0);
   static void *newArray_XFourMuonAnalysis(Long_t size, void *p);
   static void delete_XFourMuonAnalysis(void *p);
   static void deleteArray_XFourMuonAnalysis(void *p);
   static void destruct_XFourMuonAnalysis(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::XFourMuonAnalysis*)
   {
      ::XFourMuonAnalysis *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::XFourMuonAnalysis >(0);
      static ::ROOT::TGenericClassInfo 
         instance("XFourMuonAnalysis", ::XFourMuonAnalysis::Class_Version(), "FourMuonAnalysis/XFourMuonAnalysis.h", 111,
                  typeid(::XFourMuonAnalysis), DefineBehavior(ptr, ptr),
                  &::XFourMuonAnalysis::Dictionary, isa_proxy, 4,
                  sizeof(::XFourMuonAnalysis) );
      instance.SetNew(&new_XFourMuonAnalysis);
      instance.SetNewArray(&newArray_XFourMuonAnalysis);
      instance.SetDelete(&delete_XFourMuonAnalysis);
      instance.SetDeleteArray(&deleteArray_XFourMuonAnalysis);
      instance.SetDestructor(&destruct_XFourMuonAnalysis);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::XFourMuonAnalysis*)
   {
      return GenerateInitInstanceLocal((::XFourMuonAnalysis*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::XFourMuonAnalysis*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr XFourMuonAnalysis::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *XFourMuonAnalysis::Class_Name()
{
   return "XFourMuonAnalysis";
}

//______________________________________________________________________________
const char *XFourMuonAnalysis::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::XFourMuonAnalysis*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int XFourMuonAnalysis::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::XFourMuonAnalysis*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *XFourMuonAnalysis::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::XFourMuonAnalysis*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *XFourMuonAnalysis::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::XFourMuonAnalysis*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void XFourMuonAnalysis::Streamer(TBuffer &R__b)
{
   // Stream an object of class XFourMuonAnalysis.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(XFourMuonAnalysis::Class(),this);
   } else {
      R__b.WriteClassBuffer(XFourMuonAnalysis::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_XFourMuonAnalysis(void *p) {
      return  p ? new(p) ::XFourMuonAnalysis : new ::XFourMuonAnalysis;
   }
   static void *newArray_XFourMuonAnalysis(Long_t nElements, void *p) {
      return p ? new(p) ::XFourMuonAnalysis[nElements] : new ::XFourMuonAnalysis[nElements];
   }
   // Wrapper around operator delete
   static void delete_XFourMuonAnalysis(void *p) {
      delete ((::XFourMuonAnalysis*)p);
   }
   static void deleteArray_XFourMuonAnalysis(void *p) {
      delete [] ((::XFourMuonAnalysis*)p);
   }
   static void destruct_XFourMuonAnalysis(void *p) {
      typedef ::XFourMuonAnalysis current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::XFourMuonAnalysis

namespace ROOT {
   static TClass *vectorlEstringgR_Dictionary();
   static void vectorlEstringgR_TClassManip(TClass*);
   static void *new_vectorlEstringgR(void *p = 0);
   static void *newArray_vectorlEstringgR(Long_t size, void *p);
   static void delete_vectorlEstringgR(void *p);
   static void deleteArray_vectorlEstringgR(void *p);
   static void destruct_vectorlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<string>*)
   {
      vector<string> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<string>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<string>", -2, "vector", 214,
                  typeid(vector<string>), DefineBehavior(ptr, ptr),
                  &vectorlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<string>) );
      instance.SetNew(&new_vectorlEstringgR);
      instance.SetNewArray(&newArray_vectorlEstringgR);
      instance.SetDelete(&delete_vectorlEstringgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstringgR);
      instance.SetDestructor(&destruct_vectorlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<string> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<string>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<string>*)0x0)->GetClass();
      vectorlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstringgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<string> : new vector<string>;
   }
   static void *newArray_vectorlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<string>[nElements] : new vector<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstringgR(void *p) {
      delete ((vector<string>*)p);
   }
   static void deleteArray_vectorlEstringgR(void *p) {
      delete [] ((vector<string>*)p);
   }
   static void destruct_vectorlEstringgR(void *p) {
      typedef vector<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<string>

namespace {
  void TriggerDictionaryInitialization_FourMuonAnalysisCINT_Impl() {
    static const char* headers[] = {
"FourMuonAnalysis/XFourMuonAnalysis.h",
0
    };
    static const char* includePaths[] = {
"/atlas/data18a/helingzh/workplace/summer2016/Run2/FourMuonAna/FourMuonAnalysis/Root",
"/atlas/data18a/helingzh/workplace/summer2016/Run2/FourMuonAna/FourMuonAnalysis",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/atlas/data18a/helingzh/workplace/summer2016/Run2/FourMuonAna/RootCoreBin/include",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/atlas/data18a/helingzh/workplace/summer2016/Run2/FourMuonAna/FourMuonAnalysis/cmt/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$/atlas/data18a/helingzh/workplace/summer2016/Run2/FourMuonAna/FourMuonAnalysis/Root/LinkDef.h")))  XFourMuonAnalysis;
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
  #define ROOTCORE_PACKAGE "FourMuonAnalysis"
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "FourMuonAnalysis/XFourMuonAnalysis.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"XFourMuonAnalysis", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("FourMuonAnalysisCINT",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_FourMuonAnalysisCINT_Impl, {{"namespace DataVector_detail { template <typename B1, typename B2, typename B3> class VirtBases; }", 1},{"template <typename T> class DataVectorBase;", 1},{"template <typename T, typename BASE> class DataVector;", 1},{"namespace DataVector_detail { template <typename T> class DVLEltBaseInit; }", 1}}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_FourMuonAnalysisCINT_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_FourMuonAnalysisCINT() {
  TriggerDictionaryInitialization_FourMuonAnalysisCINT_Impl();
}
