#ifndef ANIMATOR_C
#define ANIMATOR_C
#include "Library.h"
#include "Artist.c"
#include "Controller.c"
#include "Physics.c"
static int length = 0;
//Добавить в очередь прорисовки кадров
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
//Обновление кадра анимации
void updateSprite(GameObject* object, int frame)
{
	if (object->animation[object->current_animation].interrupt == 1)
	{
		object->animation[object->current_animation].interrupt = 0;
		object->busy = 0;
		return;
	}
	object->animation[object->current_animation].frame = mainArray[object->animation[object->current_animation].id].data + mainArray[object->animation[object->current_animation].id].sizeX * mainArray[object->animation[object->current_animation].id].sizeY / object->animation[object->current_animation].frames * 3 * frame;
	drawObject(object, object->x, object->y);
	if (++frame != object->animation[object->current_animation].frames)
		addToOrder(object, frame);
	else
		if (object->animation[object->current_animation].loop == 0) object->busy = 0;
		else addToOrder(object, 0);
}
//Удалить из очереди
void removeFromOrder(int id)
{
	--length;
	register int i = id;
	for (; i < length; i++)
		order[i] = order[i + 1];
}
void resetAnimation(GameObject* object)
{
	register int i = 0;
	for (i; i < length; i++)
		if (order[i].object == object)
		{
			removeFromOrder(i);
			break;
		}
}
//Итератор для очереди кадров
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
//Функция, вызывающая перерисовку текущего кадра анимации
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
