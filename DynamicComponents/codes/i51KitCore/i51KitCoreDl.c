#include "i51KitAde.h"
#include "i51KitCore.h"

//http://sa.test.pkingame.net/Monitor.jsp
#define i51TESTHOST "sa.test.pkingame.net"
#define i51REALHOST "sa.pkingame.net"

//#define i51HTTP_PROXY_HEADER_PATTERN "POST /Monitor.jsp HTTP/1.1\r\nX-Online-Host: %s:%d\r\nConnection: Close\r\n"
//#define i51HTTP_HEADER_PATTERN "POST /Monitor.jsp HTTP/1.1\r\nHost: %s:%d\r\nConnection: Close\r\n"

/*
#define i51HTTP_PROXY_HEADER_PATTERN "POST /MainServlet HTTP/1.1\r\nX-Online-Host: %s:%d\r\nConnection: Close\r\n"
#define i51HTTP_HEADER_PATTERN "POST /MainServlet HTTP/1.1\r\nHost: %s:%d\r\nConnection: Close\r\n"
*/

#define i51HTTP_PROXY_HEADER_PATTERN	"POST /MainServlet%s HTTP/1.1\r\nHost: %s:%d\r\nAccept-Encoding: gzip\r\nX-Online-Host: %s:%d\r\nAccept-Language: zh-CN, en-US\r\nUser-Agent: NokiaN73-2/2.0626 S60/3.0 Profile/MIDP-2.0 Configuration/CLDC-1.1\r\nCookie: BAIDUID=81EC29DFA5EE5A49A0676CA23AC94F93:FG=1; BAIDU_WISE_UID=wiaui_1324128233_1167\r\nAccept: application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5\r\nAccept-Charset: utf-8, iso-8859-1, utf-16, *;q=0.7\r\n"
#define i51HTTP_HEADER_PATTERN	i51HTTP_PROXY_HEADER_PATTERN

//	�ַ���"Content-Length:  %9d\r\n\r\n" �ĳ���
#define i51HTTP_CONTENT_LENGTH 30

#define i51SlDlCAHCE_SIZE (2*1024) 
#define i51SlDlPATH_SIZE 64

typedef enum 
{

	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes : ������״̬
	
	i51_SAP_DL_STATE_IDLE ,
	i51_SAP_DL_STATE_INIT ,
	i51_SAP_DL_STATE_CONNECT ,
	i51_SAP_DL_STATE_SEND ,
	i51_SAP_DL_STATE_RECEIVE ,
	i51_SAP_DL_STATE_CLOSE ,
	i51_SAP_DL_STATE_RESTART ,
	
} i51DLSTATE ;

typedef enum
{

	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes : ��·������
	
	i51_SAP_DL_LINKLAYER_CMWAP ,
	i51_SAP_DL_LINKLAYER_CMNET ,

} i51DLLINKLAYER ;

typedef enum
{

	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes : ��������

	//	�Զ�����
	i51_SAP_DL_REQ_MODE_AUTO = 1 ,
	//	�������
	i51_SAP_DL_REQ_MODE_CLICK = 2 ,

} i51DLREQMODE; 

typedef enum
{

	//	author : Jelo
	//	since : 2011.6.25
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes : ����ģʽ

	//	������
	i51_SAP_DL_MODE_NEW = 0 ,
	//	��������
	i51_SAP_DL_MODE_BREAK ,	

} i51DLMODE ;

typedef struct
{

	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes : ������

	//	����״̬��ض�ʱ��
	iU32 timer ;

	//	����dl��ʱ��
	iU32 Createdltimer ;

	//	��ض�ʱ��������Ƿ�ʱ
	iU32 monitortimer ;
	//	��¼��ʱ,��ⳬʱ�õ�
	iU32 recordtime ;

	//	��ʱ��ʱ������
	iU32 monitortimerType ;

	//	ͷ���յ�ƫ����
	iU32 headRecvOffset ;

	i51DLSTATE state ;
	i51DLMODE mode ;

	void (*i51SlDlCreateCallback)(iBOOL ret) ;
		
	struct 
	{

		i51DLLINKLAYER type ;
		iBOOL successed ;
	
	} LinkLayer ;
	
	struct
	{
		iS32 id ;
		
		i51SOCADDR server ;

		//	datas that needs sending
		iU32 SendTotall ;
		iU32 SendOffset ;

		//	datas that needs receive
		iU32 RecvTotall ;	
		iU32 RecvOffset ;

		
		
		
	} Socket ;


	struct
	{
		i51DLREQMODE reqmode ;
		i51SlDlCOMMAND command ;
		i51SlDlCALLBACK callback ;
	} User ;

	struct		
	{
			
		//	����http Э��ͷ����
		iCHAR hskip ;
		//	����Э���Ӧͷ����
		iCHAR pskip ;
		
		iCHAR uri[50] ;

		//	���ص���������
		iCHAR name[50] ;
		//	�ļ�·��
		iCHAR path [ i51SlDlPATH_SIZE ] ;
		
		iFILE file ;
		iU32 urisize ;
		iU32 internal_crc ;
		iU32 final_crc ;

		//	�汾��
		iU32 versionId ;

		//	���������
		iU16 RefCount ;

		//	���������ܳ���
		iU32 reftotalsize ;

		//	totall size of the file 
		iU32 totallsize ;
		//	totall offset of the file been writen
		iU32 totalloffset ;
		iU32 wirten ;

		//	��ǰ���ص��ֽ�������
		iU32 bitssize ;
		//	��·id
		iU32	layerid ;
		//	�������Ƿ񻹴���
		iBOOL DownloaderIsExsit  ;

	} Download ;

	struct
	{
		iBOOL ParseHttpHeader ;
		iU32 waplen ;
		iU32 recvLen ;
		iCHAR *ContentBuf ;
		
	}wap ;

			//	���ڷ����Լ���������
	iU8 cache[i51SlDlCAHCE_SIZE] ;
			
} i51SlDl ;

typedef struct  {

	//	i51SlDlCALLBACK ����
	iS32 state ;
	iU32 textlen ;
	iU32 offset ;
	iU32 totall ;
	iU32 refcount ;

}i51SlDlCALLBACK_PARAMETER  ;

static i51SlDlCALLBACK_PARAMETER i51SlCallbackParameter = {0} ;

#define I51_ADDR		"120.197.129.19"
#define I51_TEST_ADDR	"121.37.60.20"

static i51SlDl i51SlDlDownloader = { 0 } ;
static iU8 i51HOST [ 32 ] = { 0 } ;
	//	��wapͷ��ʱ����Ҫ�ض���
static iCHAR gReUrl[20] = {0} ;

	
#define SET_BIT(v,n,b)   ((b)==0)?((v)&=~(1<<(n))):((v)|=(1<<(n)))
#define GET_BIT(v,n)     ((v)&(1<<(n)))

//	��ʱʱ��
#define I51_DL_TIME_OUT	(30*1000)
static unsigned char lla_utils_get_u8 (unsigned char **p)
{
    return *(*p)++;
}

static unsigned int lla_utils_get_u16 (unsigned char **p)
{
    unsigned int n;
    n = (*(*p+1) ) + (*(*p) << 8);
    *p += 2;
    return n;
}

static unsigned int lla_utils_get_u32 (unsigned char **p)
{
    unsigned int n;
    n = *(*p+3) + (*(*p+2) << 8) + (*(*p+1) << 16) + (*(*p) << 24);
    *p += 4;
    return n;
}

static unsigned int lla_utils_get_u64 (unsigned char **p)
{
    unsigned long int n;
    n =  ( *(*p+7) ) + ( *(*p+6) << 8 ) + ( *(*p+5) << 16 ) + ( (*(*p + 4)) << 24 ) + 
		( (*(*p + 3)) << 32 ) + ( (*(*p + 2)) << 40 ) + ( (*(*p + 1)) << 48 ) + ( *(*p) << 56 );
    *p += 8;
    return n;
}

static void lla_utils_set_u8 (unsigned char **p, unsigned char n)
{
    *(*p)++ = n;
}
static void lla_utils_set_u16 (unsigned char **p, unsigned short n)
{
    *(*p)++ = (unsigned char)(n >> 8);
    *(*p)++ = (unsigned char)(n & 0xff);
}
static void lla_utils_set_u32 (unsigned char **p, unsigned int n)
{
    *(*p)++ = (unsigned char)((n>>24)&0xff);            
    *(*p)++ = (unsigned char)((n>>16)&0xff);            
    *(*p)++ = (unsigned char)((n>>8)&0xff);             
    *(*p)++ = (unsigned char)(n & 0xff);                
}

static void lla_utils_set_u64(unsigned char **p, unsigned long n)
{

	*p += 4; 

    *(*p)++ = (unsigned char)( (n>>24)&0xff );            
    *(*p)++ = (unsigned char)( (n>>16)&0xff );            
    *(*p)++ = (unsigned char)( (n>>8)&0xff  );             
    *(*p)++ = (unsigned char)( n & 0xff );                

}


#define SET_U8(p,n)   lla_utils_set_u8((unsigned char **)&p, (unsigned char)(n))
#define SET_U16(p,n)  lla_utils_set_u16((unsigned char **)&p, (unsigned short)(n))
#define SET_U32(p,n)  lla_utils_set_u32((unsigned char **)&p, (unsigned int)(n))
#define SET_U64(p,n)  lla_utils_set_u64((unsigned char **)&p, (unsigned long)(n))

#define GET_U8(p)     lla_utils_get_u8((unsigned char **)&p)
#define GET_U16(p)	lla_utils_get_u16((unsigned char **)&p)
#define GET_U32(p)    lla_utils_get_u32((unsigned char **)&p)
#define GET_U64(p)	  lla_utils_get_u64( (unsigned char **) &p)

#define I51_DL_HEAD_LEN		512

#define I51_MAX_URI_LEN	64
#define i51_MAX_PATH_LEN 64
#define i51_MAX_NAME_LEN 64

static iBOOL TEST_FLAG ;
static iU32 TEST_ProjectId ;
static iU16 TEST_LCDWidtrh ;
static iU16 TEST_LCDHeight ;

typedef struct {

	//	i51KitCoreSlDlStart �Ĳ���
	iHANDLE dl ;
	iCHAR uri[I51_MAX_URI_LEN] ;
	iU32 urisize ;
	iCHAR path[i51_MAX_PATH_LEN] ;
	iCHAR name[i51_MAX_NAME_LEN] ;
	iU32 versionId ;
	i51SlDlCOMMAND cmd ;
	i51SlDlCALLBACK callback ;

} I51DLSTARTPARA;

typedef struct {

	//	��ʱ����
	iU32 OverTimerCount ;
	//	������ʽ�Ƿ��Ϊcmnet
	iBOOL gNetIsCmnet ;
	//	����ģʽ,0Ϊ��ʱ����1Ϊ�����л�����ģʽ
	iU8 mode ;
	
} I51DLOVERTIMEADNNET ;

static I51DLSTARTPARA i51DlStartParameters = {0} ;
static I51DLOVERTIMEADNNET i51DlOvertimeAndNet = {0} ;


//	dl �Ƿ��Ѿ�������
iBOOL AlreadyCreate = iFALSE ;

iHANDLE i51KitCoreSlDlCreate (void (*callback) (iBOOL ret));
iBOOL i51KitCoreSlDlStart ( 
	iHANDLE dl , 
	const iCHAR *uri , 
	iU32 urisize ,  
	const iCHAR* path , 
	const iCHAR* name , 
	iU32 versionId ,
	i51SlDlCOMMAND cmd , 
	i51SlDlCALLBACK callback 
);
iBOOL i51KitCoreSlDlStop ( iHANDLE dl );

