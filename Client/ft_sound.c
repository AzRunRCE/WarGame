#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "include/ft_sound.h"

FMOD_SYSTEM *soundSystem;
FMOD_SOUND *sound_Music;
FMOD_SOUND *sound_Fire;
FMOD_SOUND *sound_Grunt;


void sound_Init(bool musicEnable)
{
	startPosition_time = endPosition_time = -1;
	if (FMOD_System_Create(&soundSystem))
		exit(EXIT_FAILURE);
	if (FMOD_System_Init(soundSystem, 3, FMOD_INIT_NORMAL, NULL))
		exit(EXIT_FAILURE);
	if (musicEnable)
	{
		if (FMOD_System_CreateSound(soundSystem, "res/music.mp3", FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, 0, &sound_Music))
			exit(EXIT_FAILURE);
		FMOD_Sound_SetLoopCount(sound_Music, -1);
		FMOD_System_PlaySound(soundSystem, sound_Music, NULL, 0, &soundChannelMusic);
	}
	sound_Load("res/fire.wav", &sound_Fire);
	sound_Load("res/grunt.mp3", &sound_Grunt);
}

void sound_Load(char file[MAX_FILENAME], FMOD_SOUND **sound)
{
	if (FMOD_System_CreateSound(soundSystem, file, FMOD_CREATESAMPLE, 0, sound))
		exit(EXIT_FAILURE);	
}

void sound_Grunt_Poll(void)
{
	if (startPosition_time != -1)
	{
		uint64_t current = 0;
		FMOD_RESULT result = 0;
		result = FMOD_Channel_GetPosition(soundChannelGrunt, &current, FMOD_TIMEUNIT_MS);
		if (result)
			exit(result);
		if (current > endPosition_time)
		{
			if (FMOD_Sound_Release(sound_Grunt))
				exit(EXIT_FAILURE);
			sound_Load("res/grunt.mp3", &sound_Grunt);
			startPosition_time = -1;
		}
	}
}

void sound_Play_Fire(FMOD_CHANNEL **channel)
{
	FMOD_System_PlaySound(soundSystem, sound_Fire, NULL, 0, channel);
}

void sound_Play_Grunt(FMOD_CHANNEL **channel, uint8_t startPosition)
{
	if (startPosition_time == -1) {
		uint64_t start = 1000 * startPosition, end = 1000 * (startPosition + 1);
		FMOD_System_PlaySound(soundSystem, sound_Grunt, NULL, 0, channel);
		FMOD_Channel_SetPosition(*channel, start, FMOD_TIMEUNIT_MS);
		startPosition_time = start;
		endPosition_time = end;
	}

}

void sound_Close(void)
{
	if (FMOD_Sound_Release(sound_Fire))
		exit(EXIT_FAILURE);
	if (FMOD_Sound_Release(sound_Grunt))
		exit(EXIT_FAILURE);
	if (FMOD_Sound_Release(sound_Music))
		exit(EXIT_FAILURE);
	if (FMOD_System_Close(soundSystem))
		exit(EXIT_FAILURE);
	if (FMOD_System_Release(soundSystem))
		exit(EXIT_FAILURE);
}