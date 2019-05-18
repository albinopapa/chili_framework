#pragma once

#ifndef _INC_WINDOWS
#include <Windows.h>
#endif

#include <emmintrin.h>
#include <pmmintrin.h>

#include <memory>
#include "ChiliMath.h"
#include "MathOps.h"

using FLOAT8 = __m256;
using FLOAT4 = __m128;
using DQWORD = __m128i;
using DDQWORD = __m256i;

//************************************************************/
//*   SSE float intrinsic functions end in ss, ps, sd and pd */
//*   SS = Scalar Single precision float ie. float           */
//*   PS = Packed Single precision float ie. four floats		*/
//*   SD = Scalar Double precision float ie. double          */
//*   PD = Packed Double precision float ie. two doubles     */
//*															*/
//*   The same notation will be used in this file            */
//************************************************************/

template <typename T>
inline void SafeRelease(T *A)
{
	if (A)
	{
		A->Release();
		A = nullptr;
	}
}

template <typename T>
inline void SafeDelete(T *A)
{
	if (A)
	{
		delete A;
		A = nullptr;
	}
}

template <typename T>
inline void SafeDeleteArray(T *A)
{
	if (A)
	{
		delete[] A;
		A = nullptr;
	}
}

template <typename T>
inline void SafeDeletePtrArray(T *A, UINT Count)
{
	if (Count > 0)
	{
		if (A)
		{
			for (int i = 0; i < Count; ++i)
			{
				if (A[i])
				{
					delete A[i];
					A[i] = nullptr;
				}
			}
		}

		SafeDeleteArray(A);
	}
}

// Aligned declarations
/*
Put Align16 before structs or classes to have them aligned
when creating objects
*/
#define Align16 _declspec(align(16))

// Aligned memory allocation
/*
Used for allocating and freeing memory for arrays of data you need aligned
*/
template <class T>
class alignas( 16 ) UniqueAlign
{
public:
	UniqueAlign()
		:
		m_ptr( nullptr )
	{
	}
	explicit UniqueAlign( T* Ptr )
		:
		m_ptr( Ptr )
	{
	}
	UniqueAlign( const UniqueAlign &Other ) = delete;
	explicit UniqueAlign( UniqueAlign &&Other )
	{
		Release();
		std::swap( *this, Other );
	}
	T *operator->()
	{
		return m_ptr;
	}
	T &operator[]( unsigned Idx )
	{		
		return m_ptr[ Idx ];
	}
	T* Get()const
	{
		return m_ptr;
	}
	UniqueAlign &operator=( const UniqueAlign &Other ) = delete;
	UniqueAlign &operator=( UniqueAlign &&Other )
	{
		Release();
		m_ptr = Other.m_ptr;
		Other.m_ptr = nullptr;
		return *this;
	}
	void Release()
	{
		if( m_ptr )
		{
			_aligned_free( m_ptr );
			m_ptr = nullptr;
		}
	}
	void Reset( T *Ptr )
	{
		if( m_ptr == Ptr )
		{
			return;
		}
		Release();
		m_ptr = Ptr;
	}

private:
	T *m_ptr;
};
template <typename T>
inline T* AlignedMalloc(UINT Count)
{
	return (T*)_aligned_malloc(Count * sizeof(T), 16);
}
template <typename T>
inline void SafeAlignedFree(T *Pointer)
{
	if (Pointer)
	{
		_aligned_free(Pointer);
		Pointer = nullptr;
	}
}

// Float helpers
/*
LoadA_ps
Loads a float array of 4 floats and returns a FLOAT4
FLOAT4 fp4 = LoadA_ps(fpA);
*/
#define LoadA_ps _mm_load_ps

/*
LoadU_ps
Loads a float array of 4 floats that is unaligned and returns
a FLOAT4.  Working with unaligned data is slower than working
with aligned data.
FLOAT4 fp4 = LoadU_ps(fpU);
*/
#define LoadU_ps _mm_loadu_ps