static void i51DlCreateCB ( iBOOL ret )
{

	i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51DlCreateCB , %d" , ret ) ;
	if ( !ret ) {

		if ( i51DlOvertimeAndNet.mode)
			i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_NET, 0 , 0 , 0 , 0 ) ;
		else
			i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_TIMEROUT, 0 , 0 , 0 , 0 ) ;

		return ;
	}

	i51KitCoreSlDlStart (i51DlStartParameters.dl , 
				i51DlStartParameters.uri ,
				i51DlStartParameters.urisize ,
				i51DlStartParameters.path ,
				i51DlStartParameters.name ,
				i51DlStartParameters.versionId ,
				i51DlStartParameters.cmd ,
				i51DlStartParameters.callback 
				) ;
	
}
static void i51SlDlOverTimeAndCmnetProcessing ()
{

	//	��һ��cmwap ����
	//	�ڶ���cmnet ����
	//	������֪ͨ�ϲ���ʾ��ʱ

	if ( !i51DlOvertimeAndNet.mode) {
		
		i51DlOvertimeAndNet.OverTimerCount ++ ;
	}
	else {

		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlOverTimeAndCmnetProcessing cmnet mode" ) ;
		goto CLOSE_LINK ;
	}
		 

	
	i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlOverTimeAndCmnetProcessing , %d" , i51DlOvertimeAndNet.OverTimerCount ) ;
	if ( i51DlOvertimeAndNet.OverTimerCount == 2 ) {

		i51DlOvertimeAndNet.gNetIsCmnet = iTRUE ;
	}
	if ( i51DlOvertimeAndNet.OverTimerCount == 3 ) {

		i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_TIMEROUT, 0 , 0 , 0 , 0 ) ;
		return ;
	}

	CLOSE_LINK:
	i51AdeSocCloseLinkLayer ( (iU32 )&i51KitCoreSlDlCreate , i51SlDlDownloader.Download.layerid ) ;

	if ( i51SlDlDownloader.Download.layerid )
		i51AdeSocReleaseLinkLayer (i51SlDlDownloader.Download.layerid) ;
	//	�ڴ�����·��û�лص�ʱ��Ҳ�ᴴ����ʱ��ʱ��
	if (i51SlDlDownloader.monitortimer > 0) {
		
		i51AdeOsTimerStop ( i51SlDlDownloader.monitortimer) ; 
		i51SlDlDownloader.monitortimer = 0 ;
	}
	i51SlDlDownloader.Download.layerid = 0 ;
	i51SlDlDownloader.Download.DownloaderIsExsit  = iFALSE ;	

	i51KitCoreSlDlCreate ( i51DlCreateCB ) ;
	

}

static iBOOL i51SlDlFileTruct ()
{

	#define FILE_MALLOC_SIZE 1024*50
	iU16 destabpath [ i51SlDlPATH_SIZE ] = { 0 } ; 
	iU16 srcabpath [ i51SlDlPATH_SIZE ] = { 0 } ; 
	iCHAR path [ i51SlDlPATH_SIZE ] = {0} ;
	iFILE file = 0 ;
	iU32 size = 0 ;
	iU32 len = 0 ;
	void *data = 0 ;
//	iU8 tempfile [ 64 ] = { 0 } ;
	strcpy ( path , i51SlDlDownloader.Download.path ) ;
	strcat ( path , i51SlDlDownloader.Download.name ) ;
	strcat ( path , ".temp" ) ;
	i51AdeStdAscii2Unicode ( path , srcabpath ) ;
	strcat ( path , ".temp" ) ;
	i51AdeStdAscii2Unicode ( path , destabpath ) ;
	i51AdeOsFileDelete(destabpath) ;

	file = i51AdeOsFileOpen ( destabpath , 
	i51_OS_FL_WRITE|i51_OS_FL_ALWAYS_CREATE , 
		__FILE__ , __LINE__ ) ;  
	  
	if ( 0 == file )
	{
	
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlFileTruct FO F , %s Open Failed\r\n" , path ) ;
		return iFALSE ;
	}

	data = i51AdeOsMalloc (FILE_MALLOC_SIZE, __FILE__, __LINE__) ;
	if ( !data )
		return iFALSE ;

	i51AdeOsFileSetSeek (i51SlDlDownloader.Download.file, 0 , i51_OS_FL_SEEK_HEAD) ;
	while ( size < i51SlDlDownloader.Download.totallsize) {

		if ( i51SlDlDownloader.Download.totallsize - size < FILE_MALLOC_SIZE )
			len = i51SlDlDownloader.Download.totallsize - size ;
		else
			len = FILE_MALLOC_SIZE ;
		i51AdeOsFileRead (i51SlDlDownloader.Download.file , data , len, 0) ;
		i51AdeOsFileWrite (file , data , len, 0) ;
		size += len ;
		
	}
	i51AdeOsFree (data) ;	//i51AdeOsFreeEx
	i51AdeOsFileClose(file) ;
	i51AdeOsFileClose(i51SlDlDownloader.Download.file) ;
	i51AdeOsFileDelete(srcabpath) ;
	i51AdeOsFileRename ( destabpath , srcabpath ) ;

	i51SlDlDownloader.Download.file = i51AdeOsFileOpen ( destabpath , 
	i51_OS_FL_WRITE|i51_OS_FL_EXISTING_OPEN , 
		__FILE__ , __LINE__ ) ;  
	  
	if ( 0 == file )
	{
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlFileTruct FO F , %s Open Failed , %d " , path , file ) ;
		return iFALSE ;
	}
	return iTRUE ;
	
}
static void i51SlDlStopProcess ()
{

	//	author : Jelo
	//	since : 2011.6.27
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	�ر����ؽ���

	
	i51AdeOsLog ( 0 , "SL - Dl : i51SlDlStopProcess") ;
	if ( 0 < i51SlDlDownloader.Download.file ) {
		
		i51AdeOsFileClose ( i51SlDlDownloader.Download.file ) ;
		i51SlDlDownloader.Download.file = 0 ;
	}
	
	if ( i51SlDlDownloader.timer > 0 ) {
		
		i51AdeOsTimerStop ( i51SlDlDownloader.timer ) ;  
		i51SlDlDownloader.timer = 0 ;
	}
	
	if (i51SlDlDownloader.monitortimer > 0) {
		i51AdeOsTimerStop ( i51SlDlDownloader.monitortimer) ;  
		i51SlDlDownloader.monitortimer = 0 ;
	}

	if (i51SlDlDownloader.wap.ContentBuf) {

		i51AdeOsFree (i51SlDlDownloader.wap.ContentBuf) ;
		i51SlDlDownloader.wap.ContentBuf = 0 ;
	}
	
	i51AdeSocClose ( i51SlDlDownloader.Socket.id ) ;  
//	memset ( &i51SlDlDownloader , 0 , sizeof(i51SlDl) ) ;   //back
	i51SlDlDownloader.state = i51_SAP_DL_STATE_CLOSE;
	i51SlDlDownloader.state = i51_SAP_DL_STATE_IDLE ;
}   


static void i51SlDlTimerCB ( iU32 id )
{

	//	author : william
	//	since : 20111125
	//	PKIG Tech. Co., Ltd.


	if (  i51AdeOsGetTick () - i51SlDlDownloader.recordtime > I51_DL_TIME_OUT) {

		//	�趨60��Ϊ��ʱ
		if ( i51SlDlDownloader.monitortimerType == 1 ) {

			if (i51SlDlDownloader.monitortimer > 0) {

				//	������·��ʱ
				i51AdeOsLog ( 0 , "SL - Dl : i51SlDlTimerCB time out 1") ;
				i51AdeOsTimerStop ( i51SlDlDownloader.monitortimer) ;  
				i51SlDlDownloader.monitortimer = 0 ;
				i51SlDlDownloader.LinkLayer.successed = iFALSE ;
				i51AdeSocCloseLinkLayer ( (iU32 )&i51KitCoreSlDlCreate , i51SlDlDownloader.Download.layerid ) ;
				i51AdeSocReleaseLinkLayer (i51SlDlDownloader.Download.layerid) ;
				i51SlDlDownloader.i51SlDlCreateCallback (i51SlDlDownloader.LinkLayer.successed) ;
			}

		} else {

				//	soc��ʱ
			i51AdeOsLog ( 0 , "SL - Dl : i51SlDlTimerCB time out 2") ;
			i51SlDlStopProcess () ;
//			i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_TIMEROUT, 0 , 0 , 0 , 0 ) ;
			i51DlOvertimeAndNet.mode = 0 ;
			i51SlDlOverTimeAndCmnetProcessing ();
		}
		
	}

	

	

}

static void i51SlDlTimerStart ( iU32 monitortimerType )
{
	//	author : william
	//	since : 20111125
	//	PKIG Tech. Co., Ltd.

	//	����Ϊ1��������·����
	//	����Ϊ2��soc����
	
	i51SlDlDownloader.monitortimer = i51AdeOsTimerCreate(1,1) ;
	i51SlDlDownloader.monitortimerType = monitortimerType ;
	if ( i51SlDlDownloader.monitortimer == 0 ) {

		i51SlDlStopProcess () ;
		i51AdeOsLog ( 0 , "SL - Dl : i51SlDlTimerStart CT F" ) ;
		i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_CREATETIMER, 0 , 0 , 0 , 0 ) ;
		return ;

	}

	if ( !i51AdeOsTimerStart (i51SlDlDownloader.monitortimer , i51SlDlTimerCB ,3*1000)) {

		if ( monitortimerType == 1 ) {

			
		} else {

		i51SlDlStopProcess () ;
		i51AdeOsLog ( 0 , "SL - Dl : i51SlDlTimerStart ST F") ;
		i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_CREATETIMER, 0 , 0 , 0 , 0 ) ;
		}

		i51SlDlDownloader.monitortimer = 0 ;
		return ; 
	}
	i51AdeOsLog ( 0 , "SL - Dl : i51SlDlTimerStart timerid = %d" , i51SlDlDownloader.monitortimer) ;
	i51SlDlDownloader.recordtime = i51AdeOsGetTick() ;
	
	

}



