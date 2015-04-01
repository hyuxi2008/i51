#ifndef I51PUBLIC_H
#define I51PUBLIC_H

#include "i51.h"
#include "i51KitAde.h"
#include "i51KitStd.h"
#include "i51KitG2.h"

#include "LLPInput.h"
//#include "version.h"

///////////////////////////////////// ����ÿ����Ϸ��ͬ �ڴ��޸� //////////////////////////////////////////

#define PKIG_SCREEN_VERTICAL  //#define PKIG_SCREEN_HORIZONTAL  //����  PKIG_SCREEN_VERTICAL ����  
//#define INSTALL_VERTICAL      //INSTALL_VERTICAL  ��װ������Ϊ������      INSTALL_HORIZONTAL  ����
#define AUDIO_RESOURCE        //����install�Ƿ���ֽ�����������

//#define MODE_LOCAL  //�Ƿ��������ĵ����汾����������Ϸ(��������ʧ�ܺ�ĵ����治ͬ)

//#define LANGUAGE_ENGLISH  //Ӣ�İ�ĺ�

//#define GAME_LOGO   //�Ƿ���Ҫ��˾LOGO�ĺ�

//#define PKIG_SCREEN_240X400
#define INDEPENDENT_51PKPAY

#define AUDIO_PLAYER_MIDI_NUMS  (4)  //midi����������Ŀ
#define AUDIO_PLAYER_WAV_NUMS   (0)  //wav����������Ŀ    //ע��midi_nums + wav_nums <= 5

#define GAME_DIR       "WheresMyWater"       //��ϷĿ¼
#define IMAGE_RESOURCE_VERSION  0x01   //ͼƬ�汾

/* game */
#define GAME_KIT_VERSION (1)   //game.kit�İ汾�ţ�ÿ�θ��������
#define GAME_KIT_NUM   1            //kit�ĸ���
#define GAME_KIT       {"wmw.kit"}   //��Ϸkit
#define GAME_RESOURCE_NUM   1   //re�ĸ���
#define GAME_RESOURCE  {"wmw.re"}    //��Ϸ��Դ
#define  CONTAINER_NAMES      {"wmw.contain"}  //��������
#define  RESOURCE_IMAGE_NUMS  {186}   //ÿ����Դ�ļ���ͼƬ����

#define GAME_PACKAGE_URL    "app://%s/%d/wmw/"    //�����������ص���Ϸurl����Ҫ�޸�����cf����Ϊ���Լ��İ���

/* install */
#define INSTALL_KIT_VERSION  (1)
#define INSTALL_KIT_NAME  "install.kit"
#define INSTALL_RESOURCE_NUM  (2)  //re�ĸ�����install.re��UI_Frame.re
#define INSTALL_RESOURCE  {"install.re", "UI_frame.re"}
#define INSTALL_CONTAINER {"install.container", "UI_frame.container"}

#define KIT_GAME_PATH      L"WheresMyWater\\wmw.kit"       //kit��·��
#define KIT_INSTALL_PATH   L"WheresMyWater\\install.kit"

#ifdef GAME_LOGO
#define INSTALL_RESOURCE_IMAGE_NUMS  {4, 17}   //install.re���ͼƬ��
#else
#define INSTALL_RESOURCE_IMAGE_NUMS  {4, 17}
#endif

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

//#ifdef HTTP_SERVE_TEST
//#define HOST_NAME "sa.test.pkingame.net";
//#else
//#define HOST_NAME "sa.pkingame.net";
//#endif

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

typedef struct  
{
	iS16 x;
	iS16 y;
}iPoint;

enum
{
	SRC_IMAGE_WIDTH = 9999,
	SRC_IMAGE_HEIGHT
};

