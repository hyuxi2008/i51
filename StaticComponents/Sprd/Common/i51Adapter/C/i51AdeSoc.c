 
/*

	The Implementing of 'Adapter' , Module : 'SOC'.	
	Layer belong : 'Static Components'.
	For more details about 'Adapter' you could read : "The_i51_SC_Adapter_Interfaces.xls"
	For more details about 'Static Components' you could read : "The_i51_Architecture_2.0.xls"
	
	The 51PK Platform was implementing based on 'The i51 Architecture'.
	'The i51 Architecture' is designed by Jelo.
	
	Copyright (C) 2011 , PKIG Tech. Co., Ltd.
 
*/

#include "i51AdeOs.h"
#include "i51AdeSoc.h"
#include "i51_define.h"

#include "os_api.h"
#include "scttypes.h"
#include "socket_types.h"
#include "ctypes.h"
#include "sci_api.h"
#include "tasks_id.h"
#include "mn_type.h"        
#include "sim_macro_switch.h"
#include "sim_type.h"
#include "sim_file_structure.h"
#include "nv_type.h"
#include "mmisms_nv.h"
#include "mmisms_send.h"
#include "mmi_signal_ext.h"
#include "mmisms_app.h"

#ifdef _SPRD_6800H_
#include "mmidm_app.h"
#include "mmidm_nv.h"
#endif

#ifdef SPRD_6530
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#include "mmibrowser_export.h"
#endif


#if defined(SPRD_6620)||defined(SPRD_6800)
#include "mmipdp_api.h"
#endif



#include "mn_api.h"
#if defined (SPRD_6600) || defined (SPRD_6620) || defined (SPRD_6800)
#include "mmiphone.h"
#include "mmiconnection.h"      
#endif
#include "sig_code.h"
#include "Socket_api.h"

#define i51SOCSELECT_TIME 300
#define i51SOCLLINKER_MODE_CMWAP 1  
#define i51SOCLLINKER_MODE_CMNET 2
	
//	���5 ��socket
#define i51SOCMAX_COUNT 5

#define MAX_LINKED_COUNT 32

typedef enum
{

	//	author : jelo
	//	since : 2011.6.22
	//	(C) PKIG Tech. Co., Ltd.

	i51_SOC_POLL_CONNECT ,
	i51_SOC_POLL_SEND ,
	i51_SOC_POLL_RECEIVE ,

} i51SOCPOLLTYPE ;
typedef enum {

	//	author : william
	//	since : 20110830
	//	(C)NON TEST

	//cmwap ����
	I51_NET_CONNCT_CMWAP = 1 ,
	
	//cmnet ����
	I51_NET_CONNCT_CMNET ,
	
	// 3g����
	I51_NET_CONNCT_3G ,
	
	//wifi
	I51_NET_CONNCT_WIFI 
	
}ENUM_NET_CONNCT_MODE ;

typedef enum {

	//	author : william
	//	since : 20110830
	//	(C)NON TEST

	//��1
	I51_SIM_CARD_1 ,
	
	//��2
	I51_SIM_CARD_2 ,
	
	//��3
	I51_SIM_CARD_3 ,
	
	//��4
	I51_SIM_CARD_4 ,


} ENUM_SIM_CARD ;


typedef struct 
{

	//	author : jelo
	//	since : 2011.6.22
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : ������ѯsocket ״̬
	//	�ýṹ��Ķ��岻��i51 Adapter ��׼�淶Լ��
	//	�����߿������ⶨ��
	
	//	��Ҫ��ѯ��socket
	iS32 socket ;

	//	connetct ,send ��ѯselect timer
	iU32 timer ; 

	//	��������timer Ϊѭ���Ķ�ʱ��
	iU32 ReceiveTimer ;

	//	��ѯ�ص�timer
	iU32 CallBackTimer ;

	//	���ʹ��������ڳ�����
	iU32 sendcount ;
	
	//	��ѯ����
	i51SOCPOLLTYPE type ;

	//	send , receive buffer , ���ⲿ�������ṩ
	iU8* buffer ;
	iU32 length ;

	

	//	�û�֪ͨӦ�ò�Ļص�
	struct 
	{

		//	�ص������Լ��ص�����		
		iBOOL ConnState ;
		i51SOCCONCBK ConnCallBack ;

		//	�ص������Լ��ص�����
		iU32 SendTextLen ;
		iBOOL SendState ;		
		i51SOCSENDCBK SendCallBack ;

		//	�ص������Լ��ص�����
		iBOOL RecvState ;
		iU8* RecvBuffer ;		
		iU32 RecvTextlen ;		
		i51SOCRECVCBK RecvCallBack ;

	} CallBack ;

} i51SOCPOLLER ;

struct {

	//	soc��· �ṹ

	//	����ģʽ1��CMWAP��2��CMNET��3��3G��4��WIFI��
	iU8 i51NetMode ;
	//	ѡ���sim��
	iU8 i51ChooseSim  ;

	//	Jelo Edited Start 2011.12.15
	//������·�Ĵ���
     // iU32 i51CreateLinkCount ;
	//	Jelo Editing Finished
	
	//	��·�Ƿ��Ѿ�����
	iBOOL LinkExsit ;
	
	iBOOL used[MAX_LINKED_COUNT] ;
	//	������·��·id
	iU32 layerid ;
	//	�ص��ڲ�����ÿ���ص������Լ��Ĳ���
	iBOOL CreateLayerState[MAX_LINKED_COUNT] ;
	//	�ص��������Լ���timerid
	iU32 timerid[MAX_LINKED_COUNT] ;
	//	������·�ص�
	
	i51LLAYERCALLBACK callback[MAX_LINKED_COUNT] ;
	
	
}i51SocLinkLayer = {0} ;
static iS32 i51AdeSocNetid = -1 ;
static iU32 i51SocPollerWalker = 0 ;
static i51SOCPOLLER i51SocPoller [ i51SOCMAX_COUNT ] = {{-1},{-1},{-1},{-1},{-1}} ;
static iU32 i51AdeCreateTimer = 0 ;
static i51ADECALLBACK i51AdeCallback = 0 ;
static i51LLAYERCALLBACK i51GprsCallBack = 0 ;
static iU32 GprsStack = 0 ;


static iU8 i51ConvetSimCard ( iS32 format)
{

	//	author : william
	//	since : 20110801
	//	PKIG Tech. Co., Ltd.
	
	switch ( format  ) {

		case 1 :
			return I51_SIM_CARD_1 ;
		case 2 :
			return I51_SIM_CARD_2 ;
		case 3 :
			return I51_SIM_CARD_3 ;
		case 4 :
			return I51_SIM_CARD_4 ;		
		default:
            i51AdeOsLog(0, "SOC : i51ConvetSimCard , NO SIM Card" ) ;
			return 0 ;
	
	}
	
} 



static void i51AdeSocStartCallBack ( iU8 timer_id )
{

	//	author : jelo 
	//	since : 2011.6.23
	//	(C) PKIG Tech. Co., Ltd.    

	//	�ýӿڵĶ��岻��i51 Adapter ��׼�淶Լ��
	//	�����߿������ⶨ��

	iU32 looper = 0 ;
	//	��ѯ������Ҫ�ص����׽���
	for ( looper = 0 ; looper < i51SOCMAX_COUNT ; looper ++ )
	{

		if ( timer_id  != i51SocPoller[looper].CallBackTimer )
			continue ;

		if ( i51SocPoller[looper].CallBackTimer ) {

			i51AdeOsTimerStop ( i51SocPoller[looper].CallBackTimer ) ;	
			i51SocPoller[looper].CallBackTimer = 0 ;
		}

		//	����ΪʲôҪ���رն�ʱ���Ĳ����ŵ�����ȥ
//		i51AdeOsTimerStop ( i51SocPoller[looper].CallBackTimer ) ;	
//		i51SocPoller[looper].CallBackTimer = 0 ;
		
		if ( 0 != i51SocPoller[looper].CallBack.ConnCallBack )
		{
			i51SocPoller[looper].CallBack.ConnCallBack ( 
				i51SocPoller[looper].CallBack.ConnState , 
				looper  ) ;
			i51SocPoller[looper].CallBack.ConnCallBack = 0 ;
			break ;
		}
		 
		if ( 0 != i51SocPoller[looper].CallBack.SendCallBack )
		{		
			i51SocPoller[looper].CallBack.SendCallBack ( 
			i51SocPoller[looper].CallBack.SendState , 
			looper  , i51SocPoller[looper].CallBack.SendTextLen ) ;
			//������send�ɹ��Ļص������ڵ���send����������β�����0
//			i51SocPoller[looper].CallBack.SendCallBack = 0 ;
			break ;
		}
	}

	if ( looper == i51SOCMAX_COUNT ) {

		//	��ֹ���ص���û�й�����ʱ��soc close �����������ʱ��û�йص�
        i51AdeOsLog(0, "SOC : i51AdeSocStartCallBack , no match" ) ;
		i51AdeOsTimerStop ( timer_id ) ;
	}

	
}

