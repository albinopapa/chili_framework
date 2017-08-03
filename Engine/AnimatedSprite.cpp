#include "AnimatedSprite.h"
#include "Graphics.h"
#include "AlphaSprite.h"
#include <sstream>
#include <iomanip>

Frames::Frames( std::vector<std::unique_ptr<Sprite>>&& pSprites )
{
	m_pSprites.resize( pSprites.size() );
	for( int i = 0; i < pSprites.size(); ++i )
	{
		m_pSprites[ i ] = std::move( pSprites[ i ] );
	}
}

Frames::Frames(
	SpriteType Type,
	int NumFrames,
	const std::string & BaseFilename,
	const std::string &FileExtension )
{
	auto ParseFilename = [ &BaseFilename, &FileExtension ](unsigned int Idx)
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
			m_pSprites.push_back( std::make_unique<Sprite>( ParseFilename( i ) ) );
		}
	}
	else
	{
		for( int i = 0; i < NumFrames; ++i )
		{
			m_pSprites.push_back( std::make_unique<AlphaSprite>( ParseFilename( i ) ) );
		}
	}
}

std::vector<std::unique_ptr<Sprite>> Frames::CloneMirrored() const
{
	std::vector<std::unique_ptr<Sprite>> result( m_pSprites.size() );
	for( int i = 0; i < m_pSprites.size(); ++i )
	{
		auto &pResult = result[ i ];
		const auto &pSprite = m_pSprites[ i ];
		pResult = pSprite->CloneMirrored( pSprite->GetRect() );
	}

	return result;
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

void PendulumAnimController::Advance( float DeltaTime )
{
	m_frameTimeCounter -= DeltaTime;
	if( m_frameTimeCounter <= 0.f )
	{
		m_frameTimeCounter = m_holdFrameTime;
		++m_frameCounter;

		if( m_frameCounter >= m_pFrames->Count() )
		{
			m_frameCounter = 0;
			m_forward = !m_forward;
		}

		m_currFrame = (m_forward) ? m_frameCounter : m_pFrames->Count() - ( m_frameCounter + 1 );
	}
}
