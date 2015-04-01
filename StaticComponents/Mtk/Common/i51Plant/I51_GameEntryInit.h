//game entry init

#if !defined(MMI_BOOTUP_INIT_ADP)&&!defined(VAPP_BOOTUP_INIT_ADP)

    	{
		extern void i51Boot( void );	
		i51Boot() ;
		}
	#ifdef 	__I51_Portal__		//51PK��Ϸ
		{
		extern void I51_mmi_menu_init (void);
		I51_mmi_menu_init();
		}
	#endif
		
	#ifdef	__I51_TQ__		// ����Ԥ��
		{
		extern void weather_I51_entry_init (void);
		weather_I51_entry_init();
		}
	#endif
		
	#ifdef	__I51_SavageAdventure__		// Ұ��������
		{
		extern void sa_game_entry_init_2 (void);
		sa_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_BM__		// ��ʯ����
		{extern void bm_game_entry_init_2 (void);
		bm_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_DM__		// ���������ռ�
		{
		extern void dm_game_entry_init_2 (void);
		dm_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_ST__		// ��ͷ�ٵ�
		{
		extern void streetboy_game_entry_init_2 (void);
		streetboy_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_Loli__		// Ӫ��СLOLI
		{
		extern void sl_game_entry_init_2 (void);
		sl_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_Mjump__		// ������
		{
		extern void Mjump_game_entry_init_2 (void);
		Mjump_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_X_Space__	// ��������
		{
		extern void X_Space_game_entry_init_2 (void);
		X_Space_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_DontRun__		// ������
		{
		extern void DR_game_entry_init_2 (void);
		DR_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_DS__		// �����㳡
		{
		extern void Ds_game_entry_init_2 (void);
		Ds_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_Convoy__		// �������㵺
		{
		extern void convoy_game_entry_init_2 (void);
		convoy_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_HundredStorey__		// ���ַɽ�
		{
		extern void hs_game_entry_init_2 (void);
		hs_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_fighter__		// õ�������
		{
		extern void fighter_game_entry_init_2 (void);
		fighter_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_RaceCar__		// �ڵ��ɳ�
		{
		extern void rc_game_entry_init_2 (void);
		rc_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_VexedJewels__		// ������ʯ
		{
		extern void VJ_game_entry_init_2 (void);
		VJ_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_CutFruit__		// ˮ������
		{
		extern void cf_game_entry_init_2 (void);
		cf_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_JigsawPuzzle__		// ����ƴͼ
		{
		extern void JigsawPuzzle_game_entry_init_2 (void);
		JigsawPuzzle_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_Snidget__	// ���ַ���
		{
		extern void nin_game_entry_init_2 (void);
		nin_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_IP__		// �����Ծ�
		{
		extern void IdiomPuzzle_game_entry_init_2 (void);
		IdiomPuzzle_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_DDZ__	// ������
		{
		extern void DDZ_game_entry_init_2 (void);
		DDZ_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_FruitShot__		// ˮ�����
		{
		extern void FruitShot_game_entry_init_2 (void);
		FruitShot_game_entry_init_2();
		}
	#endif
		
	#ifdef	__I51_FishingMaster__		// �������
		{
		extern void CFM_game_entry_init_2 (void);
		CFM_game_entry_init_2();
		}
	#endif
	
	#ifdef	__I51_AngryBird__ 	// ��ŭ��С��
		{
		
		extern void ANGRYBIRD_game_entry_init_2 (void);
		ANGRYBIRD_game_entry_init_2();	
		
		}
	#endif
	
	#ifdef	__I51_GlassDestroyer__	// �鲣��
		{
		extern void GD_game_entry_init_2 (void);
		GD_game_entry_init_2();	
		}
	#endif
	
	#ifdef	__I51_PullSkirt__	// ��ȹ��
		{
		extern void PS_game_entry_init_2 (void);
		PS_game_entry_init_2();	
		}
	#endif

	#ifdef	__I51_TOM__	// TOM
		{
		extern void TOM_game_entry_init_2 (void);
		TOM_game_entry_init_2();	
		}
	#endif

	#ifdef	__I51_Ninjump__		// ������Ծ
		{
		extern void Ninjump_game_entry_init_2 (void);
		Ninjump_game_entry_init_2();	
		}	
	#endif

	#ifdef	__I51_ZapBird__		// ������
		extern void ZapBird_game_entry_init_2 (void);
		ZapBird_game_entry_init_2();	
	#endif

	#ifdef	__I51_PlantsVsZombies__		// ֲ���ս��ʬ
		extern void PlantsVsZombies_game_entry_init_2 (void);
		PlantsVsZombies_game_entry_init_2();	
	#endif	

    #ifdef	__I51_VXin__		//V��
		extern void VXin_game_entry_init_2 (void);
		VXin_game_entry_init_2();	
    #endif

    #ifdef	__I51_RipClothes__		//˺�·�
		extern void RipClothes_game_entry_init_2 (void);
		RipClothes_game_entry_init_2();	
    #endif

    #ifdef	__I51_AppStore__		//Ӧ���̵�
		extern void AppStore_game_entry_init_2 (void);
		AppStore_game_entry_init_2();	
    #endif
    
    #ifdef	__I51_wheresMywater__		 //����ϴ��
		extern void wheresMywater_game_entry_init_2 (void);
		wheresMywater_game_entry_init_2();	
    #endif
    
    #ifdef	__I51_MusicJam__        //���ִ���
		extern void MusicJam_game_entry_init_2 (void);
		MusicJam_game_entry_init_2();	
    #endif
    
    #ifdef	__I51_AuPlanet__        //��������
		extern void AuPlanet_game_entry_init_2 (void);
		AuPlanet_game_entry_init_2();	
    #endif

#elif defined(VAPP_BOOTUP_INIT_ADP)
	
	#ifdef __I51__
		VAPP_BOOTUP_INIT_ADP(i51Boot)
	#endif

#elif defined(MMI_BOOTUP_INIT_ADP)
	
	#ifdef __I51__
		MMI_BOOTUP_INIT_ADP(i51Boot)
	#endif

#endif

