#ifndef FT_CONFIGURATION_H
#define FT_CONFIGURATION_H
typedef struct
{
	int version;
	char* nickname;
	char* server;
} configuration;

static int handler(void* user, const char* section, const char* name,
	const char* value);
configuration *ft_loadConf();
#endif