#pragma once
#include "Object.h"

#define df_MAXHOUSE 6

class Object_House : public Object
{
private:

	image* m_house[df_MAXHOUSE];

public:

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc, enHOUSE house, int x, int y);

	int getFrameX(enHOUSE house) { return m_house[house]->getFrameX(); }
	void setFrameX(enHOUSE house) { return m_house[house]->setFrameX(1); }
	void resetFrameX(int house) { return m_house[house]->setFrameX(0); }

public:
	Object_House() {};
	~Object_House() {};
};

