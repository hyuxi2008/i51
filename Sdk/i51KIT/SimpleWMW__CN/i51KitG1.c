
#include "i51KitAde.h"
#include "i51KitG1.h"
//#include "i51KitG2.h"
#include "mwPublic.h"

//static iU16 *se_ScreenBuf = iNULL;
#ifdef __MODIS_DEBUG__
static iU16 se_ScreenWidth=320, se_ScreenHeight=480;

#else
static iU16 se_ScreenWidth=SCREEN_WIDTH, se_ScreenHeight=SCREEN_HEIGHT;

#endif
//static iCOLOR se_BorderWidth = 3;
//static iCOLOR se_FillColor = 0xFFFF, se_BorderColor = 0xDDDD;
static iCOLOR se_FillColor = COLOR_WATER, se_BorderColor = COLOR_WATER;
//static iS16 se_BaseCount = 0;
iS16 se_TrackPointCount = 0;
iS16 se_TrackPoint[EFFECT_MAXTRACKPOINT][2] = {0};
//static iS16 se_endDrawFlag = 0;
static iALPHA se_alpha = 0xFF;

#define G1_MAXBLADEWIDTH	4		//���������(һ��)
#define G1_MAXKNIFEPOINT    12		//���ⳤ
#define G1_MAXTRACKPOINT	8		//�켣������������
#define G1_MINDISTANCE		144		//�켣�����С�����ƽ��
#define G1_MAXPOINTNUM		5

#define G1_MAXTOPPOINT      8		//����߶������ֵ
//���Ա߱�ṹ
typedef struct tagACTIVE_EDGE
{
	iS16 IsValid;		 //�Ƿ���Ч�ı�־ 0==��Ч 1==��Ч
	iS16 MaxCoord;		 //ɨ�����ƶ��ķ��������ֵ
	iS16 factor;		 //�˻�����
	iS32 slope;			 //б�ʣ�����factor����
	iS32 IntersectCoord; //��ɨ���߽������꣨����factor����

	struct tagACTIVE_EDGE *next; //��һ�����Ա�
}G1_ACTIVE_EDGE;


//�߷����ṹ
typedef struct tagCLASS_EDGE
{
	iS16 coord;					  //ɨ��������
	iS16 count;					  //������ı�����
	iS16 id[G1_MAXTOPPOINT];	  //��id����
}G1_CLASS_EDGE;

typedef struct tagEDGE_TABLE
{
	iS16	type;							//ɨ�跽ʽ 0==X�᷽�� 1==Y�᷽��
	iS16	coord;							//��ǰɨ��������

	iS32    EdgeCount;						//������
	iS16	line[G1_MAXTOPPOINT][4];		//�� [2] x, y

	iS32	ClassCount;							//�߷������
	G1_CLASS_EDGE	ClassTable[G1_MAXTOPPOINT]; //����߱�

	iS32    ActiveCount;						//���Ա�����
	G1_ACTIVE_EDGE *ActiveHead;						//����ͷ
	G1_ACTIVE_EDGE ActiveTable[G1_MAXTOPPOINT]; //�߱�
}G1_EDGE_TABLE;

static G1_EDGE_TABLE  G1_EdgeTable = {0};

static iCOLOR i51KitG1GetNewColor(iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha)
{
	/**
	* @brief	����alphaֵ��alpha�㷨��dest <- (dest*(255 - alpha) + src * alpha) / 255
	* 
	* @param	dpixel	Ŀ������ֵ
	* @param	spixel	Դ����ֵ
	* @param	alpha	alphaֵ
	* 
	* @return	����alpha������
	*/
	iU32 rd, gd, bd;
	iU32 rs, gs, bs;
	iALPHA DesAlpha = 0XFF - SrcAlpha;

	rs = (spixel) & 0XF800;
	gs = (spixel) & 0X07E0;
	bs = spixel & 0X1F;

	rd = (dpixel) & 0XF800;
	gd = (dpixel) & 0X07E0;
	bd = dpixel & 0X1F;

	rd = ((rd*DesAlpha+ rs * SrcAlpha)>>8)&0XF800;
	gd = ((gd*DesAlpha+ gs * SrcAlpha)>>8)&0X7E0;
	bd = ((bd*DesAlpha+ bs * SrcAlpha)>>8)&0X1F;

	return (bd +(gd)+ (rd ));
}

