
/*

	The Implementing of 'Adapter' , Module : 'MMI'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by osca.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/
#include "i51AdeCnf.h"
#include "i51AdeMmi.h"
#include "i51Script.h"
#include "i51AdeMStar.h"


#include "ven_std.h"
#include "ven_sdk.h"
#include "ven_handle.h"
#include "ven_graphics.h"
#include "ven_font.h"     
#include "ven_stc.h"
#include "ven_ts.h"
#include "ven_os.h"
#include "ven_kpd.h"
#include "ven_setting.h"
#include "ven_clipboard.h"
#include "ven_dbg.h"
#include "ven_drv.h"
#include "ven_codec.h"
#include "ven_socket.h"
#include "ven_util.h"
#include "ven_ui.h"
#include "ven_wap.h"
#include "ven_cam.h"
#include "ven_editor.h"
#include "ven_msg.h"
#include "ven_time.h"

//#define i51ADAPTER_DEBUG

#define R_OF_565(p)      (((p)&0xf800) >> 8)
#define G_OF_565(p)      (((p)&0x7e0) >> 3)
#define B_OF_565(p)      (((p)&0x1f) << 3)

extern iU32 g_i51PkigHandle;

extern void* i51AdeOsMalloc ( iU32 length , const iCHAR* file , iU32 line );
extern void i51AdeStdMemset16(void* dest, iU16 value, iU32 length);
extern iU16 *i51S_AdeFileFixPath(const iU16 *oldpath); //�ⲿ���ã��Ǳ�׼�ӿ�

typedef void (*i51PlayerCB)(iHANDLE handle);    
typedef void (*i51ADEPLAYRECORDCB)( iBOOL results );   

extern ven_GraphicsCanvas *g_i51PkigCanvas ;
extern iS32 g_i51MediaHandle ;
extern ven_FontAttr *g_i51PkigFontNormal ;

i51RecoderStruct  i51Recoder = { 0 } ;
I51_RECORDER_ACTION_FLAG g_i51RecorderActionFlag = I51_RECORDER_ACTION_NONE;


static i51ADECALLBACK i51AdeCallback = 0 ;
static i51ADECALLBACK i51RecorderStop = 0 ;
static i51ADEPLAYRECORDCB i51PlayRecordCb = 0 ; 
static iBOOL i51AdeMmiMediaPlayerLock = iFALSE ;
static iBOOL i51AdeMmiMediaPlaying = iFALSE ;
static iU32 g_i51LastVolume = VEN_MEDIA_VOLUME_0;
/*========================================================
	Media Player -- start
=========================================================*/
static void _i51AdeMmiMediaPlayerMessageProcessing( iU32 timerid );
static iBOOL _i51AdeMediaPlayerDeviceStopPlayImmediately();
static iBOOL _i51AdeMmiMediaPlayerIsWorking();

extern iS32 g_i51MediaHandle;
extern iU32 g_i51PkigHandle;


#define I51_MEDIA_PLAYER_OBJECT_NUM			(8)
#define I51_MEDIA_PLAYER_MESSAGE_QUEUE_SIZE	(32)
#define I51_MEIDA_PLAYER_INVALIDATE_ID		(0xFF)
#define I51_WAV_HEAD_LEN					(44)
#define I51_MEDIA_PLAYER_SECTOR_NUM			(3)
#define I51_MEDIA_PLAYER_SECTOR_SIZE		(1024 * 50)

typedef enum
{
	i51_PLAYMODE_MIX = 0,
	i51_PLAYMODE_SINGLE,
	i51_PLAYMODE_INVALIDATE
}i51PLAYMODE;

typedef enum
{
	i51_PLAYSTATUS_PLAYING = 0,
	i51_PLAYSTATUS_PAUSE,
	i51_PLAYSTATUS_STOP
}i51PLAYSTATUS;

typedef struct{
	char magic[4];		//0x52, 0x49, 0x46, 0x46 "RIFF"��־ 
	int  len;			//�ļ����� - 8 
	char wav[4];		//0x57, 0x41, 0x56, 0x45   "WAVE"��־ 
	char fmt[4];		//0x66, 0x6D, 0x74, 0x20, // "fmt"��־ 
	int  ignore;		//0x10,0x00,0x00,0x00 
	u16  format; 		//0x01,0x00 ��ʽ���10HΪPCM��ʽ����������) 
	u16  channel;		//0x01  ͨ����
	int  sample; 		 //0x40, 0x1F,0x00,0x00   ������
	int  nAvgBytesPerSec; //0x80,0x3E,0x00,0x00 ������Ƶ���ݴ������ʣ���ֵΪͨ������ÿ������λ����ÿ   ����������λ����8������������ô�ֵ���Թ��ƻ������Ĵ�С�� 
	u16  align; 		//0x02  ���ݿ�ĵ����������ֽ���ģ�����ֵΪͨ������ÿ����������λֵ��8
	u16  elemt; 		//0x10 ÿ����������λ������ʾÿ�������и�������������λ����
	char data[4];		//0x64, 0x61, 0x74, 0x61,���ݱ�Ƿ���data�� 
	int  pcmlen; 		//pcm len �������ݵĳ���
}wavheader_t;

typedef enum
{
	i51_WAVSAMPLE_8_KHZ = 0,
	i51_WAVSAMPLE_11_025_KHZ,
	i51_WAVSAMPLE_12_KHZ,
	i51_WAVSAMPLE_16_KHZ,
	i51_WAVSAMPLE_22_KHZ,
	i51_WAVSAMPLE_24_KHZ,
	i51_WAVSAMPLE_32_KHZ,
	i51_WAVSAMPLE_44_1_KHZ,
	i51_WAVSAMPLE_48_KHZ,
	i51_WAVSAMPLE_64_KHZ,
	i51_WAVSAMPLE_96_KHZ,
	i51_WAVSAMPLE_MAX
}i51WAVSAMPLE;

typedef struct
{
	i51WAVSAMPLE eSampleType;
	iU8 nBit;
	iS32 sampleValue;
}i51WavSampleStruct;

static const i51WavSampleStruct stWavSample[] = 
{
	{i51_WAVSAMPLE_8_KHZ,  8 , 8000},
	{i51_WAVSAMPLE_16_KHZ, 16, 16000},
	{i51_WAVSAMPLE_24_KHZ, 24, 24000},
	{i51_WAVSAMPLE_32_KHZ, 32, 32000},
	{i51_WAVSAMPLE_48_KHZ, 48, 48000},
	{i51_WAVSAMPLE_64_KHZ, 64, 64000},
	{i51_WAVSAMPLE_96_KHZ, 96, 96000},
};

typedef struct
{
	iBOOL	bUsed;				//�Ƿ�ʹ��
	void*	handle;				//���
	iU32	priority;			//���ȼ�
	iU32	length;				//���ݳ���
	iU32	codec;				//��������
	iU32	dec;				//������Դ
	iBOOL	loop;				//�Ƿ�ѭ������
	iFILE	iFileHandle;		//�ļ����
	i51PLAYSTATUS status;		//����״̬
	iU32	pos;				//����ָ��
	void	(*pfnStopCallback)(iHANDLE handle);	//ֹͣ���ź�Ļص�����
	
}i51MediaPlayerObjStruct;

typedef struct
{
	iU8* id;
	iU32 priority;
	void* handle;
	iU32 length;
	iU32 codec;
	iU32 dec;
	iBOOL loop;
	void (*STOP)(iHANDLE handle);
}i51MediaPlayerParamDataStruct;


typedef enum
{
	i51_MEDIAOPERATOR_PLAY = 0,
	i51_MEDIAOPERATOR_PAUSE,
	i51_MEDIAOPERATOR_RESUME,
	i51_MEDIAOPERATOR_STOP,
	i51_MEDIAOPERATOR_RELEASE
}i51MEDIAOPERATOR;

typedef struct
{
	iBOOL bEnable;
	void* handle;
	i51MEDIAOPERATOR eOpreator;
	i51MediaPlayerParamDataStruct stDataParam;
}i51MediaPlayerMessageStruct;

typedef struct
{
	iU8*	pStreamBuf;			//����Buffer
	iU32	nStreamLen;			//���ų���
	iU32	nPlayLen;			//�Ѳ��ų���
}i51MediaPlayerStreamStruct;

