#include "include/ft_delay.h"

bool ft_delay(uint32_t *lastAnim, uint32_t SleepTimeAnim)
{
	int ActualTimeAnim = SDL_GetTicks();
	if (ActualTimeAnim - *lastAnim > SleepTimeAnim)
	{
		*lastAnim = ActualTimeAnim;
		return true;
	}
	else
	{
		return false;
	}

}