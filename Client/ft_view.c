#include <stdio.h>
#include "include/ft_View.h"
#include "include/ft_engine.h"
#include <stdlib.h>
#include <math.h>

void ft_ViewGetDegrees(double a, double b)
{
	a = _engine.mousePos.y - _engine.pCenter.y;
	b = _engine.mousePos.x - _engine.pCenter.x;
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
		_engine.viewRect.x = ((_engine.pCenter.x + 16) * 2 - _engine.viewRect.w) / 2;
		_engine.viewRect.y = ((_engine.pCenter.y + 16) * 2 - _engine.viewRect.h) / 2;
		_engine.viewDegrees = _engine.viewDegrees * 180 / M_PI; // Conversion en degrés
	}
}
