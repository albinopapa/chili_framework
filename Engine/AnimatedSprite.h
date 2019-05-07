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

	Sprite const& GetFrame( size_t Idx )const noexcept;
	size_t Count()const noexcept;

private:
	std::vector<Sprite> m_sprites;
};



class AnimationController
{
public:
	AnimationController() = default; 
	AnimationController( float HoldFrameTime, const Frames &FrameSet, size_t _startFrame = 0u )noexcept;
	virtual ~AnimationController() = default;

	Sprite const& CurrentFrame()const noexcept;
	virtual void Advance( float DeltaTime )noexcept;
	size_t CurrentFrameIndex()const noexcept;
protected:
	size_t m_currFrame = 0;
	const Frames *m_pFrames;
	float m_frameTimeCounter = 0.f, m_holdFrameTime = 0.f;
};

class PendulumAnimController :public AnimationController
{
public:
	using AnimationController::AnimationController;

	void Advance( float DeltaTime )noexcept override;

private:
	bool m_forward = true;
	int m_frameCounter = 0;
};