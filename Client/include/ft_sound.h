#ifndef FT_SOUND_H
#define FT_SOUND_H
#include <fmod.h>
#define true 1
#define false 0
FMOD_CHANNEL **soundChannelMusic;
FMOD_CHANNEL **soundChannelMainPlayer;
FMOD_CHANNEL **soundChannelEnemies;


void sound_Init();
void sound_Load(char file[30]);
void sound_Play(FMOD_CHANNEL **channel);
void sound_Close();

#endif // FT_SOUND_H
