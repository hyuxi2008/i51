
#ifndef THE_TReport_H
#define THE_TReport_H

#include "i51.h"
typedef enum {
	
	i51_CASE_ADAPTER = 0 ,
	i51_CASE_KERNEL,
	i51_CASE_BENCHMARK ,
	i51_CASE_INTER ,
	i51_CASE_ENVIR ,
} CASETYPE ;

typedef struct {
	
	iU16 type ;
	//	iTRUE��iFALSE
	iU16 results ;

} CASE ;

#if 0
#ifdef WIN32
#pragma pack(1)
#else
__align(1)
#endif
#endif

typedef struct {
	
	//	author : jelo
	//	since : 2012.4.10
	//	PKIG Tech. Co., Ltd.
	
	//	"TR"
       iCHAR Magic [2] ;
	iU32 Version ;
	iU32 TotallCases ;
	iU32 CaseEntry ;

	struct {
		
		//	��������ʱ��

		iU16 Year ;
		iU16 Month ;
		iU16 Day ;
		iU16 Hour ;
		iU16 Minute ;
		iU16 Second ;

	} CreateTime ;

	struct {

		//	�ֻ���Ϣ

		//	��С��
		iS8 Endian ;
		//	Kernel�汾��
		iU32 KernelVersion ;
		//	Adapter�汾��
		iU32 AdapterVersion ;
		//	��Ŀid
		iU32 Projectid ;
              //  �ͻ� id
              iU32 CustomerId;
		//	ƽ̨ʶ��
		iCHAR Platform[64] ;
              //  �ֱ���
              iCHAR Resolution[10];
		//	SN
		iU8 Sn[16] ;
		//	imei
		iCHAR IMEI[32] ;
		//	��ֲ����
		iU8 BatchCode[16] ;
		//	ʣ����̿ռ�
		iU32 DiskFreeSpace ;
		//	ʣ���ڴ�ռ�	
		iU32 MemoryAvalible ;
	
	} PhoneInfo ;

	struct {

		//	����SIM���ĸ�����Ϣ
	
		//	imsi
		iCHAR IMSI[32] ;
		//	�������ĺ�
		iCHAR IMSC[32] ;
		//	��վid
		iU32 bid ;
		//	��վ����id
		iU32 zid ;

	} Sim ;

	struct {

		//	���ŷ����ܴ���
		iU32 sendsmstotall ;
		//	���ͳɹ��Ĵ���
		iU32 succsmstotall ;
		//	���Ž�������
		iU32 hookedtotall ;
		//	�������ش���
		iU32 Intercepted ;
		//	�������ز��ظ��Ĵ���
		iU32 Interceptedsend ;

	} KitServerInfo ;

	struct {
		
		//	��������ָ��
		iU16 Integer ;
		iU16 Float ;
		iU16 FSO ;
		iU16 FsSeek ;
		iU16 FsWrite ;
		iU16 FsRead ;
		iU16 Memory ;
		iU16 Graphics ;
		iU16 Image ;
		iU16 Font ;
		iU16 Sound ;
		iU16 UPS ;
		iU16 TotallCost ;

	} Benchmark ;
    
    struct {
        
        //Dotpay �汾
        iU32 DotpayVersion ;
        //����DotPay��ʼ����ʱ��
        iU32 InitTime1;
        iU32 InitTime2;
    }DotpayInfo;

} TREPORT ;

//#pragma pack()

iBOOL i51KitTReportCreateHeader ( iFILE file );
iBOOL i51KitTReportAddCase ( iFILE file , iU32 casetype , iBOOL results );
iBOOL i51KitTReportReadHeader ( iFILE file , TREPORT* report );
void i51UpdataReport(iU8 para,TREPORT report);
#endif
