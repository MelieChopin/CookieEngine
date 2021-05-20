#include "ResourcesManager.hpp"
#include "SoundManager.hpp"
#include "Serialization.hpp"
#include <iostream>
#include <filesystem>


namespace fs = std::filesystem;

using namespace Cookie::Resources;

FMOD::System* SoundManager::system;
std::unordered_map<std::string, std::shared_ptr<Cookie::Resources::Sound>>* SoundManager::sounds;

void SoundManager::InitSystem()
{
	if (FMOD::System_Create(&system) != FMOD_OK)
	{
		std::cout << "ERROR!";
		return;
	}

	if (system->init(512, FMOD_INIT_NORMAL, 0) != FMOD_OK)
	{
		std::cout << "ERROR!";
		return;
	}
}

void SoundManager::Release()
{
	system->close();
	system->release();
}

void SoundManager::LoadAllMusic(ResourcesManager& resourcesManager)
{
	std::vector<std::string> filesPath;
	std::vector<std::string> MAsset;
	for (const fs::directory_entry& path : fs::directory_iterator("Assets/Music"))
	{
		if (path.path().string().find(".mp3") != std::string::npos)
			filesPath.push_back(path.path().string());
		if (path.path().string().find(".MAsset") != std::string::npos)
			MAsset.push_back(path.path().string());
	}

	for (unsigned int i = 0; i < filesPath.size(); i++)
	{
		std::string& iFile = filesPath.at(i);
		std::replace(iFile.begin(), iFile.end(), '\\', '/');
	}

	for (unsigned int i = 0; i < MAsset.size(); i++)
	{
		std::string& iFile = MAsset.at(i);
		std::replace(iFile.begin(), iFile.end(), '\\', '/');
	}

	FMOD_RESULT result;
	FMOD_MODE mode = FMOD_DEFAULT | FMOD_CREATECOMPRESSEDSAMPLE;

	for (int i = 0; i < filesPath.size(); i++)
	{
		std::cout << filesPath[i] << "\n";

		std::size_t pos = filesPath[i].find("c/");
		std::string name = filesPath[i].substr(pos + 2);

		Cookie::Resources::Sound newSound;
		newSound.filepath = filesPath[i];
		FMOD::Channel* chan = nullptr;
		newSound.chan = chan;
		newSound.mode = mode;

		resourcesManager.sounds[name] = std::make_shared<Cookie::Resources::Sound>(newSound);
	}

	sounds = &resourcesManager.sounds;

	for (int i = 0; i < MAsset.size(); i++)
	{
		std::size_t pos = MAsset[i].find("c/");
		std::string name = MAsset[i].substr(pos + 2);
		pos = name.find(".MAsset");
		std::size_t end = name.length();
		name.replace(pos, end, ".mp3");
		Cookie::Resources::Serialization::Load::LoadVolumAndModeMusic(MAsset[i], name);
	}
}

void SoundManager::PlayMusic(std::string key)
{
	if ((*(sounds))[key].get()->sound == nullptr)
		FMOD_RESULT result = system->createSound((*(sounds))[key].get()->filepath.c_str(), 
									(*(sounds))[key].get()->mode, nullptr, &(*(sounds))[key].get()->sound);
	
	system->playSound((*(sounds))[key].get()->sound, nullptr, false, &(*(sounds))[key].get()->chan);
	(*(sounds))[key].get()->chan->setVolume((*(sounds))[key].get()->vol);
	if ((*(sounds))[key].get()->mode & FMOD_3D)
	{
		Cookie::Core::Math::Vec3 posSound = (*(sounds))[key].get()->pos;
		FMOD_VECTOR pos = { posSound.x, posSound.y, posSound.z};
		(*(sounds))[key].get()->chan->set3DAttributes(&pos, nullptr);
	}
}

void SoundManager::Play3DMusic(std::string key, const Cookie::Core::Math::Vec3& pos)
{
	if ((*(sounds))[key].get()->sound == nullptr)
		FMOD_RESULT result = system->createSound((*(sounds))[key].get()->filepath.c_str(),
			(*(sounds))[key].get()->mode, nullptr, &(*(sounds))[key].get()->sound);

	system->playSound((*(sounds))[key].get()->sound, nullptr, false, &(*(sounds))[key].get()->chan);
	(*(sounds))[key].get()->chan->setVolume((*(sounds))[key].get()->vol);
	if ((*(sounds))[key].get()->mode & FMOD_3D)
	{
		FMOD_VECTOR posSound = { pos.x, pos.y, pos.z };
		(*(sounds))[key].get()->chan->set3DAttributes(&posSound, nullptr);
	}
}

void SoundManager::SetVolume(std::string key, float vol)
{
	(*(sounds))[key].get()->vol = vol;
}

void SoundManager::SetPaused(std::string key, bool isPaused)
{
	(*(sounds))[key].get()->chan->setPaused(isPaused);
}

void SoundManager::Loop(std::string key)
{
	(*(sounds))[key].get()->mode |= FMOD_LOOP_NORMAL;
}

void SoundManager::Normal(std::string key)
{
	(*(sounds))[key].get()->mode -= FMOD_LOOP_NORMAL;
}

void SoundManager::Set3D(std::string key, const Cookie::Core::Math::Vec3& pos)
{
	(*(sounds))[key].get()->mode |= FMOD_3D;
	(*(sounds))[key].get()->pos = pos;
}

void SoundManager::SetPosition(std::string key, const Cookie::Core::Math::Vec3& pos)
{
	(*(sounds))[key].get()->pos = pos;
}

void SoundManager::Set2D(std::string key)
{
	(*(sounds))[key].get()->mode -= FMOD_2D;
}

void SoundManager::SetMode(std::string key, FMOD_MODE mode)
{
	(*(sounds))[key].get()->mode = mode;
}