static iS32 i51SlDlVerifyCheck ()
{


	//	author : Jelo
	//	since : 2011.6.25
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes : ���֮ǰ�������������ϵ����ػ���������
	//	�ɹ�����iTRUE , ʧ�ܷ���iFALSE
	
	iU16 abpath [ i51SlDlPATH_SIZE ] = { 0 } ; 
	iCHAR path [ i51SlDlPATH_SIZE ] = {0} ;
//	iU8 tempfile [ 64 ] = { 0 } ;
	
	strcpy ( path , i51SlDlDownloader.Download.path ) ;
	strcat ( path , i51SlDlDownloader.Download.name ) ;
	strcat ( path , ".temp" ) ;

	i51AdeStdAscii2Unicode ( path , abpath ) ;
	   
	if ( !i51AdeOsFileIsExist ( abpath ) )
	{   
	
	//	�ļ��������״����� 
		i51SlDlDownloader.Download.file = i51AdeOsFileOpen ( abpath , 
		i51_OS_FL_WRITE|i51_OS_FL_ALWAYS_CREATE , 
			__FILE__ , __LINE__ ) ;  
		  
		if ( 0 == i51SlDlDownloader.Download.file )
		{
			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlVerifyCheck FO F , %s Open Failed , %d " , path ,  i51SlDlDownloader.Download.file ) ;
			return iFALSE ;
		}

		i51SlDlDownloader.state = i51_SAP_DL_STATE_INIT ;
		i51SlDlDownloader.mode = i51_SAP_DL_MODE_NEW ;
		i51SlDlDownloader.Download.totalloffset = 0 ;
		i51SlDlDownloader.Download.totallsize = 0 ;
		i51SlDlDownloader.Download.internal_crc = 0 ;
		i51SlDlDownloader.Download.wirten = 0 ;
		
		return iTRUE ;
		
	}
	else
	{
	//	�ļ�����
 
		iS32 ret = 0 ;
	 
		i51SlDlDownloader.Download.file = i51AdeOsFileOpen ( abpath , 
			i51_OS_FL_WRITE|i51_OS_FL_EXISTING_OPEN , 
			__FILE__ , __LINE__ ) ;
		
		if ( 0 == i51SlDlDownloader.Download.file )
		{
			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlVerifyCheck FO  , %s Open Failed(2) , %d" , path , i51SlDlDownloader.Download.file  ) ;
			return iFALSE ;
		}

		i51AdeOsFileSetSeek ( i51SlDlDownloader.Download.file , -4 , i51_OS_FL_SEEK_TAIL ) ;
		i51AdeOsFileRead ( i51SlDlDownloader.Download.file , 
			&i51SlDlDownloader.Download.internal_crc , 
			sizeof(i51SlDlDownloader.Download.internal_crc) , 0 ) ;
		
		i51AdeOsFileGetSize (i51SlDlDownloader.Download.file ,&ret ) ;

		if ( ret > 0 ) {

		//	�ϵ�����
		i51SlDlDownloader.state = i51_SAP_DL_STATE_INIT ;
		i51SlDlDownloader.mode = i51_SAP_DL_MODE_BREAK ;

		//	�ϴ����ص�������   
		i51SlDlDownloader.Download.totalloffset = ret - 4 ;
		i51SlDlDownloader.Download.totallsize = 0 ;
		i51SlDlDownloader.Download.wirten = 0 ;

		i51AdeOsLog ( i51_LOG_SC , "SL - DL :  i51SlDlVerifyCheck Offset = %d\r\n" , i51SlDlDownloader.Download.totalloffset ) ;
		i51AdeOsLog ( i51_LOG_SC , "SL - DL :  i51SlDlVerifyCheck CRC = %d\r\n" , i51SlDlDownloader.Download.internal_crc ) ;
		} else {

		//	�ļ����ڵ���û�����ݣ�Ҳ�����״�����
		i51SlDlDownloader.state = i51_SAP_DL_STATE_INIT ;
		i51SlDlDownloader.mode = i51_SAP_DL_MODE_NEW ;
		i51SlDlDownloader.Download.totalloffset = 0 ;
		i51SlDlDownloader.Download.totallsize = 0 ;
		i51SlDlDownloader.Download.internal_crc = 0 ;
		i51SlDlDownloader.Download.wirten = 0 ;			
		}

		return iTRUE ;
		
	}
  
	return iTRUE ;
	
}

static iCHAR* i51SlDlSkipHttpHeader ( iCHAR* buffer , iU32 length )
{


	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	notes : skip http header

	iCHAR* based = buffer ;

	//	skip http header
	while ( (buffer-based) < length )
	{

		if ( '\r' == *buffer && 0 == strncmp ( buffer , "\r\n\r\n" , 4 ) ) 
		{
			buffer = buffer + 4 ;
			break ;
		}
		else
			buffer ++ ;
	
	}

	return buffer ;
	
}

static void i51SlDlParseSerialNumber ( iCHAR* buffer , iU32 length )
{

	//	author : Jelo
	//	since : 2011.6.24
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	iU32 results = 0 ;
	//	SN Ϊ64λ��������32λ����
	iU32 SerialNumber1 = 0 ;
	iU32 SerialNumber2 = 0 ;

	i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlParseSerialNumber " ) ;

		


	//	����У����
	buffer = buffer + 4 ;
	//	�����ײ�Э��
	buffer = buffer + 4 ;

	
	results = GET_U32 ( buffer ) ;
 
	if ( 0 != results )
	{
		i51AdeOsLog ( i51_LOG_SC , "SL - DL :SL - DL : i51SlDlParseSerialNumber Fatal Protocol Error = %d" , results ) ;
		i51SlDlStopProcess () ;
		//	֪ͨӦ�ò�
		i51SlDlDownloader.User.callback ( -results , 0 , 0 , 0 , 0 ) ;
		//i51SlDlDownloader.state = i51_SAP_DL_STATE_CLOSE ;
		
		return ;
	}

	//	������������
	buffer = buffer + 1 ;
 
	//	������Ӧʱ��
	buffer = buffer + 8 ;

	//	�������峤��
	buffer = buffer + 4 ;
	//	��������id
	buffer = buffer + 2 ;

//	memcpy ( &SerialNumber , buffer , 8 ) ;
	SerialNumber1 = GET_U32 (buffer) ;
	SerialNumber2 = GET_U32 (buffer) ;
	
	i51AdeOsLog ( i51_LOG_SC , "SL - DL : SL - DL : i51SlDlParseSerialNumber Serial Number1 = %d\r\n" , SerialNumber1 ) ;
	i51AdeOsLog ( i51_LOG_SC , "SL - DL : SL - DL : i51SlDlParseSerialNumber Serial Number2 = %d\r\n" , SerialNumber2 ) ;

//	buffer = buffer - 8 ;
	{
		iU8 snbuffer[16] = {0} ;
		iU8 PermanitBuffer[16] = {0} ;
		iU32 snCrc = 0 ;
		memcpy ( snbuffer , &SerialNumber1 , 4 ) ;
		memcpy ( snbuffer + 4 , &SerialNumber2 , 4 ) ;
		snCrc = i51AdeStdCrc32 ( snbuffer , 8 , 0 ) ;
		memcpy ( PermanitBuffer , &snCrc , 4 ) ;
		memcpy ( PermanitBuffer + 4 , snbuffer , 8 ) ;
		if ( !i51AdeOsPermanitBufferWrite ( PermanitBuffer , 12 )) {

			i51AdeOsLog ( 0 , "SL - DL : i51SlDlParseSerialNumber W SN F" ) ;
		}
	}
	i51SlDlStopProcess () ;
	i51SlDlDownloader.User.callback ( i51_SL_DL_FINISHED , 0 , 0 , 0 , i51SlDlDownloader.Download.RefCount ) ;
	//i51SlDlDownloader.state = i51_SAP_DL_STATE_CLOSE ;
	
		
}

static iBOOL i51SlDlParseBitstream ( iCHAR* buffer , iU32 bufferlen , iCHAR *textbuffer , iU32 length )
{

	//	author : Jelo
	//	since : 2011.6.24
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	iU32 based = (iU32 )buffer ;
	iU32 results = 0 ;

	//	notes : ����Э��ͷ����ʱ������iFALSE
	//	�����������iTRUE
	
	//	Э���Ӧͷֻ����һ��
	if ( iFALSE == i51SlDlDownloader.Download.pskip )
	{

		//	���� CRC
		buffer = buffer + 4 ;

		//	�����ײ�汾
		buffer = buffer + 4 ;

		//	��Ӧ���
		results = GET_U32 ( buffer ) ;

		if ( 0 != results )
		{
			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlParseBitstream Fatal Protocol Error = %d\r\n" , results ) ;
			i51SlDlStopProcess () ;
			//	֪ͨӦ�ò�		
			i51SlDlDownloader.User.callback ( -results , 0 , 0 , 0 , 0 ) ;		
			//i51SlDlDownloader.state = i51_SAP_DL_STATE_CLOSE ;
			
			return iFALSE ;
		}

		buffer = buffer + 1 ;
		//	����ʱ��
		buffer = buffer + 8 ; 

		//	�������峤��
		buffer = buffer + 4 ;

		//	��������id
		buffer = buffer + 2 ;

		i51SlDlDownloader.Download.final_crc = GET_U32 ( buffer ) ;
		i51SlDlDownloader.Download.RefCount = GET_U16 ( buffer ) ;
		i51SlDlDownloader.Download.reftotalsize = GET_U32 ( buffer ) ;
		i51SlDlDownloader.Download.totallsize = GET_U32 ( buffer ) ;
		i51SlDlDownloader.Download.bitssize = GET_U32 ( buffer ) ;
 

		if ( i51_SAP_DL_MODE_BREAK == i51SlDlDownloader.mode )
		{

			//	�����ļ���
			//	��������
			if ( i51SlDlDownloader.Download.internal_crc != i51SlDlDownloader.Download.final_crc )
			{

				iU16 abpath [ i51SlDlPATH_SIZE ] = { 0 } ;
				iCHAR path [ i51SlDlPATH_SIZE ] = {0} ;

				strcpy ( path , i51SlDlDownloader.Download.path ) ;
				strcat ( path , i51SlDlDownloader.Download.name ) ;
				strcat ( path , ".temp" ) ;

				i51AdeStdAscii2Unicode ( path , abpath ) ;
				
				i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlParseBitstream  The Local File is Crashed\r\n" ) ;
				i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlParseBitstream Restart Downloading Process\r\n" ) ;
				i51AdeOsFileClose ( i51SlDlDownloader.Download.file ) ;	
				i51SlDlDownloader.Download.file = 0 ;
				i51AdeOsFileDelete ( abpath ) ; 
				i51SlDlVerifyCheck () ;
				if (i51SlDlDownloader.monitortimer > 0)
					i51AdeOsTimerStop ( i51SlDlDownloader.monitortimer) ;  
				i51AdeSocClose ( i51SlDlDownloader.Socket.id ) ;  	
				return iFALSE ;
				
			}
			
		
		}
		
		i51SlDlDownloader.Download.internal_crc = i51SlDlDownloader.Download.final_crc ;

		//	������Ƕϵ�����
		if ( i51_SAP_DL_MODE_BREAK != i51SlDlDownloader.mode ) 
		{
			i51SlDlDownloader.Download.totalloffset = 0 ;
		}
	
		i51SlDlDownloader.Download.pskip = iTRUE ;

		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlParseBitstream CRC = %d\r\n" , i51SlDlDownloader.Download.final_crc ) ;
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlParseBitstream Totall = %d\r\n" , i51SlDlDownloader.Download.totallsize ) ;
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlParseBitstream Bitssize = %d\r\n" , i51SlDlDownloader.Download.bitssize ) ;
			
	}
 
	if ( 0 < length )
	{
		
		//	Skip CRC
		iU32 wirten = 0 ;
		if ( (0 < i51SlDlDownloader.Download.totalloffset) &&
			(i51SlDlDownloader.Download.totalloffset < i51SlDlDownloader.Download.totallsize)
		)
		{ 
			i51AdeOsFileSetSeek ( i51SlDlDownloader.Download.file , -4 ,  i51_OS_FL_SEEK_TAIL ) ;
		}  


		if (!i51AdeOsFileWrite ( i51SlDlDownloader.Download.file , textbuffer , length , &wirten ) ) {


			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlParseBitstream wirte error (1)" ) ;
			i51SlDlStopProcess () ;
			//	֪ͨӦ�ò�		
			i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_FILE , 0 , 0 , 0 , 0 ) ;	
			return iFALSE ;
		}
		i51AdeOsFileFlush ( i51SlDlDownloader.Download.file ) ;

		if ( i51SlDlDownloader.Download.totalloffset > i51SlDlDownloader.Download.totallsize -4  ) {
			
			if (!i51SlDlFileTruct () ) {

				i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlParseBitstream Truct error" ) ;
				i51SlDlStopProcess () ;
				//	֪ͨӦ�ò�		
				i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_FILE , 0 , 0 , 0 , 0 ) ;	
				return iFALSE ;
			}
		}
		
		i51SlDlDownloader.Download.wirten += wirten ;
		i51SlDlDownloader.Download.totalloffset += length ; 
		
		
		if ( i51SlDlDownloader.Download.totalloffset < i51SlDlDownloader.Download.totallsize )
		{   
			if (!i51AdeOsFileWrite ( i51SlDlDownloader.Download.file , 
				&i51SlDlDownloader.Download.internal_crc ,
				sizeof(i51SlDlDownloader.Download.internal_crc) , 0 ) ) {

				i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlParseBitstream wirte error (2)" ) ;
				i51SlDlStopProcess () ;
				//	֪ͨӦ�ò�		
				i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_FILE , 0 , 0 , 0 , 0 ) ;	
				return iFALSE ;
			}
			
		}

  		
	}

	return iTRUE ;
	
}