static void i51AdeSocSelect ( iU8 timer_id )
{

	//	author : jelo
	//	since : 2011.6.22
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : ��ѯsocket 
	//	�ýӿڵĶ��岻��i51 Adapter ��׼�淶Լ��
	//	�����߿������ⶨ��

	iS32 errorCode;
	sci_fd_set read_fdset;
	sci_fd_set write_fdset;
	sci_fd_set exp_fdset;
	iS64 time_term = 0;
	iU32 looper = 0 ;

//	i51AdeOsLog(0, "SOC : i51AdeSocSelect , %d " , timer_id ) ;
	//	��ѯ�����׽���
	for ( looper = 0 ; looper < i51SOCMAX_COUNT ; looper ++ )
	{

		if ( -1 == i51SocPoller[looper].socket ) 
			continue ;

		if ( timer_id  != i51SocPoller[looper].timer )
			continue ;

        i51AdeOsLog(0, "SOC : i51AdeSocSelect , looper = %d " , looper ) ;
	
		SCI_FD_ZERO(&read_fdset);
		SCI_FD_ZERO(&write_fdset);
		SCI_FD_ZERO(&exp_fdset);	
			
		if ( i51_SOC_POLL_CONNECT == i51SocPoller[looper].type )
		{

		       SCI_FD_SET(i51SocPoller[looper].socket, &write_fdset);
			errorCode = sci_sock_select(&read_fdset, &write_fdset, &exp_fdset, time_term);		

		}
		else if ( i51_SOC_POLL_SEND == i51SocPoller[looper].type )
		{

		       SCI_FD_SET(i51SocPoller[looper].socket, &write_fdset);
			errorCode = sci_sock_select(&read_fdset, &write_fdset, &exp_fdset, time_term);		

		}

		//	֪ͨ�ϲ�Socket ��д
	       if ( (errorCode >0) && (0<SCI_FD_ISSET(i51SocPoller[looper].socket, &write_fdset)))
	       {    

            i51AdeOsLog(0, "SOC : i51AdeSocSelect , socket can write ") ;
			if ( i51SocPoller[looper].timer ) {

				//	�ر���ѯ���Ӻͷ��͵Ķ�ʱ��
				i51AdeOsTimerStop ( i51SocPoller[looper].timer ) ;
				i51SocPoller[looper].timer = 0 ;
			}

			if ( i51_SOC_POLL_CONNECT == i51SocPoller[looper].type )
			{
				if ( 0 != i51SocPoller[looper].CallBack.ConnCallBack )
					i51SocPoller[looper].CallBack.ConnCallBack ( iTRUE , looper ) ;
				i51SocPoller[looper].CallBack.ConnCallBack = 0 ;
			}
			else if ( i51_SOC_POLL_SEND == i51SocPoller[looper].type )
			{
				if ( 0 != i51SocPoller[looper].CallBack.SendCallBack )			
					i51SocPoller[looper].CallBack.SendCallBack ( iTRUE , looper , i51SocPoller[looper].CallBack.SendTextLen ) ;		
				//������send�ɹ��Ļص������ڵ���send����������β�����0
//				i51SocPoller[looper].CallBack.SendCallBack = 0 ;
			}
	              return ;

	       }

		else if ( errorCode < 0 )
		{

			//	�������֪ͨ�ⲿ�ر�����
			
			if ( i51SocPoller[looper].timer ) {

			
				i51AdeOsTimerStop ( i51SocPoller[looper].timer ) ;
				i51SocPoller[looper].timer = 0 ;
			}

			if ( i51_SOC_POLL_CONNECT == i51SocPoller[looper].type )
			{
				if ( 0 != i51SocPoller[looper].CallBack.ConnCallBack ) 
					i51SocPoller[looper].CallBack.ConnCallBack ( iFALSE , looper ) ;
				i51SocPoller[looper].CallBack.ConnCallBack = 0 ;
			}
			else if ( i51_SOC_POLL_SEND == i51SocPoller[looper].type )
			{
				if ( 0 != i51SocPoller[looper].CallBack.SendCallBack )
					i51SocPoller[looper].CallBack.SendCallBack ( iFALSE , looper , 0 ) ;		
//				i51SocPoller[looper].CallBack.SendCallBack = 0 ;
			}
	             return ;

	        }
	        else if ( errorCode == 0 ) 
	        {	

			//	������ѯ, ��Ϊi51SocPoller.timer �Ǹ�������timer
			//	���ر�timer �ú����᲻ͣ�ر����� 
					
	        }

	}
	
}


static void i51AdeSocReceiving ( iU32 id )
{
	//   ��ѯ��������

	iS32 ret = 0 ;
	iU16 looper = 0;
	iU32	error = 0;

	for ( looper = 0 ; looper < i51SOCMAX_COUNT ; looper ++ ) {

		if ( i51SocPoller[looper].ReceiveTimer == id)
			break ;
	}
    i51AdeOsLog(0, "SOC : i51AdeSocReceiving , looper = %d" , looper  ) ;	

	if ( looper == i51SOCMAX_COUNT ) {

        i51AdeOsLog(0, "SOC : i51AdeSocReceiving , not find" ) ;	
		return ;
	}
				
	ret = sci_sock_recv ( (TCPIP_SOCKET_T)i51SocPoller[looper].socket , i51SocPoller[looper].buffer  , i51SocPoller[looper].length , 0 ) ;

	if ( ret > 0 ) {

        i51AdeOsLog(0, "SOC : i51AdeSocReceiving , Recv %d byte" , ret ) ;	
		i51SocPoller[looper].CallBack.RecvCallBack ( iTRUE , looper , i51SocPoller[looper].buffer , ret ) ;
	}

	else if ( 0 > ret )
	{

		error = sci_sock_errno((TCPIP_SOCKET_T)i51SocPoller[looper].socket);
		
		if ( EWOULDBLOCK == error || EINPROGRESS == error )
		{
			//	�������ڽ��ܼ�������
            i51AdeOsLog(0, "SOC : i51AdeSocReceiving , continue check (1)" ) ; 
			return ;   
			
		}
		else
		{
		//	����ʧ��
		//	֪ͨ�ϲ�ر�����
            i51AdeOsLog(0, "SOC : i51AdeSocReceiving , F (1) , %d" , error ) ; 
			//   ��֪ͨ�ϲ�ȴ���ʱ����
//			i51SocPoller[looper].CallBack.RecvCallBack ( iFALSE , 0 , 0 , 0 ) ;
			return ;
		}
       
	}
	
	else if ( 0 == ret )
	{

		error = sci_sock_errno((TCPIP_SOCKET_T)i51SocPoller[looper].socket);

		if ( EWOULDBLOCK == error || EINPROGRESS == error )
		{

            i51AdeOsLog(0, "SOC : i51AdeSocReceiving , continue check (2)" ) ; 
			return ;   
		}
		else
		{
			//	Զ�˷��������ӹر�  
            i51AdeOsLog(0, "SOC : i51AdeSocReceiving , F (2) , %d" , error ) ; 
			//   ��֪ͨ�ϲ�ȴ���ʱ����
//			i51SocPoller[looper].CallBack.RecvCallBack ( iFALSE , 0 , 0 , 0 ) ;
			
			return ;
		}
	}

}
iU32 i51AdeSocHtonl ( iU32 ipAddr )
{

	//	author : Jelo
	//	since : 2011.7.9
	//	(C) PKIG Tech. Co., Ltd.

	//	32λ��������ת��
	
	return htonl( ipAddr ) ;
}

iU16 i51AdeSocHtons( iU16 port )
{

	//	author : Jelo
	//	since : 2011.7.9
	//	(C) PKIG Tech. Co., Ltd.

	//	16λ��������ת��
	
	return htons (port) ;
	
}

iBOOL i51AdeSocInetAton ( iCHAR* ipstr , iU32 *pIPValue)
{
	//	author : Jelo
	//	since : 2011.7.9
	//	(C) PKIG Tech. Co., Ltd.

	//	����: 192.168.1.1
	//	���: c0 a8 01 01
	//	���ֵ��Ϊ������
	       
	iU32 ret = 0;

	ret = inet_aton( ipstr,(TCPIP_IPADDR_T *) pIPValue );

	htonl ( *pIPValue ) ;
	
	return iTRUE ;
	
}

//add by steven since 20120605
typedef struct GetHostCallbackInfo
{
    i51DNSCALLBACK GetHostCallback;
    iU64 ip_addr;
}i51GetHostCallbackInfo;
static i51GetHostCallbackInfo i51CallbackInfo = {0};