typedef struct
{
	iBOOL	bCreated;						//�������Ƿ񱻴���
	i51MEDIACALLBACK pfnCreateCb;			//�������������CallBack
	iU32	proccessTimerId;				//�����¼�����Timer
	
	iBOOL	bPlaying;						//�Ƿ����ڲ���
	iBOOL	bBusyPlaying;					//�豸��æ���ȴ�������Ϣ
	iBOOL	bBusyStopping;					//�豸��æ���ȴ�ֹͣ��Ϣ
	i51PLAYMODE	eMode;						//����ģʽ(����Ƶ/����)
	i51WavSampleStruct*	pSample;			//����������(��ǰֻ֧����ͬ�����ʵ�WAV����)
	i51MediaPlayerStreamStruct	streamSector[I51_MEDIA_PLAYER_SECTOR_NUM];	//����������
	iU32		sectorId;					//��ǰ���ŵ�Sector
}i51MediaPlayerMgrStruct;

static i51MediaPlayerObjStruct i51MediaPlayerAudioPlayList[I51_MEDIA_PLAYER_OBJECT_NUM];
i51MediaPlayerMessageStruct i51MediaPlayerMessageQueue[I51_MEDIA_PLAYER_MESSAGE_QUEUE_SIZE];
static i51MediaPlayerMgrStruct i51MediaPlayerManager;

extern iU8 gi51IsRunning;
/*========================================================
	Media Player -- end
=========================================================*/


static void i51S_AdeMmiRecoderPlayCB (u32 UserData, u32 RspData)
{

 	//	author : osca
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	�ӿ�"i51AdeMmiMediaPlay" ��Callback
    ven_codec_ret_t retCode = RspData;
    iHANDLE stream = (iHANDLE)UserData;
    
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51S_AdeMmiRecoderPlayCB , S , retCode = %d ,stream=0x%x", retCode,stream);
#endif
    if (retCode == VEN_CODEC_PLAY_FINISHED)
    {
        if(i51PlayRecordCb)
        {
            i51AdeOsLog(i51_LOG_SC, "MMI : i51S_AdeMmiRecoderPlayCB , Recoder-Play Callback S");
            i51PlayRecordCb(1);
            i51AdeOsLog(i51_LOG_SC, "MMI : i51S_AdeMmiRecoderPlayCB , Recoder-Play Callback E");
        }
    }
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51S_AdeMmiRecoderPlayCB , E ");
#endif		
 	
}

static void i51S_AdeMmiCreateCB ( iU32 timerid )
{

    if(i51AdeCallback)
    {
        i51AdeCallback(1);
    }
    i51AdeOsTimerStop ( timerid ) ;
    
}

iBOOL i51AdeMmiGetScreenScale ( iU16* width , iU16* height )
{

 	//	author : osca
	//	since : 2011.4.20
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : �ɹ�����> 0 ��ʧ��0
	
	ven_GraphicsDisplayInfo_t DispInfo;	
	iU32 ret = 0;
	
#ifdef i51ADAPTER_DEBUG
	//i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetScreenScale S , %x , %x", width, height);
#endif

	if(VEN_GRAPHICS_RET_SUCCESS != ven_graphics_getDisplayInfo( &DispInfo ))
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetScreenScale, ERR, Get Scale");
		return 0 ; 
	}

	*width = DispInfo.width ;
	*height = DispInfo.height ;

#ifdef i51ADAPTER_DEBUG
	//i51AdeOsLog(i51_LOG_SC, "ADE - MMI : i51AdeMmiGetScreenScale E");
#endif
	return 1 ; 
}

void i51AdeMmiUpdateScreen ()
{

 	//	author : osca
	//	since : 2011.4.20
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : �ɹ�����> 0 ��ʧ��0
    ven_GraphicsDisplayInfo_t DispInfo;
    ven_GraphicsRect_t rect={0,0,0,0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiUpdateScreen , S");
#endif
 
    if (!g_i51PkigCanvas)
    {
        return;  
    }
    
	#ifdef i51PROFILINGS
		i51KernelDrawProfiles () ;
	#endif
	
	ven_graphics_getDisplayInfo( &DispInfo );
    rect.w = DispInfo.width;
    rect.h = DispInfo.height;
    ven_graphics_setDirtyArea(g_i51PkigCanvas, &rect);
    ven_graphics_setClipArea(g_i51PkigCanvas, &rect);
    ven_graphics_screenUpdate( g_i51PkigCanvas, VEN_GRA_UPDATE_NORMAL);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiUpdateScreen , E");
#endif

}


void i51AdeMmiUpdateScreenEx ( iRECT* irect , iU32 length )
{

 	//	author : jelo
	//	since : 2012.3.31
	// 	(C) 2011 , PKIG Tech. Co., Ltd.


	//	notes : �ɹ�����> 0 ��ʧ��0

	iU32 counter = 0 ;
	ven_GraphicsRect_t rect={0,0,0,0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiUpdateScreenEx , S , %x,%d" , irect, length);
#endif

	if (!g_i51PkigCanvas)
	{
		return;  
	}
	
	#ifdef i51PROFILINGS
		i51KernelDrawProfiles () ;
	#endif
	
	for ( counter = 0 ; counter < length ; counter ++ ) {

		rect.x = irect[counter].X ;
		rect.y = irect[counter].Y ;
		rect.w = irect[counter].Width ;
		rect.h = irect[counter].Height ;
		
	    ven_graphics_setDirtyArea(g_i51PkigCanvas, &rect);
	    ven_graphics_setClipArea(g_i51PkigCanvas, &rect);
	    ven_graphics_screenUpdate( g_i51PkigCanvas, VEN_GRA_UPDATE_NORMAL);
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiUpdateScreenEx , E");
#endif	

}

iU32* i51AdeMmiGetPanel ()
{

 	//	author : osca
	//	since : 2011.4.20
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : ���ز����飬���д���˲��buffer ��ַ������
	//	array[0] ����˲���������鳤��= ��ĸ���* 3 + 1
	//	array[1...n] ����˲��buffer�Լ����
	//	�磺array[1] ��0�����Ļָ�롢array[2] ��0��Ŀ�array[3]��0��ĸ�
	//	array[4] Ϊ��1�����Ļָ�롢array[5]��array[6] ͬ�ϡ���0��Ϊ�����Դ桢����Ϊ����㡣
	//	MTK ��ʵ�ֿ���ֱ����ϵͳ�Ĳ���Դ��
	
	static iU32 array [4] = { 0 } ;	
	ven_GraphicDIB_t *CanvasDIB = NULL;
	
#ifdef i51ADAPTER_DEBUG
//	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetPanel , S");
#endif

    if (!g_i51PkigCanvas)
    {
		i51AdeOsLog(i51_LOG_SC, "ADE - MMI : Get Panel BE");
        return NULL;
    }

	CanvasDIB = ven_graphics_getCanvasDIB(g_i51PkigCanvas);
    if (CanvasDIB)
    {
        array[0] = 1 ; 
        array[1] = (iU32 )CanvasDIB->pData;
        array[2] = CanvasDIB->width;
        array[3] = CanvasDIB->height;
    }
	
#ifdef i51ADAPTER_DEBUG
//	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetPanel , E , %x", array);
#endif

	return array ;

}

void i51AdeMmiSetPanelUsed ( void* panel ) 
{


 	//	author : osca
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	return ;

}

void i51AdeMmiRecorderGetDataCb(void* pData, u32 nDataLen)
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderGetDataCb , S");
#endif	

	if(i51Recoder.bRecord == iFALSE)
	{
		return;
	}

	if(i51Recoder.appcb && gi51IsRunning)
	{
		i51Recoder.appcb(pData , nDataLen);
	}
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderGetDataCb , E");
#endif	
}

void i51AdeMmiRecorderStopCb(iBOOL bResult)
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStopCb , S");
#endif

	if(i51Recoder.stopCb && gi51IsRunning)
	{
		i51Recoder.stopCb(bResult);
	}

	memset(&i51Recoder, 0, sizeof(i51Recoder));
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStopCb , E");
#endif	
}

void i51AdeMmiRecorderStatusCb(iBOOL bResult)
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStatusCb , S");
#endif

	if(i51Recoder.bRecord == iFALSE)
	{
		return;
	}

	if(i51Recoder.finishCb && gi51IsRunning)
	{
		i51Recoder.finishCb(bResult);
	}

	memset(&i51Recoder, 0, sizeof(i51Recoder));

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStatusCb ,  E");
#endif
	
}



