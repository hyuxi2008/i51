#ifndef I51PUBLIC_H
#define I51PUBLIC_H
#include "i51.h"
#include "i51KitAde.h"
#include "Image_Hex.h"
#include "LLPInput.h"
#include "localG2.h"
//#include "version.h"
#include "i51KitP2.h"
///////////////////////////////////// ����ÿ����Ϸ��ͬ �ڴ��޸� //////////////////////////////////////////

#define PKIG_SCREEN_VERTICAL  //#define PKIG_SCREEN_HORIZONTAL  //����  PKIG_SCREEN_VERTICAL ����  
#define INSTALL_HORIZONTAL      //INSTALL_VERTICAL  ��װ������Ϊ������      INSTALL_HORIZONTAL  ����
//#define MODE_LOCAL  //�Ƿ��������ĵ����汾����������Ϸ(��������ʧ�ܺ�ĵ����治ͬ)
#define  LANGUAGE_CHINESE//���İ�
//#define LANGUAGE_ENGLISH  //Ӣ�İ�ĺ�
//#define PKIG_SCREEN_320X480
//#define PKIG_SCREEN_240X400
#define AUDIO_PLAYER_MIDI_NUMS  (4)  //midi����������Ŀ
#define AUDIO_PLAYER_WAV_NUMS   (0)  //wav����������Ŀ    //ע��midi_nums + wav_nums <= 5

#define GAME_DIR       "SimpleWMW"       //��ϷĿ¼
///* game */
//#define GAME_KIT_VERSION (5)   //game.kit�İ汾�ţ�ÿ�θ��������
//#define GAME_KIT_NUM   1            //kit�ĸ���
//#define GAME_KIT       {"DdzKit.kit"}   //��Ϸkit
//#define GAME_RESOURCE_NUM   1   //re�ĸ���
//#define GAME_RESOURCE  {"DdzKit.re"}    //��Ϸ��Դ
//#define  CONTAINER_NAMES      {"game.container"}  //��������
//#define  RESOURCE_IMAGE_NUMS  {76}   //ÿ����Դ�ļ���ͼƬ����
//
//#define GAME_PACKAGE_URL    "app://%s/%d/DdzKit/"    //�����������ص���Ϸurl����Ҫ�޸�����cf����Ϊ���Լ��İ���
//
///* install */
//#define INSTALL_KIT_VERSION  (1)
//#define INSTALL_KIT_NAME  "Dinstall.kit"
//#define INSTALL_RESOURCE_NUM  (2)  //re�ĸ�����install.re��UI_Frame.re
//#define INSTALL_RESOURCE  {"Dinstall.re", "UI_frame.re"}
//#define INSTALL_CONTAINER {"install.container", "UI_frame.container"}
//
//#define KIT_GAME_PATH      L"DDZ\\DdzKit.kit"       //kit��·��
//#define KIT_INSTALL_PATH   L"DDZ\\install.kit"

