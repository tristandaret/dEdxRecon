// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME PIDdIdict_PID
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
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

// Header files passed as explicit arguments
#include "PID/dEdx.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace PID {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *PID_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("PID", 0 /*version*/, "PID/Variables.h", 7,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &PID_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *PID_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_PIDcLcLTPad(void *p = nullptr);
   static void *newArray_PIDcLcLTPad(Long_t size, void *p);
   static void delete_PIDcLcLTPad(void *p);
   static void deleteArray_PIDcLcLTPad(void *p);
   static void destruct_PIDcLcLTPad(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PID::TPad*)
   {
      ::PID::TPad *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PID::TPad >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PID::TPad", ::PID::TPad::Class_Version(), "PID/dEdx.h", 19,
                  typeid(::PID::TPad), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PID::TPad::Dictionary, isa_proxy, 4,
                  sizeof(::PID::TPad) );
      instance.SetNew(&new_PIDcLcLTPad);
      instance.SetNewArray(&newArray_PIDcLcLTPad);
      instance.SetDelete(&delete_PIDcLcLTPad);
      instance.SetDeleteArray(&deleteArray_PIDcLcLTPad);
      instance.SetDestructor(&destruct_PIDcLcLTPad);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PID::TPad*)
   {
      return GenerateInitInstanceLocal(static_cast<::PID::TPad*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::PID::TPad*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PIDcLcLTCluster(void *p = nullptr);
   static void *newArray_PIDcLcLTCluster(Long_t size, void *p);
   static void delete_PIDcLcLTCluster(void *p);
   static void deleteArray_PIDcLcLTCluster(void *p);
   static void destruct_PIDcLcLTCluster(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PID::TCluster*)
   {
      ::PID::TCluster *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PID::TCluster >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PID::TCluster", ::PID::TCluster::Class_Version(), "PID/dEdx.h", 44,
                  typeid(::PID::TCluster), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PID::TCluster::Dictionary, isa_proxy, 4,
                  sizeof(::PID::TCluster) );
      instance.SetNew(&new_PIDcLcLTCluster);
      instance.SetNewArray(&newArray_PIDcLcLTCluster);
      instance.SetDelete(&delete_PIDcLcLTCluster);
      instance.SetDeleteArray(&deleteArray_PIDcLcLTCluster);
      instance.SetDestructor(&destruct_PIDcLcLTCluster);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PID::TCluster*)
   {
      return GenerateInitInstanceLocal(static_cast<::PID::TCluster*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::PID::TCluster*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PIDcLcLTModule(void *p = nullptr);
   static void *newArray_PIDcLcLTModule(Long_t size, void *p);
   static void delete_PIDcLcLTModule(void *p);
   static void deleteArray_PIDcLcLTModule(void *p);
   static void destruct_PIDcLcLTModule(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PID::TModule*)
   {
      ::PID::TModule *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PID::TModule >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PID::TModule", ::PID::TModule::Class_Version(), "PID/dEdx.h", 57,
                  typeid(::PID::TModule), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PID::TModule::Dictionary, isa_proxy, 4,
                  sizeof(::PID::TModule) );
      instance.SetNew(&new_PIDcLcLTModule);
      instance.SetNewArray(&newArray_PIDcLcLTModule);
      instance.SetDelete(&delete_PIDcLcLTModule);
      instance.SetDeleteArray(&deleteArray_PIDcLcLTModule);
      instance.SetDestructor(&destruct_PIDcLcLTModule);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PID::TModule*)
   {
      return GenerateInitInstanceLocal(static_cast<::PID::TModule*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::PID::TModule*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PIDcLcLTEvent(void *p = nullptr);
   static void *newArray_PIDcLcLTEvent(Long_t size, void *p);
   static void delete_PIDcLcLTEvent(void *p);
   static void deleteArray_PIDcLcLTEvent(void *p);
   static void destruct_PIDcLcLTEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PID::TEvent*)
   {
      ::PID::TEvent *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PID::TEvent >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("PID::TEvent", ::PID::TEvent::Class_Version(), "PID/dEdx.h", 71,
                  typeid(::PID::TEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PID::TEvent::Dictionary, isa_proxy, 4,
                  sizeof(::PID::TEvent) );
      instance.SetNew(&new_PIDcLcLTEvent);
      instance.SetNewArray(&newArray_PIDcLcLTEvent);
      instance.SetDelete(&delete_PIDcLcLTEvent);
      instance.SetDeleteArray(&deleteArray_PIDcLcLTEvent);
      instance.SetDestructor(&destruct_PIDcLcLTEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PID::TEvent*)
   {
      return GenerateInitInstanceLocal(static_cast<::PID::TEvent*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::PID::TEvent*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace PID {
//______________________________________________________________________________
atomic_TClass_ptr TPad::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TPad::Class_Name()
{
   return "PID::TPad";
}

//______________________________________________________________________________
const char *TPad::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PID::TPad*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TPad::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PID::TPad*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TPad::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PID::TPad*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TPad::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PID::TPad*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace PID
namespace PID {
//______________________________________________________________________________
atomic_TClass_ptr TCluster::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TCluster::Class_Name()
{
   return "PID::TCluster";
}

//______________________________________________________________________________
const char *TCluster::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PID::TCluster*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TCluster::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PID::TCluster*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TCluster::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PID::TCluster*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TCluster::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PID::TCluster*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace PID
namespace PID {
//______________________________________________________________________________
atomic_TClass_ptr TModule::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TModule::Class_Name()
{
   return "PID::TModule";
}

//______________________________________________________________________________
const char *TModule::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PID::TModule*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TModule::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PID::TModule*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TModule::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PID::TModule*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TModule::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PID::TModule*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace PID
namespace PID {
//______________________________________________________________________________
atomic_TClass_ptr TEvent::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TEvent::Class_Name()
{
   return "PID::TEvent";
}

//______________________________________________________________________________
const char *TEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PID::TEvent*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PID::TEvent*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PID::TEvent*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PID::TEvent*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace PID
namespace PID {
//______________________________________________________________________________
void TPad::Streamer(TBuffer &R__b)
{
   // Stream an object of class PID::TPad.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PID::TPad::Class(),this);
   } else {
      R__b.WriteClassBuffer(PID::TPad::Class(),this);
   }
}

} // namespace PID
namespace ROOT {
   // Wrappers around operator new
   static void *new_PIDcLcLTPad(void *p) {
      return  p ? new(p) ::PID::TPad : new ::PID::TPad;
   }
   static void *newArray_PIDcLcLTPad(Long_t nElements, void *p) {
      return p ? new(p) ::PID::TPad[nElements] : new ::PID::TPad[nElements];
   }
   // Wrapper around operator delete
   static void delete_PIDcLcLTPad(void *p) {
      delete (static_cast<::PID::TPad*>(p));
   }
   static void deleteArray_PIDcLcLTPad(void *p) {
      delete [] (static_cast<::PID::TPad*>(p));
   }
   static void destruct_PIDcLcLTPad(void *p) {
      typedef ::PID::TPad current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::PID::TPad

namespace PID {
//______________________________________________________________________________
void TCluster::Streamer(TBuffer &R__b)
{
   // Stream an object of class PID::TCluster.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PID::TCluster::Class(),this);
   } else {
      R__b.WriteClassBuffer(PID::TCluster::Class(),this);
   }
}

} // namespace PID
namespace ROOT {
   // Wrappers around operator new
   static void *new_PIDcLcLTCluster(void *p) {
      return  p ? new(p) ::PID::TCluster : new ::PID::TCluster;
   }
   static void *newArray_PIDcLcLTCluster(Long_t nElements, void *p) {
      return p ? new(p) ::PID::TCluster[nElements] : new ::PID::TCluster[nElements];
   }
   // Wrapper around operator delete
   static void delete_PIDcLcLTCluster(void *p) {
      delete (static_cast<::PID::TCluster*>(p));
   }
   static void deleteArray_PIDcLcLTCluster(void *p) {
      delete [] (static_cast<::PID::TCluster*>(p));
   }
   static void destruct_PIDcLcLTCluster(void *p) {
      typedef ::PID::TCluster current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::PID::TCluster

namespace PID {
//______________________________________________________________________________
void TModule::Streamer(TBuffer &R__b)
{
   // Stream an object of class PID::TModule.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PID::TModule::Class(),this);
   } else {
      R__b.WriteClassBuffer(PID::TModule::Class(),this);
   }
}

} // namespace PID
namespace ROOT {
   // Wrappers around operator new
   static void *new_PIDcLcLTModule(void *p) {
      return  p ? new(p) ::PID::TModule : new ::PID::TModule;
   }
   static void *newArray_PIDcLcLTModule(Long_t nElements, void *p) {
      return p ? new(p) ::PID::TModule[nElements] : new ::PID::TModule[nElements];
   }
   // Wrapper around operator delete
   static void delete_PIDcLcLTModule(void *p) {
      delete (static_cast<::PID::TModule*>(p));
   }
   static void deleteArray_PIDcLcLTModule(void *p) {
      delete [] (static_cast<::PID::TModule*>(p));
   }
   static void destruct_PIDcLcLTModule(void *p) {
      typedef ::PID::TModule current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::PID::TModule

namespace PID {
//______________________________________________________________________________
void TEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class PID::TEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PID::TEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(PID::TEvent::Class(),this);
   }
}

} // namespace PID
namespace ROOT {
   // Wrappers around operator new
   static void *new_PIDcLcLTEvent(void *p) {
      return  p ? new(p) ::PID::TEvent : new ::PID::TEvent;
   }
   static void *newArray_PIDcLcLTEvent(Long_t nElements, void *p) {
      return p ? new(p) ::PID::TEvent[nElements] : new ::PID::TEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_PIDcLcLTEvent(void *p) {
      delete (static_cast<::PID::TEvent*>(p));
   }
   static void deleteArray_PIDcLcLTEvent(void *p) {
      delete [] (static_cast<::PID::TEvent*>(p));
   }
   static void destruct_PIDcLcLTEvent(void *p) {
      typedef ::PID::TEvent current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::PID::TEvent

namespace ROOT {
   static TClass *vectorlEPIDcLcLTPadmUgR_Dictionary();
   static void vectorlEPIDcLcLTPadmUgR_TClassManip(TClass*);
   static void *new_vectorlEPIDcLcLTPadmUgR(void *p = nullptr);
   static void *newArray_vectorlEPIDcLcLTPadmUgR(Long_t size, void *p);
   static void delete_vectorlEPIDcLcLTPadmUgR(void *p);
   static void deleteArray_vectorlEPIDcLcLTPadmUgR(void *p);
   static void destruct_vectorlEPIDcLcLTPadmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<PID::TPad*>*)
   {
      vector<PID::TPad*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<PID::TPad*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<PID::TPad*>", -2, "vector", 389,
                  typeid(vector<PID::TPad*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEPIDcLcLTPadmUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<PID::TPad*>) );
      instance.SetNew(&new_vectorlEPIDcLcLTPadmUgR);
      instance.SetNewArray(&newArray_vectorlEPIDcLcLTPadmUgR);
      instance.SetDelete(&delete_vectorlEPIDcLcLTPadmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEPIDcLcLTPadmUgR);
      instance.SetDestructor(&destruct_vectorlEPIDcLcLTPadmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<PID::TPad*> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<PID::TPad*>","std::vector<PID::TPad*, std::allocator<PID::TPad*> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<PID::TPad*>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEPIDcLcLTPadmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<PID::TPad*>*>(nullptr))->GetClass();
      vectorlEPIDcLcLTPadmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEPIDcLcLTPadmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEPIDcLcLTPadmUgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<PID::TPad*> : new vector<PID::TPad*>;
   }
   static void *newArray_vectorlEPIDcLcLTPadmUgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<PID::TPad*>[nElements] : new vector<PID::TPad*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEPIDcLcLTPadmUgR(void *p) {
      delete (static_cast<vector<PID::TPad*>*>(p));
   }
   static void deleteArray_vectorlEPIDcLcLTPadmUgR(void *p) {
      delete [] (static_cast<vector<PID::TPad*>*>(p));
   }
   static void destruct_vectorlEPIDcLcLTPadmUgR(void *p) {
      typedef vector<PID::TPad*> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<PID::TPad*>

namespace ROOT {
   static TClass *vectorlEPIDcLcLTModulemUgR_Dictionary();
   static void vectorlEPIDcLcLTModulemUgR_TClassManip(TClass*);
   static void *new_vectorlEPIDcLcLTModulemUgR(void *p = nullptr);
   static void *newArray_vectorlEPIDcLcLTModulemUgR(Long_t size, void *p);
   static void delete_vectorlEPIDcLcLTModulemUgR(void *p);
   static void deleteArray_vectorlEPIDcLcLTModulemUgR(void *p);
   static void destruct_vectorlEPIDcLcLTModulemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<PID::TModule*>*)
   {
      vector<PID::TModule*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<PID::TModule*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<PID::TModule*>", -2, "vector", 389,
                  typeid(vector<PID::TModule*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEPIDcLcLTModulemUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<PID::TModule*>) );
      instance.SetNew(&new_vectorlEPIDcLcLTModulemUgR);
      instance.SetNewArray(&newArray_vectorlEPIDcLcLTModulemUgR);
      instance.SetDelete(&delete_vectorlEPIDcLcLTModulemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEPIDcLcLTModulemUgR);
      instance.SetDestructor(&destruct_vectorlEPIDcLcLTModulemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<PID::TModule*> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<PID::TModule*>","std::vector<PID::TModule*, std::allocator<PID::TModule*> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<PID::TModule*>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEPIDcLcLTModulemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<PID::TModule*>*>(nullptr))->GetClass();
      vectorlEPIDcLcLTModulemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEPIDcLcLTModulemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEPIDcLcLTModulemUgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<PID::TModule*> : new vector<PID::TModule*>;
   }
   static void *newArray_vectorlEPIDcLcLTModulemUgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<PID::TModule*>[nElements] : new vector<PID::TModule*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEPIDcLcLTModulemUgR(void *p) {
      delete (static_cast<vector<PID::TModule*>*>(p));
   }
   static void deleteArray_vectorlEPIDcLcLTModulemUgR(void *p) {
      delete [] (static_cast<vector<PID::TModule*>*>(p));
   }
   static void destruct_vectorlEPIDcLcLTModulemUgR(void *p) {
      typedef vector<PID::TModule*> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<PID::TModule*>

namespace ROOT {
   static TClass *vectorlEPIDcLcLTClustermUgR_Dictionary();
   static void vectorlEPIDcLcLTClustermUgR_TClassManip(TClass*);
   static void *new_vectorlEPIDcLcLTClustermUgR(void *p = nullptr);
   static void *newArray_vectorlEPIDcLcLTClustermUgR(Long_t size, void *p);
   static void delete_vectorlEPIDcLcLTClustermUgR(void *p);
   static void deleteArray_vectorlEPIDcLcLTClustermUgR(void *p);
   static void destruct_vectorlEPIDcLcLTClustermUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<PID::TCluster*>*)
   {
      vector<PID::TCluster*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<PID::TCluster*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<PID::TCluster*>", -2, "vector", 389,
                  typeid(vector<PID::TCluster*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEPIDcLcLTClustermUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<PID::TCluster*>) );
      instance.SetNew(&new_vectorlEPIDcLcLTClustermUgR);
      instance.SetNewArray(&newArray_vectorlEPIDcLcLTClustermUgR);
      instance.SetDelete(&delete_vectorlEPIDcLcLTClustermUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEPIDcLcLTClustermUgR);
      instance.SetDestructor(&destruct_vectorlEPIDcLcLTClustermUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<PID::TCluster*> >()));

      instance.AdoptAlternate(::ROOT::AddClassAlternate("vector<PID::TCluster*>","std::vector<PID::TCluster*, std::allocator<PID::TCluster*> >"));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<PID::TCluster*>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEPIDcLcLTClustermUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<PID::TCluster*>*>(nullptr))->GetClass();
      vectorlEPIDcLcLTClustermUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEPIDcLcLTClustermUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEPIDcLcLTClustermUgR(void *p) {
      return  p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<PID::TCluster*> : new vector<PID::TCluster*>;
   }
   static void *newArray_vectorlEPIDcLcLTClustermUgR(Long_t nElements, void *p) {
      return p ? ::new(static_cast<::ROOT::Internal::TOperatorNewHelper*>(p)) vector<PID::TCluster*>[nElements] : new vector<PID::TCluster*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEPIDcLcLTClustermUgR(void *p) {
      delete (static_cast<vector<PID::TCluster*>*>(p));
   }
   static void deleteArray_vectorlEPIDcLcLTClustermUgR(void *p) {
      delete [] (static_cast<vector<PID::TCluster*>*>(p));
   }
   static void destruct_vectorlEPIDcLcLTClustermUgR(void *p) {
      typedef vector<PID::TCluster*> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<PID::TCluster*>

namespace {
  void TriggerDictionaryInitialization_libSoft_dict_PID_Impl() {
    static const char* headers[] = {
nullptr
    };
    static const char* includePaths[] = {
"Analysis/",
"EvtModel/",
"EvtModel/src/",
"EvtModelTools/",
"EvtModelTools/src/",
"Fitters/",
"Fitters/src/",
"Misc/",
"Misc/src/",
"MTools/",
"PID/",
"PID/src/",
"SampleTools/",
"SampleTools/src/",
"SignalModel/",
"SignalModel/src/",
"/local/home/td263283/root/include",
"/local/home/td263283/root/include/",
"/local/home/td263283/Documents/Code/AOB/AOBt_3.11/Code/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libSoft_dict_PID dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace PID{class __attribute__((annotate("$clingAutoload$PID/dEdx.h")))  TPad;}
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
namespace PID{class __attribute__((annotate("$clingAutoload$PID/dEdx.h")))  TCluster;}
namespace PID{class __attribute__((annotate("$clingAutoload$PID/dEdx.h")))  TModule;}
namespace PID{class __attribute__((annotate("$clingAutoload$PID/dEdx.h")))  TEvent;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libSoft_dict_PID dictionary payload"

#ifndef HAS_ROOT
  #define HAS_ROOT 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include <vector>
#include "PID/dEdx.h"
#ifdef __ROOTCLING__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace PID;
#pragma link C++ class PID::TPad+;
#pragma link C++ class std::vector<PID::TPad*>+;
#pragma link C++ class PID::TCluster+;
#pragma link C++ class std::vector<PID::TCluster*>+;
#pragma link C++ class PID::TModule+;
#pragma link C++ class std::vector<PID::TModule*>+;
#pragma link C++ class PID::TEvent+;

#endif
#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"PID::TCluster", payloadCode, "@",
"PID::TEvent", payloadCode, "@",
"PID::TModule", payloadCode, "@",
"PID::TPad", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libSoft_dict_PID",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libSoft_dict_PID_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libSoft_dict_PID_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libSoft_dict_PID() {
  TriggerDictionaryInitialization_libSoft_dict_PID_Impl();
}
