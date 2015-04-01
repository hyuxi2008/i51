
#ifndef THE_CutFruit_H
#define THE_CutFruit_H

#include "i51.h"
#include "i51Public.h"
//#include "GameInstall.h"
//#include "game.h"
#include "LLPInput.h"
//#include "fee_pay.h"
//#include "51pkpay.h"
//#include "http_public.h"
//#include "update.h"
//#include "UI_frame.h"

//#define  FEE_NODE_NUM_MAX  (10)   //������Ϸ�Ʒѵ�������

//#define FEE_APP_NAME  L"51pkpay"    //�Ʒ�Ӧ������

typedef enum
{
	STATE_INSTALL = 0,
	STATE_GAME,
	STATE_I51,
}state_type;

typedef enum
{
	I51_STATE_UPDATE = 0,
	I51_STATE_FEE_PAY,
	I51_STATE_UNLOAD_AND_LOAD
}i51_state_type;

typedef struct  
{
	iU16 srcapp[32];
	char buf[64];
}app_param_struct;

typedef struct  
{
	iU8  IsRun;
	iU32 aid;
	iU32 tick;
	iU16 screen_w;
	iU16 screen_h;
	iBOOL BltEachTime;
	iU8  state;
	iU8  i51_state;
	iU8  unload_kit;
	iU8  run_mode;
	iPoint offset;
//	fee_struct  fee;
//	fee_node_struct fee_node[FEE_NODE_NUM_MAX];
	iBOOL sound_music;     //����
	iBOOL sound_effects;   //��Ч
	iU32  volume; //����
	app_param_struct app_param;
}app_struct;

extern app_struct app;

//extern void *i51_malloc(unsigned int size);
//extern void i51_free(void *ptr);

extern void unload_kit_msg(iU8 state);
extern void unload_app(char *name);
extern iU32 get_game_aid(void);
extern void pfunc_init(void *p);
extern void update_check(char *game, void (*cb)(void));
extern void i51_exit(void);
extern iBOOL time_is_second(void);
extern void link_layer_close(void);

//����ת��Ϊ�ַ���
extern void public_itoa(iS32 value, char *str);

//��������ֵ
extern iRECT *public_build_rect(iRECT *rect, iS16 x, iS16 y, iS16 width, iS16 height);

//���긳ֵ
extern iPoint *public_build_point(iPoint *point, iS32 x, iS32 y);

//�����ĸ����㸳ֵ
extern void public_build_rect_vertex(iS32 point[][2], iS32 x, iS32 y, iS32 width, iS32 height);

//ͼ��ṹ��ʼ��
extern void public_image_init(public_image_struct *image, iHANDLE pContainer, iU32 image_id);

//��ͼ
extern iS32 public_draw_image(public_image_struct *image, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h);

//�����ʼ��
extern iFILE public_font_init(iU16 *font_re);
extern iBOOL public_font_close(void);

//����������
extern iBOOL public_draw_text(char *text, iS16 dst_x, iS16 dst_y, iU16 length, iCOLOR color);

//����������
extern iBOOL public_draw_string_text(char *text, iS16 dst_x, iS16 dst_y, iS16 width, iS16 height, iCOLOR color, iU16 spacing);

//��x����Ϊ���Ļ�һ������
extern iBOOL public_draw_text_x_middle(char *text, iS16 middle_x, iS16 dst_y, iCOLOR color);

//��y����Ϊ���Ļ�һ������
extern iBOOL public_draw_text_y_middle(char *text, iS16 dst_x, iS16 middle_y, iCOLOR color);

//��y����Ϊ���Ļ���������
extern iBOOL public_draw_string_text_y_middle(char *text, iS16 dst_x, iS16 middle_y, iCOLOR color, iU16 spacing);

//����������(��ת90��)
extern iBOOL public_draw_text_rotate_90(char *text, iS16 dst_x, iS16 dst_y, iU16 length, iCOLOR color);

//����������(��ת90��)
extern iBOOL public_draw_string_text_rotate_90(char *text, iS16 dst_x, iS16 dst_y, iS16 width, iS16 height, iCOLOR color, iU16 spacing);

//��y����Ϊ���Ļ�һ������(��ת90��)
extern iBOOL public_draw_text_y_middle_rotate_90(char *text, iS16 dst_x, iS16 dst_y, iCOLOR color);

//��x����Ϊ���Ļ���������(��ת90��)
extern void public_draw_string_text_x_middle_rotate_90(char *text, iS16 middle_x, iS16 dst_y, iU32 color, iU32 spacing);

//������
extern void public_fill_rect(iS32 x, iS32 y, iS32 width, iS32 height, iCOLOR color);

//�����߿��������
extern void public_draw_frame_rect(iS32 x, iS32 y, iS32 width, iS32 height, iCOLOR frame_color, iCOLOR fill_color);

//������
extern void public_draw_score(iU32 score, iS16 dst_x, iS16 dst_y, iS16 src_x, iS16 src_y, iU16 src_w, iU16 src_h, public_image_struct *image);

//�жϴ������Ƿ��ھ�����
extern iU8 public_pen_enabled_rect(iU16 pen_x, iU16 pen_y, iU16 min_x, iU16 max_x, iU16 min_y, iU16 ma1x_y);

//�����Ұ�ť
extern void public_draw_button(char *lsk_word, char *rsk_word, public_image_struct *image);

//д�ļ�
extern iS32 public_write_file(char *file_name, void *data, iU32 size);

//���ļ�
extern iS32 public_read_file(char *file_name, void *data);
extern iS32 public_delect_file(char *dir, char *filename);

//��������ʼ��
extern iBOOL public_audio_init(void (*cb)(iBOOL results));         //��ʼ����ǰ��Ҫ���ŵ�����ͨ����, midi_num+wav_num���ܴ���AUDIO_PLAYER_ID_MAX

//�ر����в�����
extern void public_audio_all_exit(void);

extern void audio_resume(iBOOL results);

//��������
extern iBOOL public_audio_play(iU32 priority, void *audio_data, iU32 audio_len, i51MMIDEC mem_or_file, iU32 channel, iBOOL loop);

//ֹͣ����
extern iBOOL public_audio_stop(audio_player_type player_type);

extern iU8 save_screen(void);
extern iU8 resume_screen(void);


extern iBOOL find_aim_dir(char *dir);
extern iBOOL check_file_is_exist(char *dir, char *file);

extern void public_pfunc_init(i51Public_struct *func);
//extern void public_pfunc_http_init(http_public_struct *func);
//extern iU8  game_run_mode_init(void);
//extern iU8 set_game_run_mode(iU8 mode);
//extern iU8 get_game_run_mode(void);
//extern void update_game(void);
//extern void draw_wait_link(void);
//extern void set_sound_state(iBOOL music, iBOOL effects);
//extern void get_sound_state(iBOOL *music, iBOOL *effects);
extern void kernel_send_request(iU32 cmd);
#endif