static void i51SlDlBuildHttpHeader ( iCHAR* buffer , iU32 size , iU16 port , iCHAR* host )
{


	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	����д��i51SlDlDownloader.cache

	memset ( buffer , 0 , size ) ;

//	i51SlDlDownloader.Socket.server.family = AF_INET ;
 
	if ( i51_SAP_DL_LINKLAYER_CMWAP == i51SlDlDownloader.LinkLayer.type )
	{

/*
		i51SlDlDownloader.Socket.server.ipaddr = gI51NetAccount.ipaddr ;
		i51SlDlDownloader.Socket.server.port = 80 ; */
		#ifndef  i51SIMULATOR		
		sprintf ( buffer , i51HTTP_PROXY_HEADER_PATTERN , gReUrl , host , port , host , port) ;	
		#else
		sprintf ( buffer , i51HTTP_HEADER_PATTERN  , gReUrl ,host , port , host , port) ;
		#endif

		i51AdeOsLog ( 0 , "SL - DL : %s" , buffer ) ;
		memset ( gReUrl , 0 , sizeof(gReUrl) ) ;
		     
	}
	else if ( i51_SAP_DL_LINKLAYER_CMNET == i51SlDlDownloader.LinkLayer.type )
	{
		//	��֧��
	}
		
	return ;
	
}

static void i51SlDlBuildRequest ( iCHAR* header )
{


	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	Э��ͷ��ο�"PKIG-ADMINͨѶЭ��2.04"
	
	iU8* content = 0 ;
	iU8* mark = 0 ;

	//	skip http header
	content = i51SlDlDownloader.cache + strlen(header) + i51HTTP_CONTENT_LENGTH ;


#if 0
	SET_U32 ( content , i51_SC_NET_VERSION ) ;
	SET_U8 ( content , i51SlDlDownloader.User.command ) ;
	//	Skip CRC
	content = content + 4 ;  
	SET_U32 ( content ,  i51Config.Kerver ) ;
	SET_U32 ( content , i51Config.projectid ) ;  
	SET_U32 ( content , ((iU32)i51AdeOsLog));
	SET_U32 ( content , ((iU32)i51AdeOsFileOpen));
	SET_U32 ( content , ((iU32)i51AdeSmsSend));
	SET_U32 ( content , ((iU32)i51AdeStdWSprintf));
	
	SET_U8 ( content , strlen (i51Config.OSPlatform ) );		
	strncpy ( content, i51Config.OSPlatform , strlen ( i51Config.OSPlatform ) );
	content += strlen (i51Config.OSPlatform );
	
    	SET_U16 ( content, i51Config.sewidth ) ;   
    	SET_U16 ( content, i51Config.seheight ) ;  
	
	SET_U8 ( content , strlen (i51Config.SMSC ) );
	strncpy ( content, i51Config.SMSC, strlen (i51Config.SMSC) );
	content += strlen ( i51Config.SMSC );
	
	SET_U8 ( content , strlen (i51Config.IMSI) ) ;
	strncpy ( content , i51Config.IMSI, strlen (i51Config.IMSI) ) ;
	content += strlen ( i51Config.IMSI );

	SET_U8 ( content , strlen (i51Config.IMEI) );
	strncpy ( content, i51Config.IMEI, strlen (i51Config.IMEI) );
	content += strlen ( i51Config.IMEI );
 
    	SET_U32( content, i51Config.Tcardspace ); 
    	SET_U32 ( content, i51Config.callingtime );
    	SET_U32 ( content, i51Config.netratio );
    	SET_U32 ( content, i51Config.Tcardspaceleft ); 
    	SET_U8 ( content, i51SlDlDownloader.User.reqmode );

	if ( i51_SAP_DLCOM_BITSTREAM == i51SlDlDownloader.User.command )
	{
	
		iU8* SerialNumber = i51AdeOsReadSerialNumber () ;
		
		SET_U64 ( content , SerialNumber); 
		
		SET_U32 (content, i51SlDlDownloader.Download.urisize);
		memcpy (content, i51SlDlDownloader.Download.uri, i51SlDlDownloader.Download.urisize); 
		content += i51SlDlDownloader.Download.urisize;
		
		SET_U32 (content, i51SlDlDownloader.Download.totalloffset);
		SET_U32 (content, i51SlDlDownloader.Download.totallsize);		 
		SET_U32 (content, i51SlDlDownloader.Download.internal_crc); 	 
		
	}

	{

		iU8* CrcBuffer = 0 ;
		iU32 CRC = 0 ;

		//	plus 9 for skiping version , command , crc , check out "PKIG-ADMINͨѶЭ��2.04"
		CrcBuffer = i51SlDlDownloader.cache + strlen(header) + i51HTTP_CONTENT_LENGTH + 9 ; 

		CRC = i51AdeStdCrc32 ( (iU8 *)CrcBuffer , content-CrcBuffer ) ;
		CrcBuffer = CrcBuffer - 4 ;

		SET_U32 ( CrcBuffer , CRC ) ;
		
	}

	//	Set http header
	{

		iCHAR* line [ 128 ] = { 0 } ;
		iCHAR* buffer = i51SlDlDownloader.cache ;
		iU32 headlen = strlen ( header ) ;
			
		memcpy ( buffer , header , headlen ) ;
		buffer = buffer + headlen ;

		sprintf ( line , "Content-Length:  %9d\r\n\r\n" , content - (i51SlDlDownloader.cache + strlen(header) + i51HTTP_CONTENT_LENGTH) ) ;

		memcpy ( buffer , line , i51HTTP_CONTENT_LENGTH ) ;
		
	}

	i51SlDlDownloader.Socket.SendTotall = content - i51SlDlDownloader.cache ;
	i51SlDlDownloader.Socket.SendOffset = 0 ;

	#else

	//	����У���룬��Ҫ����
	content = content + 4 ;  

	//	Jelo Edited 2012.05.03
	//	Jelo Commited 2012.05.03
	//	�ײ�Э��汾
	//	SET_U32 ( content ,  i51AdeCnfGetVersion()) ;
	
	//	kernel�汾
	//	SET_U32 ( content ,  i51KernelGetVersion()) ;
	
	//	adapter�汾
	//	SET_U32 ( content ,  1) ;
	//	Jelo Commited Finished

	//	�ײ�Э��汾
	SET_U32 ( content ,  1) ;
	//	kernel�汾
	SET_U32 ( content ,  i51KernelGetVersion()) ;
	//	adapter�汾
	SET_U32 ( content ,  i51AdeCnfGetVersion()) ; 
	//	Jelo Edited Finished
	
	//	�ͻ�id
	SET_U32 ( content ,  i51AdeCnfGetCustomerId()) ;
	
	//	��Ŀid
	if(!TEST_FLAG)
	    SET_U32 ( content ,  i51AdeCnfGetProjectId()) ;
	else
	    SET_U32 ( content ,  TEST_ProjectId) ;
	//	��ֲ����
	#if 0
	SET_U32 ( content , ((iU32)i51AdeOsLog));
	SET_U32 ( content , ((iU32)i51AdeOsFileOpen));
	SET_U32 ( content , ((iU32)i51AdeSmsSend));
	SET_U32 ( content , ((iU32)i51AdeStdWSprintf));
	#else
	{
	iU32 length = 0 ;
	i51KernelGetBatchCode ( content , 16 , &length ) ;
	content += length ;
	}
	#endif
	//	��С��
	{
//		iU8 port = 0 ;
		SET_U8 ( content , i51AdeCnfGetEndian ()|(i51AdeCnfGetCpu()<<1) ) ;
	}
	
	//	�ͻ�ƽ̨��ʾ
	{
	SET_U8 ( content , strlen (i51AdeCnfGetPlatformId()) ) ;		
	strncpy ( (iCHAR*)content, i51AdeCnfGetPlatformId() , strlen ( i51AdeCnfGetPlatformId() ) ) ;
	content += strlen (i51AdeCnfGetPlatformId()) ;
	}
	
	//	�ֱ��ʿ�
	//	�ֱ��ʸ�
	if(!TEST_FLAG)
	{
		//	�õ���͸�
		iU16 LCDWidtrh , LCDHeight ;
		i51AdeMmiGetScreenScale ( &LCDWidtrh , &LCDHeight ) ;
		SET_U16 ( content, LCDWidtrh ) ;   
    	SET_U16 ( content, LCDHeight ) ;  
	}
	else
	{
		SET_U16 ( content, TEST_LCDWidtrh ) ;   
    	SET_U16 ( content, TEST_LCDHeight ) ;  
    }
	//	smsc
	//	imsi
	//	imei
	{
		iS32 *SimCardInfo = NULL ;
		iCHAR *SMSC = NULL ;
		iCHAR *SMSI = NULL ;
		iCHAR *IMEI = NULL ;
		
		SimCardInfo = i51AdeSmsGetSims () ;
		SMSC = i51AdeSmsGetCenter ( SimCardInfo[1] ) ;
		SMSI = i51AdeSmsGetImsi ( SimCardInfo[1] ) ;
		IMEI = i51AdeSmsGetImei () ;

		SET_U8 ( content , strlen (SMSC) ) ;
		strncpy ( (iCHAR*)content, SMSC , strlen ( SMSC ) ) ;
		content += strlen ( SMSC ) ;

		SET_U8 ( content , strlen (SMSI) ) ;
		strncpy ( (iCHAR*)content, SMSI , strlen ( SMSI ) ) ;
		content += strlen ( SMSI ) ;

		SET_U8 ( content , strlen (IMEI) ) ;
		strncpy ( (iCHAR*)content, IMEI , strlen ( IMEI ) ) ;
		content += strlen ( IMEI ) ;

	}

	//	ͨ��ʱ��(��λ����)
	SET_U32 ( content ,  i51AdeOsGetCallingTime ()/60) ;

	//	T���ܴ�С
	//SET_U32 ( content , i51AdeOsGetAppDiskSpace () ) ;
	SET_U32 ( content , i51AdeOsGetAppDiskSpace () ) ;
	
	//	T��ʣ���С
	SET_U32 ( content , i51AdeOsGetAppDiskSpaceLeft () ) ;
	
	//	ϵͳ���ܴ�С
	SET_U32 ( content , i51AdeOsGetSysDiskSpace () ) ;
	
	//	�ֻ���ʣ��ռ�
	SET_U32 ( content , i51AdeOsGetSysDiskSpaceLeft () ) ;
	
	//	�ֻ�����
	SET_U8 ( content , 0 ) ;

	
	if ( i51_SAP_DLCOM_BITSTREAM == i51SlDlDownloader.User.command )
	{
		iU8 SerialNumber[16] = { 0 } ;
		iU32 SerialNumber32bit1 = 0 ;
		iU32 SerialNumber32bit2 = 0 ;
		if (i51AdeOsPermanitBufferRead ( SerialNumber , 12 ) ) {

			memcpy ( &SerialNumber32bit1 , SerialNumber+4 , 4 ) ;
			memcpy ( &SerialNumber32bit2 , SerialNumber+8 , 4 ) ;
		} else {

			i51AdeOsLog ( 0 , "SL - DL : i51SlDlBuildRequest R SN F" ) ;
		}


		//	sn 
		
		SET_U32 ( content , SerialNumber32bit1);
		SET_U32 ( content , SerialNumber32bit2); 
	} else {
		//	ע��sn��ʱ��Ϊ0
		SET_U32 ( content , 0) ; 
		SET_U32 ( content , 0) ; 

	}

	//	����Ϊ��ͷ
	SET_U8 ( content ,i51_SAP_DL_REQ_MODE_AUTO ) ;

	mark = content ;
	//	�������峤�ȣ���Ҫ����
	content += 4 ;

	
	//	����id
	SET_U16 ( content , i51SlDlDownloader.User.command ) ;

	if ( i51_SAP_DLCOM_BITSTREAM == i51SlDlDownloader.User.command )
	{
		//	�ļ���Դ����

		//	ʣ��ռ��С��û��t��ʱ��ϵͳ�̿ռ�

		SET_U32 ( content , i51AdeOsGetAppDiskSpaceLeft () ) ;
		//	��Դ�汾��
		SET_U32 ( content , i51SlDlDownloader.Download.versionId ) ;
		
		//	��Դ�ļ���λ
		SET_U8 ( content , i51SlDlDownloader.Download.urisize ) ;
		strncpy ( (iCHAR*)content, i51SlDlDownloader.Download.uri , i51SlDlDownloader.Download.urisize ) ;
		content +=  i51SlDlDownloader.Download.urisize ;
	
		
		//	������ʼλ��ƫ����
		SET_U32 (content, i51SlDlDownloader.Download.totalloffset) ;
		
		//	�������ݵĳ���
		SET_U32 (content, i51SlDlDownloader.Download.totallsize);		 
		
		//	�����ļ���crcֵ
		SET_U32 (content, i51SlDlDownloader.Download.internal_crc); 	 
		
	
	} else {

		//	sn����
		//	��̬ram��С
		SET_U32 ( content , 30 ) ;

	}

	{

		//	���峤�Ȼ���
		iU32 len = content - mark - 4 ;
		SET_U32 ( mark , len ) ; 

	}

	{

		
		//	CRC����
		iU8* CrcBuffer = 0 ;
		iU32 CRC = 0 ;

		//	+4 ����crc
		CrcBuffer = i51SlDlDownloader.cache + strlen(header) + i51HTTP_CONTENT_LENGTH + 4 ; 

		CRC = i51AdeStdCrc32 ( (iU8 *)CrcBuffer , content-CrcBuffer , NULL) ;
		CrcBuffer = CrcBuffer - 4 ;

		//	������������src
		SET_U32 ( CrcBuffer , CRC ) ;
		
	}

	//	Set http header
	{

		iCHAR line [ 128 ] = { 0 } ;
		iU8* buffer = i51SlDlDownloader.cache ;
		iU32 headlen = strlen ( header ) ;
			
		memcpy ( buffer , header , headlen ) ;
		buffer = buffer + headlen ;

		sprintf ( line , "Content-Length:  %9d\r\n\r\n" , content - (i51SlDlDownloader.cache + strlen(header) + i51HTTP_CONTENT_LENGTH) ) ;

		memcpy ( buffer , line , i51HTTP_CONTENT_LENGTH ) ;
		
	}

	i51SlDlDownloader.Socket.SendTotall = content - i51SlDlDownloader.cache ;
	i51SlDlDownloader.Socket.SendOffset = 0 ;


	#endif
	   

	
		
}