//����չ�ӿڣ�Ĭ��¼��PCM
iS32 i51AdeMmiRecorderStart ( void* buffer , iU32 bufflen , void (*callback ) ( void* stream , iU32 length ) ) 
{  

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStart , S , %x , %d , %x ", buffer, bufflen, callback);
#endif

    if(_i51AdeMmiMediaPlayerIsWorking())
    {
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStart , F, MediaPlayer working..");
		return iFALSE;
	}

	if(i51Recoder.bRecord == iTRUE)
	{
		i51AdeOsLog(i51_LOG_SC, "ADE - MMI : i51AdeMmiRecorderStart , F, Recorder working..");
		return iFALSE;
	}
	
   	memset(&i51Recoder,0,sizeof(i51RecoderStruct));
	i51Recoder.bRecord = iTRUE;
	i51Recoder.mediaType = i51_REC_PCM;
	i51Recoder.appcb = callback;
	i51Recoder.obj = buffer;
    i51Recoder.len = bufflen;
	
	g_i51RecorderActionFlag = I51_RECORDER_ACTION_START_PCM;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStart , E");
#endif
	
    return iTRUE;
	
}


iBOOL i51AdeMmiRecorderStartEx ( iU16* filename , void (*callback ) ( iBOOL finished ) ) 
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStartEx , S");
#endif

	if(_i51AdeMmiMediaPlayerIsWorking())
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStartEx , MediaPlayer working..");
		return iFALSE;
	}
	
	if(i51Recoder.bRecord == iTRUE)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStartEx , Recorder is working");
		return iFALSE;
	}
		
	memset(&i51Recoder,0,sizeof(i51RecoderStruct));
	i51Recoder.bRecord = iTRUE;
	i51Recoder.mediaType = i51_REC_AMR;
	i51Recoder.finishCb = callback;
	i51Recoder.obj = (void*)filename;
    i51Recoder.len = 0;
	ven_std_wstrcpy ( (iU16*)&i51Recoder.fileName[0], (iU16*)L"fs:") ;	
	ven_std_wstrcat ((iU16*)&i51Recoder.fileName[0],i51S_AdeFileFixPath(filename) );

	g_i51RecorderActionFlag = I51_RECORDER_ACTION_START_AMR;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStartEx , E");
#endif

	return iTRUE;
	
}


iBOOL i51AdeMmiRecorderStop ( void (*callback) ( iBOOL results )) 
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStop , S");
#endif

	if(i51Recoder.bRecord == iFALSE)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStop , ERR , already stop");
		return iFALSE;
	}

	i51Recoder.stopCb = callback;

	i51Recoder.bRecord = iFALSE;

	if(i51Recoder.mediaType == i51_REC_PCM)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStop , PCM");
		g_i51RecorderActionFlag = I51_RECORDER_ACTION_STOP_PCM;
	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStop , AMR");
		g_i51RecorderActionFlag = I51_RECORDER_ACTION_STOP_AMR;
		
	}

#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderStop , E");
#endif

	return iTRUE;
}

iBOOL i51AdeMmiRecoderPlay ( void* stream , iU32 length , void (*callback) ( iBOOL results ) ) 
{
 	iU32 ret = 0 ,codec= 0xff;
	ven_req_data_t ptVenReq;
	ven_codec_mediaFormat_t eFormat;

#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecoderPlay , S , %x,%d,%x", stream, length, callback);
#endif

    if(_i51AdeMmiMediaPlayerIsWorking())
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecoderPlay , ERR, MediaPlayer working..");
		return iFALSE;
	}

    i51PlayRecordCb = callback;

    ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iFALSE;
	ptVenReq.pfnCb = i51S_AdeMmiRecoderPlayCB;
	ptVenReq.userData = (iU32)stream;

	ret = ven_codec_playAudioRaw(g_i51MediaHandle, VEN_MEDIA_FORMAT_WAVE, (iU8*)stream, length, g_i51LastVolume, 0, &ptVenReq);

#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecoderPlay , S , %d" , ret);
#endif

    return (ret==VEN_CODEC_SUCCESS); 
    
}

iBOOL i51AdeMmiRecorderPlayStop (void)
{
    ven_req_data_t ReqData = {0};
    iU32 ret=0;

#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderPlayStop , S");
#endif
	
    ReqData.Handle = g_i51PkigHandle;
	ReqData.bSync = iTRUE;

    if ( 0 == i51PlayRecordCb ) {
        i51AdeOsLog ( "MMI : i51AdeMmiRecorderPlayStop , ERR , Recoder-Play No Need Stop" ) ;
        return iTRUE ;
    }

	ret = ven_codec_stopAudio(g_i51MediaHandle, &ReqData);

	i51PlayRecordCb = 0 ;

#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiRecorderPlayStop , E , %d" , ret);
#endif
	
    return (ret == 0); 

}

void i51AdeMmiRecorderFormat ( iU32* samples , iU32* bitscale , iU32* chanels )
{

    *samples = 8*1024;
    *bitscale = 16;
    *chanels = 1;

}

void i51AdeMmiGetTextAttr ( iU16* text , iU16* textwidth , iU16* textheight ) 
{
    //	author : osca
    //	since : 20111228
    //	PKIG Tech. Co., Ltd.
    //	����
	iU32 width=0,height=0;
    iU16 pAscent,pDescent,pLeading;
    
#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetTextAttr , S , %x , %x, %x", text,textwidth,textheight);
#endif

    if (!g_i51PkigFontNormal)
    {
        i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetTextAttr , Get Text-Attr F");
        return;  
    }
    ven_font_getTextWidth(g_i51PkigFontNormal, text, &width);
    ven_font_getFontInfo(g_i51PkigFontNormal, &height, &pAscent, &pDescent, &pLeading);

	*textwidth = width;
	*textheight = height;
	
#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiGetTextAttr ,  E");
#endif

}

void i51AdeMmiDrawText ( iU8* panel , iU16 width , iU16 height , const iU16* text , iS32 desx , iS32 desy , iCOLOR color ) 
{

    //	author : jone
    //	since : 20111010
    //	PKIG Tech. Co., Ltd.
    //	����

#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiDrawText , S , %x,%d,%d,%x,%d,%d,%d", panel,width,height, text, desx, desy, color);
#endif

    if (!g_i51PkigCanvas ||!text||!panel)
    {
        i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiDrawText , Draw Text F");
        return;  
    }
    ven_graphics_setColor( g_i51PkigCanvas, VEN_CLR_TEXT,  VEN_MAKE_RGB888( R_OF_565(color),G_OF_565(color),B_OF_565(color) )   );  
    ven_graphics_drawText(g_i51PkigCanvas, g_i51PkigFontNormal, text , desx, desy);
#ifdef i51ADAPTER_DEBUG
    i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiDrawText , E");
#endif
}

iS32 i51AdeMmiMediaPlayerFormat (void)
{
	//	author : jone
	//	since : 2012428
	//	PKIG Tech. Co., Ltd.

	iS32 format;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI :  i51AdeMmiMediaPlayerFormat ,  S ");
#endif

	format = i51_MEDIA_TYPE_MIDI|i51_MEDIA_TYPE_WAV|i51_MEDIA_TYPE_MP3|i51_MEDIA_TYPE_AMR;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerFormat , E , %d", format);
#endif

    return format;
}

iBOOL i51AdeMmiCreate ( i51ADECALLBACK callback ) 
{

 	//	author : osca
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	ven_GraphicsDisplayInfo_t DispInfo;
	ven_req_data_t ReqData = {0};
	iU32 timerid;
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiCreate , S , %x", callback);
#endif

	i51AdeCallback = callback ;
	timerid = i51AdeOsTimerCreate ( 0 , 0) ;
	i51AdeOsTimerStart ( timerid, i51S_AdeMmiCreateCB, 20);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiCreate ,  E");
#endif
	return ( 1 ) ;		
}

iBOOL i51AdeMmiRelease ( void ) 
{

 	//	author : osca
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	
	i51AdeCallback = 0;
		
	i51MediaPlayerManager.bCreated = iFALSE;
	return (1) ;	
		
}


/*========================================================
	Media Player -- start
=========================================================*/
static iBOOL _i51AdeMmiMediaPlayerIsWorking()
{
	if(!i51MediaPlayerManager.bCreated)
	{
		return iFALSE;
	}
	if(i51MediaPlayerManager.bPlaying || i51MediaPlayerManager.bBusyPlaying || i51MediaPlayerManager.bBusyStopping)
	{
		return iTRUE;
	}
	return iFALSE;
}


