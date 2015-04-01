
#include "i51KitAde.h"
#include "i51KitFontDemo.h"
#include "i51KitG2.h"
#include "i51KitStd.h"

i51KIT_STATIC_KIT_DEF(Adapter);
i51KIT_DYNAMIC_KIT_DEF(i51KitG2);
i51KIT_DYNAMIC_KIT_DEF(i51KitStd);

static iU8    UseCach = iTRUE;
static iFILE TypeheadHandle = iNULL;

void TestFontDraw(void)
{
    static iCOLOR Color=0;
    iRECT DrawRect;
    static iHANDLE VP1 = iNULL;
    static iU16 X = 0,Y = 0;
//    iU16 * Str = (iU16 *)"���ڶ�������Ƽ�";
//    iU16 * Str = (iU16 *)"�ҵ�һ���ڲ����к������д�Ϊ�ϸ���";
//    iU16 * Str = (iU16 *)"���οƼ����οƼ����οƼ�";
    //iU8 * Str = (iU8 *)"��һ���ڲ����к������д�Ϊ�ϸ�������Ҫ��ʱ���������������ڳ��ͷֶԳɻ�������궯ͬ��Ҳ���¹���˵�����������ඨ��ѧ������þ�ʮ��֮���ŵȲ��ȼҵ�������ˮ�����Զ�����С����ʵ�����������ƻ���ʹ���ҵ��ȥ���Ժ�Ӧ�����ϻ�������ЩȻǰ������������������ƽ����ȫ�������ظ��������������ķ�������ԭ��ô���Ȼ�������������˱���ֻû������⽨�¹���ϵ������������������ͨ����ֱ�⵳��չ�������Ա��λ�볣���ܴ�Ʒʽ���輰���ؼ�������ͷ���ʱ���·����ͼɽͳ��֪�Ͻ�����Ʊ����ֽ��ڸ�����ũָ������ǿ�ž�����������ս�Ȼ�����ȡ�ݴ����ϸ�ɫ���ż����α���ٹ������ߺ��ڶ�����ѹ־���������ý���˼����������ʲ������Ȩ��֤���强���ٲ�ת�������д�׽��ٻ������������������ÿĿ�����߻�ʾ��������������뻪��ȷ�ſ�������ڻ�������Ԫ�����´�����Ⱥ��ʯ������н������ɽ��Ҿ���Խ֯װӰ��ͳ������鲼���ݶ�����̷����������ѽ���ǧ��ί�ؼ��������ʡ��ϰ��Լ֧��ʷ���ͱ����������п˺γ���������̫׼��ֵ������ά��ѡ��д���ë�׿�Ч˹Ժ�齭�����������������ɲ�Ƭʼȴר״������ʶ����Բ����ס�����ؾ��ղκ�ϸ������������";
//    iU16 * Str = (iU16 *)"��һ���ڲ����к������д�Ϊ�ϸ�������Ҫ��ʱ���������������ڳ��ͷֶԳɻ�������궯ͬ��Ҳ���¹���˵�����������ඨ��ѧ������þ�ʮ��֮���ŵȲ��ȼҵ�������ˮ�����Զ�����С����ʵ�����������ƻ���ʹ���ҵ��ȥ���Ժ�Ӧ�����ϻ�������ЩȻǰ������������������ƽ����ȫ�������ظ��������������ķ�������ԭ��ô���Ȼ�������������˱���ֻû������⽨�¹���ϵ������������������ͨ����ֱ�⵳��չ�������Ա��λ�볣���ܴ�Ʒʽ���輰���ؼ�������ͷ���ʱ���·����ͼɽͳ��֪�Ͻ�����Ʊ����ֽ��ڸ�����ũָ������ǿ�ž�����������ս�Ȼ�����ȡ�ݴ����ϸ�ɫ���ż����α���ٹ������ߺ��ڶ�����ѹ־���������ý���˼����������ʲ������Ȩ��֤���强���ٲ�ת�������д�׽��ٻ������������������ÿĿ�����߻�ʾ��������������뻪��ȷ�ſ�������ڻ�������Ԫ�����´�����Ⱥ��ʯ������н������ɽ��Ҿ���Խ֯װӰ��ͳ������鲼���ݶ�����̷����������ѽ���ǧ��ί�ؼ��������ʡ��ϰ��Լ֧��ʷ���ͱ����������п˺γ���������̫׼��ֵ������ά��ѡ��д���ë�׿�Ч˹Ժ�齭�����������������ɲ�Ƭʼȴר״������ʶ����Բ����ס�����ؾ��ղκ�ϸ������������";
    iU16 * Str = (iU16 *)"  1967��,�������ĵ�һ����Ŀ��multics,����һ������ϵͳ��Ŀ,������ڴ��ͼ��������������,������������Ŀ������Ӵ�,�ۺ���̫�������,multics�����ܺܵ�,������ʧ�ܶ�����.multics��Ŀ�Ŀ�����֮һ��.����ѷ�����Ϊge-5�������,�����ձ�д��һ���²���ϵͳ��Ŀ,�ܹ�֧��ͬʱ�Ķ��û�����,Ҳ���Ǻ�����unix.";
//    iS8 * Str = "6.strlen�Ľ��Ҫ�����е�ʱ����ܼ������,�����������ַ����ĳ���,��������ռ�ڴ�Ĵ�С.";
//    iU16 * Str = (iU16 *)"�������ĵ�һ����Ŀ!!!!!!!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
//    iU16 * Str = (iU16 *)"�������ĵ�һ����Ŀ!!!!!!!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
//    iU16 * Str = (iU16 *)"�����˵�";
    iU8 i;	

    DrawRect.X = 0;
    DrawRect.Y = 0;
    DrawRect.Width = 320;
    DrawRect.Height= 480;

#if 0
if(VP1==iNULL)
{
    VP1 = i51KitG2VPCreate(4, &DrawRect);
    i51KitG2VPSetActivity(VP1);
    DrawRect.Y = 20;
    i51KitG2FontDraw(TypeheadHandle,0,(void *)Str, 0, &DrawRect, 0,2,2, 0XF000, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 80;
    i51KitG2FontDraw(TypeheadHandle,1,(void *)Str, 0, &DrawRect, 0,2,2, 0X0F00, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 160;
    i51KitG2FontDraw(TypeheadHandle,2,(void *)Str, 0, &DrawRect, 0,2,2, 0X00F0, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 250;
    i51KitG2FontDraw(TypeheadHandle,2,(void *)Str, 0, &DrawRect, 0,4,4, 0X00F0, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
}


    DrawRect.Y = 20;
//    i51KitG2FontDraw((void *)Str, 66, &DrawRect, 0, Color+500, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 40;
//    i51KitG2FontDraw( TypeheadHandle , 0,(void *)Str, 0, &DrawRect, 2,2,0, Color+500, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    i51KitG2VPRender(VP1, iNULL);
    i51KitG2Update();
#else
    Color += 5000;
    i51KitG2FontDraw(TypeheadHandle,2,(void *)Str, 0, &DrawRect, &DrawRect,0,0,0, Color, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    i51KitG2Update();
#endif

}


iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

	//	TODO :	��Ϣѭ��

	switch ( message )
	{
	        case i51_MSG_KEYUP :
			if ( UseCach)
			{
				UseCach = iFALSE;
				i51KitG2FontCachEnable(TypeheadHandle, 2);
			}
			else
			{
				UseCach = iTRUE;
				i51KitG2FontCachDisable(TypeheadHandle, 2);
			}
	             break;
			 
		case i51_MSG_PAINT :
			TestFontDraw();
			break ;
		case i51_MSG_INIT :
		{
	
			i51KIT_STATIC_KIT_SET(Adapter,parameter);
			i51KIT_STATIC_KIT_ASSERT(Adapter);
	             //	����i51KitStd��̬��
	             i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,L"PKiGAPI\\i51KitStd.kit");
	             if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitStd) ) 
	             {
	                 return 0 ;
	             }
			 
	             //	����i51KitStd��̬��
	             i51KIT_DYNAMIC_KIT_LOAD(i51KitG2,L"PKiGAPI\\i51KitG2.kit");
	             if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitG2) ) 
	             {
	                 return 0 ;
	             }

TypeheadHandle = i51KitStdResOpen(L"i51KitFontDemo\\Typehead.re");
i51KitG2FontCachEnable(TypeheadHandle, 2);
//i51KitG2VPEnable();
i51KitG2CleanScreen(0, iNULL) ;
i51KitG2Update();
		}
		break ;

		case i51_MSG_PAUSE :
		break ;

		case i51_MSG_EXIT :
		break ;
		

	}

	

	return iTRUE ;

}
