/****************************************************************************/
/*!
\file Sound.cpp
\author Teo Jia Hao
\par email: kelvinteojiahao\@hotmail.com
\brief
Class to handle music and sound effect for the game
*/
/****************************************************************************/
#include "Sound.h"

Sound::Sound()
{
	musicEngine = irrklang::createIrrKlangDevice();
}
Sound::~Sound()
{

}
/******************************************************************************/
/*!
\brief
Plays the background music looped

\param Music
Takes in path of background music to play

\exception None
\return None
*/
/******************************************************************************/
void Sound::playMusic(string Music)
{
	Song = musicEngine->play2D(Music.c_str(), true, false, true);
}
/******************************************************************************/
/*!
\brief
Plays the sound effect without loop

\param Music
Takes in path of sound effect to play

\exception None
\return None
*/
/******************************************************************************/
void Sound::playSoundEffect2D(string Music)
{
	SoundEffect2D = musicEngine->play2D(Music.c_str(), false, false, false);
}
/******************************************************************************/
/*!
\brief
Stop the music when called

\param Music
Takes in path of background music/sound effect to stop

\exception None
\return None
*/
/******************************************************************************/
void Sound::stopMusic(string Music)
{
	Song->stop();
}