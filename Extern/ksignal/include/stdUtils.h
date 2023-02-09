
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


#include <algorithm>

// <Functor for removing with () operator>
// ---------------------------------------
class delete_pointer
{
public:
   template <class T>
   void operator()(const T* target) const
   {
      delete target;
   }
};



// Example:
// #include <algorithm>
// and use std::for_each for this functor 'delete_pointer'
// to delete all objects* in a vector
// i.e. on std::vector b
// std::for_each(b.begin(), b.end() , delete_pointer());
//
// --- Note that the COMPILER will sort out witch type you want to delete.
//

// An alternative approach
// -----------------------
// You could create a functor which does what you need (see template below)
// And then somewhere else on std::vector b
// you call
//         for_each(b.begin(), b.end(), doDelete<base>());
// <Functor example:>
//   template <class T>
//   class doDelete
//   {
//     public:
//       void operator()(T *target)
//      {
//        delete target;
//      }
//   };



