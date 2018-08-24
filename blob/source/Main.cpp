#include <iostream>
#include <string>

#include "Parser.hpp"
#include "Image.hpp"

int main( int argc, char** argv ) {
	//Parse arguments
	Options opt{ argc, argv };

	//Load and compress image
	Image img{ opt.GetImage( ) };

	//Set up proxy if necessary

	//Start bot
}