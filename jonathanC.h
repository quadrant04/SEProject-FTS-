#ifndef _JONATHANC_H_
#define _JONATHANC_H_

extern void initPaths(int ID);

enum {
	TYPE_NORMAL = 0,
	TPYE_FIRE,
	TYPE_TANK
};

class Path {
public:
	int npathPoints;
	Vec pathPoints[20];
	int pathID;
	Path(int p) {
		pathID = p;
		initPaths(pathID);
	}
	
};

class Unit {
public:
	int checkpoint, baseHealth, health, baseMoveSpeed, moveSpeed, pathPoints;
	int type;
	Vec pos;
	Vec vel;
	Image *image;
	Path *path;
	GLuint tex;
	int frame, size;
	struct timespec time;
public:
	Unit() {
		type = TYPE_NORMAL;
		path = NULL;
		pathPoints = 1;
		image = NULL;
		checkpoint = 1;
		moveSpeed = 1;
		health = 100;
		frame = 0;
		size = 1;
	}

	void setType(int typeID)
	{
		if (typeID == 0) {
			type = TYPE_NORMAL;
			baseHealth = 100;
			baseMoveSpeed = 2;
			size = 35;
		}

		if (typeID == 1) {
			type = TPYE_FIRE;
			baseHealth = 50;
			baseMoveSpeed = 3;
			size = 30;
		}

		if (typeID == 2) {
			type = TYPE_TANK;
			baseHealth = 200;
			baseMoveSpeed = 1;
			size = 50;
		}
	}

	void setHealth(int bonus)
	{
		health = baseHealth + (baseHealth * bonus);
	}

	void setSpeed(int bonus)
	{
		moveSpeed = baseMoveSpeed + (baseMoveSpeed * bonus);
	}

	void takeDamage(int dmg)
	{
		health -= dmg;
	}

	void setupSlime(int typeID, int healthBonus, int moveBonus)
	{
		setType(typeID);
		setHealth(healthBonus);
		setSpeed(moveBonus);
	}
};
#endif
