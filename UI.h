#pragma once
#include "Object.h"
#include"progressBar.h"

#define MAXDIGIT 6

class UI : public Object
{
private:

	image* m_blackScreen, *m_timeOver, *m_stageClear;
	BYTE m_fadeCount;

	bool m_bTimeOver, m_bStageClear;
	int m_count, m_index;

	progressBar* m_timeBar;
	int m_iCurrentGauge;

	float m_fTime;
	int m_iScore[MAXDIGIT];

public:

	HRESULT init();
	void release();
	void update();
	void render(enCHARACTER_TYPE CHAR_TYPE);

	void timer();
	void score();
	void timeOver();
	void stageClear();

	bool getTimeOver() { return m_bTimeOver; }
	void setCoinClear() { m_stageClear->setFrameX(0); }
	void setAllClear() { m_stageClear->setFrameX(1); }

	//타임 보너스
	int getTimeBonus() { return m_iCurrentGauge / 10; }

public:
	UI() {};
	~UI() {};
};

