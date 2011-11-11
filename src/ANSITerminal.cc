#include <ansiescape.hh>

#include "ANSITerminal.hh"

ANSITerminal::ANSITerminal() {
	this->_init_ANSITerminal();
	this->_init_Terminal( 80, 25 );
}
ANSITerminal::ANSITerminal( int width, int height ) {
	this->_init_ANSITerminal();
	this->_init_Terminal( width, height );
}
ANSITerminal::~ANSITerminal() {}

void ANSITerminal::_init_ANSITerminal() {
	ansi_escape_parser_reset();
}

void ANSITerminal::insert( unsigned char c ) {
	int res = ansi_escape_parser_feed( c );
	switch ( res ) {
		case ANSI_ESCAPE_PARSE_OK:
			ansi_sequence * last = ansi_escape_get_last_sequence();
			break;
		case ANSI_ESCAPE_PARSE_BAD:
			break;
		case ANSI_ESCAPE_PARSE_INCOMPLETE:
			break;
	}
}
