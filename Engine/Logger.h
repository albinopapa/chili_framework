#pragma once

#include <cassert>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#define GetFunctionName() __FUNCSIG__

class Logger
{
public:
	Logger( const std::string &LogFilename );
	virtual ~Logger();

	template<class ValType>
	Logger &LogMessage( std::string &&Var, const ValType &Val )
	{
		return LogMessage( std::move( Var ), std::to_string( Val ) );
	}

	template<>
	Logger &LogMessage<std::string>( std::string &&Var, const std::string &Val )
	{
		return LogMessage( Var + " = " + Val );
	}

	Logger &LogMessage( const std::string &Message );
	Logger &LogMessage( std::string &&Var );	
	void Flush();

protected:
	std::ofstream m_logfile;
	std::vector<std::string> m_messages;
};

class VarLogger :public Logger
{
public:
	using Logger::Logger;
	static VarLogger &Instance( const std::string &LogFilename )
	{		
		if( !s_pLog )
		{
			s_pLog = std::make_unique<VarLogger>( LogFilename );
		}

		return *s_pLog;
	}

private:
	static std::unique_ptr<VarLogger> s_pLog;
};
class TraceLog :public Logger
{
public:

	TraceLog()
		:
		Logger( "Tracelog.txt" )
	{
	}

	static TraceLog &Instance()
	{
		if( !s_pLog )
		{
			s_pLog = std::make_unique<TraceLog>();
		}
		
		return *s_pLog;
	}

	void Enter( const std::string &FnName )
	{
		if( m_messages.size() >= 64 )
		{
			m_messages.erase( m_messages.begin() );
		}
		m_messages.push_back( "Entering: " + FnName + "\n" );
	}
	void Leave()
	{
		if( !m_messages.empty() )
		{
			m_messages.pop_back();
		}
	}

private:
	static std::unique_ptr<TraceLog> s_pLog;
};

class AutoLogger
{
public:
	AutoLogger( std::string &&LogMsg )
		:
		m_logMsg( std::move( LogMsg ) )
	{
		TraceLog::Instance().Enter( m_logMsg );
	}
	~AutoLogger()
	{
		TraceLog::Instance().Leave();
	}

private:
	std::string m_logMsg;
};