#pragma once

#include "Sprite.h"
#include <memory>
#include <string>
#include <vector>

class Frames
{
public:
	Frames() = default;
	Frames( std::vector<Sprite> pSprites );
	Frames( int NumFrames, std::string BaseFilename, std::string FileExtension );

	Sprite const& GetFrame( size_t Idx )const;
	size_t Count()const;

private:
	std::vector<Sprite> m_sprites;
};



class AnimationController
{
public:
	AnimationController() = default; 
	AnimationController( float HoldFrameTime, const Frames &FrameSet);
	virtual ~AnimationController() = default;

	Sprite const& CurrentFrame()const noexcept;
	virtual void Advance( float DeltaTime );

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