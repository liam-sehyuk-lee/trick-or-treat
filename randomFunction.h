#pragma once
#include"singletonBase.h"
class randomFunction : public singletonBase<randomFunction>
{

	public:
	randomFunction()
	{
		srand(GetTickCount());
	}
	~randomFunction()
	{

	}

	//지정한 숫자범위내에서 난수를 받아온다 0 ~ 지정한숫자
	inline int getInt(int num) { return rand() % num; }

	//      특정숫자 범위내에서 난수를 뽑아온다 예) 5 ~ 10       
	inline int getFromIntTo(int fromNum, int toNum)
	{
		return rand() % (toNum - fromNum + 1) + fromNum;
	}
	inline float getFloat(float num)
	{
		return ((float)rand() / (float)RAND_MAX) * num;
	}
	inline float getFromFloatTo(float fromNum, float toNum)
	{
		float rnd = (float)rand() / (float)RAND_MAX;
		return (rnd * (toNum - fromNum) + fromNum);
	}
};
