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

void AnimationController::Draw( const Rectf & Dst, Graphics & Gfx ) const
{
	const auto &sprite = m_pFrames->GetFrame( m_currFrame );
	sprite.Draw( sprite.GetRect(), Dst, Gfx );
}

void AnimationController::Draw( const Rectf &Src, const Rectf &Dst, Graphics & Gfx ) const
{
	m_pFrames->GetFrame( m_currFrame ).Draw( Src, Dst, Gfx );
}

void AnimationController::DrawReverse( const Rectf & Dst, Graphics & Gfx ) const
{
	const auto &sprite = m_pFrames->GetFrame( m_currFrame );
	sprite.DrawReverse( sprite.GetRect(), Dst, Gfx );
}

void AnimationController::DrawReverse( const Rectf &Src, const Rectf &Dst, Graphics & Gfx ) const
{
	m_pFrames->GetFrame( m_currFrame ).DrawReverse( Src, Dst, Gfx );
}

int AnimationController::GetWidth() const
{
	return m_pFrames->GetFrame( m_currFrame ).GetWidth();
}

int AnimationController::GetHeight() const
{
	return m_pFrames->GetFrame( m_currFrame ).GetHeight();
}

AnimationController::AnimationController( float HoldFrameTime, const Frames &FrameSet )
	:
	m_holdFrameTime( HoldFrameTime ),
	m_frameTimeCounter( HoldFrameTime ),
	m_pFrames( &FrameSet )
{
}
