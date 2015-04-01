 /*********************************************************************************/
/**
 * Copyright (C) 2010, PKIG Tech. Co., Ltd.
 *
 * @file	Pk_Rank.h
 * @brief	declare
 * 
 * @author	terrcy
 * @version	$Id: Pk_Rank.h 293 2010-12-24 15:00:56Z terrcy $
 * @date	$Date:: 2010-12-24 15:00:56#$
 *********************************************************************************/
#ifndef _PK_RANK_H_
#define _PK_RANK_H_

#include "i51.h"
#include "i51KitAde.h"
//#include "i51KitG2.h"
//#include "i51KitStd.h"

#define APP_NAME  "SimpleWMW"    //�ڴ��޸����Ӧ�����֣��浵������ \WIN32FS\DRIVE_E\i51\i51APP\APP_NAME\0_0
#define GAME_MAX_USERNAME_LEN 12 
#define MAX_RECORD_NUM	10

#define RevivedID "TTT_001" 

typedef struct gameInfo{
	int 	game_id;
	int 	game_mode;
	int 	game_ver;
	int		user_id;
	char	user_name[GAME_MAX_USERNAME_LEN];
	char    RunMode;
}UAPP_GAME_INFO;
typedef struct tagRankData
{
	int		mUserId;
	char	mName[GAME_MAX_USERNAME_LEN];	//�������
	int		mScore;                         //����
}RANK_DATA_TAG;
typedef struct local_data{
	int	 mGameId;
	int  mVersion;  
	int  mChp;      //��óɼ���ؿ�
	int  mUserId;
	char mName[GAME_MAX_USERNAME_LEN];	
	RANK_DATA_TAG mRankData[MAX_RECORD_NUM];   //�������±�,0-9����1-10��
	char crc[28];
}LOCAL_DATA_TAG;
  
extern void init_local_data(const UAPP_GAME_INFO *pGameInfo, LOCAL_DATA_TAG *pLocalData);   //��ʼ�����а�,�������ݱ����� pLocalData ��
extern void set_local_score_data(int score,LOCAL_DATA_TAG *pLocalData);                     //�������а񣬱���ɼ�             
extern void update_local_chp_data(int chp,LOCAL_DATA_TAG *pLocalData);                      //������ˮ��������������óɼ������йؿ��ģ����ô˱�������ؿ�
extern int	get_local_chp_data(const LOCAL_DATA_TAG *pLocalData);                           //��ȡ��óɼ���ؿ�
#endif