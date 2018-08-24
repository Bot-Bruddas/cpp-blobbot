#pragma once
#include <filesystem>
#include <fstream>
#include <array>
#include "LodePNG/LodePNG.h"

//rgba, each takes 1 byte
enum class Color : uint32_t {
	None = 0x00000000,
	White = 0xFFFFFFFF,
	Mercury = 0xE4E4E4FF,
	Gray = 0x888888FF,
	MineShaft = 0x222222FF,
	CarnationPink = 0xFFA7D1FF,
	Red = 0xE50000FF,
	Tangerine = 0xE59500FF,
	CapePalliser = 0xA06A42FF,
	Turbo = 0xE5D900FF,
	Conifer = 0x94E044FF,
	Green = 0x02BE01FF,
	RobinsEggBlue = 0x00D3DDFF,
	Lochmara = 0x0083C7FF,
	Blue = 0x0000EAFF,
	Lavender = 0xCF6EE4FF,
	FreshEggplant = 0x820080FF
};

//This is naive linear search
//Should be improved in further versions
//Perhaps convert colors to HSL?
Color NearestColor( unsigned char r, unsigned char g, unsigned char b, unsigned char a ) {
	unsigned int minDistance{ 0xFFFFFFFF };
	unsigned int distance{ 0 };
	Color value{ Color::None };

	std::vector<Color> colors{
		Color::White,
		Color::Mercury,
		Color::Gray,
		Color::MineShaft,
		Color::CarnationPink,
		Color::Red,
		Color::Tangerine,
		Color::CapePalliser,
		Color::Turbo,
		Color::Conifer,
		Color::Green,
		Color::RobinsEggBlue,
		Color::Lochmara,
		Color::Blue,
		Color::Lavender,
		Color::FreshEggplant
	};
	if ( a > 128 ) {
		for ( size_t i{ 0 }; i < 16; ++i ) {
			distance = (
				( ( r - ( static_cast<unsigned char>( static_cast<unsigned int>( colors[ i ] ) >> 6*4 ) ) ) *
				( r - ( static_cast<unsigned char>( static_cast<unsigned int>( colors[ i ] ) >> 6 * 4 ) ) ) ) +
				( ( g - ( static_cast<unsigned char>( static_cast<unsigned int>( colors[ i ] ) >> 4 * 4 ) ) ) *
				( g - ( static_cast<unsigned char>( static_cast<unsigned int>( colors[ i ] ) >> 4 * 4 ) ) ) ) +
				( ( b - ( static_cast<unsigned char>( static_cast<unsigned int>( colors[ i ] ) >> 2 * 4 ) ) ) *
				( b - ( static_cast<unsigned char>( static_cast<unsigned int>( colors[ i ] ) >> 2 * 4 ) ) ) ) );

			if ( distance < minDistance ) {
				minDistance = distance;
				value = colors[ i ];
			}

		}
	}

	return value;
}

class Image {
public:
	Image( std::string filePath ) {
		//Load image from path
		//Path may be relative or absolute
		std::filesystem::path path( filePath );
		std::vector<unsigned char> data;
		unsigned int width{ 0 };
		unsigned int height{ 0 };
		unsigned error{ 0 };

		if ( path.extension( ) == ".png" ) {
			error = lodepng::decode( data, width, height, path.string( ) );
		}

		if ( error ) {
			std::cout << "decoder error " << error << ": " << lodepng_error_text( error ) << std::endl;
		}

		////Unconventional for loop for debugging image data
		//for ( size_t i{ 1 }; i <= data.size( ); ++i ) {
		//	std::cout << '[' << static_cast<unsigned>( data[i-1] ) << ']';
		//	if ( i % 4 == 0 && i != 0 ) {
		//		std::cout << '\n';
		//	}
		//}

		//Compress image to use only 16 selected colors
		std::vector<std::array<unsigned char, 4>> colorData;
		size_t i{ 0 };
		while ( i < data.size() ) {
			colorData.push_back( {data[ i++ ], data[ i++ ], data[ i++ ], data[ i++ ]} );
		}

		////Testing that nearest color works.
		//for ( std::array<unsigned char, 4> pixel : colorData ) {
		//	Color temp = NearestColor( pixel[ 0 ], pixel[ 1 ], pixel[ 2 ], pixel[ 3 ] );
		//	switch ( temp ) {
		//		case Color::None:
		//			std::cout << "None\n";
		//			break;
		//		case Color::White:
		//			std::cout << "White\n";
		//			break;
		//		case Color::Mercury:
		//			std::cout << "Mercury\n";
		//			break;
		//		case Color::Gray:
		//			std::cout << "Gray\n";
		//			break;
		//		case Color::MineShaft:
		//			std::cout << "Mine Shaft\n";
		//			break;
		//		case Color::CarnationPink:
		//			std::cout << "Carnation Pink\n";
		//			break;
		//		case Color::Red:
		//			std::cout << "Red\n";
		//			break;
		//		case Color::Tangerine:
		//			std::cout << "Tangerine\n";
		//			break;
		//		case Color::CapePalliser:
		//			std::cout << "Cape Palliser\n";
		//			break;
		//		case Color::Turbo:
		//			std::cout << "Turbo\n";
		//			break;
		//		case Color::Conifer:
		//			std::cout << "Conifer\n";
		//			break;
		//		case Color::Green:
		//			std::cout << "Green\n";
		//			break;
		//		case Color::RobinsEggBlue:
		//			std::cout << "Robin's Egg Blue\n";
		//			break;
		//		case Color::Lochmara:
		//			std::cout << "Lochmara\n";
		//			break;
		//		case Color::Blue:
		//			std::cout << "Blue\n";
		//			break;
		//		case Color::Lavender:
		//			std::cout << "Lavender\n";
		//			break;
		//		case Color::FreshEggplant:
		//			std::cout << "Fresh Eggplant\n";
		//			break;
		//		default:
		//			break;
		//	}
		//}
	}
	~Image( ) = default;
private:
};