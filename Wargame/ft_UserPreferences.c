
#include "include/ini.h"

#include "include\ft_userPreferences.h"

static int handler(void* user, const char* section, const char* name,
	const char* value)
{
	configuration* pconfig = (configuration*)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("game", "version")) {
		pconfig->version = atoi(value);
	}
	else if (MATCH("game", "server")) {
		pconfig->server = strdup(value);
	}
	else if (MATCH("player", "nickname")) {
		pconfig->nickname = strdup(value);
	}
	else {
		return 0;  /* unknown section/name, error */
	}
	return 1;
}

configuration *CONF_Load(const char* path)
{
	configuration config;

	if (ini_parse(path, handler, &config) < 0) {
		printf("Can't load '%s'\n",path);
		return NULL;
	}
	//printf("Config loaded from '%s': version=%d, name=%s",
	//	path,config.version, config.name);
	return &config;
}