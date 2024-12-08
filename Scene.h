#pragma once
#include "gameNode.h"

enum enTERRAIN;
enum enOBJECT;
enum enHOUSE;
enum enLOCK;
enum enFLYABLE;

#define df_MAXOBJECT 15
#define df_TILEX 32
#define df_TILEY 32

struct stScene { image* img; RECT rc; POINT pos; int index; };
struct stObject { bool on; float num; };							//오브젝트
struct stTile														//타일 구조체
{
	enTERRAIN terrain; enOBJECT obj; enHOUSE house; enFLYABLE fly; enLOCK lock;
	RECT rc; RECT rcDebug[4];
	int terrainFrameX, terrainFrameY, objFrameX, objFrameY;
};

class Scene : public gameNode
{
protected:

	bool m_bDebug;
	int m_speed, m_count;

	image* m_backBuffer;

	//로드
	HANDLE file1, file2;
	DWORD read;

	stObject m_object[df_MAXOBJECT];			//숫자 계산 오브젝트
	stTile m_tiles[df_TILEX* df_TILEY];			//타일

public:
	Scene() {};
	~Scene() {};
};

