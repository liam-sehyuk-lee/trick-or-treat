#pragma once

#define DEG_TO_RAD 0.017435 //디그리 1도(라디안값)

#define PI 3.141592654f
#define PI2 PI * 2


#define PI_2	(PI / 2)
#define PI_4	(PI / 4)
#define PI_8	(PI / 8)
#define PI_16	(PI / 16)
#define PI_32	(PI / 32)

#define PI_12	(PI / 12)
namespace UTIL
{

	//거리 가져오는 함수
	float getDistance(float startX, float startY, float endX, float endY);

	//각도 가져오는 함수
	float getAngle(float x1, float y1, float x2, float y2);



}

