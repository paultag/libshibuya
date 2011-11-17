#include "Exceptions.hh"

const char * DeadChildException::what() const throw() {
	return "Child process has died.";
}
