#include <stdio.h>
#include "include/ft_view.h"
#include "include/ft_engine.h"
#include <stdlib.h>
#include <math.h>

void ft_ViewGetDegrees(double a, double b)
{
	if (a != 0 && b != 0)
	{
		double tangente;
		if (b > 0)
		{
			tangente = a / b;
			_engine.viewDegrees = atan(tangente); // En radians
		}
		else if (a < 0)
		{
			tangente = b / a;
			tangente = -tangente;
			_engine.viewDegrees = atan(tangente) - M_PI / 2; // En radians
		}
		else
		{
			tangente = b / a;
			tangente = -tangente;
			_engine.viewDegrees = atan(tangente) + M_PI / 2; // En radians
		}
		_engine.viewRect.x = _engine.mainPlayer.RelativePos.x + 16 - _engine.viewRect.w / 2;
		_engine.viewRect.y = _engine.mainPlayer.RelativePos.y + 16 - _engine.viewRect.h / 2;
		_engine.viewDegrees = _engine.viewDegrees * 180 / M_PI; // Conversion en degrés
	}
}