/*
StoreA_ps stores a FLOAT4 into a float array that must be 16 byte aligned
Align16 float a[4];
FLOAT4 B;
StoreA_ps(a, B);
*/
#define StoreA_ps _mm_store_ps

/*
StoreU_ps stores a FLOAT4 into a float array that does not have to be aligned
float a[4];
FLOAT4 B;
StoreU_ps(a, B);
*/
#define StoreU_ps _mm_storeu_ps


enum ShuffConstants
{
	Axx	=	0,
	Ayx = (0 | 1),
	Azx = (0 | 2),
	Awx = (0 | 3),
	Axy = (1 << 2 | 0),
	Ayy = (1<<2 | 1),
	Azy = (1<<2 | 2),
	Awy = (1<<2 | 3),
	Axz = (2<<2 | 0),
	Ayz = (2<<2 | 1),
	Azz = (2<<2 | 2),
	Awz = (2<<2 | 3),
	Axw = (3<<2 | 0),
	Ayw = (3<<2 | 1),
	Azw = (3<<2 | 2),
	Aww = (3<<2 | 3),

	Bxx = 0,
	Byx = (0 | 1 << 4),
	Bzx = (0 | 2 << 4),
	Bwx = (0 | 3 << 4),
	Bxy = (1 << 6 | 0),
	Byy = (1 << 6 | 1 << 4),
	Bzy = (1 << 6 | 2 << 4),
	Bwy = (1 << 6 | 3 << 4),
	Bxz = (2 << 6 | 0),
	Byz = (2 << 6 | 1 << 4),
	Bzz = (2 << 6 | 2 << 4),
	Bwz = (2 << 6 | 3 << 4),
	Bxw = (3 << 6 | 0),
	Byw = (3 << 6 | 1 << 4),
	Bzw = (3 << 6 | 2 << 4),
	Bww = (3 << 6 | 3 << 4)
};

enum ShuffleOrder
{
	// Splat
	XXXX = Axx + Bxx,
	YYYY = Ayy + Byy,
	ZZZZ = Azz + Bzz,
	WWWW = Aww + Bww,

	// Replicate
	XYXY = Axy + Bxy,
	XZXZ = Axz + Bxz,
	XWXW = Axw + Bxw,
	YXYX = Ayx + Byx,
	YZYZ = Ayz + Byz,
	YWYW = Ayw + Byw,
	ZXZX = Azx + Bzx,
	ZYZY = Azy + Bzy,
	ZWZW = Azw + Bzw,

	// Mirror
	XYYX = Axy + Byx,
	XZZX = Axz + Bzx,
	XWWX = Axw + Bwx,
	YXXY = Ayx + Bxy,
	YZZY = Ayz + Bzy,
	YWWY = Ayw + Bwy,
	ZXXZ = Azx + Bxz,
	ZYYZ = Azy + Byz,
	ZWWZ = Azw + Bwz,
	WXXW = Awx + Bxw,
	WYYW = Awy + Byw,
	WZZW = Awz + Bzw,

	// Roll
	YZWX = Ayz + Bwx,
	ZWXY = Azw + Bxy,
	WXYZ = Awx + Byz,

	// Reverse
	WZYX = Awz + Byx

};

#define Shuffle(A, B, C) (_mm_shuffle_ps(A, B, C))

#pragma region Splatting
/*
Splatting is taking one of the components and making all the
other components the same.  For instance, SplatX takes the
first component X, and makes all the components the same value
as X.
A = x, y, z, w;
A = SplatX(A);
A = x, x, x, x;
*/

#define SplatX(A) (Shuffle(A, A, XXXX))
#define SplatY(A) (Shuffle(A, A, YYYY))
#define SplatZ(A) (Shuffle(A, A, ZZZZ))
#define SplatW(A) (Shuffle(A, A, WWWW))
#pragma endregion Splat shuffling

/*
Reverse
Example:
FLOAT4 res = Reverse(x,y,z,w)
res = w,z,y,x
*/
#define ReverseVector(A) (Shuffle(A, A, WZYX))

