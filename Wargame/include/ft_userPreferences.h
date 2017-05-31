#ifndef FT_WARGAMEINIT_H
#define FT_WARGAMEINIT_H
typedef struct
{
	int version;
	char* nickname;
	char* server;
} configuration;
configuration *CONF_Load(const char* path);
#endif // FT_WARGAMEINIT_H
