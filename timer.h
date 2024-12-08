#pragma once
class timer
{
private:
	bool _isHardware;	//고성능 타이머를 지원하냐?
	float _timeScale;	//시간 경과량
	float _timeElapsed;	//한 프레임당 경과량
	__int64 _curTime;	//현재시간
	__int64 _lastTime;	//마지막시간
	__int64 _periodFrequency;	//시간 주기

	unsigned long _frameRate;	//FPS
	unsigned long _FPSFrameCount;	//FPS 카운트
	float _FPStimeElapsed;		//FPS 마지막시간과 현재 시간의 경과량
	float _worldTime;	//전체 시간 경과량

public:
	timer();
	~timer();

	//타이머 초기화
	HRESULT  init();

	//현재 시간 계산
	void tick(float lockFPS = 0.0f);
	//현재 FPS 가져오기
	unsigned long getFrameRate(char* str = nullptr) const;
	//한프레임당 경과 시간
	float getElapsedTime() const { return _timeElapsed; }
	//전체 경과 시간 가져오기
	float getWorldTime() const { return _worldTime; }
};

