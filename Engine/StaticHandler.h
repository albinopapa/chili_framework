#pragma once


template<class InitterType>
class StaticHandler
{
public:
	StaticHandler()
	{
		InitterType::Instance();
	}
	~StaticHandler()
	{
		InitterType::Release();
	}
};