#pragma once
#include "singletonBase.h"

class txtData :public singletonBase <txtData>
{
public:
	txtData();
	~txtData();

	HRESULT init();

	//���̺�
	void txtSave(char* saveFileName, vector<string> vStr);
	char* vectorArrayCombine(vector<string> vArray);

	//�ε�
	//���ٿ� ���� ������
	vector<string> txtLoad(char* loadFileName);
	//�����͵��� ����Ʈ
	vector<string> charArraySeparation(char charArray[]);

};