static iBOOL i51KitG1DrawPoint( iS32 X, iS32 Y, iCOLOR color ,iALPHA Alpha)
{
	if( Y>=0 && Y<SCREEN_HEIGHT&& X>=0 && X<SCREEN_WIDTH)
	{
		iS32 WorldAttributeY = Y/2;
		iS32 WorldAttributeX = X/2;
		iS32 RemainderY = Y%2;
		iS32 RemainderX = X%2;
		iS32 ScreenPos = 0;
		iU8 WorldAttributeFlag = 0;
	
		
		if( RemainderX) WorldAttributeFlag = 0X20;
		else WorldAttributeFlag = 0X10;

		if( RemainderY ) WorldAttributeFlag = WorldAttributeFlag<<2;

		if( (((mwWorldAttribute[WorldAttributeY][WorldAttributeX]&0XF)==0  || WaterDrawAll))
		&& ((mwWorldAttribute[WorldAttributeY][WorldAttributeX]&WorldAttributeFlag)==0) )
		{

#ifdef __MODIS_DEBUG__
			ScreenPos = Y*320 + X;

#else
			ScreenPos = Y*SCREEN_WIDTH + X;

#endif
			mwScreenBuffer[ScreenPos] = i51KitG1GetNewColor(mwScreenBuffer[ScreenPos], color, Alpha);
			mwWorldAttribute[WorldAttributeY][WorldAttributeX] |= WorldAttributeFlag;
		}

		return iTRUE;
	}

	return iFALSE;
}
/************************************************************************/
/*          �����ǻ�ͼ��������                                          */
/************************************************************************/
static iBOOL i51KitG1GetNormalPoints(iS16 NormalPoint[2][2], iS16 LinePoint[2][2], iS16 width, iS16 CentreX, iS16 CentreY)
{
	//  author : yepan
	//  since  : 2012-7-23
	//  (C) PKIG Tech. Co., Ltd.
	//  
	//  Param  :
	//			NormalPoint  ���������
	//			LinePoint    ֱ��
	//			width		 ����
	//			CentreX,CentreY �Գ����ĵ�
	//  return :
	//			iTRUE==�ɹ��� iFALSE==ʧ��
	//  note   :
	//			���߶�LinePoint�ϣ�CentreX, CentreY)�����ΪWidth������
	

	iS16 dx=0, dy=0, tmp_var=0;
	iS16 abs_dx=0, abs_dy=0;
	iS16 move_x=0, move_y=0;
	iS32 fun_var = 0;

	dx= LinePoint[1][0] -  LinePoint[0][0];
	dy= LinePoint[1][1] -  LinePoint[0][1];

	if ( 0 == width || (0==dx &&0==dy)){
		NormalPoint[0][0] = CentreX;
		NormalPoint[0][1] = CentreY;
		NormalPoint[1][0] = CentreX;
		NormalPoint[1][1] = CentreY;
		return iTRUE;
	}

	//��ֱx
	if ( 0 == dx ){
		NormalPoint[0][0] = CentreX - width;
		NormalPoint[0][1] = CentreY;
		NormalPoint[1][0] = CentreX + width;
		NormalPoint[1][1] = CentreY;
		return iTRUE;
	}

	//��ֱy
	if ( 0 == dy ){
		NormalPoint[0][0] = CentreX;
		NormalPoint[0][1] = CentreY - width;
		NormalPoint[1][0] = CentreX;
		NormalPoint[1][1] = CentreY + width;
		return iTRUE;
	}

	tmp_var = dx;
	dx = -dy;
	dy = tmp_var;
	abs_dx = dx > 0 ? dx : -dx;
	abs_dy = dy > 0 ? dy : -dy;

	if ( abs_dx > abs_dy ){

		if (dx < 0){
			dx = -dx;
			dy = -dy;
		}

		move_x	= width;
		fun_var = dy*move_x;

		if (fun_var > 0){
			move_y = (fun_var%dx > dx/2) ? (fun_var/dx + 1) : (fun_var/dx);
		}else{
			move_y = (fun_var%dx < -dx/2) ? (fun_var/dx - 1) : (fun_var/dx);
		}

		NormalPoint[0][0] = CentreX + move_x;
		NormalPoint[0][1] = CentreY + move_y;
		NormalPoint[1][0] = CentreX - move_x;
		NormalPoint[1][1] = CentreY - move_y;

	}else{

		if (dy < 0){
			dy = -dy;
			dx = -dx;
		}

		move_y	= width;
		fun_var = dx*move_y;

		if (fun_var > 0){
			move_x = (fun_var%dy > dy/2) ? (fun_var/dy + 1) : (fun_var/dy);
		}else{
			move_x = (fun_var%dy < -dy/2) ? (fun_var/dy - 1) : (fun_var/dy);
		}

		NormalPoint[0][0] = CentreX + move_x;
		NormalPoint[0][1] = CentreY + move_y;
		NormalPoint[1][0] = CentreX - move_x;
		NormalPoint[1][1] = CentreY - move_y;
	}

	return iTRUE;
}

static iBOOL i51KitG1GetForwardPoints(iS16 ForwardPoint[2], iS16 LinePoint[2][2], iS16 length)
{
//  author : yepan
//  since  : 2012-7-23
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			ForwardPoint ����ĵ�
//			LinePoint	 �߶�
//			length		 ����
//  return :
//			iTRUE==�ɹ��� iFALSE==ʧ��			
//  note   :
//			��ȡֱ�߷����Ͼ����յ�length�ĵ�

	iS16 dx=0, dy=0;
	iS16 abs_dx=0, abs_dy=0;
	iS16 move_x=0, move_y=0;
	iS32 fun_var = 0;

	dx= LinePoint[1][0] -  LinePoint[0][0];
	dy= LinePoint[1][1] -  LinePoint[0][1];
	abs_dx = dx > 0 ? dx : -dx;
	abs_dy = dy > 0 ? dy : -dy;

	if ( 0 == length || (0==dx &&0==dy)){
		ForwardPoint[0] = LinePoint[1][0];
		ForwardPoint[1] = LinePoint[1][1];
		return iTRUE;
	}

	//��ֱx
	if ( 0 == dx ){
		ForwardPoint[0] = LinePoint[1][0];

		if (dy > 0){
			ForwardPoint[1] = LinePoint[1][1] + length;
		}else{
			ForwardPoint[1] = LinePoint[1][1] - length;
		}
		return iTRUE;
	}

	//��ֱy
	if ( 0 == dy ){
		if (dx>0){
			ForwardPoint[0] = LinePoint[1][0] + length;
		}else{
			ForwardPoint[0] = LinePoint[1][0] - length;
		}

		ForwardPoint[1] = LinePoint[1][1];
		return iTRUE;
	}

	if ( abs_dx > abs_dy ){

		move_x = dx > 0 ? length : -length;
		fun_var = dy*move_x;
		
		fun_var = dx > 0 ? fun_var : -fun_var;

		if (fun_var > 0){
			move_y = (fun_var%abs_dx > abs_dx/2) ? (fun_var/abs_dx + 1) : (fun_var/abs_dx);
		}else{
			move_y = (fun_var%abs_dx < -abs_dx/2) ? (fun_var/abs_dx - 1) : (fun_var/abs_dx);
		}

	}else{

		move_y = dy > 0 ? length : -length;
		fun_var = dx*move_y;

		fun_var = dy > 0 ? fun_var : -fun_var;

		if (fun_var > 0){
			move_x = (fun_var%abs_dy > abs_dy/2) ? (fun_var/abs_dy + 1) : (fun_var/abs_dy);
		}else{
			move_x = (fun_var%abs_dy < -abs_dy/2) ? (fun_var/abs_dy - 1) : (fun_var/abs_dy);
		}
	}

	ForwardPoint[0] = LinePoint[1][0] + move_x;
	ForwardPoint[1] = LinePoint[1][1] + move_y;

	return iTRUE;
}

