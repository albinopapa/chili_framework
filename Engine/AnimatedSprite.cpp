#include "AnimatedSprite.h"
#include "Graphics.h"
#include "AlphaSprite.h"
#include <sstream>
#include <iomanip>

Frames::Frames(
	SpriteType Type,
	int NumFrames,
	const std::string & BaseFilename,
	const std::string &FileExtension,
	const WicInitializer &Wic )
{
	auto ParseFilename = [ &BaseFilename, &FileExtension, &Wic ](unsigned int Idx)
	{
		std::stringstream ss;
		ss.fill( '0' );
		ss << BaseFilename << std::setw( 2 ) << Idx << FileExtension;
		return ss.str();
	};

	if( Type == SpriteType::Solid )
	{
		for( int i = 0; i < NumFrames; ++i )
		{
			m_pSprites.push_back( std::make_unique<Sprite>( ParseFilename( i ), Wic ) );
		}
	}
	else
	{
		for( int i = 0; i < NumFrames; ++i )
		{
			m_pSprites.push_back( std::make_unique<AlphaSprite>( ParseFilename( i ), Wic ) );
		}
	}
}

const Sprite & Frames::GetFrame( size_t Idx )const
{
	return *m_pSprites[ Idx ];
}

size_t Frames::Count() const
{
	return m_pSprites.size();
}

void AnimationController::Advance( float DeltaTime )
{
	m_frameTimeCounter -= DeltaTime;
	if( m_frameTimeCounter <= 0.f )
	{
		m_frameTimeCounter = m_holdFrameTime;
		++m_currFrame;
		m_currFrame %= m_pFrames->Count();
	}
}

void AnimationController::Draw( float OffsetX, float OffsetY, Graphics & Gfx ) const
{
	m_pFrames->GetFrame( m_currFrame ).Draw( OffsetX, OffsetY, Gfx );
}

void AnimationController::DrawReverse( float OffsetX, float OffsetY, Graphics & Gfx ) const
{
	m_pFrames->GetFrame( m_currFrame ).DrawReverse( OffsetX, OffsetY, Gfx );
}

int AnimationController::GetWidth() const
{
	return m_pFrames->GetFrame( m_currFrame ).GetWidth();
}

int AnimationController::GetHeight() const
{
	return m_pFrames->GetFrame( m_currFrame ).GetHeight();
}

AnimationController::AnimationController( float HoldFrameTime, Frames & FrameSet )
	:
	m_holdFrameTime( HoldFrameTime ),
	m_frameTimeCounter( HoldFrameTime ),
	m_pFrames( &FrameSet )
{
}
