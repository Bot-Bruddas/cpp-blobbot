#pragma once

enum class Strategy : uint8_t {
	None = 0x00000000,
	Random = 0x00000001
}; // How would priority work?

class Options {
public:
	Options( const int argc, char** argv ) : _strategy(Strategy::Random) {
		for ( int i{ 0 }; i < argc; ++i ) {
			if ( _strcmpi( argv[ i ], "--strategy" ) == 0 || _strcmpi( argv[ i ], "-s" ) == 0 ) {
				if ( _strcmpi( argv[ ++i ], "random" ) == 0) {
					_strategy = Strategy::Random;
				}
			} else if ( _strcmpi( argv[ i ], "--image" ) == 0 || _strcmpi( argv[ i ], "-i" ) == 0 ) {
				_image = argv[ ++i ];
			}
		}
	}

	~Options( ) = default;

	Strategy GetStrategy( ) const { return _strategy; }
	std::string GetImage( ) const { return _image; }
private:
	Strategy _strategy;
	std::string _image;
};