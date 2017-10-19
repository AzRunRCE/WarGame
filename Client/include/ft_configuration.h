#ifndef FT_CONFIGURATION_H
#define FT_CONFIGURATION_H
#include "main.h"
#define MAX_LENGTH 32
typedef struct
{
	char* version;
	char* nickname;
	char* server;
} configuration;

static int handler(void* user, const char* section, const char* name,
	const char* value);
configuration *ft_loadConf();
bool ft_saveConf(configuration *settings);
#endif