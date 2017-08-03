#pragma once

#include "Graphics.h"
#include "ImageLoader.h"
#include "Rect.h"

class Sprite
{
private:
	struct SpriteData
	{
		Sizei m_size;
		std::unique_ptr<Color[]> m_pPixels;
	};
public:
	Sprite() = default;
	Sprite( const std::string &Filename );
	Sprite( Sprite &&Src );
	Sprite( SpriteData &&Data );
	virtual~Sprite();

	Sprite &operator=( Sprite &&Src );

	virtual std::unique_ptr<Sprite> CopyFromRegion( const Recti &Src )const;
	virtual std::unique_ptr<Sprite> CloneMirrored( const Recti &Src )const;

	virtual void Draw( const Rectf &Dst, Graphics &Gfx )const;
	virtual void Draw( const Rectf &Src, const Rectf &Dst, Graphics &Gfx )const;

	int GetWidth()const;
	int GetHeight()const;
	Recti GetRect()const;

protected:
	Recti Rectify( const Rectf &Src )const;
	SpriteData MakeResource( const Recti &Src )const;
	SpriteData Clone( const Recti &Src )const;
	SpriteData ReverseClone( const Recti &Src )const;
private:
	std::unique_ptr<Color[]> GatherBitmapPixels( Microsoft::WRL::ComPtr<IWICBitmap> pBitmap )const;
	Sizei GatherSize( Microsoft::WRL::ComPtr<IWICBitmap> pBitmap );

protected:	
	Sizei m_size;
	std::unique_ptr<Color[]> m_pPixels;
};