//����������
static iBOOL _i51AdeMmiMediaPlayerManagerDataInit(void (*pfnCreateCb) ( iBOOL results ))
{
	iU8 i;
	
	memset(&i51MediaPlayerAudioPlayList[0], 0, sizeof(i51MediaPlayerAudioPlayList));
	memset(&i51MediaPlayerMessageQueue[0], 0, sizeof(i51MediaPlayerMessageQueue));
	memset(&i51MediaPlayerManager, 0, sizeof(i51MediaPlayerMgrStruct));

	
	i51MediaPlayerManager.bCreated = iTRUE;
	i51MediaPlayerManager.pfnCreateCb = pfnCreateCb;
	i51MediaPlayerManager.eMode = i51_PLAYMODE_INVALIDATE;

	for( i = 0; i <  I51_MEDIA_PLAYER_SECTOR_NUM; i++ )
	{
		i51MediaPlayerManager.streamSector[i].nPlayLen = 0;
		i51MediaPlayerManager.streamSector[i].nStreamLen = 0;
#if 0		
		i51MediaPlayerManager.streamSector[i].pStreamBuf = (iU8*)i51AdeOsMalloc(I51_MEDIA_PLAYER_SECTOR_SIZE, __FILE__, __LINE__);
		if(i51MediaPlayerManager.streamSector[i].pStreamBuf == NULL)
		{
			return iFALSE;
		}
		memset(i51MediaPlayerManager.streamSector[i].pStreamBuf, 0, I51_MEDIA_PLAYER_SECTOR_SIZE);
#endif		
	}

	i51MediaPlayerManager.sectorId = 0;

	return iTRUE;
}

//����������
static void _i51AdeMmiMediaPlayerManagerDataRelease()
{
	iU8 i;

#if 0			
	for( i = 0; i <  I51_MEDIA_PLAYER_SECTOR_NUM; i++ )
	{
		if(i51MediaPlayerManager.streamSector[i].pStreamBuf)
		{
			i51AdeOsFree(i51MediaPlayerManager.streamSector[i].pStreamBuf);
		}
	}
#endif
	memset(&i51MediaPlayerAudioPlayList[0], 0, sizeof(i51MediaPlayerAudioPlayList));
	memset(&i51MediaPlayerMessageQueue[0], 0, sizeof(i51MediaPlayerMessageQueue));
	memset(&i51MediaPlayerManager, 0, sizeof(i51MediaPlayerMgrStruct));

	i51MediaPlayerManager.bCreated = iFALSE;
}

//ͨ����������handle��ȡ����Id
static iU8 _i51AdeMmiMediaPlayerManagerGetRestNum()
{
	iU8 i, num = 0;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(!i51MediaPlayerAudioPlayList[i].bUsed)
		{
			num ++;
		}
	}

	return num;
}


//ͨ����������handle��ȡ����Id
static iU8 _i51AdeMmiMediaPlayerManagerGetIdByHandle(void* handle)
{
	iU8 i;
	iU8 nId = I51_MEIDA_PLAYER_INVALIDATE_ID;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(i51MediaPlayerAudioPlayList[i].bUsed && i51MediaPlayerAudioPlayList[i].handle == handle)
		{
			nId = i;
			break;
		}
	}

	return nId;
}

//�����µĲ��Ŷ�����ӵ����Ŷ���
static iBOOL _i51AdeMmiMediaPlayerManagerAddNewObject(iU32 priority , void* handle , iU32 length , 
		iU32 codec , iU32 dec  , iBOOL loop , void (*STOP)(iHANDLE handle)  )

{
	iU8 i;
	iBOOL bAddSuccess = iFALSE;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(!i51MediaPlayerAudioPlayList[i].bUsed)
		{
			i51MediaPlayerAudioPlayList[i].handle = handle;
			i51MediaPlayerAudioPlayList[i].priority = priority;
			i51MediaPlayerAudioPlayList[i].length = length;
			i51MediaPlayerAudioPlayList[i].codec = codec;
			i51MediaPlayerAudioPlayList[i].dec = dec;
			i51MediaPlayerAudioPlayList[i].loop = loop;
			i51MediaPlayerAudioPlayList[i].pfnStopCallback = STOP;
			i51MediaPlayerAudioPlayList[i].bUsed = iTRUE;
			i51MediaPlayerAudioPlayList[i].status = i51_PLAYSTATUS_PLAYING;
			i51MediaPlayerAudioPlayList[i].pos = 0;
			bAddSuccess = TRUE;
			break;
		}
	}

	return bAddSuccess;
}

//�Ӳ��Ŷ������Ƴ�ĳ���Ŷ���(���)
static void _i51AdeMmiMediaPlayerManagerRemoveObjectByHandle(void* handle)
{
	iU8 i;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(i51MediaPlayerAudioPlayList[i].bUsed && i51MediaPlayerAudioPlayList[i].handle == handle)
		{
			memset(&i51MediaPlayerAudioPlayList[i], 0, sizeof(i51MediaPlayerObjStruct));
			break;
		}
	}
}

//�Ӳ��Ŷ������Ƴ�ĳ���Ŷ���(����Id)
static void _i51AdeMmiMediaPlayerManagerRemoveObjectById(iU8 id)
{
	
	if(id < I51_MEDIA_PLAYER_OBJECT_NUM)
	{
		memset(&i51MediaPlayerAudioPlayList[id], 0, sizeof(i51MediaPlayerObjStruct));
	}
}

//�Ƴ����ڲ��ŵĶ���
static void _i51AdeMmiMediaPlayerManagerRemovePlayingObject()
{
	iU8 i;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(i51MediaPlayerAudioPlayList[i].bUsed && i51MediaPlayerAudioPlayList[i].status == i51_PLAYSTATUS_PLAYING)
		{
			memset(&i51MediaPlayerAudioPlayList[i], 0, sizeof(i51MediaPlayerObjStruct));
		}
	}
}


//�Ƴ�ȫ�����Ŷ���
static void _i51AdeMmiMediaPlayerManagerRemoveAllObject()
{
	iU8 i;
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		memset(&i51MediaPlayerAudioPlayList[i], 0, sizeof(i51MediaPlayerObjStruct));
	}
}

//�����������ڲ��Ŷ����STOP�ص�����
static void _i51AdeMmiMediaPlayerManagerCallAllStopCb()
{
	iU8 i;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(i51MediaPlayerAudioPlayList[i].bUsed && i51MediaPlayerAudioPlayList[i].status == i51_PLAYSTATUS_PLAYING)
		{
			if(i51MediaPlayerAudioPlayList[i].pfnStopCallback)
				i51MediaPlayerAudioPlayList[i].pfnStopCallback(i51MediaPlayerAudioPlayList[i].handle);
		}
	}
}


//�Ӳ��Ŷ����л�ȡ���Ŷ���(���)
static i51MediaPlayerObjStruct* _i51AdeMediaPlayerManagerGetObjectByHandle(void* handle)
{
	iU8 i;
	i51MediaPlayerObjStruct* pObj = NULL;

		
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(i51MediaPlayerAudioPlayList[i].bUsed && i51MediaPlayerAudioPlayList[i].handle == handle)
		{
			pObj = &i51MediaPlayerAudioPlayList[i];
			break;
		}
	}

	return pObj;
}

//�Ӳ��Ŷ����л�ȡ���Ŷ���(����ID)
static i51MediaPlayerObjStruct* _i51AdeMediaPlayerManagerGetObjectById(iU8 id)
{
	i51MediaPlayerObjStruct* pObj = NULL;

	
	if(id < I51_MEDIA_PLAYER_OBJECT_NUM)
	{
		pObj = &i51MediaPlayerAudioPlayList[id];
	}

	return pObj;
}

static iU8 _i51AdeMediaPlayerManagerGetObjectNumber()
{
	iU8 i, num = 0;

	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		if(i51MediaPlayerAudioPlayList[i].bUsed)
		{
			num++;
		}
	}

	return num;

}

//�ж�ĳ��Դ�Ƿ����ڲ���״̬
static iBOOL _i51AdeMediaPlayerManagerAudioIsPlaying(void* handle)
{
	i51MediaPlayerObjStruct* pObj = NULL;


	if(i51MediaPlayerManager.bPlaying)
	{
		pObj = _i51AdeMediaPlayerManagerGetObjectByHandle(handle);
		if(pObj != NULL)
		{
			if(pObj->status == i51_PLAYSTATUS_PLAYING || pObj->status == i51_PLAYSTATUS_PAUSE)
			{
				return iTRUE;
			}
		}
	}

	return iFALSE;
}

//������Ϣ
static iBOOL _i51AdeMediaPlayerManagerSendMessage(void* handle, i51MEDIAOPERATOR eOpreator)
{
	iU8 i;
	iBOOL ret = iFALSE;
	
	for(i = 0; i < I51_MEDIA_PLAYER_MESSAGE_QUEUE_SIZE; i++)
	{
		if(i51MediaPlayerMessageQueue[i].bEnable == iFALSE)
		{
			i51MediaPlayerMessageQueue[i].handle = handle;
			i51MediaPlayerMessageQueue[i].eOpreator = eOpreator;
			i51MediaPlayerMessageQueue[i].bEnable = iTRUE;
			ret = iTRUE;
			break;
		}
	}
	return ret;
}

