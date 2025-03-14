// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>


#include<iostream>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include<commdlg.h> //오빨넴용(OpenFileName)

//STL
#include<string>
#include<map>
#include<vector>


//====================================
//	## 내가 만든 헤더파일 이곳에 추가 ## 
//====================================
#include"commonMacroFunction.h"
#include"keyManager.h"
#include"imageManager.h"
#include"timeManager.h"
#include"sceneManager.h"
#include"soundManager.h"
#include"animationManager.h"
#include"effectManager.h"
#include"randomFunction.h"
#include"utils.h"
#include"txtData.h"
#include"iniData.h"

using namespace UTIL;
using namespace std;
//====================================
//	## 싱글톤 추가 ## 
//====================================

#define RND						randomFunction::getSingleton()
#define KEYMANAGER				keyManager::getSingleton()
#define IMAGEMANAGER			imageManager::getSingleton()
#define TIMEMANAGER             timeManager::getSingleton()
#define SCENEMANAGER			sceneManager::getSingleton()
#define SOUNDMANAGER			soundManager::getSingleton()
#define ANIMATIONMANAGER		animationManager::getSingleton()
#define EFFECTMANAGER			effectManager::getSingleton()
#define TXTDATA					txtData::getSingleton()
#define INIDATA					iniData::getSingleton()
//====================================
//	## 디파인문 ## (윈도우 초기화 셋팅) 
//====================================

#define WINNAME (LPTSTR)(TEXT("경일 프로그래밍 14기"))
#define WINSTARTX 100
#define WINSTARTY 100
#define WINSIZEX 1500
#define WINSIZEY 768
#define WINSTYLE WS_CAPTION | WS_SYSMENU

//==================================================
//	## 매크로 함수 ## (메인게임의 릴리즈 부분에서 사용) 
//==================================================

#define SAFE_DELETE(p)	{if(p) {delete(p); (p) = nullptr;}}
#define SAFE_RELEASE(p)	{if(p) {(p)->release(); (p) = nullptr;}}

//==================================================
//	## 전역변수 ## (인스턴스, 윈도우 핸들, 마우스좌표) 
//==================================================
extern HINSTANCE g_hInstance;
extern HWND g_hWnd;
extern POINT g_ptMouse;

extern enSTAGE g_stage;
extern bool g_stageLock[df_MAXSTAGE];
extern int g_iScore;

//==================================================
//	## 디파인문 ##
//==================================================

//스테이지 타일 사이즈
#define df_STAGE_TILE_SIZEX 48							
#define df_STAGE_TILE_SIZEY 48	

//스테이지 전체 사이즈
#define df_STAGE_SIZEX (df_STAGE_TILE_SIZEX * df_TILEX)	
#define df_STAGE_SIZEY (df_STAGE_TILE_SIZEY * df_TILEY)

//카메라 사이즈
#define df_CAMERA_SIZEX 700								
#define df_CAMERA_SIZEY 700	

//==== 맵툴용 ===============================================================================
#define df_TILE_SIZE 24				//타일 사이즈
#define df_TILE_SIZEX (df_TILEX * df_TILE_SIZE)
#define df_TILE_SIZEY (df_TILEY * df_TILE_SIZE)

//샘플 타일 위치조정
#define df_TERRAIN_POSX 170
#define df_TERRAIN_POSY 50

#define df_OBJ_POSX 310
#define df_OBJ_POSY 200

#define df_OBJ_TILEX 5
#define df_OBJ_TILEY 4

#define df_TERRAIN_TILE_SIZEX (df_TILE_SIZE * df_TERRAIN_TILEX)
#define df_TERRAIN_TILE_SIZEY (df_TILE_SIZE * df_TERRAIN_TILEY)

#define df_OBJ_TILE_SIZEX (df_TILE_SIZE * df_OBJ_TILEX)
#define df_OBJ_TILE_SIZEY (df_TILE_SIZE * df_OBJ_TILEY)

//빈 타일
#define df_EMPTY 88

//타일좌표 x,y
#define df_TILE(x,y) (frameX == x && frameY == y)
#define df_TILE_FROM_TO(x1,y1,x2,y2) (x1 <= frameX && frameX <= x2 && y1 <= frameY && frameY <= y2)
#define df_CUR_TILE(x1,y1,x2,y2) m_currentTile.frameX1 = x1; m_currentTile.frameY1 = y1; m_currentTile.frameX2 = x2; m_currentTile.frameY2 = y2

//디버그RECT
#define df_DEBUG_RECT(z,x1,y1,x2,y2) m_tiles[i].rcDebug[z] = RectMake(m_tiles[i].rc.left + x1, m_tiles[i].rc.top + y1, x2, y2)
#define df_STAGE_RECT(z,x1,y1,x2,y2) m_tiles[i].rcDebug[z] = RectMake(m_tiles[i].rc.left*2 + x1, m_tiles[i].rc.top*2 + y1, x2, y2)

//플레이어
#define df_MAXCHAR 5

#define df_MAXSTAGE 4

//==============================================================================================

//==================================================
//	## 이넘문 ##
//==================================================

//맵툴용
enum enSTAGE { STAGE1, STAGE2, STAGE3, STAGE4, STAGE_NONE };
enum enTILEMAP { MAP_TERRAIN, MAP_OBJECT };
enum enTERRAIN { TR_NONE, TR_TOP, TR_LEFT, TR_RIGHT, TR_LT, TR_RT, TR_LR, TR_ALL,
				 TR_HOUSE, TR_ECT, TR_SKU, TR_FRK, TR_FISH_NONE, TR_FISH_ALL};
enum enHOUSE { HOUSE_NONE, HOUSE1, HOUSE2, HOUSE3, HOUSE4, HOUSE5 };
enum enFLYABLE { FLY_NO, FLY_YES };
enum enOBJECT { OBJ_NONE, OBJ_SKULL, OBJ_DRACULA, OBJ_FRANKEN, OBJ_WITCH, OBJ_FISHMAN,
				OBJ_COIN, OBJ_CANDY, OBJ_SUPER_COIN, OBJ_GHOST, OBJ_NPC0, OBJ_NPC1, OBJ_NPC2, OBJ_NPC3, OBJ_HOUSE };

//플레이어
enum enPOSITION { POS_FRONT, POS_BACK, POS_LEFT, POS_RIGHT };
enum enANIMATION_STATE { ANI_IDLE, ANI_MOVE, ANI_SPELL, ANI_DIE };
enum enCHARACTER_TYPE { SKULL, DRACULA, FRANKEN, WITCH, FISHMAN };
enum enCHARACTER_STATE { CHAR_IDLE, CHAR_SPELL };

//오브젝트
enum enINGAME_ITEM { IN_CANDY, IN_COIN, IN_SUPER_COIN, IN_ICON };
enum enINGAME_NPC { IN_NPC0, IN_NPC1, IN_NPC2, IN_NPC3 };
enum enLOCK { LOCK_NONE, LOCK1, LOCK2 };

//이펙트
enum enEFFECT { EFT_NONE, EFT_SKULL, EFT_WITCH, EFT_FISH };

//스테이지 선택
enum enSTAGE_LOCK { ST_OPENED, ST_SELECTED, ST_LOCKED };

//점수 계산
enum enSCORE { TOTAL_SCORE, UNITS, TENS, HUNDREDS, THOUSANDS, TEN_THOUSANDS };

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
