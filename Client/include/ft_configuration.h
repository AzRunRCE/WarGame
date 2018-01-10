#ifndef FT_CONFIGURATION_H
#define FT_CONFIGURATION_H
#define MAX_LENGTH 32
#include <stdbool.h>
#include <SDL_config.h>
#include "ini.h"

typedef struct _configuration
{
	char version[MAX_LENGTH];
	char nickname[MAX_LENGTH];
	char server[MAX_LENGTH];
	bool sound;
	bool music;
	uint8_t soundLevel;
	uint8_t musicLevel;
	uint16_t height;
	uint16_t width;

} configuration;

static int handler(void* user, const char* section, const char* name,
	const char* value);
configuration *ft_loadConf();

bool ft_saveConf(configuration *settings);
#endif
