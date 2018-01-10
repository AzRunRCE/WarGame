#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "include/ft_configuration.h"
#include "include/ft_engine.h"

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
	else if (MATCH("sound", "sound"))
		pconfig->sound = atoi(value);
	else if (MATCH("sound", "music"))
		pconfig->music = atoi(value);
	else if (MATCH("sound", "soundLevel"))
		pconfig->soundLevel = atoi(value);
	else if (MATCH("sound", "musicLevel"))
		pconfig->musicLevel = atoi(value);
	else if (MATCH("player", "nickname"))
		strncpy(pconfig->nickname, value, strlen(value) + 1);
	else if (MATCH("video", "resolution"))
	{
		pconfig->width = atoi(value);
		pconfig->height = pconfig->width * (3.0 / 4.0);
	}
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
	printf("Version: %s\nNickname: %s\nServer: %s\nSound: %d\nMusic: %d\nSoundLevel: %d\nMusicLevel: %d\nResolution w: %d\nResolution h: %d\n", settings->version, settings->nickname, settings->server, settings->sound, settings->music, settings->soundLevel, settings->musicLevel, settings->width, settings->height);

	return settings;
}

bool ft_saveConf(configuration *settings)
{
	if (settings->width != _engine.WIDTH)
	{
		_engine.WIDTH = settings->width;
		_engine.HEIGHT = settings->height;
	}
	FILE * fp;
	fp = fopen(path, "w+");
	fprintf(fp, "[game]\nversion=%s\nserver=%s\n[player]\nnickname=%s\n[sound]\nmusic=%d\nsound=%d\nsoundLevel=%d\nmusicLevel=%d\n[video]\nresolution=%d\n", settings->version, settings->server, settings->nickname, settings->music, settings->sound, settings->soundLevel, settings->musicLevel, settings->width);
	fclose(fp);
	printf("Settings saved in '%s'\n", path);
	return true;
}
