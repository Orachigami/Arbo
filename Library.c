#define true 1;
#define false 0;
typedef _Bool bool;
typedef struct GameActor
{
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
};
typedef struct Enemy
{
	struct GameActor inner;
	int aggression;
};
typedef struct Entity
{
	int hp;
	bool invulnerable;
	bool walkthrough;
};
void Test() {
	struct Entity box;
	box.invulnerable = true;
	
}