static void i51SocGetHostByNameCallback(iU32 ID)
{
    if(NULL!=i51CallbackInfo.GetHostCallback)
    {
        i51CallbackInfo.GetHostCallback ( i51CallbackInfo.ip_addr ) ;
        i51CallbackInfo.GetHostCallback = NULL;
        i51CallbackInfo.ip_addr = 0;
    }
    i51AdeOsTimerStop(ID);
}

static iCHAR DNSNAME[20] = {0};
static iU32 i51Soclayerid = 0;
static i51DNSCALLBACK DnsCallback = NULL;
static iU64 i51GetHostIp_addr = 0;

void i51GetHostCallback( iBOOL state )
{
    struct sci_hostent	*pHostent = 0;
    iU8 i ;
    
    if(state)
    {
        for(i = 0;i<5;i++)
        {
            pHostent = sci_gethostbyname(DNSNAME);

            if(NULL!=pHostent)
            {
                memcpy ( &i51GetHostIp_addr , pHostent->h_addr_list[0] , pHostent->h_length ) ;                
                
                if(NULL!=DnsCallback)DnsCallback(i51GetHostIp_addr);

                DnsCallback = NULL;

                i51AdeOsLog(0, "SOC : i51GetHostCallback , E Linker:%d,%d",i51GetHostIp_addr,i);
                return;
            }    
        }
    }
    if(i51Soclayerid!=0)
    {
        i51AdeSocCloseLinkLayer((iU32)i51AdeSocGetHostByName,i51Soclayerid);
        i51AdeSocReleaseLinkLayer(i51Soclayerid);   
    }
}

static void i51GetHostTime( iU32 id )
{
    if(NULL!=DnsCallback)DnsCallback(i51GetHostIp_addr);

    i51AdeOsLog(0, "SOC : i51GetHostTime , E timer:%d,%d",i51GetHostIp_addr);
    DnsCallback = NULL;
    
    i51AdeOsTimerStop(id);
}

iS32 i51AdeSocGetHostByName ( iCHAR* name , i51DNSCALLBACK callback )
{

	//	author : Jelo	
	//	since : 2011.5.18
	//	(C) PKIG Tech. Co., Ltd.
	
	iS32 *LinkMode = NULL ;
	iS32 *sims = NULL ;	
    iBOOL LinkStats = 0;
    iS32 SimId = 0;
    iS32 mode = 0;
    iU32 TimeId;
    
    struct sci_hostent	*pHostent = 0;
    iU8 i ;
    
    i51AdeOsLog(0, "SOC : i51AdeSocGetHostByName , %s",name);  

	memset(DNSNAME,0,sizeof(DNSNAME));
	memcpy(DNSNAME,name,strlen(name));
	DnsCallback = callback;
    
    i51AdeSocGetLinkLayerStatus ( &LinkStats , &SimId , &mode );
    if(LinkStats==0)
    {
        LinkMode =  i51AdeSocGetLinkLayerMode () ;
    	sims = i51AdeSmsGetSims () ;
    	i51Soclayerid = i51AdeSocCreateLinkLayer( LinkMode[1] , sims[1]) ;
    	if(i51Soclayerid==0)
    	    return 0;
    	if(i51AdeSocOpenLinkLayer((iU32)i51AdeSocGetHostByName,i51Soclayerid, i51GetHostCallback))
    	{
    	    return 1 ;
    	}
    	return 0 ;	
    }

    for(i = 0;i<5;i++)
    {
        pHostent = sci_gethostbyname(DNSNAME);

        if(NULL!=pHostent)
        {
            memcpy ( &i51GetHostIp_addr , pHostent->h_addr_list[0] , pHostent->h_length ) ;  
            TimeId = i51AdeOsTimerCreate(0,1);
            if(i51AdeOsTimerStart(TimeId,i51GetHostTime,5))
            {
                return 1;
            }
        }    
    }
   
    return 0;
	
}

iS32 i51AdeSocGet ()
{

	//	author : Jelo	
	//	since : 2011.5.17
	//	(C) PKIG Tech. Co., Ltd.

	//	returns : if success-->socket fd, else failure---->return < 0	
	
	iS32 sockfd = 0 ;

    i51AdeOsLog(0, "SOC : i51AdeSocGet" ) ;
	//	Jelo Edited 2011.6.8
	sockfd = (iS32) sci_sock_socket ( AF_INET , SOCK_STREAM , 0 , i51AdeSocNetid ) ;

	if ( 0 > sockfd ) return -1 ;

/*
	{
		int wap_send_buflen=1400;
		int net_send_buflen=6144;
		int send_buflen=-1;
		int timeout=-1;

		int keeplive=0;
		

		//buf len
		sci_sock_getsockopt(sockfd,SO_SNDBUF,&send_buflen);
		test_log("old send_buflen=%d",send_buflen);
		
		sci_sock_setsockopt(sockfd,SO_SNDBUF,&wap_send_buflen);
		sci_sock_getsockopt(sockfd,SO_SNDBUF,&send_buflen);
		test_log("new send_buflen=%d",send_buflen);

		//timeout
		//sci_sock_getsockopt(soc_id,SO_SNDTIMEO,&timeout);
		//test_log("old timeout=%d",timeout);

		//keep-live
		sci_sock_getsockopt(sockfd,SO_KEEPALIVE,&keeplive);
		test_log("old keeplive=%d",keeplive);

		keeplive = 1;
		sci_sock_setsockopt(sockfd,SO_KEEPALIVE,&keeplive);
		test_log("new keeplive=%d",keeplive);
	}
*/	

	if ( 0 > sci_sock_setsockopt ( (TCPIP_SOCKET_T)sockfd , SO_NBIO , NULL ) )
	{
		sci_sock_socketclose ( sockfd ) ;
        i51AdeOsLog(0, "SOC : i51AdeSocGet , OPT F\r\n" ) ;
		return -1 ;
	}

	{

		iBOOL found = iFALSE ;
		iU32 index = 0 ;
		iU32 looper = 0 ;

		//	��һ��λ�����
		for ( looper = 0 ; looper < i51SOCMAX_COUNT ; looper ++ )
		{
			
			if ( -1 == i51SocPoller[looper].socket )
			{
				found = iTRUE ;
				index = looper ;
				break ;
			}
		}

		//	û�п���λ��
		if ( iFALSE == found ) 
		{
            i51AdeOsLog(0, "SOC : i51AdeSocGet , N SOCKET\r\n" , index ) ;
			return -1 ;
		}
		 
		memset ( (void*)((iU32)i51SocPoller+index*sizeof(i51SOCPOLLER)) , 0 , sizeof(i51SOCPOLLER) ) ;
		
		i51SocPoller[index].socket = sockfd ;

        i51AdeOsLog(0, "SOC : i51AdeSocGet , %d" , index ) ;
		return index ;
		
	}
	
}

