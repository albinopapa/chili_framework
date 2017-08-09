#pragma once

class Observer;
class Fireworks;
class Entity;
class Visitor
{
public:
	Visitor() = default;
	virtual ~Visitor() = default;

	virtual void Visit( Observer &Obs ) {}
	virtual void Visit( Fireworks &FWorks ) {}
};

