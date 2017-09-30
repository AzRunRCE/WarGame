#ifndef FT_SOUND_H
#define FT_SOUND_H
#include <fmod.h>
#define true 1
#define false 0
int *soundChannelMusic;
int *soundChannelMainPlayer;
int *soundChannelEnemies;


void sound_Init();
void sound_Load(char file[30]);
void sound_Play(int *channel);
void sound_Close();

#endif // FT_SOUND_H
