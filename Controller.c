#ifndef _CONTROLLER_C
#define _CONTROLLER_C
#include <GL/freeglut.h>
#include "Library.h"
#include "Animator.c"
#include "Physics.c"

int status = 0;
void mouseMove(int x, int y) 
{
	if (status == 1) {

		TechMap[x][600 - y] = 1;
		memcpy(image->data + ((600 - y) * mainArray->sizeX + x) * 3, pixel, 3);
	}
}
void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{

		if (state == GLUT_UP) {
			;
		}
		else 
		{
			status = 1;
			TechMap[x][600-y] = 1;
			memcpy(image->data + ((600-y) * mainArray->sizeX + x) * 3, pixel, 3);
		}
	}
}

void moveObject(union _Convertor obj, int key)
{
	GameObject* object = obj.toGameObject;
	GameActor* actor = obj.toGameActor;
	int speed = actor->speed,
		jump = actor->jump,
		oldx, oldy;
	switch (key)
	{
	case 0:
		if (object->x - speed < 0) break;
		oldx = object->x;
		oldy = object->y;
		object->vector = -1;
		if (isNearRoof(object) == 0)
		overStep(object, 5);
		drawObject(object, oldx, oldy);
		break;
	case 1:
		if (object->x + speed + object->animation[object->current_animation].width > 799) break;
		oldx = object->x;
		oldy = object->y;
		object->vector = 1;
		overStep(object, 15);
		drawObject(object, oldx, oldy);
		break;
	case 2:
		if (object->y + jump + object->animation[object->current_animation].height > 599 || isNearRoof(object)) break;
		object->y += jump;
		drawObject(object, object->x, object->y - jump);
		break;
	case 3:
		if (object->y - object->weight < 0) break;
		oldx = object->x;
		oldy = object->y;
		isOnGround(object, 1);
		drawObject(object, oldx, oldy);
		break;
	default: break;
	}
}
void TeleportTo(GameObject* object, int x, int y)
{
	int temp;
	temp = x;
	x = object->x;
	object->x = temp;
	temp = y;
	y = object->y;
	object->y = temp;
	drawObject(object, x, y);

}
void playerController(int id)
{
	static char status = 0;
	Convertor.toGameActor = &player;
	//printf("w");
	status = 0;
	if (GetAsyncKeyState(VK_RIGHT))
	{
		if (player.itself.current_animation != run1)
		{
			resetAnimation(&player);
			player.itself.busy = 0;
			playAnimation(&player, run1);
		}
		moveObject(Convertor, 1);
		status = 1;
	}
	else
	if (GetAsyncKeyState(VK_LEFT))
	{
		if (player.itself.current_animation != run0)
		{
			resetAnimation(&player);
			player.itself.busy = 0;
            playAnimation(&player, run0);
		}
		moveObject(Convertor, 0);
		status = 1;
	}

		if (GetAsyncKeyState(VK_UP))
		{
			player.itself.air_state = 1;
			moveObject(Convertor, 2);
			status = 1;
		}
		else if (player.itself.air_state == 1) player.itself.air_state = 2;
		if (GetAsyncKeyState(VK_F4))
		{
			TeleportTo(&player, player.itself.x, 450);
			status = 1;
		}
		//if (GetAsyncKeyState(VK_DOWN)) moveObject(player, 3);
		if (status == 0)
		{
			if (player.itself.current_animation != idle)
			{
				resetAnimation(&player);
				player.itself.busy = 0;
				playAnimation(&player, idle);
			}
		}
		moveObject(Convertor, 3);
		glutTimerFunc(50, playerController, id);
}
#endif // !_CONTROLLER_C
