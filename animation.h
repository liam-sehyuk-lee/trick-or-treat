#pragma once
class animation
{
private:
	typedef vector<POINT> _vFrameList;
	typedef vector<int> _vPlayList;		//프레임 플레이 목록

	int _frameNum;						//프레임 갯수
	_vFrameList _frameList;				//프레임 위치목록
	_vPlayList _playList;				//플레이 목록

	int _frameWidth;					//프레임 가로크기					
	int _frameHeight;					//프레임 세로크기
	bool _loop;


	float _frameUpdateSec;				//프레임 업데이트 
	float _elapsedSec;					//프레임 경과시간
	DWORD _nowPlayIndex;				//현재 플레이 인덱스
	bool _play;							//플레이 여부


public:
	animation();
	~animation();

	//초기화(이미지 전체가로 크기, 세로크기, 한프레임 가로, 세로 크기)
	HRESULT init(int totalW, int totalH, int frameW, int frameH);

	//처음부터 끝까지 애니메이션 셋팅
	void setDefPlayFrame(bool reverse = false, bool loop = false);
	//배열에 담겨져 있는 것만
	void setPlayFrame(int* playArr, int arrLen, bool loop = false);

	//시작과 끝구간을 정해서
	void setPlayFrame(int start, int end, bool reverse = false, bool loop = false);
	//초당 갱신횟수
	void setFPS(int framePerSec);

	//프레임 업데이트 타임
	void frameUpdate(float elapsedTime);

	void start();//시작
	void stop();//정지
	void pause();//일시정지
	void resume();//다시

	inline bool isPlay() { return _play; }
	inline POINT getFramePos() { return _frameList[_playList[_nowPlayIndex]]; }
	inline int getFrameWidth() { return _frameWidth; }
	inline int getFrameHeight() { return _frameHeight; }

};

