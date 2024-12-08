#pragma once
#include "Object.h"

class Object_Ghost : public Object
{
private:

	image* m_image;
	int m_count, m_index, m_move;

	enPOSITION m_pos;

public:

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc, int x, int y);

	void move();
	void collision();

	void frame();

	int getMove() { return m_move; }
	image* getImage() { return m_image; }
	bool getChanged() { if (m_image->getFrameY() == 2) return true; }

	void setFrame() { m_image->setFrameY(2); }
	void resetFrame() { m_image->setFrameY(0); }

public:
	Object_Ghost() {};
	~Object_Ghost() {};
};

