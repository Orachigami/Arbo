#ifndef PHYSICS_C
#define PHYSICS_C
#include "Library.h"
#include "Controller.c"
#include "Animator.c"
#include <math.h>
//FALL
//JUMP
//Action queue length
static int action_length = 0;

//OVERSTEP

//Movement logic/behaviour for overstepping blocks
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
//Tests the "roof" above the game object
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
//Tests the ground under the game object
//*Uses TechMap: 1 - wall/ground, 0 - air
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
	if (object->air_state == 2 && TechMap[current_x][object->y - 1] == 0) code = 0;
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
//Updates action's current stage time and adds action row into the Queue
void addToQueue(struct row* subject)
{
	static int dozen = 1;
	if (++action_length >= dozen * 10) realloc(queue, sizeof(struct row) * 10 * (++dozen));
	subject->sent_time = GetTickCount();
	queue[action_length - 1] = *subject;
}
//Maths jump logic
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
	object->x += actor->speed*object->vector;
	object->y = -1* pow((object->x - actor->speed*sqrt(actor->jump)*object->vector - subject->help1)/actor->speed,2) + actor->jump + subject->help2;
	//printf("X: %d; Y: %d;\n", object->x, object->y);
	drawObject(object, x, y);
	subject->step++;
	if (isOnGround(object, 0))
	{
		actor->stunned = 0;
		return;
	}
	addToQueue(subject);
}
//Initializates action row for the "Jump" action
void Jump(union _Convertor obj)
{
	struct row* startJump = malloc(sizeof(struct row));
	GameActor* actor = obj.toGameActor;
	GameObject* object = obj.toGameObject;
	actor->stunned++;
	if (resetAnimation(obj.toGameObject))
	{
		actor->itself.busy = 0;
		object->current_animation = jump0;
	}
	//playAnimation(object, jump0);
	startJump->object = object;
	startJump->todo = jumpProceed;
	startJump->step = 0;
	startJump->sent_time = GetTickCount();
	startJump->msecs = 50;
	startJump->help1 = object->x;
	startJump->help2 = object->y;
	addToQueue(startJump);
}
//**Removes the action from the ActionQueue via it's id
void removeFromQueue(int id)
{
	register int i = id;
	union _Convertor obj;
	obj.toGameObject = queue[id].object;
	--action_length;
	for (; i < action_length; i++)
		queue[i] = queue[i + 1];
}
//Global cycle which looks for the action that should be updated
//Uses Ticks for checking action's time
//P.S. mb concatenate with animatior.c "iterate" or/and player controller
void iterator(int timer_id)
{
	register int i = 0;
	register long now = GetTickCount();
	for (i; i < action_length; i++)
		if (now - queue[i].sent_time >= queue[i].msecs)
		{
			queue[i].todo(&queue[i]);
			removeFromQueue(i);
		}
	//i = GetTickCount() - now - 16;
	//if (i <= 0) i = 0;
	//else
	//printf("---------\nACTION: %d\n---------", i);
	glutTimerFunc(i, iterator, timer_id);
}
#endif // !PHYSICS_C
