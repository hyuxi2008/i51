
#ifndef _I51KITG1_H_

#include "i51.h"

#define EFFECT_MAXBLADEWIDTH	4		//���������(һ��)
#define EFFECT_MAXKNIFEPOINT    12		//���ⳤ
#define EFFECT_MAXTRACKPOINT	6		//�켣������������
#define EFFECT_MINDISTANCE		144		//�켣�����С�����ƽ��
#define EFFECT_MAXPOINTNUM		5

extern iS16 se_TrackPointCount;
extern iS16 se_TrackPoint[EFFECT_MAXTRACKPOINT][2];

extern iBOOL i51KitG1Draw(void);

#endif

