#pragma once

#include "Sprite.h"
#include <string>
#include <vector>

class Frames
{
public:
	Frames( int NumFrames, const std::string &BaseFilename, const std::string &FileExtension, const WicInitializer &Wic );

	const Sprite &GetFrame( size_t Idx )const;
	size_t Count()const;

private:
	std::vector<Sprite> m_sprites;
};

class AnimationController
{
public:
	AnimationController( float HoldFrameTime, Frames &FrameSet);
	
	void Advance( float DeltaTime );
	void Draw( float OffsetX, float OffsetY, class Graphics &Gfx )const;

	int GetWidth()const;
	int GetHeight()const;

private:
	float m_frameTimeCounter = 0.f, m_holdFrameTime = 0.f;
	size_t m_currFrame = 0;
	const Frames *m_pFrames;
};