#pragma once

#include <memory>

template<class T>
class aligned_ptr
{
public:
	aligned_ptr() = default;
	aligned_ptr( size_t CountElements, std::align_val_t Alignment = std::align_val_t{ alignof( T ) } )
		:
		m_count( CountElements ),
		m_alignment( Alignment ),
		m_pPtr( reinterpret_cast< T* >( _aligned_malloc( CountElements * sizeof( T ), (size_t)Alignment ) ) )
	{
	}
	aligned_ptr( aligned_ptr &&Src )
	{
		*this = std::move( Src );
	}
	~aligned_ptr()
	{
		if( m_pPtr )
		{
			_aligned_free( m_pPtr );
			m_pPtr = nullptr;
		}
	}

	aligned_ptr &operator=( aligned_ptr &&Src )
	{
		if(this != std::addressof(Src))
		{
			m_count = Src.m_count;
			m_alignment = Src.m_alignment;
			m_pPtr = Src.m_pPtr;

			Src.m_alignment = std::align_val_t{ alignof( T ) };
			Src.m_count = 0u;
			Src.m_pPtr = nullptr;
		}

		return *this;
	}

	aligned_ptr( const aligned_ptr & ) = delete;
	aligned_ptr &operator=( const aligned_ptr & ) = delete;

	std::align_val_t Alignment()const
	{
		return m_alignment;
	}
	size_t Size()const
	{
		return m_count * sizeof( T );
	}
	size_t Count()const
	{
		return m_count;
	}

	operator bool()const
	{
		return m_pPtr != nullptr;
	}
	bool operator==( const aligned_ptr &Src )const
	{
		return m_pPtr == Src.m_pPtr;
	}
	bool operator!=( const aligned_ptr &Src )const
	{
		return !( *this == Src );
	}

	T& operator[]( size_t idx )
	{
		return *( m_pPtr + idx );
	}
	const T& operator[]( size_t idx )const
	{
		return *( m_pPtr + idx );
	}

	T* operator->()
	{
		return m_pPtr;
	}

	T& operator*( size_t idx )
	{
		return *m_pPtr;
	}
	const T& operator*( size_t idx )const
	{
		return *m_pPtr;
	}

	T* Get()
	{
		return m_pPtr;
	}
	const T* Get()const
	{
		return m_pPtr;
	}

	void Fill( const T &Val )
	{
		for( size_t i = 0; i < Count(); ++i )
		{
			m_pPtr[ i ] = Val;
		}
	}
	void Clear()
	{
		memset( m_pPtr, 0, Size() );
	}

private:
	T *m_pPtr;
	size_t m_count = 0u;
	std::align_val_t m_alignment = std::align_val_t{ alignof( T ) };
};

