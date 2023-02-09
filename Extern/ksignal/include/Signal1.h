
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

#ifndef SIGNAL_1_H_INC
#define SIGNAL_1_H_INC

#include "stdUtils.h"

#include <vector>
#include <cassert>

template <class T> class BaseSlot1;

// Signals WITH arguments to callback function
namespace ksignal1
{
   // Connects/calls/disconnects slots
   // containing callback -object and method
   template <class Parameter>
   class Signal1
   {
   public:

      Signal1(): emitInProgress(false){}  // Constructors & destructors
      virtual ~Signal1(){std::for_each(slots.begin(), slots.end(), delete_pointer());}

      // emit() will call all the stored slots' (callback)
      // functions (with argument)
      virtual void emit(Parameter param);

      // connect a slot (containing an object function callback)
      virtual void connect(BaseSlot1<Parameter>* slot_);

      // disconnect ALL slots that correspond to this slot
      virtual void disconnect(const BaseSlot1<Parameter>& slot_);

      // Equal operator
      bool operator==(const Signal1& signal) {return this == &signal;}


   private:
      Signal1(const Signal1& )      {abort();}      // disable copy
      void operator=(const Signal1&)   {abort();}      // disable assignment

      std::vector< BaseSlot1<Parameter>* > slots;   // storage of slots
      bool emitInProgress;                     // flag to assert on recursive emit
   };


#ifndef USE_EXPORT_KEYWORD
   #include "Signal1.cpp"
#endif

#endif   /* SIGNAL_1_H_INC */
