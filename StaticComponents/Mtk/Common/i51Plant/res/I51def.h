#include "PixtelDataTypes.h"
typedef enum 
{//String Ids
	#ifdef  __I51__
	MENU_ID_I51_TEXT_INNVRAM = APP_I51_BASE + 1,
	#endif
	
	#ifdef  __I51_Portal__		//51PK��Ϸ
		MENU_ID_I51_TEXT_1,
		MENU_ID_I51_TEXT_2,
		MENU_ID_I51_TEXT_3,
		MENU_ID_I51_TEXT_4,
		MENU_ID_I51_TEXT_5,
		MENU_ID_I51_TEXT_6,
		MENU_ID_I51_JAVA_TEXT_1,
		MENU_ID_I51_JAVA_TEXT_2,
		MENU_ID_I51_JAVA_TEXT_3,
		MENU_ID_I51_JAVA_TEXT_4,
		MENU_ID_I51_JAVA_TEXT_5,
		MENU_ID_I51_JAVA_TEXT_6,
	#endif
	
	#ifdef	__I51_TQ__		// ����Ԥ��
		STR_ID_I51_TQ,
	#endif
	
	#ifdef	__I51_SavageAdventure__		// Ұ��������
		STR_ID_I51_SA,
	#endif
	
	#ifdef	__I51_BM__		// ��ʯ����
		STR_ID_I51_BM,
	#endif
	
	#ifdef	__I51_DM__		// ���������ռ�
		STR_ID_I51_DM,
	#endif
	
	#ifdef	__I51_ST__		// ��ͷ�ٵ�
		STR_ID_I51_ST,
	#endif
	
	#ifdef	__I51_Loli__		// Ӫ��СLOLI
		STR_ID_I51_SL,
	#endif
	
	#ifdef	__I51_Mjump__		// ������
		STR_ID_I51_MJ,
	#endif
	
	#ifdef	__I51_X_Space__	// ��������
		STR_ID_I51_SPACE,
	#endif
	
	#ifdef	__I51_DontRun__		// ������
		STR_ID_I51_DR,
	#endif
	
	#ifdef	__I51_DS__		// �����㳡
		STR_ID_I51_DS,
	#endif
	
	#ifdef	__I51_Convoy__		// �������㵺
		STR_ID_I51_CY,
	#endif
	
	#ifdef	__I51_HundredStorey__		// ���ַɽ�
		STR_ID_I51_HS,
	#endif
	
	#ifdef	__I51_fighter__		// õ�������
		STR_ID_I51_FT,
	#endif
	
	#ifdef	__I51_RaceCar__		// �ڵ��ɳ�
		STR_ID_I51_RC,
	#endif
	
	#ifdef	__I51_VexedJewels__		// ������ʯ
		STR_ID_I51_VJ,
	#endif
	
	#ifdef	__I51_CutFruit__		// ˮ������
		STR_ID_I51_CF,
	#endif
	
	#ifdef	__I51_JigsawPuzzle__		// ����ƴͼ
		STR_ID_I51_JP,
	#endif
	
	#ifdef	__I51_Snidget__	// ���ַ���
		STR_ID_I51_NIN,
	#endif
	
	#ifdef	__I51_IP__		// �����Ծ�
		STR_ID_I51_IP,
	#endif
	
	#ifdef	__I51_DDZ__	// ������
		STR_ID_I51_DDZ,
	#endif
	
	#ifdef	__I51_FruitShot__		// ˮ�����
		STR_ID_I51_FS,
	#endif
	
	#ifdef	__I51_FishingMaster__	// �������
		STR_ID_I51_CFM,
	#endif

	#ifdef	__I51_AngryBird__ 	// ��ŭ��С��
		STR_ID_I51_ANGRYBIRD,
	#endif
	
	#ifdef	__I51_GlassDestroyer__	// �鲣��
		STR_ID_I51_GD,
	#endif
	
	#ifdef	__I51_PullSkirt__	// ��ȹ��
		STR_ID_I51_PS,
	#endif

	#ifdef	__I51_TOM__	// TOM
		STR_ID_I51_TOM,
	#endif

	#ifdef	__I51_Ninjump__		// ������Ծ
		STR_ID_I51_NINJUMP,
	#endif

	#ifdef	__I51_ZapBird__		// ������
		STR_ID_I51_ZAPBIRD,
	#endif

	#ifdef	__I51_PlantsVsZombies__		// ֲ���ս��ʬ
		STR_ID_I51_PLANTSVSZOMBIES,
	#endif

    #ifdef	__I51_VXin__		//V��
		STR_ID_I51_VXin,
    #endif

    #ifdef	__I51_RipClothes__		//˺�·�
		STR_ID_I51_RipClothes,
    #endif
    
    #ifdef	__I51_AppStore__		 //Ӧ���̵�
		STR_ID_I51_AppStore,
    #endif
    
    #ifdef	__I51_wheresMywater__		 //����ϴ��
        STR_ID_I51_wheresMywater,
    #endif
    
    #ifdef	__I51_MusicJam__        //���ִ���
        STR_ID_I51_MusicJam,
    #endif
    
    #ifdef	__I51_AuPlanet__        //��������
        STR_ID_I51_AuPlanet,
    #endif
    
	STR_I51_END
}STR_I51_MENU_LIST;
   
