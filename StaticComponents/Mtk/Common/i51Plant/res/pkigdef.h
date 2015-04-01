#include "PixtelDataTypes.h"
typedef enum 
{//String Ids
	#ifdef  __PKIG__		//51PK��Ϸ
		MENU_ID_PKIG_TEXT_1 = LL_BASE + 1,
		MENU_ID_PKIG_TEXT_2,
		MENU_ID_PKIG_TEXT_3,
		MENU_ID_PKIG_TEXT_4,
		MENU_ID_PKIG_TEXT_5,
		MENU_ID_PKIG_TEXT_6,
		MENU_ID_PKIG_JAVA_TEXT_1,
		MENU_ID_PKIG_JAVA_TEXT_2,
		MENU_ID_PKIG_JAVA_TEXT_3,
		MENU_ID_PKIG_JAVA_TEXT_4,
		MENU_ID_PKIG_JAVA_TEXT_5,
		MENU_ID_PKIG_JAVA_TEXT_6,
	#endif
	
	#ifdef	__PKIG_TQ__		// ����Ԥ��
		STR_ID_PK_TQ,
	#endif
	
	#ifdef	__PKIG_SA__		// Ұ��������
		STR_ID_PK_SA,
	#endif
	
	#ifdef	__PKIG_BM__		// ��ʯ����
		STR_ID_PK_BM,
	#endif
	
	#ifdef	__PKIG_DM__		// ���������ռ�
		STR_ID_PK_DM,
	#endif
	
	#ifdef	__PKIG_ST__		// ��ͷ�ٵ�
		STR_ID_PK_ST,
	#endif
	
	#ifdef	__PKIG_SL__		// Ӫ��СLOLI
		STR_ID_PK_SL,
	#endif
	
	#ifdef	__PKIG_MJ__		// ������
		STR_ID_PK_MJ,
	#endif
	
	#ifdef	__PKIG_SPACE__	// ��������
		STR_ID_PK_SPACE,
	#endif
	
	#ifdef	__PKIG_DR__		// ������
		STR_ID_PK_DR,
	#endif
	
	#ifdef	__PKIG_DS__		// �����㳡
		STR_ID_PK_DS,
	#endif
	
	#ifdef	__PKIG_CY__		// �������㵺
		STR_ID_PK_CY,
	#endif
	
	#ifdef	__PKIG_HS__		// ���ַɽ�
		STR_ID_PK_HS,
	#endif
	
	#ifdef	__PKIG_FT__		// õ�������
		STR_ID_PK_FT,
	#endif
	
	#ifdef	__PKIG_RC__		// �ڵ��ɳ�
		STR_ID_PK_RC,
	#endif
	
	#ifdef	__PKIG_VJ__		// ������ʯ
		STR_ID_PK_VJ,
	#endif
	
	#ifdef	__PKIG_CF__		// ˮ������
		STR_ID_PK_CF,
	#endif
	
	#ifdef	__PKIG_JP__		// ����ƴͼ
		STR_ID_PK_JP,
	#endif
	
	#ifdef	__PKIG_NIN__	// ���ַ���
		STR_ID_PK_NIN,
	#endif
	
	#ifdef	__PKIG_IP__		// �����Ծ�
		STR_ID_PK_IP,
	#endif
	
	#ifdef	__PKIG_DDZ__	// ������
		STR_ID_PK_DDZ,
	#endif
	
	#ifdef	__PKIG_FS__		// ˮ�����
		STR_ID_PK_FS,
	#endif
	
	#ifdef	__PKIG_CFM__	// �������
		STR_ID_PK_CFM,
	#endif
	
	STR_PKIG_END
}STR_PKIG_MENU_LIST;
   
typedef enum 
{//Image Ids
	#ifdef  __PKIG__		//51PK��Ϸ
		IMG_PKIG_MAIN_ICON = LL_BASE + 1,
	#endif
	
	#ifdef	__PKIG_TQ__		// ����Ԥ��
		IMG_ID_PK_TQ,
	#endif
	
	#ifdef	__PKIG_SA__		// Ұ��������
		IMG_ID_PK_SA,
	#endif
	
	#ifdef	__PKIG_BM__		// ��ʯ����
		IMG_ID_PK_BM,
	#endif
	
	#ifdef	__PKIG_DM__		// ���������ռ�
		IMG_ID_PK_DM,
	#endif
	
	#ifdef	__PKIG_ST__		// ��ͷ�ٵ�
		IMG_ID_PK_ST,
	#endif
	
	#ifdef	__PKIG_SL__		// Ӫ��СLOLI
		IMG_ID_PK_SL,
	#endif
	
	#ifdef	__PKIG_MJ__		// ������
		IMG_ID_PK_MJ,
	#endif
	
	#ifdef	__PKIG_SPACE__	// ��������
		IMG_ID_PK_SPACE,
	#endif
	
	#ifdef	__PKIG_DR__		// ������
		IMG_ID_PK_DR,
	#endif
	
	#ifdef	__PKIG_DS__		// �����㳡
		IMG_ID_PK_DS,
	#endif
	
	#ifdef	__PKIG_CY__		// �������㵺
		IMG_ID_PK_CY,
	#endif
	
	#ifdef	__PKIG_HS__		// ���ַɽ�
		IMG_ID_PK_HS,
	#endif
	
	#ifdef	__PKIG_FT__		// õ�������
		IMG_ID_PK_FT,
	#endif
	
	#ifdef	__PKIG_RC__		// �ڵ��ɳ�
		IMG_ID_PK_RC,
	#endif
	
	#ifdef	__PKIG_VJ__		// ������ʯ
		IMG_ID_PK_VJ,
	#endif
	
	#ifdef	__PKIG_CF__		// ˮ������
		IMG_ID_PK_CF,
	#endif
	
	#ifdef	__PKIG_JP__		// ����ƴͼ
		IMG_ID_PK_JP,
	#endif
	
	#ifdef	__PKIG_NIN__	// ���ַ���
		IMG_ID_PK_NIN,
	#endif
	
	#ifdef	__PKIG_IP__		// �����Ծ�
		IMG_ID_PK_IP,
	#endif
	
	#ifdef	__PKIG_DDZ__	// ������
		IMG_ID_PK_DDZ,
	#endif
	
	#ifdef	__PKIG_FS__		// ˮ�����
		IMG_ID_PK_FS,
	#endif
	
	#ifdef	__PKIG_CFM__	// �������
		IMG_ID_PK_CFM,
	#endif
	
	IMG_PKIG_END
}IMG_PKIG_MENU_LIST;

