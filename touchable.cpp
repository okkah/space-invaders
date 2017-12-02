#include <tchar.h>
#include <windows.h>
#define D3D_DEBUG_INFO
#include <stdlib.h>
#include <math.h>
#include <d3dx9.h>
#include <XAudio2.h>
#include <vector>
#include <list>

#include "../include/WindowManager.h"
#include "../include/ars.h"
#include "touchable.h"
#include "arstest.h"

inline Event* Touchable::checkTouching(Texture* hitArea, unsigned int threshold)
{
	static Texture txtr;
	ARSG* g = shape->GetARSG();
	txtr.Init(g, sizex, sizey);

	unsigned int pixel_count;

	g->Draw(shape, &txtr);
	ARSC::and(&txtr, &txtr, hitArea, 0x10101010);
	int Gx, Gy;
	ARSC::getCG(&Gx, &Gy, &pixel_count, &txtr);

	return pixel_count > threshold ?
		new Event{ Gx, Gy } : nullptr;

}
inline void Touchable::updateState(Texture* hitArea, unsigned int threshold)
{
	Event* ev = checkTouching(hitArea, threshold);

	if (state == OUT_TOUCH && ev) {
		state = IN_TOUCH;
		onTouch(ev);
	}
	else if (state == IN_TOUCH && !ev) {
		state = OUT_TOUCH;
		onLeave();
	}
	delete ev;
}
std::list<Touchable*> Touchable::targets;
void Touchable::update(Texture* hitArea, unsigned int threshold){
	for (auto it = targets.begin(); it != targets.end(); it++)
		(*it)->updateState(hitArea, threshold);
}

VECTOR2D Touchable::getPosition2D()
{
	VECTOR2D c;
	shape->GetARSG()->Convert3Dto2D(&c, shape->GetPosition());
	return c;
}