void i51AdeSocConnect ( iS32 sockfd , i51SOCADDR* address , i51SOCCONCBK callback )
{

	//	author : Jelo 
	//	since : 2011.6.22
	//	(C) PKIG Tech. Co., Ltd.

	//	notes : �ɹ�����iTRUE , ʧ�ܷ���iFALSE
	
	iS32 ret  = 0 ;
	iU32	error = 0;
	struct sci_sockaddr addr = { 0 } ;

    i51AdeOsLog(0, "SOC : i51AdeSocConnect , %d , %x , %x " , sockfd ,address , callback ) ;
	if ( !address || 0 > sockfd ) 
	{	
		return ;
	}

	//	˵�����лص�δ����
	//	˵��i51AdeSocSelect δ������
	if ( 0 != i51SocPoller[sockfd].CallBack.ConnCallBack  )
	{		
		return ;  
	}
	
	addr.family = AF_INET ;
	if ( i51SocLinkLayer.i51NetMode == I51_NET_CONNCT_CMWAP) {
		#ifdef  i51SIMULATOR
			addr.port = i51AdeSocHtons(address->port);
			addr.ip_addr = address->ipaddr ;
		#else
		{
		/*
			iU32 host = 0 ;
			addr.port = i51AdeSocHtons (80);
			addr.ip_addr = (10<<24) + 127 ;
			i51AdeOsLog(0, "SOC : i51AdeSocConnect , %d , %d (1) " , addr.port ,  addr.ip_addr ) ;
			inet_aton("10.0.0.172", &host) ;
			i51AdeOsLog(0, "SOC : i51AdeSocConnect , host = %d" , host ) ;
			addr.ip_addr = htons (host) ;
			addr.port = htons (80);
			i51AdeOsLog(0, "SOC : i51AdeSocConnect , %d , %d (2) " , addr.port ,  addr.ip_addr ) ;
		*/
			iU32 host = 0 ;
			addr.port = htons(80);
			inet_aton("10.0.0.172", &host) ;
			addr.ip_addr = host ;
            i51AdeOsLog(0, "SOC : i51AdeSocConnect , cmwap , %d , %d" ,addr.port , addr.ip_addr  ) ;
		}
		#endif
		
	} else if( i51SocLinkLayer.i51NetMode == I51_NET_CONNCT_CMNET) {

		addr.port = i51AdeSocHtons(address->port);
		addr.ip_addr = address->ipaddr ;
        i51AdeOsLog(0, "SOC : i51AdeSocConnect , cmnet , %d , %d" ,addr.port , addr.ip_addr  ) ;
	}else {

        i51AdeOsLog(0, "SOC : i51AdeSocConnect , E %d , Mode error" , error ) ;
	}
	//SCI_MEMCPY ( addr.sa_data , address->ipaddr , sizeof(address->ipaddr) ) ;

	ret = sci_sock_connect( (TCPIP_SOCKET_T)i51SocPoller[sockfd].socket , &addr , sizeof(addr) ) ;
     
	i51SocPoller[sockfd].type = i51_SOC_POLL_CONNECT ;		
	i51SocPoller[sockfd].CallBack.ConnCallBack = callback ;
	
	if ( 0 != ret )
	{	

		error = sci_sock_errno((TCPIP_SOCKET_T)i51SocPoller[sockfd].socket);

		//	�����Ѿ�����
		if ( EISCONN == error )
		{

            i51AdeOsLog(0, "SOC : i51AdeSocConnect , E %d , CF(1)" , error ) ;
			
			i51SocPoller[sockfd].CallBack.ConnState = iTRUE  ;
			i51SocPoller[sockfd].CallBackTimer = i51AdeOsTimerCreate ( iFALSE , 1 ) ;
			i51AdeOsTimerStart ( i51SocPoller[sockfd].CallBackTimer , i51AdeSocStartCallBack , 10 ) ;
			
			return ;
			
		}
		//	���ڽ������ӣ���Ҫ��ʱ��ѯ����״̬
		else if ( EWOULDBLOCK == error || EINPROGRESS == error )
		{

            i51AdeOsLog(0, "SOC : i51AdeSocConnect , E %d , CF(2)" , error ) ;
			
			i51SocPoller[sockfd].timer = i51AdeOsTimerCreate ( iTRUE , 1 ) ;
			i51AdeOsTimerStart ( i51SocPoller[sockfd].timer , i51AdeSocSelect , i51SOCSELECT_TIME ) ;
			
			return ;
			
		}
		else
		{
		//	����ʧ��
		//	֪ͨ�ϲ�ر�����

            i51AdeOsLog(0, "SOC : i51AdeSocConnect , Fatal E %d , CF\r\n" , error ) ; 	
			i51SocPoller[sockfd].CallBack.ConnState = iFALSE ;
			i51SocPoller[sockfd].CallBackTimer = i51AdeOsTimerCreate ( iFALSE , 1 ) ;
			i51AdeOsTimerStart ( i51SocPoller[sockfd].CallBackTimer , i51AdeSocStartCallBack , 10 ) ;
				
			return ;
		}
       
	}
	else
	{
	
		i51SocPoller[sockfd].CallBack.ConnState = iTRUE  ;		
		i51SocPoller[sockfd].CallBackTimer = i51AdeOsTimerCreate ( iFALSE , 1 ) ;
		i51AdeOsTimerStart ( i51SocPoller[sockfd].CallBackTimer , i51AdeSocStartCallBack , 10 ) ;
					
		return ;
		
	}
	
	return ;

}


void http_req_data(char *sendbuf,int bufsize)
{
	
	/*
	GET /go_13596557 HTTP/1.1

	Host: kong.net

	User-Agent: SQH_D480B_01/LB19504/WAP2.0 Profile

	Accept:  application/vnd.wap.wmlc, **  //(�뵱����ʡȥ���沿��)

	Connection: Keep-Alive
	*/

	char CR[]="\r\n";
	
	memset(sendbuf,0x00,bufsize);

	sprintf(sendbuf,"GET /ilogob.gif");
	strcat(sendbuf," HTTP/1.1");
	strcat(sendbuf,CR);
	
	
	strcat(sendbuf,"Accept:*/*");
	strcat(sendbuf,CR);

	strcat(sendbuf,"Connection:Keep-Alive");
	strcat(sendbuf,CR);
	
	strcat(sendbuf,"User-Agent:");
	strcat(sendbuf,"MAUI_WAP_Browser");
	strcat(sendbuf,CR);

	
	strcat(sendbuf,"Host:ps.test.pkingame.net:80");
	strcat(sendbuf,CR);

	strcat(sendbuf,"X-Online-Host:");

	strcat(sendbuf,"ps.test.pkingame.net");

	strcat(sendbuf,CR);
	
	strcat(sendbuf,CR);
	
}

void i51AdeSocSend ( iS32 sockfd , void* data , iU32 length , i51SOCSENDCBK callback )
{

	//	author : Jelo 
	//	since : 2011.6.22
	//	(C) PKIG Tech. Co., Ltd.
	
	//	notes:  ����ֵ-1 ��ʾ��������ϲ�Ӧ����Ҫ���·�������
	//	����0 ��ʾ�豸æ����Ҫ�ȴ�
	//	����n > 0 ����ʾ�ɹ����ͣ�������n ���ֽ�    
	
#if 0
	iS32 ret = 0 ;
	iU32 error = 0 ;

	iS32 errorCode;
	sci_fd_set read_fdset;
	sci_fd_set write_fdset;
	sci_fd_set exp_fdset;
	iS64 time_term = 0;	

	//	˵�����лص�δ����
	//	˵��i51AdeSocSelect ��δ������
	if ( 0 != i51SocPoller.CallBack.SendCallBack  )
		return 0 ;

	//	for select
	SCI_FD_ZERO(&read_fdset);
	SCI_FD_ZERO(&write_fdset);
	SCI_FD_ZERO(&exp_fdset);	
	SCI_FD_SET(sockfd, &write_fdset);
	errorCode = sci_sock_select(&read_fdset, &write_fdset, &exp_fdset, time_term);

    i51AdeOsLog(0, "SOC : i51AdeSocSend , errorCode = %d\r\n" , errorCode ) ;

	i51SocPoller.socket = sockfd ;
	i51SocPoller.type = i51_SOC_POLL_SEND ;
	i51SocPoller.CallBack.SendCallBack = callback ;
		
	if ( (errorCode >0) && (0<SCI_FD_ISSET(sockfd, &write_fdset)))
	{    

		length = sci_sock_send ( (TCPIP_SOCKET_T)sockfd , data , length , 0 ) ;
		
        i51AdeOsLog(0, "SOC : i51AdeSocSend , i51AdeSocReceive = %d\r\n" , length ) ;
			
		if ( -1 == length ) 
			return -1 ;
		else
		{

			//	֪ͨ�ϲ���Լ�������
			
			i51SocPoller.CallBack.SendState = iTRUE  ;
			i51SocPoller.CallBack.SendSocket = sockfd  ;
				
			i51SocCallBackTimer = i51AdeOsTimerCreate ( 1 , 100 ) ;
			i51AdeOsTimerStart ( i51SocCallBackTimer , i51AdeSocStartCallBack , 100 ) ;			

			return length ;			
		}
	      	
	}
	else if ( errorCode < 0 )
	{
		return -1 ;			
	}
	else if ( 0 == errorCode ) ;
	{
							
		//	���ٺ�����ѯһ��socket
		i51SocPoller.timer = i51AdeOsTimerCreate ( 1 , 300 ) ;
		i51AdeOsTimerStart ( i51SocPoller.timer , i51AdeSocSelect , 300 ) ;
				
		return 0 ;

	}

#else

	iS32	ret = 0;
	iU32	error = 0;

    i51AdeOsLog(0, "SOC : i51AdeSocSend , %d , %x , %d , %x " , sockfd , data , length , callback ) ;

	//������send�ɹ��Ļص������ڵ���send����������β�����ô�ж�
	/*
	if ( 0 != i51SocPoller[sockfd].CallBack.SendCallBack  )
		return ;	
	*/
	if ( i51SocPoller[sockfd].sendcount == -1) {

		//	�����ʼ��ֵӰ������
		i51SocPoller[sockfd].sendcount = 0 ;
	} ;

	i51SocPoller[sockfd].sendcount ++ ;
    i51AdeOsLog(0, "SOC : i51AdeSocSend , sendcount = %d" , i51SocPoller[sockfd].sendcount ) ;
	
	ret = sci_sock_send ( (TCPIP_SOCKET_T)i51SocPoller[sockfd].socket , data , length , 0 ) ;	

//	i51AdeOsLog(0, "SOC : i51AdeSocSend , %d %d , %d , %s , %d" ,ret ,  sockfd  ,i51SocPoller[sockfd].socket ,(iCHAR*)data ,length ) ;
	i51SocPoller[sockfd].CallBack.SendTextLen = ret ;
	i51SocPoller[sockfd].type = i51_SOC_POLL_SEND ;	
	i51SocPoller[sockfd].CallBack.SendCallBack = callback ;

	if ( 0 > ret )
	{

		error = sci_sock_errno((TCPIP_SOCKET_T)i51SocPoller[sockfd].socket);
		
		//	���ڷ��ͣ���Ҫ��ʱ��ѯ����״̬
		if ( EWOULDBLOCK == error || EINPROGRESS == error )
		{
		
			i51SocPoller[sockfd].timer = i51AdeOsTimerCreate ( iTRUE , 1 ) ;
			i51AdeOsTimerStart ( i51SocPoller[sockfd].timer , i51AdeSocSelect , i51SOCSELECT_TIME ) ;
			
			return ;
			
		}
		else
		{
		//	����ʧ��
		//	֪ͨ�ϲ�ر�����
		
            i51AdeOsLog(0, "SOC : i51AdeSocSend , Fatal E %d , SF\r\n" ) ; 
			i51SocPoller[sockfd].CallBack.SendState = iFALSE ;
			i51SocPoller[sockfd].CallBackTimer = i51AdeOsTimerCreate ( iFALSE , 1 ) ;
			i51AdeOsTimerStart ( i51SocPoller[sockfd].CallBackTimer , i51AdeSocStartCallBack , 10 ) ;
		
			return ;
		}
       
	}
	else
	{
		//	֪ͨ�ϲ���Լ�������

        i51AdeOsLog(0, "SOC : i51AdeSocSend , send ok" ) ; 
		i51SocPoller[sockfd].CallBack.SendState = iTRUE  ;
		i51SocPoller[sockfd].CallBackTimer = i51AdeOsTimerCreate ( iFALSE , 1 ) ;
		i51AdeOsTimerStart ( i51SocPoller[sockfd].CallBackTimer , i51AdeSocStartCallBack , 10 ) ;
		
	}

	return ;

#endif	

}

