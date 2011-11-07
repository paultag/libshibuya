#ifndef _ANSIESCAPEPARSER_HH_
#define _ANSIESCAPEPARSER_HH_ CAN_HAS

#include "Shibuya.hh"

#define PARSE_RESULT_T          int
#define PARSE_RESULT_SUCCCESS   1
#define PARSE_RESULT_FAILURE    2
#define PARSE_RESULT_INCOMPLETE 4
#define PARSE_RESULT_UNKNOWN    8

class ANSIEscapeParser {
	protected:
		char * parsed_queue;

	public:
		ANSIEscapeParser();
		~ANSIEscapeParser();

		PARSE_RESULT_T process( char c );

		void clear();
};

#endif
