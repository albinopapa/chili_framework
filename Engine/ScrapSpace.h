#pragma once

// Code I plan to keep around for now.
// It's event/messaging code that does work, but doesn't fit
// for framework stuff at the moment.  Keeping it around in case I 
// forget how to do something.


//class EmitCondition
//{
//public:
//	operator bool()const
//	{
//		return IsMet();
//	}
//	virtual bool IsMet()const = 0;
//	virtual void Activate() = 0;
//};
//
//template<class Ty>
//using ConditionCallback = bool( Ty::* )( )const;
//template<class Ty>
//using ActionCallback = void( Ty::* )( );
//
//template<class Ty>
//class TimeBasedCondition :public EmitCondition
//{
//public:
//	TimeBasedCondition() = default;
//	TimeBasedCondition( float TimeDelay, Ty &Actor, ActionCallback<Ty> ActCallback )
//		:
//		delay( TimeDelay ),
//		start( std::chrono::high_resolution_clock::now() ),
//		actor( &Actor ),
//		Action( ActCallback )
//	{
//
//	}
//	bool IsMet()const override
//	{
//		const auto current = std::chrono::high_resolution_clock::now();
//		const auto epoch = std::chrono::duration<float>( current - start ).count();
//		return epoch > delay;
//	}
//	void Activate() override
//	{
//		( ( *actor ).*Action )( );
//		start = std::chrono::high_resolution_clock::now();
//	}
//private:
//	std::chrono::high_resolution_clock::time_point start;
//	float delay;
//	Ty *actor;
//	ActionCallback<Ty> Action;
//};
//
//template<class Ty>
//class InputBasedCondition :public EmitCondition
//{
//public:
//	enum class InputType
//	{
//		SingleKeyPress, MultiKeyPress, MouseClick, KeyMouseCombo
//	};
//
//public:
//	InputBasedCondition(
//		Keyboard &Kbd,
//		Mouse &_Mouse,
//		InputType IptType,
//		std::vector<char> &&InputCodes,
//		const Ty &Cond, ActionCallback<Ty> ActCallback )
//		:
//		pKbd( &Kbd ),
//		pMouse( &_Mouse ),
//		inType( IptType ),
//		codes( std::move( InputCodes ) ),
//		condition( Cond ),
//		Action( ActCallback )
//	{
//	}
//	InputBasedCondition( Keyboard &Kbd, InputType IptType, std::vector<char> &&InputCodes )
//		:
//		pKbd( &Kbd ),
//		inType( IptType ),
//		codes( std::move( InputCodes ) )
//	{
//	}
//	InputBasedCondition( Mouse &_Mouse, InputType IptType, std::vector<char> &&InputCodes )
//		:
//		pMouse( &_Mouse ),
//		inType( IptType ),
//		codes( std::move( InputCodes ) )
//	{
//	}
//
//	bool IsMet()const override
//	{
//		const auto KeysArePressed = [ this ]
//		{
//			bool pass = false;
//			for( const auto code : codes )
//			{
//				pass &= pKbd->KeyIsPressed( code );
//			}
//
//			return pass;
//		};
//		const auto ButtonPressed = [ this ]
//		{
//			return pMouse->LeftIsPressed() | pMouse->RightIsPressed();
//		};
//		switch( inType )
//		{
//			case InputBasedCondition::InputType::SingleKeyPress:
//				return pKbd->KeyIsPressed( codes[ 0 ] );
//			case InputBasedCondition::InputType::MultiKeyPress:
//				return KeysArePressed();
//			case InputBasedCondition::InputType::MouseClick:
//				return ButtonPressed();
//			case InputBasedCondition::InputType::KeyMouseCombo:
//				return ( pMouse->LeftIsPressed() | pMouse->RightIsPressed() ) &&
//					KeysArePressed();
//				break;
//			default:
//				assert( false || "You should never have come here." );
//				throw std::runtime_error( "You should never have come here." );
//				break;
//		}
//	}
//	void Activate() override
//	{
//		( condition.*Action )( );
//	}
//private:
//	Keyboard *pKbd = nullptr;
//	Mouse *pMouse = nullptr;
//	InputType inType;
//	std::vector<char> codes;
//	ActionCallback<Ty> Action;
//	const Ty &condition;
//};
//
//template<class Ty, class Ty2>
//class EventBasedCondition :public EmitCondition
//{
//public:
//	EventBasedCondition(
//		const Ty &Cond,
//		ConditionCallback<Ty> CondCallback,
//		Ty2 &Actor,
//		ActionCallback<Ty2> ActCallback )
//		:
//		condition( Cond ),
//		IsConditionMet( CondCallback ),
//		actor( Actor ),
//		Action( ActCallback )
//	{
//	}
//	bool IsMet()const override { return ( condition.*IsConditionMet )( ); }
//	void Activate() override
//	{
//		( actor.*Action )( );
//	}
//private:
//	ConditionCallback<Ty> IsConditionMet;
//	const Ty &condition;
//	Ty2 &actor;
//	ActionCallback<Ty2> Action;
//};
