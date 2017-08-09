#pragma once

#include "Vec2.h"
#include "MessageType.h"
#include <vector>

class Observer
{
public:
	Observer() = default;
	virtual ~Observer() = default;

	virtual void OnNotify( Message &Msg ) = 0;
protected:
};

class Subject
{
public:
	void RegisterObserver( Observer *pSubject );
	void UnregisterObserver( Observer *pSubejct );

protected:
	virtual void Notify(  ) = 0;

protected:
	std::vector<Observer*> m_pObservers;

};