//������Ϣ
static iBOOL _i51AdeMediaPlayerManagerSendMessageWithParam(void* handle, i51MEDIAOPERATOR eOpreator, i51MediaPlayerParamDataStruct* pDataParam)
{
	iU8 i;
	iBOOL ret = iFALSE;
	
	for(i = 0; i < I51_MEDIA_PLAYER_MESSAGE_QUEUE_SIZE; i++)
	{
		if(i51MediaPlayerMessageQueue[i].bEnable == iFALSE)
		{
			i51MediaPlayerMessageQueue[i].handle = handle;
			i51MediaPlayerMessageQueue[i].eOpreator = eOpreator;
			i51MediaPlayerMessageQueue[i].bEnable = iTRUE;
			memcpy(&i51MediaPlayerMessageQueue[i].stDataParam, pDataParam, sizeof(i51MediaPlayerParamDataStruct));
			ret = iTRUE;
			break;
		}
	}
	return ret;
}


//��ȡ��Ϣ
static i51MediaPlayerMessageStruct* _i51AdeMediaPlayerManagerGetMessage()
{
	if(i51MediaPlayerMessageQueue[0].bEnable)
	{
		return &i51MediaPlayerMessageQueue[0];
	}
	else
	{
		return NULL;	
	}
}

//�Ƴ���Ϣ
static iBOOL _i51AdeMediaPlayerManagerRemoveMessage()
{
	iU8 i;

	if(!i51MediaPlayerMessageQueue[0].bEnable)
	{
		return iFALSE;
	}
	
	for( i = 1; i < I51_MEDIA_PLAYER_MESSAGE_QUEUE_SIZE; i ++)
	{
		if(i51MediaPlayerMessageQueue[i].bEnable)
			ven_std_memcpy(&i51MediaPlayerMessageQueue[i - 1], &i51MediaPlayerMessageQueue[i], sizeof(i51MediaPlayerMessageStruct));
		else
			break;
	}

	i51MediaPlayerMessageQueue[i - 1].bEnable = FALSE;

	return iTRUE;
}


//���ļ��ж�ȡһ������
static iBOOL _i51AdeMediaPlayerGetFileData(const iU16* path, void* data, iU32 pos, iU32 len )
{
	iFILE hFile = 0 ;
	iU32 readByte = 0;
	iBOOL ret = FALSE;
	
	hFile = i51AdeOsFileOpen(path, i51_OS_FL_READ|i51_OS_FL_EXISTING_OPEN, __FILE__, __LINE__);
	if(hFile)
	{
		do
		{
			ret = i51AdeOsFileSetSeek(hFile, pos, VEN_FILE_SEEK_SET);
			if(!ret) 
				break;

			ret = i51AdeOsFileRead(hFile, data, len, &readByte);
			if(!ret)
				break;

			if(readByte != len)
			{
				ret = iFALSE;
				break;
			}
		}while(0);

		i51AdeOsFileClose(hFile);
	}

	return ret;
}


//�ж���Ƶ�����Ƿ�֧�ֻ���--Ŀǰֻ֧����ͬ�����ʵ�WAV��ʽ�ļ����л������Ҳ����ʱ�����8��������(ֻȡ���ֽڣ����㷨)
static i51WavSampleStruct* _i51AdeMediaPlayerMixFormatSupport(void* handle , iU32 length , iU32 codec, iU32 dec)
{
	iS8 data[I51_WAV_HEAD_LEN];
	wavheader_t* pWavHead = NULL;
	iU8 i;
	i51WavSampleStruct* pSample = NULL;
	
	if(codec == VEN_MEDIA_FORMAT_WAVE)
	{
		if(dec == i51_MMI_DEC_FILE_BASED)
		{
			ven_std_memset(&data[0], 0, I51_WAV_HEAD_LEN);
			if(_i51AdeMediaPlayerGetFileData((const iU16*)handle, &data[0], 0, I51_WAV_HEAD_LEN))
			{
				pWavHead = (wavheader_t*)&data[0];
			}
		}
		else
		{
			pWavHead = (wavheader_t*)handle;
		}

		if(pWavHead)
		{
			for(i =0 ; i < sizeof(stWavSample) / sizeof(i51WavSampleStruct); i++)
			{
				if(pWavHead->sample == stWavSample[i].sampleValue)
				{
					pSample = &stWavSample[i];
					break;
				}
			}
		}
	}
	
	return pSample;
}

//�жϵ�ǰ�Ƿ���Բ��Ż�����Ƶ(�����ǰû��������Ƶ�ڲ��ţ���֧��; �����ǰ�ڲ��ŵ�ģʽ�ǻ��������Ҳ�������ͬ����֧��)
static iBOOL _i51AdeMediaPlayerMixCanPlay(i51WavSampleStruct* pWavSample)
{
	if(!i51MediaPlayerManager.bPlaying)
	{
		return iTRUE;
	}

	if(i51MediaPlayerManager.eMode == i51_PLAYMODE_MIX && i51MediaPlayerManager.pSample->eSampleType == pWavSample->eSampleType)
	{
		return iTRUE;
	}
	else
	{
		return iFALSE;
	}
}

static void _i51AdeMediaPlayerLoadMixData()
{

}

static iBOOL _i51AdeMediaPlayerJudgePriority(iU32 priority)
{
	iU8 i;
	iBOOL bHigh = iTRUE;
	
	for(i = 0; i < I51_MEDIA_PLAYER_OBJECT_NUM; i++)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerJudgePriority new priority = %d,  bUsed = %d, priority = %d, i = %d", 
			priority, i51MediaPlayerAudioPlayList[i].bUsed, i51MediaPlayerAudioPlayList[i].priority, i);
		
		if(i51MediaPlayerAudioPlayList[i].bUsed && i51MediaPlayerAudioPlayList[i].priority < priority
			&& i51MediaPlayerAudioPlayList[i].status != i51_PLAYSTATUS_STOP)
		{
			bHigh = FALSE;
			break;
		}
	}
	return bHigh;
}

static void _i51AdeMediaPlayerResetManager()
{
	iU8 i;
	
	i51MediaPlayerManager.bPlaying = iFALSE;
	i51MediaPlayerManager.bBusyPlaying = iFALSE;
	i51MediaPlayerManager.bBusyStopping = iFALSE;
	i51MediaPlayerManager.eMode = i51_PLAYMODE_INVALIDATE;
	i51MediaPlayerManager.pSample = NULL;
	i51MediaPlayerManager.sectorId = 0;
	for( i = 0; i <  I51_MEDIA_PLAYER_SECTOR_NUM; i++ )
	{
		i51MediaPlayerManager.streamSector[i].nPlayLen = 0;
		i51MediaPlayerManager.streamSector[i].nStreamLen = 0;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////
//Device Control
////////////////////////////////////////////////////////////////////////////////////////////

static void _i51AdeMediaPlayerDeviceStartSinglePlay_Cb(u32 UserData, u32 RspData)
{
	ven_codec_ret_t retCode = RspData;
	void* handle = (void*)UserData;
	i51MediaPlayerObjStruct* pAudioObj = _i51AdeMediaPlayerManagerGetObjectByHandle(handle);

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay_Cb. retCode = %d, handle = 0x%x", retCode, handle);

	if(!i51MediaPlayerManager.bCreated && retCode != VEN_CODEC_PLAY_FINISHED)
	{
		//�豸��æʱ���ͷţ��˴�ֹͣ����
		_i51AdeMediaPlayerDeviceStopPlayImmediately();
		return;
	}

	if(!pAudioObj)		//Debug: ����MStarƽ̨�쳣�������Stop�Ѿ��ɹ����ú�Play Callback�Ի�֪ͨFinish��Ϣ
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay_Cb. NULL Object, return");
		return;
	}

	if ( retCode == VEN_CODEC_PLAY_FINISHED )
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay_Cb. Play finish");
		if(pAudioObj->pfnStopCallback)
				pAudioObj->pfnStopCallback(handle);

		_i51AdeMmiMediaPlayerManagerRemoveAllObject();

		_i51AdeMediaPlayerResetManager();
	}
	else if( retCode == VEN_CODEC_SUCCESS )
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay_Cb. Codec Success");
	}

	i51MediaPlayerManager.bBusyPlaying = iFALSE;

}

