
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

#include "stdUtils.h"
#include "Signal0.h"
#include "Slot0.h"
#include <cassert>

using namespace ksignal0;

// Constructor
// -----------
Signal0::Signal0()
{
   emitInProgress = false;
}


// Destructor
// -----------
Signal0::~Signal0()
{
   std::for_each(slots.begin(), slots.end(), delete_pointer());
}


// connect a slot to this signal
// (containing an object function callback)
// ----------------------------------------
void Signal0::connect(BaseSlot0* slot_)
{
   slots.push_back(slot_);
}


// disconnect ALL slots that correspond to this slot
// -------------------------------------------------
void Signal0::disconnect(const BaseSlot0& slot_)
{
   std::vector<BaseSlot0*>::iterator it;
   for( it = slots.begin(); it != slots.end();)
   {
      BaseSlot0* tmp = *it;

      if (*tmp == slot_)
      {
         it = slots.erase(it); // goes to next item
         delete tmp;
      }
      else
      {
         it++;
      }
   }
}


// emit() will call all the stored slots' (callback)
// functions
// ----------------------------------
void Signal0::emit()
{

   // if(emitInProgress == true) abort
   //assert(!emitInProgress && "abort due to recursive calling of emit");

   emitInProgress = true;
   for(unsigned int i = 0; i < slots.size(); i++)
   {
      slots[i]->execute();
   }

   emitInProgress = false;
}

