
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

#ifndef SLOT_0_H_INC
#define SLOT_0_H_INC



// Abstract class for slot - storage of
// function(void) callbacks
// ----------------------------------------
class BaseSlot0
{
public:

   // Destructor
   virtual ~BaseSlot0(){}

   // Compares slots and returns true if they are equal
   // -------------------------------------------------
   virtual bool operator==(const BaseSlot0& aCopy) const
   {
      return (objectID() == aCopy.objectID());
   }

   // Call slot's(object's) stored callback
   virtual void execute() = 0;

protected:

   // Return the callback i.e. &object::method
   virtual void* objectID() const = 0;

};



// Concrete & template implementation of slots
// i.e. storage of  function(void) callbacks
// ----------------------------------------
template <class T, class ClassMethod>
class Slot0 : public BaseSlot0
{
public:
   // Construct slot to object->method
   Slot0(T* object_, ClassMethod method_);

   // Copy Constructor
   Slot0(const Slot0 &slot_);

   // Destructor
   virtual ~Slot0(){} 


   // Call slot's(object's) method function
   virtual void execute();

protected:
   // Return the callback i.e. &object::method
   virtual void* objectID() const;

private:
   T*         object;      // object
   ClassMethod method;      // callback, i.e. &object::function
};




// ------------------------------
// Slot0<T, ClassMethod> inlines
// ------------------------------


// Constructor
// ----------
template <class T, class ClassMethod>
inline Slot0<T, ClassMethod>::Slot0(T* object_, ClassMethod method_)
: object(object_),
method(method_)
{
}


// Copy Constructor
// ----------------
template <class T, class ClassMethod>
inline Slot0<T, ClassMethod>::Slot0(const Slot0<T, ClassMethod>& slot_)
: object(slot_.object),
method(slot_.method)
{
}


// Call slot's(object's) method function
// -------------------------------------
template <class T, class ClassMethod>
inline void Slot0<T, ClassMethod>::execute()
{
   (object->*method)();
}


// Return the callback i.e. &object::method
// -------------------------------------
template <class T, class ClassMethod>
inline void* Slot0<T, ClassMethod>::objectID() const
{
   return object;
}


#endif

