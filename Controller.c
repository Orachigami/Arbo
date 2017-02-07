#ifndef _CONTROLLER_C
#define _CONTROLLER_C
#include <GL/freeglut.h>
#include "Library.h"
#include "Animator.c"
#include "Physics.c"

int status = 0;
int oooo = 0;
void mouseMove(int x, int y) 
{
	if (status == 1 && x >= 0 && y >= 0) {
		//grass.itself.animation[0].frame[((599 - y) * 800 + x) * 3] = pixel;
		TechMap[x][599 - y] = 1;
		memcpy(image->data + ((600 - y) * image->sizeX + x) * 3, pixel, 3);
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
			//grass.itself.animation[0].frame[((599 - y) * 800 + x) * 3] = pixel;
			
			for (int i = 0; i < 599 - y; i++)
			{
				for (int g = 0; g < 10; g++) 
				{
					TechMap[x + g][i] = 1;
					memcpy(image->data + ((i)* image->sizeX + x + g) * 3, pixel, 3);
				}
			}
		}
	}
}

void moveObject(union _Convertor obj, int key)
{
	int i = 0;
	GameObject* object = obj.toGameObject;
	GameActor* actor = obj.toGameActor;
	int speed = object->speed,
		jump = actor->jump,
		oldx, oldy;
	switch (key)
	{
	case 0:
		//if (object->x - speed < 0) break;
		oldx = object->x;
		oldy = object->y;
		object->vector = -1;
		player.itself.x -= player.itself.speed;
		if (player.itself.x < 0)player.itself.x = 0;
		//if (isNearRoof(object) == 0)
		//overStep(object, 5);
		//drawObject(object, oldx, oldy);
		break;
	case 1:
		//if (object->x + speed + object->animation[object->current_animation].width > 799) break;
		oldx = object->x;
		oldy = object->y;
		object->vector = 1;
		player.itself.x += player.itself.speed;
		//overStep(object, 5);
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
int offfset = 0;
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
			if (player.itself.x != 3200)
				moveObject(Convertor, 1);
			if (GameObjects[CameraTarget]->x > 120 && GameObjects[CameraTarget]->x < 2400)
			{
				FR += GameObjects[CameraTarget]->speed;
				FL += GameObjects[CameraTarget]->speed;
				if (FR > 3200)
				{
					FL = 2600;
					FR = 3200;
				}
				BACKGROUND.animation[0].frame = mainArray->data + (GameObjects[CameraTarget]->x - 120) * 3;
				FOREGROUND.animation[0].frame = (mainArray + FOREGROUND.animation[0].id)->data + (GameObjects[CameraTarget]->x - 120) * 3;
			}
			drawObject(&BACKGROUND, 0, 0);
			status = 1;
			/*times++;*/
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
				if (GameObjects[CameraTarget]->x != 0)
					moveObject(Convertor, 0);
				if (GameObjects[CameraTarget]->x >= 120 && GameObjects[CameraTarget]->x <=2400)
				{
					FR -= GameObjects[CameraTarget]->speed;
					FL -= GameObjects[CameraTarget]->speed;
					if (FL < 0)
					{
						FL = 0;
						FR = 800;
					}
					BACKGROUND.animation[0].frame = mainArray->data + (GameObjects[CameraTarget]->x - 120) * 3;
					FOREGROUND.animation[0].frame = (mainArray + FOREGROUND.animation[0].id)->data + (GameObjects[CameraTarget]->x - 120) * 3;
				}
				drawObject(&BACKGROUND, 0, 0);
				status = 1;
			}

		if (GetAsyncKeyState(VK_UP))/* && player.itself.air_state == 0)*/
		{
			player.itself.air_state = 1;
			Jump(Convertor);
			status = 1;
		}
		//else if (player.itself.air_state == 1) player.itself.air_state = 2;
		if (GetAsyncKeyState(VK_F1))
		{
			drawObject(GameObjects[2], GameObjects[2]->x, GameObjects[2]->y);
		}
		if (GetAsyncKeyState(VK_F2))
		{
			CameraLocked = 0;
			CameraTarget = 2;
			GetCameraFocus(2);
			status = 1;
		}
		if (GetAsyncKeyState(VK_F3))
		{
			CameraLocked = 0;
			CameraTarget = 3;
			GetCameraFocus(3);
			status = 1;
		}
		isOnGround(&player.itself, 1);
	}
	else GetAsyncKeyState(VK_UP);
		if (status == 0)
		{
			if (player.itself.current_animation != idle && 1)
			{
				if (resetAnimation(&player.itself))
				{
					player.itself.busy = 0;
					player.itself.current_animation = idle;
				}
				playAnimation(&player, idle);
			}
		}
		/*if (GetAsyncKeyState(VK_ESCAPE)) glutLeaveGameMode();*/
		glutTimerFunc(50, playerController, id);
}
#endif /* !_CONTROLLER_C*/
