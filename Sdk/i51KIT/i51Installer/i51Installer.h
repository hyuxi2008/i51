
#ifndef THE_InstallApp_H
#define THE_InstallApp_H

#include "i51.h"
//ģ���ڰ�ť�ؼ��ĸ���
#define I51_BUTTON_CONTROL_MAX_COUNT 	2

//ģ���ڹ������ؼ��ĸ���
#define I51_TIP_CONTROL_MAX_COUNT 	1

//ģ���ڽ������ؼ��ĸ���
#define I51_PROCESS_CONTROL_MAX_COUNT 	1

//��������������
#define I51_TIP_INFO_MAX_COUNT		10
//���������ܹ���ʾ�����ֵĳ��� 
#define I51_TIP_INFO_MAX_LENGTH		32

//��ʾ���ܹ���ʾ�ĳ���
#define I51_MSG_INFO_MAX_LENGTH	100


typedef enum {

	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	ģ������

	//������Ӧģ��
	i51_SAP_GUI_TEMPLATE_NOT_ADEPTIVE  ,
	
	//����Ӧģ��
	i51_SAP_GUI_TEMPLATE_ADEPTIVE  ,
	

	
}  i51SAPGUI_TEMPLATE ;

typedef enum {

	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	�ؼ�����

	//��ť�ؼ�
	i51_SAP_GUI_CONTROL_BUTTON  ,
	
	//�������ؼ�
	i51_SAP_GUI_CONTROL_TIP  ,
	
	//�������ؼ�
	i51_SAP_GUI_CONTROL_PROCESS  ,
	
}  i51SAPGUI_CONTROL ;

typedef enum {

	//	author : william
	//	since : 20111011
	//	PKIG Tech. Co., Ltd.
	//	��ʾ������

	//��ɫ��ʾ��
	i51_SAP_GUI_MSG_SIMPLE ,
	//ͼƬ��ʾ��
	i51_SAP_GUI_MSG_IMAGE ,
	


} ;




//�ؼ��ص�
typedef void (*i51SAPGUICONTROLCALLBACK) ( i51SAPGUI_CONTROL ControlType ) ;

typedef struct {
	
	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	��ť�ؼ�

	//�����Ļ���Ͻǵ�x����/�������ͼƬ�ľ��Ǳ�������
	iS32 x ;

	//�������Ļ���Ͻ�y����
	iS32 y ;

	//��ťͼƬ�Ŀ��/�������ͼƬ�ģ����ǻ������Ŀ�Ⱥ͸߶�
	iS32 width ;

	//��ťͼƬ�ĸ߶�
	iS32 height ;

	//����״̬�µ�ͼƬ
	iHANDLE NomalImg ;

	//ѡ�к��ͼƬ
	iHANDLE SelectedImg ;

	/*******��������ְ�ť******/
	iU8 *text ;

	iCOLOR TextColor ;

	//��ť���ֵĿ��
	iU16 TextWidth ;

	//��ť���ֵĸ߶�
	iU16 TextHeght ;

	//��ť���ֵĸ���
	iU16 TextLength ;

	//���ְ�ť��������ɫ
	iCOLOR buttonNormalColor ;

	//���ְ�ťѡ�к����ɫ
	iCOLOR buttonSelectColor ;
	/**********************************/

	//�ؼ���ģ�����Ƿ�ʹ��
	iBOOL used ;

	//��ť�ؼ��Ļص�
	i51SAPGUICONTROLCALLBACK callback ;

} iSAPBUTTON ;

typedef struct {
	
	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	�������ؼ�

	//�����Ļ���Ͻǵ�x����
	iS32 x ;

	//�������Ļ���Ͻ�y����
	iS32 y ;

	//������ͼƬ������ģ��������Ŀ��
	iS32 width ;

	//������ͼƬ������ģ��������ĸ߶�
	iS32 height ;

	//	�������Ľ���
	iS32 seek ;

	//�յĽ�����ͼƬ
	iHANDLE EmptyImg ;

	//���Ľ�����ͼƬ
	iHANDLE FillImg ;

	//��ģ��������յ���ɫ
	iCOLOR EmptyColor ;
	
	
	//��ģ�������������ɫ
	iCOLOR FillColor ;	
	

	iCOLOR PointColor ;

	//�ؼ���ģ�����Ƿ�ʹ��
	iBOOL used ;
	//	��ǰ���صڼ���
	iU32 curdl ;
	//	����Ҫ���ض��ٸ�
	iU32 dlcount ;

	//��ť�ؼ��Ļص�
	i51SAPGUICONTROLCALLBACK callback ;

} iSAPPROGRESS ;



