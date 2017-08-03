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
	Logger( const std::string &LogFilename )
		:
		m_logfile( LogFilename )
	{
	}
	virtual ~Logger()
	{
		m_logfile.close();
	}

	virtual void LogMessage( const std::string &Message ) = 0;
	virtual void LogMessage( std::string &&Message ) = 0;
	virtual void Flush() = 0;

protected:
	std::ofstream m_logfile;

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
		if( m_lines.size() >= 64 )
		{
			m_lines.erase( m_lines.begin() );
		}
		m_lines.push_back( "Entering: " + FnName + "\n" );
	}
	void Leave()
	{
		if( !m_lines.empty() )
		{
			m_lines.pop_back();
		}
	}
	void LogMessage( const std::string &Message )override 
	{
		m_lines.push_back( Message );
	}
	void LogMessage( std::string &&Message )override
	{
		m_lines.push_back( std::move( Message ) );
	}
	void Flush()
	{
		for( const auto &line : m_lines )
		{
			m_logfile << line;
		}
		m_lines.clear();
	}
private:
	static std::unique_ptr<TraceLog> s_pLog;
	std::vector<std::string> m_lines;
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