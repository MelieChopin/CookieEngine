#ifndef __DEBUG_HPP__
#define __DEBUG_HPP__

namespace Cookie
{
	namespace Core
	{
		class Debug
		{
		public:
			static void Log(const char* text)
			{
				std::cout << "LOG: " << text << std::endl;
			}
			static void Error(const char* text)
			{
				std::cout << "INFO: \terror : " << text << std::endl;
			}
			static void Warning(const char* text)
			{
				std::cout << "INFO: \twarning : " << text << std::endl;
			}
			static void Exception(const char* text)
			{
				std::cout << "INFO: \texception : " << text << std::endl;
			}

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
