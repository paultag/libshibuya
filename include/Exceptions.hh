#ifndef _EXCEPTIONS_HH_
#define _EXCEPTIONS_HH_ I_CAN_HAS

#include "ShibuyaObject.hh"

#include <exception>

class DeadChildException : public ShibuyaExceptionObject {
	public:
		virtual const char * what() const throw();
};

#endif