/*
//////////////////////////////////////////////////////////////////////////
//�������»��ƺ���
static iBOOL i51KitG1DrawLine(iS16 line[2][2], iS16 width, iCOLOR color, iALPHA alpha)
{
//  author : yepan
//  since  : 2012-7-5
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			 line  ֱ��
//			 width ֱ�߿��
//			 color ��ɫ
//			 alpha ͸����
//  return :
//  note   :
//			���ƶ��߿�����ֱ��

	iS16 i=0, j=0, count = 0;
	iS16 dx=0, dy=0;
	iS16 abs_dx=0, abs_dy=0;
	iS16 unit_dx=0, unit_dy=0;
	iS16 TempX = 0, TempY=0;
	iS16 StartX = 0, StartY=0;
	iS16 BaseX = 0, BaseY=0;
	iALPHA AlphaValue = 0xFF;
	iU16 *DesPix = iNULL;
	iS16 d0=0, di=0, dWidth=0;
	iS32 BaseValue=0, factor=0, dis=0;

	if (width <= 0) return iFALSE;

	width =  width<2 ? 2 : width;

	dx = line[1][0] - line[0][0];
	dy = line[1][1] - line[0][1];
	abs_dx = dx > 0 ? dx : -dx;
	abs_dy = dy > 0 ? dy : -dy;

	count = abs_dx > abs_dy ? abs_dx : abs_dy;
	AlphaValue = alpha;

	//ֱ�������غ�
	if ( 0 == abs_dx &&  0 == abs_dy )
	{
		TempX = line[0][0];
		TempY = line[0][1];
		if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
		{
			i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
		}
		
		return iTRUE;
	}
	else if ( 0 == abs_dx ) //��ֱ��x��
	{
		StartX = line[0][0] - width/2;
		StartY = line[0][1];
		unit_dy = dy/abs_dy;
		dWidth = unit_dy>0 ? se_ScreenWidth : -se_ScreenWidth;
		for (i=0; i<width; i++)
		{
			TempX = StartX+i;
			TempY = StartY;
			for (j=0; j<=count; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}
				TempY += unit_dy;
			}
		}

		//�Գ��㷨 ��
		return iTRUE;	
	}
	else if (0 == abs_dy) //��ֱ��y��
	{
		StartX = line[0][0];
		StartY = line[0][1] - width/2;
		unit_dx = dx/abs_dx;
		dWidth = unit_dx>0 ? 1 : -1;
		for (i=0; i<width; i++)
		{
			TempX = StartX;
			TempY = StartY + i;
			for (j=0; j<=count; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}

				TempX += unit_dx;

			}
		}

		return iTRUE;
	}


	//�Ȳ���ֱx��Ҳ����ֱy��
	//bresenhamֱ�߻���
	unit_dx = dx/abs_dx;
	unit_dy = dy/abs_dy;
	BaseX = line[0][0];
	BaseY = line[0][1];
	//б�ʾ���ֵС��1
	if ( abs_dx > abs_dy )
	{
		d0 = 2*abs_dy - abs_dx;
		di = d0;

		factor = (dx*line[0][1] - dy*line[0][0]);
		for (i=0; i<=count; i++)
		{	
			//��һ��ɨ����
			BaseValue = dy*BaseX + factor;
			dis = dx*BaseY - BaseValue;
			dis = dis*unit_dx + abs_dx/2;
			AlphaValue = alpha*dis/abs_dx;
			StartY = BaseY - width/2;
			
			TempX = BaseX;
			TempY = StartY;
			if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
			{
				i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
			}

			AlphaValue = alpha - AlphaValue;
			TempY = StartY + width-1;
			if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
			{
				i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
			}

			AlphaValue = alpha;
			TempY = StartY + 1;
			for (j=1; j<=width-2; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}
				TempY++;
			}

			if (di < 0)
			{
				di += 2*abs_dy;
			}
			else
			{
				di += 2*(abs_dy - abs_dx);
				BaseY += unit_dy;
			}
			BaseX += unit_dx;
		}
	}
	else
	{
		d0 = 2 * abs_dx - abs_dy;
		di = d0;
		factor = (dx*line[0][1] - dy*line[0][0]);
		for (i=0; i<=count; i++)
		{
			//��һ��ɨ����
			BaseValue = dx*BaseY - factor;
			dis = dy*BaseX - BaseValue;
			dis = dis*unit_dy + abs_dy/2;
			AlphaValue = alpha*dis/abs_dy;
			StartX = BaseX - width/2;

			TempX = StartX;
			TempY = BaseY;
			if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
			{
				i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
			}

			AlphaValue = alpha - AlphaValue;
			TempX = StartX + width-1;
			if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
			{
				i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
			}

			AlphaValue = alpha;
			TempX = StartX+1;
			for (j=1; j<=width-2; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}

				TempX++;
			}

			if (di < 0)
			{
				di += 2*abs_dx;
			}
			else
			{
				di += 2*(abs_dx - abs_dy);
				BaseX += unit_dx;
			}
			BaseY += unit_dy;
		}
	}

	return iTRUE;
}

*/
/*
static iBOOL i51KitG1DrawLineEx(iS16 line[2][2], iS16 type, iS16 width, iCOLOR color)
{
	//  author : yepan
	//  since  : 2012-7-5
	//  (C) PKIG Tech. Co., Ltd.
	//  
	//  Param  :
	//			 line  ֱ��
	//			 type  alpha���� 0==�� 1==�� 2==�� 3==��
	//			 width ֱ�߿��
	//			 color ��ɫ
	//  return :
	//  note   :
	//			���ƶ��߿�����ֱ��

	iS16 i=0, j=0, count = 0;
	iS16 dx=0, dy=0;
	iS16 abs_dx=0, abs_dy=0;
	iS16 unit_dx=0, unit_dy=0;
	iS16 TempX = 0, TempY=0;
	iS16 StartX = 0, StartY=0;
	iS16 BaseX = 0, BaseY=0;
	iALPHA AlphaValue = 0xFF;
	iU16 *DesPix = iNULL;
	iS16 d0=0, di=0, dWidth=0;
	iS32 BaseValue=0, factor=0, dis=0;

	return iTRUE;
	
	if (width <= 0) return iFALSE;

	width =  width<2 ? 2 : width;

	dx = line[1][0] - line[0][0];
	dy = line[1][1] - line[0][1];
	abs_dx = dx > 0 ? dx : -dx;
	abs_dy = dy > 0 ? dy : -dy;

	count = abs_dx > abs_dy ? abs_dx : abs_dy;
	AlphaValue = 0xFF;

	//ֱ�������غ�
	if ( 0 == abs_dx &&  0 == abs_dy )
	{
		TempX = line[0][0];
		TempY = line[0][1];
		if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
		{
			i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
		}

		return iTRUE;
	}
	else if ( 0 == abs_dx ) //��ֱ��x��
	{
		StartX = line[0][0] - width/2;
		StartY = line[0][1];
		unit_dy = dy/abs_dy;
		dWidth = unit_dy>0 ? se_ScreenWidth : -se_ScreenWidth;

		if ( 2 == type )//��
		{
			i = 1;
		}
		else if (3 == type)//��
		{
			width--;			
		}

		for (i; i<width; i++)
		{
			TempX = StartX+i;
			TempY = StartY;
			for (j=0; j<=count; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}

				TempY += unit_dy;
			}
		}

		return iTRUE;	
	}
	else if (0 == abs_dy) //��ֱ��y��
	{
		StartX = line[0][0];
		StartY = line[0][1] - width/2;
		unit_dx = dx/abs_dx;
		dWidth = unit_dx>0 ? 1 : -1;

		if ( 0 == type )//��
		{
			i = 1;
		}
		else if (1 == type)//��
		{
			width--;			
		}

		for (i=0; i<width; i++)
		{
			TempX = StartX;
			TempY = StartY + i;
			for (j=0; j<=count; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}

				TempX += unit_dx;

			}
		}

		return iTRUE;
	}


	//�Ȳ���ֱx��Ҳ����ֱy��
	//bresenhamֱ�߻���
	unit_dx = dx/abs_dx;
	unit_dy = dy/abs_dy;
	BaseX = line[0][0];
	BaseY = line[0][1];
	//б�ʾ���ֵС��1
	if ( abs_dx > abs_dy )
	{
		d0 = 2*abs_dy - abs_dx;
		di = d0;

		factor = (dx*line[0][1] - dy*line[0][0]);
		for (i=0; i<=count; i++)
		{	
			//��һ��ɨ����
			BaseValue = dy*BaseX + factor;
			dis = dx*BaseY - BaseValue;
			dis = dis*unit_dx + abs_dx/2;
			AlphaValue = COLOR_ALPHA*dis/abs_dx;
			StartY = BaseY - width/2;

			TempX = BaseX;
			if ( 0 == type) //��
			{
				TempY = StartY;
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, AlphaValue);
				}
			}

			if (1 == type) //��
			{
				AlphaValue = COLOR_ALPHA - AlphaValue;
				TempY = StartY + width-1;
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, AlphaValue);
				}
			}

			TempY = StartY + 1;
			for (j=1; j<=width-2; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, AlphaValue);
				}
				TempY++;
			}

			if (di < 0)
			{
				di += 2*abs_dy;
			}
			else
			{
				di += 2*(abs_dy - abs_dx);
				BaseY += unit_dy;
			}
			BaseX += unit_dx;
		}
	}
	else
	{
		d0 = 2 * abs_dx - abs_dy;
		di = d0;
		factor = (dx*line[0][1] - dy*line[0][0]);
		for (i=0; i<=count; i++)
		{
			//��һ��ɨ����
			BaseValue = dx*BaseY - factor;
			dis = dy*BaseX - BaseValue;
			dis = dis*unit_dy + abs_dy/2;
			AlphaValue = COLOR_ALPHA*dis/abs_dy;
			StartX = BaseX - width/2;

			TempY = BaseY;
			if ( 2 == type)//��
			{
				TempX = StartX;
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, AlphaValue);
				}
			}

			if ( 3 == type ) //��
			{
				AlphaValue = COLOR_ALPHA - AlphaValue;
				TempX = StartX + width-1;
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, AlphaValue);
				}
			}

			TempX = StartX+1;
			for (j=1; j<=width-2; j++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, AlphaValue);
				}

				TempX++;
			}

			if (di < 0)
			{
				di += 2*abs_dx;
			}
			else
			{
				di += 2*(abs_dx - abs_dy);
				BaseX += unit_dx;
			}
			BaseY += unit_dy;
		}
	}

	return iTRUE;
}
*/

