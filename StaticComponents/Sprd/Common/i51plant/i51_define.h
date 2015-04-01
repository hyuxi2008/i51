
#ifndef __I51_DEFINE_H__
#define __I51_DEFINE_H__

#ifndef __I51_TINY__
//	���������潫�˴���
//#define __I51_TINY__
#endif



//����ƽ̨���󿪺�
//#define SPRD_6800
//#define SPRD_6600
//#define SPRD_6620
#define SPRD_6530

//	���MMIAPICC_MakeCall������6�����������ǽ�I51_MAKECALL_PARA_6 �ĺ�򿪣�
//�Դ����ƣ�7�������� I51_MAKECALL_PARA_7��


//#define I51_MAKECALL_PARA_6 
//#define I51_MAKECALL_PARA_7
#define I51_MAKECALL_PARA_8

//Ӧ�������
#define __I51_Explorer__

//i51��Ϸ����
#define __I51_Portal__

//ˮ������
//#define __I51_CutFruit__

//õ�������
//#define __I51_fighter__		
//�������㵺
//#define __I51_Convoy__

//���ַ���
//#define __I51_Snidget__

//ˮ�����
//#define __I51_FruitShot__

//Ұ��������
//#define __I51_SavageAdventure__

//������ʯ
//#define __I51_VexedJewels__

//��������
//#define __I51_X_Space__

//������
//#define __I51_DontRun__

//������
//#define __I51_Mjump__

//������
//#define __I51_DDZ__

//���ַɽ�
//#define __I51_HundredStorey__

//�ڵ��ɳ�
//#define __I51_RaceCar__

//�������
//#define __I51_FishingMaster__

//����ƴͼ
//#define __I51_JigsawPuzzle__

//��ŭ��С��
//#define __I51_AngryBird__

 //����
//#define __I51_GlassDestroyer__

//������Ծ
//#define __I51_NinJump__

//ֲ���ս��ʬ
//#define __I51_PlantsVsZombies__

//΢��
//#define __I51_VXin__

//˺�·�
//#define __I51_RipClothes__



//�������ϵĺ���Ҫ����ʵ�ʵ�����򿪣�����ĺ���ֲ��Ա����Ҫ�ܡ� 


#if defined (MAINLCD_DEV_SIZE_128X128)
#define __I51_APP_128X128__
#elif  defined (MAINLCD_DEV_SIZE_176X220)
#define __I51_APP_176X220__
#elif  defined (MAINLCD_DEV_SIZE_220X176)
#define __I51_APP_220X176__
#elif  defined (MAINLCD_DEV_SIZE_240X320)
#define __I51_APP_240X320__
#elif  defined (MAINLCD_DEV_SIZE_320X240)
#define __I51_APP_320X240__
#elif  defined (MAINLCD_DEV_SIZE_240X400)
#define __I51_APP_240X400__
#elif  defined (MAINLCD_DEV_SIZE_320X400)
#define __I51_APP_320X400__
#elif  defined (MAINLCD_DEV_SIZE_272X480)
#define __I51_APP_272X480__
#elif  defined (MAINLCD_DEV_SIZE_320X480)
#define __I51_APP_320X480__
#elif  defined (MAINLCD_DEV_SIZE_480X320)
#define __I51_APP_480X320__
#endif

#if defined (__I51_TINY__)
//	ĿǰΪֹ����涼���þ����Ľ���
#define __I51_SIMPLE_INSTALL__
#endif

#ifndef SPRD_6530
#define __I51_BIG_ENDIAN__
#endif


#define __I51_CPU_ARM__



#endif