static void i51SocConCallback ( iBOOL state , iS32 socket )
{


	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	i51SlDlDownloader.recordtime = i51AdeOsGetTick() ;
	if ( iTRUE == state )
	{
		i51AdeOsLog ( 0 , "SL - DL : i51SocConCallback SOC CONNECTED\r\n" ) ;					
		i51SlDlDownloader.state = i51_SAP_DL_STATE_SEND ;  
	}
	else if ( iFALSE == state )
	{
		//	�������
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocConCallback F" ) ;
		//i51SlDlDownloader.state = i51_SAP_DL_STATE_CLOSE ;	
		i51SlDlStopProcess () ;
		//	֪ͨӦ�ò�
		i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_NET , 0 , 0 , 0 , 0 ) ;
		
	}
	
	
}

static void i51SocSendCallback ( iBOOL state , iS32 socket , iU32 textlen )
{


	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	i51SlDlDownloader.recordtime = i51AdeOsGetTick() ;
	if ( iTRUE == state )
	{
		i51AdeOsLog ( 0 , "SL - DL : i51SocSendCallback SOC SEND OK\r\n" ) ;					
		i51SlDlDownloader.state = i51_SAP_DL_STATE_RECEIVE ;
	}
	else if ( iFALSE == state )
	{
		//	�������
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocSendCallback  F " ) ;				
		//i51SlDlDownloader.state = i51_SAP_DL_STATE_CLOSE ;	
		//	֪ͨӦ�ò�
		i51SlDlStopProcess () ;
		i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_NET , 0 , 0 , 0 , 0 ) ;
		
	}
	
}

static iBOOL i51SocHttpHeadRecvEnd ( iCHAR* buffer , iU32 length , iU32 *HeadEndIndex )
{

	//	����Ƿ��Ѿ�httpЭ��Ľ�����
	iU32 i = 0 ;

	//	skip http header
	while ( i  < length )
	{

		if ( '\r' == *(buffer+i) && 0 == strncmp ( buffer+i , "\r\n\r\n" , 4 ) ) 
		{
			//	����Ѿ�
			*HeadEndIndex = i ;
			break ;
		}
		i ++ ;
	}	

	if ( i == length )
		return iFALSE ;

	return iTRUE ;
	
}

static iBOOL i51SocWaitRevc ( iU8 * headbuffer , iU8 *buffer , iU32 bufferlen , iU32 *startoffset )
{
	//	�Ƿ�ȴ�����
	//	����1 : ���ͷbuffer
	//	����2 : ���յ�����buffer
	//	����3 : ���ܵ����ݳ���
	//	����4 : �������Ҫ�ȴ��ˣ��ı����ݵĿ�ʼ�����������buffer

	iU32 HeadEndIndex = 0 ;
	iU32 result ;
	iU16 i = 0;
	iU16 lastHeadoffset = 0 ;


	*startoffset = 0 ;
	lastHeadoffset = i51SlDlDownloader.headRecvOffset ;
	if ( i51SlDlDownloader.Download.hskip )  {

		//	ͷ�Ѿ������ˡ�
		return iFALSE ;
	}


	i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocWaitRevc %x , %x , %d ,%d " , headbuffer , buffer ,bufferlen ,startoffset ) ;	
	for ( ; i < bufferlen && i51SlDlDownloader.headRecvOffset < I51_DL_HEAD_LEN ; i ++ ) {

		headbuffer[lastHeadoffset + i] = buffer[i] ;
		i51SlDlDownloader.headRecvOffset ++ ;
	}

	i51SocHttpHeadRecvEnd ( (iCHAR*)headbuffer , i51SlDlDownloader.headRecvOffset , &HeadEndIndex ) ;

	i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocWaitRevc HeadEndIndex = %d " , HeadEndIndex) ;
		
	{
		//	��ӡ��httpͷ
		iCHAR httphead[256] = {0} ;
		memcpy ( httphead , (iCHAR*)headbuffer , 255) ;
		httphead[255] = '\0' ;
		i51AdeOsLog ( i51_LOG_SC , "%s" , httphead ) ;
		
	}
	//	��û�м�⵽http�������� \r\n\r\n
	if ( !HeadEndIndex) {

		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocWaitRevc need more(1)" ) ;
		return iTRUE ;

	}
	HeadEndIndex += 4 ;
	//	i51Э��ͷ��������12��
	if ( i51SlDlDownloader.headRecvOffset  -HeadEndIndex < 12  ) {

		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocWaitRevc need more(2)" ) ;
		return iTRUE ;
	}
	
	{
	iU8 *headbuffer1 =  headbuffer + HeadEndIndex + 8 ;		
	result = GET_U32 (headbuffer1) ;
	i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocWaitRevc result = %d" , result ) ;
	}
	
	if ( result )
		return iFALSE ;

	if (i51_SAP_DLCOM_BITSTREAM == i51SlDlDownloader.User.command  ) {
		
		if ( i51SlDlDownloader.headRecvOffset  -HeadEndIndex < 45 ) {

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocWaitRevc need more(3)" ) ;
			return iTRUE ;
		}
		else {

			//*startoffset = i51SlDlDownloader.headRecvOffset - HeadEndIndex - 45 ;
			*startoffset = HeadEndIndex + 45 - lastHeadoffset ;
			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocWaitRevc startoffset = %d(1)" , *startoffset ) ;
			return iFALSE ;
		}
		
	} else {

		if ( i51SlDlDownloader.headRecvOffset  -HeadEndIndex < 35 ) {

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocWaitRevc need more(4)" ) ;
			return iTRUE ;
		}
		else {

			//	sn ��û���ı���Ϣ��
//			*startoffset = i51SlDlDownloader.headRecvOffset - HeadEndIndex - 35 ;
			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocWaitRevc startoffset = %d(2)" , *startoffset ) ;
			return iFALSE ;
		}
	}

}

/*
static int i51TestSocStrtoValue(iCHAR *string_value)
{
	iS32 len,num,ret;
	len = strlen(string_value);
	ret = 0;
	for(num = 0;num < len;num ++)
	{
		ret = ret * 10 + string_value[num]-'0';
	}
	return ret;
}
*/
int i51Strncmp(iCHAR *src, iCHAR *dst, iS32 n)
{
    iS32 i;
    iCHAR a,b;
    for(i=0; i<n; i++)
    {
        a=src[i];
        if ((a >= 'A') && (a <= 'Z'))
            a += 'a' - 'A';

        b=dst[i];
        if ((b >= 'A') && (b <= 'Z'))
            b += 'a' - 'A';   

        if(a!=b) return -1;
    }
    return 0;
}
static iBOOL i51GetWord(iCHAR *src, iCHAR **ppFind, int *pFindLen)//��ȡsrc���ĵ�һ�����ʣ����ʼ��� " " "\r" "\n" �ַ��ָ�
//����/r/n/r/n���ط�, FindΪ���ص��ʵ��׵�ַ��FindLenΪ���ʳ���
{
    *ppFind = 0;
    *pFindLen=0;
    while(1)
    {
        if( i51Strncmp( src, "\r\n\r\n", 4) == 0 )
            return iFALSE;
        if( *src==' ' || *src=='\r' || *src=='\n' )
        {
            src++;
        }else{
            *ppFind = src;
            break;
        }
    }
    while(1)
    {
        src++;
        (*pFindLen)++; 
        if( *src==' ' || *src=='\r' || *src=='\n' )
        {
            return iTRUE;
        }
    }
    return iFALSE;
}

