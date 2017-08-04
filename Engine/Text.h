#pragma once

#include "Font.h"
#include <vector>

class Text
{
public:
	enum class EndOfLine
	{
		WordWrap, Truncate, None
	};
	enum class Alignment_H
	{	
		Left, Centered, Right
	};
	enum class Alignment_V
	{
		Top, Middle, Bottom
	};
	struct TextLayout
	{
		EndOfLine endofline = EndOfLine::WordWrap;
		Alignment_H align_h = Alignment_H::Left;
		Alignment_V align_v = Alignment_V::Top;
		size_t tabSpaceCount = 4;
	};
public:
	Text();
	Text( const std::string &Str, const TextLayout &Layout, const Rectf &Boundary, const Font &Fnt);

	void AlignTop();
	void AlignMiddle();
	void AlignBottom();
	void AlignLeft();
	void AlignCenter();
	void AlignRight();

	Rectf GetActualBoundary()const;
	static Rectf EstimateMinRect( const std::string &Str, const Font &Fnt );
	void Draw( const Vec2f &Position, Color C, Graphics &Gfx )const;

private:
	void FormatTrunc();
	void FormatWrap();
	void FormatNone();
	void UpdatePositionsY( size_t yStart, size_t NumLines, size_t CharHeight );
private:
	const Font* m_pFont;
	TextLayout m_layout;
	Recti m_boundary;
	std::string m_baseString;
	std::vector<std::string> m_strings;
	std::vector<Vec2i> m_positions;
};

