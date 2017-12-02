#pragma once

#ifndef _DEBUG
#pragma comment(lib, "../lib/ARS.lib")
#pragma comment(lib, "../lib/WML.lib")
#else
#pragma comment(lib, "../lib/ARSd.lib")
#pragma comment(lib, "../lib/WMLd.lib")
#endif



#define _CRT_SECURE_NO_WARNINGS

#define CLASSNAME "ARSTEST"
#define APPNAME "ARSTEST"



const unsigned int sizex = 640;
const unsigned int sizey = 480;

struct Event{
	int x;
	int y;
};
class Touchable {	
	static enum { OUT_TOUCH, IN_TOUCH };
	int state;
	static std::list<Touchable*> targets;
	Event* checkTouching(Texture* hitArea, unsigned int threshold);
	void updateState(Texture* hitArea, unsigned int threshold);
protected:
	Shape* shape;
	VECTOR2D getPosition2D();
	virtual void onTouch(Event*){};
	virtual void onLeave(){};
public:
	static void update(Texture* hitArea, unsigned int threshold);

	Touchable() : state(OUT_TOUCH)
	{
		targets.push_back(this);
	}
};

