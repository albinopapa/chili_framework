#pragma once

#include "ImageLoader.h"
#include "Graphics.h"

class Sprite
{
public:
	Sprite(const std::string &Filename, const WicInitializer &Wic);
	virtual~Sprite();

	virtual void Draw( float OffsetX, float OffsetY, Graphics &Gfx )const;
	virtual void DrawReverse( float OffsetX, float OffsetY, Graphics &Gfx )const;

	int GetWidth()const;
	int GetHeight()const;
	
protected:
	void Rectify( int &xStart, int &xEnd, int &yStart, int &yEnd )const;

private:
	Color *GatherBitmapPixels()const;
	int GatherWidth()const;
	int GatherHeight()const;

protected:
	Microsoft::WRL::ComPtr<IWICBitmap> m_pBitmap;
	int m_width = 0, m_height = 0;
	Color *m_pPixels = nullptr;
};