#pragma region Replicate
/*
Replicate
Example:
x, y, z, w to (x, y, x, y)
*/
#define ReplicateXY(A) (Shuffle(A, A, XYXY))
#define ReplicateXZ(A) (Shuffle(A, A, XZXZ))
#define ReplicateXW(A) (Shuffle(A, A, XWXW))

#define ReplicateYX(A) (Shuffle(A, A, YXYX))
#define ReplicateYZ(A) (Shuffle(A, A, YZYZ))
#define ReplicateYW(A) (Shuffle(A, A, YWYW))

#define ReplicateZX(A) (Shuffle(A, A, ZXZX))
#define ReplicateZY(A) (Shuffle(A, A, ZYZY))
#define ReplicateZW(A) (Shuffle(A, A, ZWZW))
#pragma endregion Replicate shuffling

#pragma region Mirror
/*
Mirror??
Example:
A = x, y, z, w
B = MirrorXY(A)
B = Ax, Ay, Ay, Ax
*/
#define MirrorXY(A) (Shuffle(A, A, XYYX))
#define MirrorXZ(A) (Shuffle(A, A, XZZX))
#define MirrorXW(A) (Shuffle(A, A, XWWX))

#define MirrorYX(A) (Shuffle(A, A, YXXY))
#define MirrorYZ(A) (Shuffle(A, A, YZZY))
#define MirrorYW(A) (Shuffle(A, A, YWWY))

#define MirrorZX(A) (Shuffle(A, A, ZXXZ))
#define MirrorZY(A) (Shuffle(A, A, ZYYZ))
#define MirrorZW(A) (Shuffle(A, A, ZWWZ))

#define MirrorWX(A) (Shuffle(A, A, WXXW))
#define MirrorWY(A) (Shuffle(A, A, WYYW))
#define MirrorWZ(A) (Shuffle(A, A, WZZW))
#pragma endregion Mirror shuffling

/*
Repeat
float a = 0.0f
FLOAT4 A = Repeat(a);
A = 0.0f, 0.0f, 0.0f, 0.0f
*/
#define Repeat _mm_set1_ps

/*
SetFloat4
Creates a FLOAT4 using 4 floats
SetFloat4(f0, f1, f2, f3).  SetFloat4 reverses the values so that
they appear in the order they were set
*/
#define SetFloat4(f0, f1, f2, f3) (_mm_setr_ps(f0, f1, f2, f3))

/*
MoveHiLo
Example:
A = x, y, z, w
B = x, y, z, w
res = Bz, Bw, Az, Aw;
*/
#define MoveHiLo _mm_movehl_ps
/*
MoveLoHi
Example:
A = x, y, z, w
B = x, y, z, w
res = Ax, Ay, Bx, By
*/
#define MoveLoHi _mm_movelh_ps

/*
Add
A + B
*/
#define Add _mm_add_ps

/*
Sub
A - B
*/
#define Sub _mm_sub_ps

/*
Mul
A * B
*/
#define Mul _mm_mul_ps

/*
Div
A * recip(B)
It's faster to take the reciprocal of a number and multiply 
than to divide
*/
#define Div _mm_div_ps

/*
Recip
1.0f / A
*/
#define Recip _mm_rcp_ps

/*
Sqrt
square root
*/
#define Sqrt _mm_sqrt_ps

/*
RecipSqrt
1.0 / sqrt(A)
*/
#define RecipSqrt _mm_rsqrt_ps

/*
AndNot
Inverts the bits in A and (and's) them with B
A = 0011, B = 1010
C = AndNot(A, B)  (1100 & 1010)
C = 1000
*/
#define AndNot _mm_andnot_ps

/*
And
Logical and ( & )
*/
#define And _mm_and_ps

/*
Or
Logical or ( | )
*/
#define Or _mm_or_ps

