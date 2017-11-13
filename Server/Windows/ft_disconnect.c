#include "include/ft_disconnect.h"
#include "include/client.h"

int threadStartDisconnect(void)
{
	if (pthread_create(&DisconnectThread, NULL, DisconnectThreading, playerCount) == -1) {
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
		//printf("playerCount: %d\n", playerCount);
		if (playerCount > 0)
		{
			for (int i = 0; i < playerCount; i++)
			{
				time_t actualTime = time(NULL);
				/*printf("actual time: %d\n", actualTime);
				printf("Client %d last update: %d\n",i, clients[i].lastUpdate);*/
				if (actualTime - clients[i].lastUpdate > 2)
				{
					remove_client(&clients, i, &playerCount);
					printf("%s has been disconnected for timeout\n", Players[i].name);
				}
			}
		}
	}
		
}