typedef struct {
	
	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	�������ؼ�

	//ͨ����ͼƬ�����Ļ���Ͻǵ�x����
	iS32 x ;

	//ͨ����ͼƬ�������Ļ���Ͻ�y����
	iS32 y ;

	//������ͼƬ������ģ�幫�����Ŀ��
	iS32 width ;

	//������ͼƬ������ģ�幫�����ĸ߶�	
	iS32 height ;

	//������ͼƬ
	iHANDLE TipImg ;

	//���������������Ļ���Ͻǵ�x���꣬�����x����
	iS32 TextX ;

	//�����������������Ļ���Ͻ�y���꣬�ֶ���y����
	iS32 TextY ;

	//����������������
	iCHAR text[I51_TIP_INFO_MAX_COUNT][I51_TIP_INFO_MAX_LENGTH+1] ; /*???????????????*/

	//	���й�������Ϣ����Ŀ
	iU32 TextCount ;

	//���ֵĿ��
	iU16 TextWidth ;

	//���ֵĸ߶�
	iU16 TextHeght ;

	//���ֵĸ���
	iU16 TextLength ;

	//������������ɫ
	iCOLOR TextColor ;

	//�������������ɫ
	iCOLOR FillColor ;

	//�ؼ���ģ�����Ƿ�ʹ��
	iBOOL used ;

	//��ť�ؼ��Ļص�
	i51SAPGUICONTROLCALLBACK callback ;

} i51SAPTIP ;


typedef struct {

	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	ģ��

	//ģ������
	iU32 type ;

	//����ͼƬ
	iHANDLE* bgimage ;

	//������ɫ
	iCOLOR bgcolor ;

	//��ť�ؼ�
	iSAPBUTTON button[I51_BUTTON_CONTROL_MAX_COUNT] ;

	//�������ؼ�
	i51SAPTIP tip[I51_TIP_CONTROL_MAX_COUNT] ;

	//�������ؼ�
	iSAPPROGRESS process[I51_PROCESS_CONTROL_MAX_COUNT] ;
	
} iSAPTEMPLATE ;


#if 0
typedef struct {

	//	author : william
	//	since : 20111009
	//	PKIG Tech. Co., Ltd.
	//	��ʾ��

	//��ʾ������
	iU32 type ;

	//��ʾͼƬ�����Ļ���Ͻǵ�x����
	iS32 x ;

	//��ʾͼƬ�������Ļ���Ͻ�y����
	iS32 y ;

	//��ʾͼƬ������ģ�幫�����Ŀ��
	iS32 width ;

	//��ʾͼƬ������ģ�幫�����ĸ߶�	
	iS32 height ;

	//����ͼƬ
	iHANDLE* bgimage ;

	//������ɫ
//	iCOLOR bgcolor ;

	iCOLOR color1 ;

	iCOLOR color2 ;
	
	iCOLOR color3 ;

	//��ʾ���������Ļ���Ͻǵ�x���꣬�����x����
	iS32 TextX ;

	//��ʾ�����������Ļ���Ͻ�y���꣬�ֶ���y����
	iS32 TextY ;

	//����������������
	iCHAR text[I51_MSG_INFO_MAX_LENGTH] ; /*???????????????*/

	//������������ɫ
	iCOLOR TextColor ;

	//��ť�ؼ�
	iSAPBUTTON button[I51_BUTTON_CONTROL_MAX_COUNT] ;



} iSAPMESSAGEBOX ;
#endif

extern iSAPTEMPLATE* i51SlCtrlCreateTemplate ( iU32 type , iHANDLE* bgimage , iCOLOR bgcolor ) ;
extern iBOOL i51SlCtrlTempInsertButtom ( iSAPTEMPLATE* template , iSAPBUTTON* button  ) ;
extern iBOOL i51SlCtrlTempInsertProgress ( iSAPTEMPLATE* template , iSAPPROGRESS* progress  ) ;
extern iBOOL i51SlCtrlTempSetProcessSeek ( iSAPTEMPLATE* template , iSAPPROGRESS* progress  , iU32 seek ) ;
extern iBOOL i51SlCtrlThempInsertTip ( iSAPTEMPLATE* template , i51SAPTIP* tip ) ;
extern iBOOL i51SlCtrlTempDraw ( iSAPTEMPLATE* template ) ;
//extern iSAPMESSAGEBOX* i51SlCtrlCreateMessagebox ( iU32 type , iHANDLE* bgimage , iCOLOR color1  ,iCOLOR color2  ,iCOLOR color3  , iS32 x , iS32 y , iS32 width ,iS32 height , iS32 TextX , iS32 TextY ,iCHAR * text , iS32 TextLength , iCOLOR TextColor )  ;
//extern iBOOL i51SlCtrlMessageboxInsertButtom ( iSAPMESSAGEBOX* messagebox , iSAPBUTTON* Button  ) ;
//extern iBOOL i51SlCtrlMessageboxInsertButtom ( iSAPMESSAGEBOX* messagebox , iSAPBUTTON* Button  )  ;

#endif