#define gt _mm_cmpgt_ps
#define lt _mm_cmplt_ps
#define equals _mm_cmpeq_ps
#define noteq _mm_cmpneq_ps
#define lte _mm_cmple_ps
#define gte _mm_cmpge_ps

/*
Roll(FLOAT4, ShuffleOrder) or Roll(FLOAT4, ShuffleConstants)
Shuffles the elements to mimic rolling
A = 1.0f, 2.0f, 3.0f, 4.0f
Roll(A, ShuffleOrder::WXYZ) 
A = 4.0f, 1.0f, 2.0f, 3.0f
Roll(A, ShuffleConstants::Ayz + ShuffleConstants::Bwx)
A = 1.0f, 2.0f, 3.0f, 4.0f
*/
#define Roll(A, B) (Shuffle((A), (A), (B)))

/*
RollR
Shifts all elements to the right, and puting the far right element
on the left.
X, Y, Z, W becomes W, X, Y, Z
*/
#define RollR(A) (_mm_shuffle_ps(A, A, Awx + Byz))

/*
RollL
Shifts all elements to the left and putting the far left element
on the right
X, Y, Z, W becomes Y, Z, W, X
*/
#define RollL(A) (_mm_shuffle_ps(A, A, Ayz + Bwx))

/*
Zerops returns a FLOAT4 with all elements set to 0.0f
A = Zerops
A = 0.0f, 0.0f, 0.0f, 0.0f
*/
#define Zerops _mm_setzero_ps()

/*
Oneps sets all elements to 1.0f
A = Oneps
A = 1.0f, 1.0f, 1.0f, 1.0f
*/
#define Oneps _mm_set1_ps(1.0f)

/*
ConvertToFloat4
Converts DQWORD (__m128i) to FLOAT4 (__m128)
Converts the four ints to four floats 
1, 2, 10, 20 becomes 1.0f, 2.0f, 10.0f, 20.0f
*/
#define CvtToFloat4 _mm_cvtepi32_ps

/*
ConvertToDQWord
Converts FLOAT4 (__m128) to DQWORD (__m128i)
Converts the four floats to four ints, rounding to nearest whole number
1.1f, 2.0092f, 10.9f, 2.0f becomes 1, 2, 11, 2
*/
#define CvtToDQWord _mm_cvtps_epi32

/*
CvtTrunc
Converts FLOAT4 to DQWORD by first truncating or removing everything 
to the right of the decimal
*/
#define CvtTrunc _mm_cvttps_epi32

/*
CastToFloat4
Casts the binary value from DQWORD to FLOAT4, no conversion or rounding
is done.
*/
#define CastToFloat4 _mm_castsi128_ps

/*
CastToDQWord
Casts the binary value from FLOAT4 to DQWORD, no conversion is done.
*/
#define CastToDQWord _mm_castps_si128

/*
Streamps
Bypasses the cpu cache.  Use when you want to copy large amounts of data 
from one location to another and aren't going to be processing it.

*/
#define Streamps _mm_stream_ps

#define Minps _mm_min_ps
#define Maxps _mm_max_ps


Align16 
struct Matrix
{
	FLOAT4 r[4];
};


inline FLOAT4 _vectorcall operator+( const FLOAT4 &A, const FLOAT4 &B )
{
	return Add( A, B );
}
inline FLOAT4 _vectorcall operator+=( FLOAT4 &A, const FLOAT4 &B )
{
	A = Add( A, B );
	return A;
}
inline FLOAT4 _vectorcall operator-( const FLOAT4 &A, const FLOAT4 &B )
{
	return Sub( A, B );
}
inline FLOAT4 _vectorcall operator*( const FLOAT4 &A, const FLOAT4 &B )
{
	return Mul( A, B );
}
inline FLOAT4 _vectorcall operator*( const FLOAT4 &A, const float &B )
{
	return A * Repeat( B );
}
inline FLOAT4 _vectorcall operator/( const FLOAT4 &A, const FLOAT4 &B )
{
	return A * Recip( B );
}
inline FLOAT4 _vectorcall operator-( const FLOAT4 &A )
{
	return Sub( Zerops, A );
}