static iBOOL i51KitG1GreateClassET(iS16 (*Polygon)[2])
{
	//  author : yepan
	//  since  : 2012-7-2
	//  (C) PKIG Tech. Co., Ltd.
	//  
	//  Param  :
	//			��
	//  return :
	//			iTRUE==�ɹ��� iFALSE==ʧ��
	//  note   :
	//			�����߷����, �±��Ͽ�������ҿ�
	
	iS16 i=0, j=0, k=0;
	iS16 EdgeCount=0, CoordCount = 0;
	iS16 coord[G1_MAXTOPPOINT] = {0}; //����
	iS16 CoordValue = 0;
	iS16 flag = 1;					//�Ƿ���Ҫ��������ı�־ 1==��, 0==��
	G1_CLASS_EDGE *pNode = iNULL;

	if (G1_EdgeTable.EdgeCount < 3) return iFALSE;

	G1_EdgeTable.ClassCount = 0;
	i51AdeStdMemset16(G1_EdgeTable.ClassTable, 0, sizeof(G1_EdgeTable.ClassTable));

	//���ಢ����ɨ���߷��������ֵ
	EdgeCount = G1_EdgeTable.EdgeCount;
	for ( i=0; i<EdgeCount; i++ )
	{
		flag = 1;
		if ( 0 == G1_EdgeTable.type )
		{
			CoordValue = Polygon[i][0];
		}
		else
		{
			CoordValue = Polygon[i][1];	
		}

		//��λҪ�����λ��
		for( j=0; j<CoordCount; j++)
		{
			if (CoordValue <= coord[j])
			{
				flag = (CoordValue == coord[j]) ? 0 : 1;
				break;
			}
		}

		if ( flag )
		{
			for (k=CoordCount-1; k>=j; k--)
			{
				coord[k+1] = coord[k];
			}

			coord[j] = CoordValue;
			CoordCount++;
		}
	}

	G1_EdgeTable.ClassCount = CoordCount;
	//�����߷����
	if ( 0 == G1_EdgeTable.type )
	{
		for (i=0; i<CoordCount; i++)
		{
			pNode = &(G1_EdgeTable.ClassTable[i]);
			pNode->coord = coord[i];

			for (j=0; j<EdgeCount; j++)
			{
				if ( coord[i] >= G1_EdgeTable.line[j][0] && coord[i] < G1_EdgeTable.line[j][2])
				{
					pNode->id[pNode->count] = j;
					(pNode->count)++;
				}
			}
		}
	}
	else
	{
		for (i=0; i<CoordCount; i++)
		{
			pNode = &(G1_EdgeTable.ClassTable[i]);
			pNode->coord = coord[i];

			for (j=0; j<EdgeCount; j++)
			{
				if ( coord[i] >= G1_EdgeTable.line[j][1] && coord[i] < G1_EdgeTable.line[j][3])
				{
					pNode->id[pNode->count] = j;
					(pNode->count)++;
				}
			}
		}
	}

	return iTRUE;
}