static iBOOL i51SocRecvIsWapData ( iBOOL state , iS32 socket , iU8* buffer , iU32 textlen )
{
	//	��������ʱ ���ж��Ƿ�Ϊwap ҳ��

	iCHAR *pFind;
   	iS32 FindLen;
	iCHAR TempBuf[10] = {0} ;
	iCHAR *p1 = 0 , *p2 = 0 ; 
	iU32 len = 0; 
	iCHAR lenstr[32] = {0} ;
	iU32 RecvLength = textlen ;
	iS32  ContentLen, HeadLen, Findi51Mark;
	
	if ( iFALSE == state || i51SlDlDownloader.Download.hskip ) {

		//	ͷ�Ѿ������������߽��ܴ��󣬲��ڴ˴�����
		return iFALSE ;
	}

	if ( !i51SlDlDownloader.wap.ParseHttpHeader ) {

		if ( !i51GetWord ((iCHAR*) buffer , &pFind , &FindLen ) )
			return iFALSE ;
		if ( !i51GetWord ( pFind + FindLen  , &pFind , &FindLen ) )
			return iFALSE ;

		if ( i51Strncmp ( pFind , "200" , 3 ) != 0 ) {

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvIsWapData error(1) " ) ;
			return iFALSE ;
		}
			
		ContentLen = -1; 
		Findi51Mark = 0;   
		while(1){
			
		    if( !i51GetWord(pFind+FindLen, &pFind, &FindLen) ) 
		        break;
			
		    if ( i51Strncmp ( pFind , "application/octet-stream", 24 ) == 0 )
		        Findi51Mark = 1;
			
		    if ( i51Strncmp ( pFind , "Content-length: ", 16 ) == 0 ){
		        if( !i51GetWord(pFind+FindLen, &pFind, &FindLen) ) 
		            break;
		        memset( TempBuf, 0, 10 );
		        strncpy( TempBuf, pFind, FindLen );
		        ContentLen = atoi( pFind );
		    }
		}	

		if( Findi51Mark == 1 || ContentLen == -1 ) {

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvIsWapData error(2) , %d , %d " ,Findi51Mark , ContentLen ) ;
			return iFALSE;
		}
	    
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvIsWapData is wap http header " ) ;
		i51SlDlDownloader.wap.waplen = ContentLen ;

		i51SlDlDownloader.wap.ContentBuf = (iCHAR*)i51AdeOsMalloc (ContentLen, __FILE__, __LINE__) ;
		if ( i51SlDlDownloader.wap.ContentBuf == 0 ) {

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvIsWapData malloc F" ) ;
			return iFALSE ;
		}
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvIsWapData  len = %d" , i51SlDlDownloader.wap.waplen) ;	
		p1 = (iCHAR*)strstr((iCHAR*)buffer,"\r\n\r\n");
		if ( !p1 ) {

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvIsWapData error(3) " ) ;
			return iFALSE;
		}
		RecvLength = RecvLength - (p1 - (iCHAR*)buffer + 4) ;
		memcpy ( i51SlDlDownloader.wap.ContentBuf , p1+4 , RecvLength ) ;
		i51SlDlDownloader.wap.ParseHttpHeader = iTRUE ;
	}

	if ( i51SlDlDownloader.wap.ParseHttpHeader ) {

//		if ( RecvLength > 0 )
//			i51AdeOsFileWrite ( i51TestDl.file , Writebuffer , RecvLength , 0 ) ;
		if (!p1)
			memcpy ( i51SlDlDownloader.wap.ContentBuf + i51SlDlDownloader.wap.recvLen , buffer , RecvLength ) ;
		i51SlDlDownloader.wap.recvLen += RecvLength ;
		if ( i51SlDlDownloader.wap.recvLen == i51SlDlDownloader.wap.waplen ) {

			//	wapҳ��������
			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvIsWapData , %s "  , i51SlDlDownloader.wap.ContentBuf ) ;
			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvIsWapData , %s "  , i51SlDlDownloader.wap.ContentBuf + 256 ) ;
			pFind = i51SlDlDownloader.wap.ContentBuf ;
			while ( pFind < i51SlDlDownloader.wap.ContentBuf + i51SlDlDownloader.wap.waplen ) {
				
		        if(i51Strncmp(pFind, "MainServlet?", 12)==0)
		        {
		            int k = 0;
		            pFind +=11;          
			     i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvIsWapData find MainServlet " );
		            while(*pFind!=' '&&*pFind!='\r'&&*pFind!='\n'&&*pFind!=0&&*pFind!='\"'&&*pFind!='>')
		            {
		                gReUrl[k]=*pFind;
		                k++;
		                pFind++;
		            }
		            break;
		        }
				
				pFind ++ ;
			}

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvIsWapData , %d , %s " , strlen(gReUrl) , gReUrl ) ;
	#if 0
			if ( strlen(gReUrl) == 0 ) {

				gNetIsCmnet = 1 ;
				i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvIsWapData Not Find MainServlet") ;
//				return iFALSE ;
			}
	#endif
			
			i51SlDlStopProcess () ;
			if ( strlen(gReUrl) == 0 ) {

				i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvIsWapData Not Find MainServlet") ;
				i51DlOvertimeAndNet.mode = 1 ;
				i51DlOvertimeAndNet.gNetIsCmnet = iTRUE ;
				i51SlDlOverTimeAndCmnetProcessing ();
			} else {

				i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvIsWapData again") ;
				i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_AGAIN_DLSTART , 0 , 0 , 0 , 0 ) ;
			}

		}
	}
	return iTRUE ;
		
}
static void i51SocRecvCallback ( iBOOL state , iS32 socket , iU8* buffer , iU32 textlen )
{


	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
	static iU8 headbuffer[I51_DL_HEAD_LEN] = {0} ;
	i51SlDlDownloader.recordtime = i51AdeOsGetTick() ;

	i51DlOvertimeAndNet.OverTimerCount = 0 ;
	if ( i51SocRecvIsWapData ( state , socket , buffer , textlen ) ){

		return ;
	}
	if ( iTRUE == state )
	{
		//	�ı�����
		iU8* textbuffer1 = 0 ;
		//	i51Э����Ӧͷ
		iU8* i51Responsebuffer = 0 ;
		iU32 headerlen = 0 ;
		iU32 startoffset = 0 ;

		if ( i51SocWaitRevc ( headbuffer , buffer , textlen , &startoffset  ) ) {

			//	����������Ҫ���գ���ֹͷ����������ɽ�������
			return ;
		}

		textbuffer1 = buffer + startoffset  ;

		//	�ѽ���������
		i51SlDlDownloader.Socket.RecvOffset += textlen ;
					
		//	HTTP ͷֻ����һ��
		if ( iFALSE ==i51SlDlDownloader.Download.hskip )
		{

			i51Responsebuffer = (iU8*)i51SlDlSkipHttpHeader ( (iCHAR*)headbuffer , I51_DL_HEAD_LEN ) ;
			headerlen = (iU32)i51Responsebuffer - (iU32)headbuffer ;

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvCallback headerlen = %d\r\n" , headerlen ) ;
			
			i51SlDlDownloader.Download.hskip = iTRUE ;
		}
	
		if ( i51_SAP_DLCOM_SERIALNUMBER == i51SlDlDownloader.User.command ) 
		{

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvCallback Protocol Length = %d\r\n" , i51SlDlDownloader.Socket.RecvOffset-headerlen ) ;	
			i51SlDlParseSerialNumber ( (iCHAR*)i51Responsebuffer  , headerlen) ;
			
		}
		else if ( i51_SAP_DLCOM_BITSTREAM == i51SlDlDownloader.User.command )
		{

//			i51SlDlDownloader.state = i51_SAP_DL_STATE_RECEIVE ;
// 			i51SlDlDownloader.Socket.RecvOffset = 0 ;
			if ( iFALSE == i51SlDlParseBitstream ( (iCHAR*)i51Responsebuffer , headerlen ,(iCHAR*)textbuffer1  ,textlen-startoffset ) ) {
				i51SlDlDownloader.Download.hskip = iFALSE ;
				return ;

			}
			
//			i51SlDlDownloader.Socket.RecvOffset = 0 ;
//			i51SlDlDownloader.state = i51_SAP_DL_STATE_RECEIVE ;
			if ( i51SlDlDownloader.Download.totalloffset < i51SlDlDownloader.Download.totallsize )
			{
				i51SlDlDownloader.User.callback ( 
					i51_SL_DL_RUNING , 
					textlen , 
					i51SlDlDownloader.Download.totalloffset , 
					i51SlDlDownloader.Download.totallsize ,
					i51SlDlDownloader.Download.RefCount) ;				
			}
			else
			{


				iU16 abpath [ i51SlDlPATH_SIZE ] = { 0 } ;
				iU16 abpathnew [ i51SlDlPATH_SIZE ] = { 0 } ;
				iCHAR u8path [ i51SlDlPATH_SIZE ] = { 0 } ;
				iCHAR path [ i51SlDlPATH_SIZE ] = { 0 } ;
				
				iU32 u8len = 0 ;

				strcpy ( path , i51SlDlDownloader.Download.path ) ;
				strcat ( path , i51SlDlDownloader.Download.name ) ;
				strcat ( path , ".temp" ) ;
	
				strcpy ( u8path , path ) ;
				u8len = strlen ( u8path ) ;
				u8path [ u8len - 1 ] = 0 ;  
				u8path [ u8len - 2 ] = 0 ;
				u8path [ u8len - 3 ] = 0 ;
				u8path [ u8len - 4 ] = 0 ;
				u8path [ u8len - 5 ] = 0 ;
				
				i51AdeStdAscii2Unicode ( path , abpath ) ;
				i51AdeStdAscii2Unicode ( u8path , abpathnew ) ;
					
				i51AdeOsFileClose ( i51SlDlDownloader.Download.file ) ;
				i51SlDlDownloader.Download.file = 0 ;

				if (!i51AdeOsFileRename ( abpath , abpathnew ) ) {
					
					i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlParseBitstream rename error" ) ;
					i51SlDlStopProcess () ;
					//	֪ͨӦ�ò�		
					i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_FILE , 0 , 0 , 0 , 0 ) ;	
					return ;
				}
				//	�ر����ؽ���
				i51SlDlStopProcess () ;// back
				i51SlDlDownloader.User.callback ( i51_SL_DL_FINISHED , 
					textlen , i51SlDlDownloader.Download.totalloffset , i51SlDlDownloader.Download.totallsize , i51SlDlDownloader.Download.RefCount ) ;

				
				

				

				
			}
			
		}
		
	}

	else if ( iFALSE == state )
	{
		//	�������
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SocRecvCallback  F" ) ;	
		
//		i51AdeOsFileClose ( i51SlDlDownloader.Download.file ) ;
		//i51SlDlDownloader.state = i51_SAP_DL_STATE_CLOSE ;
		i51SlDlStopProcess () ;
				//	֪ͨӦ�ò�
		i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_NET , 0 , 0 , 0 , 0 ) ;

		
	}
	
}

