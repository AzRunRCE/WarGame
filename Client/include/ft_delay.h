#ifndef FT_DELAY_H
#define FT_DELAY_H
#include <stdbool.h>
#include <SDL.h>

/*
*  \brief Delaying an action.
Returns true if the delay time has elapsed
and false if not
*  \param *lastAnim pointer with actual time (SDL ticks) in it
You may leave 0 or < current SDL_GetTicks() but this will return true the first time
*  \param SleepTimeAnim Time you want to wait in ms
*/
bool ft_delay(uint32_t *lastAnim, uint32_t SleepTimeAnim);

#endif // FT_DELAY_H