static iBOOL i51KitG1UpdateActiveET(iS16 ScanCoord)
{
//  author : yepan
//  since  : 2012-7-5
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			ScanCoord ɨ��������
//  return :
//  note   :
//			���»��Ա߱�

	iS16 i=0, count=0;
	iS32 ValueTmp=0, ValueCur=0;
	iS16 (*line)[4] = 0;
	G1_ACTIVE_EDGE *pNodePre = iNULL;
	G1_ACTIVE_EDGE *pNodeCur = iNULL;
	G1_ACTIVE_EDGE *ActiveEdge = iNULL;
	G1_CLASS_EDGE  *pClassNode = iNULL;

	if ( iNULL == G1_EdgeTable.ActiveHead )
	{
		G1_EdgeTable.ActiveCount = 0;
		G1_EdgeTable.ActiveHead = G1_EdgeTable.ActiveTable;
	}

	//���»��Ա���������ݲ���ɾ����Ч��
	if ( G1_EdgeTable.ActiveCount > 0)
	{
		//���»��Ա����������
		pNodeCur = G1_EdgeTable.ActiveHead;
		while (pNodeCur)
		{
			pNodeCur->IntersectCoord += pNodeCur->slope;
			pNodeCur = pNodeCur->next;
		}

		//ɾ����Ч��
		pNodeCur = G1_EdgeTable.ActiveHead;
		pNodePre = pNodeCur;
		while (pNodeCur)
		{
			if (pNodeCur->MaxCoord <= ScanCoord)
			{
				if ( G1_EdgeTable.ActiveHead == pNodeCur )
				{
					G1_EdgeTable.ActiveHead = pNodeCur->next;
					i51AdeStdMemset16(pNodeCur, 0, sizeof(G1_ACTIVE_EDGE));
					pNodeCur = G1_EdgeTable.ActiveHead;
					pNodePre = pNodeCur;
				}
				else
				{
					pNodePre->next = pNodeCur->next;
					i51AdeStdMemset16(pNodeCur, 0, sizeof(G1_ACTIVE_EDGE));
					pNodeCur = pNodePre->next;
				}

				G1_EdgeTable.ActiveCount--;
			}
			else
			{
				pNodePre = pNodeCur;
				pNodeCur = pNodeCur->next;
			}
		}
	}
	
	//��������Ƿ��и�����
	count = G1_EdgeTable.ClassCount;
	for (i=0; i<count; i++)
	{
		pClassNode = G1_EdgeTable.ClassTable + i;
		//�÷����б��ཻ
		if ( ScanCoord == pClassNode->coord && pClassNode->count > 0) break;
	}

	if ( count  == i ) return iTRUE;


	//���»��Ա�
	pClassNode = G1_EdgeTable.ClassTable + i;
	count = pClassNode->count;
	//x����ɨ��
	if ( 0 == G1_EdgeTable.type)
	{
		for (i=0; i<count; i++)
		{
			line = G1_EdgeTable.line + pClassNode->id[i];
			
			//��ȡ���еĽڵ�
			ActiveEdge = G1_EdgeTable.ActiveTable;
			while ( 1 == ActiveEdge->IsValid ) ActiveEdge++;

			ActiveEdge->MaxCoord =  (*line)[2];
			ActiveEdge->factor = (*line)[2] - (*line)[0];
			ActiveEdge->slope = (*line)[3] - (*line)[1];
			ActiveEdge->IntersectCoord = ActiveEdge->slope * ScanCoord
				+ ( (ActiveEdge->factor)*((*line)[1]) - (ActiveEdge->slope)*((*line)[0]) );

			//���Ա����޽ڵ������Ա�
			if ( 0 == G1_EdgeTable.ActiveCount )
			{
				ActiveEdge->IsValid = 1;
				ActiveEdge->next = iNULL;
				G1_EdgeTable.ActiveCount = 1;
				G1_EdgeTable.ActiveHead = ActiveEdge;
				continue;
			}

			pNodeCur = G1_EdgeTable.ActiveHead;
			pNodePre = pNodeCur;
			while (pNodeCur)
			{
				ValueCur = (ActiveEdge->IntersectCoord) * (pNodeCur->factor);
				ValueTmp = (pNodeCur->IntersectCoord) * (ActiveEdge->factor);

				if (ValueCur < ValueTmp)
				{
					break;
				}
				else if (ValueCur == ValueTmp)
				{
					ValueTmp = ActiveEdge->slope * pNodeCur->factor;
					ValueCur = pNodeCur->slope * ActiveEdge->factor;

					if( ValueTmp > ValueCur )
					{
						pNodePre = pNodeCur;
						pNodeCur = pNodeCur->next;
					}
					else if ( ValueTmp == ValueCur )
					{
						if ( pNodePre != pNodeCur )
						{
							pNodePre->next = pNodeCur->next;
							i51AdeStdMemset16(pNodeCur, 0, sizeof(G1_ACTIVE_EDGE));
							G1_EdgeTable.ActiveCount--;
							pNodeCur = pNodePre->next;	
						}
						else
						{
							pNodePre = pNodeCur->next;
							i51AdeStdMemset16(pNodeCur, 0, sizeof(G1_ACTIVE_EDGE));
							G1_EdgeTable.ActiveCount--;
							pNodeCur = pNodePre;
						}
					}
					break;
				}
				else
				{
					pNodePre = pNodeCur;
					pNodeCur = pNodeCur->next;
				}
			}

			if ( pNodePre == pNodeCur )
			{
				ActiveEdge->IsValid = 1;
				ActiveEdge->next = pNodeCur;
				G1_EdgeTable.ActiveHead = ActiveEdge;
			}
			else
			{
				ActiveEdge->IsValid = 1;
				pNodePre->next = ActiveEdge;
				ActiveEdge->next = pNodeCur;
			}

			G1_EdgeTable.ActiveCount++;
		}
	}
	else //y����ɨ��
	{
		for (i=0; i<count; i++)
		{
			line = G1_EdgeTable.line + pClassNode->id[i];

			//��ȡ���еĽڵ�
			ActiveEdge = G1_EdgeTable.ActiveTable;
			while ( 1 == ActiveEdge->IsValid ) ActiveEdge++;

			ActiveEdge->MaxCoord =  (*line)[3];
			ActiveEdge->factor = (*line)[3] - (*line)[1];
			ActiveEdge->slope = (*line)[2] - (*line)[0];
			ActiveEdge->IntersectCoord = ActiveEdge->slope * ScanCoord
				- ( (ActiveEdge->slope)*((*line)[1]) - (ActiveEdge->factor)*((*line)[0]));

			//���Ա����޽ڵ������Ա�
			if ( 0 == G1_EdgeTable.ActiveCount )
			{
				ActiveEdge->IsValid = 1;
				ActiveEdge->next = iNULL;
				G1_EdgeTable.ActiveCount = 1;
				G1_EdgeTable.ActiveHead = ActiveEdge;
				continue;
			}

			pNodeCur = G1_EdgeTable.ActiveHead;
			pNodePre = pNodeCur;
			while (pNodeCur)
			{
				ValueCur = (ActiveEdge->IntersectCoord) * (pNodeCur->factor);
				ValueTmp = (pNodeCur->IntersectCoord) * (ActiveEdge->factor);

				if (ValueCur < ValueTmp)
				{
					break;
				}
				else if (ValueCur == ValueTmp)
				{
					ValueTmp = ActiveEdge->slope * pNodeCur->factor;
					ValueCur = pNodeCur->slope * ActiveEdge->factor;
					if( ValueTmp > ValueCur )
					{
						pNodePre = pNodeCur;
						pNodeCur = pNodeCur->next;
					}
					else if (ValueTmp == ValueCur )
					{
						if ( pNodePre != pNodeCur )
						{
							pNodePre->next = pNodeCur->next;
							i51AdeStdMemset16(pNodeCur, 0, sizeof(G1_ACTIVE_EDGE));
							G1_EdgeTable.ActiveCount--;
							pNodeCur = pNodePre->next;	
						}
						else
						{
							pNodePre = pNodeCur->next;
							i51AdeStdMemset16(pNodeCur, 0, sizeof(G1_ACTIVE_EDGE));
							G1_EdgeTable.ActiveCount--;
							pNodeCur = pNodePre;
						}
					}
					break;
				}
				else
				{
					pNodePre = pNodeCur;
					pNodeCur = pNodeCur->next;
				}
			}

			if ( pNodePre == pNodeCur )
			{
				ActiveEdge->IsValid = 1;
				ActiveEdge->next = pNodeCur;
				G1_EdgeTable.ActiveHead = ActiveEdge;
			}
			else
			{
				ActiveEdge->IsValid = 1;
				pNodePre->next = ActiveEdge;
				ActiveEdge->next = pNodeCur;
			}

			G1_EdgeTable.ActiveCount++;
		}
	}

	return iTRUE;
}

