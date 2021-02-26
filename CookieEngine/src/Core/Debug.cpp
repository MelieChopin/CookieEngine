#include "Debug.hpp"

#include <iostream>


using namespace Cookie::Core;


void DebugMessageHandler::Log(const char* text)
{
	storedMessages.push_back(DebugMessage{ text, DebugMessage::Log });
}

void DebugMessageHandler::Log(const std::string& strext)
{
	storedMessages.push_back(DebugMessage{ strext.c_str(), DebugMessage::Log });
}


void DebugMessageHandler::Warning(const char* text)
{
	storedMessages.push_back(DebugMessage{ text, DebugMessage::Warning, 1 });
}

void DebugMessageHandler::Warning(const std::string& strext)
{
	storedMessages.push_back(DebugMessage{ strext.c_str(), DebugMessage::Warning, 1 });
}


void DebugMessageHandler::Error(const char* text)
{
	storedMessages.push_back(DebugMessage{ text, DebugMessage::Error, 2 });
}

void DebugMessageHandler::Error(const std::string& strext)
{
	storedMessages.push_back(DebugMessage{ strext.c_str(), DebugMessage::Error, 2 });
}


void DebugMessageHandler::Exception(const char* text)
{
	storedMessages.push_back(DebugMessage{ text, DebugMessage::Exception, 7 });
}

void DebugMessageHandler::Exception(const std::string& strext)
{
	storedMessages.push_back(DebugMessage{ strext.c_str(), DebugMessage::Exception, 7 });
}


void DebugMessageHandler::Assertion(int line, const char* file, bool element)
{
	if (!element)
	{
		std::cout << "\nINFO : \t" << "Problem insertion at " << line << " in " << file << "\n\n";
		exit(1);
	}
}