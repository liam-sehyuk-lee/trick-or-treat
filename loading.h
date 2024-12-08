#pragma once
#include"gameNode.h"
#include"progressBar.h"

//로드 아이템의 종류
enum LOAD_KIND
{
	LOAD_KIND_IMAGE_0,	//빈 비트맵	이미지
	LOAD_KIND_IMAGE_1,	//일반 이미지
	LOAD_KIND_IMAGE_2,	//일반이미지(위치좌표있음)
	LOAD_KIND_FRAMEIMAGE_0,	//프레임
	LOAD_KIND_FRAMEIMAGE_1,	//플레임(위치 좌표 있음)
	LOAD_KIND_SOUND,
	LOAD_KIND_END
};
//이미지 리소스 구조체
struct tagImageResource
{
	string keyName;			//키값
	const char* fileName;	//파일이름
	int x, y;				//위치좌표
	int width, height;		//가로세로 크기
	int frameX, frameY;		//프레임 xy
	bool trans;				//배경 없앨꺼니
	COLORREF transColor;
};

class loadItem
{
private:

	LOAD_KIND _kind;
	tagImageResource _imageResource;

public:
	loadItem() {}
	~loadItem() {}

	//키 값으로 빈 비트맵 초기화
	HRESULT initForImage(string keyName, int width, int height);

	//키값으로 이미지 파일 초기화
	HRESULT initForImage(string keyname, const char* fileName, int width, int height, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
	HRESULT initForImage(string keyName, const char* fileName, float x, float y, int width, int height, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	//키값으로 프레임 이미지 파일 초기화
	HRESULT initForFrameImage(string keyName, const char* fileName, int width, int height, int frameX, int frameY, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
	HRESULT initForFrameImage(string keyName, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	//로딩 아이템 종류를 가져와라
	LOAD_KIND getLoadingKind() { return _kind; }

	//이미지 리소스를 가져오자
	tagImageResource getImageResource() { return _imageResource; }
};

class loading :  public gameNode
{
private:
	typedef vector<loadItem*> arrLoadItem;

	arrLoadItem _vLoadItem;
	//로딩화면에서 사용할 이미지 및 로딩바
	image* _background;
	//로딩화면에서 사용할 녀석
	progressBar* _loadingBar;
	//로딩바 게이지
	int _currentGauge;

public:
	loading();
	~loading();

	HRESULT init();
	void release();
	void update();
	void render();

	void loadImage(string keyName, int width, int height);

	void loadImage(string keyName, const char* fileName, int width, int height, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
	void loadImage(string keyName, const char* fileName, float x, float y, int width, int height, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	void loadFrameImage(string keyName, const char* fileName, int width, int height, int frameX, int frameY, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));
	void loadFrameImage(string keyName, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, BOOL isTrans = FALSE, COLORREF transColor = RGB(0, 0, 0));

	//로딩완료 됐냐?(로딩 완료후 씬변경)
	BOOL loadingDone();

	//로드 아이템 가져오자
	vector<loadItem*>getLoadItem() { return _vLoadItem; }

};

