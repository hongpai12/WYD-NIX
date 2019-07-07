/*
* Nix Team
* Erick Mota e Washington Paiva
* Todos os direitos de uso reservados
*/
#ifndef __LOG_H__
#define __LOG_H__

#include <Windows.h>
#include <fstream>
#include <string>
#include <utility>
#include <ctime>
#include <iostream>
#include <io.h>
#include <fcntl.h>

class Log
{
public:
	explicit Log(const std::string filePath, const std::string initMessage, bool defaultOutput = true)
	{
		m_defaultOutput = defaultOutput;
		// Set to non-buffering mode
		m_logFile.rdbuf()->pubsetbuf(0, 0);

		m_logFile.open(filePath, std::ofstream::out | std::ofstream::trunc);

		if (m_logFile.is_open())
		{
			m_logFile << initMessage << std::endl;
			if (m_defaultOutput)
				std::cout << initMessage << std::endl;
		}
	}
	~Log()
	{
		if (m_logFile.is_open())
			m_logFile.close();
	}

	template<typename ...T>
	void debug(const std::string str, const T&... args)
	{
		if (m_logFile.is_open())
		{
			put(getTimeString(), "[DEBUG]:", str, args...);
			endLine();
		}
	}

	template<typename ...T>
	void info(const std::string str, const T&... args)
	{
		if (m_logFile.is_open())
		{
			put(getTimeString(), "[INFO]:", str, args...);
			endLine();
		}
	}

	template<typename ...T>
	void warn(const std::string str, const T&... args)
	{
		if (m_logFile.is_open())
		{
			put(getTimeString(), "[WARN]:", str, args...);
			endLine();
		}
	}

	template<typename ...T>
	void error(const std::string str, const T&... args)
	{
		if (m_logFile.is_open())
		{
			put(getTimeString(), "[ERROR]:", str, args...);
			endLine();
		}
	}

private:

	inline const std::string getTimeString()
	{
		std::time_t now;
		std::time(&now);
		std::tm when;
		localtime_s(&when, &now);
		char timeFmt[256];
		std::strftime(timeFmt, 256, "[%d/%m/%y %H:%M:%S]", &when);
		return std::string(timeFmt);
	}

	template<typename T, typename ...U>
	void put(const T& t, const U&... args)
	{
		m_logFile << t << " ";
		if (m_defaultOutput)
			std::cout << t << " ";

		if (sizeof...(args) > 0)
			put(args...);
	}

	template<typename T>
	void put(const T& t)
	{
		m_logFile << t << " ";
		if (m_defaultOutput)
			std::cout << t << " ";
	}

	void endLine()
	{
		m_logFile << std::endl;
		if (m_defaultOutput)
			std::cout << std::endl;
	}

private:
	std::ofstream m_logFile;
	bool m_defaultOutput;
	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;
};

#endif // !__LOG_H__

