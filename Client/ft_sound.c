#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/ft_sound.h"

FMOD_SYSTEM *soundSystem;
FMOD_SOUND *sound;
FMOD_SOUND *music;

void sound_Init(bool musicEnable)
{
	if (FMOD_System_Create(&soundSystem))
		exit(EXIT_FAILURE);
	if (FMOD_System_Init(soundSystem, 3, FMOD_INIT_NORMAL, NULL))
		exit(EXIT_FAILURE);
	if (musicEnable)
	{
		FMOD_System_CreateSound(soundSystem, "res/music.mp3", FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &music);
		FMOD_Sound_SetLoopCount(music, -1);
		FMOD_System_PlaySound(soundSystem, music, NULL, 0, soundChannelMusic);
	}
	sound_Load("res/fire.wav");
}

void sound_Load(char file[30])
{
	if (FMOD_System_CreateSound(soundSystem, file, FMOD_CREATESAMPLE, 0, &sound))
		exit(EXIT_FAILURE);
}
void sound_Play(FMOD_CHANNEL **channel)
{
	FMOD_System_PlaySound(soundSystem, sound, NULL, 0, channel);
}

void sound_Close(void)
{
	if (FMOD_Sound_Release(sound))
		exit(EXIT_FAILURE);
	if (FMOD_Sound_Release(music))
		exit(EXIT_FAILURE);
	if (FMOD_System_Close(soundSystem))
		exit(EXIT_FAILURE);
	if (FMOD_System_Release(soundSystem))
		exit(EXIT_FAILURE);
}