static void i51SlDlDownloading ( iU32 id )
{


	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	switch ( i51SlDlDownloader.state )
	{

		//	��ת
		case i51_SAP_DL_STATE_IDLE :
		break ;
		
		case i51_SAP_DL_STATE_INIT :

			i51SlDlDownloader.Socket.id = i51AdeSocGet() ;

			if ( 0 > i51SlDlDownloader.Socket.id )
			{
				//	֪ͨ�ϲ�����ʧ��
				i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_CREATESOC , 0 , 0 , 0 , 0 ) ;

				return ;
			}  

			i51SlDlDownloader.state = i51_SAP_DL_STATE_CONNECT ;

			//	http header composing bellow
			{

//				iCHAR header [1024] = { 0 } ;
				iCHAR *header = (iCHAR *)i51AdeOsMalloc ( 1024 , __FILE__, __LINE__ ) ;
				memset ( header , 0 , 1024 ) ;
				if ( header == 0 ) {
				
					i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlDownloading malloc F ");
					i51SlDlStopProcess () ;
					i51SlDlDownloader.User.callback ( i51_SL_DL_ERROR_NET , 
					0 , 0 ,0 , 0 ) ;
					return ;
					
				}
				i51SlDlBuildHttpHeader ( header , 1024 , 80 , (iCHAR *) i51HOST ) ;
				i51SlDlBuildRequest ( header ) ; 
				i51AdeOsFree ( header ) ;
				
			}
			
		//	ע��˴�û��break ; 


		//	���ӣ����ͣ��������ս����֪ͨ�ص������ֻ��״̬�Ĵ���
		case i51_SAP_DL_STATE_CONNECT :
		{

			#if 0
			iS32	ret = 0 ;
			ret = i51AdeSocConnect (
				i51SlDlDownloader.Socket.id , 
				&i51SlDlDownloader.Socket.server , 
				i51SocConCallback 
				) ;

			if (  -1 == ret  )
			{
				//	���ӳ����ر�����
				i51AdeOsLog ( i51_LOG_SC , "SAP - DL : CONN F (3)\r\n" ) ;
//				i51KitCoreSlDlStop (0) ;
				//	֪ͨӦ�ò�
				
			}else{		

				if ( i51SlDlDownloader.state <= i51_SAP_DL_STATE_CONNECT )
					i51SlDlDownloader.state = i51_SAP_DL_STATE_IDLE ;
			}
			#else
			i51SlDlTimerStart (2) ;
			i51AdeSocConnect (
				i51SlDlDownloader.Socket.id , 
				&i51SlDlDownloader.Socket.server , 
				i51SocConCallback 
			) ;
			if ( i51SlDlDownloader.state <= i51_SAP_DL_STATE_CONNECT )
					i51SlDlDownloader.state = i51_SAP_DL_STATE_IDLE ;
			#endif

		}
		
		break ;

		case i51_SAP_DL_STATE_SEND :
			if ( i51SlDlDownloader.Socket.SendTotall == i51SlDlDownloader.Socket.SendOffset )
	        	{        		

				i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlDownloading %d(B) Send , Finished\r\n" , i51SlDlDownloader.Socket.SendOffset );
	        		i51SlDlDownloader.state = i51_SAP_DL_STATE_RECEIVE ;
					
	        	}
			else 
			{
			#if 0
				iS32 ret = 0 ;
				
				ret = i51AdeSocSend ( 
					i51SlDlDownloader.Socket.id , 
					(iU8*)((iU32)i51SlDlDownloader.cache+i51SlDlDownloader.Socket.SendOffset), 
					i51SlDlDownloader.Socket.SendTotall-i51SlDlDownloader.Socket.SendOffset , 
					i51SocSendCallback ) ;
				
				if ( -1 == ret )
				{
					i51AdeOsLog ( i51_LOG_SC , "SAP - DL : CONN F (4)\r\n" ) ;
					//	���ӳ����ر�����
					//i51SlDlDownloader.state = i51_SAP_DL_STATE_CLOSE ;
//					i51KitCoreSlDlStop (0) ;
				
				}
				else if ( 1 == ret )
				{
					if ( i51SlDlDownloader.state <= i51_SAP_DL_STATE_SEND ) {
						
					i51SlDlDownloader.Socket.SendOffset += ret ;
					i51SlDlDownloader.state = i51_SAP_DL_STATE_IDLE ;
					}
				}
				else if ( 0 == ret )
				{
					//	�ȴ��´η���
					if ( i51SlDlDownloader.state <= i51_SAP_DL_STATE_SEND ) {
	
						i51SlDlDownloader.state = i51_SAP_DL_STATE_IDLE ;
					}				
				}

				i51AdeOsLog ( i51_LOG_SC , "SAP - DL : Send = %d , Totall %d\r\n" , 
					i51SlDlDownloader.Socket.SendOffset , i51SlDlDownloader.Socket.SendTotall ) ;
			#else
			 i51AdeSocSend ( 
					i51SlDlDownloader.Socket.id , 
					(iU8*)((iU32)i51SlDlDownloader.cache+i51SlDlDownloader.Socket.SendOffset), 
					i51SlDlDownloader.Socket.SendTotall-i51SlDlDownloader.Socket.SendOffset , 
					i51SocSendCallback ) ;
			if ( i51SlDlDownloader.state <= i51_SAP_DL_STATE_SEND ) {
	
				i51SlDlDownloader.state = i51_SAP_DL_STATE_IDLE ;
			}			
			#endif

			}
			
		break ;

		case i51_SAP_DL_STATE_RECEIVE :
		{

		#if 0
			iS32 ret = 0 ;
			ret = i51AdeSocReceive ( 
				i51SlDlDownloader.Socket.id , 
				(iU8*)(i51SlDlDownloader.cache+i51SlDlDownloader.Socket.RecvOffset) , 
				i51SlDlCAHCE_SIZE-i51SlDlDownloader.Socket.RecvOffset , 
				i51SocRecvCallback
				) ;
//			i51AdeOsLog ( i51_LOG_SC , "cont  %s\r\n" , i51SlDlDownloader.cache ) ;
			if ( -1 == ret )
			{
				i51AdeOsLog ( i51_LOG_SC , "SAP - DL : CONN F (4)\r\n" ) ;
				//	���ӳ����ر�����
				//i51SlDlDownloader.state = i51_SAP_DL_STATE_CLOSE ;
				//i51KitCoreSlDlStop (0) ;
				
			} else{
				//	�ȴ��´ν���
				//	��Ϊ�ص��������첽�����> 0Ҳ��Ҫ�ȴ��´ν���
//				i51SlDlDownloader.state = i51_SAP_DL_STATE_IDLE;
			}
		#else
			 i51AdeSocReceive ( 
				i51SlDlDownloader.Socket.id , 
				(iU8*)(i51SlDlDownloader.cache+i51SlDlDownloader.Socket.RecvOffset) , 
				i51SlDlCAHCE_SIZE-i51SlDlDownloader.Socket.RecvOffset , 
				i51SocRecvCallback
			) ;
		if (i51SlDlDownloader.state ==  i51_SAP_DL_STATE_RECEIVE)
			i51SlDlDownloader.state = i51_SAP_DL_STATE_IDLE;
			
		#endif
		


		}
		break ;

		case i51_SAP_DL_STATE_CLOSE :
  
			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDlDownloading Stop\r\n" ) ;
			i51SlDlStopProcess () ;		  
			
		break ;
		
	}

	
}

static void i51SlBuildLinkLayerTimerCallback (iU32 id)
{

	i51AdeOsTimerStop (id) ;
	//	����ʧ�ܣ���״̬��ԭ
	AlreadyCreate = iFALSE ;
	i51SlDlDownloader.i51SlDlCreateCallback (i51SlDlDownloader.LinkLayer.successed) ;
}

static void i51SlBuildLinkLayerCallback (iBOOL ret)
{	

	if (i51SlDlDownloader.monitortimer > 0) {
		
		i51AdeOsTimerStop ( i51SlDlDownloader.monitortimer) ;  
		i51SlDlDownloader.monitortimer = 0 ;
	}
	
	if (ret) {

		i51SlDlDownloader.Download.DownloaderIsExsit = iTRUE ;
		i51SlDlDownloader.LinkLayer.successed = iTRUE ;
		i51SlDlDownloader.i51SlDlCreateCallback (i51SlDlDownloader.LinkLayer.successed) ;
	}else {

		i51SlDlDownloader.LinkLayer.successed = iFALSE ;
		//	����ʧ�ܣ���״̬��ԭ
		AlreadyCreate = iFALSE ;
		i51AdeSocCloseLinkLayer ( (iU32 )&i51KitCoreSlDlCreate , i51SlDlDownloader.Download.layerid ) ;
		i51AdeSocReleaseLinkLayer (i51SlDlDownloader.Download.layerid) ;
		i51SlDlDownloader.i51SlDlCreateCallback (i51SlDlDownloader.LinkLayer.successed) ;
	}
}

static void i51SlDl_i51SlDlCALLBAC_TIMERCB ( iU32 id )
{

	i51AdeOsTimerStop(id) ;
	i51SlDlDownloader.User.callback ( i51SlCallbackParameter.state , 
								i51SlCallbackParameter.textlen ,
								i51SlCallbackParameter.offset ,
								i51SlCallbackParameter.totall ,
								i51SlCallbackParameter.refcount ) ;
}
static iBOOL i51SlDl_ASYN_i51SlDlCALLBACK (  iS32 state , iU32 textlen , iU32 offset , iU32 totall , iU32 refcount  )
{

	//	i51SlDlCALLBACK �ص����첽֪ͨ

	iU32 timerid = 0 ;
	i51SlCallbackParameter.state = state ;
	i51SlCallbackParameter.textlen = textlen ;
	i51SlCallbackParameter.offset = offset ;
	i51SlCallbackParameter.totall = totall ;
	i51SlCallbackParameter.refcount = refcount ;

	timerid = i51AdeOsTimerCreate ( 0 , 1 ) ;
	if ( 0 == timerid )
	{
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDl_ASYN_i51SlDlCALLBACK TC F\r\n" ) ;
		return iFALSE ;
	}

	if ( 0 == i51AdeOsTimerStart (timerid , i51SlDl_i51SlDlCALLBAC_TIMERCB , 1 ) )
	{
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51SlDl_ASYN_i51SlDlCALLBACK TS F\r\n" ) ;		
		return iFALSE ;
	}		

	
	

}


iBOOL i51KitCoreSlDlAbleCreate ()
{

	//	author : William
	//	since : 2012.4.17
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	

	//	dl �Ƿ���Ա�����
	//	���Դ���true
	//	�����Դ���false
	return !AlreadyCreate ;
}


