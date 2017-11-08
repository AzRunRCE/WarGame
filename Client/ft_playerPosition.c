#include "include/ft_playerPosition.h"

void checkPlayerPosition(void)
{
	if (_engine.viewDegrees >= -22.5 && _engine.viewDegrees < 22.5)
		_engine.mainPlayer.playerBase.orientation = RIGHT;
	else if (_engine.viewDegrees >= 22.5 && _engine.viewDegrees < 67.5)
		_engine.mainPlayer.playerBase.orientation = DOWN_RIGHT;
	else if (_engine.viewDegrees >= 67.5 && _engine.viewDegrees < 112.5)
		_engine.mainPlayer.playerBase.orientation = DOWN;
	else if (_engine.viewDegrees >= 112.5 && _engine.viewDegrees < 157.5)
		_engine.mainPlayer.playerBase.orientation = DOWN_LEFT;
	else if (_engine.viewDegrees >= 157.5 || _engine.viewDegrees < -157.5)
		_engine.mainPlayer.playerBase.orientation = LEFT;
	else if (_engine.viewDegrees >= -157.5 && _engine.viewDegrees < -112.5)
		_engine.mainPlayer.playerBase.orientation = UP_LEFT;
	else if (_engine.viewDegrees >= -112.5 && _engine.viewDegrees < -67.5)
		_engine.mainPlayer.playerBase.orientation = UP;
	else if (_engine.viewDegrees >= -67.5 && _engine.viewDegrees < -22.5)
		_engine.mainPlayer.playerBase.orientation = UP_RIGHT;
}