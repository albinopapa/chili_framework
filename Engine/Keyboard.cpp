/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Keyboard.cpp																		  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "Keyboard.h"

template<typename Task>
struct do_final
{
	do_final() = delete;
	do_final( const do_final& ) = delete;
	do_final( do_final&& ) = delete;
	do_final& operator=( const do_final& )=delete;
	do_final& operator=(do_final&&)=delete;
	do_final( Task&& _task )noexcept
		:
		m_task( _task )
	{}
	~do_final()
	{
		m_task();
	}

	Task m_task;
};

bool Keyboard::KeyIsPressed( unsigned char keycode ) const noexcept
{
	return keystates[keycode];
}

std::optional<Keyboard::Event> Keyboard::ReadKey()noexcept
{
	do_final task( [ this ]() { if( !keybuffer.empty() ) { keybuffer.pop(); } } );
	return (!keybuffer.empty())?
		std::optional<Keyboard::Event>{keybuffer.front()}:
		std::optional<Keyboard::Event>{};
}

std::optional<char> Keyboard::ReadChar()noexcept
{
	do_final task( [ this ]() {if( !charbuffer.empty() ) { charbuffer.pop(); } } );
	return ( !charbuffer.empty() ) ? 
		std::optional<char>{ charbuffer.front() } : 
		std::optional<char>{ std::nullopt };
}

void Keyboard::FlushKey()noexcept
{
	keybuffer = std::queue<Event>();
}

void Keyboard::FlushChar()noexcept
{
	charbuffer = std::queue<char>();
}

void Keyboard::Flush()noexcept
{
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutorepeat()noexcept
{
	autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat()noexcept
{
	autorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept
{
	return autorepeatEnabled;
}

void Keyboard::OnKeyPressed( unsigned char keycode )
{
	keystates[ keycode ] = true;	
	keybuffer.push( Keyboard::Event( Keyboard::Event::Type::Press,keycode ) );
	TrimBuffer( keybuffer );
}

void Keyboard::OnKeyReleased( unsigned char keycode )
{
	keystates[ keycode ] = false;
	keybuffer.push( Keyboard::Event( Keyboard::Event::Type::Release,keycode ) );
	TrimBuffer( keybuffer );
}

void Keyboard::OnChar( char character )noexcept
{
	charbuffer.push( character );
	TrimBuffer( charbuffer );
}

template<typename T>
void Keyboard::TrimBuffer( std::queue<T>& buffer )noexcept
{
	while( buffer.size() > bufferSize )
	{
		buffer.pop();
	}
}

Keyboard::Event::Event() noexcept
	:
	type( Type::Invalid ),
	code( 0u )
{}

Keyboard::Event::Event( Type type, unsigned char code ) noexcept
	:
	type( type ),
	code( code )
{}

bool Keyboard::Event::IsPress()const noexcept
{
	return type == Type::Press;
}

bool Keyboard::Event::IsRelease() const noexcept
{
	return type == Type::Release;
}

bool Keyboard::Event::IsValid() const noexcept
{
	return type != Type::Invalid;
}

unsigned char Keyboard::Event::GetCode() const noexcept
{
	return code;
}
