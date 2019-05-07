#include "WallCrawler.h"
#include <random>

WallCrawler::WallCrawler( Maze const & _maze ) noexcept
	:
	m_pmaze( std::addressof( _maze ) )
{

}

Rectf WallCrawler::GetRect() const noexcept
{
	return Rectf();
}
