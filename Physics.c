#ifndef PHYSICS_C
#define PHYSICS_C
#include "Library.h"
#include "Controller.c"
#include "Animator.c"
#include <math.h>
/*FALL*/
/*JUMP*/
/*Action queue length*/
static int action_length = 0;

/*OVERSTEP*/

/*Movement logic/behaviour for overstepping blocks*/
void overStep(GameObject* object, int adm)
{
	int x = object->vector == 1 ? object->rx : object->lx, y = 0, //current_x = object->x,
		speed = x + object->speed * object->vector;
	if (object->x + speed < 0)
	{
		object->x = 0;
		return;
	}
	for (y = 0; y < 5 && x != speed && y > -object->weight; y++)
	{
		for (x = x; x != speed; x += object->vector)
		{
			if (TechMap[object->x + x][object->y + y] == 1) break;
			if (TechMap[object->x + x][object->y + y - 1] != 1) y--;
			object->x += object->vector;
		}
	}
	object->y += y;




	/*if (object->vector < 0)
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
	object->y += y;*/
}
/*Tests the "roof" above the game object*/
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
/*Tests the ground under the game object*/
/**Uses TechMap: 1 - wall/ground, 0 - air*/
int isOnGround(GameObject* object, int doFall)
{
	int current_x = object->x, code = 1, y = 0;
	if (TechMap[current_x + object->lx][object->y - 1] == 0 && TechMap[current_x + object->rx][object->y - 1] == 0 && TechMap[current_x + object->cx][object->y - 1] == 0)
	{
		object->air_state = 2;
		code = 0;
	}
	if (doFall)
	{
		for (; TechMap[current_x + object->lx][object->y + y] != 1 && TechMap[current_x + object->cx][object->y + y] != 1 && TechMap[current_x + object->rx][object->y + y] != 1 && y > -object->weight;)
		{
			y--;
		}
		object->y += y;
	}
	return code;
}
/*Updates action's current stage time and adds action row into the Queue*/
void addToQueue(struct row* subject)
{
	static int dozen = 1;
	if (++action_length >= dozen * 10) realloc(queue, sizeof(struct row) * 10 * (++dozen));
	subject->sent_time = GetTickCount();
	queue[action_length - 1] = *subject;
}

/*Camera movement logic*/
void cameraProceed(struct row* CameraVector)
{
	int i, _continue = 1;
	FL += CameraVector->step;
	FR += CameraVector->step;
	if (FL < 0)
	{
		FL = 0;
		FR = 800;
		CameraLocked = 1;
		_continue--;
	}
	if (FR > 3200)
	{
		FL = 3200 - 800;
		FR = 3200;
		CameraLocked = 1;
		_continue--;
	}
	if (GameObjects[CameraVector->help1]->x - FL - 120 < 10 && GameObjects[CameraVector->help1]->x - FL - 120 > -10)
	{
		_continue--;
		CameraLocked = 1;
	}
	for (i = 0; i < 600; i++)
	{
		memcpy(BACKGROUND.animation[0].frame + i * 800 * 3, mainArray->data + (i* mainArray->sizeX + FL) * 3, 2400);
		memcpy(FOREGROUND.animation[0].frame + i * 800 * 3, (mainArray + FOREGROUND.animation->id)->data + (i* (mainArray + FOREGROUND.animation->id)->sizeX + FL) * 3, 2400);
	}
	if (_continue) addToQueue(CameraVector);
}
/*Get camera focus*/
void GetCameraFocus(int GameObjectID)
{
	struct row* CameraVector;
	if (GameObjectID >= gmlen || FL + 120 == GameObjects[GameObjectID]->x) return;
	CameraVector = malloc(sizeof(struct row));
	if (FL < GameObjects[GameObjectID]->x) CameraVector->step = 10;
	else CameraVector->step = -10;
	CameraVector->msecs = 50;
	CameraVector->todo = cameraProceed;
	CameraVector->help1 = GameObjectID;
	addToQueue(CameraVector);
}
/*Maths jump logic*/
void jumpProceed(struct row* subject)
{
	int step = subject->step;
	union _Convertor obj;
	int y = subject->object->y,
		x = subject->object->x;
	GameActor* actor;
	GameObject* object;
	obj.toGameObject = subject->object;
	actor = obj.toGameActor;
	object = obj.toGameObject;
	for (y = 0;; y+= actor->jump-step)
	{
		if (isOnGround(object, 0) && y!=0)
		{
			object->y += y;
			object->x += x;
			actor->stunned = 0;
			return;
		}
		for (x = object->vector; x != (actor->jump-1)*object->vector; x+= object->vector)
		{
			if (TechMap[object->x + object->lx + x][object->y + y] == 1 || TechMap[object->x + object->rx + x][object->y + y] == 1)
			{
				object->y += y;
				object->x += x;
				actor->stunned = 0;
				return;
			}
		}
	}
	subject->step -= 1;
	object->y += y;
	object->x += x;
	drawObject(object, object->x - x, object->y - y);
	addToQueue(subject);
}
/*Initializates action row for the "Jump" action*/
void Jump(union _Convertor obj)
{
	struct row* startJump = malloc(sizeof(struct row));
	GameActor* actor = obj.toGameActor;
	GameObject* object = obj.toGameObject;
	int state = object->vector == -1 ? jump0 : jump1;
	actor->stunned++;
	if (resetAnimation(obj.toGameObject))
	{
		actor->itself.busy = 0;
		object->current_animation = state;
	}
	playAnimation(object, state);
	startJump->object = object;
	startJump->todo = jumpProceed;
	startJump->step = 0;
	startJump->sent_time = GetTickCount();
	startJump->msecs = 50;
	addToQueue(startJump);
}
/***Removes the action from the ActionQueue via it's id*/
void removeFromQueue(int id)
{
	register int i = id;
	union _Convertor obj;
	obj.toGameObject = queue[id].object;
	--action_length;
	for (; i < action_length; i++)
		queue[i] = queue[i + 1];
}
/*Global cycle which looks for the action that should be updated*/
/*Uses Ticks for checking action's time*/
/*P.S. mb concatenate with animatior.c "iterate" or/and player controller*/
static void iterator(int timer_id)
{
	register int i = 0;
	register long now = GetTickCount();
	for (i; i < action_length; i++)
		if (now - queue[i].sent_time >= queue[i].msecs)
		{
			queue[i].todo(&queue[i]);
			removeFromQueue(i);
		}
	glutTimerFunc(i, iterator, timer_id);
}
#endif /* !PHYSICS_C*/
