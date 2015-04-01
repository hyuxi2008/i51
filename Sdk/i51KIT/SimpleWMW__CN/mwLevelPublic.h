
#ifndef _MW_LEVEL_PUBLIC_H_
#define _MW_LEVEL_PUBLIC_H_

#include "i51Public.h"
#include "mwPublic.h"
#include "mwLevelChoose.h"
#include "mwLevel001.h"
#include "mwLevel002.h"
#include "mwLevel003.h"
#include "mwLevel004.h"
#include "mwLevel005.h"
#include "mwLevel006.h"
#include "mwLevel007.h"
#include "mwLevel008.h"
#include "mwLevel009.h"
#include "mwLevel010.h"
#include "mwLevel011.h"
#include "mwLevel012.h"
#include "mwLevel013.h"
#include "mwLevel014.h"
#include "mwLevel015.h"

typedef enum
{
	MW_LEVEL_IDLE = 0,
	MW_LEVEL_INIT,
	MW_LEVEL_RUN,
	MW_LEVEL_WIN,
	MW_LEVEL_RESULT,
	MW_LEVEL_PAUSE,
	MW_LEVEL_RESUME,
	MW_LEVEL_EXIT	
}mwLevelState;

typedef enum
{
	MW_LEVEL_CHOOSE = 0,
	MW_LEVEL_001,
	MW_LEVEL_002,
	MW_LEVEL_003,
	MW_LEVEL_004,
	MW_LEVEL_005,
	MW_LEVEL_006,
	MW_LEVEL_007,
	MW_LEVEL_008,
	MW_LEVEL_009,
	MW_LEVEL_010,
	MW_LEVEL_011,
	MW_LEVEL_012,
	MW_LEVEL_013,
	MW_LEVEL_014,
	MW_LEVEL_015,
	MW_LEVEL_END
}mwLevelNum;

#define MW_LEVEL_MAX	MW_LEVEL_008

#define TOTAL_DUCK_NUM	24
#define TOTAL_LEVEL		8 //xie



// ��ǰ�ǵڼ��صı��
extern mwLevelNum CurrLevelNum;
// ��ǰ�ؿ���״̬�ı��
extern mwLevelState CurrLevelState;
//// ��ǰ����ı�ǣ���¼ÿһ�����ص������
//extern iU8 mwWorldAttribute[SCREEN_HEIGHT_HALF][SCREEN_WIDTH_HALF];
// ��ǰ����ı�ǣ���¼ÿһ�����ص������
extern iU8 ** mwWorldAttribute;
// ÿ�صõ���Ѽ����
extern iU8 LevelDuckNum[TOTAL_LEVEL];
// �Ѿ������Ĺؿ���
extern iU32 UnlockMaxLevel;
// ��ǰ�ܵõ���Ѽ����
extern iU32 CurrTotalDuck;
// �ؿ���ͣ�ı��
extern iU8 mwWordPause;
// ���ֿ��صı��
extern iU8 MusicCloseFlag;
// ��Ч���صı��
extern iU8 SoundCloseFlag;
// �ؿ���ʼ��ʱ��
extern iU32 TimeLevelStart;
// �ؿ���ǰ��ʱ��
extern iU32 TimeLevelCurr;
// ��Ļbuff����
extern iU16 *TempScreenBuf;
// ����ȴ�ˮ�������ṹ
extern AnimationStr CrocodileWait;
// ����ϴ�趯���ṹ
extern AnimationStr CrocodileBathe;
// ����״̬,Ϊ�������к���Ϊ0���޺���
//extern iU8  AlgaeMess[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength];
// �ؿ����Ƿ��������ı��
extern iU8  AlgaeNeed ;
// ���ؾ�����Ϣ
extern OrganData OrganMess[MAX_ORGAN];
// ���ع�����
extern OrganManage Organ_Manage;

extern iU8 InitFlag;


extern iBOOL mwStateGame( void );
extern iBOOL mwInitSpace( mwLevelNum level);
extern iBOOL mwLevelExit( mwLevelNum Level);
extern iBOOL mwLevelInit( mwLevelNum Level);
extern iBOOL mwLevelPause( mwLevelNum Level);
extern iBOOL mwLevelResume( mwLevelNum Level);
extern iBOOL mwLevelRun( mwLevelNum level);
extern iBOOL mwLevelWin( mwLevelNum level);
extern iBOOL mwLevelResult( mwLevelNum level);
extern iBOOL mwDrawNumStep( iHANDLE Container, iU32 ImageID, iRECT * DesRect, iU32 Number1, iU32 Number2 );

