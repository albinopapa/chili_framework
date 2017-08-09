#pragma once

#include "Visitor.h"
#include "Vec2.h"

enum class MessageTypes
{
	NOT_SET,
	COLLISION,
	LIFE_START,
	LIFE_END,
	TIME_START,
	TIME_STOP,
	KEY_PRESS
};

class Message
{
public:	
	Message() = default;
	Message( MessageTypes MsgType )
		:
		m_type( MsgType )
	{
	}
	MessageTypes Type()const
	{
		return m_type;
	}

protected:
	MessageTypes m_type = MessageTypes::NOT_SET;
};

class DeathMessage :public Message
{
public:
	DeathMessage( Entity &Ent )
		:
		Message( MessageTypes::LIFE_END ),
		m_entity( Ent )
	{
	}

	Entity &GetEntity()
	{
		return m_entity;
	}
private:
	Entity &m_entity;
};