//Menu ID add to 'Res_MainMenu.c' as Submenus of MAIN_MENU_FUNANDGAMES_MENUID

	#ifdef  __PKIG__		//51PK游戏
		MENU_ID_FNG_PK_1,
	#endif
	
	#ifdef	__PKIG_TQ__		// 天气预报
		MENU_ID_PK_TQ,
	#endif
	
	#ifdef	__PKIG_SA__		// 野人逃生记
		MENU_ID_PK_SA,
	#endif
	
	#ifdef	__PKIG_BM__		// 宝石迷情
		MENU_ID_PK_BM,
	#endif
	
	#ifdef	__PKIG_DM__		// 金字塔历险记
		MENU_ID_PK_DM,
	#endif
	
	#ifdef	__PKIG_ST__		// 街头速递
		MENU_ID_PK_ST,
	#endif
	
	#ifdef	__PKIG_SL__		// 营救小LOLI
		MENU_ID_PK_SL,
	#endif
	
	#ifdef	__PKIG_MJ__		// 跳跳堂
		MENU_ID_PK_MJ,
	#endif
	
	#ifdef	__PKIG_SPACE__	// 完美真相
		MENU_ID_PK_SPACE,
	#endif
	
	#ifdef	__PKIG_DR__		// 虫虫别跑
		MENU_ID_PK_DR,
	#endif
	
	#ifdef	__PKIG_DS__		// 豆豆广场
		MENU_ID_PK_DS,
	#endif
	
	#ifdef	__PKIG_CY__		// 保卫钓鱼岛
		MENU_ID_PK_CY,
	#endif
	
	#ifdef	__PKIG_HS__		// 丛林飞将
		MENU_ID_PK_HS,
	#endif
	
	#ifdef	__PKIG_FT__		// 玫瑰机动队
		MENU_ID_PK_FT,
	#endif
	
	#ifdef	__PKIG_RC__		// 炮弹飞车
		MENU_ID_PK_RC,
	#endif
	
	#ifdef	__PKIG_VJ__		// 重力宝石
		MENU_ID_PK_VJ,
	#endif
	
	#ifdef	__PKIG_CF__		// 水果忍者
		MENU_ID_PK_CF,
	#endif
	
	#ifdef	__PKIG_JP__		// 魅力拼图
		MENU_ID_PK_JP,
	#endif
	
	#ifdef	__PKIG_NIN__	// 武林飞侠
		MENU_ID_PK_NIN,
	#endif
	
	#ifdef	__PKIG_IP__		// 成语迷局
		MENU_ID_PK_IP,
	#endif
	
	#ifdef	__PKIG_DDZ__	// 斗地主
		MENU_ID_PK_DDZ,
	#endif
	
	#ifdef	__PKIG_FS__		// 水果快点
		MENU_ID_PK_FS,
	#endif
	
	#ifdef	__PKIG_CFM__	// 捕鱼达人
		MENU_ID_PK_CFM,
	#endif

//#else

	/*#ifdef  __PKIG__		//51PK游戏
		<MENUITEM_ID>MENU_ID_FNG_PK_1</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_TQ__		// 天气预报
		<MENUITEM_ID>MENU_ID_PK_TQ</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_SA__		// 野人逃生记
		<MENUITEM_ID>MENU_ID_PK_SA</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_BM__		// 宝石迷情
		<MENUITEM_ID>MENU_ID_PK_BM</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_DM__		// 金字塔历险记
		<MENUITEM_ID>MENU_ID_PK_DM</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_ST__		// 街头速递
		<MENUITEM_ID>MENU_ID_PK_ST</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_SL__		// 营救小LOLI
		<MENUITEM_ID>MENU_ID_PK_SL</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_MJ__		// 跳跳堂
		<MENUITEM_ID>MENU_ID_PK_MJ</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_SPACE__	// 完美真相
		<MENUITEM_ID>MENU_ID_PK_SPACE</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_DR__		// 虫虫别跑
		<MENUITEM_ID>MENU_ID_PK_DR</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_DS__		// 豆豆广场
		<MENUITEM_ID>MENU_ID_PK_DS</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_CY__		// 保卫钓鱼岛
		<MENUITEM_ID>MENU_ID_PK_CY</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_HS__		// 丛林飞将
		<MENUITEM_ID>MENU_ID_PK_HS</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_FT__		// 玫瑰机动队
		<MENUITEM_ID>MENU_ID_PK_FT</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_RC__		// 炮弹飞车
		<MENUITEM_ID>MENU_ID_PK_RC</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_VJ__		// 重力宝石
		<MENUITEM_ID>MENU_ID_PK_VJ</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_CF__		// 水果忍者
		<MENUITEM_ID>MENU_ID_PK_CF</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_JP__		// 魅力拼图
		<MENUITEM_ID>MENU_ID_PK_JP</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_NIN__	// 武林飞侠
		<MENUITEM_ID>MENU_ID_PK_NIN</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_IP__		// 成语迷局
		<MENUITEM_ID>MENU_ID_PK_IP</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_DDZ__	// 斗地主
		<MENUITEM_ID>MENU_ID_PK_DDZ</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_FS__		// 水果快点
		<MENUITEM_ID>MENU_ID_PK_FS</MENUITEM_ID>
	#endif
	
	#ifdef	__PKIG_CFM__	// 捕鱼达人
		<MENUITEM_ID>MENU_ID_PK_CFM</MENUITEM_ID>
	#endif
*/

