#include "stdafx.h"
#include "soundManager.h"


soundManager::soundManager()
	:m_system(nullptr)
	, m_channel(nullptr)
	, m_sound(nullptr)
{
}


soundManager::~soundManager()
{
}

HRESULT soundManager::init()
{
	//사운드 시스템 생성
	System_Create(&m_system);

	//사운드 채널수 생성
	m_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//사운드 채널 할당
	m_sound = new Sound *[TOTALSOUNDBUFFER];
	m_channel = new Channel *[TOTALSOUNDBUFFER];


	memset(m_sound, 0, sizeof(Sound*)*(TOTALSOUNDBUFFER));
	memset(m_channel, 0, sizeof(Channel*)*(TOTALSOUNDBUFFER));

	//음악 init
	addAll();

	return S_OK;
}

void soundManager::release()
{
	if (m_channel != nullptr | m_sound != nullptr)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (m_channel != nullptr)
			{
				if (m_channel[i])m_channel[i]->stop();
			}
			if (m_sound != nullptr)
			{
				if (m_sound[i])m_sound[i]->release();
			}
		}
	}

	SAFE_DELETE(m_channel);
	SAFE_DELETE(m_sound);

	//시스템을 닫자
	if (m_system != nullptr)
	{
		m_system->release();
		m_system->close();
	}

	/*
	참고
	fmod에서 3d를 쓰고 싶으면 이런거 설정하면 됨.

	3d음향 거리값 설정 가능
	m_channel[0]->set3DminMaxDistance()

	3d 음향 위치값 설정 가능
	m_channel[0]->set3DCustomRolloff()

	*/
}

void soundManager::update()
{
	m_system->update();
}

void soundManager::render()
{
}

void soundManager::addSound(string keyName, string soundName, bool bgm, bool loop)
{
	if (loop)
	{
		//브금
		if (bgm)
		{
			m_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_sound[m_totalSounds.size()]);
		}

		//이펙트
		else
		{
			m_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_sound[m_totalSounds.size()]);
		}
	}

	else
	{
		m_system->createSound(soundName.c_str(), FMOD_DEFAULT, nullptr, &m_sound[m_totalSounds.size()]);
	}

	m_totalSounds.insert(make_pair(keyName, &m_sound[m_totalSounds.size()]));
}

void soundManager::play(string keyName, float volume)
{
	stopAll();

	int count = 0;
	arrSoundIter iter = m_totalSounds.begin();
	//채널에서 플레이 해야하니 카운트 돌리자.
	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_system->playSound(*iter->second, nullptr, false, &m_channel[count]);
			m_channel[count]->setVolume(volume);
		}
	}
}

void soundManager::stop(string keyName)
{
	int count = 0;
	arrSoundIter iter = m_totalSounds.begin();
	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->stop();
			break;
		}
	}
}

void soundManager::pause(string keyName)
{
	int count = 0;
	arrSoundIter iter = m_totalSounds.begin();

	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->setPaused(true);
			break;
		}
	}
}

void soundManager::resume(string keyName)
{
	int count = 0;
	arrSoundIter iter = m_totalSounds.begin();

	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->setPaused(false);
			break;
		}
	}
}

bool soundManager::isPlaySound(string keyName)
{
	int count = 0;
	bool isPlay;

	arrSoundIter iter = m_totalSounds.begin();

	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool soundManager::isPauseSound(string keyName)
{
	int count = 0;
	bool isPause;

	arrSoundIter iter = m_totalSounds.begin();

	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_channel[count]->getPaused(&isPause);
			break;
		}
	}

	return isPause;
}

//사운드 이펙트
void soundManager::effectPlay(string keyName, float volume)
{
	int count = 0;
	arrSoundIter iter = m_totalSounds.begin();
	//채널에서 플레이 해야하니 카운트 돌리자.
	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_system->playSound(*iter->second, nullptr, false, &m_channel[count]);
			m_channel[count]->setVolume(volume);
		}
	}
}

void soundManager::addAll()
{
	//bgm
	addSound("인게임음악", "sound/ingame_bgm.mp3", true, true);
	addSound("맵툴음악", "sound/maptool_bgm.mp3", true, true);

	addSound("타임오버", "sound/time_over.mp3", true, false);

	//sound effect
	addSound("캐릭터1_효과음", "sound/char1_skill.mp3", false, false);
	addSound("캐릭터2_효과음", "sound/char2_skill.mp3", false, false);
	addSound("캐릭터3_효과음", "sound/char3_skill.mp3", false, false);
	addSound("캐릭터4_효과음", "sound/char4_skill.mp3", false, false);
	addSound("캐릭터5_효과음", "sound/char5_skill.mp3", false, false);

	addSound("커서효과음", "sound/cursor.mp3", false, false);
	addSound("선택효과음", "sound/select.mp3", false, false);

	addSound("사탕", "sound/candy.mp3", false, false);
	addSound("동전", "sound/coin.mp3", false, false);
	addSound("동전들", "sound/coins.mp3", false, false);
	addSound("아이콘", "sound/alter.mp3", false, false);
}

void soundManager::stopAll()
{
	stop("인게임음악");
	stop("맵툴음악");
	stop("타임오버");
}