// Logical operators
inline FLOAT4 _vectorcall operator&( const FLOAT4 &A, const FLOAT4 &B )
{
	return And( A, B );
}
inline FLOAT4 _vectorcall operator|( const FLOAT4 &A, const FLOAT4 &B )
{
	return Or( A, B );
}
inline FLOAT4 _vectorcall operator~( const FLOAT4 &A )
{
	return AndNot( equals( A, A ), A );
}

// Compare operators
inline FLOAT4 _vectorcall operator>( const FLOAT4 &A, const FLOAT4 &B )
{
	return gt( A, B );
}
inline FLOAT4 _vectorcall operator<( const FLOAT4 &A, const FLOAT4 &B )
{
	return lt( A, B );
}

inline FLOAT4 _vectorcall operator==( const FLOAT4 &A, const FLOAT4 &B )
{
	return equals( A, B );
}

inline FLOAT4 _vectorcall operator>=( const FLOAT4 &A, const FLOAT4 &B )
{
	return gte( A, B );
}
inline FLOAT4 _vectorcall operator<=( const FLOAT4 &A, const FLOAT4 &B )
{
	return lte( A, B );
}
inline FLOAT4 _vectorcall operator!=( const FLOAT4 &A, const FLOAT4 &B )
{
	return noteq( A, B );
}



namespace float_sse
{
	inline FLOAT4 _vectorcall MaskX(const FLOAT4 &A)
	{
		FLOAT4 B = Shuffle(A, Zerops, XXXX);
		return Shuffle(B, B, Axz + Bzw);
	}
	inline FLOAT4 _vectorcall MaskY(const FLOAT4 &A)
	{
		FLOAT4 B = Shuffle(A, Zerops, Ayy);
		return Shuffle(B, B, Azy + Bzw);
	}
	inline FLOAT4 _vectorcall MaskZ(const FLOAT4 &A)
	{
		FLOAT4 B = Shuffle(Zerops, A, Bzz);
		return Shuffle(B, B, Bzx);
	}
	inline FLOAT4 _vectorcall MaskW(const FLOAT4 &A)
	{
		FLOAT4 B = Shuffle(Zerops, A, Bww);
		return Shuffle(B, B, Bxw);
	}
	inline FLOAT4 _vectorcall Abs(const FLOAT4 &A)
	{
		return AndNot(Repeat(absMask), A);
	}

	// Vector operations
	inline FLOAT4 _vectorcall DotProduct(const FLOAT4 &A, const FLOAT4 &B)
	{
		FLOAT4 t0 = A * B;
		FLOAT4 t5 = _mm_hadd_ps(t0, t0);
		t5 = _mm_hadd_ps(t5, t5);
		return t5;
	}
	inline FLOAT4 _vectorcall LengthSqr(const FLOAT4 &A)
	{
		return DotProduct(A, A);
	}
	inline FLOAT4 _vectorcall Length(const FLOAT4 &A)
	{
		return Sqrt(LengthSqr(A));
	}
	inline FLOAT4 _vectorcall RecipLength(const FLOAT4 &A)
	{
		return RecipSqrt(LengthSqr(A));
	}
	inline FLOAT4 _vectorcall Normalize(const FLOAT4 &A)
	{
		return A * RecipLength(A);
	}
	inline FLOAT4 _vectorcall Interpolate(const FLOAT4 &A, const FLOAT4 &B, const FLOAT4 &Step)
	{
		const FLOAT4 dist = (B - A);
		const FLOAT4 rcpDist = Length(dist);
		const FLOAT4 stepDist = Step / rcpDist;
		const FLOAT4 deltaDist = dist * stepDist;
		const FLOAT4 dif = A + deltaDist;
		return dif;
	}
	inline FLOAT4 _vectorcall Ceil(const FLOAT4 &A)
	{
		DQWORD b = _mm_cvttps_epi32(A);
		FLOAT4 B = _mm_cvtepi32_ps(b);
		FLOAT4 fMask = B < A;
		DQWORD iMask = _mm_castps_si128(fMask);
		return B - _mm_cvtepi32_ps(iMask);
	}
	inline FLOAT4 _vectorcall Floor(const FLOAT4 &A)
	{
		DQWORD b = _mm_cvttps_epi32(A);
		FLOAT4 B = _mm_cvtepi32_ps(b);
		FLOAT4 fMask = B > A;
		DQWORD iMask = _mm_castps_si128(fMask);
		fMask = _mm_cvtepi32_ps(iMask);
		return B + fMask;
	}

