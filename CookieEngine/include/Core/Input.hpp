#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include <Map>
#include <string>
#include <functional>
#include <windows.h>
#include <iostream>

namespace Cookie
{
	namespace Core
	{
		class Input
		{
		private:
			std::map<int, std::function<void()> > inputs;

		public:
			Input() {}
			~Input() {}

			void Set(int KeyName, std::function<void()> funcToCall) { inputs[KeyName] = funcToCall; }
			void Set(std::map<int, std::function<void()> >& other)
			{
				for (std::map< int, std::function<void()> >::iterator it = other.begin(); it != other.end(); ++it)
					Set(it->first, it->second);
			}

			void CheckInputs()
			{
				for (std::map< int, std::function<void()> >::iterator it = inputs.begin(); it != inputs.end(); ++it)
					if (GetAsyncKeyState(it->first) & 0xff)
						it->second();
			}

		};

	}
}

#endif