//#ifdef GAME_LOGO
//#define INSTALL_RESOURCE_IMAGE_NUMS  {4, 17}   //install.re���ͼƬ��
//#else
//#define INSTALL_RESOURCE_IMAGE_NUMS  {3, 17}
//#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////// �ֱ��� ////////////////////////////////////////////////
#if defined(PKIG_SCREEN_240X320)
#define PK_SCREEN_WIDTH	 (240)
#define PK_SCREEN_HEIGHT (320)
#define SCREEN_OFFSET_Y (0)
#define SCREEN_OFFSET_X (0)
#elif defined(PKIG_SCREEN_240X400)
#define PK_SCREEN_WIDTH	 (240)
#define PK_SCREEN_HEIGHT (400)
#define SCREEN_OFFSET_Y (40)
#define SCREEN_OFFSET_X (0)
#elif defined(PKIG_SCREEN_320X400)
#define PK_SCREEN_WIDTH	(320)
#define PK_SCREEN_HEIGHT (400)
#define SCREEN_OFFSET_Y (0)
#define SCREEN_OFFSET_X (0)
#elif defined(PKIG_SCREEN_320X480)
#define PK_SCREEN_WIDTH	(320)
#define PK_SCREEN_HEIGHT (480)
#define SCREEN_OFFSET_Y (40)
#define SCREEN_OFFSET_X (0)
#elif defined(PKIG_SCREEN_320X240)
#define PK_SCREEN_WIDTH	(320)
#define PK_SCREEN_HEIGHT (240)
#define SCREEN_OFFSET_Y (0)
#define SCREEN_OFFSET_X (0)
#elif defined(PKIG_SCREEN_220X176)
#define PK_SCREEN_WIDTH	(220)
#define PK_SCREEN_HEIGHT (176)
#define SCREEN_OFFSET_Y (0)
#define SCREEN_OFFSET_X (0)
#elif defined(PKIG_SCREEN_272X480)
#define PK_SCREEN_WIDTH	(272)
#define PK_SCREEN_HEIGHT (480)
#define SCREEN_OFFSET_Y (40)
#define SCREEN_OFFSET_X (0)
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* package */
#define GAME_PACKAGE     "game_package"     //��Ϸ����
#define UPDATE_PACKAGE   "update_package"   //��������

#define SCREEN_CONTENT_FILE   "screen.dat"   //��Ļ�����ļ�
#define GAME_UPDATE_FILE  "update"

#define FEE_APP_NAME  L"51pkpay"    //�Ʒ�Ӧ������

#define PK_FONT_HEIGHT   (12)
#define PEN_RECT_WORH    (80)
#define FONT_WHITE     0xffff  //�����ɫ

#define HTTP_PROTOCOL_VERSION   (0x01)    //Э��汾

#define i51KITG2_ALPHA_NONEED (0xFF) //xie

typedef enum
{
	GAME_RUN_MODE_LOCAL = 0,
	GAME_RUN_MODE_NETWORK,
	GAME_RUN_MODE_LOCAL_NOT_LIMIT
};

typedef enum
{
	KIT_I51,
	KIT_INSTALL,
	KIT_GAME
};

enum
{
	SRC_IMAGE_WIDTH = 9999,
	SRC_IMAGE_HEIGHT
};

typedef struct 
{
	iU32 srcData;
	iU32 pContainer;
	iU32 id;
	iS32 w;
	iS32 h;
}public_image_struct;

///////////////////////////////////////////////////////////////////////////
//////////////////////��������//////////////////////////

#define CHANNEL_AUDIO_NUM   (5)  //ÿ��ͨ����๲�õ���������

typedef enum
{
	AUDIO_PLAYER_MIDI_0 = 0,
	AUDIO_PLAYER_MIDI_1,
	AUDIO_PLAYER_MIDI_2,
	AUDIO_PLAYER_MIDI_3,

	AUDIO_PLAYER_WAV_0,
	AUDIO_PLAYER_WAV_1,
	AUDIO_PLAYER_WAV_2,

	AUDIO_PLAYER_MP3,

	AUDIO_PLAYER_NUM_MAX
}audio_player_type;

typedef struct  
{
	void *p;   //�����ĵ�ַ
	iU32 leng;   //�����ĳ���
	iU8  loop;  //����ģʽ
	iU8  playing;    //�Ƿ����ڲ���
	iU8  resume;     //�Ƿ���Ҫ�ָ�
	i51METYPE type;  //���������ͣ�midi����wav��
	i51MMIDEC mem_or_file;  //������������ʽ�����ڴ滹���ļ�
	iU32 priority;   //��ǰ�������������ȼ�,ֵԽС������ֵԽ��
}audio_struct;

///////////////////////////////////////////////////////////////////////////

typedef enum
{
	ERROR_DECODE_FALSE = 0,  //����ʧ��
	ERROR_LINK_FALSE,        //��ͨ����ʧ��
	ERROR_SYSTEM,            //ϵͳ����
	ERROR_GAME_LOCAL,        //������
}error_type_enum;