	// Matrix operations
	inline Matrix _vectorcall Transpose(const Matrix &M)
	{
		FLOAT4 t0, t1;
		Matrix out;
		t0 = Shuffle(M.r[0], M.r[1], XYXY);
		t1 = Shuffle(M.r[2], M.r[3], XYXY);
		out.r[0] = Shuffle(t0, t1, XZXZ);
		out.r[1] = Shuffle(t0, t1, YWYW);

		t0 = Shuffle(M.r[0], M.r[1], ZWZW);
		t1 = Shuffle(M.r[2], M.r[3], ZWZW);
		out.r[2] = Shuffle(t0, t1, XZXZ);
		out.r[3] = Shuffle(t0, t1, YWYW);

		return out;
	}
	inline Matrix _vectorcall operator+(const Matrix &M0, const Matrix &M1)
	{
		Matrix res;
		for (int i = 0; i < 4; ++i)
		{
			res.r[i] = M0.r[i] + M1.r[i];
		}

		return res;
	}
	inline Matrix _vectorcall operator-(const Matrix &M0, const Matrix &M1)
	{
		Matrix res;
		for (int i = 0; i < 4; ++i)
		{
			res.r[i] = M0.r[i] - M1.r[i];
		}

		return res;
	}
	inline FLOAT4 _vectorcall operator*(const FLOAT4 &V, const Matrix &M)
	{		
		Matrix tm = Transpose( M );

		FLOAT4 X = V * tm.r[0];
		FLOAT4 Y = V * tm.r[1];
		FLOAT4 Z = V * tm.r[2];
		FLOAT4 W = V * tm.r[3];
		FLOAT4 tY = _mm_hadd_ps(X, Y);
		FLOAT4 tW = _mm_hadd_ps(Z, W);
		FLOAT4 t4 = _mm_hadd_ps(tY, tW);
			
		return t4;
	}
	inline Matrix _vectorcall operator*(const Matrix &M0, const Matrix &M1)
	{
		Matrix res;
		
		for (int i = 0; i < 4; ++i)
		{
			res.r[i] = M0.r[i] * M1;
		}

		return res;
	}
	
	inline Matrix _vectorcall Identity()
	{
		Matrix res;
		res.r[0] = MaskX(Oneps);
		res.r[1] = RollR(res.r[0]);
		res.r[2] = RollR(res.r[1]);
		res.r[3] = RollR(res.r[2]);

		return res;
	}
	inline Matrix _vectorcall RotateX(const float theta)
	{
		float angle = ToRadians(theta);
		float sinTheta = sin(angle);
		float cosTheta = cos(angle);

		Matrix res = Identity();
		res.r[1] = SetFloat4(0.0f, cosTheta, sinTheta, 0.0f);
		res.r[2] = SetFloat4(0.0f, -sinTheta, cosTheta, 0.0f);
		
		return res;
	}
	inline Matrix _vectorcall RotateY(const float theta)
	{
		float angle = ToRadians( theta );
		float sinTheta = sin(angle);
		float cosTheta = cos(angle);

		Matrix res = Identity();
		res.r[0] = SetFloat4(cosTheta, 0.0f, -sinTheta, 0.0f);
		res.r[2] = SetFloat4(sinTheta, 0.0f, cosTheta, 0.0f);

		return res;
	}
	inline Matrix _vectorcall RotateZ(const float theta)
	{
		float angle = ToRadians( theta );
		float sinTheta = sin(angle);
		float cosTheta = cos(angle);

		Matrix res = Identity();
		res.r[0] = SetFloat4(cosTheta, sinTheta, 0.0f, 0.0f);
		res.r[1] = SetFloat4(-sinTheta, cosTheta, 0.0f, 0.0f);

		return res;
	}
	inline Matrix _vectorcall Rotate(const FLOAT4 Theta)
	{
		float *ft = (float *)&Theta;
		return RotateZ(ft[2]) * RotateX(ft[0]) * RotateY(ft[1]);
	}
	inline Matrix _vectorcall Translate(const FLOAT4 Trans)
	{
		Matrix res = Identity();
		res.r[3] = Trans;
		return res;
	}
	inline Matrix _vectorcall Scale(const FLOAT4 scale)
	{
		Matrix res = Identity();
		for (int i = 0; i < 4; ++i)
		{
			res.r[i] = res.r[i] * scale;
		}

		return res;
	}
	
