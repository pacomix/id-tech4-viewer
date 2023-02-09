// KjellKod header file ---------------------------------//
//
// (C) Copyright Kjell Hedström 2008
// Distributed under the KjellKod Software license,
// see (accompanying file) LICENSE_1_0.txt or copy at
// http://license.kjellkod.cc
//
// The software license grants permission without
// fee to copy, use and modify the software for any use
// (commercial and non-commercial)
// ------------------------------------------------------


#ifndef SIGNAL_1_CPP_IMPL
#define SIGNAL_1_CPP_IMPL


#ifndef USE_EXPORT_KEYWORD
   #define export /* nothing */
#endif

#include "Signal1.h"

   // Connect signal with slot
   export template<class Parameter>
   void ksignal1::Signal1<Parameter>::connect(BaseSlot1<Parameter>* slot_)
   {
      slots.push_back(slot_);
   }


   // Disconnect all corresponding slots
   export template<class Parameter>
   void ksignal1::Signal1<Parameter>::disconnect(const BaseSlot1<Parameter>& slot_)
   {
      std::vector< BaseSlot1<Parameter> *>::iterator it;

      for( it = slots.begin(); it != slots.end();)
      {
         BaseSlot1<Parameter>* tmp = *it;
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
   // functions (with argument)
   export template<class Parameter>
   void ksignal1::Signal1<Parameter>::emit(Parameter param)
   {
      //assert(!emitInProgress && "abort due to recursive calling of emit");

      emitInProgress = true;

      for(unsigned int i = 0; i < slots.size(); i++)
      {
         slots[i]->execute(param);
      }

      emitInProgress = false;
   }
}

#endif /* SIGNAL_1_CPP_IMPL */