typedef struct 
{
	iHANDLE pContainer;
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

#define  unload_dynamic_kit(kit)\
{\
	if(i51KIT_DYNAMIC_KIT_PROTOTYPE(kit))\
{\
	i51KIT_DYNAMIC_KIT_UNLOAD(kit);\
	i51KIT_DYNAMIC_KIT_PROTOTYPE(kit) = iNULL;\
}\
}

typedef void (*FuncPtr) (void);

typedef struct  
{
	//�����ʼ��
	iFILE (*public_font_init)(iU16 *font_re);
	iBOOL (*public_font_close)(void);

	//����������
	iBOOL (*public_draw_text)(char *text, iS16 dst_x, iS16 dst_y, iU16 length, iCOLOR color);

	//����������
	iBOOL (*public_draw_string_text)(char *text, iS16 dst_x, iS16 dst_y, iS16 width, iS16 height, iCOLOR color, iU16 spacing);

	//��x����Ϊ���Ļ�һ������
	iBOOL (*public_draw_text_x_middle)(char *text, iS16 middle_x, iS16 dst_y, iCOLOR color);

	//��y����Ϊ���Ļ�һ������
	iBOOL (*public_draw_text_y_middle)(char *text, iS16 dst_x, iS16 middle_y, iCOLOR color);

	//��y����Ϊ���Ļ���������
	iBOOL (*public_draw_string_text_y_middle)(char *text, iS16 dst_x, iS16 middle_y, iCOLOR color, iU16 spacing);

	//����������(��ת90��)
	iBOOL (*public_draw_text_rotate_90)(char *text, iS16 dst_x, iS16 dst_y, iU16 length, iCOLOR color);

	//����������(��ת90��)
	iBOOL (*public_draw_string_text_rotate_90)(char *text, iS16 dst_x, iS16 dst_y, iS16 width, iS16 height, iCOLOR color, iU16 spacing);

	//��y����Ϊ���Ļ�һ������(��ת90��)
	iBOOL (*public_draw_text_y_middle_rotate_90)(char *text, iS16 dst_x, iS16 dst_y, iCOLOR color);

	//��x����Ϊ���Ļ���������(��ת90��)
	void (*public_draw_string_text_x_middle_rotate_90)(char *text, iS16 middle_x, iS16 dst_y, iU32 color, iU32 spacing);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//������
	void (*public_fill_rect)(iS32 x, iS32 y, iS32 width, iS32 height, iCOLOR color);

	//�����߿��������
	void (*public_draw_frame_rect)(iS32 x, iS32 y, iS32 width, iS32 height, iCOLOR frame_color, iCOLOR fill_color);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//ͼ��ṹ��ʼ��
	void (*public_image_init)(public_image_struct *image, iHANDLE pContainer, iU32 image_id);

	//��ͼ
	iS32 (*public_draw_image)(public_image_struct *image, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h);

	//������
	void (*public_draw_score)(iU32 score, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h, public_image_struct *image);

	//�����Ұ�ť
	void (*public_draw_button)(char *lsk_word, char *rsk_word, public_image_struct *image);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//����ת��Ϊ�ַ���
	void (*public_itoa)(iS32 value, char *str);

	//��������ֵ
	iRECT *(*public_build_rect)(iRECT *rect, iS16 x, iS16 y, iS16 width, iS16 height);

	//���긳ֵ
	iPoint *(*public_build_point)(iPoint *point, iS32 x, iS32 y);

	//�����ĸ����㸳ֵ
	void (*public_build_rect_vertex)(iS32 point[][2], iS32 x, iS32 y, iS32 width, iS32 height);

	//�жϴ������Ƿ��ھ�����
	iU8 (*public_pen_enabled_rect)(iU16 pen_x, iU16 pen_y, iU16 min_x, iU16 max_x, iU16 min_y, iU16 ma1x_y);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//д�ļ�
	iS32 (*public_write_file)(char *file_name, void *data, iU32 size);

	//���ļ�
	iS32 (*public_read_file)(char *file_name, void *data);
	iS32 (*public_delect_file)(char *dir, char *file_name);

	iBOOL (*public_audio_init)(void (*cb)(iBOOL results));
	iBOOL (*public_audio_play)(iU32 priority, void *audio_data, iU32 audio_len, i51MMIDEC mem_or_file, audio_player_type channel, iBOOL loop);
	iBOOL (*public_audio_stop)(audio_player_type player_type);
	void (*public_audio_all_exit)(void);

	iBOOL (*find_aim_dir)(char *dir);
	iBOOL (*check_file_is_exist)(char *dir, char *file);

	//UI frame
	void (*UI_image_init)(void);
	void (*UI_frame_msg_init)(iU8 type, char *msg);
	void (*UI_frame_init)(iU8 type, char *msg);
	iS32 (*UI_frame_render)(void);
	void (*UI_frame_input_key)(LLPPhoneKey *key, FuncPtr lsk_func, FuncPtr rsk_func);
	void (*UI_frame_input_pen)(LLPPhonePen *pen, FuncPtr lsk_func, FuncPtr rsk_func);

	int (*llp_get_input)(LLPInputStruct * input);

	iS32 (*load_fee_app)(void);

	///* HTTP */
	//const char *(*hostname)(void);
	//unsigned short (*probuf_pop_short)(ProBuffer *buf);
	//unsigned char (*probuf_pop_byte)(ProBuffer *buf);
	//unsigned long (*probuf_pop_long)(ProBuffer *buf);
	//char *(*probuf_pop_string)(ProBuffer *buf, char s[], int len);
	//ProBuffer *(*probuf_init)(ProBuffer *buf, int capacity, void *pbuf, int siz);
	//ProBuffer *(*probuf_push)(ProBuffer *buf, const void *data, int size);
	//ProBuffer *(*probuf_push_short)(ProBuffer *buf, unsigned short val);
	//ProBuffer *(*probuf_push_long)(ProBuffer *buf, unsigned long val);
	//ProBuffer *(*probuf_push_string)(ProBuffer *buf, const char* s);
	//void *(*pkig_httpc_postbuf)(Pkig_Httpc *q, const char *host, const char *path, const ProBuffer *buf, Pkig_Httpc_Callback fp);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void (*unload_kit_msg)(iU8 state);
	void (*kernel_send_request)(iU32 cmd); //����Ϣ��kernel(��i51_REQ_EXIT���˳�)
	iU8 (*set_game_run_mode)(iU8 mode);  //������Ϸģʽ��������������,��install.kit
	iU8 (*get_game_run_mode)(void);      //��ȡ��Ϸģʽ,��game.kit
	void (*update_check)(char *game, void (*cb)(void));      //������Ϸ�汾
	void (*set_sound_state)(iBOOL music, iBOOL effects);    //�����Ƿ�������
	void (*get_sound_state)(iBOOL *music, iBOOL *effects);  //��ȡ��ǰ�Ƿ�������

	/*fee_point �Ʒѽڵ�
	**title     "����"��"�������"
	**cb        �ص�����������ֵΪ�� 
	**          <0 �Ʒ�ʧ�ܣ�����ʧ��,ϵͳ����ȣ�
	**          =0 ȡ���Ʒ�(����ȡ��)
	**          0< �Ʒѳɹ�
	*/
	void (*fee_pay)(char *fee_point, char *title, void (*cb)(iS32));
	void (*fee_node_init)(char num, char *point, iU32 money, char *msg);   //install.kit��
	iS32 (*fee_node_save)(void);  
}i51Public_struct;


#endif  /* i51Public.h */