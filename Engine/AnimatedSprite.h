#pragma once

#include "Sprite.h"
#include <memory>
#include <string>
#include <vector>

class Frames
{
public:
	enum class SpriteType
	{
		Solid, Alpha
	};
public:
	Frames() = default;
	Frames( std::vector<std::unique_ptr<Sprite>> &&pSprites );
	Frames( SpriteType Type, int NumFrames, const std::string &BaseFilename, const std::string &FileExtension );

	std::vector<std::unique_ptr<Sprite>> CloneMirrored()const;

	const Sprite &GetFrame( size_t Idx )const;
	size_t Count()const;

private:
	std::vector<std::unique_ptr<Sprite>> m_pSprites;
};



class AnimationController
{
public:
	AnimationController() = default; 
	AnimationController( float HoldFrameTime, const Frames &FrameSet);
	virtual ~AnimationController() = default;

	virtual void Advance( float DeltaTime );
	void Draw( const Rectf &Dst, class Graphics &Gfx )const;
	void Draw( const Rectf &Src, const Rectf &Dst, class Graphics &Gfx )const;

	int GetWidth()const;
	int GetHeight()const;

protected:
	float m_frameTimeCounter = 0.f, m_holdFrameTime = 0.f;
	size_t m_currFrame = 0;
	const Frames *m_pFrames;
};

class PendulumAnimController :public AnimationController
{
public:
	using AnimationController::AnimationController;

	void Advance( float DeltaTime )override;

private:
	bool m_forward = true;
	int m_frameCounter = 0;
};