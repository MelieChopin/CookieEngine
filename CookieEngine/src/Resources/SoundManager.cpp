#include "SoundManager.hpp"
#include <filesystem>
#include <vector>
#include <iostream>


using namespace Cookie::Resources;

SoundManager::SoundManager()
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

   /* std::vector<std::string> filesPath;
    FMOD_RESULT result;
    FMOD_MODE mode = FMOD_DEFAULT;
    for (const fs::directory_entry& path : fs::directory_iterator("Assets/Music"))
    {
        filesPath.push_back(path.path().string());
        FMOD::Sound sound;
        result = system->createSound("Assets/Music/Dancin.mp3", mode, nullptr, &sound);
        //if ()
    }*/
       
    FMOD_RESULT result;
    FMOD_MODE mode = FMOD_DEFAULT;
    mode |= FMOD_LOOP_NORMAL;
    result = system->createSound("Assets/Music/Editor.mp3", mode, nullptr, &sound);

}

SoundManager::~SoundManager()
{
	system->release();
}
