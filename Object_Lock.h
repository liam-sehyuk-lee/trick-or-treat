#pragma once
#include "Object.h"

#define df_MAXLOCK 3

struct stLock { image* img; int count, index; bool on; };

class Object_Lock : public Object
{
private:

	stLock m_lock[df_MAXLOCK];

public:

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc, enLOCK lock, int x, int y);

	void frame();
	void setFrame(enLOCK lock);

	void resetFrame(int lock) { m_lock[lock].img->setFrameX(0); }

	image* getImage(enLOCK lock) { return m_lock[lock].img; }

public:
	Object_Lock() {};
	~Object_Lock() {};
};

