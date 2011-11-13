#include "BGFile.hh"

#include <fstream>


std::vector<std::string> * get_bg_vector ( const char * fd ) {
	std::string line;
	std::ifstream myfile(fd);
	
	std::vector<std::string> * bg = new std::vector<std::string>();
	/* dealloc... */
	
	unsigned int fmax = 0;
	int flen = 0;
	
	if (myfile.is_open()) {
		while ( myfile.good() ) {
			getline(myfile,line);
			fmax = fmax < line.length() ? line.length() : fmax;
			++flen;
			bg->insert( bg->end(), line );
		}
		myfile.close();
	}
	
	return bg;
}
