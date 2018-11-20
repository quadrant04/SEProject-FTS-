#ifndef _JONATHANC_H_
#define _JONATHANC_H_

class Unit {
public:
	int onoff;
	int checkpoint;
	int frame;
	double delay;
	Vec pos;
	Vec vel;
	Image *image;
	GLuint tex;
	struct timespec time;
	Unit() {
		onoff = 0;
		frame = 0;
		image = NULL;
		delay = 0.1;
		checkpoint = 1;
	}
};

#endif
