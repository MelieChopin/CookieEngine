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
			static std::unordered_map<std::string, std::shared_ptr<Cookie::Resources::Sound>>* sounds;

			SoundManager();
			~SoundManager();

			void LoadAllMusic(ResourcesManager& resourcesManager);
			static void PlayMusic(std::string key);
			static void SetVolume(std::string key, float vol);
			static void Loop(std::string key);
			static void Normal(std::string key);
			static void Set3D(std::string key, const Cookie::Core::Math::Vec3& pos);
			static void SetPosition(std::string key, const Cookie::Core::Math::Vec3& pos);
			static void Set2D(std::string key);
		};
	}
}

#endif