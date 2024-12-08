#pragma once
#include "gameNode.h"

enum enEFFECT;
enum enOBJECT;
enum enLOCK;
enum enPOSITION;
enum enANIMATION_STATE;
enum enCHARACTER_TYPE;
enum enCHARACTER_STATE;
enum enINGAME_ITEM;
enum enINGAME_NPC;
enum enICON { ICON_SKULL, ICON_DRACULA, ICON_FRANKEN, ICON_WITCH, ICON_FISHMAN, ICON_NULL };

struct stBase
{
	int speed, index, count;
	image* img;
	POINT pos, rcPos;
	RECT rc;
};

class Object : public gameNode
{
public:
	Object() {};
	~Object() {};
};

