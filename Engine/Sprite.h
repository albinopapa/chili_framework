#pragma once

#include "Graphics.h"
#include "ImageLoader.h"
#include "Rect.h"

class Sprite
{
public:
	Sprite() = default;
	Sprite( const std::string &Filename );
	Sprite( Sprite &&Src );
	Sprite( const Sizei &SrcSize, std::unique_ptr<Color[]> &&pPixels );
	virtual~Sprite();

	Sprite &operator=( Sprite &&Src );

	virtual std::unique_ptr<Sprite> CopyFromRegion( const Recti &Src )const;

	virtual void Draw( const Rectf &Dst, Graphics &Gfx )const;
	virtual void Draw( const Rectf &Src, const Rectf &Dst, Graphics &Gfx )const;
	virtual void DrawReverse( const Rectf &Dst, Graphics &Gfx )const;
	virtual void DrawReverse( const Rectf &Src, const Rectf &Dst, Graphics &Gfx )const;

	int GetWidth()const;
	int GetHeight()const;
	Recti GetRect()const;
protected:
	Recti Rectify( const Rectf &Src )const;

private:
	std::unique_ptr<Color[]> GatherBitmapPixels( Microsoft::WRL::ComPtr<IWICBitmap> pBitmap )const;
	Sizei GatherSize( Microsoft::WRL::ComPtr<IWICBitmap> pBitmap );

protected:	
	Sizei m_size;
	std::unique_ptr<Color[]> m_pPixels;
};

