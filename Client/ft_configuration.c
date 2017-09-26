#include "include\ft_configuration.h"
#include <stdio.h>
#include <stdlib.h>

const char path[] = "settings.ini";

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
	if (ini_parse(path, handler, settings) < 0) {
		printf("Can't load '%s'\n", path);
		settings = NULL;
	}
	printf("Version: %d\nNickname: %s\nServer: %s\n", settings->version, settings->nickname, settings->server);

	return settings;
}

bool ft_saveConf(configuration *settings)
{

	FILE * fp;
	fp = fopen(path, "w+");
	fprintf(fp, "[game]\nversion=%i\nserver=%s\n[player]\nnickname=%s\n", settings->version, settings->server, settings->nickname);

	fclose(fp);
	printf("Settings saved in '%s'\n", path);
	return true;
}