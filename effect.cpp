#include "stdafx.h"
#include "effect.h"

effect::effect()
	:_effectImage(nullptr)
	, _effectAni(nullptr)
	, _isRunnig(NULL)
	, _x(0)
	, _y(0)
{
}

effect::~effect()
{
}

HRESULT effect::init(image * effectImage, int frameW, int frameH, int FPS, float elapsedTime)
{
	//이펙트 이미지가 없을때 바로 리턴
	if (!effectImage) return E_FAIL;

	_effectImage = effectImage;
	_elapsedTime = elapsedTime;
	_isRunnig = false;

	//이펙트 애니메이션 없으면 새로 생성!
	if (!_effectAni) _effectAni = new animation;
	//애니메이션 정보 초기화
	_effectAni->init(_effectImage->getWidth(), _effectImage->getHeight(), frameW, frameH);
	_effectAni->setDefPlayFrame();
	_effectAni->setFPS(FPS);

	return S_OK;
}

void effect::release(void)
{
	SAFE_DELETE(_effectAni);
}

void effect::update(void)
{
	if (_isRunnig)
	{
		_effectAni->frameUpdate(_elapsedTime);
	}

	//이펙트 애니메이션이 종료가 되면 멈추자ㅏ
	if (!_effectAni->isPlay()) stopEffect();
}

void effect::render(void)
{
	if (_isRunnig)
	{
		_effectImage->aniRender(getMemDC(), _x, _y, _effectAni);
	}
}

void effect::startEffect(int x, int y)
{
	//위치 좌표를 선터 좌표로 만들기
	_x = x - (_effectAni->getFrameWidth() / 2);
	_y = y - (_effectAni->getFrameHeight() / 2);

	_isRunnig = true;
	_effectAni->start();

}

void effect::stopEffect()
{
	_isRunnig = false;
}
