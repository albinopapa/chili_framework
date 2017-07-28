#pragma once

#include "ImageLoader.h"
#include "Rect.h"
#include "Graphics.h"

class Sprite
{
public:
	Sprite( const std::string &Filename );
	virtual~Sprite();

	virtual void Draw( const Rectf &Src, const Rectf &Dst, Graphics &Gfx )const;
	virtual void DrawReverse( const Rectf &Src, const Rectf &Dst, Graphics &Gfx )const;

	int GetWidth()const;
	int GetHeight()const;
	Recti GetRect()const;
protected:
	void Rectify( int &xStart, int &xEnd, int &yStart, int &yEnd )const;
	Recti Rectify( const Rectf &Src )const;

private:
	Color *GatherBitmapPixels()const;
	int GatherWidth()const;
	int GatherHeight()const;

protected:
	Microsoft::WRL::ComPtr<IWICBitmap> m_pBitmap;
	int m_width = 0, m_height = 0;
	Color *m_pPixels = nullptr;
};

