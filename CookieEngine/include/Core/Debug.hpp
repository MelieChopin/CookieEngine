#ifndef __DEBUG_HPP__
#define __DEBUG_HPP__

#include <vector>
#include <string>

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
				
			unsigned short	colorBounces	= 0;
			uint8_t			colorVariant	= 255;
			bool			bouncing		= false;
		};

		// [Singleton class closely linked with the console widget. Can be used anywhere after being initialized.]
		class DebugMessageHandler
		{
		public:
			std::vector<DebugMessage> storedMessages;


		private:
			DebugMessageHandler()						= default;
			DebugMessageHandler(DebugMessageHandler&&)	= delete;
			DebugMessageHandler(DebugMessageHandler& )	= delete;
		
		public:
			// Summons the Debug manager.
			inline static DebugMessageHandler& Summon()
			{ static DebugMessageHandler debugSingleton; return debugSingleton; }

			void Log(const char* text);
			void Log(const std::string& strext);
			
			void Warning(const char* text);
			void Warning(const std::string& strext);

			void Error(const char* text);
			void Error(const std::string& strext);

			void Exception(const char* text);
			void Exception(const std::string& strext);


			static void Assertion(int line, const char* file, bool element);
		};
	}
}


#define CDebug Cookie::Core::DebugMessageHandler::Summon()

#endif // !__DEBUG__