void i51AdeSocReceive ( iS32 sockfd , void* data, iU32 length , i51SOCRECVCBK callback )
{

	//	author : Jelo 
	//	since : 2011.6.22
	//	(C) PKIG Tech. Co., Ltd.
	
	//	notes:  ����ֵ-1 ��ʾ��������ϲ�Ӧ����Ҫ���·�������
	//	����0 ��ʾ�豸æ����Ҫ�ȴ�
	//	����1 ����ʾ�ɹ�����


	iS32	ret = 0;
    i51AdeOsLog(0, "SOC : i51AdeSocReceive , %d , %x , %d , %x " , sockfd , data , length , callback ) ;
	

	i51SocPoller[sockfd].type = i51_SOC_POLL_RECEIVE ;
	i51SocPoller[sockfd].buffer = data ;
	i51SocPoller[sockfd].length = length ;
	i51SocPoller[sockfd].CallBack.RecvCallBack = callback ;

	
	i51SocPoller[sockfd].ReceiveTimer = i51AdeOsTimerCreate ( iTRUE , 1 ) ;

	if ( i51SocPoller[sockfd].ReceiveTimer <= 0 ) {

        i51AdeOsLog(0, "SOC : i51AdeSocReceive , Timer F" ) ;
	}
	i51AdeOsTimerStart ( i51SocPoller[sockfd].ReceiveTimer , i51AdeSocReceiving , i51SOCSELECT_TIME ) ;

#if 0		
	if ( 0 > ret )
	{

		error = sci_sock_errno((TCPIP_SOCKET_T)i51SocPoller[sockfd].socket);
		
		//	���ڷ��ͣ���Ҫ��ʱ��ѯ����״̬
		if ( EWOULDBLOCK == error || EINPROGRESS == error )
		{

//			i51SocPoller[sockfd].timer = MMK_CreateTimerCallback ( i51SOCSELECT_TIME , i51AdeSocSelect , 0 , iTRUE ) ;

            i51AdeOsLog(0, "SOC : i51AdeSocReceive , ayns (1)" ) ; 
			i51SocPoller[sockfd].timer = i51AdeOsTimerCreate ( iTRUE , 1 ) ;
            i51AdeOsLog(0, "SOC : i51AdeSocReceive , i51SocPoller[%d].timer = %d" , sockfd , i51SocPoller[sockfd].timer ) ; 
			i51AdeOsTimerStart ( i51SocPoller[sockfd].timer , i51AdeSocSelect , i51SOCSELECT_TIME ) ;
						
			return ;   
			
		}
		else
		{
		//	����ʧ��
		//	֪ͨ�ϲ�ر�����
            i51AdeOsLog(0, "SOC : i51AdeSocReceive , Fatal E %d , RF\r\n" , error ) ; 
			i51SocPoller[sockfd].CallBack.RecvState = iFALSE ;
			i51SocPoller[sockfd].CallBackTimer = i51AdeOsTimerCreate ( iFALSE , 1 ) ;
			i51AdeOsTimerStart ( i51SocPoller[sockfd].CallBackTimer , i51AdeSocStartCallBack , 10 ) ;
			return ;
		}
       
	}
	//	Զ�˷��������ӹر�
	//	�������������ѷ��ͽ���
	//	֪ͨӦ�ò㣬���ζԻ��Ѿ�����	
	else if ( 0 == ret )
	{

		error = sci_sock_errno((TCPIP_SOCKET_T)i51SocPoller[sockfd].socket);

		if ( EWOULDBLOCK == error || EINPROGRESS == error )
		{

			//i51SocPoller[sockfd].timer = MMK_CreateTimerCallback ( i51SOCSELECT_TIME , i51AdeSocSelect , 0 , iTRUE ) ;
			

            i51AdeOsLog(0, "SOC : i51AdeSocReceive , ayns (2)" ) ; 
			i51SocPoller[sockfd].timer = i51AdeOsTimerCreate ( iTRUE , 1 ) ;
			i51AdeOsTimerStart ( i51SocPoller[sockfd].timer , i51AdeSocSelect , i51SOCSELECT_TIME ) ;
			
			return ;   
		}
		else
		{
			//	Զ�˷��������ӹر�  
			//	�������������ѷ��ͽ���
			//	֪ͨӦ�ò㣬���ζԻ��Ѿ�����
			//	����֪ͨӦ�ã������Ѿ�����
			//	Ӧ�ý��յ�����Ϣʱ����������������i51AdeSocConnect��i51AdeSocSend��i51AdeSocReceive

            i51AdeOsLog(0, "SOC : i51AdeSocReceive , Fatal E %d , RF\r\n" , error ) ; 

			i51SocPoller[sockfd].CallBack.RecvState = iFALSE  ;
			i51SocPoller[sockfd].CallBack.RecvBuffer = 0 ;
			i51SocPoller[sockfd].CallBack.RecvTextlen = 0 ;

			i51SocPoller[sockfd].CallBackTimer = i51AdeOsTimerCreate ( iFALSE , 1 ) ;
			i51AdeOsTimerStart ( i51SocPoller[sockfd].CallBackTimer , i51AdeSocStartCallBack , 10 ) ;
		
//			i51SocPoller[sockfd].CallBack.RecvCallBack ( iFALSE , i51SocPoller[sockfd].socket , i51SocPoller[sockfd].buffer , 0 ) ;				
			
			return ;
		}
	
	}
	else
	{

        i51AdeOsLog(0, "SOC : i51AdeSocReceive , recv ok !" ) ; 
		i51SocPoller[sockfd].CallBack.RecvState = iTRUE  ;
		i51SocPoller[sockfd].CallBack.RecvBuffer = data ;
		i51SocPoller[sockfd].CallBack.RecvTextlen = ret ;

//		i51AdeOsLog(0, "SOC : i51AdeSocReceive , %d , %d , %x , %d " , sockfd , i51SocPoller[sockfd].CallBack.RecvState , i51SocPoller[sockfd].CallBack.RecvBuffer , i51SocPoller[sockfd].CallBack.RecvTextlen  ) ; 
		i51SocPoller[sockfd].CallBackTimer = i51AdeOsTimerCreate ( iFALSE , 1 ) ;
		i51AdeOsTimerStart ( i51SocPoller[sockfd].CallBackTimer , i51AdeSocStartCallBack , 10 ) ;
		//	���յ����ݣ����ǻ�Ҫ���ö�ʱ��������Ƿ������ݹ���
		i51SocPoller[sockfd].timer = i51AdeOsTimerCreate ( iTRUE , 1 ) ;
		i51AdeOsTimerStart ( i51SocPoller[sockfd].timer , i51AdeSocSelect , i51SOCSELECT_TIME ) ;
				
	}
	#endif
}

