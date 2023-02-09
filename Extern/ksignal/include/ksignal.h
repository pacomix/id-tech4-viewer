
// Code originally from KjellKod.cc by influence of
// www.boost.org and sigslot

// KjellKod header file ---------------------------------//
//
// (C) Copyright Kjell Hedström 2007
// Distributed under the KjellKod Software license,
// see (accompanying file) LICENSE_1_0.txt or copy at
// http://license.kjellkod.cc
//
// The software license grants permission without
// fee to copy, use and modify the software for any use
// (commercial and non-commercial)
// ------------------------------------------------------

#ifndef KSIGNAL_H_INC
#define KSIGNAL_H_INC

#include "Signal0.h"
#include "Signal1.h"
#include "Slot0.h"
#include "Slot1.h"

// ------KSignal Description----------------------------------------
//
// KSignal is an signal/slot implementation of the Observer pattern
//
// Signals represent typesafe function callback with multiple targets.
// Signals are connected to slots, which are callback receivers. All
// slots/receivers connected to a signal are called when the signal
// is "emitted"
//
// Signals are typically used to notify dependents/receivers of
// state changes within an object
//
// ----------------------------
// Example of usage of ksignal0
// ----------------------------
// #include <KSignal.h>
//
// class sender
// {
//    public:
//    ...
//       ksignal0::Signal0  sig0;
//    ...
//       void tellEveryone()
//       {
//         sig0.emit();
//       }
//    ...
//  };
//
//
// class receiver
// {
//    public:
//    ...
//       void someFunction()
//       {
//         std::cout << "receiver::someFunction() was called" << std::endl;
//       }
//    ...
//  };
//
//
// #include <KSignal.h>
// int main()
// {
//      sender sendObj;
//      receiver recObj;
//
//      ksignal0::connect(sendObj.sig0, recObj, &receiverClass::someFunction);
//
//      // calling sender::tellEveryone() will do "emit"
//      // on the signal connected to recObj::someFunction()
//      sendObj.tellEveryone();
// }
//
//
// ----------------------------
// Example of usage of ksignal1
// ----------------------------
// #include <KSignal.h>
//
// class sender1
// {
//    public:
//    ...
//       ksignal1::Signal1<bool>  sig1;
//    ...
//       void tellEveryone(bool aValue)
//       {
//         sig1.emit(aValue);
//       }
//    ...
//  };
//
// class receiver
// {
//    public:
//    ...
//       void someFunction(bool value)
//       {
//         std::cout << "receiver::someFunction() was called "
//                 << "with value: " << value " << std::endl;
//       }
//    ...
//  };
//
// #include <KSignal.h>
// int main()
// {
//      sender sendObj;
//      receiver recObj;
//
//      ksignal1::connect(sendObj.sig1, recObj, &receiverClass::someFunction);
//
//      // calling sender::tellEveryone(bool aValue) will do "emit"
//      // on the signal connected to recObj::someFunction(bool value),
//      // executing it with the passed argument
//
//      sendObj.tellEveryone(true);
// }
//
//
// ----------------------
// Limitations of KSignal (Could be implemented in the future)
// ----------------------
// * No usage of smart pointers
//                 - Warning : Important to not destroy objects
//                          that are still signal connected since slots will
//                          then contain callback functions for an object
//                                that is destroyed
//                  I.e. important to first disconnect - THEN delete object
//
//
// * Thread safety - Warning:     KSignal is not implemented for a multiple
//                           thread usage.
//
// * Callback return values:     Are ignored & will not be implemented since
//                          Signal/Slot (Observer pattern) is made to make
//                          the software loosely coupled. Return values would
//                          make it tighter coupled which is a bad thing.
//
//
//
//  ---------------MORE INFORMATION ------------------------
// More information of the Observer pattern can be found at:
//         http://en.wikipedia.org/wiki/Observer_pattern
//
//
// More information of Signal/Slot mechanism can be found at:
// http://en.wikipedia.org/wiki/Signals_andslots
//
//
// Other implementations of Signal/Slot mechanisms are found
// at (boost)     http://www.boost.org,
//    (libsigc++) http://libsigc.sourceforge.net,
//    (sigslot)  http://sigslot.sourceforge.net/
//
//    - and the original -
//    (Qt)        http://www.trolltech.com
//
//
// --------------------------------------------------------
namespace ksignal0
{
   // --------------------------------------------
   // Connect & disconnect to Signal0 definitions
   // --------------------------------------------



   // Connect for Signal0
   // -------------------
   template<class T, class ClassMethod>
   void connect(Signal0& signal, T& object_, ClassMethod method_);


   // Signal's disconnected completely (all connections)
   // from object's method
   // -----------------------------------------------------
   template<class T> void disconnect(Signal0& signal, T& object_);




   // -----------------------------------------------
   // Connect & disconnect to Signal0 implementation
   // -----------------------------------------------

   // Signal's connected to object's method
   // -------------------------------------
   template<class T, class ClassMethod>
   inline void connect(Signal0& signal, T& object_, ClassMethod method_)
   {
      Slot0<T,ClassMethod>* slot =
         new Slot0<T,ClassMethod>(&object_, method_);

      signal.connect(slot);
   }


   // Signal's disconnected completely (all connections)
   // from object's method
   // -----------------------------------------------------
   template<class T>
   inline void disconnect(Signal0& signal, T& object_)
   {
      // Create a slot-key to
      // disconnect everything between signal & object
      Slot0<T, void (T::*)() > slot(&object_, 0);
      signal.disconnect(slot);
   }
}






namespace ksignal1
{

   // --------------------------------------------
   // Connect & disconnect to Signal0 definitions
   // --------------------------------------------


   // Connect for Signal1
   // -------------------
   template<class Parameter, class T, class ClassMethod>
   void connect(Signal1<Parameter>& signal, T& object_, ClassMethod method_);


   // Signal's disconnected completely (all connections)
   // from object's method
   // -----------------------------------------------------
   template<class Parameter, class T>
   void disconnect(Signal1<Parameter>& signal, T& obj_);




   // -----------------------------------------------
   // Connect & disconnect to Signal1 implementation
   // -----------------------------------------------


   // Signal's connected to object's method
   // -------------------------------------
   template<class Parameter, class T, class ClassMethod>
   void connect(Signal1<Parameter>& signal, T& object_, ClassMethod method_)
   {
      Slot1<Parameter, T, ClassMethod>* slot =
         new Slot1<Parameter, T, ClassMethod>(&object_, method_);

      signal.connect(slot);
   }


   // Signal's disconnected completely (all connections)
   // from object's method
   // -----------------------------------------------------
   template<class Parameter, class T>
   void disconnect(Signal1<Parameter>& signal, T& object_)
   {
      // Create a key to disconnect the signal
      Slot1<Parameter, T, void (T::*)(Parameter) > slot(&object_, 0);

      signal.disconnect(slot);
   }

}

#endif

