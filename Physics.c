#ifndef PHYSICS_C
#define PHYSICS_C
#include "Library.h"
#include "Controller.c"
//FALL
//JUMP
//OVERSTEP

void overStep(GameObject* object, int adm)
{
	int x = 0, y = 0, current_x = object->x,
		speed = 10 * object->vector;
	if (object->vector < 0)
	{
		current_x += object->lx;
		for (; (y != adm) && (x != speed);)
		{
			if (TechMap[current_x + x][object->y + y] == 1) y++;
			else x--;
		}
	}
	else
	{
		current_x += object->rx;
		for (; (y != adm) && (x != speed);)
		{
			if (TechMap[current_x + x][object->y + y] == 1) y++;
			else x++;
		}
	}
	object->x += x;
	object->y += y;
}

int isNearRoof(GameObject* object)
{
	int realHeight = mainArray[object->animation[object->current_animation].id].sizeY / object->animation[object->current_animation].frames,
		jump = 20,
		i = object->y + realHeight + jump,
		g = object->y + realHeight;
	for (; i > g; i--)
		if (TechMap[object->x + 48][i] == 1 || TechMap[object->x + 68][i]) return 1;



	return 0;
}

int isOnGround(GameObject* object, int doFall)
{
	int current_x = object->x, code = 1, y = 0;
	if (object->vector == -1)
	{
		current_x += object->lx;
		if (TechMap[current_x][object->y - 1] == 0) object->air_state = 2;
	}
	else
	{
		current_x += object->rx;
		if (TechMap[current_x][object->y - 1] == 0) object->air_state = 2;
	}
	if (object->air_state == 2 && TechMap[current_x][object->y] == 0) code = 0;
	if (doFall)
	{
		for (; TechMap[object->x + object->lx + 1][object->y + y - 1] != 1 && TechMap[object->x + object->rx - 1][object->y + y - 1] != 1 && TechMap[object->x + object->animation[object->current_animation].height / 2][object->y + y - 1] != 1 && -y != object->weight;)
		{
			y--;
		}
		object->y += y;
	}
	return code;
}
#endif // !PHYSICS_C
