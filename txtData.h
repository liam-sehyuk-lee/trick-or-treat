#pragma once
#include "singletonBase.h"

class txtData :public singletonBase <txtData>
{
public:
	txtData();
	~txtData();

	HRESULT init();

	//세이브
	void txtSave(char* saveFileName, vector<string> vStr);
	char* vectorArrayCombine(vector<string> vArray);

	//로드
	//한줄에 넣을 데이터
	vector<string> txtLoad(char* loadFileName);
	//데이터들의 리스트
	vector<string> charArraySeparation(char charArray[]);

};