iBOOL i51AdeSocClose ( iS32 sockfd )
{

	//	author : Jelo 
	//	since : 2011.5.19
	//	(C) PKIG Tech. Co., Ltd.
	
	//	notes:  if success, return iTRUE , else iFALSE	

	iS32 ret = 0;

    i51AdeOsLog(0, "SOC : i51AdeSocClose , %d " , sockfd) ;
	if ( 0 > sockfd ) return 0 ;

	if ( i51SOCMAX_COUNT <= sockfd ) return 0 ;

	if ( -1 == i51SocPoller[sockfd].socket )
	{
		//	���ѹر�
		return iTRUE ;
	}
	
	ret = sci_sock_socketclose( (TCPIP_SOCKET_T)i51SocPoller[sockfd].socket ) ;

	if ( i51SocPoller[sockfd].timer )
		i51AdeOsTimerStop ( i51SocPoller[sockfd].timer ) ;
	if (i51SocPoller[sockfd].CallBackTimer)
		i51AdeOsTimerStop ( i51SocPoller[sockfd].CallBackTimer ) ;
	if ( i51SocPoller[sockfd].ReceiveTimer )
		i51AdeOsTimerStop (i51SocPoller[sockfd].ReceiveTimer ) ;
	memset ( (void*)((iU32)i51SocPoller+sockfd*sizeof(i51SOCPOLLER)) , 0 , sizeof(i51SOCPOLLER) ) ;
	i51SocPoller[sockfd].socket = -1 ;
	
	if ( 0 == ret )
	{
		return iTRUE ;
	}
	
	return iFALSE ;
		
}


#ifdef i51PROFILINGS
iU32 i51AdeSocGetLive ()
{

	//	author : Jelo 
	//	since : 2011.7.11
	//	(C) PKIG Tech. Co., Ltd.
	
	//	notes : ��ȡ��ǰ��Ծ���׽���
	//	�ýӿڲ��Զ�̬�㿪��
	 
	iU32 looper = 0 ;
	iU32 lives = 0 ;

	for ( looper = 0 ; looper < i51SOCMAX_COUNT ; looper ++ )
	{	
		if ( -1 != i51SocPoller[looper].socket )
		{
			lives ++ ;
//			break ;
		}
	}

	return lives ;	
	
}
iU32 i51AdeSocGetRatio ( void ) 
{
	return 0 ;
}
#endif

iS32* i51AdeSocGetLinkLayerMode ()
{

 	//	author : Jelo
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	static iS32 mode [ 3 ] = { 0 } ;

	mode [0] = 2 ;
	mode [1] = i51SOCLLINKER_MODE_CMWAP ;
	mode [2] = i51SOCLLINKER_MODE_CMNET ;

	return mode ;

}

void i51AdeSocGetLinkLayerStatus ( iBOOL* linked , iS32* simid , iS32* mode ) 
{
	if ( iTRUE == i51SocLinkLayer.LinkExsit ) {
	//	Jelo Adding Finished
	
		*linked = iTRUE ;
		*simid = i51SocLinkLayer.i51ChooseSim + 1 ;
		*mode = i51SocLinkLayer.i51NetMode ;
		
	} else {

		*linked = iFALSE;
		*simid = 0 ;
		*mode = 0 ;
	}
	
}


#define i51AdeSocAidTableLength 5
static iU32 i51AdeSocAidTable [ i51AdeSocAidTableLength ]  = { 0 } ;
static iU32 i51AdeSocLastAddAid = 0 ;
//	gprs �Ƿ����ڼ���
iBOOL i51GprsActive = iFALSE ;

//	��·�Ƿ��Ѿ���. �Իص�Ϊ׼
static iBOOL i51LinkOpen = iFALSE ;

static iBOOL i51AdeSocAidTableEmpty () {

	//	author : Jelo
	//	since : 2011.12.15
	// 	(C) PKIG Tech. Co., Ltd.

	//	�ж�Ӧ����·���Ƿ�ա�
	
	iBOOL empty = iTRUE ;
	iU32 looper = 0 ;
	
	for ( looper = 0 ; looper < i51AdeSocAidTableLength ; looper ++ ) {

		if ( 0 != i51AdeSocAidTable[looper] ) {
			empty = iFALSE ;
		}
		
	}
	
	return empty ;
	
}

static iBOOL i51AdeSocAidTableFull () {

	//	author : Jelo
	//	since : 2011.12.15
	// 	(C) PKIG Tech. Co., Ltd.

	//	�ж�Ӧ����·���Ƿ�����
	
	iU32 looper = 0 ;

	for ( looper = 0 ; looper < i51AdeSocAidTableLength ; looper ++ ) {

		if ( 0 == i51AdeSocAidTable[looper] ) {
			return iFALSE ;
		}
		
	}

	return iTRUE ;
	
}

static iBOOL i51AdeSocAidTableAdd ( iU32 aid ) {

	//	author : Jelo
	//	since : 2011.12.15
	// 	(C) PKIG Tech. Co., Ltd.

	//	���Ӧ����·����Ŀ��
	
	iU32 looper = 0 ;

	for ( looper = 0 ; looper < i51AdeSocAidTableLength ; looper ++ ) {

		if ( 0 == i51AdeSocAidTable[looper] ) {
			i51AdeSocAidTable[looper] = aid ;
			return iTRUE ;
		}
		
	}

	return iTRUE ;
	
}

static iBOOL i51AdeSocAidTableDelete ( iU32 aid ) {

	//	author : Jelo
	//	since : 2011.12.15
	// 	(C) PKIG Tech. Co., Ltd.

	//	ɾ��Ӧ����·����Ŀ��
	
	iU32 looper = 0 ;

	for ( looper = 0 ; looper < i51AdeSocAidTableLength ; looper ++ ) {

		if ( aid == i51AdeSocAidTable[looper] ) {
			i51AdeSocAidTable[looper] = 0 ;
			return iTRUE ;
		}
		
	}
	
	return iTRUE ;
	
}

static iBOOL i51AdeSocAidTableFind ( iU32 aid ) {

	//	author : Jelo
	//	since : 2011.12.15
	// 	(C) PKIG Tech. Co., Ltd.

	//	����Ӧ����·����Ŀ��
	
	iU32 looper = 0 ;

	for ( looper = 0 ; looper < i51AdeSocAidTableLength ; looper ++ ) {

		if ( aid == i51AdeSocAidTable[looper] ) {
			return iTRUE ;
		}
		
	}
	
	return iFALSE ;
	
}

static iBOOL i51AdeSocAidTableClear () {

	//	author : Jelo
	//	since : 2011.12.15
	// 	(C) PKIG Tech. Co., Ltd.

	//	���Ӧ����·��

	i51AdeStdMemset16 ( i51AdeSocAidTable , 0 , sizeof(i51AdeSocAidTable) ) ;

	return iTRUE ;
	
}
#ifdef SPRD_6600
MMI_RESULT_E i51AdeSocHandleWapi51Msg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
	MMI_RESULT_E ret=MMI_RESULT_FALSE;

    i51AdeOsLog(0, "SOC : i51AdeSocHandleWapi51Msg , %d , %d , %x" , app_ptr , msg_id , param )  ;
	i51GprsActive = iFALSE ;
	switch(msg_id)
	{ 
		case APP_MN_ACTIVATE_PDP_CONTEXT_CNF:
		{
			MMI_GPRS_T  *signal_ptr = (MMI_GPRS_T*)param;

			if(MN_GPRS_ERR_SUCCESS == signal_ptr->result)
			{
						//	gprs����ɹ�
                i51AdeOsLog(0, "SOC : i51AdeSocHandleWapi51Msg , active gprs ok")  ;
				i51AdeSocAidTableAdd ( i51AdeSocLastAddAid ) ;
				i51AdeSocLastAddAid = 0 ;
				i51SocLinkLayer.LinkExsit = iTRUE ;
				if (i51SocLinkLayer.callback[i51SocLinkLayer.layerid -1])
					i51SocLinkLayer.callback[i51SocLinkLayer.layerid -1](iTRUE) ;
			}else{

                i51AdeOsLog(0, "SOC : i51AdeSocHandleWapi51Msg , active gprs no")  ;
				if (i51SocLinkLayer.callback[i51SocLinkLayer.layerid -1])
					i51SocLinkLayer.callback[i51SocLinkLayer.layerid -1](iFALSE) ;	
				i51AdeSocLastAddAid = 0 ;
				i51SocLinkLayer.layerid = 0 ;
			}

			ret=MMI_RESULT_TRUE;
		}
		break;
			
		case APP_MN_DEACTIVATE_PDP_CONTEXT_CNF:
			ret=MMI_RESULT_TRUE;
			break;
			
		case APP_MN_DEACTIVATE_PDP_CONTEXT_IND:
			ret=MMI_RESULT_TRUE;
			break;
		default:
			break;
		
	}
	
	return ret;	
	
}
#else