//��ʾ�����࣬��Ҫ�ǰ�ť��ͬ
enum
{
	TIPS_NULL,         //�ް�ť
	TIPS_LINK,         //TIPS
	TIPS_REVIVED,      //���ȡ��
	TIPS_EXIT,         //ȷ�����˳�
	TIPS_RETRY,        //���ԣ�ȡ��
	TIPS_NORMAL,       //ȷ��������
	TIPS_RETRY_EXIT    //���ԣ��˳�
};

////////////////////////////////////////////////////////////////////////////


//#define  __MODIS_DEBUG__

extern iBOOL find_aim_dir(char *dir);

extern iBOOL check_file_is_exist(char *dir, char *file);

extern void public_itoa(iS32 value, char *str);

extern void public_image_init(public_image_struct *image, iU32 image_id, iU32 image_data);

extern void public_image_destroy(public_image_struct *image);

extern iRECT *public_build_rect(iRECT *rect, iS16 x, iS16 y, iS16 width, iS16 height);

extern iPoint *public_build_point(iPoint *point, iS32 x, iS32 y);

extern iS32 public_draw_image(public_image_struct *image, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h);

extern iU8 public_pen_enabled_rect(iU16 pen_x, iU16 pen_y, iU16 min_x, iU16 max_x, iU16 min_y, iU16 max_y);

extern iS32 public_write_file(char *file_name, void *data, iU32 size);

extern iS32 public_read_file(char *file_name, void *data);

extern iS32 public_delect_file(char *dir, char *filename);

extern iBOOL public_audio_init(void (*cb)(iBOOL results));

extern void public_audio_all_exit(void);

extern void audio_resume(iBOOL results);

extern void audio_play_cb(void* handle);

extern iBOOL public_audio_play_x(iU32 priority, void *audio_data, iU32 audio_len, i51MMIDEC mem_or_file, iU32 channel, iBOOL loop);

extern iBOOL public_audio_stop(audio_player_type player_type);

extern iU8 save_screen(void);

extern iU8 resume_screen(void);

extern iBOOL time_is_second(void);

extern void public_draw_score(iU32 score, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h, public_image_struct *image);




extern public_image_struct iMageInfo[];

extern iU8 GameData[];

extern iBOOL iScreenFlag;

extern iS32 SWMW_i51KitG2DrawImage(public_image_struct *image, iRECT *dstRect, iRECT *srcRect);

extern void wmw_main_page();

extern void wmw_choose_page();

extern void wmw_image_destroy();

extern void wmw_partol_page();

extern void wmw_game_page();

extern void wmw_pause_page();

extern void wmw_win_page();

extern void wmw_result_page();

extern void wmw_destroy_win_page();

extern void wmw_destroy_result_page();

extern void portal_cb(iBOOL data);

extern void wmw_portal_ui();

extern unsigned char wmw_SoundChoose_mp3[];

extern unsigned int wmw_SoundChoose_mp3_len;

extern unsigned char wmw_SoundWin_mp3[];

extern unsigned int wmw_SoundWin_mp3_len;

extern iVECT INTERSECTION_P2S[10];

extern iS32 IntersectionNum;

extern iU32 CurrDirection;

extern iS32 DirectionNum;

extern iS16 se_TrackPointCount;

extern iS16 se_TrackPoint[6][2];


extern iU8 IsCompleten;

extern iU32 word_frame;

extern iU8 IsEntry;
//
////extern G1_EDGE_TABLE  G1_EdgeTable;
//
extern audio_struct audio[AUDIO_PLAYER_NUM_MAX];

extern iS8 ChooseErrFlag;

extern iU32 ChooseErrTime;

extern iU8 mwLevelWinFlag;


extern iU8 mwLevelResultFlag;

extern iU32 LevelErrStartTime;

extern iU8 LevelErrFlag;

extern iU8 DrawBlankAlpha;

extern iS16 OldGridding_X;

extern iS16 OldGridding_Y;

extern iS16 CurrGridding_X;

extern iS16 CurrGridding_Y;




#endif 