	inline void _vectorcall FastCopy(float *const fcArray, const FLOAT4 &Val, const UINT floatCount)
	{
		int fCount = (int)floatCount % 16;
		int inLoop = (int)floatCount / 16;

		size_t fAddy = (size_t)fcArray;
		if (fAddy % 16 != 0)
		{
			throw("Float array not aligned to 16 byte boundary.");
		}

		if (floatCount < 64)
		{
			for (UINT i = 0; i < floatCount; i += 4)
			{
				StoreA_ps(&fcArray[i], Val);
			}
		}
		else
		{
			for (int i = 0; i < inLoop; i += 16)
			{				
				Streamps(&fcArray[i], Val);
			}
			for (int i = inLoop - fCount; i < inLoop + fCount; i += 4)
			{
				fcArray[i] = (Val.m128_f32[i]);
			}
		}
	}

	struct alignas( 16 ) VectorFloat4
	{
		VectorFloat4()
			:
			x( Zerops ), y( Zerops ), z( Zerops ), w( Zerops )
		{
		}
		VectorFloat4( FLOAT4 X, FLOAT4 Y, FLOAT4 Z, FLOAT4 W )
			:
			x( X ), y( Y ), z( Z ), w( W )
		{

		}

		VectorFloat4 _vectorcall operator+( const VectorFloat4 &V )const
		{
			return VectorFloat4( x + V.x, y + V.y, z + V.z, w + V.w );
		}
		VectorFloat4 &_vectorcall operator+=( const VectorFloat4 &V )
		{
			auto &self = *this;
			self = self + V;
			return self;
		}
		VectorFloat4 _vectorcall operator-( const VectorFloat4 &V )const
		{
			return VectorFloat4( x - V.x, y - V.y, z - V.z, w - V.w );
		}
		VectorFloat4 _vectorcall operator*( const FLOAT4 &S )const
		{
			return VectorFloat4( x * S, y * S, z * S, w * S );
		}
		VectorFloat4 _vectorcall operator/( const VectorFloat4 &V )const
		{
			return VectorFloat4( x / V.x, y / V.y, z / V.z, w / V.w );
		}
		VectorFloat4 _vectorcall operator-()
		{
			return VectorFloat4( -x, -y, -z, -w );
		}

		FLOAT4 _vectorcall DotProduct( const VectorFloat4 &V )
		{
			return ( ( x * V.x ) + ( y * V.y ) + ( z * V.z ) + ( w * V.w ) );
		}
		FLOAT4 _vectorcall LengthSqr()
		{
			return DotProduct( *this );
		}
		FLOAT4 _vectorcall Length()
		{
			return Sqrt( LengthSqr() );
		}
		FLOAT4 _vectorcall RecipLength()
		{
			return RecipSqrt( LengthSqr() );
		}
		VectorFloat4 _vectorcall Normalize()
		{
			return *this * RecipLength();
		}

		FLOAT4 x, y, z, w;
	};
}