static iBOOL i51KitG1DrawActiveET(iCOLOR color, iALPHA alpha)
{
	iS16 i=0;
	iS16 StartCoord=0, EndCoord=0, FixCoord=0;
	iS16 TempX=0, TempY=0;
	G1_ACTIVE_EDGE *EdgeFirst=iNULL;
	G1_ACTIVE_EDGE *EdgeSecond=iNULL;
	iU16 *DesPix = iNULL;

	if (  0 != G1_EdgeTable.ActiveCount%2 ||  0 == G1_EdgeTable.ActiveCount)
	{
		return iFALSE;
	}

	FixCoord = G1_EdgeTable.coord;
	if ( 0 == G1_EdgeTable.type)
	{
		EdgeFirst = G1_EdgeTable.ActiveHead;
		while (EdgeFirst)
		{
			EdgeSecond = EdgeFirst->next;

			//StartCoord = (EdgeFirst->IntersectCoord + EdgeFirst->factor/2 )/EdgeFirst->factor+1;
			//EndCoord   = (EdgeSecond->IntersectCoord - EdgeSecond->factor/2) /EdgeSecond->factor;

			StartCoord = (EdgeFirst->IntersectCoord + EdgeFirst->factor/2 )/EdgeFirst->factor;
			EndCoord   = (EdgeSecond->IntersectCoord - EdgeSecond->factor/2) /EdgeSecond->factor + 1;

			TempX = FixCoord;
			TempY = StartCoord;
			for (i=StartCoord; i<=EndCoord; i++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}
				TempY++;
			}

			EdgeFirst = EdgeSecond->next;
		}
	}
	else
	{
		EdgeFirst = G1_EdgeTable.ActiveHead;
		while (EdgeFirst)
		{
			EdgeSecond = EdgeFirst->next;
			//StartCoord = (EdgeFirst->IntersectCoord + EdgeFirst->factor/2 )/EdgeFirst->factor+1;
			//EndCoord   = (EdgeSecond->IntersectCoord - EdgeSecond->factor/2) /EdgeSecond->factor;
			StartCoord = (EdgeFirst->IntersectCoord + EdgeFirst->factor/2 )/EdgeFirst->factor;
			EndCoord   = (EdgeSecond->IntersectCoord - EdgeSecond->factor/2) /EdgeSecond->factor + 1;

			TempY = FixCoord;
			TempX = StartCoord;
			for (i=StartCoord; i<=EndCoord; i++)
			{
				if (TempX>=0 && TempX<se_ScreenWidth && TempY>=0 && TempY<se_ScreenHeight)
				{
					i51KitG1DrawPoint(TempX, TempY, COLOR_WATER, COLOR_ALPHA);
				}
				TempX++;
			}

			EdgeFirst = EdgeSecond->next;
		}
	}

	return iTRUE;
}

