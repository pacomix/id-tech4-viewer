
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

#ifndef Signal_0_H_INC
#define Signal_0_H_INC

#include <vector>

class BaseSlot0;


namespace ksignal0
{
   // Signals WITH NO arguments to callback function
   //
   // Connects/calls/disconnects slots
   // containing callback -object and method
   //
   // Since this is not a template class - implementation
   // of it is in the Signal0.cpp and not with inline
   // functions
   // ----------------------------------------
   class Signal0
   {
   public:
      // Constructors & destructors
      Signal0();
      virtual ~Signal0();

      // emit() will call all the stored slots' (callback)
      // functions
      virtual void emit();

      // connect a slot (containing an object function callback)
      virtual void connect(BaseSlot0* slot_);

      // disconnect ALL slots that correspond to this slot
      virtual void disconnect(const BaseSlot0& slot_);

      // Equal operator
      bool operator==(const Signal0& signal) {return this == &signal;}

   private:
      Signal0(const Signal0& ) {abort();}         // disable copy
      void operator=(const Signal0&) {abort();}   // disble assignment

      std::vector<BaseSlot0*> slots;         // storage of slots
      bool emitInProgress;               // flag to assert on recursive emit
   };

}

#endif

