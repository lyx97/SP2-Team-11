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
	void playMusic(string Music);
	void stopMusic(string Music);
	void playSoundEffect(string Music);

private:
	irrklang::ISoundEngine* musicEngine;
	irrklang::ISound* Song;
	irrklang::ISound* SoundEffect;
};
#endif