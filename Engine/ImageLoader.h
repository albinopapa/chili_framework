#pragma once

#include "ComponentInitter.h"
#include <string>

// ImageLoader loads .bmp, .jpg and .png files.
class ImageLoader
{
public:
	static Microsoft::WRL::ComPtr<IWICBitmap> Load( const std::string &Filename );
};

