#include "i51KitAde.h"
#include "i51KitG2Ex.h"
//ϵͳ��ǰ��Alphaֵ
static iALPHA AlphaSys = i51KITG2_ALPHA_NONEED;
// ԭͼ��Alpha
static iALPHA AlphaSrc = i51KITG2_ALPHA_NONEED;
// Ŀ�ĵص�Alpha
static iALPHA AlphaDes = 0;
iCOLOR i51KitG2GetNewColor (iCOLOR dpixel, iCOLOR spixel);
static iS32 SinData[91] = 
{
	0, 285,  571,  857, 1142, 1427, 1712, 1996, 2280, 2563, 2845,
	3126, 3406, 3685, 3963, 4240, 4516, 4790, 5062, 5334, 5603,
	5871, 6137, 6401, 6663, 6924, 7182, 7438, 7691, 7943, 8191,
	8438, 8682, 8923, 9161, 9397, 9630, 9860, 10086, 10310, 10531,
	10748, 10963, 11173, 11381, 11585, 11785, 11982, 12175, 12365, 12550,
	12732, 12910, 13084, 13254, 13420, 13582, 13740, 13894, 14043, 14188,
	14329, 14466, 14598, 14725, 14848, 14967, 15081, 15190, 15295, 15395,
	15491, 15582, 15668, 15749, 15825, 15897, 15964, 16025, 16082, 16135,
	16182, 16224, 16261, 16294, 16321, 16344, 16361, 16374, 16381, 16384,
};