iHANDLE i51KitCoreSlDlCreate (void (*callback) (iBOOL ret))
{

	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
  
	//	notes : ��ʼ��������

	//	�ɹ�����iTRUE , ʧ�ܷ���iFALSE

	iBOOL ret = 0 ;
	iS32 *LinkMode = NULL ;
	iS32 *sims = NULL ;
	iBOOL LinkIsExist = iFALSE ;
	iS32 simid = 0 , linkmode = 0 ;

	memset ( gReUrl , 0 , sizeof(gReUrl) ) ;
	i51SlDlDownloader.i51SlDlCreateCallback = callback ;

	AlreadyCreate = iTRUE ;

	i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51KitCoreSlDlCreate start create" ) ;

	//	������·ǰ ,�ж���·�Ƿ����
	i51AdeSocGetLinkLayerStatus (&LinkIsExist , &simid , &linkmode) ;
	if ( LinkIsExist ) {

		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51KitCoreSlDlCreate Net Link exsit" ) ;
		i51SlDlDownloader.Download.layerid = i51AdeSocCreateLinkLayer (linkmode, simid) ;
	} else {
	
		LinkMode =  i51AdeSocGetLinkLayerMode () ;
		sims = i51AdeSmsGetSims () ;
		
		if ( i51DlOvertimeAndNet.gNetIsCmnet )
			i51SlDlDownloader.Download.layerid = i51AdeSocCreateLinkLayer (LinkMode[2], sims[1]) ;
		else
			i51SlDlDownloader.Download.layerid = i51AdeSocCreateLinkLayer (LinkMode[1], sims[1]) ;
	}
	if (!i51SlDlDownloader.Download.layerid) {

		// ������·ʧ�ܣ��첽֪ͨ
		i51SlDlDownloader.LinkLayer.successed = iFALSE ;
		i51SlDlDownloader.Createdltimer = i51AdeOsTimerCreate ( 0 , 1 ) ;
		if ( i51SlDlDownloader.Createdltimer == 0 ) {

			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51KitCoreSlDlCreate TC F" ) ;	
//			i51SlDlDownloader.i51SlDlCreateCallback (iFALSE) ;
			AlreadyCreate = iFALSE ;
			return iFALSE ;
		}
		if ( 0 == i51AdeOsTimerStart ( i51SlDlDownloader.Createdltimer , i51SlBuildLinkLayerTimerCallback , 1 ) )
		{
			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51KitCoreSlDlCreate TS F" ) ;	
//			i51SlDlDownloader.i51SlDlCreateCallback (iFALSE) ;
			return iFALSE ;
		}		
		return 0 ;
	}
	i51SlDlTimerStart (1) ;

	//	Jelo Edting Start 2011.12.15
	//	i51AdeSocOpenLinkLayer (i51SlDlDownloader.Download.layerid , i51SlBuildLinkLayerCallback) ;
	//	Jelo Edting Finished

	//	Jelo Adding Start 2011.12.15
	i51AdeSocOpenLinkLayer ( (iU32 )&i51KitCoreSlDlCreate , i51SlDlDownloader.Download.layerid , i51SlBuildLinkLayerCallback ) ;
	//	Jelo Adding Finished
	
	return (iHANDLE )&i51SlDlDownloader ; 

}

iBOOL i51KitCoreSlDlStop ( iHANDLE dl )
{

	//	author : Jelo
	//	since : 2011.7.12
	// 	(C) 2011 , PKIG Tech. Co., Ltd.	
  	
	//	notes : �˳����ؽ���
	//	���ùܲ���dl����Ϊֻ�ܴ���һ��������������i51SlDlDownloader

//	if ( i51SlDlDownloader.Download.DownloaderIsExsit  ) {

	i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51KitCoreSlDlStop" ) ;	
	memset (&i51DlOvertimeAndNet , 0 , sizeof(i51DlOvertimeAndNet)) ;
	memset (&i51DlStartParameters , 0 , sizeof(i51DlStartParameters)) ;
	i51SlDlStopProcess () ;

	//	Jelo Edting Start 2011.12.15
	//	i51AdeSocCloseLinkLayer (i51SlDlDownloader.Download.layerid) ;
	//	Jelo Edting Finished

	//	Jelo Adding Start 2011.12.15
	i51AdeSocCloseLinkLayer ( (iU32 )&i51KitCoreSlDlCreate , i51SlDlDownloader.Download.layerid ) ;
	//	Jelo Adding Finished

//	}
	if ( i51SlDlDownloader.Download.layerid )
		i51AdeSocReleaseLinkLayer (i51SlDlDownloader.Download.layerid) ;
	//	�ڴ�����·��û�лص�ʱ��Ҳ�ᴴ����ʱ��ʱ��
	if (i51SlDlDownloader.monitortimer > 0) {
		
		i51AdeOsTimerStop ( i51SlDlDownloader.monitortimer) ; 
		i51SlDlDownloader.monitortimer = 0 ;
	}
	i51SlDlDownloader.Download.layerid = 0 ;
	i51SlDlDownloader.Download.DownloaderIsExsit  = iFALSE ;
	//	stop ֮��״̬��ԭ
	AlreadyCreate = iFALSE ;
	return iTRUE ;
	
}

iBOOL i51KitCoreSlDlStart ( 
	iHANDLE dl , 
	const iCHAR *uri , 
	iU32 urisize ,  
	const iCHAR* path , 
	const iCHAR* name , 
	iU32 versionId ,
	i51SlDlCOMMAND cmd , 
	i51SlDlCALLBACK callback 
)
{
  

	//	author : Jelo
	//	since : 2011.6.23
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	//	notes : �ɹ�����iTRUE , ʧ�ܷ���iFALSE
	//	���ùܲ���dl����Ϊֻ�ܴ���һ��������������i51SlDlDownloader


		iCHAR *p1 = iNULL , *p2 = iNULL ;
		iCHAR fieldName8bit[32] ;
		iU16 fieldName16bit[32] ;
		iU32 layerid = 0 ;
		iBOOL DownloaderIsExsit ;

		i51DlStartParameters.dl = dl ;
		i51DlStartParameters.callback = callback ;
		i51DlStartParameters.cmd = cmd ;
		strcpy(i51DlStartParameters.name , name) ;
		strcpy(i51DlStartParameters.path ,path) ;
		strcpy(i51DlStartParameters.uri , uri ) ;
		i51DlStartParameters.urisize = urisize ;
		i51DlStartParameters.versionId = versionId ;
		p1 = (iCHAR*)path ;


		

		layerid = i51SlDlDownloader.Download.layerid  ;
		DownloaderIsExsit = i51SlDlDownloader.Download.DownloaderIsExsit ;
		memset ( &i51SlDlDownloader , 0 , sizeof(i51SlDl) ) ;   
		i51SlDlDownloader.Download.layerid = layerid ;
		i51SlDlDownloader.Download.DownloaderIsExsit  = DownloaderIsExsit ;

		
		i51AdeOsLog ( i51_LOG_SC , "SL - DL :  i51KitCoreSlDlStart S , %s , %d  , %s , %s , %d" , uri ,urisize , path , name ,versionId ) ; 
		iASSERT(callback) ;
		
		if ( 0 != path ) {
			
			iASSERT ( strlen (path) < i51SlDlPATH_SIZE ) ;
			while (1) {

				memset ( fieldName8bit , 0 , sizeof (fieldName8bit) ) ;
				memset ( fieldName16bit , 0 , sizeof (fieldName16bit) ) ;
				p2 = (iCHAR*)strstr(p1,"\\") ;
				if ( p2 ) {
					
					strncpy ( fieldName8bit ,path , p2 - path ) ;
				} else {

					break ;
				}
				i51AdeStdAscii2Unicode (fieldName8bit,fieldName16bit) ;
				if ( !i51AdeOsFileIsDirExist(fieldName16bit)) {

					i51AdeOsFileCreateDir (fieldName16bit) ;
				}
				if ( !p2 )
					break ; 
				p1 = p2 ;
				p1 ++ ;
		
			}
		}

	i51SlDlDownloader.LinkLayer.type = i51_SAP_DL_LINKLAYER_CMWAP ;
	i51SlDlDownloader.state = i51_SAP_DL_STATE_INIT ;

	i51SlDlDownloader.User.reqmode = i51_SAP_DL_REQ_MODE_AUTO ;
	i51SlDlDownloader.User.command = cmd ;
	i51SlDlDownloader.User.callback = callback ;
	i51SlDlDownloader.Download.versionId= versionId ;	

	if ( 0 != path )
		strcpy ( i51SlDlDownloader.Download.path , path ) ; 
	if (name)
		strcpy(i51SlDlDownloader.Download.name ,(iCHAR*)name );
	if (uri)
		strcpy(i51SlDlDownloader.Download.uri , (iCHAR*) uri ) ;
	i51SlDlDownloader.Download.urisize = urisize ;
    
    TEST_FLAG = iFALSE ;
	if ( i51AdeOsFileIsExist (L"TEST.TXT") )
	{
        iFILE fh;
        iS32 fhsize,i,m,j=0;
        iU32 Read,TEST_result[3];
        iU8 readbuff[50]={0};
        iU8 savebuff[10]={0};
        
        fh = i51AdeOsFileOpen( L"TEST.TXT" , i51_OS_FL_EXISTING_OPEN |i51_OS_FL_READ , __FILE__ , __LINE__ );
        if(!fh)
            i51AdeOsLog( 0 ,"SL - DL : i51KitCoreSlDlStart open TEST.TXT F");
        else
        {
            memset(readbuff, 0 ,50 );
            i51AdeOsFileGetSize(fh,&fhsize);
            i51AdeOsFileSetSeek( fh , 0, i51_OS_FL_SEEK_HEAD);
            i51AdeOsFileRead(fh , (void *)readbuff , fhsize,  &Read);
            i51AdeOsFileClose(fh);

            for(i=0;i<fhsize;i++)
            {
                if(readbuff[i]==':')
                {
                    memset(savebuff, 0 ,10 );
                    
                    for(m=0;(m<10)&&(readbuff[i+m+1]!='\r');m++)
                        savebuff[m]=readbuff[i+m+1];
                    
                    TEST_result[j] = atoi((char*)savebuff);
                    j++;
                }
            }
            TEST_ProjectId = TEST_result[0];
            TEST_LCDWidtrh = (iU16)TEST_result[1];
            TEST_LCDHeight = (iU16)TEST_result[2];
            i51AdeOsLog( 0 ,"SL - DL : TEST_ProjectId=%d,TEST_LCDWidtrh=%d,TEST_LCDHeight=%d,",TEST_ProjectId,TEST_LCDWidtrh,TEST_LCDHeight);
            TEST_FLAG = iTRUE;
        }
    }
    
	if ( i51AdeOsFileIsExist (L"i51TEST") )
	{
		strcpy ( (iCHAR*)i51HOST , i51TESTHOST ) ;	
		i51AdeSocInetAton ( I51_TEST_ADDR , &i51SlDlDownloader.Socket.server.ipaddr) ;
	}
	else
	{
		strcpy ((iCHAR*) i51HOST , i51REALHOST ) ;
		i51AdeSocInetAton ( I51_ADDR , &i51SlDlDownloader.Socket.server.ipaddr) ;
	}  

	i51SlDlDownloader.Socket.server.port = 80  ;

	i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51KitCoreSlDlStart HOST %s\r\n" , i51HOST ) ;
	
	if ( i51_SAP_DLCOM_BITSTREAM == i51SlDlDownloader.User.command )
	{
		//	checking verify
		if ( iFALSE == i51SlDlVerifyCheck () ) 
		{
			i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51KitCoreSlDlStart FILE CHECK F" ) ;		
			i51SlDl_ASYN_i51SlDlCALLBACK ( i51_SL_DL_ERROR_FILE, 0 , 0 , 0 , 0 ) ;
			return iFALSE;
		}
	}

	i51SlDlDownloader.timer = i51AdeOsTimerCreate ( 1 , 1 ) ;
	if ( 0 == i51SlDlDownloader.timer )
	{
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51KitCoreSlDlStart TC F" ) ;
		return iFALSE ;
	}

	if ( 0 == i51AdeOsTimerStart ( i51SlDlDownloader.timer , i51SlDlDownloading , 100 ) )
	{
		i51AdeOsLog ( i51_LOG_SC , "SL - DL : i51KitCoreSlDlStart TS" ) ;	
		return iFALSE ;
	}		
	i51AdeOsLog ( 0 , "SL - Dl : i51KitCoreSlDlStart timerid = %d" , i51SlDlDownloader.timer) ;
	
	return iTRUE ; 
	
}