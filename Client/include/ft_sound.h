#ifndef FT_SOUND_H
#define FT_SOUND_H
#include <fmod.h>
#include <SDL_config.h>
#define MAX_SOUND 6
#define MAX_FILENAME 30


FMOD_CHANNEL *soundChannelMusic;
FMOD_CHANNEL *soundChannelMainPlayer;
FMOD_CHANNEL *soundChannelEnemies;
FMOD_CHANNEL *soundChannelGrunt;
static int64_t startPosition_time;
static int64_t endPosition_time;


void sound_Init(bool musicEnable);
void sound_Load(char file[MAX_FILENAME], FMOD_SOUND **sound);
void sound_Play_Fire(FMOD_CHANNEL **channel);
void sound_Play_Grunt(FMOD_CHANNEL **channel, uint8_t startPosition);
void sound_Grunt_Poll(void);
void sound_Close(void);

#endif // FT_SOUND_H