static void _i51AdeMediaPlayerDeviceStartSinglePlay(i51MediaPlayerParamDataStruct* pDataParam)
{
    iU32 ret = 0;
	ven_req_data_t ptVenReq;
	iU16 filename[128];

	if(pDataParam->dec== i51_MMI_DEC_FILE_BASED) 
	{
        ptVenReq.Handle = g_i51PkigHandle;
        ptVenReq.bSync = iFALSE ;
        ptVenReq.pfnCb = _i51AdeMediaPlayerDeviceStartSinglePlay_Cb;
        ptVenReq.userData = (iU32)pDataParam->handle ;
            	                     
        ven_std_wstrcpy ( filename , i51S_AdeFileFixPath((iU16*)pDataParam->handle) ) ;
	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay. Name Len = %d, loop = %d", ven_std_wstrlen(filename), pDataParam->loop);
    
        ret = ven_codec_playAudio (g_i51MediaHandle , 
                filename, 
                ven_std_wstrlen(filename) , 
                0 , 
                g_i51LastVolume,
                pDataParam->loop, 
                &ptVenReq ) ;

		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay. Play File   ret = %d", ret);

		if(ret == VEN_CODEC_SUCCESS)
		{
			i51MediaPlayerManager.bPlaying = iTRUE;
			i51MediaPlayerManager.bBusyPlaying = iTRUE;
		}

	}
	else if(pDataParam->dec == i51_MMI_DEC_MEM_BASED) 
	{
        	
		ptVenReq.Handle = g_i51PkigHandle;
		ptVenReq.bSync = iFALSE ;
		ptVenReq.pfnCb = _i51AdeMediaPlayerDeviceStartSinglePlay_Cb;
		ptVenReq.userData = (iU32)pDataParam->handle;

		ret = ven_codec_playAudioRaw (
		    g_i51MediaHandle , 
		    pDataParam->codec , 
		    (iU8*)pDataParam->handle , 
		    pDataParam->length , 
		    g_i51LastVolume , 
		    pDataParam->loop , 
		    &ptVenReq ) ; 

		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay    Play Memory   ret = %d", ret);
		
		if(ret == VEN_CODEC_SUCCESS)
		{
			i51MediaPlayerManager.bPlaying = iTRUE;
			i51MediaPlayerManager.bBusyPlaying = iTRUE;
		}
	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStartSinglePlay   Error dec");

		i51MediaPlayerManager.bBusyPlaying = iFALSE;
	}

}


static void _i51AdeMediaPlayerDeviceStartMixPlay(i51MediaPlayerParamDataStruct* pDataParam)
{
	//ven_codec_playAudioStreaming
}


static void _i51AdeMmiMediaPlayerDeviceStopCallback ( iU32 UserData , iU32 RspData )
{
	ven_codec_ret_t retCode = RspData;
	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerDeviceStopCallback. ret = %d", retCode);	

	i51MediaPlayerManager.bBusyStopping = iFALSE;
	
	if (VEN_CODEC_SUCCESS == retCode)
    {
        
    }
    else if (VEN_CODEC_NOT_PLAYING == retCode || VEN_CODEC_PLAY_FINISHED == retCode)
    {
        
    }
    else
    {
        _i51AdeMediaPlayerDeviceStopPlayImmediately();		//�첽STOPʧ��ֱ��ͬ��STOP
    }
}

static iBOOL _i51AdeMediaPlayerDeviceStopPlay()
{
	ven_req_data_t ptVenReq = {0};
	iU32 ret;

	if(!g_i51PkigHandle)
	{
		return iFALSE;
	}
		
	ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iFALSE ;
	ptVenReq.userData = 0 ;
	ptVenReq.pfnCb = _i51AdeMmiMediaPlayerDeviceStopCallback;
	ret = ven_codec_stopAudio(g_i51MediaHandle, &ptVenReq);
	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStopPlay    ret = %d", ret);	

	if(ret == VEN_CODEC_SUCCESS)
	{
		i51MediaPlayerManager.bBusyStopping = iTRUE;
		i51MediaPlayerManager.bPlaying = iFALSE;
		return iTRUE;
	}
	else if(ret == VEN_CODEC_PLAY_FINISHED || ret == VEN_CODEC_NOT_PLAYING)	//lynn edit, 20120716, �����쳣����
	{
		i51MediaPlayerManager.bBusyStopping = iFALSE;
		i51MediaPlayerManager.bPlaying = iFALSE;
		return iTRUE;
	}

	return iFALSE;
}

static iBOOL _i51AdeMediaPlayerDeviceStopPlayImmediately()
{
	ven_req_data_t ptVenReq = {0};
	iU32 ret;

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStopPlayImmediately E");	

	if(!g_i51PkigHandle || !g_i51MediaHandle)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStopPlayImmediately Error 11111111111");	
		return iFALSE;
	}
		
	ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iTRUE ;
	ptVenReq.userData = 0 ;
	ptVenReq.pfnCb = 0;
	ret = ven_codec_stopAudio(g_i51MediaHandle, &ptVenReq);

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMediaPlayerDeviceStopPlayImmediately    ret = %d", ret);	

	return ret == VEN_CODEC_SUCCESS ? iTRUE : iFALSE ;
}

////////////////////////////////////////////////////////////////////////////////////////////
//Message Process
////////////////////////////////////////////////////////////////////////////////////////////
static iBOOL _i51AdeMmiMediaPlayerMessageProcessing_Play(i51MediaPlayerMessageStruct* pMsg)
{
	i51WavSampleStruct*	pSample = NULL;
	i51MediaPlayerParamDataStruct* pDataParam = &pMsg->stDataParam;
	iBOOL bJoinMix = iFALSE;
	iBOOL bHandled = iTRUE;

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play handle = 0x%x", pDataParam->handle);
	
#if 0
	//�Ƿ�֧�ֻ���
	if(pDataParam->codec == VEN_MEDIA_FORMAT_WAVE)			
	{
		pSample = _i51AdeMediaPlayerMixFormatSupport(pDataParam->handle, pDataParam->length, pDataParam->codec, pDataParam->dec);
		if(pSample)
		{
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play --- Mix Supprot, sample = %d", pSample->sampleValue);
			if(i51MediaPlayerManager.bPlaying == iFALSE)
			{
				bJoinMix = iTRUE;
			}
			else if(i51MediaPlayerManager.bPlaying == iTRUE && i51MediaPlayerManager.eMode == i51_PLAYMODE_MIX
				&& i51MediaPlayerManager.pSample == pSample)
			{
				bJoinMix = iTRUE;
			}
		}
	}
#endif
	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play. bJoinMix = %d", bJoinMix);

	if(!i51MediaPlayerManager.bPlaying)
	{//ֹͣ״̬
		
		if(bJoinMix)
		{//����ģʽ
			i51MediaPlayerManager.eMode = i51_PLAYMODE_MIX;
			i51MediaPlayerManager.pSample = pSample;
			i51MediaPlayerManager.bPlaying = iTRUE;
			i51MediaPlayerManager.bBusyPlaying = iTRUE;
			
			_i51AdeMediaPlayerLoadMixData();
			_i51AdeMediaPlayerDeviceStartMixPlay(pDataParam);
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play. Start Mix Play");
		}
		else
		{//����ģʽ
			i51MediaPlayerManager.eMode = i51_PLAYMODE_SINGLE;
			i51MediaPlayerManager.pSample = 0;
		
			_i51AdeMmiMediaPlayerManagerAddNewObject(
					pDataParam->priority, 
					pDataParam->handle, 
					pDataParam->length,
					pDataParam->codec,
					pDataParam->dec,
					pDataParam->loop,
					pDataParam->STOP);
			_i51AdeMediaPlayerDeviceStartSinglePlay(pDataParam);
			
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play. Start Single Play");
		}
		
	}
	else													
	{//���ڲ���
		if(bJoinMix)		//�ɼ������������������װ������,��������
		{
			_i51AdeMediaPlayerLoadMixData();

			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play. Continue Mix Play");
		}
		else				//���ɼ�����������֮ǰ���ţ����²�������Դ
		{
			if(_i51AdeMediaPlayerJudgePriority(pDataParam->priority))
			{
				//���ȼ��ϸ߻���ͬ����ֹ֮ͣǰ�Ĳ��ţ��ټ�������
				i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play. Single Play New Audio");
				
				_i51AdeMediaPlayerDeviceStopPlay();

				_i51AdeMmiMediaPlayerManagerCallAllStopCb();

				_i51AdeMmiMediaPlayerManagerRemovePlayingObject();

				i51MediaPlayerManager.pSample = 0;
				i51MediaPlayerManager.eMode = i51_PLAYMODE_SINGLE;

				bHandled = iFALSE;
			}
			else
			{//���ȼ��ϵͣ�������������ֱ��֪ͨ�û��������

				i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Play. Single Play Ignore");
				
				if(pDataParam->STOP)
					pDataParam->STOP(pDataParam->handle);
			}
		}
	}
	
	return bHandled;
}