static iBOOL i51KitG1DrawPolygon(iS16 (*Polygon)[2], iS16 number, iS16 type, iCOLOR color, iALPHA alpha)
{
//  author : yepan
//  since  : 2012-7-5
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			Polygon ����ζ��㼯��
//			number ������
//			color ��ɫ
//			alpha ͸����
//  return :
//  note   :
//		   ����λ���	
//			

	int i=0, count = 0;
	iS16 StartIndex = 0, EndIndex=0;
	iS16 min_coord=0, max_coord=0;
	iS16 (*line)[4] = 0;
	iS16 LineTemp[2][2] = {0};

	if ( iNULL == Polygon || number <= 0 ||  number > G1_MAXTOPPOINT ) return iFALSE;

	if ( number <= 2)
	{
		//�������
//		i51KitG1DrawLine(Polygon, 2, color, alpha);
		return iTRUE;
	}

	//��ʼ���߱�
	i51AdeStdMemset16(&G1_EdgeTable, 0, sizeof(G1_EdgeTable));

	G1_EdgeTable.type = type;
	G1_EdgeTable.EdgeCount = number;
	if ( 0 == G1_EdgeTable.type )
	{
		for (i=0; i<number; i++)
		{
			StartIndex = i;
			EndIndex   = (i+1)%number;
			line = G1_EdgeTable.line + i;
			if ( Polygon[StartIndex][0] > Polygon[EndIndex][0] )
			{
				EndIndex   ^= StartIndex;
				StartIndex ^=  EndIndex;
				EndIndex   ^= StartIndex;
			}

			(*line)[0] = Polygon[StartIndex][0];
			(*line)[1] = Polygon[StartIndex][1];
			(*line)[2] = Polygon[EndIndex][0];
			(*line)[3] = Polygon[EndIndex][1];
		}
	}
	else
	{
		for (i=0; i<number; i++)
		{
			StartIndex = i;
			EndIndex   = (i+1)%number;
			line = G1_EdgeTable.line + i;
			if ( Polygon[StartIndex][1] > Polygon[EndIndex][1] )
			{
				EndIndex   ^= StartIndex;
				StartIndex ^=  EndIndex;
				EndIndex   ^= StartIndex;
			}

			(*line)[0] = Polygon[StartIndex][0];
			(*line)[1] = Polygon[StartIndex][1];
			(*line)[2] = Polygon[EndIndex][0];
			(*line)[3] = Polygon[EndIndex][1];
		}
	}

	if ( iFALSE == i51KitG1GreateClassET(Polygon) ) return iFALSE;
	
	count = G1_EdgeTable.ClassCount - 1;
	min_coord = G1_EdgeTable.ClassTable[0].coord;
	max_coord = G1_EdgeTable.ClassTable[count].coord;

	//���»��Աߣ�����ͼ��
	for (i=min_coord; i<=max_coord; i++)
	{
		G1_EdgeTable.coord = i;

		i51KitG1UpdateActiveET(i);

		i51KitG1DrawActiveET(color, alpha);
	}

	////����η�������
	//count = number - 1;
	//for (i=0; i<count; i++)
	//{
	//	i51KitG1DrawLine(Polygon+i, 3, color, alpha);
	//}

	//i51AdeStdMemcpy16(LineTemp, Polygon+count, sizeof(LineTemp)/2);
	//i51AdeStdMemcpy16(LineTemp+1, Polygon, sizeof(LineTemp)/2);
	//i51KitG1DrawLine(LineTemp, 3, color, alpha);

	return iTRUE;
}

static iBOOL i51KitG1DrawBladeHead(iS16 line[2][2], iS16 width, iS16 length, iALPHA alpha)
{
	mwFillCircle(line[1][0], line[1][1], width, se_FillColor);

	return iTRUE;
}

//��ȡ��ֱ�߽���ĶԳƵ�
static iBOOL i51KitG1GetSymmetricalPoints(iS16 PointOut[2], iS16 PointIn[2], iS16 Line1[2][2], iS16 Line2[2][2], iS16 ReferPoint[2],  iS16 width)
{
	iS16 first_dx=0, first_dy=0;
	iS16 second_dx=0, second_dy=0;
	iS16 Third_dx = 0, Third_dy=0;
	iS16 ForwardPoint[2] = {0};
	iS16 NormalPoint[2][2]={0};
	iS16 line[2][2]={0};
	iS32 dValue = 0, flag1=0, flag2=0;
	iS16 pointTemp[2] = {0};

	first_dx = Line1[1][0] -  Line1[0][0];
	first_dy = Line1[1][1] -  Line1[0][1];
	second_dx = Line2[1][0] -  Line2[0][0];
	second_dy = Line2[1][1] -  Line2[0][1];

	flag1 = first_dx*first_dx + first_dy*first_dy;
	flag2 = second_dx*second_dx + second_dy*second_dy;

	if ( 0 == flag1)
	{
		i51KitG1GetNormalPoints(NormalPoint, Line2, width, Line2[0][0], Line2[0][1]);
	}else if ( 0 == flag2)
	{
		i51KitG1GetNormalPoints(NormalPoint, Line1, width, Line1[1][0], Line1[1][1]);
	}else{
		NormalPoint[0][0] = 0;
		NormalPoint[0][1] = 0;
		NormalPoint[1][0] = first_dx;
		NormalPoint[1][1] = first_dy;
		i51KitG1GetForwardPoints(ForwardPoint, NormalPoint, 50);
		Third_dx = ForwardPoint[0] - NormalPoint[1][0];
		Third_dy = ForwardPoint[1] - NormalPoint[1][1];

		NormalPoint[0][0] = 0;
		NormalPoint[0][1] = 0;
		NormalPoint[1][0] = second_dx;
		NormalPoint[1][1] = second_dy;
		i51KitG1GetForwardPoints(ForwardPoint, NormalPoint, 50);
		Third_dx += (ForwardPoint[0] - NormalPoint[1][0]);
		Third_dy += (ForwardPoint[1] - NormalPoint[1][1]);

		line[0][0] = 0;
		line[0][1] = 0;
		line[1][0] = Third_dx;
		line[1][1] = Third_dy;
		i51KitG1GetNormalPoints(NormalPoint, line, width, 0, 0);

		NormalPoint[0][0] = NormalPoint[0][0] + Line1[1][0];
		NormalPoint[0][1] = NormalPoint[0][1] + Line1[1][1];
		NormalPoint[1][0] = NormalPoint[1][0] + Line1[1][0];
		NormalPoint[1][1] = NormalPoint[1][1] + Line1[1][1];
	}

	PointOut[0] = NormalPoint[0][0];
	PointOut[1] = NormalPoint[0][1];
	PointIn[0] = NormalPoint[1][0];
	PointIn[1] = NormalPoint[1][1];

	//��֤ͬһ��ĵ�
	//F(x) = dx*y-dy*x - (dx*y0 - dy*x0)
	dValue = first_dx*Line1[0][1] - first_dy*Line1[0][0];
	flag1 = first_dx*ReferPoint[1] - first_dy*ReferPoint[0] - dValue;
	flag2 = first_dx*PointOut[1] - first_dy*PointOut[0] - dValue;
	if ( flag1*flag2 < 0 )
	{
		//������
		pointTemp[0] = PointIn[0];
		pointTemp[1] = PointIn[1];
		PointIn[0] = PointOut[0];
		PointIn[1] = PointOut[1];
		PointOut[0] = pointTemp[0];
		PointOut[1] = pointTemp[1];		
	}

	return iTRUE;
}

