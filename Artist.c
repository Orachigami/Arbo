#ifndef ARTIST_C
#define ARTIST_C
#include <stdlib.h>
#include "Library.h"
#include "ResourceManager.c"
#define temp 3
AUX_RGBImageRec* image;

int last_id = 0;

struct response checkCrossing(struct response* object, GameObject* checked_object)
{
	int id1 = object->id,
		id2 = checked_object->animation[checked_object->current_animation].id,
		height1 = object->height,
		height2 = mainArray[id2].sizeY / checked_object->animation[checked_object->current_animation].frames;
	struct response _response;
	if (object->x <= checked_object->x && object->x + object->width > checked_object->x)
	{
		_response.x = checked_object->x;
		_response.width = (object->x + object->width > _response.x + mainArray[id2].sizeX) ? mainArray[id2].sizeX : object->x + object->width - _response.x;
		if (object->y <= checked_object->y && object->y + height1 > checked_object->y)
		{
			_response.y = checked_object->y;
			_response.height = (object->y + height1 > _response.y + height2) ? height2 : object->y + height1 - _response.y;
			return _response;
		}
		if (object->y > checked_object->y && checked_object->y + height2 > object->y)
		{
			_response.y = object->y;
			_response.height = (checked_object->y + height2 > _response.y + height1) ? height1 : checked_object->y + height2 - _response.y;
			return _response;
		}
	}
	if (object->x > checked_object->x && checked_object->x + mainArray[id2].sizeX >= object->x)
	{
		_response.x = object->x;
		_response.width = (checked_object->x + mainArray[id2].sizeX > _response.x + object->width) ? object->width : checked_object->x + mainArray[id2].sizeX - _response.x;
		if (object->y <= checked_object->y && object->y + height1 > checked_object->y)
		{
			_response.y = checked_object->y;
			_response.height = (object->y + height1 > _response.y + height2) ? height2 : object->y + height1 - _response.y;
			return _response;
		}
		if (object->y > checked_object->y && checked_object->y + height2 > object->y)
		{
			_response.y = object->y;
			_response.height = (checked_object->y + height2 > _response.y + height1) ? height1 : checked_object->y + height2 - _response.y;
			return _response;
		}
	}
	_response.height = -1;
	return _response;
}

int drawArea(struct response _response, int id)
{
	int y = 0,
		x = 0,
		width = mainArray[GameObjects[id]->animation[GameObjects[id]->current_animation].id].sizeX,
		offset = (_response.y + y - GameObjects[id]->y)*width + _response.x - GameObjects[id]->x;

	offset -= width;
	for (y = 0; y < _response.height; y++)
	{
		offset += width;
		for (x = 0; x < _response.width; x++)
		{
			if (GameObjects[id]->animation[GameObjects[id]->current_animation].frame[(offset + x) * 3] != 255 || GameObjects[id]->animation[GameObjects[id]->current_animation].frame[(offset + x) * 3 + 1] != 0 || GameObjects[id]->animation[GameObjects[id]->current_animation].frame[(offset + x) * 3 + 2] != 0)
				memcpy(image->data + ((_response.y + y) * image->sizeX + _response.x + x) * 3, GameObjects[id]->animation[GameObjects[id]->current_animation].frame + (offset + x) * 3, 3);
		}
	}
	return 0;
}
void drawObject(GameObject* object, int oldx, int oldy)
{
	struct response _response,
		original_area,
		excluded_area;
	static int busy = 0;
	GameObject* checked_object;
	int delta_x = object->x - oldx,
		delta_y = object->y - oldy;
	int id = object->animation[object->current_animation].id, 
		real_Height = (mainArray + id)->sizeY / object->animation[object->current_animation].frames, 
		tmp0, tmp1;
	register int i = 0,
		g = 0,
		k = 0;
	for (i = 0; i < real_Height; i++)
	{
		tmp1 = ((i + oldy) * image->sizeX + oldx) * 3;
		memcpy(image->data + tmp1, (mainArray)->data + tmp1, 3 * (mainArray + id)->sizeX);
	}
	excluded_area.id = id;
	excluded_area.width = (delta_x < 0) ? -delta_x + (mainArray + id)->sizeX : delta_x + (mainArray + id)->sizeX;
	excluded_area.height = (delta_y < 0) ? -delta_y + real_Height : delta_y + real_Height;
	excluded_area.x = (delta_x < 0) ? object->x : oldx;
	excluded_area.y = (delta_y < 0) ? object->y : oldy;

	original_area.id = id;
	original_area.width = (mainArray + id)->sizeX;
	original_area.height = real_Height;
	original_area.x = object->x;
	original_area.y = object->y;
	//OrderToClear.area[++OrderToClear.last_id] = original_area;
	for (i = 0; i < temp; i++)
	{
		checked_object = GameObjects[i];
		if (object != checked_object)
			_response = checkCrossing(&excluded_area, checked_object); 
		else _response = checkCrossing(&original_area, checked_object);
		if (_response.height != -1)
		{
			_response.id = i;
			OrderToClear.area[++OrderToClear.last_id] = _response;
		}
	}
	for (i = 0; i <= OrderToClear.last_id; i++)
		drawArea(OrderToClear.area[i], OrderToClear.area[i].id);
	OrderToClear.last_id = -1;
	glutPostRedisplay();
}
#endif // !ARTIST_C
