#ifndef _CONTROLLER_C
#define _CONTROLLER_C
#include <GL/freeglut.h>
#include "Library.h"
#include "Animator.c"
#include "Physics.c"

int status = 0;
void mouseMove(int x, int y) 
{
	if (status == 1 && x >= 0 && y >= 0) {
		grass.itself.animation[0].frame[((599 - y) * 800 + x) * 3] = pixel;
		TechMap[x][599 - y] = 1;
		//memcpy(image->data + ((600 - y) * mainArray->sizeX + x) * 3, pixel, 3);
	}
}
void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && x >= 0 && y >= 0)
	{
		if (state == GLUT_UP) 
		{
			status = 0;
		}
		else 
		{
			status = 1;
			grass.itself.animation[0].frame[((599 - y) * 800 + x) * 3] = pixel;
			TechMap[x][599-y] = 1;
			playAnimation(&grass.itself, 0);
			//memcpy(image->data + ((600-y) * mainArray->sizeX + x) * 3, pixel, 3);
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
		overStep(object, 5);
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
	if (!player.stunned)
	{
		status = 0;
		if (GetAsyncKeyState(VK_RIGHT))
		{
			if (player.itself.current_animation != run1)
			{
				if (resetAnimation(&player.itself))
				{
					player.itself.busy = 0;
					player.itself.current_animation = run1;
				}
				playAnimation(&player.itself, run1);
			}
			moveObject(Convertor, 1);
			status = 1;
			//times++;
		}
		else
			if (GetAsyncKeyState(VK_LEFT))
			{
				if (player.itself.current_animation != run0)
				{
					if (resetAnimation(&player.itself))
					{
						player.itself.busy = 0;
						player.itself.current_animation = run0;
					}
					playAnimation(&player, run0);
				}
				moveObject(Convertor, 0);
				status = 1;
			}

		if (GetAsyncKeyState(VK_UP))// && player.itself.air_state == 0)
		{
			player.itself.air_state = 1;
			Jump(Convertor);
			status = 1;
		}
		else if (player.itself.air_state == 1) player.itself.air_state = 2;
		if (GetAsyncKeyState(VK_F4))
		{
			TeleportTo(&player, player.itself.x, 450);
			status = 1;
		}
		//if (GetAsyncKeyState(VK_DOWN)) moveObject(player, 3);
		moveObject(Convertor, 3);
	}
		if (status == 0)
		{
			if (player.itself.current_animation != idle)
			{
				if (resetAnimation(&player.itself))
				{
					player.itself.busy = 0;
					player.itself.current_animation = idle;
				}
				playAnimation(&player, idle);
			}
		}
	
		glutTimerFunc(50, playerController, id);
}
#endif // !_CONTROLLER_C