static iBOOL _i51AdeMmiMediaPlayerMessageProcessing_Pause(i51MediaPlayerMessageStruct* pMsg)
{
	ven_req_data_t ptVenReq;
	iU32 ret;
	
	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Pause. handle = 0x%x", pMsg->handle);

	if( iTRUE != i51MediaPlayerManager.bPlaying )
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Pause. No audio playing");
       	 	return iTRUE ;
	}

	ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iTRUE;
	ret = ven_codec_pauseAudio(g_i51MediaHandle, &ptVenReq);

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Pause ret = %d", ret);

	return iTRUE;
}

static iBOOL _i51AdeMmiMediaPlayerMessageProcessing_Resume(i51MediaPlayerMessageStruct* pMsg)
{
	ven_req_data_t ptVenReq;
	iU32 ret;

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Resume. handle = 0x%x", pMsg->handle);

	if( iTRUE != i51MediaPlayerManager.bPlaying )
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Resume. No audio playing ");
		 return iTRUE ;
	}

	ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iTRUE;
	ret = ven_codec_resumeAudio(g_i51MediaHandle, &ptVenReq);

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Resume ret = %d", ret);

	return iTRUE;
}

static iBOOL _i51AdeMmiMediaPlayerMessageProcessing_Stop(i51MediaPlayerMessageStruct* pMsg)
{
	i51MediaPlayerObjStruct* pAudioObj = NULL;
	iBOOL bHandled = iTRUE;

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. handle = 0x%x", pMsg->handle);

	pAudioObj = _i51AdeMediaPlayerManagerGetObjectByHandle(pMsg->handle);
	if(!pAudioObj)
	{
		//��ֹͣ�����ڲ��Ŷ�����
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. Object is null");
		return bHandled;
	}

	if(i51MediaPlayerManager.bPlaying == iFALSE)
	{
		//δ����
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. Already stopped");
		_i51AdeMmiMediaPlayerManagerRemovePlayingObject();
		return bHandled;
	}

	if(i51MediaPlayerManager.eMode == i51_PLAYMODE_MIX)	
	{
		//��������ģʽ��ֹֻͣ��Ҫֹͣ��Ƶ���ݵ�Merage
		if(_i51AdeMmiMediaPlayerManagerGetIdByHandle(pAudioObj->handle) != I51_MEIDA_PLAYER_INVALIDATE_ID)
		{
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. Mix...Remove Object");

			if(pAudioObj->pfnStopCallback != 0)
				pAudioObj->pfnStopCallback(pAudioObj->handle);
			
			_i51AdeMmiMediaPlayerManagerRemoveObjectByHandle(pAudioObj->handle);
		}

		//ֻ�е���Ҫ�����Ķ������Ϊ0ʱ��ֹͣ�豸
		if(_i51AdeMediaPlayerManagerGetObjectNumber() == 0)
		{
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. Mix...Stop Device");

			_i51AdeMediaPlayerDeviceStopPlay();

			_i51AdeMmiMediaPlayerManagerRemovePlayingObject();
		}
		else
		{
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop.  Mix...Stop Merage Only");
		}
	}
	else if(i51MediaPlayerManager.eMode == i51_PLAYMODE_SINGLE)	
	{
		//�ǻ���ģʽ����Ҫֹͣ�豸
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. Single..Stop Device");
		
		_i51AdeMediaPlayerDeviceStopPlay();

		_i51AdeMmiMediaPlayerManagerRemovePlayingObject();

		i51MediaPlayerManager.bPlaying = iFALSE;

	}
	else
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing_Stop. Error play mode");
	}

	return bHandled;
}

static iBOOL _i51AdeMmiMediaPlayerMessageProcessing_Release(i51MediaPlayerMessageStruct* pMsg)
{
	i51AdeOsTimerStop(i51MediaPlayerManager.proccessTimerId);

	if(i51MediaPlayerManager.bPlaying)
	{
		_i51AdeMediaPlayerDeviceStopPlayImmediately();
	}	
	
	_i51AdeMmiMediaPlayerManagerDataRelease();		//Lynn add 20120606	
}

//��������Ϣ����
static void _i51AdeMmiMediaPlayerMessageProcessing( iU32 timerid )		
{
	i51MediaPlayerMessageStruct *pMsg = NULL;
	iBOOL bHandled = iFALSE;

	if(!g_i51MediaHandle)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Null meida handle");
		return;
	}

	if(!i51MediaPlayerManager.bCreated)
	{
		i51AdeOsTimerStop(i51MediaPlayerManager.proccessTimerId);		//�������ƣ���ֹTimer�޷�ֹͣ
		return;
	}

	if(i51MediaPlayerManager.bBusyPlaying || i51MediaPlayerManager.bBusyStopping)		//�豸��æ�ݲ���Ӧ��Ϣ����
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Busy Playing = %d, Busy Stopping = %d..... ",
		i51MediaPlayerManager.bBusyPlaying , i51MediaPlayerManager.bBusyStopping );
		return;
	}

	pMsg = _i51AdeMediaPlayerManagerGetMessage();
	if(!pMsg)								//û����Ҫ�������Ϣ
	{
		//i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Idle ...no message ");
		return;
	}
	if(!pMsg->handle || !pMsg->bEnable)		//��Ч��Ϣ
	{
		i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Error message ");
		_i51AdeMediaPlayerManagerRemoveMessage();	
		return;
	}

	

	switch(pMsg->eOpreator)					//��Ϣ�ַ�����
	{
		case i51_MEDIAOPERATOR_PLAY:
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Start processing PLAY ================= handle = 0x%x", pMsg->handle);
			bHandled = _i51AdeMmiMediaPlayerMessageProcessing_Play(pMsg);
			break;
			
		case i51_MEDIAOPERATOR_PAUSE:
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Start processing PAUSE================= handle = 0x%x", pMsg->handle);
			bHandled = _i51AdeMmiMediaPlayerMessageProcessing_Pause(pMsg);
			break;

		case i51_MEDIAOPERATOR_RESUME:
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Start processing RESUME================= handle = 0x%x", pMsg->handle);
			bHandled = _i51AdeMmiMediaPlayerMessageProcessing_Resume(pMsg);
			break;

		case i51_MEDIAOPERATOR_STOP:
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Start processing STOP================= handle = 0x%x", pMsg->handle);
			bHandled = _i51AdeMmiMediaPlayerMessageProcessing_Stop(pMsg);
			break;

		case i51_MEDIAOPERATOR_RELEASE:
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Start processing STOP================= handle = 0x%x", pMsg->handle);
			bHandled = _i51AdeMmiMediaPlayerMessageProcessing_Release(pMsg);
			break;

		default:
			i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing -- Cannot dispose the message  handle = 0x%x", pMsg->handle);
			bHandled = iTRUE;
			break;
	}

	if(bHandled)
		_i51AdeMediaPlayerManagerRemoveMessage();	//��Ϣ������ϣ��Ӷ������Ƴ���Ϣ

	i51AdeOsLog(i51_LOG_SC, "MMI :  _i51AdeMmiMediaPlayerMessageProcessing End.....");
	
}



//�����������ɹ�����ʼ��Ϣ����
static void _i51S_AdeMmiMediaPlayerCreateCB( iU32 timerid )
{	
	iBOOL bCreateSuccess = iTRUE;

	i51AdeOsLog(i51_LOG_SC, "MMI : _i51S_AdeMmiMediaPlayerCreateCB Entry");

	i51AdeOsTimerStop(timerid);
	
	if(!gi51IsRunning)
	{
		return;
	}

	do
	{
		i51MediaPlayerManager.proccessTimerId = i51AdeOsTimerCreate(1, 1);
		if(i51MediaPlayerManager.proccessTimerId == 0)
		{
			bCreateSuccess = iFALSE;
			break;
		}

		if(iFALSE == i51AdeOsTimerStart(i51MediaPlayerManager.proccessTimerId, _i51AdeMmiMediaPlayerMessageProcessing, 1))
		{
			bCreateSuccess = iFALSE;
			break;
		}
			
		if( 0 != i51MediaPlayerManager.pfnCreateCb )
		{
			i51MediaPlayerManager.pfnCreateCb ( bCreateSuccess ) ;
		}
		
	}while(0);

	i51AdeOsLog(i51_LOG_SC, "MMI : _i51S_AdeMmiMediaPlayerCreateCB -- Process Timer Id = %d", i51MediaPlayerManager.proccessTimerId);
}



