#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "i51Public.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DL_FILE_TEMP    "dl.package"       //��ǰ���ص�package���������
#define PACKAGE_URL    "app://%s/%d/%s/"    //�����������ص���Ϸurl

//����
#define PROGRESS_NUM (1)
#define PROGRESS_START (500)  //�ʼ����ʼ����
#define PROGRESS_EACH_PERCENT {9700} //�����̽������ٷֱ�,������100��

#define  ERROR_MSG_LINK_FALSE  "   ���ӷ�����ʧ�ܣ������ԣ�"

//�������
#define  ERROR_MSG_DECODE_FALSE  "    ��Ϸ����ʧ�ܣ�����洢��״̬���ֻ��ػ�������!           �ͷ��绰��0755-83232356"

enum
{
	DL_STATE_INIT = 0,
	DL_STATE_DOWNLOAD,
	DL_STATE_DECODE,
	DL_STATE_FINISH
};

enum  
{
	PROGRESS_DOWNLOAD = 0,
};

typedef struct  
{
	iS8   progress_state;
	iS8   IsError;
	iS8   error_type;
	iBOOL dl_create_ret;
	iU16 package_name[30];
	iU32 version;
	iS8   lsk_down;
	iS8   rsk_down;
	iS8   init;
	void (*FuncRender)(iU32);
	void (*cb)(iBOOL ret);
}download_struct;

typedef struct  
{
	iU16 current;  //�������ٷֱ�
	iU16 each[PROGRESS_NUM];   //ÿ�����̣���ռ�İٷֱȣ�����25%����Ϸ25%���Ʒ�SDK 10%����Ϸ����20%
}percent_struct;

/* 
**������Դ������
** package_name         ���ص���Դ������
** res_name             ÿ��re������
** ctn_name             ÿ��re�����������ļ���
** image_num            ÿ��re���ͼƬ��Ŀ
** resource_num         ��Ҫ�����re��Ŀ
** FuncRender(iU32)     ��������ͼ����������Ϊ��ǰ���ȣ��ܽ���Ϊ10000
** callback(iBOOL)      �ص�������iBOOL = iTRUE���سɹ��� iFALSE����ʧ��
*/
//�˺�����ÿ��ѭ�������á����ô˺�������Ϸ��ͼ�����ػ档����Ҫ�ػ������⴦��
extern void download_resource(iU16 *package_name, void (*FuncRender)(iU32), void (*callback)(iBOOL));

//��ϵͳ��Ϣexitʱ����
extern void download_exit(void);

//��������
extern void download_key_handle(LLPPhoneKey *key);

//����������
extern void download_pen_handle(LLPPhonePen *pen);

extern void portal_entry(iU32 aid, void (*cb)(iBOOL));
extern void portal_exit(void);


#ifdef PKIG_SCREEN_320X480

#define LOADING_BAR_LEN  (258)

#ifdef PKIG_SCREEN_HORIZONTAL
#define BG_X   158
#define BG_Y   117
#define LOADING_BAR_X    96
#define LOADING_BAR_Y    110
#define LEFT_BUTTON_X    9
#define LEFT_BUTTON_Y    10
#define RIGHT_BUTTON_X   9
#define RIGHT_BUTTON_Y   421
#define PROGRESS_NUM_X   69
#define PROGRESS_NUM_Y   230
#define WORD_TIPS_X      121
#define WORD_TIPS_Y      189
#else
#define BG_X   37
#define BG_Y   85
#define LOADING_BAR_X    30
#define LOADING_BAR_Y    291
#define LEFT_BUTTON_X    14
#define LEFT_BUTTON_Y    450
#define RIGHT_BUTTON_X   256
#define RIGHT_BUTTON_Y   450
#define PROGRESS_NUM_X   150
#define PROGRESS_NUM_Y   322
#define WORD_TIPS_X      107
#define WORD_TIPS_Y      267
#endif

#elif defined(PKIG_SCREEN_240X320)

#define LOADING_BAR_LEN  (198)

#ifdef PKIG_SCREEN_HORIZONTAL
#define BG_X   123
#define BG_Y   70
#define LOADING_BAR_X    79
#define LOADING_BAR_Y    60
#define LEFT_BUTTON_X    6
#define LEFT_BUTTON_Y    8
#define RIGHT_BUTTON_X   7
#define RIGHT_BUTTON_Y   269
#define PROGRESS_NUM_X   61
#define PROGRESS_NUM_Y   149
#define WORD_TIPS_X      98
#define WORD_TIPS_Y      107
#else
#define BG_X   30
#define BG_Y   51
#define LOADING_BAR_X    20
#define LOADING_BAR_Y    195
#define LEFT_BUTTON_X    10
#define LEFT_BUTTON_Y    294
#define RIGHT_BUTTON_X   185
#define RIGHT_BUTTON_Y   295
#define PROGRESS_NUM_X   109
#define PROGRESS_NUM_Y   218
#define WORD_TIPS_X      67
#define WORD_TIPS_Y      175
#endif

#elif defined(PKIG_SCREEN_240X400) 

#define LOADING_BAR_LEN  (198)

#ifdef PKIG_SCREEN_HORIZONTAL
#define BG_X   123
#define BG_Y   109
#define LOADING_BAR_X    79
#define LOADING_BAR_Y    101
#define LEFT_BUTTON_X    6
#define LEFT_BUTTON_Y    8
#define RIGHT_BUTTON_X   7
#define RIGHT_BUTTON_Y   346
#define PROGRESS_NUM_X   62
#define PROGRESS_NUM_Y   190
#define WORD_TIPS_X      97
#define WORD_TIPS_Y      149
#else
#define BG_X   30
#define BG_Y   62
#define LOADING_BAR_X    20
#define LOADING_BAR_Y    216
#define LEFT_BUTTON_X    9
#define LEFT_BUTTON_Y    377
#define RIGHT_BUTTON_X   187
#define RIGHT_BUTTON_Y   378
#define PROGRESS_NUM_X   109
#define PROGRESS_NUM_Y   239
#define WORD_TIPS_X      67
#define WORD_TIPS_Y      196
#endif

#endif

#endif /* DOWNLOAD_H */