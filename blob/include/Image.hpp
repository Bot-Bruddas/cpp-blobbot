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
				( ( r - ( static_cast<unsigned char>( static_cast<unsigned int>( colors[ i ] ) >> 6 * 4 ) ) ) *
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
		std::vector<unsigned char> tempData;
		unsigned error{ 0 };

		if ( path.extension( ) == ".png" ) {
			error = lodepng::decode( data, _width, _height, path.string( ) );
		}

		if ( error ) {
			std::cout << "decoder error " << error << ": " << lodepng_error_text( error ) << std::endl;
		}

		//Compress image to use only 16 selected colors
		size_t i{ 0 };
		while ( i < data.size() ) {
			_data.push_back( NearestColor(data[ i + 0 ], data[ i + 1 ], data[ i + 2 ], data[ i + 3 ]) );
			i += 4;
		}

		for ( Color pixel : _data ) {
			tempData.push_back( ( static_cast<unsigned char>( static_cast<unsigned int>( pixel ) >> 6 * 4 ) ) );
			tempData.push_back( ( static_cast<unsigned char>( static_cast<unsigned int>( pixel ) >> 4 * 4 ) ) );
			tempData.push_back( ( static_cast<unsigned char>( static_cast<unsigned int>( pixel ) >> 2 * 4 ) ) );
			tempData.push_back( ( static_cast<unsigned char>( static_cast<unsigned int>( pixel ) >> 0 * 4 ) ) );
		}

		error = lodepng::encode( "F:/Projects/blob/blob/Debug.png", tempData, _width, _height );

		if ( error ) {
			std::cout << "encoder error " << error << ": " << lodepng_error_text( error ) << std::endl;
		}
	}
	~Image( ) = default;
private:
	std::vector<Color> _data;
	unsigned int _width;
	unsigned int _height;
};