static void i51AdeSocHandleWapi51Msg ( MMIPDP_CNF_INFO_T *msg_ptr )
{

    i51AdeOsLog(0, "SOC : i51AdeSocHandleWapi51Msg , %x", msg_ptr)  ;
	i51GprsActive = iFALSE ;
	switch ( msg_ptr->msg_id)
	{

		case MMIPDP_ACTIVE_CNF :
			i51LinkOpen = iTRUE ;
		
			if(MMIPDP_RESULT_SUCC == msg_ptr->result)
			{
						//	gprs����ɹ�
                i51AdeOsLog(0, "SOC : i51AdeSocHandleWapi51Msg , active gprs ok")  ;
				i51AdeSocAidTableAdd ( i51AdeSocLastAddAid ) ;
				i51AdeSocLastAddAid = 0 ;
				i51SocLinkLayer.LinkExsit = iTRUE ;
				if (i51SocLinkLayer.callback[i51SocLinkLayer.layerid -1])
					i51SocLinkLayer.callback[i51SocLinkLayer.layerid -1](iTRUE) ;
			}else{
                i51AdeOsLog(0, "SOC : i51AdeSocHandleWapi51Msg , active gprs no  new, %d , %x" , i51SocLinkLayer.layerid , i51SocLinkLayer.callback[i51SocLinkLayer.layerid -1])  ;
				i51AdeSocLastAddAid = 0 ;
				i51SocLinkLayer.layerid = 0 ;
				if (i51SocLinkLayer.callback[i51SocLinkLayer.layerid -1])
					i51SocLinkLayer.callback[i51SocLinkLayer.layerid -1](iFALSE) ;	
				
			}

			break ;
		case MMIPDP_DEACTIVE_CNF :
			i51LinkOpen = iFALSE ;
            i51AdeOsLog(0, "SOC : i51AdeSocHandleWapi51Msg , close gprs")  ;
			break ;

	}

}
#endif



iU32 i51AdeSocCreateLinkLayer ( iS32 mode , iS32 sim ) 
{
	iU32 i = 0 ;
	iU32 ret = 0 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SOC : i51AdeSocCreateLinkLayer , S , %x , %x" , mode ,sim) ;
	#endif	

    i51AdeOsLog(0, "SOC : i51AdeSocCreateLinkLayer , %d ,%d" , mode , sim) ;
	//	��ת�����˻���ʱ����Ҫ�õ�
	i51SocLinkLayer.i51NetMode = mode ;
	//	չѶsim����0��ʼ
	i51SocLinkLayer.i51ChooseSim = i51ConvetSimCard (sim) ;

	for ( i = 0 ; i < MAX_LINKED_COUNT; i ++ ) {

		if (!i51SocLinkLayer.used[i])
			break ;

	}
	if ( i == MAX_LINKED_COUNT )
		ret = 0 ;
	else {

		i51SocLinkLayer.used[i] = iTRUE ;
		ret =  i + 1 ;

	}

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SOC : i51AdeSocCreateLinkLayer , E , ret = %x" , ret ) ;
	#endif	
	
	return ret ;
}

static void i51AdeSocLAYERCALLBACK_TIMERCB ( iU32 id )
{

	iU32 i = 0 ;

    i51AdeOsLog(0, "SOC : i51AdeSocLAYERCALLBACK_TIMERCB , %d " , id ) ;
	i51AdeOsTimerStop(id) ;
	for ( i = 0 ; i < MAX_LINKED_COUNT ; i ++ ) {

		if ( i51SocLinkLayer.timerid[i] == id )
			break ;
	}

	if ( i == MAX_LINKED_COUNT ) {

        i51AdeOsLog(0, "SOC : i51AdeSocLAYERCALLBACK_TIMERCB , id OUT " ) ;
		return ;
	}
	if ( i51SocLinkLayer.callback[i] )
		i51SocLinkLayer.callback[i]( i51SocLinkLayer.CreateLayerState[i]) ;
}
static iBOOL i51AdeSocAsynLAYERCALLBACK (  iU32 layerid )
{

	//	i51MEDIACALLBACK �ص����첽֪ͨ

	i51SocLinkLayer.timerid[layerid - 1] = i51AdeOsTimerCreate ( 0 , 1 ) ;
	if ( 0 == i51SocLinkLayer.timerid[layerid - 1]  )
	{
        i51AdeOsLog(0, "SOC : i51AdeSocAsynLAYERCALLBACK , TC " ) ;
		return iFALSE ;
	}

	if ( 0 == i51AdeOsTimerStart (i51SocLinkLayer.timerid[layerid - 1] , i51AdeSocLAYERCALLBACK_TIMERCB , 1 ) )
	{
        i51AdeOsLog(0, "SOC : i51AdeSocAsynLAYERCALLBACK , TS F" ) ;		
		return iFALSE ;
	}		

}

struct {

	//	�����������·����
	iU32 aid ;
	iU32 layerid ;
	i51LLAYERCALLBACK callback ;
	//	���״̬�Ķ�ʱ��
	iU32 timerid ;

}OpenLinkParmemer = {0};

static void i51AdeSocCheckOpenLinkState ( iU32 id )
{
	if ( !i51LinkOpen ) {

		//	�ﵽ��������
        i51AdeOsLog(0, "SOC : i51AdeSocCheckOpenLinkState , %d", id) ;
		i51AdeOsTimerStop (OpenLinkParmemer.timerid) ;
		OpenLinkParmemer.timerid = 0 ;
		i51AdeSocOpenLinkLayer ( OpenLinkParmemer.aid , OpenLinkParmemer.layerid , OpenLinkParmemer.callback ) ;
	}
}

