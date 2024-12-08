#pragma once
#include "singletonBase.h"

/*

	INI파일은 설정을 내보거나 읽어올때 혹은 초기정보를 읽어올때 쓰이는 파일 포맷
*/
struct tagIniData
{
	char* section;
	char* key;
	char* value;
};

class iniData : public singletonBase <iniData>
{
private:
	typedef vector<tagIniData> arrIniData;

	typedef vector<arrIniData> arrIniDatas;

private:
	arrIniDatas _vIniData;

public:
	iniData();
	~iniData();

	HRESULT init();

	//데이터 추가하기(섹션,키,벨류)
	void addData(char* section, char* key, char* value);
	//세이브 -> 파일이름만 넣어도 자동 저장
	void iniSave(char* fileName);

	//문자값 로드
	char* loadDataString(char* fileName, char* section, char* key);
	//정수값 로드
	int loadDataIntrger(char* fileName, char* section, char* key);
};

