//#define MAMOTA_BACKEND_MTK
//#define MAMOTA_BACKEND_MSTAR
//#define MAMOTA_BACKEND_ST


/****************************
MAMOTA_DEBUG ��ӡ������Ϣ�ļ��꿪��
****************************/
//#define MAMOTA_DEBUG

#ifdef MAMOTA_DEBUG

#define MM_DBG_PRINT(...) mmDbgPrint(__VA_ARGS__);

#else

#define MM_DBG_PRINT

#endif


#define LAYER_TRANSPARENCE_COLOR		0x1f
