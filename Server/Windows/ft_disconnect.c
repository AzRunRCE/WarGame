#include <stdio.h>
#include "include/ft_disconnect.h"
#include "include/client.h"

int threadStartDisconnect(void)
{
	if (pthread_create(&DisconnectThread, NULL, DisconnectThreading, NULL) == -1) {
		perror("pthread_create");
		return false;
	}
	else
		return true;

}

void *DisconnectThreading(void)
{
	while (true)
	{
		SLEEP100MS;
		if (*playerCount > 0)
		{
			for (uint16_t i = 0; i < *playerCount; i++)
			{
				time_t actualTime = time(NULL);
				if (actualTime - clients[i].lastUpdate > 2)
				{
					remove_client(&clients, i, playerCount);
					printf("%s has been disconnected. Reason: timeout\n", Players[i].name);
				}
			}
		}
	}
		
}