#ifndef ArboLib
#define ArboLib
#include <GL/freeglut.h>
unsigned char pixel[3] = { 0, 255, 0 };
unsigned char red[3] = { 255, 0, 0 };
typedef struct _GameObject
{
	unsigned char busy;
	unsigned char current_animation;
	unsigned char animations;
	struct model* animation;
	int x;
	int y;
	int lx, ty, rx;	//Object Edge
	signed char vector; //Eyesight vector
	unsigned char air_state; //0 - ground, 1 - flies up, 2 - falls
	unsigned char weight; //Falling offset
} GameObject;
//Animation's frame structure
struct model
{
	unsigned char* frame;
	int id;
	int msecs;
	int frames;
	int name;
	unsigned char loop;
	unsigned char interruptable;
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
	//Actor controller status
	char stunned;
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
//A union for universal work with the GameObjects
union _Convertor
{
	struct _GameObject* toGameObject;
	struct _GameActor* toGameActor;
}Convertor;

GameActor player;
//An image with width(sizeX) and height(sizeY)
//+Data
typedef struct _AUX_RGBImageRec
{
	GLint sizeX, sizeY;
	unsigned char *data;
} AUX_RGBImageRec;
//An array where all loaded images are located
struct _mainArray
{
	AUX_RGBImageRec *main_arr;
	int length;
} main_array;
#define mainArray main_array.main_arr
//The structure that describes a certain area/zone
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
//The structure of one frame for the FrameOrder
struct line
{
	//The time when the frame was set
	long sent_time;
	//How much time should the frame be shown
	int msecs;
	GameObject* object;
	//The frame number
	int frame;
};
//The FrameOrder
struct line order[10];
//A structure of one action for the ActionQueue
struct row
{
	//The time when the action was set
	long sent_time;
	//Action func
	void (*todo) (struct row*);
	//Subject
	GameObject* object;
	//Milliseconds
	int msecs;
	//Additional variable 1
	int help1;
	//Additional variable 2
	int help2;
	//Action's stage
	int step;
};
//The ActionQueue
struct row* queue;
enum Animations
{
	idle = 0,
	run0 = 1,
	run1 = 2,
	jump0 = 3,
	jump1 = 4,
	attack01 = 5

};
//Magic 2D path eye
unsigned char** TechMap;
//Image names
char** StringPool;

void moveObject(GameObject *object, int key);
void playerController(int id);
void playAnimation(GameObject* object, int animation);

int resetAnimation(GameObject* object);
int times;
Entity grass;
#endif // !ArboLib
