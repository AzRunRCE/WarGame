#include "include\ft_configuration.h"
#include <stdio.h>
#include <stdlib.h>
static int handler(void* user, const char* section, const char* name,
	const char* value)
{
	configuration* pconfig = (configuration*)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("game", "version")) {
		pconfig->version = atoi(value);
	}
	else if (MATCH("game", "server")) {
		pconfig->server = _strdup(value);
	}
	else if (MATCH("player", "nickname")) {
		pconfig->nickname = _strdup(value);
	}
	else {
		return 0;  /* unknown section/name, error */
	}
	return 1;
}
configuration *ft_loadConf()
{
	configuration *settings = malloc(sizeof(configuration));
	char path[] = "settings.ini";
	if (ini_parse(path, handler, settings) < 0) {
		printf("Can't load '%s'\n", path);
		settings = NULL;
	}
	return settings;
}