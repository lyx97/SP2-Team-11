#ifndef SOUND_H
#define SOUND_H

#include "irrKlang.h"
#include <string>
using std::string;

#pragma comment(lib, "irrKlang.lib")

class Sound
{
public:

	Sound();
	~Sound();
	void Init();
	void playMusic(string Music);
	void stopMusic(string Music);
	void playSoundEffect2D(string Music);
	void playSoundEffect3D(string Music, irrklang::vec3df pos, irrklang::vec3df TargetPos);

private:
	irrklang::ISoundEngine* musicEngine;
	irrklang::ISound* Song;
	irrklang::ISound* SoundEffect2D;
	irrklang::ISoundEngine* SoundEffect3D;
};
#endif