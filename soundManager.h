#pragma once
#include "singletonBase.h"
#include "fmod.hpp"
#if defined(_DEBUG)
#pragma comment(lib, "fmodL_vc.lib")	// Debug 빌드 시
#else
#pragma comment(lib, "fmod_vc.lib")		// Release 빌드 시
#endif

using namespace FMOD;

/*
FMOD
사운드를 플레이 하기 위해 도와주는 녀석
예전에 만들어져서 개발이 중단되었으나 요즘에도
많이 사용됨. 간단하게 사용할 수 있고 지원하는
확장자가 많아서 자주사용함

사운드 편집가능 :  fmod studio 사이트 가보면 확인 가능

*/
//채널버퍼와 사운드 버퍼
#define EXTRACHANNELBUFFER 5
#define SOUNDBUFFER 100


//총 사운드 버퍼 :  버퍼가 작아서 씹히는 문제를 해결하기 위함.
#define TOTALSOUNDBUFFER SOUNDBUFFER + EXTRACHANNELBUFFER

class soundManager : public singletonBase<soundManager>
{

private:
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundIter;


	System* m_system;	//fmod의 시스템 클래스임.


						//더블포인터를 쓰는건 사운드로 들어가는 인자가 더블포인터로 되어있다.
	Sound** m_sound;	//사운드클래스
	Channel** m_channel;//채널 클래스

	arrSounds m_totalSounds; //맵에 담긴 사운드 목록

public:
	soundManager();
	~soundManager();


	HRESULT init();
	void release();
	void update();
	void render();

	//사운드 추가(키값, 파일이름, BGM, LOOP)
	void addSound(string keyName, string soundName, bool bgm = FALSE, bool loop = FALSE);

	//사운드 플레이(키값, 볼륨 0.0f~1.0f)
	void play(string keyName, float volume = 1.0f);
	//정지
	void stop(string keyName);
	//일시정지
	void pause(string keyName);
	//다시재생
	void resume(string keyName);

	//플레이 중임?
	bool isPlaySound(string keyName);
	//일시정지 중임?
	bool isPauseSound(string keyName);

	//사운드 이펙트
	void effectPlay(string keyName, float volume = 1.0f);

	void addAll();
	void stopAll();
};

