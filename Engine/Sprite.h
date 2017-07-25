#pragma once

#include "ImageLoader.h"
#include "Graphics.h"

class Sprite
{
public:
	Sprite(const std::string &Filename, const WicInitializer &Wic);
	virtual~Sprite();

	virtual void Draw( float OffsetX, float OffsetY, Graphics &Gfx )const;

	int GetWidth()const;
	int GetHeight()const;
	
protected:
	void Rectify( int &xStart, int &xEnd, int &yStart, int &yEnd )const;

private:
	Microsoft::WRL::ComPtr<IWICBitmapLock> Lock()const;
	int GatherWidth()const;
	int GatherHeight()const;
private:
	Microsoft::WRL::ComPtr<IWICBitmap> m_pBitmap;
	int m_width = 0, m_height = 0;
	Microsoft::WRL::ComPtr<IWICBitmapLock> m_pLock;

};

