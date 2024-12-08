#include "stdafx.h"
#include "UI.h"

HRESULT UI::init()
{
	IMAGEMANAGER->addImage("프레임", "images/ui/frame.bmp", 720, 720, true);

	IMAGEMANAGER->addFrameImage("ui0", "images/ui/ui0.bmp", 29 * 2, 174 * 2, 1, 6, true);
	IMAGEMANAGER->addFrameImage("ui1", "images/ui/ui1.bmp", 89 * 2, 174 * 2, 1, 6, true);
	IMAGEMANAGER->addFrameImage("ui2", "images/ui/ui2.bmp", 186 * 2, 174 * 2, 1, 6, true);

	//점수
	IMAGEMANAGER->addFrameImage("숫자", "images/ui/number.bmp", 50 * 2, 9 * 2, 10, 1, true);

	//타임오버
	m_blackScreen = IMAGEMANAGER->addImage("검은화면", "images/ui/black.bmp", WINSIZEX, WINSIZEY, true);
	m_timeOver = IMAGEMANAGER->addFrameImage("타임오버", "images/ui/time_over.bmp", 134 * 2, 128 * 2, 1, 8, true);

	//스테이지 클리어
	m_stageClear = IMAGEMANAGER->addFrameImage("스테이지클리어", "images/ui/stage_clear.bmp", 1200, 400, 2, 1, true);
	IMAGEMANAGER->addImage("스테이지언락", "images/ui/level_unlock.bmp", 600, 400, true);

	m_bTimeOver = false;
	m_fadeCount = 0;
	m_count = m_index = 0;

	m_bStageClear = false;

	//타임 초기화
	m_timeBar = new progressBar;
	m_timeBar->init("images/ui/time.bmp", "images/ui/timeBack.bmp", 306, 50, 186 * 2, 29 * 2);

	m_timeBar->setGauge(0, 0);

	//로딩바 게이지 초기화
	m_iCurrentGauge = 952;

	//시간
	m_fTime = 0;

	return S_OK;
}

void UI::release()
{
	SAFE_DELETE(m_timeBar);
}

void UI::update()
{
	score();
	if (m_bTimeOver) timeOver();
}

void UI::render(enCHARACTER_TYPE CHAR_TYPE)
{
	IMAGEMANAGER->render("프레임", getMemDC(), 690, 20);
	
	IMAGEMANAGER->frameRender("ui0", getMemDC(), 50, 50, 0, CHAR_TYPE);
	IMAGEMANAGER->frameRender("ui1", getMemDC(), 118, 50, 0, CHAR_TYPE);
	IMAGEMANAGER->frameRender("ui2", getMemDC(), 306, 50, 0, CHAR_TYPE);
	
	//로딩바
	m_timeBar->render();
	
	//점수
	for (int i = UNITS; i <= TEN_THOUSANDS; i++)
	{
		IMAGEMANAGER->frameRender("숫자", getMemDC(), 280 - 12 * i, 74, m_iScore[i], 0);
	}

	//타임오버
	if (m_bTimeOver)
	{
		m_blackScreen->alphaRender(getMemDC(), m_fadeCount);
		m_timeOver->frameRender(getMemDC(), WINSIZEX / 2 - 134, WINSIZEY / 2 - 16, 0, m_index);
	}
}

void UI::timer() //시간
{
	m_timeBar->update();
	m_timeBar->setGauge(m_iCurrentGauge, 1000);

	m_fTime += TIMEMANAGER->getElapsedTime();

	if (m_fTime >= 3.0f)
	{
		m_fTime = 0;
		if (!m_bTimeOver && m_iCurrentGauge > 240) m_iCurrentGauge -= 16;
		else m_bTimeOver = true;
	}
}

void UI::score() //점수
{
	m_iScore[TOTAL_SCORE] = g_iScore;

	int j = 1;
	for (int i = UNITS; i <= TEN_THOUSANDS; i++)
	{
		j *= 10;
		m_iScore[i] = (m_iScore[TOTAL_SCORE] % j / (0.1 * j));
	}
}

void UI::timeOver() //타임 오버
{
	if (m_fadeCount == 0) SOUNDMANAGER->play("타임오버", 1.0f);
	if (m_fadeCount < 250) m_fadeCount += 5;

	m_count++;
	if (m_count >= 10)
	{
		m_count = 0;
		m_index++;
		if (m_index > m_timeOver->getMaxFrameY()) m_index = 0;
		m_timeOver->setFrameY(m_index);
	}

	if (m_fadeCount >= 250 && KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		SOUNDMANAGER->effectPlay("선택효과음");
		SCENEMANAGER->changeScene("타이틀");
	}
}

void UI::stageClear()
{
	m_blackScreen->alphaRender(getMemDC(), 127);
	m_stageClear->frameRender(getMemDC(), WINSIZEX / 2 - 300, WINSIZEY / 2 - 200);
	
	//스테이지 언락
	if (g_stageLock[g_stage + 1] && g_stage + 1 <= STAGE3)
	{
		IMAGEMANAGER->render("스테이지언락", getMemDC(), WINSIZEX / 2 - 300, WINSIZEY / 2 - 200);
	}
}