////////////////////////////////////////////////////////////////////////////////////////////
// Adapter Interface
////////////////////////////////////////////////////////////////////////////////////////////

//����������
iBOOL i51AdeMmiMediaPlayerCreate ( void (*callback) ( iBOOL results ) )
{
	iU32 timerid;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerCreate , S ");
#endif
		
    if (i51MediaPlayerManager.bCreated == iTRUE)		//�Ѵ���
    {
    	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerCreate , ERR 1");
        return iFALSE;
    }
    if (!g_i51MediaHandle )
    {
    	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerCreate , ERR 2");
        return iFALSE;
    }

	if(!_i51AdeMmiMediaPlayerManagerDataInit(callback))
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerCreate , ERR 3");
		return iFALSE;
	}
	
    timerid = i51AdeOsTimerCreate(0, 0);
    i51AdeOsTimerStart(timerid, _i51S_AdeMmiMediaPlayerCreateCB, 50);

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerCreate , E ");
#endif

	return iTRUE;

}

//�رղ�����
iBOOL i51AdeMmiMediaPlayerRelease () 
{

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerRelease , S");
#endif
	
	if(!i51MediaPlayerManager.bCreated)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerRelease , ERR 1");
		return iFALSE;
	}

	//_i51AdeMediaPlayerManagerSendMessage(1, i51_MEDIAOPERATOR_RELEASE);
	i51AdeOsTimerStop(i51MediaPlayerManager.proccessTimerId);

	if(i51MediaPlayerManager.bPlaying && !i51MediaPlayerManager.bBusyPlaying)		//���ڲ��Ų����豸����,ֹͣ����
	{
		_i51AdeMediaPlayerDeviceStopPlayImmediately();
	}	
	
	_i51AdeMmiMediaPlayerManagerDataRelease();		//Lynn add 20120606	

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerRelease , E");
#endif

	return iTRUE;
}

////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

iBOOL i51AdeMmiMediaPlayerPlay ( iU32 priority , void* handle , iU32 length , iU32 type , iU32 dec  , iBOOL loop , void (*STOP)(iHANDLE handle)  ) 
{
	iU32 codec= 0xff;
	iU8 id;
	i51MediaPlayerParamDataStruct stParam = {0};

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , S , priority = %d, handle = 0x%x, length = %d, type = %d, dec = %d, loop = %d",
							priority , handle , length , type , dec, loop );
#endif
	
	if(!g_i51MediaHandle)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , ERR 1");
		return 0;
	}

	/*	����У��	*/
    if(!handle ||dec >i51_MMI_DEC_MEM_BASED || i51MediaPlayerManager.bCreated== 0)
    {
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , ERR 2");
        return 0;
    }
    switch ( type )
    {
	    case i51_MEDIA_TYPE_MIDI :
	        codec = VEN_MEDIA_FORMAT_MID ;
	        break ;
	    case i51_MEDIA_TYPE_WAV :
	        codec = VEN_MEDIA_FORMAT_WAVE ;
	        break ;
		case i51_MEDIA_TYPE_AMR :
			codec = VEN_MEDIA_FORMAT_AMR ;
			break ;
		case i51_MEDIA_TYPE_MP3 :
			codec = VEN_MEDIA_FORMAT_MP3 ;
			break ;
	    default:
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , ERR 3");
	        return 0;
    }
	if(dec == i51_MMI_DEC_FILE_BASED)
	{
		if(!i51AdeOsFileIsExist((const u16*)handle))
		{
			i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , ERR 4");
			return 0;
		}
	}

	/*	��ͬʱ���ŵĶ�������	*/
	if(!_i51AdeMmiMediaPlayerManagerGetRestNum())
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , ERR 5");
		return 0;
	}
	
	/*	������Ϣ	*/
	stParam.handle = handle;
	stParam.priority = priority;
	stParam.length = length;
	stParam.codec= codec;
	stParam.dec = dec;
	stParam.loop = loop;
	stParam.STOP = STOP;
	
	if(!_i51AdeMediaPlayerManagerSendMessageWithParam(handle, i51_MEDIAOPERATOR_PLAY, &stParam))
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , ERR 6");
		return 0;
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPlay , E");
#endif

	return 1;

}

iBOOL i51AdeMmiMediaPlayerPause ( void* handle  )
{
	i51MediaPlayerObjStruct* pObj = NULL;

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPause , S , 0x%x", handle);
#endif
	
	if(!g_i51MediaHandle)
	{
		return 0;
	}
	if(!handle)
	{
        return 0;
	}

	//��ȡ����
	pObj =  _i51AdeMediaPlayerManagerGetObjectByHandle(handle);
	if(!pObj)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerResume , ERR ,  Null Obj");
		return 0;
	}

	//������Ϣ
	if(!_i51AdeMediaPlayerManagerSendMessage(handle, i51_MEDIAOPERATOR_PAUSE))
	{
		return 0;
	}

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerPause , E");
#endif
	
	return 1;
	
}

iBOOL i51AdeMmiMediaPlayerResume ( void* handle  )
{
	i51MediaPlayerObjStruct* pObj = NULL;

#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerResume , S , 0x%x", handle);
#endif	

    if(!g_i51MediaHandle)
    {
        return 0;
    }

	if(!handle)
	{
        return 0;
	}

	//��ȡ����
	pObj =  _i51AdeMediaPlayerManagerGetObjectByHandle(handle);
	if(!pObj)
	{
		return 0;
	}

	//������Ϣ
	if(!_i51AdeMediaPlayerManagerSendMessage(handle, i51_MEDIAOPERATOR_RESUME))
	{
		return 0;
	}

#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerResume , E");
#endif	
	
	return 1;
	
}

iBOOL i51AdeMmiMediaPlayerStop ( void* handle  )
{
	i51MediaPlayerObjStruct* pObj = NULL;

#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerStop , S , 0x%x", handle);
#endif
	
	if(!g_i51MediaHandle)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerStop , ERR 1");
		return 0;
	}
	if(!i51MediaPlayerManager.bCreated)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerStop , ERR 2");
		return 0;
	}

	if(!handle)
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerStop , ERR 3");
		return 0;
	}

	//������Ϣ
	if(!_i51AdeMediaPlayerManagerSendMessage(handle, i51_MEDIAOPERATOR_STOP))
	{
		i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerStop , ERR 4");
		return 0;
	}

#ifdef i51ADAPTER_DEBUG	
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerStop , E");
#endif

	return 1;
	
}

iU32 i51AdeMmiMediaPlayerGetVolume ()
{

 	//	author : osca
	//	since : 2011.6.14
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : ��������������1= volume <=7.

	iU8 volume = 0 ;
	float rate = 0 ;
	ven_req_data_t ptVenReq={0};
	
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerGetVolume , S ");
#endif

	ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iTRUE;
	
	ven_codec_getAudioVolume(g_i51MediaHandle, &volume, &ptVenReq);
    if(volume == VEN_MEDIA_VOLUME_MAX)
    {
        volume = 7;
    }
    else if(volume == VEN_MEDIA_VOLUME_MIN)
    {
        volume = 1;
    }
    else if(volume == VEN_MEDIA_VOLUME_1)
    {
        volume = 2;
    }

#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerGetVolume , E , %d", volume);
#endif
	return volume ;	
	
}

iBOOL i51AdeMmiMediaPlayerSetVolume ( iU32 volume )
{

 	//	author : osca
	//	since : 2011.6.14
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : ����������0= volume <=8.
	//	�ɹ�����1 �� ʧ�ܷ���0

	iU32 ret = 0 ;
	float rate = 0 ;
	ven_req_data_t ptVenReq={0};
		
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerSetVolume , S  , %d",  volume);
#endif

    if(volume >7||volume <1)
    {
        i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerSetVolume , ERR , Media-Play Set Volume");
        return 0;
    }
    
    if(volume == 7)
    {
        volume = VEN_MEDIA_VOLUME_8;
    }
    else if(volume == 1)
    {
        volume = VEN_MEDIA_VOLUME_0;
    }
    else if(volume == 2)
    {
        volume = VEN_MEDIA_VOLUME_1;
    }

	ptVenReq.Handle = g_i51PkigHandle;
	ptVenReq.bSync = iTRUE;
	ret = ven_codec_setAudioVolume(g_i51MediaHandle, volume , &ptVenReq);

	g_i51LastVolume = volume;
#ifdef i51ADAPTER_DEBUG
	i51AdeOsLog(i51_LOG_SC, "MMI : i51AdeMmiMediaPlayerSetVolume , E , %d", ret);
#endif

	return ( ret == VEN_CODEC_SUCCESS ) ;
}

/*========================================================
	Media Player -- end
=========================================================*/


