#include "Logger.h"

std::unique_ptr<TraceLog> TraceLog::s_pLog;
std::unique_ptr<VarLogger> VarLogger::s_pLog;

Logger::Logger(const std::string &LogFilename)
	:
	m_logfile( LogFilename )
{
}

Logger::~Logger()
{
	m_logfile.close();
}

Logger &Logger::LogMessage( const std::string &Message )
{
	return LogMessage( std::string( Message ) );
}

Logger &Logger::LogMessage( std::string &&Var )
{
	m_messages.push_back( std::move( Var ) );
	return *this;
}

void Logger::Flush()
{
	for( const auto &msg : m_messages )
	{
		m_logfile << msg << "\n";
	}
	m_messages.clear();
}
