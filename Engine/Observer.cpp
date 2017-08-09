#include "Observer.h"

void Subject::RegisterObserver( Observer * pSubject )
{
	m_pObservers.push_back( pSubject );
}

void Subject::UnregisterObserver( Observer * pSubejct )
{	
	auto endIt = std::find( m_pObservers.begin(), m_pObservers.end(), pSubejct );

	if( endIt != m_pObservers.end() )
	{
		m_pObservers.erase( endIt );
	}
}
