#ifndef ArboLib
#define ArboLib
#include <GL/freeglut.h>
typedef struct _GameObject
{
	unsigned char busy;
	unsigned char current_animation;
	unsigned char animations;
	struct model* animation;
	int x;
	int y;
} GameObject;

struct _GameObject** GameObjects;
typedef struct _AUX_RGBImageRec
{
	GLint sizeX, sizeY;
	unsigned char *data;
} AUX_RGBImageRec;
struct model
{
	unsigned char* frame;
	int id;
	int msecs;
	int frames;
	int name;
	unsigned char loop;
	unsigned char interrupt;
};
typedef struct _GameActor
{
	GameObject itself;
	int hp;
	int move_speed;
	int jump;
	int dexterity;
	int strength;
	int accuracy;
	int adequacy;
	int vitality;
	int mysticism;
	int blood_rust;
}GameActor;
typedef struct _Enemy
{
	GameActor inner;
	int aggression;
}Enemy;
typedef struct _Entity
{
	GameObject itself;
	int hp;
	short invulnerable;
	short walkthrough;
}Entity;
struct line
{
	long sent_time;
	int msecs;
	GameObject* object;
	int frame;
};
struct line order[10];
struct _mainArray
{
	AUX_RGBImageRec *main_arr;
	int length;
} main_array;
#define mainArray main_array.main_arr
enum Animations
{
	idle = 0,
	run = 1,
	jump = 2,
	attack01 = 3

};
#endif // !ArboLib