static  iBOOL i51KitG1GetAlphaFillType(iS16 line1[2][2], iS16 line2[2][2], iS16 *out_type)
{
	iS16 CenterX = 0, CenterY=0;
	iS16 dx = 0, dy=0;
	iS16 abs_dx = 0, abs_dy=0;
	iS32 flag = 0, factor=0, slope=0;

	CenterX = (line1[0][0] + line1[1][0] + line2[0][0] + line2[1][0])/4;
	CenterY = (line1[0][1] + line1[1][1] + line2[0][1] + line2[1][1])/4;

	
	dx = line1[1][0] - line1[0][0];
	dy = line1[1][1] - line1[0][1];
	abs_dx = dx>0 ? dx : -dx;
	abs_dy = dy>0 ? dy : -dy;
	slope = dx*dy;

	//F(x) = dx*y-dy*x - (dx*y0 - dy*x0)
	factor = dx*line1[0][1] - dy*line1[0][0];
	flag = dx*CenterY - dy*CenterX - factor;

	if ( 0 == dx )
	{
		*out_type = CenterX > line1[0][0] ? 2 : 3;
		return iTRUE;
	}

	if ( 0 == dy )
	{
		*out_type = CenterY > line1[0][1] ? 0 : 1;
		return iTRUE;
	}

	flag = dx < 0 ? -flag : flag;
	
	if (abs_dx>abs_dy)
	{
		*out_type = flag > 0 ? 0 : 1;
	}
	else
	{
		if (slope > 0)
		{
			*out_type = flag > 0 ? 3 : 2;
		}
		else
		{
			*out_type = flag > 0 ? 2 : 3;
		}
	}

	return iTRUE;
}

iBOOL i51KitG1Draw()
{

	iS16 i=0, count=0;
	iS16 width = 0, type=0;
	iS16 abs_dx = 0, abs_dy=0;
	iS16 (*point)[2] = iNULL;
	iS16 (*line)[2] = iNULL;
	iS16 PolygonPoint[4][2] = {0};

	if (se_TrackPointCount>=2)
	{
		count = se_TrackPointCount-2;
		PolygonPoint[0][0] = se_TrackPoint[0][0];
		PolygonPoint[0][1] = se_TrackPoint[0][1];
		PolygonPoint[3][0] = se_TrackPoint[0][0];
		PolygonPoint[3][1] = se_TrackPoint[0][1];
		point = PolygonPoint;
		line = se_TrackPoint;

		if (2==se_TrackPointCount) width = G1_MAXBLADEWIDTH;

		for (i=0; i<count; i++)
		{
			width = width < G1_MAXBLADEWIDTH ? (width+2) : width;

			//iLog("11 i51KitG1Draw");

			i51KitG1GetSymmetricalPoints(*(point+1), *(point+2), line+i, line+i+1, (*point),  width);

			abs_dx = se_TrackPoint[i+1][0] - se_TrackPoint[i][0];
			abs_dy = se_TrackPoint[i+1][1] - se_TrackPoint[i][1];
			abs_dx = abs_dx > 0 ? abs_dx : -abs_dx;
			abs_dy = abs_dy > 0 ? abs_dy : -abs_dy;
			type = abs_dy > abs_dx ? 0 : 1;

			//iLog("22 i51KitG1Draw");

			i51KitG1DrawPolygon(PolygonPoint, 4, type, se_FillColor, se_alpha);
			//iLog("33 i51KitG1Draw");

			//i51KitG1DrawLine(point, se_BorderWidth, se_BorderColor, se_alpha);
			//i51KitG1DrawLine(point+2, se_BorderWidth, se_BorderColor, se_alpha);
			//iLog("44 i51KitG1Draw");

			i51KitG1GetAlphaFillType(point, point+2, &type);
			//iLog("55 i51KitG1Draw");

//			i51KitG1DrawLineEx(point,  type, se_BorderWidth, se_BorderColor);
			i51KitG1GetAlphaFillType(point+2, point, &type);
			//iLog("66 i51KitG1Draw");

//			i51KitG1DrawLineEx(point+2, type, se_BorderWidth, se_BorderColor);


			PolygonPoint[0][0] = PolygonPoint[1][0];
			PolygonPoint[0][1] = PolygonPoint[1][1];
			PolygonPoint[3][0] = PolygonPoint[2][0];
			PolygonPoint[3][1] = PolygonPoint[2][1];
		}
		//iLog("77 i51KitG1Draw");

		i51KitG1GetSymmetricalPoints(*(point+1), *(point+2), line+count, line+count, (*point),  width);
		//iLog("88 i51KitG1Draw");

		abs_dx = se_TrackPoint[i+1][0] - se_TrackPoint[i][0];
		abs_dy = se_TrackPoint[i+1][1] - se_TrackPoint[i][1];
		abs_dx = abs_dx > 0 ? abs_dx : -abs_dx;
		abs_dy = abs_dy > 0 ? abs_dy : -abs_dy;
		type = abs_dy > abs_dx ? 0 : 1;
		//iLog("99 i51KitG1Draw");

		i51KitG1DrawPolygon(PolygonPoint, 4, type, se_FillColor, se_alpha);
		//iLog("100 i51KitG1Draw");

		//i51KitG1DrawLine(point, se_BorderWidth, se_BorderColor, se_alpha);
		//i51KitG1DrawLine(point+2, se_BorderWidth, se_BorderColor, se_alpha);
		i51KitG1GetAlphaFillType(point, point+2, &type);
		//iLog("101 i51KitG1Draw");

//		i51KitG1DrawLineEx(point,  type, se_BorderWidth, se_BorderColor);
		i51KitG1GetAlphaFillType(point+2, point, &type);
		//iLog("102 i51KitG1Draw");

//		i51KitG1DrawLineEx(point+2, type, se_BorderWidth, se_BorderColor);

		i51KitG1DrawBladeHead(se_TrackPoint+count, width, G1_MAXKNIFEPOINT, se_alpha);
		//iLog("103 i51KitG1Draw");

	}


	return iTRUE;

}
//�������»��ƺ���
//////////////////////////////////////////////////////////////////////////


