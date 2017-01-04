#ifndef ArboLib
#define ArboLib
#include <GL/freeglut.h>
unsigned char pixel[3] = { 255, 0, 0 };

typedef struct _GameObject
{
	unsigned char busy;
	unsigned char current_animation;
	unsigned char animations;
	struct model* animation;
	int x;
	int y;
	int lx, ty, rx;	//Крайние точки
	signed char vector;
	unsigned char air_state;
	unsigned char weight;
} GameObject;

struct model
{
	unsigned char* frame;
	int id;
	int msecs;
	int frames;
	int name;
	unsigned char loop;
	unsigned char interrupt;
	int height;
	int width;
};

struct _GameObject** GameObjects;
int gmlen;

typedef struct _GameActor
{
	GameObject itself;
	int hp;
	int speed;
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

union _Convertor
{
	struct _GameObject* toGameObject;
	struct _GameActor* toGameActor;

}Convertor;

GameActor player;

typedef struct _AUX_RGBImageRec
{
	GLint sizeX, sizeY;
	unsigned char *data;
} AUX_RGBImageRec;
struct _mainArray
{
	AUX_RGBImageRec *main_arr;
	int length;
} main_array;
#define mainArray main_array.main_arr

struct response
{
	int id;
	int x;
	int y;
	int width;
	int height;
};
struct _OrderToClear
{
	struct response* area;
	int last_id;
} OrderToClear;
struct line
{
	long sent_time;
	int msecs;
	GameObject* object;
	int frame;
};
struct line order[10];
enum Animations
{
	idle = 0,
	run0 = 1,
	run1 = 2,
	jump = 3,
	attack01 = 4

};
unsigned char** TechMap;
char** StringPool;
void moveObject(GameObject *object, int key);
void playerController(int id);
#endif // !ArboLib