typedef enum 
{//Image Ids
	#ifdef  __I51__
		IMG_I51_MAIN__INNVRAM = APP_I51_BASE + 1,
	#endif
		
	#ifdef  __I51_Portal__		//51PK��Ϸ
		IMG_I51_Portal_ICON,
	#endif
	
	#ifdef	__I51_TQ__		// ����Ԥ��
		IMG_ID_I51_TQ,
	#endif
	
	#ifdef	__I51_SavageAdventure__		// Ұ��������
		IMG_ID_I51_SA,
	#endif
	
	#ifdef	__I51_BM__		// ��ʯ����
		IMG_ID_I51_BM,
	#endif
	
	#ifdef	__I51_DM__		// ���������ռ�
		IMG_ID_I51_DM,
	#endif
	
	#ifdef	__I51_ST__		// ��ͷ�ٵ�
		IMG_ID_I51_ST,
	#endif
	
	#ifdef	__I51_Loli__		// Ӫ��СLOLI
		IMG_ID_I51_SL,
	#endif
	
	#ifdef	__I51_Mjump__		// ������
		IMG_ID_I51_MJ,
	#endif
	
	#ifdef	__I51_X_Space__	// ��������
		IMG_ID_I51_SPACE,
	#endif
	
	#ifdef	__I51_DontRun__		// ������
		IMG_ID_I51_DR,
	#endif
	
	#ifdef	__I51_DS__		// �����㳡
		IMG_ID_I51_DS,
	#endif
	
	#ifdef	__I51_Convoy__		// �������㵺
		IMG_ID_I51_CY,
	#endif
	
	#ifdef	__I51_HundredStorey__		// ���ַɽ�
		IMG_ID_I51_HS,
	#endif
	
	#ifdef	__I51_fighter__		// õ�������
		IMG_ID_I51_FT,
	#endif
	
	#ifdef	__I51_RaceCar__		// �ڵ��ɳ�
		IMG_ID_I51_RC,
	#endif
	
	#ifdef	__I51_VexedJewels__		// ������ʯ
		IMG_ID_I51_VJ,
	#endif
	
	#ifdef	__I51_CutFruit__		// ˮ������
		IMG_ID_I51_CF,
	#endif
	
	#ifdef	__I51_JigsawPuzzle__		// ����ƴͼ
		IMG_ID_I51_JP,
	#endif
	
	#ifdef	__I51_Snidget__	// ���ַ���
		IMG_ID_I51_NIN,
	#endif
	
	#ifdef	__I51_IP__		// �����Ծ�
		IMG_ID_I51_IP,
	#endif
	
	#ifdef	__I51_DDZ__	// ������
		IMG_ID_I51_DDZ,
	#endif
	
	#ifdef	__I51_FruitShot__		// ˮ�����
		IMG_ID_I51_FS,
	#endif
	
	#ifdef	__I51_FishingMaster__	// �������
		IMG_ID_I51_CFM,
	#endif

	#ifdef	__I51_AngryBird__ 	// ��ŭ��С��
		IMG_ID_I51_ANGRYBIRD,
	#endif
	
	#ifdef	__I51_GlassDestroyer__	// �鲣��
		IMG_ID_I51_GD,
	#endif
	
	#ifdef	__I51_PullSkirt__	// ��ȹ��
		IMG_ID_I51_PS,
	#endif

	#ifdef	__I51_TOM__	// TOM
		IMG_ID_I51_TOM,
	#endif

	#ifdef	__I51_Ninjump__		// ������Ծ
		IMG_ID_I51_NINJUMP,
	#endif

	#ifdef	__I51_ZapBird__		// ������
		IMG_ID_I51_ZAPBIRD,
	#endif

	#ifdef	__I51_PlantsVsZombies__		// ֲ���ս��ʬ
		IMG_ID_I51_PLANTSVSZOMBIES,
	#endif
    
    #ifdef	__I51_VXin__		//V��
		IMG_ID_I51_VXin,
    #endif

    #ifdef	__I51_RipClothes__		//˺�·�
		IMG_ID_I51_RipClothes,
    #endif
    
    #ifdef	__I51_AppStore__		 //Ӧ���̵�
		IMG_ID_I51_AppStore,
    #endif

    #ifdef	__I51_wheresMywater__		 //����ϴ��
        IMG_ID_I51_wheresMywater,
    #endif
    
    #ifdef	__I51_MusicJam__        //���ִ���
        IMG_ID_I51_MusicJam,
    #endif
    
    #ifdef	__I51_AuPlanet__        //��������
        IMG_ID_I51_AuPlanet,
    #endif
    
	IMG_I51_END
}IMG_I51_MENU_LIST;

