/****************************************************************************/
/*!
\file Sound.h
\author Teo Jia Hao
\par email: kelvinteojiahao\@hotmail.com
\brief
Class to handle music and sound effect for the game
*/
/****************************************************************************/

#ifndef SOUND_H
#define SOUND_H

#include "irrKlang.h"
#include <string>
using std::string;

#pragma comment(lib, "irrKlang.lib")

/******************************************************************************/
/*!
Class Sound:
\brief	Creates sound engine and allows music to be played
*/
/******************************************************************************/
class Sound
{
public:

	Sound();
	~Sound();
	void playMusic(string Music);
	void stopMusic(string Music);
	void playSoundEffect2D(string Music);

private:
	irrklang::ISoundEngine* musicEngine;
	irrklang::ISound* Song;
	irrklang::ISound* SoundEffect2D;
};
#endif