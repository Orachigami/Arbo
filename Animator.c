#ifndef ANIMATOR_C
#define ANIMATOR_C
#include "Library.h"
#include "Artist.c"
#include "Controller.c"
#include "Physics.c"
static int length = 0;
//Adds the sent frame of object's current animation into the FrameOrder
void addToOrder(GameObject* object, int frame)
{
	static int dozen = 1;
	struct line _line;
	if (++length >= dozen * 10) realloc(&order, sizeof(struct line) * 10 * (++dozen));
	_line.msecs = object->animation[object->current_animation].msecs;
	_line.frame = frame;
	_line.object = object;
	_line.sent_time = GetTickCount();
	order[length - 1] = _line;
}
//*Changes the object's current frame on the sent one.
//If animation doesn't have the sent frame and if the animation isn't looped then nothing adds into the FrameOrder
void updateSprite(GameObject* object, int frame)
{
	object->animation[object->current_animation].frame = mainArray[object->animation[object->current_animation].id].data + mainArray[object->animation[object->current_animation].id].sizeX * mainArray[object->animation[object->current_animation].id].sizeY / object->animation[object->current_animation].frames * 3 * frame;
	drawObject(object, object->x, object->y);
	if (++frame != object->animation[object->current_animation].frames)
		addToOrder(object, frame);
	else
		if (object->animation[object->current_animation].loop == 0)
		{
			object->busy = 0;
		}
		else addToOrder(object, 0);
}
//**Removes the frame from the FrameOrder via it's id
void removeFromOrder(int id)
{
	--length;
	register int i = id;
	for (; i < length; i++)
		order[i] = order[i + 1];
}
//Soft animation reset for the sent object
//Animations such as 'idle' or 'run' should be interruptable by 'jump'
int resetAnimation(GameObject* object)
{
	register int i = 0;
	if (object->animation[object->current_animation].interruptable == 1)
	{
		for (i; i < length; i++)
			if (order[i].object == object)
			{
				removeFromOrder(i);
				break;
			}
		return 1;
	}
	return 0;
}
//Global cycle which looks for the frame that should be updated
//Uses Ticks for checking frame's time
//If time has come:
//1. The function requests an update* for the frame
//2. The function requests the removal** of the frame
void iterate(int timer_id)
{
	register int i = 0;
	register long now = GetTickCount();
	for (i; i < length; i++)
		if (now - order[i].sent_time >= order[i].msecs)
		{
			updateSprite(order[i].object, order[i].frame);
			removeFromOrder(i);
		}
	//playerController(0);
	i = 16 - now;
	if (i < 0) i = 0;
	glutTimerFunc(i, iterate, timer_id);
}
//Starts the sent id animation cycle for the sent object
//Just changes the current animation ID;
//Marks the object as 'busy';
//Requests the first animation frame's update*
void playAnimation(GameObject* object, int animation)
{
	register int i = 0;
	if (object->busy == 0)
	{
		for (; i < object->animations; i++)
			if (object->animation[i].name == animation)
			{
				object->current_animation = animation;
				object->busy = 1;
				updateSprite(object, 0);
				break;
			}
	}
}
#endif // !ANIMATOR_C
