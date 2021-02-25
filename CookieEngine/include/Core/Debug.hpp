#ifndef __DEBUG_HPP__
#define __DEBUG_HPP__

#include <vector>
#include <string>
#include <iostream>

namespace Cookie
{
	namespace Core
	{
		struct DebugMessage
		{
			const char*	text;
				
			enum Type
			{ 
				// Basic information. Will slowly flash blue at arrival.
				Log, 
					
				// Indicate a warning. Will flash yellow once.
				Warning,

				// Signal an error. Will quickly flash red thrice.
				Error,

				// An exception will make the whole console flash red. Use in really bad cases.
				Exception 
				
			} messageType;
				
			uint8_t			colorVariant;
			unsigned short	colorBounces;
			bool			bouncing = false;
		};

		// [Singleton class closely linked with the console widget. Can be used anywhere after being initialized.]
		class Debug
		{
		public:
			std::vector<DebugMessage> storedMessages;


		private:
			Debug() = default;
		
		public:
			// Summons the Debug manager.
			inline static Debug& Summon()
			{ static Debug debugSingleton; return debugSingleton; }


			inline void Log(const char* text)
			{ storedMessages.push_back(DebugMessage{ text, DebugMessage::Log, 255, 0 }); }

			inline void Log(const std::string& strext)
			{ storedMessages.push_back(DebugMessage{ strext.c_str(), DebugMessage::Log, 255, 0 }); }
			
			
			inline void Warning(const char* text)
			{ storedMessages.push_back(DebugMessage{ text, DebugMessage::Warning, 0, 0, true }); }

			inline void Warning(const std::string& strext)
			{ storedMessages.push_back(DebugMessage{ strext.c_str(), DebugMessage::Warning, 0, 0, true }); }
			

			inline void Error(const char* text)
			{ storedMessages.push_back(DebugMessage{ text, DebugMessage::Error, 255, 3 }); }

			inline void Error(const std::string& strext)
			{ storedMessages.push_back(DebugMessage{ strext.c_str(), DebugMessage::Error, 255, 3 }); }
			

			inline void Exception(const char* text)
			{ storedMessages.push_back(DebugMessage{ text, DebugMessage::Exception, 255, 10 }); }

			inline void Exception(const std::string& strext)
			{ storedMessages.push_back(DebugMessage{ strext.c_str(), DebugMessage::Exception, 255, 10 }); }


			static void Assertion(int line, const char* file, bool element)
			{
				if (!element)
				{
					std::cout << "\nINFO : \t" << "Problem insertion at " << line << " in " << file << "\n\n";
					exit(1);
				}
			}
		};
	}
}

#endif // !__DEBUG__
