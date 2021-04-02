#ifndef __SOUNDMANAGER_HPP__
#define __SOUNDMANAGER_HPP__

#include <fmod.hpp>

namespace Cookie
{
	namespace Resources
	{
		class SoundManager
		{
		public:
			FMOD::System* system = nullptr;
		
			FMOD::Sound* sound;

			SoundManager();
			~SoundManager();
		};
	}
}

#endif