iBOOL i51AdeSocOpenLinkLayer ( iU32 aid , iU32 layerid , i51LLAYERCALLBACK callback  ) 
{
	iU8 apn[10] = {0} ;
	iU8	ret = 0;
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , S , %x , %x , %x" , aid , layerid ,callback) ;
	#endif	

    i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , %d ,%d ,%x" , aid , layerid , callback) ;
	iASSERT ( callback ) ;

	if (layerid < 1 || layerid > MAX_LINKED_COUNT ) {
        i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , Link-Layer id overflowed" ) ;	
		return iFALSE ;
	}	
	
	i51SocLinkLayer.callback[layerid-1] = callback ;	
	//	�ж�Ӧ����·���Ƿ���
	//	�����ֱ�ӷ��� 
	if ( i51AdeSocAidTableFull () ) {
	
        i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , Link Layer Already Created (1)") ;
		i51SocLinkLayer.CreateLayerState[layerid-1] = iTRUE ;
		i51AdeSocAsynLAYERCALLBACK (layerid) ;
		
		return iTRUE ;
		
	} else if ( i51SocLinkLayer.LinkExsit ) {

        i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , Link Layer Already Created (2)") ;
		i51SocLinkLayer.CreateLayerState[layerid-1] = iTRUE ;
		i51AdeSocAsynLAYERCALLBACK (layerid) ;	
	
		//	��Aid ������·�������¼����		
		if ( 0 == i51AdeSocAidTableFind ( aid ) ) {
			i51AdeSocAidTableAdd ( aid ) ;
		}
		
		return iTRUE ;
		
	} else {

		if ( 0 != i51SocLinkLayer.layerid ) {

			//	��·���ڴ���
            i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , Link-Layer is Opening") ;
			return iFALSE ;
		}
	}


	if ( i51LinkOpen ) {

		//	��·�ص�����Ϣ��δ����������������ȴ�����
        i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , Link-Layer N Ready Close") ;
		memset ( &OpenLinkParmemer , 0 , sizeof(OpenLinkParmemer)) ;
		OpenLinkParmemer.timerid = i51AdeOsTimerCreate (iTRUE , 1) ;
		if ( OpenLinkParmemer.timerid <= 0 ) {

            i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , Link-Layer CT F") ;
			return iFALSE ;
		}
		i51AdeOsTimerStart (OpenLinkParmemer.timerid, i51AdeSocCheckOpenLinkState ,10 ) ;
		OpenLinkParmemer.aid = aid ;
		OpenLinkParmemer.layerid = layerid ;
		OpenLinkParmemer.callback = callback ;
		return iTRUE ;
	}
	i51AdeSocLastAddAid = aid ;	
	i51SocLinkLayer.layerid = layerid ;	


	if ( 1 == i51SocLinkLayer.i51NetMode ) {

		//	��¼���ڴ�����·��id

		//	�˴�������·��ʹ�÷����д��о�

		strcpy ( apn , "cmwap" ) ;
		
	} else if ( 2 == i51SocLinkLayer.i51NetMode ) {

		strcpy ( apn , "cmnet" ) ;
	} else {

		i51AdeSocLastAddAid = 0 ;
		i51SocLinkLayer.layerid = 0 ;
		
        i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , Not Support Link-Layer Mode %x" , i51SocLinkLayer.i51NetMode ) ;
		return iFALSE ;
	}

#ifdef SPRD_6600
	#ifdef MMI_DUALSIM_SUPPORT
		MNGPRS_SetPdpContextPcoEx(i51SocLinkLayer.i51ChooseSim, 1, (uint8*)"", (uint8*)"");
		#ifdef MMI_DUALMODE_ENABLE
			MNGPRS_SetAndActivePdpContextEx(i51SocLinkLayer.i51ChooseSim, apn, DOWNLOAD_E);
		#else
			MNGPRS_SetAndActivePdpContextEx(i51SocLinkLayer.i51ChooseSim, apn);
		#endif
	#else
		MNGPRS_SetPdpContextPco(1, (uint8*)"", (uint8*)"");
		#ifdef MMI_DUALMODE_ENABLE
			MNGPRS_SetAndActivePdpContext(apn, DOWNLOAD_E);
		#else
			MNGPRS_SetAndActivePdpContext(apn);
		#endif
	#endif

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , E , ret = %x" , iTRUE ) ;
	#endif	
#else
	{

	#ifdef BROWSER_SUPPORT
		iU8	setting_index = 0;
		MMIPDP_ACTIVE_INFO_T	app_info = {0};
		MMICONNECTION_LINKSETTING_DETAIL_T	*setting_item_ptr = PNULL;
		setting_index = MMIBROWSER_GetNetSettingIndex( i51SocLinkLayer.i51ChooseSim) ;
#ifdef SPRD_6800
		//		setting_item_ptr = MMICONNECTION_GetLinkSettingItemByIndex ( i51SocLinkLayer.i51ChooseSim , setting_index );      
		setting_item_ptr = MMICONNECTION_GetLinkSettingItemByIndex ( setting_index );      
#elif defined (SPRD_6530)
		setting_item_ptr = MMICONNECTION_GetLinkSettingItemByIndex ( i51SocLinkLayer.i51ChooseSim , setting_index );      
#else
		setting_item_ptr = MMICONNECTION_GetLinkSettingItemByIndex ( setting_index );      
#endif

		if ( NULL != setting_item_ptr )
		{    
		    app_info.app_handler = MMI_MODULE_i51;
		    app_info.auth_type = setting_item_ptr->auth_type;
		    //app_info.apn_ptr = (char*)setting_item_ptr->apn;
		    app_info.apn_ptr = apn ;
		    app_info.user_name_ptr = (char*)setting_item_ptr->username;
		    app_info.psw_ptr = (char*)setting_item_ptr->password;
		    app_info.dual_sys = i51SocLinkLayer.i51ChooseSim;
		    app_info.priority = 3;
		//app_info.ps_service_rat = MN_UNSPECIFIED;
		app_info.handle_msg_callback = i51AdeSocHandleWapi51Msg;

		ret = MNGPRS_ReadStorageExtReqQosEx(i51SocLinkLayer.i51ChooseSim,MN_GPRS_STORAGE_ALL,IM_E,&app_info.qos_info) ;
		if ( ERR_MNGPRS_NO_ERR != ret )
		{
            i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , Big Error Here (1)") ; 	 
			return iFALSE ;
		}

		//et = MMIAPIPDP_Active(&app_info);
		if ( !MMIAPIPDP_Active(&app_info) )
		{
            i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , Big Error Here (2)") ; 	 
			return iFALSE ;
		}			 
			 
		}
	#else
		iU8	setting_index = 0;
		MMIPDP_ACTIVE_INFO_T	app_info = {0};

		app_info.app_handler = MMI_MODULE_i51;
		app_info.auth_type = 0;
		app_info.apn_ptr = apn ;
		app_info.user_name_ptr = (char*)"";
		app_info.psw_ptr = (char*)"";
		app_info.dual_sys = i51SocLinkLayer.i51ChooseSim;
		app_info.priority = 3;
		app_info.handle_msg_callback = i51AdeSocHandleWapi51Msg;

		ret = MNGPRS_ReadStorageExtReqQosEx(i51SocLinkLayer.i51ChooseSim,MN_GPRS_STORAGE_ALL,IM_E,&app_info.qos_info) ;
		if ( ERR_MNGPRS_NO_ERR != ret )
		{
            i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , Big Error Here (1)") ; 	 
		    return iFALSE ;
		}

		//et = MMIAPIPDP_Active(&app_info);
		if ( !MMIAPIPDP_Active(&app_info) )
		{
            i51AdeOsLog(0, "SOC : i51AdeSocOpenLinkLayer , Big Error Here (2)") ; 	 
		    return iFALSE ;
		}			 
	#endif

	}
	
#endif
	i51GprsActive = iTRUE ;
	return iTRUE ;

}

iBOOL i51AdeSocCloseLinkLayer ( iU32 aid , iU32 layerid )
{

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SOC : i51AdeSocCloseLinkLayer , S , %x , %x" , aid , layerid ) ;
	#endif	

    i51AdeOsLog(0, "SOC : i51AdeSocCloseLinkLayer , %d ,%d" , aid , layerid) ;

	//	�����·û�д���������ֱ�ӷ���iTRUE��
	//	��Ӱ���ϲ��߼���

	if ( OpenLinkParmemer.aid == aid  ) {

		if ( OpenLinkParmemer.timerid ) {

            i51AdeOsLog(0, "SOC : i51AdeSocCloseLinkLayer , close timer") ;
			i51AdeOsTimerStop (OpenLinkParmemer.timerid) ;
			OpenLinkParmemer.timerid = 0 ;
		}
	}
	if ( i51SocLinkLayer.layerid && !i51SocLinkLayer.LinkExsit )
	{
		// 	��·�����ڣ������ڴ����Ĺ������˳�������G��־����ʧ
		//	ͬ��ҲҪ���ر���·�Ĵ���
		goto CLOSE_LINK ;
	}
	if ( iFALSE == i51SocLinkLayer.LinkExsit ) {
		i51GprsActive = iFALSE ;
		return iTRUE ;
	} 
	
	
	i51AdeSocAidTableDelete ( aid ) ;
	//	�����·���������Ӧ��������·�㣬�򲻹ر���·��
	if ( iFALSE == i51AdeSocAidTableEmpty () ) {
		return iTRUE ; 
	}

CLOSE_LINK:
    i51AdeOsLog(0, "SOC : i51AdeSocCloseLinkLayer , ready close Link") ;

	if ( DnsCallback ) {

		//	����·�ر�ʱ�����������ӿڻص�û�й�������Ҫ������Ϊ0����ֹ����
		DnsCallback = 0 ;
	}
	
	//	�����ر���·
	i51GprsActive = iFALSE ;
	i51SocLinkLayer.LinkExsit = iFALSE ;
	i51SocLinkLayer.layerid= 0 ;
	
#ifdef SPRD_6600	
	#ifdef MMI_DUALSIM_SUPPORT
		MNGPRS_ResetAndDeactivePdpContextEx(i51SocLinkLayer.i51ChooseSim);
	#else
		MNGPRS_ResetAndDeactivePdpContext();
	#endif
#else
	MMIAPIPDP_Deactive(MMI_MODULE_i51);
#endif
	return iTRUE;	
}

iBOOL i51AdeSocReleaseLinkLayer ( iU32 layerid ) 
{
	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SOC : i51AdeSocReleaseLinkLayer , S , %x" , layerid ) ;
	#endif	

    i51AdeOsLog(0, "SOC : i51AdeSocReleaseLinkLayer , %d" ,layerid) ;
	
	if (  layerid < 1 || layerid > MAX_LINKED_COUNT  )
	{
        i51AdeOsLog(0, "SOC : i51AdeSocReleaseLinkLayer , BE " ) ;
		return iFALSE ;
	}
	i51SocLinkLayer.callback[layerid-1] = 0 ;
	i51SocLinkLayer.used[layerid-1] = iFALSE ;
	i51SocLinkLayer.timerid[layerid - 1] = 0 ;

	#ifdef i51ADAPTER_DEBUG
        i51AdeOsLog(0, "SOC : i51AdeSocReleaseLinkLayer , E , ret = %x" , iTRUE ) ;
	#endif		
	return iTRUE ;
}

static void i51AdeCreateCB ( iU32 id )
{
	i51AdeOsTimerStop ( i51AdeCreateTimer ) ;
	i51AdeCreateTimer = 0 ;
	if ( i51AdeCallback ) {
		
		i51AdeCallback (1) ;
	}
}
iBOOL i51AdeSocCreate ( i51ADECALLBACK callback ) 
{

 	//	author : Jelo
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.

	i51AdeCallback = callback ;

	//MMK_CreateTimerCallback ( 10 , i51AdeSocCreateCallBack , iTRUE , iFALSE ) ;

	//	��ʱ��������ʽ����Ҫʹ��timer
//	callback (1) ;
       i51AdeCreateTimer = i51AdeOsTimerCreate (0,1) ;
	if ( i51AdeCreateTimer == 0 ) {

		return iFALSE ;
	}
       i51AdeOsTimerStart(i51AdeCreateTimer,i51AdeCreateCB,10);
	return iTRUE ;

}

iBOOL i51AdeSocRelease ( void ) 
{

 	//	author : Jelo
	//	since : 2011.8.18
	// 	(C) 2011 , PKIG Tech. Co., Ltd.
	
	return iTRUE ;

}