iCOLOR i51KitG2GetNewColorEx1 (iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha)
{
	/**
	* @fn		unsigned short calculate_alpha (unsigned short dpixel, unsigned short spixel, unsigned char alpha)
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
iU32 i51KitG2ImageCreate( iU32  SrcData)
{
	// author : Otto
	// since : 10.20 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// iU32 SrcData : ��Ҫ��������ݴ��ָ��

	// notes : ��ѹһ��ͼƬ�����ڴ���
	// SrcFile�ļ��д�ŵ�ͼƬ�����Ƿֶ���Zlibѹ����
	// �����ļ���ʽ��ο��ĵ�"The_i51_DC_dynamic_library_G2_i51_Image_Format.xls"

	// ������
	iU16 NumberParts = 0;
	// ��ǰ�ν�ѹǰ���ݳ���
	iU32 LengthPartSrc = 0;
	// ��ǰ�ν�ѹ�����ݳ���
	iU32 LengthPartDes = 0;
	// ��������ֵ
	iBOOL FlagReturn = iFALSE;
	// ��ѹ������buffer
	iU8  * DesDataPoint = iNULL;
	iU32   DesDataLength = iNULL;
	iU32 SrcPos = 0;
	iU32 DesPos = 0;	
	iU32 iTemp = 0;
	iU32 DesTempPos = 0;

	// ǰ���ĸ��ֽ��Ǳ����ͼƬ��Ⱥ͸߶�	
	iU8 * SrcDataPoint = (iU8 * )(SrcData+4);

	if( SrcData==iNULL ) return iFALSE;

	//    i51AdeStdMemcpy16(&DesDataLength, &SrcDataPoint[4], 4);
	//    i51AdeStdMemcpy16(&NumberParts, &SrcDataPoint[16], 2);
	DesDataLength = SrcDataPoint[4]|(SrcDataPoint[5]<<8)|(SrcDataPoint[6]<<16)|(SrcDataPoint[7]<<24);
	NumberParts = SrcDataPoint[16]|(SrcDataPoint[17]<<8);
	////LOG("DesDataLength=%d",DesDataLength);
	////LOG("NumberParts=%d",NumberParts);
	DesDataPoint = ( iU8 *)i51AdeOsMalloc( DesDataLength,  __FILE__ , __LINE__ );
	if( iNULL == DesDataPoint ) return iFALSE;

	SrcPos = 18;
	DesPos = 0;
	for( iTemp=0; iTemp<NumberParts; iTemp++ )
	{
		//        i51AdeStdMemcpy16(&LengthPartSrc, &SrcDataPoint[SrcPos], 4);
		LengthPartSrc = SrcDataPoint[SrcPos+0]|(SrcDataPoint[SrcPos+1]<<8)|(SrcDataPoint[SrcPos+2]<<16)|(SrcDataPoint[SrcPos+3]<<24);
		////LOG("LengthPartSrc=%d",LengthPartSrc);
		SrcPos += 4;

		LengthPartDes = 32768;
		FlagReturn = i51AdeStdUncompress( &DesDataPoint[DesPos], &LengthPartDes, &SrcDataPoint[SrcPos], LengthPartSrc);
		if( iFALSE == FlagReturn ) goto ERROR;
		////LOG("LengthPartDes=%d",LengthPartDes);

		DesTempPos = DesPos;
		SrcPos += LengthPartSrc;
		DesPos += LengthPartDes;


		if(i51AdeCnfGetEndian()==1)
		{
			if(iTemp==0) DesTempPos = 0X14; 
			while(DesTempPos<DesPos)
			{
				DesDataPoint[DesTempPos] += DesDataPoint[DesTempPos+1];
				DesDataPoint[DesTempPos+1] = DesDataPoint[DesTempPos] - DesDataPoint[DesTempPos+1];
				DesDataPoint[DesTempPos] = DesDataPoint[DesTempPos] - DesDataPoint[DesTempPos+1];
				DesTempPos += 2;
			}
		}

	}

	return (iU32)(DesDataPoint);

ERROR:
	i51AdeOsFree(DesDataPoint);
	DesDataPoint = iNULL;
	return iFALSE;

}

iU32 i51KitG2GetImageAttrEx(iU32  SrcData, iU32 * ImageWidth, iU32 * ImageHeight)
{
	// ǰ���ĸ��ֽ��Ǳ����ͼƬ��Ⱥ͸߶�	
	iU8 * SrcDataPoint = (iU8 * )(SrcData);
	iU16 ImageW = 0;
	iU16 ImageH = 0;	

	if( SrcData==iNULL ) return iFALSE;

	i51AdeStdMemcpy16(&ImageW, &SrcDataPoint[0], 2);
	i51AdeStdMemcpy16(&ImageH, &SrcDataPoint[2], 2);

#ifndef __SPR__
	*ImageWidth = ImageW;   //С��
	*ImageHeight = ImageH;
#else
	*ImageWidth = ((ImageW&0xff00)>>8) + ((ImageW&0x00ff)<<8) ;//���չѶ
	*ImageHeight = ((ImageH&0xff00)>>8) + ((ImageH&0x00ff)<<8) ;
#endif
	return iTRUE;

}

iCOLOR i51KitG2GetNewColorEx (iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha)
{
	/**
	* @fn		unsigned short calculate_alpha (unsigned short dpixel, unsigned short spixel, unsigned char alpha)
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


iBOOL i51KitG2DrawImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect)
{
	iU8 * ImageDataPoint = iNULL;
	iU32 ImageWidth = 0;
	iU32 ImageHeight = 0;
	iU16 * TempReadDataBuff = iNULL;
	iU32 CopyDataEntry = 0;
	// �ۼƶ�ȡ�������ݳ���
	iU32  LengthTotalReadData = 0;
	// һ����Ҫ����������������ɫ����
	iS32 LengthCopyColor = 0;
	// ͼƬ�ļ��ܳ���
	iU32 LengthIamgeData = 0;

	// ��ǰ���ݶ�����Ļ�ϵĿ�ʼ����λ��
	iS16 TempDrawX = 0;
	iS16 TempDrawY = 0;
	// ͼƬ���Ƶ����ϽǺ����½�����  
	iS16 SrcStartX = 0;
	iS16 SrcStartY = 0;
	iS16 SrcEndX = 0;
	iS16 SrcEndY = 0;
	// ��Ļ���Ƶ����ϽǺ����½�����  
	iS16 DesStartX = 0;
	iS16 DesStartY = 0;
	iS16 DesEndX = 0;
	iS16 DesEndY = 0;
	iS32 iTemp = 0;

	// ImageX
	iS32 CurrentImageX = 0;
	iS32 CurrentImageY = 0;
	iS32 CurrentImageWidth = 0;


	iU32 LcdBufferPostion = 0;

	// ������һ�������׵�ַ
	iU16 * TempReadDataBuff1 = iNULL;

	iU16 * ScreenBuffer = iNULL;
	iU32 * LcdInfoArr = iNULL;
	iS16 SrcW, SrcH, DesW, DesH;
	iS16 DrawW, DrawH;
	iU16 ScreenWidth = 0;
	iU16 ScreenHeight= 0;
	iU16 CurrColor = 0;
	iU16 TotalAlphaNumber = 0;

	ImageDataPoint = (iU8 *)ImageData;
	if( iNULL==ImageDataPoint ) return iFALSE;

	if( ImageDataPoint[0X11]!=0X02 )
	{
		////LOG("�ļ���ʽ��������������ͼƬ�ļ�!");
		return iFALSE;
	}


	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
	{
		return iFALSE;
	}

	LcdInfoArr =  i51AdeMmiGetPanel();
	if( iNULL == LcdInfoArr )
	{
		return iFALSE;
	}

	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;

	//    i51AdeStdMemcpy16(&ImageWidth, &ImageDataPoint[0X09], 2);
	//    i51AdeStdMemcpy16(&ImageHeight, &ImageDataPoint[0X0B], 2);
	//    i51AdeStdMemcpy16(&LengthIamgeData, &ImageDataPoint[0X05], 4);
	ImageWidth = ImageDataPoint[9]|(ImageDataPoint[10]<<8);
	ImageHeight = ImageDataPoint[11]|(ImageDataPoint[12]<<8);
	LengthIamgeData = ImageDataPoint[5]|(ImageDataPoint[6]<<8)|(ImageDataPoint[7]<<16)|(ImageDataPoint[8]<<24);
	TotalAlphaNumber = ImageDataPoint[18]|(ImageDataPoint[19]<<8);
	////LOG("ImageWidth=%d",ImageWidth);
	////LOG("ImageHeight=%d",ImageHeight);
	////LOG("LengthIamgeData=%d",LengthIamgeData);

	if(SrcRect)
	{
		SrcStartX = SrcRect->X;
		SrcStartY = SrcRect->Y;
		SrcW = SrcRect->Width;
		SrcH = SrcRect->Height;
	}
	else
	{
		SrcStartX = 0;
		SrcStartY = 0;
		SrcW = ImageWidth;
		SrcH = ImageHeight;
	}
	////LOG("SrcW=%d",SrcW);
	////LOG("SrcH=%d",SrcH);

	if( DesRect )
	{
		DesStartX= DesRect->X;
		DesStartY= DesRect->Y;
		DesW = DesRect->Width;
		DesH = DesRect->Height;
	}
	else
	{
		DesStartX= 0;
		DesStartY= 0;
		DesW = ScreenWidth;
		DesH = ScreenHeight;
	}

	if(SrcStartX<0)
	{
		SrcW += SrcStartX;
		DesW += SrcStartX;
		DesStartX -= SrcStartX;
		SrcStartX= 0;		
	}

	if(SrcStartY<0)
	{
		SrcH += SrcStartY;
		DesH += SrcStartY;
		DesStartY -= SrcStartY;
		SrcStartY= 0;		
	}

	if(DesStartX<0)
	{
		DesW += DesStartX;
		SrcW += DesStartX;
		SrcStartX -= DesStartX;
		DesStartX= 0;		
	}

	if(DesStartY<0)
	{
		DesH += DesStartY;
		SrcH += DesStartY;
		SrcStartY -= DesStartY;
		DesStartY= 0;		
	}

	if( SrcStartX+SrcW > ImageWidth) SrcW = ImageWidth - SrcStartX;
	if( SrcStartY+SrcH > ImageHeight) SrcH= ImageHeight- SrcStartY;

	if( DesStartX+DesW > ScreenWidth) DesW = ScreenWidth - DesStartX;
	if( DesStartY+DesH > ScreenHeight) DesH= ScreenHeight- DesStartY;

	if( SrcW<DesW ) DrawW = SrcW;
	else DrawW = DesW;

	if( SrcH<DesH ) DrawH = SrcH;
	else DrawH = DesH;

	ImageDataPoint += 0X14;
	LengthTotalReadData = 0X14;	
	SrcEndX = SrcStartX + DrawW;
	SrcEndY = SrcStartY + DrawH;
	DesEndX = DesStartX + DrawW;
	DesEndY = DesStartY + DrawH;

	if(SrcRect)
	{
		if( SrcStartX>=SrcRect->X + SrcRect->Width || SrcEndX<SrcRect->X )
		{
			return iTRUE;
		}
		if( SrcStartY>=SrcRect->Y + SrcRect->Height|| SrcEndY<SrcRect->Y )
		{
			return iTRUE;
		}
	}
	else
	{
		if( SrcStartX>=ImageWidth || SrcEndX<0 )
		{
			return iTRUE;
		}
		if( SrcStartY>=ImageHeight || SrcEndY<0 )
		{
			return iTRUE;
		}
	}

	////LOG("DesStartX=%d",DesStartX);
	////LOG("DesStartY=%d",DesStartY);
	////LOG("DesEndX=%d",DesEndX);
	////LOG("DesEndY=%d",DesEndY);

	while(1)
	{
		TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
		CurrentImageX = TempReadDataBuff1[0];
		CurrentImageY = TempReadDataBuff1[1];
		CurrentImageWidth = TempReadDataBuff1[2];
		//CurrentImageX = ImageDataPoint[1]|(ImageDataPoint[0]<<8);
		//CurrentImageY = ImageDataPoint[3]|(ImageDataPoint[2]<<8);
		//CurrentImageWidth = ImageDataPoint[5]|(ImageDataPoint[4]<<8);
		////LOG("CurrentImageX=%d",CurrentImageX);
		////LOG("CurrentImageY=%d",CurrentImageY);
		////LOG("CurrentImageWidth=%d",CurrentImageWidth);
		ImageDataPoint += 6;
		CopyDataEntry = (iU32)ImageDataPoint;
		LengthCopyColor =  CurrentImageWidth;

		LengthTotalReadData += 6 + (CurrentImageWidth<<1);
		ImageDataPoint += ( CurrentImageWidth<<1 );

		if( CurrentImageY>= SrcEndY) break;

		// ͼƬ��ǰ���ݶ�X��ʼ������ͼƬ���������ұ߻���Y������ͼƬ���������Ϸ�            
		if( (CurrentImageY < SrcStartY) || (CurrentImageX >= SrcEndX))
		{
			if( LengthTotalReadData> (LengthIamgeData-3) ) break;
			continue;
		}

		// ��ǰ���ݶγ��ȳ���ͼƬ���������ұ�
		if( (CurrentImageX+CurrentImageWidth) >SrcEndX )
		{
			LengthCopyColor -= ((CurrentImageX+CurrentImageWidth-SrcEndX));
		}

		// ͼƬ��ǰ���ݶ�X��ʼ������ͼƬ�����������            
		if( CurrentImageX<SrcStartX )
		{
			LengthCopyColor -= ((SrcStartX - CurrentImageX));
			CopyDataEntry += ((SrcStartX - CurrentImageX)<<1);
			if( LengthCopyColor<=0 ) 
			{
				if( LengthTotalReadData> (LengthIamgeData-3) ) break;
				continue;
			}
		}

		// ���㵱ǰ���ݶ�����Ļ�Ͽ�ʼ��ʾ��λ��
		TempDrawX = DesStartX;
		TempDrawY = DesStartY + CurrentImageY - SrcStartY;

		// ͼƬ��ǰ���ݶ�X��ʼ������ͼƬ��ʼ���ƻ��������ұ�
		// ��Ҫ�ƶ���Ļ��ʼ��������
		if( CurrentImageX>SrcStartX )
		{
			TempDrawX += ((CurrentImageX-SrcStartX));
		}

		if( TempDrawX >= DesEndX)
		{
			if( LengthTotalReadData> (LengthIamgeData-3) ) break;
			continue;
		}

		TempReadDataBuff = (iU16*)CopyDataEntry;
		i51AdeStdMemcpy16( &ScreenBuffer[ TempDrawY*ScreenWidth+TempDrawX], TempReadDataBuff, (LengthCopyColor<<1));
		if( LengthTotalReadData> (LengthIamgeData-3) ) break;

	}

	if(TotalAlphaNumber>0)
	{
		iS32 CurrAlphaNum = 0;
		iS32 CurrBuffPos = LengthIamgeData;		
		iS16 AlphaPosX = 0;
		iS16 AlphaPosY = 0;
		iALPHA AlphaNumber = 0;
		iS16 AlphaColor = 0;
		iS32 LcdBuffPos = 0;
		iU32 TempHeight = ImageHeight -1;
		iU32 TempWidth = ImageWidth-1;

		ImageDataPoint = (iU8*)((iU32)ImageData+CurrBuffPos + 12);

		for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
		{
			TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
			AlphaPosX = TempReadDataBuff1[0];
			AlphaPosY = TempReadDataBuff1[1];
			AlphaNumber = (iALPHA)TempReadDataBuff1[2];
			AlphaColor = TempReadDataBuff1[3];
			ImageDataPoint += 8;

			if(AlphaPosY>=SrcEndY) break;
			if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<SrcStartY) continue;				

			LcdBuffPos = (AlphaPosY+DesStartY)*ScreenWidth+AlphaPosX-SrcStartX+DesStartX;
			ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx1(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
		}
	}		
	return iTRUE;

}





iBOOL i51KitG2ImageDestory( iU32  SrcData)
{
	// author : Otto
	// since : 10.20 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// iU32 SrcData : ��Ҫ���ٵ�ͼƬ���ݵ�ַ

	// notes : ������i51KitG2ImageCreate����������ͼƬ

	if( iNULL == SrcData ) return iFALSE;

	i51AdeOsFree((void *)SrcData);
	SrcData = 0;
	return iTRUE;

}

iBOOL i51KitG2DrawLineEx( iS32 Point[][2], iCOLOR Color)
{

	// author : Otto
	// since : Aug 10 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// iU32 Point[][2] :
	//    Point[0][0]����Ҫ�����߶ε����X����
	//    Point[0][1]����Ҫ�����߶ε����Y����
	//    Point[1][0]����Ҫ�����߶ε��յ�X����
	//    Point[1][1]����Ҫ�����߶ε��յ�Y����
	// iCOLOR Color : �����߶ε���ɫ

	// notes : ����ɫColor����һ���ӵ�(Point[0][0], Point[0][1])����(Point[1][0], Point[1][1])���߶�
	// �����㷨ʹ�õ��ǶԳ�ֱ�������㷨���Գ�ֱ�������㷨��������һ����ʵ��ֱ�����е�Ϊ�磬�������ǶԳƵ�,
	// ���������������Գ��ԣ���Bresenham�㷨���иĽ���ʹ��ÿ����һ���жϾͿ������������ֱ���е������
	// �ԳƵ㡣���������ֱ�߾����������м�����

	iS32 StartX = Point[0][0];
	iS32 StartY = Point[0][1];
	iS32 EndX = Point[1][0];
	iS32 EndY = Point[1][1];

	iS32 TempX1, TempY1;	
	iS32 TempX2, TempY2;	

	//������յ�X��Y�Ĳ�ֵ
	iS32 dx = 0;
	iS32 dy = 0;
	iS32 dx2 = 0;
	iS32 dy2 = 0;

	// ������
	iS32 DecisionTag = 0;

	// ��Ҫ���ƴ���
	iU32 HalfLength = 0;

	//��ǰ����λ��
	iU32 Location = 0;

	//У׼Yֵ
	iS16 RegulationY = 1;

	// ��ǰ���Ƶ�Ԫ����VP�ϵ�ID
	iU16 CurrDrawID;

	// ������VPʵ�ʴ�С	
	iRECT * VPActualRect = iNULL;
	// ������VP����Ļ�ϵ����Ϻ���������
	iS32 VP_TOP_LEFT_X = 0;
	iS32 VP_TOP_LEFT_Y = 0;
	iS32 VP_LOVER_RIGHT_X = 0;
	iS32 VP_LOVER_RIGHT_Y = 0;
	// ��Ҫ���Ƶ�VP����	
	iRECT * VPDrawRect = iNULL;
	// ��������	
	iS16 DrawRectStartX = 0;
	iS16 DrawRectStartY = 0;
	iU16 ScreenWidth;
	iU16 ScreenHeight;
	iS16 DrawRectEndX;
	iS16 DrawRectEndY;
	iU32 BuffPos = 0;
	iS32 Temp1 = 0;
	iU16 * ScreenBuffer = iNULL;
	iU32 * LcdInfoArr = iNULL;

#if i51KITG2_DEBUG
	//LOG( "G2DrawLine Start , %x, %x", Point, Color) ;
#endif

	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
	{
		return iFALSE;
	}

	DrawRectEndX = ScreenWidth;
	DrawRectEndY = ScreenHeight;

	LcdInfoArr =  i51AdeMmiGetPanel();
	if( iNULL == LcdInfoArr )
	{
		return iFALSE;
	}

	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;

	for( Temp1=0; Temp1<2; Temp1++ )
	{
		if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
			||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
			)
		{
#if i51KITG2_DEBUG
			//LOG( "G2DrawLine Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
#endif
			return iFALSE;
		}
	}     	

	DrawRectStartX = 0;
	DrawRectStartY = 0;
	DrawRectEndX = ScreenWidth;
	DrawRectEndY = ScreenHeight;
	StartX = Point[0][0];
	StartY = Point[0][1];
	EndX = Point[1][0];
	EndY = Point[1][1];

	//  �����Ҫ���Ƶ��߶ε����˵㶼����Ļ�⣬������Ļͬ��
	//  ����Ƶ��߶�����Ļû�н���
	if( (StartX<0&&EndX<0)
		||(StartX>=ScreenWidth&&EndX>=ScreenWidth)
		||(StartY<0&&EndY<0)
		||(StartY>=ScreenHeight&&EndY>=ScreenHeight)
		)
	{
		return iTRUE;
	}

	//����յ����������򽻻����ǵ�λ��
	if ( EndX < StartX )
	{
		EndX += StartX;
		StartX = EndX - StartX;
		EndX = EndX - StartX;

		EndY += StartY;
		StartY = EndY - StartY;
		EndY = EndY - StartY;
	}

	//����������յ�X��Y�Ĳ�ֵ
	dx = EndX - StartX;
	if( EndY > StartY )
	{
		dy = EndY - StartY;
		RegulationY = 1;
	}
	else
	{
		dy = StartY - EndY;
		RegulationY = -1;
	}

	//������յ�X��Y�Ĳ�ֵ�Ŵ�һ��
	dx2 = dx << 1;
	dy2 = dy << 1;

	//����������ֵ
	if( dx>=dy){
		DecisionTag = dy2 - dx;
		HalfLength = (dx+1) >> 1;
	} else{
		DecisionTag = dy - dx2;
		HalfLength = (dy+1) >> 1;
	}

	for ( Location = 0; Location <= HalfLength; Location++)
	{
		TempX1 = StartX;
		TempY1 = StartY;
		TempX2 = EndX;
		TempY2 = EndY;

		if( TempX1>=DrawRectStartX && TempX1<DrawRectEndX && TempY1>=DrawRectStartY&& TempY1<DrawRectEndY )
		{
#if LCDBUFF_DEBUG
			i51KitG2CheckLcdbuffer(ScreenWidth*TempY1 + TempX1,1);
#endif
			ScreenBuffer[ (ScreenWidth*TempY1 + TempX1)] = Color;
		}

		if( TempX2>=DrawRectStartX && TempX2<DrawRectEndX && TempY2>=DrawRectStartY&& TempY2<DrawRectEndY )
		{
#if LCDBUFF_DEBUG
			i51KitG2CheckLcdbuffer(ScreenWidth*TempY2 + TempX2,1);
#endif
			ScreenBuffer[ (ScreenWidth*TempY2 + TempX2)] = Color;
		}

		if( dx>= dy ){
			// ��DecisionTag>0ʱ��ʼ��y���RegulationY��ĩ��y���RegulationY
			if (DecisionTag > 0)
			{
				StartY += RegulationY;
				EndY -= RegulationY;    
				DecisionTag -= dx2;
			}

			//ʼ��x���1��ĩ��x���1
			StartX++;    
			EndX--;
			DecisionTag += dy2;
		}
		else
		{
			// ��DecisionTag<0ʱ��ʼ��X���1��ĩ��X���1
			if (DecisionTag < 0)
			{
				StartX++;    
				EndX--;
				DecisionTag += dy2;
			}

			//ʼ��y���RegulationY��ĩ��y���RegulationY
			StartY += RegulationY;
			EndY -= RegulationY;          
			DecisionTag -= dx2;

		}
	}

#if i51KITG2_DEBUG
	//LOG( "G2DrawLine Start , %x, %x", Point, Color) ;
#endif

	return iTRUE;

}

iCOLOR i51KitG2GetNewColor (iCOLOR dpixel, iCOLOR spixel)
{
/**
 * @fn		unsigned short calculate_alpha (unsigned short dpixel, unsigned short spixel, unsigned char alpha)
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
	
    rs = (spixel) & 0XF800;
    gs = (spixel) & 0X07E0;
    bs = spixel & 0X1F;
                                
    rd = (dpixel) & 0XF800;
    gd = (dpixel) & 0X07E0;
    bd = dpixel & 0X1F;
                                
    rd = ((rd*AlphaDes+ rs * AlphaSrc)>>8)&0XF800;
    gd = ((gd*AlphaDes+ gs * AlphaSrc)>>8)&0X7E0;
    bd = ((bd*AlphaDes+ bs * AlphaSrc)>>8)&0X1F;
    return (bd +(gd)+ (rd ));

}

iBOOL i51KitG2CleanScreenEx(iCOLOR Color, iRECT *Rect)
{
	// author : Otto
	// since : Sep 27 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// iCOLOR Color : �����Ļ����ɫ
	//iRECT *Rect : �����Ļ������

	// Notes : ��ָ������ɫ����Ļ����

	iS32 CleanX, CleanY;
	iS32 CleanW, CleanH;
	iS32 LcdBufPos = 0;
	iS32 TempX, TempY;
	iU16 ScreenWidth;
	iU16 ScreenHeight;
	iU16 * ScreenBuffer = iNULL;
	iU32 * LcdInfoArr = iNULL;
	iU16 * OneRowColorBuf = iNULL;

#if i51KITG2_DEBUG
	//LOG( "G2CleanScreen Start , %x, %x", Color, Rect) ;
#endif

	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
	{
		return iFALSE;
	}

	LcdInfoArr =  i51AdeMmiGetPanel();
	if( iNULL == LcdInfoArr )
	{
		return iFALSE;
	}

	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;

	if( Rect)
	{
		CleanX = Rect->X;
		CleanY = Rect->Y;
		CleanW = Rect->Width;
		CleanH = Rect->Height;
	}
	else
	{
		CleanX = 0;
		CleanY = 0;
		CleanW = ScreenWidth;
		CleanH = ScreenHeight;
	}

	if(CleanX<0)
	{
		CleanW += CleanX;
		CleanX = 0;
	}

	if(CleanY<0)
	{
		CleanH += CleanH;
		CleanY = 0;
	}

	if( (CleanX+CleanW)>ScreenWidth) CleanW = ScreenWidth-CleanX;
	if( (CleanY+CleanH)>ScreenHeight) CleanH = ScreenHeight-CleanY;

	if( ScreenHeight>ScreenWidth)
	{
		OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenHeight<<1),  __FILE__ , __LINE__ );
	}
	else
	{
		OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenWidth<<1),  __FILE__ , __LINE__ );
	}

	for( TempX=0; TempX<CleanW ; TempX++)
	{
		OneRowColorBuf[TempX] = Color;
	}

	CleanW = (CleanW<<1);	
	LcdBufPos = CleanY*ScreenWidth + CleanX;
//	if(AlphaSys==i51KITG2_ALPHA_NONEED)
	{
		for( TempY=0; TempY<CleanH; TempY++)
		{
			i51AdeStdMemcpy16(&(ScreenBuffer[LcdBufPos]), OneRowColorBuf, CleanW);
			LcdBufPos += ScreenWidth;
		}
	}
#if 0
	else
	{
		for( TempY=0; TempY<CleanH; TempY++)
		{
			LcdBufPos = (CleanY+TempY)*ScreenWidth + CleanX;
			for( TempX=0; TempX<CleanW ; TempX++)
			{
				ScreenBuffer[LcdBufPos] = i51KitG2GetNewColor(ScreenBuffer[LcdBufPos], Color);
				LcdBufPos++;
			}
		}
	}
#endif
	if(OneRowColorBuf)
	{
		i51AdeOsFree(OneRowColorBuf);
		OneRowColorBuf = iNULL;
	}

#if i51KITG2_DEBUG
	//LOG( "G2CleanScreen End , %x, %x", Color, Rect) ;
#endif
	return iTRUE;

}
iCOLOR NewColorFunc (iCOLOR spixel, iALPHA SrcAlpha)
{
	/**
	* @fn		unsigned short calculate_alpha (unsigned short dpixel, unsigned short spixel, unsigned char alpha)
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

	rd = ((rs * SrcAlpha)>>8)&0XF800;
	gd = ((gs * SrcAlpha)>>8)&0X7E0;
	bd = ((bs * SrcAlpha)>>8)&0X1F;


	return (bd +(gd)+ (rd ));

}
iBOOL i51KitG2CleanScreenBaseColorEx(iCOLOR Color, iRECT *Rect,iALPHA alpha)
{
	// author : Otto
	// since : Sep 27 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// iCOLOR Color : �����Ļ����ɫ
	//iRECT *Rect : �����Ļ������

	// Notes : ��ָ������ɫ����Ļ����

	iS32 CleanX, CleanY;
	iS32 CleanW, CleanH;
	iS32 LcdBufPos = 0;
	iS32 TempX, TempY;
	iU16 ScreenWidth;
	iU16 ScreenHeight;
	iU16 * ScreenBuffer = iNULL;
	iU32 * LcdInfoArr = iNULL;
	iU16 * OneRowColorBuf = iNULL;
	iCOLOR TransedColor;
     
	TransedColor = NewColorFunc (Color, alpha);
#if i51KITG2_DEBUG
	//LOG( "G2CleanScreen Start , %x, %x", Color, Rect) ;
#endif

	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
	{
		return iFALSE;
	}

	LcdInfoArr =  i51AdeMmiGetPanel();
	if( iNULL == LcdInfoArr )
	{
		return iFALSE;
	}

	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;

	if( Rect)
	{
		CleanX = Rect->X;
		CleanY = Rect->Y;
		CleanW = Rect->Width;
		CleanH = Rect->Height;
	}
	else
	{
		CleanX = 0;
		CleanY = 0;
		CleanW = ScreenWidth;
		CleanH = ScreenHeight;
	}

	if(CleanX<0)
	{
		CleanW += CleanX;
		CleanX = 0;
	}

	if(CleanY<0)
	{
		CleanH += CleanH;
		CleanY = 0;
	}

	if( (CleanX+CleanW)>ScreenWidth) CleanW = ScreenWidth-CleanX;
	if( (CleanY+CleanH)>ScreenHeight) CleanH = ScreenHeight-CleanY;

	if( ScreenHeight>ScreenWidth)
	{
		OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenHeight<<1),  __FILE__ , __LINE__ );
	}
	else
	{
		OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenWidth<<1),  __FILE__ , __LINE__ );
	}

	for( TempX=0; TempX<CleanW ; TempX++)
	{
		//OneRowColorBuf[TempX] = Color;
		OneRowColorBuf[TempX] = TransedColor;
		//NewColorFunc
	}
///////////////

	//////////////
	CleanW = (CleanW<<1);	
	LcdBufPos = CleanY*ScreenWidth + CleanX;
	//	if(AlphaSys==i51KITG2_ALPHA_NONEED)
	{
		for( TempY=0; TempY<CleanH; TempY++)
		{
			i51AdeStdMemcpy16(&(ScreenBuffer[LcdBufPos]), OneRowColorBuf, CleanW);
			LcdBufPos += ScreenWidth;
		}
	}
#if 0
	else
	{
		for( TempY=0; TempY<CleanH; TempY++)
		{
			LcdBufPos = (CleanY+TempY)*ScreenWidth + CleanX;
			for( TempX=0; TempX<CleanW ; TempX++)
			{
				ScreenBuffer[LcdBufPos] = i51KitG2GetNewColor(ScreenBuffer[LcdBufPos], Color);
				LcdBufPos++;
			}
		}
	}
#endif
	if(OneRowColorBuf)
	{
		i51AdeOsFree(OneRowColorBuf);
		OneRowColorBuf = iNULL;
	}

#if i51KITG2_DEBUG
	//LOG( "G2CleanScreen End , %x, %x", Color, Rect) ;
#endif
	return iTRUE;

}

//iBOOL i51KitG2DrawLineEx( iS32 Point[][2], iCOLOR Color)
//{
//
//	// author : Otto
//	// since : Aug 10 2011
//	// (C) 2011 , PKIG Tech. Co., Ltd.
//
//	// iU32 Point[][2] :
//	//    Point[0][0]����Ҫ�����߶ε����X����
//	//    Point[0][1]����Ҫ�����߶ε����Y����
//	//    Point[1][0]����Ҫ�����߶ε��յ�X����
//	//    Point[1][1]����Ҫ�����߶ε��յ�Y����
//	// iCOLOR Color : �����߶ε���ɫ
//
//	// notes : ����ɫColor����һ���ӵ�(Point[0][0], Point[0][1])����(Point[1][0], Point[1][1])���߶�
//	// �����㷨ʹ�õ��ǶԳ�ֱ�������㷨���Գ�ֱ�������㷨��������һ����ʵ��ֱ�����е�Ϊ�磬�������ǶԳƵ�,
//	// ���������������Գ��ԣ���Bresenham�㷨���иĽ���ʹ��ÿ����һ���жϾͿ������������ֱ���е������
//	// �ԳƵ㡣���������ֱ�߾����������м�����
//
//	iS32 StartX = Point[0][0];
//	iS32 StartY = Point[0][1];
//	iS32 EndX = Point[1][0];
//	iS32 EndY = Point[1][1];
//
//	iS32 TempX1, TempY1;	
//	iS32 TempX2, TempY2;	
//
//	//������յ�X��Y�Ĳ�ֵ
//	iS32 dx = 0;
//	iS32 dy = 0;
//	iS32 dx2 = 0;
//	iS32 dy2 = 0;
//
//	// ������
//	iS32 DecisionTag = 0;
//
//	// ��Ҫ���ƴ���
//	iU32 HalfLength = 0;
//
//	//��ǰ����λ��
//	iU32 Location = 0;
//
//	//У׼Yֵ
//	iS16 RegulationY = 1;
//
//	// ��ǰ���Ƶ�Ԫ����VP�ϵ�ID
//	iU16 CurrDrawID;
//
//	// ������VPʵ�ʴ�С	
//	iRECT * VPActualRect = iNULL;
//	// ������VP����Ļ�ϵ����Ϻ���������
//	iS32 VP_TOP_LEFT_X = 0;
//	iS32 VP_TOP_LEFT_Y = 0;
//	iS32 VP_LOVER_RIGHT_X = 0;
//	iS32 VP_LOVER_RIGHT_Y = 0;
//	// ��Ҫ���Ƶ�VP����	
//	iRECT * VPDrawRect = iNULL;
//	// ��������	
//	iS16 DrawRectStartX = 0;
//	iS16 DrawRectStartY = 0;
//	iU16 ScreenWidth;
//	iU16 ScreenHeight;
//	iS16 DrawRectEndX;
//	iS16 DrawRectEndY;
//	iU32 BuffPos = 0;
//	iS32 Temp1 = 0;
//	iU16 * ScreenBuffer = iNULL;
//	iU32 * LcdInfoArr = iNULL;
//
//#if i51KITG2_DEBUG
//	//LOG( "G2DrawLine Start , %x, %x", Point, Color) ;
//#endif
//
//	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
//	{
//		return iFALSE;
//	}
//
//	DrawRectEndX = ScreenWidth;
//	DrawRectEndY = ScreenHeight;
//
//	LcdInfoArr =  i51AdeMmiGetPanel();
//	if( iNULL == LcdInfoArr )
//	{
//		return iFALSE;
//	}
//
//	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;
//
//	for( Temp1=0; Temp1<2; Temp1++ )
//	{
//		if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
//			||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
//			)
//		{
//#if i51KITG2_DEBUG
//			//LOG( "G2DrawLine Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
//#endif
//			return iFALSE;
//		}
//	}     	
//
//	DrawRectStartX = 0;
//	DrawRectStartY = 0;
//	DrawRectEndX = ScreenWidth;
//	DrawRectEndY = ScreenHeight;
//	StartX = Point[0][0];
//	StartY = Point[0][1];
//	EndX = Point[1][0];
//	EndY = Point[1][1];
//
//	//  �����Ҫ���Ƶ��߶ε����˵㶼����Ļ�⣬������Ļͬ��
//	//  ����Ƶ��߶�����Ļû�н���
//	if( (StartX<0&&EndX<0)
//		||(StartX>=ScreenWidth&&EndX>=ScreenWidth)
//		||(StartY<0&&EndY<0)
//		||(StartY>=ScreenHeight&&EndY>=ScreenHeight)
//		)
//	{
//		return iTRUE;
//	}
//
//	//����յ����������򽻻����ǵ�λ��
//	if ( EndX < StartX )
//	{
//		EndX += StartX;
//		StartX = EndX - StartX;
//		EndX = EndX - StartX;
//
//		EndY += StartY;
//		StartY = EndY - StartY;
//		EndY = EndY - StartY;
//	}
//
//	//����������յ�X��Y�Ĳ�ֵ
//	dx = EndX - StartX;
//	if( EndY > StartY )
//	{
//		dy = EndY - StartY;
//		RegulationY = 1;
//	}
//	else
//	{
//		dy = StartY - EndY;
//		RegulationY = -1;
//	}
//
//	//������յ�X��Y�Ĳ�ֵ�Ŵ�һ��
//	dx2 = dx << 1;
//	dy2 = dy << 1;
//
//	//����������ֵ
//	if( dx>=dy){
//		DecisionTag = dy2 - dx;
//		HalfLength = (dx+1) >> 1;
//	} else{
//		DecisionTag = dy - dx2;
//		HalfLength = (dy+1) >> 1;
//	}
//
//	for ( Location = 0; Location <= HalfLength; Location++)
//	{
//		TempX1 = StartX;
//		TempY1 = StartY;
//		TempX2 = EndX;
//		TempY2 = EndY;
//
//		if( TempX1>=DrawRectStartX && TempX1<DrawRectEndX && TempY1>=DrawRectStartY&& TempY1<DrawRectEndY )
//		{
//#if LCDBUFF_DEBUG
//			i51KitG2CheckLcdbuffer(ScreenWidth*TempY1 + TempX1,1);
//#endif
//			ScreenBuffer[ (ScreenWidth*TempY1 + TempX1)] = Color;
//		}
//
//		if( TempX2>=DrawRectStartX && TempX2<DrawRectEndX && TempY2>=DrawRectStartY&& TempY2<DrawRectEndY )
//		{
//#if LCDBUFF_DEBUG
//			i51KitG2CheckLcdbuffer(ScreenWidth*TempY2 + TempX2,1);
//#endif
//			ScreenBuffer[ (ScreenWidth*TempY2 + TempX2)] = Color;
//		}
//
//		if( dx>= dy ){
//			// ��DecisionTag>0ʱ��ʼ��y���RegulationY��ĩ��y���RegulationY
//			if (DecisionTag > 0)
//			{
//				StartY += RegulationY;
//				EndY -= RegulationY;    
//				DecisionTag -= dx2;
//			}
//
//			//ʼ��x���1��ĩ��x���1
//			StartX++;    
//			EndX--;
//			DecisionTag += dy2;
//		}
//		else
//		{
//			// ��DecisionTag<0ʱ��ʼ��X���1��ĩ��X���1
//			if (DecisionTag < 0)
//			{
//				StartX++;    
//				EndX--;
//				DecisionTag += dy2;
//			}
//
//			//ʼ��y���RegulationY��ĩ��y���RegulationY
//			StartY += RegulationY;
//			EndY -= RegulationY;          
//			DecisionTag -= dx2;
//
//		}
//	}
//
//#if i51KITG2_DEBUG
//	//LOG( "G2DrawLine Start , %x, %x", Point, Color) ;
//#endif
//
//	return iTRUE;
//
//}
//iBOOL i51KitG2DrawLineEx( iS32 Point[][2], iCOLOR Color)
//{
//
//	// author : Otto
//	// since : Aug 10 2011
//	// (C) 2011 , PKIG Tech. Co., Ltd.
//
//	// iU32 Point[][2] :
//	//    Point[0][0]����Ҫ�����߶ε����X����
//	//    Point[0][1]����Ҫ�����߶ε����Y����
//	//    Point[1][0]����Ҫ�����߶ε��յ�X����
//	//    Point[1][1]����Ҫ�����߶ε��յ�Y����
//	// iCOLOR Color : �����߶ε���ɫ
//
//	// notes : ����ɫColor����һ���ӵ�(Point[0][0], Point[0][1])����(Point[1][0], Point[1][1])���߶�
//	// �����㷨ʹ�õ��ǶԳ�ֱ�������㷨���Գ�ֱ�������㷨��������һ����ʵ��ֱ�����е�Ϊ�磬�������ǶԳƵ�,
//	// ���������������Գ��ԣ���Bresenham�㷨���иĽ���ʹ��ÿ����һ���жϾͿ������������ֱ���е������
//	// �ԳƵ㡣���������ֱ�߾����������м�����
//
//	iS32 StartX = Point[0][0];
//	iS32 StartY = Point[0][1];
//	iS32 EndX = Point[1][0];
//	iS32 EndY = Point[1][1];
//
//	iS32 TempX1, TempY1;	
//	iS32 TempX2, TempY2;	
//
//	//������յ�X��Y�Ĳ�ֵ
//	iS32 dx = 0;
//	iS32 dy = 0;
//	iS32 dx2 = 0;
//	iS32 dy2 = 0;
//
//	// ������
//	iS32 DecisionTag = 0;
//
//	// ��Ҫ���ƴ���
//	iU32 HalfLength = 0;
//
//	//��ǰ����λ��
//	iU32 Location = 0;
//
//	//У׼Yֵ
//	iS16 RegulationY = 1;
//
//	// ��ǰ���Ƶ�Ԫ����VP�ϵ�ID
//	iU16 CurrDrawID;
//
//	// ������VPʵ�ʴ�С	
//	iRECT * VPActualRect = iNULL;
//	// ������VP����Ļ�ϵ����Ϻ���������
//	iS32 VP_TOP_LEFT_X = 0;
//	iS32 VP_TOP_LEFT_Y = 0;
//	iS32 VP_LOVER_RIGHT_X = 0;
//	iS32 VP_LOVER_RIGHT_Y = 0;
//	// ��Ҫ���Ƶ�VP����	
//	iRECT * VPDrawRect = iNULL;
//	// ��������	
//	iS16 DrawRectStartX = 0;
//	iS16 DrawRectStartY = 0;
//	iU16 ScreenWidth;
//	iU16 ScreenHeight;
//	iS16 DrawRectEndX;
//	iS16 DrawRectEndY;
//	iU32 BuffPos = 0;
//	iS32 Temp1 = 0;
//	iU16 * ScreenBuffer = iNULL;
//	iU32 * LcdInfoArr = iNULL;
//
//#if i51KITG2_DEBUG
//	iLog( "G2DrawLine Start , %x, %x", Point, Color) ;
//#endif
//
//	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
//	{
//		return iFALSE;
//	}
//
//	DrawRectEndX = ScreenWidth;
//	DrawRectEndY = ScreenHeight;
//
//	LcdInfoArr =  i51AdeMmiGetPanel();
//	if( iNULL == LcdInfoArr )
//	{
//		return iFALSE;
//	}
//
//	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;
//
//	for( Temp1=0; Temp1<2; Temp1++ )
//	{
//		if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
//			||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
//			)
//		{
//#if i51KITG2_DEBUG
//			iLog( "G2DrawLine Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
//#endif
//			return iFALSE;
//		}
//	}     	
//
//	DrawRectStartX = 0;
//	DrawRectStartY = 0;
//	DrawRectEndX = ScreenWidth;
//	DrawRectEndY = ScreenHeight;
//	StartX = Point[0][0];
//	StartY = Point[0][1];
//	EndX = Point[1][0];
//	EndY = Point[1][1];
//
//	//  �����Ҫ���Ƶ��߶ε����˵㶼����Ļ�⣬������Ļͬ��
//	//  ����Ƶ��߶�����Ļû�н���
//	if( (StartX<0&&EndX<0)
//		||(StartX>=ScreenWidth&&EndX>=ScreenWidth)
//		||(StartY<0&&EndY<0)
//		||(StartY>=ScreenHeight&&EndY>=ScreenHeight)
//		)
//	{
//		return iTRUE;
//	}
//
//	//����յ����������򽻻����ǵ�λ��
//	if ( EndX < StartX )
//	{
//		EndX += StartX;
//		StartX = EndX - StartX;
//		EndX = EndX - StartX;
//
//		EndY += StartY;
//		StartY = EndY - StartY;
//		EndY = EndY - StartY;
//	}
//
//	//����������յ�X��Y�Ĳ�ֵ
//	dx = EndX - StartX;
//	if( EndY > StartY )
//	{
//		dy = EndY - StartY;
//		RegulationY = 1;
//	}
//	else
//	{
//		dy = StartY - EndY;
//		RegulationY = -1;
//	}
//
//	//������յ�X��Y�Ĳ�ֵ�Ŵ�һ��
//	dx2 = dx << 1;
//	dy2 = dy << 1;
//
//	//����������ֵ
//	if( dx>=dy){
//		DecisionTag = dy2 - dx;
//		HalfLength = (dx+1) >> 1;
//	} else{
//		DecisionTag = dy - dx2;
//		HalfLength = (dy+1) >> 1;
//	}
//
//	for ( Location = 0; Location <= HalfLength; Location++)
//	{
//		TempX1 = StartX;
//		TempY1 = StartY;
//		TempX2 = EndX;
//		TempY2 = EndY;
//
//		if( TempX1>=DrawRectStartX && TempX1<DrawRectEndX && TempY1>=DrawRectStartY&& TempY1<DrawRectEndY )
//		{
//#if LCDBUFF_DEBUG
//			i51KitG2CheckLcdbuffer(ScreenWidth*TempY1 + TempX1,1);
//#endif
//			ScreenBuffer[ (ScreenWidth*TempY1 + TempX1)] = Color;
//		}
//
//		if( TempX2>=DrawRectStartX && TempX2<DrawRectEndX && TempY2>=DrawRectStartY&& TempY2<DrawRectEndY )
//		{
//#if LCDBUFF_DEBUG
//			i51KitG2CheckLcdbuffer(ScreenWidth*TempY2 + TempX2,1);
//#endif
//			ScreenBuffer[ (ScreenWidth*TempY2 + TempX2)] = Color;
//		}
//
//		if( dx>= dy ){
//			// ��DecisionTag>0ʱ��ʼ��y���RegulationY��ĩ��y���RegulationY
//			if (DecisionTag > 0)
//			{
//				StartY += RegulationY;
//				EndY -= RegulationY;    
//				DecisionTag -= dx2;
//			}
//
//			//ʼ��x���1��ĩ��x���1
//			StartX++;    
//			EndX--;
//			DecisionTag += dy2;
//		}
//		else
//		{
//			// ��DecisionTag<0ʱ��ʼ��X���1��ĩ��X���1
//			if (DecisionTag < 0)
//			{
//				StartX++;    
//				EndX--;
//				DecisionTag += dy2;
//			}
//
//			//ʼ��y���RegulationY��ĩ��y���RegulationY
//			StartY += RegulationY;
//			EndY -= RegulationY;          
//			DecisionTag -= dx2;
//
//		}
//	}
//
//#if i51KITG2_DEBUG
//	iLog( "G2DrawLine Start , %x, %x", Point, Color) ;
//#endif
//
//	return iTRUE;
//
//}