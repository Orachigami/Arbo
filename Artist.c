#ifndef ARTIST_C
#define ARTIST_C
#include <stdlib.h>
#include "Library.h"
#include "ResourceManager.c"
AUX_RGBImageRec* image;

static int last_id = 0;
/*Checks whether two areas are crossing each other*/
/*Returns the cross-area*/
struct Area checkCrossing(struct Area* object, GameObject* next_object)
{
	int id1 = object->id,
		id2 = next_object->animation[next_object->current_animation].id,
		height1 = object->height,
		height2 = next_object->animation[next_object->current_animation].height;
	struct Area area;
	if (object->x <= next_object->x && object->x + object->width > next_object->x)
	{
		area.x = next_object->x;
		area.width = (object->x + object->width > area.x + mainArray[id2].sizeX) ? mainArray[id2].sizeX : object->x + object->width - area.x;
		if (object->y <= next_object->y && object->y + height1 > next_object->y)
		{
			area.y = next_object->y;
			area.height = (object->y + height1 > area.y + height2) ? height2 : object->y + height1 - area.y;
			return area;
		}
		if (object->y > next_object->y && next_object->y + height2 > object->y)
		{
			area.y = object->y;
			area.height = (next_object->y + height2 > area.y + height1) ? height1 : next_object->y + height2 - area.y;
			return area;
		}
	}
	if (object->x > next_object->x && next_object->x + mainArray[id2].sizeX >= object->x)
	{
		area.x = object->x;
		area.width = (next_object->x + mainArray[id2].sizeX > area.x + object->width) ? object->width : next_object->x + mainArray[id2].sizeX - area.x;
		if (object->y <= next_object->y && object->y + height1 > next_object->y)
		{
			area.y = next_object->y;
			area.height = (object->y + height1 > area.y + height2) ? height2 : object->y + height1 - area.y;
			return area;
		}
		if (object->y > next_object->y && next_object->y + height2 > object->y)
		{
			area.y = object->y;
			area.height = (next_object->y + height2 > area.y + height1) ? height1 : next_object->y + height2 - area.y;
			return area;
		}
	}
	area.height = -1;
	return area;
}
/*Draws the sent AREA for the Object with the sent ID*/
static int drawArea(struct Area area, int id)
{
	int y = 0,
		x = 0,
		width = GameObjects[id]->animation[GameObjects[id]->current_animation].width,
		offset = (area.y - GameObjects[id]->y)*width + area.x - GameObjects[id]->x;
	area.x -= FL;
	if (id != 2 || area.x < 0)
	{
		if (area.x < 800)
		{
			if (area.x >= 0)
			{
				if (area.x + area.width > 800) area.width = 800 - area.x;
			}
			else
				if (area.x + area.width > 0)
				{
					offset = -area.x;
					area.width = area.x + area.width;
					area.x = 0;
					if (area.x + area.width > 800) area.width = 800;
				}
				else
					return 1;
		}
		else 
			return 1;
	}
	/*if (CameraTarget != 0 && area.x == GameObjects[id]->x && CameraLocked == 1 && area.id == CameraTarget && FR != 3200 && GameObjects[CameraTarget]->x >= 120)
	{
		offset = 0;
	}*/
	if ((area.id == 0 || area.id == 4) && offset != 0)
		offset = (area.y)*width + area.x;
	offset -= width;
	for (y = 0; y < area.height; y++)
	{
		offset += width;
		for (x = 0; x < area.width; x++)
		{
			if (GameObjects[id]->animation[GameObjects[id]->current_animation].frame[(offset + x) * 3] != 255 || GameObjects[id]->animation[GameObjects[id]->current_animation].frame[(offset + x) * 3 + 1] != 0 || GameObjects[id]->animation[GameObjects[id]->current_animation].frame[(offset + x) * 3 + 2] != 0)
				memcpy(image->data + ((area.y + y) * image->sizeX + area.x + x) * 3, GameObjects[id]->animation[GameObjects[id]->current_animation].frame + (offset + x) * 3, 3);
		}
	}
	return 0;
}
/*Clears the object's previous location, then*/
/*Redraws object completely*/
void drawObject(GameObject* object, int oldx, int oldy)
{
	struct Area area,
		original_area,
		extended_area;
	GameObject* next_object;
	int delta_x = object->x - oldx,
		delta_y = object->y - oldy;
	int id = object->animation[object->current_animation].id,
		width = object->animation[object->current_animation].width,
		height = object->animation[object->current_animation].height;
	register int i = 0;
	extended_area.id = id;
	extended_area.width = (delta_x < 0) ? -delta_x + width : delta_x + width;
	extended_area.height = (delta_y < 0) ? -delta_y + height : delta_y + height;
	extended_area.x = (delta_x < 0) ? object->x : oldx;
	extended_area.y = (delta_y < 0) ? object->y : oldy;

	original_area.id = id;
	original_area.width = width;
	original_area.height = height;
	original_area.x = object->x;
	original_area.y = object->y;
	for (i = 0; i < gmlen; i++)
	{
		next_object = GameObjects[i];

		if (object != next_object)
			area = checkCrossing(&extended_area, next_object);
		else
			area = checkCrossing(&original_area, next_object);
		if (area.height != -1)
		{
			area.id = i;
			OrderToClear.area[++OrderToClear.last_id] = area;
		}
	}
	for (i = 0; i <= OrderToClear.last_id; i++)
		drawArea(OrderToClear.area[i], OrderToClear.area[i].id);
	OrderToClear.last_id = -1;
	glutPostRedisplay();
}
#endif /* !ARTIST_C*/
