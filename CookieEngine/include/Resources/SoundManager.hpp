#ifndef __SOUNDMANAGER_HPP__
#define __SOUNDMANAGER_HPP__

#include <unordered_map>
#include <memory>
#include "Sound.hpp"

namespace Cookie
{
	namespace Resources
	{
		class ResourcesManager;

		class SoundManager
		{
		public:
			static FMOD::System* system;
			static std::unordered_map<std::string, std::unique_ptr<Cookie::Resources::Sound>>* sounds;

			static void InitSystem();
			static void Release();
			static void LoadAllMusic(ResourcesManager& resourcesManager);

			static void PlayMusic	(std::string key);
			static void Play3DMusic	(std::string key, const Cookie::Core::Math::Vec3& pos);
			static void SetVolume	(std::string key, float vol);
			static void SetPaused	(std::string key, bool isPaused);
			static void Loop		(std::string key);
			static void Normal		(std::string key);
			static void Set3D		(std::string key, const Cookie::Core::Math::Vec3& pos);
			static void SetPosition	(std::string key, const Cookie::Core::Math::Vec3& pos);
			static void Set2D		(std::string key);
			static void SetMode		(std::string key, FMOD_MODE mode);


			static void PlayMusic			(Sound* const & sound);


			static inline void SetVolume	(Sound* const & sound, float vol)
			{ sound->vol = vol; }

			static inline void SetPaused	(Sound* const & sound, bool isPaused)
			{ sound->chan->setPaused(isPaused); }

			static inline void Loop			(Sound* const & sound)
			{ sound->mode |= FMOD_LOOP_NORMAL; }

			static inline void Normal		(Sound* const & sound)
			{ sound->mode -= FMOD_LOOP_NORMAL; }

			static inline void Set3D		(Sound* const & sound, const Cookie::Core::Math::Vec3& pos)
			{ 
				sound->mode -= sound->mode & FMOD_2D;
				sound->mode |= FMOD_3D;
				sound->pos = pos;
			}

			static inline void Set3D		(Sound* const & sound)
			{ 
				sound->mode -= sound->mode & FMOD_2D;
				sound->mode |= FMOD_3D;
			}

			static inline void SetPosition	(Sound* const & sound, const Cookie::Core::Math::Vec3& pos)
			{ sound->pos = pos; }

			static inline void Set2D		(Sound* const & sound)
			{ 
				sound->mode -= sound->mode & FMOD_3D;
				sound->mode |= FMOD_2D;
			}

			static inline void SetMode		(Sound* const & sound, FMOD_MODE mode)
			{ sound->mode = mode; }
		};
	}
}

#endif
