#include "AnimatedSprite.h"
#include "Graphics.h"
#include <sstream>
#include <iomanip>

Frames::Frames( std::vector<Sprite> _sprites )
	:
	m_sprites( std::move( _sprites ) )
{}

Frames::Frames( int NumFrames, std::string BaseFilename, std::string FileExtension )
{
	auto ParseFilename = [ &BaseFilename, &FileExtension ](unsigned int Idx)
	{
		std::stringstream ss;
		ss.fill( '0' );
		ss << BaseFilename << std::setw( 2 ) << Idx << FileExtension;
		return ss.str();
	};

	for( int i = 0; i < NumFrames; ++i )
	{
		m_sprites.emplace_back( ParseFilename( i ) );
	}
}

Sprite const& Frames::GetFrame( size_t Idx )const
{
	assert( Idx >= 0 );
	assert( Idx < m_sprites.size() );

	return m_sprites[ Idx ];
}

size_t Frames::Count() const
{
	return m_sprites.size();
}

Sprite const & AnimationController::CurrentFrame() const noexcept
{
	return m_pFrames->GetFrame( m_currFrame );
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
	}

	if( m_frameCounter >= m_pFrames->Count() )
	{
		m_frameCounter = 0;
		m_forward = !m_forward;
	}

	m_currFrame = ( m_forward ) ? m_frameCounter : m_pFrames->Count() - ( m_frameCounter + 1 );
}
