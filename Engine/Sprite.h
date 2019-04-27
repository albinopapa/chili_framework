#pragma once

#include "Colors.h"
#include "ImageLoader.h"
#include "Rect.h"
#include "../../Includes/surface.h"

class Sprite :public dim2d::surface<Color>
{
public:
	using value_type		= Color;
	using pointer			= Color * ;
	using reference			= Color & ;
	using const_pointer		= Color const*;
	using const_reference	= Color const&;

public:
	Sprite() = default;
	Sprite( const std::string &Filename );
	Sprite( Sprite&& ) = default;
	Sprite& operator=( Sprite&& ) = default;

	int GetWidth()const;
	int GetHeight()const;
	Recti GetRect()const;

private:
	dim2d::surface<Color> GatherBitmapPixels( Microsoft::WRL::ComPtr<IWICBitmap> pBitmap )const;
	Sprite( dim2d::surface<Color> _surface )noexcept;
};

