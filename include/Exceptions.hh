#ifndef _EXCEPTIONS_HH_
#define _EXCEPTIONS_HH_ I_CAN_HAS

#include <exception>

class DeadChildException : public std::exception {
	public:
		virtual const char * what() const throw();
};

#endif
