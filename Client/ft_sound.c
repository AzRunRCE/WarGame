#include <stdio.h>
#include <stdlib.h>
#include "include/ft_sound.h"

FMOD_SYSTEM *soundSystem;
FMOD_SOUND *sound;
FMOD_SOUND *music;

void sound_Init()
{
	soundChannelMusic = malloc(sizeof(int));
	soundChannelMainPlayer = malloc(sizeof(int));
	soundChannelEnemies = malloc(sizeof(int));
	if (FMOD_System_Create(&soundSystem))
		exit(EXIT_FAILURE);
	if (FMOD_System_Init(soundSystem, 3, FMOD_LOOP_NORMAL, NULL))
		exit(EXIT_FAILURE);
	FMOD_Sound_SetLoopCount(music, -1);
	FMOD_System_CreateSound(soundSystem, "res/music.mp3", FMOD_2D | FMOD_CREATESTREAM, 0, &music);
	FMOD_System_PlaySound(soundSystem, music, NULL, 0, soundChannelMusic);

}

void sound_Load(char file[30])
{
	if (FMOD_System_CreateSound(soundSystem, file, FMOD_CREATESAMPLE, 0, &sound))
		exit(EXIT_FAILURE);
}
void sound_Play(int *channel)
{
	FMOD_System_PlaySound(soundSystem, sound, NULL, 0, channel);
}

void sound_Close()
{
	free(soundChannelMusic);
	free(soundChannelMainPlayer);
	free(soundChannelEnemies);
	if (FMOD_Sound_Release(sound))
		exit(EXIT_FAILURE);
	if (FMOD_Sound_Release(music))
		exit(EXIT_FAILURE);
	if (FMOD_System_Close(soundSystem))
		exit(EXIT_FAILURE);
	if (FMOD_System_Release(soundSystem))
		exit(EXIT_FAILURE);
}