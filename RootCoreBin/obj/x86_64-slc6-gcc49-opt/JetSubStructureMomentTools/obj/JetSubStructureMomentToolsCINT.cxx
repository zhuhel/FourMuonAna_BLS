#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#ifdef __llvm__
#pragma GCC diagnostic ignored "-Wunused-private-field"
#endif
// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIatlasdIdata18adIhelingzhdIworkplacedIsummer2016dIRun2dIFourMuonAnadIRootCoreBindIobjdIx86_64mIslc6mIgcc49mIoptdIJetSubStructureMomentToolsdIobjdIJetSubStructureMomentToolsCINT

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
#include "JetSubStructureMomentTools/AngularityTool.h"
#include "JetSubStructureMomentTools/JetSubStructureMomentToolsBase.h"
#include "JetSubStructureMomentTools/DipolarityTool.h"
#include "JetSubStructureMomentTools/JetChargeTool.h"
#include "JetSubStructureMomentTools/NSubjettinessRatiosTool.h"
#include "JetSubStructureMomentTools/ShowerDeconstructionTool.h"
#include "JetSubStructureMomentTools/BoostedXbbTagTool.h"
#include "JetSubStructureMomentTools/EnergyCorrelatorRatiosTool.h"
#include "JetSubStructureMomentTools/JetPullTool.h"
#include "JetSubStructureMomentTools/KTSplittingScaleTool.h"
#include "JetSubStructureMomentTools/NSubjettinessTool.h"
#include "JetSubStructureMomentTools/SubjetFinderTool.h"
#include "JetSubStructureMomentTools/ISubjetRecorderTool.h"
#include "JetSubStructureMomentTools/VolatilityTool.h"
#include "JetSubStructureMomentTools/BosonTagTool.h"
#include "JetSubStructureMomentTools/EnergyCorrelatorTool.h"
#include "JetSubStructureMomentTools/KtDeltaRTool.h"
#include "JetSubStructureMomentTools/PlanarFlowTool.h"
#include "JetSubStructureMomentTools/SubjetMakerTool.h"
#include "JetSubStructureMomentTools/CenterOfMassShapesTool.h"
#include "JetSubStructureMomentTools/KtMassDropTool.h"
#include "JetSubStructureMomentTools/QwTool.h"
#include "JetSubStructureMomentTools/SubjetRecorderTool.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *AngularityTool_Dictionary();
   static void AngularityTool_TClassManip(TClass*);
   static void delete_AngularityTool(void *p);
   static void deleteArray_AngularityTool(void *p);
   static void destruct_AngularityTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::AngularityTool*)
   {
      ::AngularityTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::AngularityTool));
      static ::ROOT::TGenericClassInfo 
         instance("AngularityTool", "JetSubStructureMomentTools/AngularityTool.h", 6,
                  typeid(::AngularityTool), DefineBehavior(ptr, ptr),
                  &AngularityTool_Dictionary, isa_proxy, 4,
                  sizeof(::AngularityTool) );
      instance.SetDelete(&delete_AngularityTool);
      instance.SetDeleteArray(&deleteArray_AngularityTool);
      instance.SetDestructor(&destruct_AngularityTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::AngularityTool*)
   {
      return GenerateInitInstanceLocal((::AngularityTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::AngularityTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *AngularityTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::AngularityTool*)0x0)->GetClass();
      AngularityTool_TClassManip(theClass);
   return theClass;
   }

   static void AngularityTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *BoostedXbbTagTool_Dictionary();
   static void BoostedXbbTagTool_TClassManip(TClass*);
   static void delete_BoostedXbbTagTool(void *p);
   static void deleteArray_BoostedXbbTagTool(void *p);
   static void destruct_BoostedXbbTagTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::BoostedXbbTagTool*)
   {
      ::BoostedXbbTagTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::BoostedXbbTagTool));
      static ::ROOT::TGenericClassInfo 
         instance("BoostedXbbTagTool", "JetSubStructureMomentTools/BoostedXbbTagTool.h", 6,
                  typeid(::BoostedXbbTagTool), DefineBehavior(ptr, ptr),
                  &BoostedXbbTagTool_Dictionary, isa_proxy, 4,
                  sizeof(::BoostedXbbTagTool) );
      instance.SetDelete(&delete_BoostedXbbTagTool);
      instance.SetDeleteArray(&deleteArray_BoostedXbbTagTool);
      instance.SetDestructor(&destruct_BoostedXbbTagTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::BoostedXbbTagTool*)
   {
      return GenerateInitInstanceLocal((::BoostedXbbTagTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::BoostedXbbTagTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *BoostedXbbTagTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::BoostedXbbTagTool*)0x0)->GetClass();
      BoostedXbbTagTool_TClassManip(theClass);
   return theClass;
   }

   static void BoostedXbbTagTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *BosonTagTool_Dictionary();
   static void BosonTagTool_TClassManip(TClass*);
   static void delete_BosonTagTool(void *p);
   static void deleteArray_BosonTagTool(void *p);
   static void destruct_BosonTagTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::BosonTagTool*)
   {
      ::BosonTagTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::BosonTagTool));
      static ::ROOT::TGenericClassInfo 
         instance("BosonTagTool", "JetSubStructureMomentTools/BosonTagTool.h", 6,
                  typeid(::BosonTagTool), DefineBehavior(ptr, ptr),
                  &BosonTagTool_Dictionary, isa_proxy, 4,
                  sizeof(::BosonTagTool) );
      instance.SetDelete(&delete_BosonTagTool);
      instance.SetDeleteArray(&deleteArray_BosonTagTool);
      instance.SetDestructor(&destruct_BosonTagTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::BosonTagTool*)
   {
      return GenerateInitInstanceLocal((::BosonTagTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::BosonTagTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *BosonTagTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::BosonTagTool*)0x0)->GetClass();
      BosonTagTool_TClassManip(theClass);
   return theClass;
   }

   static void BosonTagTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *CenterOfMassShapesTool_Dictionary();
   static void CenterOfMassShapesTool_TClassManip(TClass*);
   static void delete_CenterOfMassShapesTool(void *p);
   static void deleteArray_CenterOfMassShapesTool(void *p);
   static void destruct_CenterOfMassShapesTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CenterOfMassShapesTool*)
   {
      ::CenterOfMassShapesTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::CenterOfMassShapesTool));
      static ::ROOT::TGenericClassInfo 
         instance("CenterOfMassShapesTool", "JetSubStructureMomentTools/CenterOfMassShapesTool.h", 6,
                  typeid(::CenterOfMassShapesTool), DefineBehavior(ptr, ptr),
                  &CenterOfMassShapesTool_Dictionary, isa_proxy, 4,
                  sizeof(::CenterOfMassShapesTool) );
      instance.SetDelete(&delete_CenterOfMassShapesTool);
      instance.SetDeleteArray(&deleteArray_CenterOfMassShapesTool);
      instance.SetDestructor(&destruct_CenterOfMassShapesTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CenterOfMassShapesTool*)
   {
      return GenerateInitInstanceLocal((::CenterOfMassShapesTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::CenterOfMassShapesTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *CenterOfMassShapesTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::CenterOfMassShapesTool*)0x0)->GetClass();
      CenterOfMassShapesTool_TClassManip(theClass);
   return theClass;
   }

   static void CenterOfMassShapesTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *DipolarityTool_Dictionary();
   static void DipolarityTool_TClassManip(TClass*);
   static void delete_DipolarityTool(void *p);
   static void deleteArray_DipolarityTool(void *p);
   static void destruct_DipolarityTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DipolarityTool*)
   {
      ::DipolarityTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::DipolarityTool));
      static ::ROOT::TGenericClassInfo 
         instance("DipolarityTool", "JetSubStructureMomentTools/DipolarityTool.h", 6,
                  typeid(::DipolarityTool), DefineBehavior(ptr, ptr),
                  &DipolarityTool_Dictionary, isa_proxy, 4,
                  sizeof(::DipolarityTool) );
      instance.SetDelete(&delete_DipolarityTool);
      instance.SetDeleteArray(&deleteArray_DipolarityTool);
      instance.SetDestructor(&destruct_DipolarityTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DipolarityTool*)
   {
      return GenerateInitInstanceLocal((::DipolarityTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DipolarityTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *DipolarityTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::DipolarityTool*)0x0)->GetClass();
      DipolarityTool_TClassManip(theClass);
   return theClass;
   }

   static void DipolarityTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *EnergyCorrelatorRatiosTool_Dictionary();
   static void EnergyCorrelatorRatiosTool_TClassManip(TClass*);
   static void delete_EnergyCorrelatorRatiosTool(void *p);
   static void deleteArray_EnergyCorrelatorRatiosTool(void *p);
   static void destruct_EnergyCorrelatorRatiosTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EnergyCorrelatorRatiosTool*)
   {
      ::EnergyCorrelatorRatiosTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::EnergyCorrelatorRatiosTool));
      static ::ROOT::TGenericClassInfo 
         instance("EnergyCorrelatorRatiosTool", "JetSubStructureMomentTools/EnergyCorrelatorRatiosTool.h", 6,
                  typeid(::EnergyCorrelatorRatiosTool), DefineBehavior(ptr, ptr),
                  &EnergyCorrelatorRatiosTool_Dictionary, isa_proxy, 4,
                  sizeof(::EnergyCorrelatorRatiosTool) );
      instance.SetDelete(&delete_EnergyCorrelatorRatiosTool);
      instance.SetDeleteArray(&deleteArray_EnergyCorrelatorRatiosTool);
      instance.SetDestructor(&destruct_EnergyCorrelatorRatiosTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EnergyCorrelatorRatiosTool*)
   {
      return GenerateInitInstanceLocal((::EnergyCorrelatorRatiosTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::EnergyCorrelatorRatiosTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *EnergyCorrelatorRatiosTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::EnergyCorrelatorRatiosTool*)0x0)->GetClass();
      EnergyCorrelatorRatiosTool_TClassManip(theClass);
   return theClass;
   }

   static void EnergyCorrelatorRatiosTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *EnergyCorrelatorTool_Dictionary();
   static void EnergyCorrelatorTool_TClassManip(TClass*);
   static void delete_EnergyCorrelatorTool(void *p);
   static void deleteArray_EnergyCorrelatorTool(void *p);
   static void destruct_EnergyCorrelatorTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::EnergyCorrelatorTool*)
   {
      ::EnergyCorrelatorTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::EnergyCorrelatorTool));
      static ::ROOT::TGenericClassInfo 
         instance("EnergyCorrelatorTool", "JetSubStructureMomentTools/EnergyCorrelatorTool.h", 6,
                  typeid(::EnergyCorrelatorTool), DefineBehavior(ptr, ptr),
                  &EnergyCorrelatorTool_Dictionary, isa_proxy, 4,
                  sizeof(::EnergyCorrelatorTool) );
      instance.SetDelete(&delete_EnergyCorrelatorTool);
      instance.SetDeleteArray(&deleteArray_EnergyCorrelatorTool);
      instance.SetDestructor(&destruct_EnergyCorrelatorTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::EnergyCorrelatorTool*)
   {
      return GenerateInitInstanceLocal((::EnergyCorrelatorTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::EnergyCorrelatorTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *EnergyCorrelatorTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::EnergyCorrelatorTool*)0x0)->GetClass();
      EnergyCorrelatorTool_TClassManip(theClass);
   return theClass;
   }

   static void EnergyCorrelatorTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ISubjetRecorderTool_Dictionary();
   static void ISubjetRecorderTool_TClassManip(TClass*);
   static void delete_ISubjetRecorderTool(void *p);
   static void deleteArray_ISubjetRecorderTool(void *p);
   static void destruct_ISubjetRecorderTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ISubjetRecorderTool*)
   {
      ::ISubjetRecorderTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ISubjetRecorderTool));
      static ::ROOT::TGenericClassInfo 
         instance("ISubjetRecorderTool", "JetSubStructureMomentTools/ISubjetRecorderTool.h", 12,
                  typeid(::ISubjetRecorderTool), DefineBehavior(ptr, ptr),
                  &ISubjetRecorderTool_Dictionary, isa_proxy, 4,
                  sizeof(::ISubjetRecorderTool) );
      instance.SetDelete(&delete_ISubjetRecorderTool);
      instance.SetDeleteArray(&deleteArray_ISubjetRecorderTool);
      instance.SetDestructor(&destruct_ISubjetRecorderTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ISubjetRecorderTool*)
   {
      return GenerateInitInstanceLocal((::ISubjetRecorderTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ISubjetRecorderTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ISubjetRecorderTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ISubjetRecorderTool*)0x0)->GetClass();
      ISubjetRecorderTool_TClassManip(theClass);
   return theClass;
   }

   static void ISubjetRecorderTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *JetChargeTool_Dictionary();
   static void JetChargeTool_TClassManip(TClass*);
   static void delete_JetChargeTool(void *p);
   static void deleteArray_JetChargeTool(void *p);
   static void destruct_JetChargeTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JetChargeTool*)
   {
      ::JetChargeTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::JetChargeTool));
      static ::ROOT::TGenericClassInfo 
         instance("JetChargeTool", "JetSubStructureMomentTools/JetChargeTool.h", 8,
                  typeid(::JetChargeTool), DefineBehavior(ptr, ptr),
                  &JetChargeTool_Dictionary, isa_proxy, 4,
                  sizeof(::JetChargeTool) );
      instance.SetDelete(&delete_JetChargeTool);
      instance.SetDeleteArray(&deleteArray_JetChargeTool);
      instance.SetDestructor(&destruct_JetChargeTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JetChargeTool*)
   {
      return GenerateInitInstanceLocal((::JetChargeTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::JetChargeTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *JetChargeTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::JetChargeTool*)0x0)->GetClass();
      JetChargeTool_TClassManip(theClass);
   return theClass;
   }

   static void JetChargeTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *JetPullTool_Dictionary();
   static void JetPullTool_TClassManip(TClass*);
   static void delete_JetPullTool(void *p);
   static void deleteArray_JetPullTool(void *p);
   static void destruct_JetPullTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JetPullTool*)
   {
      ::JetPullTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::JetPullTool));
      static ::ROOT::TGenericClassInfo 
         instance("JetPullTool", "JetSubStructureMomentTools/JetPullTool.h", 8,
                  typeid(::JetPullTool), DefineBehavior(ptr, ptr),
                  &JetPullTool_Dictionary, isa_proxy, 4,
                  sizeof(::JetPullTool) );
      instance.SetDelete(&delete_JetPullTool);
      instance.SetDeleteArray(&deleteArray_JetPullTool);
      instance.SetDestructor(&destruct_JetPullTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JetPullTool*)
   {
      return GenerateInitInstanceLocal((::JetPullTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::JetPullTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *JetPullTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::JetPullTool*)0x0)->GetClass();
      JetPullTool_TClassManip(theClass);
   return theClass;
   }

   static void JetPullTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *JetSubStructureMomentToolsBase_Dictionary();
   static void JetSubStructureMomentToolsBase_TClassManip(TClass*);
   static void delete_JetSubStructureMomentToolsBase(void *p);
   static void deleteArray_JetSubStructureMomentToolsBase(void *p);
   static void destruct_JetSubStructureMomentToolsBase(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JetSubStructureMomentToolsBase*)
   {
      ::JetSubStructureMomentToolsBase *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::JetSubStructureMomentToolsBase));
      static ::ROOT::TGenericClassInfo 
         instance("JetSubStructureMomentToolsBase", "JetSubStructureMomentTools/JetSubStructureMomentToolsBase.h", 17,
                  typeid(::JetSubStructureMomentToolsBase), DefineBehavior(ptr, ptr),
                  &JetSubStructureMomentToolsBase_Dictionary, isa_proxy, 4,
                  sizeof(::JetSubStructureMomentToolsBase) );
      instance.SetDelete(&delete_JetSubStructureMomentToolsBase);
      instance.SetDeleteArray(&deleteArray_JetSubStructureMomentToolsBase);
      instance.SetDestructor(&destruct_JetSubStructureMomentToolsBase);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JetSubStructureMomentToolsBase*)
   {
      return GenerateInitInstanceLocal((::JetSubStructureMomentToolsBase*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::JetSubStructureMomentToolsBase*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *JetSubStructureMomentToolsBase_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::JetSubStructureMomentToolsBase*)0x0)->GetClass();
      JetSubStructureMomentToolsBase_TClassManip(theClass);
   return theClass;
   }

   static void JetSubStructureMomentToolsBase_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *KTSplittingScaleTool_Dictionary();
   static void KTSplittingScaleTool_TClassManip(TClass*);
   static void delete_KTSplittingScaleTool(void *p);
   static void deleteArray_KTSplittingScaleTool(void *p);
   static void destruct_KTSplittingScaleTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::KTSplittingScaleTool*)
   {
      ::KTSplittingScaleTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::KTSplittingScaleTool));
      static ::ROOT::TGenericClassInfo 
         instance("KTSplittingScaleTool", "JetSubStructureMomentTools/KTSplittingScaleTool.h", 6,
                  typeid(::KTSplittingScaleTool), DefineBehavior(ptr, ptr),
                  &KTSplittingScaleTool_Dictionary, isa_proxy, 4,
                  sizeof(::KTSplittingScaleTool) );
      instance.SetDelete(&delete_KTSplittingScaleTool);
      instance.SetDeleteArray(&deleteArray_KTSplittingScaleTool);
      instance.SetDestructor(&destruct_KTSplittingScaleTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::KTSplittingScaleTool*)
   {
      return GenerateInitInstanceLocal((::KTSplittingScaleTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::KTSplittingScaleTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *KTSplittingScaleTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::KTSplittingScaleTool*)0x0)->GetClass();
      KTSplittingScaleTool_TClassManip(theClass);
   return theClass;
   }

   static void KTSplittingScaleTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *KtDeltaRTool_Dictionary();
   static void KtDeltaRTool_TClassManip(TClass*);
   static void delete_KtDeltaRTool(void *p);
   static void deleteArray_KtDeltaRTool(void *p);
   static void destruct_KtDeltaRTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::KtDeltaRTool*)
   {
      ::KtDeltaRTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::KtDeltaRTool));
      static ::ROOT::TGenericClassInfo 
         instance("KtDeltaRTool", "JetSubStructureMomentTools/KtDeltaRTool.h", 6,
                  typeid(::KtDeltaRTool), DefineBehavior(ptr, ptr),
                  &KtDeltaRTool_Dictionary, isa_proxy, 4,
                  sizeof(::KtDeltaRTool) );
      instance.SetDelete(&delete_KtDeltaRTool);
      instance.SetDeleteArray(&deleteArray_KtDeltaRTool);
      instance.SetDestructor(&destruct_KtDeltaRTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::KtDeltaRTool*)
   {
      return GenerateInitInstanceLocal((::KtDeltaRTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::KtDeltaRTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *KtDeltaRTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::KtDeltaRTool*)0x0)->GetClass();
      KtDeltaRTool_TClassManip(theClass);
   return theClass;
   }

   static void KtDeltaRTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *KtMassDropTool_Dictionary();
   static void KtMassDropTool_TClassManip(TClass*);
   static void delete_KtMassDropTool(void *p);
   static void deleteArray_KtMassDropTool(void *p);
   static void destruct_KtMassDropTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::KtMassDropTool*)
   {
      ::KtMassDropTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::KtMassDropTool));
      static ::ROOT::TGenericClassInfo 
         instance("KtMassDropTool", "JetSubStructureMomentTools/KtMassDropTool.h", 6,
                  typeid(::KtMassDropTool), DefineBehavior(ptr, ptr),
                  &KtMassDropTool_Dictionary, isa_proxy, 4,
                  sizeof(::KtMassDropTool) );
      instance.SetDelete(&delete_KtMassDropTool);
      instance.SetDeleteArray(&deleteArray_KtMassDropTool);
      instance.SetDestructor(&destruct_KtMassDropTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::KtMassDropTool*)
   {
      return GenerateInitInstanceLocal((::KtMassDropTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::KtMassDropTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *KtMassDropTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::KtMassDropTool*)0x0)->GetClass();
      KtMassDropTool_TClassManip(theClass);
   return theClass;
   }

   static void KtMassDropTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NSubjettinessRatiosTool_Dictionary();
   static void NSubjettinessRatiosTool_TClassManip(TClass*);
   static void delete_NSubjettinessRatiosTool(void *p);
   static void deleteArray_NSubjettinessRatiosTool(void *p);
   static void destruct_NSubjettinessRatiosTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NSubjettinessRatiosTool*)
   {
      ::NSubjettinessRatiosTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::NSubjettinessRatiosTool));
      static ::ROOT::TGenericClassInfo 
         instance("NSubjettinessRatiosTool", "JetSubStructureMomentTools/NSubjettinessRatiosTool.h", 6,
                  typeid(::NSubjettinessRatiosTool), DefineBehavior(ptr, ptr),
                  &NSubjettinessRatiosTool_Dictionary, isa_proxy, 4,
                  sizeof(::NSubjettinessRatiosTool) );
      instance.SetDelete(&delete_NSubjettinessRatiosTool);
      instance.SetDeleteArray(&deleteArray_NSubjettinessRatiosTool);
      instance.SetDestructor(&destruct_NSubjettinessRatiosTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NSubjettinessRatiosTool*)
   {
      return GenerateInitInstanceLocal((::NSubjettinessRatiosTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::NSubjettinessRatiosTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NSubjettinessRatiosTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NSubjettinessRatiosTool*)0x0)->GetClass();
      NSubjettinessRatiosTool_TClassManip(theClass);
   return theClass;
   }

   static void NSubjettinessRatiosTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *NSubjettinessTool_Dictionary();
   static void NSubjettinessTool_TClassManip(TClass*);
   static void delete_NSubjettinessTool(void *p);
   static void deleteArray_NSubjettinessTool(void *p);
   static void destruct_NSubjettinessTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::NSubjettinessTool*)
   {
      ::NSubjettinessTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::NSubjettinessTool));
      static ::ROOT::TGenericClassInfo 
         instance("NSubjettinessTool", "JetSubStructureMomentTools/NSubjettinessTool.h", 6,
                  typeid(::NSubjettinessTool), DefineBehavior(ptr, ptr),
                  &NSubjettinessTool_Dictionary, isa_proxy, 4,
                  sizeof(::NSubjettinessTool) );
      instance.SetDelete(&delete_NSubjettinessTool);
      instance.SetDeleteArray(&deleteArray_NSubjettinessTool);
      instance.SetDestructor(&destruct_NSubjettinessTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::NSubjettinessTool*)
   {
      return GenerateInitInstanceLocal((::NSubjettinessTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::NSubjettinessTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *NSubjettinessTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::NSubjettinessTool*)0x0)->GetClass();
      NSubjettinessTool_TClassManip(theClass);
   return theClass;
   }

   static void NSubjettinessTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *PlanarFlowTool_Dictionary();
   static void PlanarFlowTool_TClassManip(TClass*);
   static void delete_PlanarFlowTool(void *p);
   static void deleteArray_PlanarFlowTool(void *p);
   static void destruct_PlanarFlowTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PlanarFlowTool*)
   {
      ::PlanarFlowTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PlanarFlowTool));
      static ::ROOT::TGenericClassInfo 
         instance("PlanarFlowTool", "JetSubStructureMomentTools/PlanarFlowTool.h", 6,
                  typeid(::PlanarFlowTool), DefineBehavior(ptr, ptr),
                  &PlanarFlowTool_Dictionary, isa_proxy, 4,
                  sizeof(::PlanarFlowTool) );
      instance.SetDelete(&delete_PlanarFlowTool);
      instance.SetDeleteArray(&deleteArray_PlanarFlowTool);
      instance.SetDestructor(&destruct_PlanarFlowTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PlanarFlowTool*)
   {
      return GenerateInitInstanceLocal((::PlanarFlowTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PlanarFlowTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PlanarFlowTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PlanarFlowTool*)0x0)->GetClass();
      PlanarFlowTool_TClassManip(theClass);
   return theClass;
   }

   static void PlanarFlowTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *QwTool_Dictionary();
   static void QwTool_TClassManip(TClass*);
   static void delete_QwTool(void *p);
   static void deleteArray_QwTool(void *p);
   static void destruct_QwTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::QwTool*)
   {
      ::QwTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::QwTool));
      static ::ROOT::TGenericClassInfo 
         instance("QwTool", "JetSubStructureMomentTools/QwTool.h", 15,
                  typeid(::QwTool), DefineBehavior(ptr, ptr),
                  &QwTool_Dictionary, isa_proxy, 4,
                  sizeof(::QwTool) );
      instance.SetDelete(&delete_QwTool);
      instance.SetDeleteArray(&deleteArray_QwTool);
      instance.SetDestructor(&destruct_QwTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::QwTool*)
   {
      return GenerateInitInstanceLocal((::QwTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::QwTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *QwTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::QwTool*)0x0)->GetClass();
      QwTool_TClassManip(theClass);
   return theClass;
   }

   static void QwTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ShowerDeconstructionTool_Dictionary();
   static void ShowerDeconstructionTool_TClassManip(TClass*);
   static void delete_ShowerDeconstructionTool(void *p);
   static void deleteArray_ShowerDeconstructionTool(void *p);
   static void destruct_ShowerDeconstructionTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ShowerDeconstructionTool*)
   {
      ::ShowerDeconstructionTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ShowerDeconstructionTool));
      static ::ROOT::TGenericClassInfo 
         instance("ShowerDeconstructionTool", "JetSubStructureMomentTools/ShowerDeconstructionTool.h", 6,
                  typeid(::ShowerDeconstructionTool), DefineBehavior(ptr, ptr),
                  &ShowerDeconstructionTool_Dictionary, isa_proxy, 4,
                  sizeof(::ShowerDeconstructionTool) );
      instance.SetDelete(&delete_ShowerDeconstructionTool);
      instance.SetDeleteArray(&deleteArray_ShowerDeconstructionTool);
      instance.SetDestructor(&destruct_ShowerDeconstructionTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ShowerDeconstructionTool*)
   {
      return GenerateInitInstanceLocal((::ShowerDeconstructionTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::ShowerDeconstructionTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ShowerDeconstructionTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ShowerDeconstructionTool*)0x0)->GetClass();
      ShowerDeconstructionTool_TClassManip(theClass);
   return theClass;
   }

   static void ShowerDeconstructionTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *SubjetFinderTool_Dictionary();
   static void SubjetFinderTool_TClassManip(TClass*);
   static void delete_SubjetFinderTool(void *p);
   static void deleteArray_SubjetFinderTool(void *p);
   static void destruct_SubjetFinderTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SubjetFinderTool*)
   {
      ::SubjetFinderTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SubjetFinderTool));
      static ::ROOT::TGenericClassInfo 
         instance("SubjetFinderTool", "JetSubStructureMomentTools/SubjetFinderTool.h", 8,
                  typeid(::SubjetFinderTool), DefineBehavior(ptr, ptr),
                  &SubjetFinderTool_Dictionary, isa_proxy, 4,
                  sizeof(::SubjetFinderTool) );
      instance.SetDelete(&delete_SubjetFinderTool);
      instance.SetDeleteArray(&deleteArray_SubjetFinderTool);
      instance.SetDestructor(&destruct_SubjetFinderTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SubjetFinderTool*)
   {
      return GenerateInitInstanceLocal((::SubjetFinderTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SubjetFinderTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SubjetFinderTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SubjetFinderTool*)0x0)->GetClass();
      SubjetFinderTool_TClassManip(theClass);
   return theClass;
   }

   static void SubjetFinderTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *SubjetMakerTool_Dictionary();
   static void SubjetMakerTool_TClassManip(TClass*);
   static void delete_SubjetMakerTool(void *p);
   static void deleteArray_SubjetMakerTool(void *p);
   static void destruct_SubjetMakerTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SubjetMakerTool*)
   {
      ::SubjetMakerTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SubjetMakerTool));
      static ::ROOT::TGenericClassInfo 
         instance("SubjetMakerTool", "JetSubStructureMomentTools/SubjetMakerTool.h", 8,
                  typeid(::SubjetMakerTool), DefineBehavior(ptr, ptr),
                  &SubjetMakerTool_Dictionary, isa_proxy, 4,
                  sizeof(::SubjetMakerTool) );
      instance.SetDelete(&delete_SubjetMakerTool);
      instance.SetDeleteArray(&deleteArray_SubjetMakerTool);
      instance.SetDestructor(&destruct_SubjetMakerTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SubjetMakerTool*)
   {
      return GenerateInitInstanceLocal((::SubjetMakerTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SubjetMakerTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SubjetMakerTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SubjetMakerTool*)0x0)->GetClass();
      SubjetMakerTool_TClassManip(theClass);
   return theClass;
   }

   static void SubjetMakerTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *SubjetRecorderTool_Dictionary();
   static void SubjetRecorderTool_TClassManip(TClass*);
   static void delete_SubjetRecorderTool(void *p);
   static void deleteArray_SubjetRecorderTool(void *p);
   static void destruct_SubjetRecorderTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SubjetRecorderTool*)
   {
      ::SubjetRecorderTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::SubjetRecorderTool));
      static ::ROOT::TGenericClassInfo 
         instance("SubjetRecorderTool", "JetSubStructureMomentTools/SubjetRecorderTool.h", 13,
                  typeid(::SubjetRecorderTool), DefineBehavior(ptr, ptr),
                  &SubjetRecorderTool_Dictionary, isa_proxy, 4,
                  sizeof(::SubjetRecorderTool) );
      instance.SetDelete(&delete_SubjetRecorderTool);
      instance.SetDeleteArray(&deleteArray_SubjetRecorderTool);
      instance.SetDestructor(&destruct_SubjetRecorderTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SubjetRecorderTool*)
   {
      return GenerateInitInstanceLocal((::SubjetRecorderTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SubjetRecorderTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *SubjetRecorderTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::SubjetRecorderTool*)0x0)->GetClass();
      SubjetRecorderTool_TClassManip(theClass);
   return theClass;
   }

   static void SubjetRecorderTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *VolatilityTool_Dictionary();
   static void VolatilityTool_TClassManip(TClass*);
   static void delete_VolatilityTool(void *p);
   static void deleteArray_VolatilityTool(void *p);
   static void destruct_VolatilityTool(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::VolatilityTool*)
   {
      ::VolatilityTool *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::VolatilityTool));
      static ::ROOT::TGenericClassInfo 
         instance("VolatilityTool", "JetSubStructureMomentTools/VolatilityTool.h", 6,
                  typeid(::VolatilityTool), DefineBehavior(ptr, ptr),
                  &VolatilityTool_Dictionary, isa_proxy, 4,
                  sizeof(::VolatilityTool) );
      instance.SetDelete(&delete_VolatilityTool);
      instance.SetDeleteArray(&deleteArray_VolatilityTool);
      instance.SetDestructor(&destruct_VolatilityTool);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::VolatilityTool*)
   {
      return GenerateInitInstanceLocal((::VolatilityTool*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::VolatilityTool*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *VolatilityTool_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::VolatilityTool*)0x0)->GetClass();
      VolatilityTool_TClassManip(theClass);
   return theClass;
   }

   static void VolatilityTool_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrapper around operator delete
   static void delete_AngularityTool(void *p) {
      delete ((::AngularityTool*)p);
   }
   static void deleteArray_AngularityTool(void *p) {
      delete [] ((::AngularityTool*)p);
   }
   static void destruct_AngularityTool(void *p) {
      typedef ::AngularityTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::AngularityTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_BoostedXbbTagTool(void *p) {
      delete ((::BoostedXbbTagTool*)p);
   }
   static void deleteArray_BoostedXbbTagTool(void *p) {
      delete [] ((::BoostedXbbTagTool*)p);
   }
   static void destruct_BoostedXbbTagTool(void *p) {
      typedef ::BoostedXbbTagTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::BoostedXbbTagTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_BosonTagTool(void *p) {
      delete ((::BosonTagTool*)p);
   }
   static void deleteArray_BosonTagTool(void *p) {
      delete [] ((::BosonTagTool*)p);
   }
   static void destruct_BosonTagTool(void *p) {
      typedef ::BosonTagTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::BosonTagTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_CenterOfMassShapesTool(void *p) {
      delete ((::CenterOfMassShapesTool*)p);
   }
   static void deleteArray_CenterOfMassShapesTool(void *p) {
      delete [] ((::CenterOfMassShapesTool*)p);
   }
   static void destruct_CenterOfMassShapesTool(void *p) {
      typedef ::CenterOfMassShapesTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CenterOfMassShapesTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_DipolarityTool(void *p) {
      delete ((::DipolarityTool*)p);
   }
   static void deleteArray_DipolarityTool(void *p) {
      delete [] ((::DipolarityTool*)p);
   }
   static void destruct_DipolarityTool(void *p) {
      typedef ::DipolarityTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DipolarityTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_EnergyCorrelatorRatiosTool(void *p) {
      delete ((::EnergyCorrelatorRatiosTool*)p);
   }
   static void deleteArray_EnergyCorrelatorRatiosTool(void *p) {
      delete [] ((::EnergyCorrelatorRatiosTool*)p);
   }
   static void destruct_EnergyCorrelatorRatiosTool(void *p) {
      typedef ::EnergyCorrelatorRatiosTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::EnergyCorrelatorRatiosTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_EnergyCorrelatorTool(void *p) {
      delete ((::EnergyCorrelatorTool*)p);
   }
   static void deleteArray_EnergyCorrelatorTool(void *p) {
      delete [] ((::EnergyCorrelatorTool*)p);
   }
   static void destruct_EnergyCorrelatorTool(void *p) {
      typedef ::EnergyCorrelatorTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::EnergyCorrelatorTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ISubjetRecorderTool(void *p) {
      delete ((::ISubjetRecorderTool*)p);
   }
   static void deleteArray_ISubjetRecorderTool(void *p) {
      delete [] ((::ISubjetRecorderTool*)p);
   }
   static void destruct_ISubjetRecorderTool(void *p) {
      typedef ::ISubjetRecorderTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ISubjetRecorderTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_JetChargeTool(void *p) {
      delete ((::JetChargeTool*)p);
   }
   static void deleteArray_JetChargeTool(void *p) {
      delete [] ((::JetChargeTool*)p);
   }
   static void destruct_JetChargeTool(void *p) {
      typedef ::JetChargeTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JetChargeTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_JetPullTool(void *p) {
      delete ((::JetPullTool*)p);
   }
   static void deleteArray_JetPullTool(void *p) {
      delete [] ((::JetPullTool*)p);
   }
   static void destruct_JetPullTool(void *p) {
      typedef ::JetPullTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JetPullTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_JetSubStructureMomentToolsBase(void *p) {
      delete ((::JetSubStructureMomentToolsBase*)p);
   }
   static void deleteArray_JetSubStructureMomentToolsBase(void *p) {
      delete [] ((::JetSubStructureMomentToolsBase*)p);
   }
   static void destruct_JetSubStructureMomentToolsBase(void *p) {
      typedef ::JetSubStructureMomentToolsBase current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JetSubStructureMomentToolsBase

namespace ROOT {
   // Wrapper around operator delete
   static void delete_KTSplittingScaleTool(void *p) {
      delete ((::KTSplittingScaleTool*)p);
   }
   static void deleteArray_KTSplittingScaleTool(void *p) {
      delete [] ((::KTSplittingScaleTool*)p);
   }
   static void destruct_KTSplittingScaleTool(void *p) {
      typedef ::KTSplittingScaleTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::KTSplittingScaleTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_KtDeltaRTool(void *p) {
      delete ((::KtDeltaRTool*)p);
   }
   static void deleteArray_KtDeltaRTool(void *p) {
      delete [] ((::KtDeltaRTool*)p);
   }
   static void destruct_KtDeltaRTool(void *p) {
      typedef ::KtDeltaRTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::KtDeltaRTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_KtMassDropTool(void *p) {
      delete ((::KtMassDropTool*)p);
   }
   static void deleteArray_KtMassDropTool(void *p) {
      delete [] ((::KtMassDropTool*)p);
   }
   static void destruct_KtMassDropTool(void *p) {
      typedef ::KtMassDropTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::KtMassDropTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NSubjettinessRatiosTool(void *p) {
      delete ((::NSubjettinessRatiosTool*)p);
   }
   static void deleteArray_NSubjettinessRatiosTool(void *p) {
      delete [] ((::NSubjettinessRatiosTool*)p);
   }
   static void destruct_NSubjettinessRatiosTool(void *p) {
      typedef ::NSubjettinessRatiosTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NSubjettinessRatiosTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_NSubjettinessTool(void *p) {
      delete ((::NSubjettinessTool*)p);
   }
   static void deleteArray_NSubjettinessTool(void *p) {
      delete [] ((::NSubjettinessTool*)p);
   }
   static void destruct_NSubjettinessTool(void *p) {
      typedef ::NSubjettinessTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::NSubjettinessTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_PlanarFlowTool(void *p) {
      delete ((::PlanarFlowTool*)p);
   }
   static void deleteArray_PlanarFlowTool(void *p) {
      delete [] ((::PlanarFlowTool*)p);
   }
   static void destruct_PlanarFlowTool(void *p) {
      typedef ::PlanarFlowTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PlanarFlowTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_QwTool(void *p) {
      delete ((::QwTool*)p);
   }
   static void deleteArray_QwTool(void *p) {
      delete [] ((::QwTool*)p);
   }
   static void destruct_QwTool(void *p) {
      typedef ::QwTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::QwTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_ShowerDeconstructionTool(void *p) {
      delete ((::ShowerDeconstructionTool*)p);
   }
   static void deleteArray_ShowerDeconstructionTool(void *p) {
      delete [] ((::ShowerDeconstructionTool*)p);
   }
   static void destruct_ShowerDeconstructionTool(void *p) {
      typedef ::ShowerDeconstructionTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ShowerDeconstructionTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SubjetFinderTool(void *p) {
      delete ((::SubjetFinderTool*)p);
   }
   static void deleteArray_SubjetFinderTool(void *p) {
      delete [] ((::SubjetFinderTool*)p);
   }
   static void destruct_SubjetFinderTool(void *p) {
      typedef ::SubjetFinderTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SubjetFinderTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SubjetMakerTool(void *p) {
      delete ((::SubjetMakerTool*)p);
   }
   static void deleteArray_SubjetMakerTool(void *p) {
      delete [] ((::SubjetMakerTool*)p);
   }
   static void destruct_SubjetMakerTool(void *p) {
      typedef ::SubjetMakerTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SubjetMakerTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_SubjetRecorderTool(void *p) {
      delete ((::SubjetRecorderTool*)p);
   }
   static void deleteArray_SubjetRecorderTool(void *p) {
      delete [] ((::SubjetRecorderTool*)p);
   }
   static void destruct_SubjetRecorderTool(void *p) {
      typedef ::SubjetRecorderTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SubjetRecorderTool

namespace ROOT {
   // Wrapper around operator delete
   static void delete_VolatilityTool(void *p) {
      delete ((::VolatilityTool*)p);
   }
   static void deleteArray_VolatilityTool(void *p) {
      delete [] ((::VolatilityTool*)p);
   }
   static void destruct_VolatilityTool(void *p) {
      typedef ::VolatilityTool current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::VolatilityTool

namespace {
  void TriggerDictionaryInitialization_JetSubStructureMomentToolsCINT_Impl() {
    static const char* headers[] = {
"JetSubStructureMomentTools/AngularityTool.h",
"JetSubStructureMomentTools/JetSubStructureMomentToolsBase.h",
"JetSubStructureMomentTools/DipolarityTool.h",
"JetSubStructureMomentTools/JetChargeTool.h",
"JetSubStructureMomentTools/NSubjettinessRatiosTool.h",
"JetSubStructureMomentTools/ShowerDeconstructionTool.h",
"JetSubStructureMomentTools/BoostedXbbTagTool.h",
"JetSubStructureMomentTools/EnergyCorrelatorRatiosTool.h",
"JetSubStructureMomentTools/JetPullTool.h",
"JetSubStructureMomentTools/KTSplittingScaleTool.h",
"JetSubStructureMomentTools/NSubjettinessTool.h",
"JetSubStructureMomentTools/SubjetFinderTool.h",
"JetSubStructureMomentTools/ISubjetRecorderTool.h",
"JetSubStructureMomentTools/VolatilityTool.h",
"JetSubStructureMomentTools/BosonTagTool.h",
"JetSubStructureMomentTools/EnergyCorrelatorTool.h",
"JetSubStructureMomentTools/KtDeltaRTool.h",
"JetSubStructureMomentTools/PlanarFlowTool.h",
"JetSubStructureMomentTools/SubjetMakerTool.h",
"JetSubStructureMomentTools/CenterOfMassShapesTool.h",
"JetSubStructureMomentTools/KtMassDropTool.h",
"JetSubStructureMomentTools/QwTool.h",
"JetSubStructureMomentTools/SubjetRecorderTool.h",
0
    };
    static const char* includePaths[] = {
"/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root",
"/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/atlas/data18a/helingzh/workplace/summer2016/Run2/FourMuonAna/RootCoreBin/include",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/x86_64/root/6.04.16-x86_64-slc6-gcc49-opt/include",
"/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/cmt/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  AngularityTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  BoostedXbbTagTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  BosonTagTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  CenterOfMassShapesTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  DipolarityTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  EnergyCorrelatorRatiosTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  EnergyCorrelatorTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  ISubjetRecorderTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  JetChargeTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  JetPullTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  JetSubStructureMomentToolsBase;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  KTSplittingScaleTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  KtDeltaRTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  KtMassDropTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  NSubjettinessRatiosTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  NSubjettinessTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  PlanarFlowTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  QwTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  ShowerDeconstructionTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  SubjetFinderTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  SubjetMakerTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  SubjetRecorderTool;
class __attribute__((annotate("$clingAutoload$/cvmfs/atlas.cern.ch/repo/sw/ASG/AnalysisBase/2.5.0/JetSubStructureMomentTools/Root/LinkDef.h")))  VolatilityTool;
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
  #define ROOTCORE_PACKAGE "JetSubStructureMomentTools"
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "JetSubStructureMomentTools/AngularityTool.h"
#include "JetSubStructureMomentTools/JetSubStructureMomentToolsBase.h"
#include "JetSubStructureMomentTools/DipolarityTool.h"
#include "JetSubStructureMomentTools/JetChargeTool.h"
#include "JetSubStructureMomentTools/NSubjettinessRatiosTool.h"
#include "JetSubStructureMomentTools/ShowerDeconstructionTool.h"
#include "JetSubStructureMomentTools/BoostedXbbTagTool.h"
#include "JetSubStructureMomentTools/EnergyCorrelatorRatiosTool.h"
#include "JetSubStructureMomentTools/JetPullTool.h"
#include "JetSubStructureMomentTools/KTSplittingScaleTool.h"
#include "JetSubStructureMomentTools/NSubjettinessTool.h"
#include "JetSubStructureMomentTools/SubjetFinderTool.h"
#include "JetSubStructureMomentTools/ISubjetRecorderTool.h"
#include "JetSubStructureMomentTools/VolatilityTool.h"
#include "JetSubStructureMomentTools/BosonTagTool.h"
#include "JetSubStructureMomentTools/EnergyCorrelatorTool.h"
#include "JetSubStructureMomentTools/KtDeltaRTool.h"
#include "JetSubStructureMomentTools/PlanarFlowTool.h"
#include "JetSubStructureMomentTools/SubjetMakerTool.h"
#include "JetSubStructureMomentTools/CenterOfMassShapesTool.h"
#include "JetSubStructureMomentTools/KtMassDropTool.h"
#include "JetSubStructureMomentTools/QwTool.h"
#include "JetSubStructureMomentTools/SubjetRecorderTool.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"AngularityTool", payloadCode, "@",
"BoostedXbbTagTool", payloadCode, "@",
"BosonTagTool", payloadCode, "@",
"CenterOfMassShapesTool", payloadCode, "@",
"DipolarityTool", payloadCode, "@",
"EnergyCorrelatorRatiosTool", payloadCode, "@",
"EnergyCorrelatorTool", payloadCode, "@",
"ISubjetRecorderTool", payloadCode, "@",
"JetChargeTool", payloadCode, "@",
"JetPullTool", payloadCode, "@",
"JetSubStructureMomentToolsBase", payloadCode, "@",
"KTSplittingScaleTool", payloadCode, "@",
"KtDeltaRTool", payloadCode, "@",
"KtMassDropTool", payloadCode, "@",
"NSubjettinessRatiosTool", payloadCode, "@",
"NSubjettinessTool", payloadCode, "@",
"PlanarFlowTool", payloadCode, "@",
"QwTool", payloadCode, "@",
"ShowerDeconstructionTool", payloadCode, "@",
"SubjetFinderTool", payloadCode, "@",
"SubjetMakerTool", payloadCode, "@",
"SubjetRecorderTool", payloadCode, "@",
"VolatilityTool", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("JetSubStructureMomentToolsCINT",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_JetSubStructureMomentToolsCINT_Impl, {{"namespace DataVector_detail { template <typename B1, typename B2, typename B3> class VirtBases; }", 1},{"template <typename T> class DataVectorBase;", 1},{"template <typename T, typename BASE> class DataVector;", 1},{"namespace DataVector_detail { template <typename T> class DVLEltBaseInit; }", 1}}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_JetSubStructureMomentToolsCINT_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_JetSubStructureMomentToolsCINT() {
  TriggerDictionaryInitialization_JetSubStructureMomentToolsCINT_Impl();
}
