#ifndef FT_CONFIGURATION_H
#define FT_CONFIGURATION_H
#include "main.h"
#define MAX_LENGTH 32
typedef struct
{
	char version[MAX_LENGTH];
	char nickname[MAX_LENGTH];
	char server[MAX_LENGTH];
} configuration;

static int handler(void* user, const char* section, const char* name,
	const char* value);
configuration *ft_loadConf();
bool ft_saveConf(configuration *settings);
#endif