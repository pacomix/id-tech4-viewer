
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


#ifndef SLOT_1_H_INC
#define SLOT_1_H_INC


// Abstract template class for slot - storage of
// function(argument) callbacks
// ----------------------------------------
template <class Param> class BaseSlot1
{
public: 
   // destructor
   virtual ~BaseSlot1(){} 

   // Compares slots and returns true if they are equal
   // -------------------------------------------------
   virtual bool operator==(const BaseSlot1<Param>& aCopy) const
   {
      return (objectID() == aCopy.objectID());
   }

   // Call slot's(object's) stored callback with argument
   virtual void execute(Param value) = 0;

protected:
   // Return the callback i.e. &object::method
   virtual void* objectID() const = 0;

};



// Concrete & template implementation of slots
// i.e. storage of  function(argument) callbacks
// ----------------------------------------
template <class Param, class T, class ClassMethod> class Slot1
: public BaseSlot1<Param>
{
public:

   // Construct slot to object->method
   Slot1(T* object, ClassMethod method);

   // Copy Constructor
   Slot1(const Slot1<Param, T, ClassMethod> &slot_);

   // destructor
   virtual ~Slot1(){} 

   // Call slot's(object's) stored callback with argument
   virtual void execute(Param value);

protected:
   // Return the callback i.e. &object::method
   virtual void* objectID() const;

private:
   T*            object;
   ClassMethod      method;
};



// ------------------------------
// Slot1<T, ClassMethod> inlines
// ------------------------------

// Constructor:  slot stores to object->method
// -------------------------------------------
template <class Param, class T, class ClassMethod>
inline Slot1<Param, T, ClassMethod>::Slot1(T* object, ClassMethod method)
: object(object),
method(method)
{
}


// Copy Constructor
// ----------------
template <class Param, class T, class ClassMethod>
inline Slot1<Param, T, ClassMethod>
::Slot1(const Slot1<Param, T, ClassMethod>& slot_)
: object(slot_.object),
method(slot.method)
{
}


// Call slot's(object's) stored callback with argument
// --------------------------------------------------
template <class Param, class T, class ClassMethod>
inline void Slot1<Param, T, ClassMethod>::execute(Param value)
{
   (object->*method)(value);
}


// Return the callback i.e. &object::method
// ----------------------------------------
template <class Param, class T, class ClassMethod>
inline void* Slot1<Param, T, ClassMethod>::objectID() const
{
   return object;
}

#endif
