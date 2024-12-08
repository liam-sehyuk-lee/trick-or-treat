#pragma once
#include "Object.h"

#define df_MAXITEM 4

struct stItem { image* img; int count, index; };

class Object_Item : public Object
{
private:
	
	stItem m_item[df_MAXITEM];

public:

	HRESULT init();
	void release();
	void update();
	void render(HDC hdc, enINGAME_ITEM item, int x, int y, enICON iconType = ICON_NULL);

	void frame();

public:
	Object_Item() {};
	~Object_Item() {};
};