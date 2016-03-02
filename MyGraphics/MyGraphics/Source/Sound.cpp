#include "Sound.h"

Sound::Sound()
{
	musicEngine = irrklang::createIrrKlangDevice();
	SoundEffect3D = irrklang::createIrrKlangDevice();
}
Sound::~Sound()
{

}
void Sound::Init()
{
	SoundEffect3D->setDefault3DSoundMinDistance(10.0f);
	SoundEffect3D->setDefault3DSoundMaxDistance(5000.0f);
	SoundEffect3D->setSoundVolume(5.0f);
}
void Sound::playMusic(string Music)
{
	Song = musicEngine->play2D(Music.c_str(), true, false, true);
}
void Sound::playSoundEffect2D(string Music)
{
	SoundEffect2D = musicEngine->play2D(Music.c_str(), false, false, false);
}
void Sound::playSoundEffect3D(string Music, irrklang::vec3df pos, irrklang::vec3df TargetPos)
{
	SoundEffect3D->play3D(Music.c_str(), pos);
	SoundEffect3D->setListenerPosition(pos, TargetPos);
}
void Sound::stopMusic(string Music)
{
	Song->stop();
}