extern iBOOL mwLevelDrawRestart( void );
extern iBOOL mwLevelPressRestart( iU16 X, iU16 Y);
extern iBOOL mwLevelDrawPause( void );
extern iBOOL mwLevelPressPause( iU16 X, iU16 Y);
extern iBOOL mwLevelDrawBKwall( void );
extern iBOOL mwLevelDrawReturn( void );
extern iBOOL mwLevelPressReturn( iU16 X, iU16 Y);
extern iBOOL mwEarthCutting( mwLevelNum level );
extern iBOOL mwLevelWordInit( mwLevelNum level );
extern iBOOL mwLevelDuckInit(mwLevelNum level );
extern iBOOL mwLevelPipeInit( mwLevelNum level  );
extern iBOOL mwLevelDrawPrompt( mwLevelNum level );
extern iBOOL cpWordDraw( void );
extern iBOOL mwLevelRedrawBG( void );
extern iBOOL mwLevelRedrawFG( void );
extern iBOOL mwLevelDrawDuck( void  );
extern iBOOL mwTempScreenBufUpdate( mwLevelNum level );
extern iBOOL mwLevelWaitInit( mwLevelNum level );
extern iBOOL mwLevelBatheInit( mwLevelNum level );
extern iBOOL mwLevelAlgaeInit( mwLevelNum level );
extern iBOOL mwLevelWaterSpray( mwLevelNum level );
extern iBOOL mwLevelOrganInit( mwLevelNum level  );
extern iBOOL mwLevelDrawOrgan( void  );
extern iBOOL mwLevelSpeedKeepInit( mwLevelNum level );
extern iBOOL mwLevelSetImageMem( mwLevelNum level );
extern iBOOL mwLevelUnsetImageMem( mwLevelNum level );

//�������� ����
extern SpeedKeep SpeedKeepMess[];
// �������������
extern SpeedKeepManage SpeedKeep_Manage;

// Ѽ������
extern DuckData DuckMess[];
// Ѽ�ӹ�����
extern DuckManage Duck_Manage;

// ���ؾ�����Ϣ
extern OrganData OrganMess[];
// ���ع�����
extern OrganManage Organ_Manage;

// ˮ�ܾ�����Ϣ
extern PipeData PipeMess[];
// ˮ�ܹ�����
extern PipeManage Pipe_Manage;
// ԡ��������ˮ������
extern iS16 WaterInBathtub;
// ���ʱ�佱���ķ���
extern iS16 CurrTimeWin;
// ���Ѽ�ӽ����ķ���
extern iS16 CurrDuckWin;
// �����ˮ�����ķ���
extern iS16 CurrWaterWin;
// �ؿ���õ��ܷ�
extern iS16 CurrTotalWin;

// ��ǰ�ǵڼ��صı��
extern mwLevelNum CurrLevelNum ;
// ��ǰ�ؿ���״̬�ı��
extern mwLevelState CurrLevelState;
// �ؿ���ͣ�ı��
extern iU8 mwWordPause;
// ��Ч���صı��
extern iU8 SoundCloseFlag;
// ���ֿ��صı��
extern iU8 MusicCloseFlag;
// �ؿ���ʼ��ʱ��
extern iU32 TimeLevelStart;
// �ؿ���ǰ��ʱ��
extern iU32 TimeLevelCurr;
//// ��Ļbuff����
//extern iU16 TempScreenBuf[SCREEN_HEIGHT][SCREEN_WIDTH];


// ��Ļ���������
extern iU8  GriddingMud[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength];
// ��Ļ����״̬
extern iU8  GriddingType[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength];
// ��Ļ�����Ƿ���Ҫ���µı��
extern iU8  GriddingUpdataFlag[SCREEN_HEIGHT/SideLength][SCREEN_WIDTH/SideLength];

extern iU8  AlgaeNeed;

// ����ȴ�ˮ�������ṹ
extern AnimationStr CrocodileWait;
// ����ϴ�趯���ṹ
extern AnimationStr CrocodileBathe;

extern iS32 TimeCount;


#endif

