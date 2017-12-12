#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/ft_configuration.h"

const char path[] = "settings.ini";

static int handler(void* user, const char* section, const char* name,
	const char* value)
{
	configuration* pconfig = (configuration*)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("game", "version"))
		strncpy(pconfig->version, value, strlen(value) + 1);
	else if (MATCH("game", "server"))
		strncpy(pconfig->server, value, strlen(value) + 1);
	else if (MATCH("game", "sound"))
		pconfig->sound = atoi(value);
	else if (MATCH("game", "music"))
		pconfig->music = atoi(value);
	else if (MATCH("player", "nickname"))
		strncpy(pconfig->nickname, value, strlen(value) + 1);
	else
		return false;  /* unknown section/name, error */
	return true;
}
configuration *ft_loadConf()
{
	configuration *settings = malloc(sizeof(configuration));
	if (ini_parse(path, handler, settings) < 0) {
		printf("Can't load '%s'\n", path);
		settings = NULL;
	}
	printf("Version: %s\nNickname: %s\nServer: %s\nSound: %d\nMusic: %d\n", settings->version, settings->nickname, settings->server, settings->sound, settings->music);

	return settings;
}

bool ft_saveConf(configuration *settings)
{

	FILE * fp;
	fp = fopen(path, "w+");
	fprintf(fp, "[game]\nversion=%s\nserver=%s\nmusic=%d\nsound=%d\n[player]\nnickname=%s\n", settings->version, settings->server, settings->music, settings->sound, settings->nickname);

	fclose(fp);
	printf("Settings saved in '%s'\n", path);
	return true;
}
