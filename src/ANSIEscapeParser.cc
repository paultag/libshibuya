#include <malloc.h>

#include "ANSIEscapeParser.hh"

#define ANSI_ESCAPE_MAXLEN 128

ANSIEscapeParser::ANSIEscapeParser() {
	this->parsed_queue = (char *)
		malloc(sizeof(char) * ANSI_ESCAPE_MAXLEN);
	this->clear();
}

ANSIEscapeParser::~ANSIEscapeParser() {

}

PARSE_RESULT_T ANSIEscapeParser::process( char c ) {

	return 1;
}

void ANSIEscapeParser::clear() {
	this->parsed_queue[0] = '\0';
}
