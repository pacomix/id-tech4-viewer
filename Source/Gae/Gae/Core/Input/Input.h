#ifndef Input_h__
#define Input_h__

#include "Windows\Input.h"

// Helpers
#define GAE_INPUT_REGISTER_EVENT(Event, classPointer, Method)							\
		ksignal1::connect(Gae::Core::Input::Manager::Singleton().##Event, *##classPointer, Method)

#define GAE_INPUT_UNREGISTER_EVENT(Event, classPointer)									\
		ksignal1::disconnect(Gae::Core::Input::Manager::Singleton().##Event, *##classPointer)

#endif // Input_h__