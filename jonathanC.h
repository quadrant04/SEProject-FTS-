#ifndef _JONATHANC_H_
#define _JONATHANC_H_

extern void initPaths(int ID);

class Path {
public:
	int npathPoints;
	Vec pathPoints[15];
	int pathID;
	Path(int p) {
		pathID = p;
		initPaths(pathID);
	}
	
};

class Unit {
public:
	int checkpoint, health, moveSpeed, pathPoints;
	Vec pos;
	Vec vel;
	Image *image;
	Path *path;
	GLuint tex;
	struct timespec time;
	Unit() {
		path = NULL;
		pathPoints = 1;
		image = NULL;
		checkpoint = 1;
		moveSpeed = 1;
		health = 100;
	}
};
#endif
