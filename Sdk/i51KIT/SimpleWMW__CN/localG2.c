#include "localG2.h"
#include "i51KitAde.h"

iU8 LocalG2AlphaSys = 0XFF;

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
    iU32   SrcDataLength = iNULL;
    iU32 SrcPos = 0;
    iU32 DesPos = 0;	
    iU32 iTemp = 0;
    iU32 DesTempPos = 0;
    // ͼƬ����ɫ��
    iU8 PaletteImage = iFALSE;
    // ��ɫ������ɫ����
    iU16 PaletteColorNum = 0;
    // ��ɫ����ɫ������
    iCOLOR * PaletteColorMem = iNULL;
    // һ�����ݽ������ʱ��Ŵ�
    iU8  * TempDesData = iNULL;
    // һ�����ݿ�ĳ���	:=6+���������
    iS32 BlockLength = 0;
    // ��ǰ���ݿ�λ��
    iS32 BlockPlace = 0;
    // ��ɫ��ʱ����	
    iCOLOR TempColor = 0;
    // ��ɫָ���ַ
    iCOLOR * ColorPoint = iNULL;
    // ������ͷ	
    iU8 TempHead[6];
    iS32 Temp = 0;		
    // ǰ���ĸ��ֽ��Ǳ����ͼƬ��Ⱥ͸߶�	
    iU8 * SrcDataPoint = iNULL;// = (iU8 * )(SrcData+4);
    iU16 ImageW = 0;
    iU16 ImageH = 0;	


    if( SrcData==iNULL ) return iFALSE;
	
    SrcDataPoint = (iU8 * )(SrcData);
    ImageW = 	SrcDataPoint[0]|(SrcDataPoint[1]<<8);
    ImageH = 	SrcDataPoint[2]|(SrcDataPoint[3]<<8);
	
    // ǰ���ĸ��ֽ��Ǳ����ͼƬ��Ⱥ͸߶�	
    SrcDataPoint = (iU8 * )(SrcData+4);
    DesDataLength = SrcDataPoint[4]|(SrcDataPoint[5]<<8)|(SrcDataPoint[6]<<16)|(SrcDataPoint[7]<<24);
    SrcDataLength = SrcDataPoint[8]|(SrcDataPoint[9]<<8)|(SrcDataPoint[10]<<16)|(SrcDataPoint[11]<<24);
    NumberParts = SrcDataPoint[16]|(SrcDataPoint[17]<<8);

    DesDataPoint = ( iU8 *)i51AdeOsMalloc( DesDataLength,  __FILE__ , __LINE__ );
    if( iNULL == DesDataPoint )
    {
        iLog("KIT - G2 : image create malloc des");
        goto ERROR;
    }

    if( NumberParts==1 && DesDataLength+514<32768 )
    {
        TempDesData = ( iU8 *)i51AdeOsMalloc( DesDataLength+514,  __FILE__ , __LINE__ );
    }
    else
    {
        TempDesData = ( iU8 *)i51AdeOsMalloc( 32768,  __FILE__ , __LINE__ );
    }
    if( iNULL == TempDesData )
    {
        iLog("KIT - G2 : image create malloc temp");
        goto ERROR;
    }

    SrcPos = 18;
    DesPos = 0;
	
    for( iTemp=0; iTemp<NumberParts; iTemp++ )
    {
        LengthPartSrc = SrcDataPoint[SrcPos]|(SrcDataPoint[SrcPos+1]<<8)|(SrcDataPoint[SrcPos+2]<<16)|(SrcDataPoint[SrcPos+3]<<24);
        SrcPos += 4;
		
        LengthPartDes = 32768;

        FlagReturn = i51AdeStdUncompress( TempDesData, &LengthPartDes, &SrcDataPoint[SrcPos], LengthPartSrc);
        if( iFALSE == FlagReturn ) goto ERROR;

        if( iTemp==0 )
        {
            if( TempDesData[4]>0X40)
            {
                PaletteImage = iTRUE;
                PaletteColorNum = (TempDesData[20]) | (TempDesData[21]<<8); 
                PaletteColorMem = i51AdeOsMalloc(PaletteColorNum*2, __FILE__ , __LINE__ );
                if ( iNULL==PaletteColorMem )
                {
                    iLog("KIT - G2 : mage create malloc PaletteColor" );
                    goto ERROR;
                }
                i51AdeStdMemcpy16(PaletteColorMem, &(TempDesData[22]), PaletteColorNum*2 );				
            }
        }
		
        SrcPos += LengthPartSrc;
        if( iFALSE==PaletteImage )
        {
//            DesTempPos = DesPos;
            i51AdeStdMemcpy16(&(DesDataPoint[DesPos]), TempDesData, LengthPartDes);				
            DesPos += LengthPartDes;
        }
        else
        {
            //���������ͼƬ��Ҫ�Ȱ�����תΪ��ɫ
            if( iTemp==0 )
            {
                i51AdeStdMemcpy16(DesDataPoint,TempDesData, 20);				
                DesTempPos = 20 + PaletteColorNum*2 +2;
                DesPos = 20;
                BlockLength = 0;
                BlockPlace = 0;
            }
            else
            {
                DesTempPos = 0;
            }

            while(DesTempPos<LengthPartDes)
            {
                if( BlockPlace==0 )
                {
                    // �����¿�ʼ
                    if( (LengthPartDes-DesTempPos)>=6 )
                    {
                        // ��ʣ�����ݳ��ȴ��ڵ��ڿ�ͷ����
                        BlockPlace = 6;
                        BlockLength = TempDesData[DesTempPos+4] | (TempDesData[DesTempPos+5]<<8);
                        i51AdeStdMemcpy16(&DesDataPoint[DesPos], &TempDesData[DesTempPos], 6);
                        i51AdeStdMemcpy16(TempHead, &TempDesData[DesTempPos], 6);
                        DesTempPos += 6;
                        DesPos += 6;
                    }
                    else
                    {
                        // ��ʣ������С�ڿ�ͷ���ȣ���ʣ�����ݱ���
                        BlockPlace = LengthPartDes-DesTempPos;
                        for( Temp = 0; Temp < BlockPlace; Temp++ )
                        {
                            TempHead[Temp] = TempDesData[DesTempPos+Temp];
                        }
                        DesTempPos = 0;						
                        break; 						
                    }
                }
                else if( BlockPlace<6 )				
                {
                    for( Temp = BlockPlace; Temp < 6; Temp++ )
                    {
                        TempHead[Temp] = TempDesData[DesTempPos+Temp-BlockPlace];
                    }
                    //i51AdeStdMemcpy16(&TempDesData[DesPos], TempHead, 6);
                    i51AdeStdMemcpy16(&DesDataPoint[DesPos], TempHead, 6);
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    DesTempPos += 6-BlockPlace;
                    DesPos += 6;
                    BlockPlace = 6;
                }
                else
                {
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    BlockLength -= BlockPlace - 6;
                }

                ColorPoint = (iCOLOR*)(((iU32)DesDataPoint)+DesPos);

                if( DesTempPos+BlockLength > LengthPartDes)
                {
                    BlockLength = LengthPartDes - DesTempPos;
                    BlockPlace = 6 + BlockLength;
                }
                else				
                {
                    BlockPlace = 0;
                }
				
                for( Temp=0; Temp<BlockLength; Temp++ )
                {
                    ColorPoint[Temp] = PaletteColorMem[TempDesData[DesTempPos]];
                    DesTempPos++;
                    DesPos += 2;
                }
            	}
     }

		
    }

    if(PaletteColorMem)
    {
        i51AdeOsFree(PaletteColorMem);
        PaletteColorMem = iNULL;
    }
 	
    if(TempDesData)
    {
        i51AdeOsFree(TempDesData);
        TempDesData = iNULL;
    }
	
    if(i51AdeCnfGetEndian()==1)
    {
        DesTempPos = 0X14; 
        while(DesTempPos<DesPos)
        {
            DesDataPoint[DesTempPos] += DesDataPoint[DesTempPos+1];
            DesDataPoint[DesTempPos+1] = DesDataPoint[DesTempPos] - DesDataPoint[DesTempPos+1];
            DesDataPoint[DesTempPos] = DesDataPoint[DesTempPos] - DesDataPoint[DesTempPos+1];
            DesTempPos += 2;
        }
    }

    return (iU32)(DesDataPoint);

 ERROR:

    if(PaletteColorMem)
    {
        i51AdeOsFree(PaletteColorMem);
        PaletteColorMem = iNULL;
    }
 	
    if(TempDesData)
    {
        i51AdeOsFree(TempDesData);
        TempDesData = iNULL;
    }

    if(DesDataPoint)
    {
        i51AdeOsFree(DesDataPoint);
        DesDataPoint = iNULL;
    }

    iLog("KIT - G2 : create image ");
	
    return iFALSE;
	
}


iU32 i51KitG2GetImageAttrEx(iU32  SrcData, iU32 * ImageWidth, iU32 * ImageHeight)
{
	// ǰ���ĸ��ֽ��Ǳ����ͼƬ��Ⱥ͸߶�	
	iU8 * SrcDataPoint = (iU8 * )(SrcData);
	iU16 ImageW = 0;
	iU16 ImageH = 0;	

	if( SrcData==iNULL ) return iFALSE;

#if 0
	i51AdeStdMemcpy16(&ImageW, &SrcDataPoint[0], 2);
	i51AdeStdMemcpy16(&ImageH, &SrcDataPoint[2], 2);

#ifndef __SPR__
	*ImageWidth = ImageW;   //С��
	*ImageHeight = ImageH;
#else
	*ImageWidth = ((ImageW&0xff00)>>8) + ((ImageW&0x00ff)<<8) ;//���չѶ
	*ImageHeight = ((ImageH&0xff00)>>8) + ((ImageH&0x00ff)<<8) ;
#endif
#else
	*ImageWidth = (SrcDataPoint[0]) + (SrcDataPoint[1]<<8) ;
	*ImageHeight = (SrcDataPoint[2]) + (SrcDataPoint[3]<<8) ;
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
	iS32 Temp = 0;
	iU32 TempScreenBufferPos = 0;

	ImageDataPoint = (iU8 *)ImageData;
	if( iNULL==ImageDataPoint ) return iFALSE;

	if( ImageDataPoint[0X11]!=0X02 )
	{
		iLog("�ļ���ʽ��������������ͼƬ�ļ�!");
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
	//iLog("ImageWidth=%d",ImageWidth);
	//iLog("ImageHeight=%d",ImageHeight);
	//iLog("LengthIamgeData=%d",LengthIamgeData);

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
	//iLog("SrcW=%d",SrcW);
	//iLog("SrcH=%d",SrcH);

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

	if(LocalG2AlphaSys==0XFF)
	{
		while(1)
		{
			TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
			CurrentImageX = TempReadDataBuff1[0];
			CurrentImageY = TempReadDataBuff1[1];
			CurrentImageWidth = TempReadDataBuff1[2];
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

				LcdBuffPos = (AlphaPosY-SrcStartY+DesStartY)*ScreenWidth+AlphaPosX-SrcStartX+DesStartX;
				ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx1(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
			}
		}	
	}
	else
	{
		while(1)
		{
			TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
			CurrentImageX = TempReadDataBuff1[0];
			CurrentImageY = TempReadDataBuff1[1];
			CurrentImageWidth = TempReadDataBuff1[2];
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
			TempScreenBufferPos = TempDrawY*ScreenWidth+TempDrawX;
			for( Temp = 0; Temp<LengthCopyColor; Temp++ )	
			{
				ScreenBuffer[TempScreenBufferPos] = i51KitG2GetNewColorEx1(ScreenBuffer[TempScreenBufferPos], TempReadDataBuff[Temp], LocalG2AlphaSys);
				TempScreenBufferPos++;
			}
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

				LcdBuffPos = (AlphaPosY-SrcStartY+DesStartY)*ScreenWidth+AlphaPosX-SrcStartX+DesStartX;
				ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx1(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber*LocalG2AlphaSys/255);
			}
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


iBOOL i51KitG2MirrorImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect)
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
		iLog("�ļ���ʽ��������������ͼƬ�ļ�!");
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

	ImageWidth = ImageDataPoint[9]|(ImageDataPoint[10]<<8);
	ImageHeight = ImageDataPoint[11]|(ImageDataPoint[12]<<8);
	LengthIamgeData = ImageDataPoint[5]|(ImageDataPoint[6]<<8)|(ImageDataPoint[7]<<16)|(ImageDataPoint[8]<<24);
	TotalAlphaNumber = ImageDataPoint[18]|(ImageDataPoint[19]<<8);

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
		DesStartX -= SrcStartX;
		SrcStartX= 0;		
	}

	if(SrcStartY<0)
	{
		SrcH += SrcStartY;
		DesStartY -= SrcStartY;
		SrcStartY= 0;		
	}

	if(DesStartX<0)
	{
		DesW += DesStartX;
		SrcStartX -= DesStartX;
		DesStartX= 0;		
	}

	if(DesStartY<0)
	{
		DesH += DesStartY;
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

	SrcStartX = ImageWidth - SrcStartX - DrawW;
	SrcEndX = SrcStartX + DrawW;

	while(1)
	{
		TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
		CurrentImageX = TempReadDataBuff1[0];
		CurrentImageY = TempReadDataBuff1[1];
		CurrentImageWidth = TempReadDataBuff1[2];

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

		CurrentImageX = ImageWidth - CurrentImageX - CurrentImageWidth;

		// ��ǰ���ݶγ��ȳ���ͼƬ���������ұ�
		if( (CurrentImageX+CurrentImageWidth) >SrcEndX )
		{
			LengthCopyColor -= ((CurrentImageX+CurrentImageWidth-SrcEndX));
		}

		// ͼƬ��ǰ���ݶ�X��ʼ������ͼƬ�����������            
		if( CurrentImageX<SrcStartX )
		{
			LengthCopyColor -= ((SrcStartX - CurrentImageX));
			//            CopyDataEntry += ((SrcStartX - CurrentImageX)<<1);
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
		LcdBufferPostion = TempDrawY*ScreenWidth+TempDrawX + LengthCopyColor-1;
		for( iTemp = 0; iTemp <LengthCopyColor; iTemp++ )
		{
			ScreenBuffer[LcdBufferPostion--] = TempReadDataBuff[iTemp];
		}

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

			AlphaPosX = TempWidth - AlphaPosX ;

			if(AlphaPosY>=SrcEndY) break;
			if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<SrcStartY) continue;				

			LcdBuffPos = (AlphaPosY+DesStartY)*ScreenWidth+AlphaPosX-SrcStartX+DesStartX;
			ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
		}
	}		
	return iTRUE;

}


iBOOL i51KitG2FlipImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect)
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
		iLog("�ļ���ʽ��������������ͼƬ�ļ�!");
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

	ImageWidth = ImageDataPoint[9]|(ImageDataPoint[10]<<8);
	ImageHeight = ImageDataPoint[11]|(ImageDataPoint[12]<<8);
	LengthIamgeData = ImageDataPoint[5]|(ImageDataPoint[6]<<8)|(ImageDataPoint[7]<<16)|(ImageDataPoint[8]<<24);
	TotalAlphaNumber = ImageDataPoint[18]|(ImageDataPoint[19]<<8);

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
		DesStartX -= SrcStartX;
		SrcStartX= 0;		
	}

	if(SrcStartY<0)
	{
		SrcH += SrcStartY;
		DesStartY -= SrcStartY;
		SrcStartY= 0;		
	}

	if(DesStartX<0)
	{
		DesW += DesStartX;
		SrcStartX -= DesStartX;
		DesStartX= 0;		
	}

	if(DesStartY<0)
	{
		DesH += DesStartY;
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

	SrcStartY = ImageHeight - SrcStartY - DrawH;
	SrcEndY = SrcStartY+ DrawH;

	while(1)
	{
		TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
		CurrentImageX = TempReadDataBuff1[0];
		CurrentImageY = TempReadDataBuff1[1];
		CurrentImageWidth = TempReadDataBuff1[2];

		ImageDataPoint += 6;
		CopyDataEntry = (iU32)ImageDataPoint;
		LengthCopyColor =  CurrentImageWidth;

		LengthTotalReadData += 6 + (CurrentImageWidth<<1);
		ImageDataPoint += ( CurrentImageWidth<<1 );

		CurrentImageY = ImageHeight -1 - CurrentImageY ;

		// ͼƬ��ǰ���ݶ�X��ʼ������ͼƬ���������ұ߻���Y������ͼƬ���������Ϸ�            
		if( (CurrentImageY < SrcStartY) || (CurrentImageY >= SrcEndY) || (CurrentImageX >= SrcEndX))
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

		if( TempDrawY > DesEndY ) break;
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
			AlphaPosY = TempHeight - AlphaPosY ;

			LcdBuffPos = (AlphaPosY+DesStartY)*ScreenWidth+AlphaPosX-SrcStartX+DesStartX;
			ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
		}
	}		
	return iTRUE;

}

iS32 i51KitG2SinEx( iS32 Angle)
{
	//iS32 Angle: ��Ҫ����ĽǶ�ֵ
	// notes : ���ؽǶ�Angle�������Ŵ�16384��(�൱������14λ)��ֵ
	iS32 ActualAngle = 0;
	iS32 Ret = 0;

	ActualAngle = Angle%360;
	if( ActualAngle<0) ActualAngle = 360+ActualAngle;

	if(ActualAngle<=90) Ret = SinData[ActualAngle];
	else if ( ActualAngle<=180) Ret = SinData[180-ActualAngle];
	else if ( ActualAngle<=270) Ret = -SinData[ActualAngle-180];
	else Ret = -SinData[360-ActualAngle];

	return Ret;

}

iS32 i51KitG2CosEx( iS32 Angle)
{
	//iS32 Angle: ��Ҫ����ĽǶ�ֵ
	// notes : ���ؽǶ�Angle�������Ŵ�10000����ֵ
	return i51KitG2SinEx(Angle+90);
}

iBOOL i51KitG2RotatePointEx(iS32 SrcX, iS32 SrcY, iS32 RotateCenterX,  iS32 RotateCenterY ,iS32 * DesX, iS32 * DesY, iS32 Angle)
{
	// A:ԭʼ����ԭ��֮�����X��������ļн�
	// B:ԭʼ���Ŀ�ĵ���ԭ��֮��ļн�,��Angle
	// C:Ŀ�ĵ���ԭ��֮�����X��������ļн�

	iS32 SinA = 0, SinB = 0, SinC = 0;
	iS32 CosA = 0, CosB = 0, CosC = 0;
	iS32 CenterX = RotateCenterX;
	iS32 CenterY = RotateCenterY;

	CosA = SrcX - CenterX;
	SinA = SrcY-CenterY;

	Angle = -Angle;
	SinB = i51KitG2SinEx(Angle);
	CosB = i51KitG2CosEx(Angle);

	SinC = SinA*CosB - CosA*SinB;
	CosC = CosA*CosB + SinA*SinB;

	*DesY = ((SinC+(CenterY<<14))>>14);
	*DesX = ((CosC+(CenterX<<14))>>14);

	//    *DesX = (((SrcX-CenterX)*CosB+(SrcY-CenterY)*SinB)>>14) + CenterX;
	//    *DesY = (((CenterX-SrcX)*SinB+(SrcY-CenterY)*CosB)>>14) + CenterY;
	return iTRUE;

}

iBOOL i51KitG2GetGirthPointsEx(iRECT SrcRect, iRECT DesRect, iS16 PointS[][2], iS32 SrcMoveX, iS32 SrcMoveY,
									  iS32 RotateCenterX,  iS32 RotateCenterY ,iU32 RotateType, iS32 Angle )
{
	// iRECT SrcRect : ��Ҫ�任������
	// iS16 PointS[][2] : �任���ܱߵĵ�������
	// iS32 Angle :��ת�Ƕ�
#define IMAGE_STRETCH 0
	// ��ת�������Ͻ�����
	iS32 UpLeftX = 0, UpLeftY = 0;
	// ��ǰ��ת������	 
	iS32 SrcX= 0, SrcY = 0;
	iS32 SrcX1= 0, SrcY1 = 0;
	// ��ǰ����ת������
	iS32 DesX = 0, DesY = 0;
	// ����ת����ĳ���	
	iS32 SrcW, SrcH;
	iS32 Location = 0;
	iS32 EdgePoints[4][2] = {0};
	iS32 DesStartX = 0, DesStartY = 0;
	iS32 DesEndX = 0, DesEndY = 0;
#if IMAGE_STRETCH
	iS32 DesPoints[5][2] = {0};
	iS32 OldX = 0, OldY = 0;
	iS32 iTemp = 0, Min = 0, Num = 0 , Temp = 0;
#endif

	//    UpLeftX = SrcRect.X + SrcMoveX;
	//    UpLeftY = SrcRect.Y + SrcMoveY;
	UpLeftX = SrcMoveX;
	UpLeftY = SrcMoveY;
	SrcW = SrcRect.Width;
	SrcH = SrcRect.Height;

	DesStartX = DesRect.X;
	DesStartY = DesRect.Y;
	DesEndX = DesRect.Width + DesStartX;
	DesEndY = DesRect.Height + DesStartY;

	i51KitG2RotatePointEx(UpLeftX, UpLeftY, RotateCenterX, RotateCenterY, &EdgePoints[0][0], &EdgePoints[0][1], Angle);
	i51KitG2RotatePointEx(UpLeftX+SrcW, UpLeftY, RotateCenterX, RotateCenterY, &EdgePoints[1][0], &EdgePoints[1][1], Angle);
	i51KitG2RotatePointEx(UpLeftX, UpLeftY+SrcH, RotateCenterX, RotateCenterY, &EdgePoints[2][0], &EdgePoints[2][1], Angle);
	i51KitG2RotatePointEx(UpLeftX+SrcW, UpLeftY+SrcH, RotateCenterX, RotateCenterY, &EdgePoints[3][0], &EdgePoints[3][1], Angle);

	if( EdgePoints[0][0]<DesStartX&&EdgePoints[1][0]<DesStartX&&EdgePoints[2][0]<DesStartX&&EdgePoints[3][0]<DesStartX) return iFALSE;
	if( EdgePoints[0][1]<DesStartY&&EdgePoints[1][1]<DesStartY&&EdgePoints[2][1]<DesStartY&&EdgePoints[3][1]<DesStartY) return iFALSE;
	if( EdgePoints[0][0]>DesEndX&&EdgePoints[1][0]>DesEndX&&EdgePoints[2][0]>DesEndX&&EdgePoints[3][0]>DesEndX) return iFALSE;
	if( EdgePoints[0][1]>DesEndY&&EdgePoints[1][1]>DesEndY&&EdgePoints[2][1]>DesEndY&&EdgePoints[3][1]>DesEndY) return iFALSE;

	SrcX = UpLeftX;
	SrcY = UpLeftY;

	for ( Location = 0; Location <SrcW; Location++)
	{
		i51KitG2RotatePointEx(SrcX, SrcY, RotateCenterX, RotateCenterY, &DesX, &DesY, Angle);
		PointS[Location][0] = DesX-SrcX;
		PointS[Location][1] = DesY-SrcY;
		SrcX++;
	}	

	SrcH = SrcW + SrcH;
	SrcX = UpLeftX;
	SrcY = UpLeftY;
	i51KitG2RotatePointEx(SrcX, SrcY, RotateCenterX,  RotateCenterY , &DesX, &DesY, Angle);
	SrcX1 = DesX;
	SrcY1 = DesY;

	for ( Location = SrcW; Location <SrcH; Location++)
	{
		i51KitG2RotatePointEx(SrcX, SrcY, RotateCenterX, RotateCenterY,&DesX, &DesY, Angle);
		PointS[Location][0] = DesX-SrcX1;
		PointS[Location][1] = DesY-SrcY1 ;
		SrcY++;
		SrcY1++;		
	}	

	for ( Location = 0; Location <SrcW; Location++)
	{
		PointS[Location][0] += SrcMoveX;
		PointS[Location][1] += SrcMoveY;
	}	

	return iTRUE;

}


iBOOL RotateRamLinearImageEx  ( iU32 ImageData , iRECT* DesRect , iRECT* SrcRect , iS16 ChangeData[][2])
{
	// author : Otto
	// since : Aug 15 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// iU32 ImageID : ����ͼ���ID��
	// iRECT* DesRect : ����ͼ����Ļ�ϵ�����
	// iRECT* SrcRect : ����ͼ�������
	// iS32 Angle : ��ת�Ƕ�

	// notes : ��ͼ��������ID��ΪImageID��ͼƬSrcRect����
	// ����Angle�ĽǶ���ת����Ƶ���Ļ��DesRect����
	// ͼƬ���������ڴ����б��ݣ���Ҫ�����ݴ��ڴ��ж�ȡ
	// ����ͼƬ�ı��뷽ʽΪI51KITG2_DECODE_LINEAR     

	// ͼƬ��Ⱥ͸߶�
	iU32 ImageWidth = 0;
	iU32 ImageHeight = 0;
	// ͼƬ�����ļ���ʼ��ַ
	iU32 DataEntry = 0;
	// ��ȡ�������ݳ���
	//    iU32  LengthReadData = 0;
	// �ۼƶ�ȡ�������ݳ���
	iU32  LengthTotalReadData = 0;
	// ��Ҫ��ȡ�����ݳ���
	//    iU32  LengthNeedReadData = 0;
	// һ����Ҫ�������Դ�����ݳ���  
	iS32 LengthCopyData = 0;
	// ͼƬ�ļ��ܳ���
	iU32 LengthIamgeData = 0;
	//    iU32 LcdBufferPostion = 0;

	//    iU16 iTempX = 0;
	//    iU16 iTempY = 0;
	// ͼƬ���Ƶ����ϽǺ����½�����  
	iS16 SrcStartX = 0;
	iS16 SrcStartY = 0;
	iS16 SrcEndX = 0;
	iS16 SrcEndY = 0;
	// ���Ƶ���Ļ�ϵ����ϽǺ����½�����  
	iS16 DrawStartX = 0;
	iS16 DrawStartY = 0;
	iS16 DrawEndX = 0;
	iS16 DrawEndY = 0;
	iU16 iTemp = 0;

	// ImageX
	iS32 CurrentImageX = 0;
	iS32 CurrentImageY = 0;
	iS32 CurrentImageWidth = 0;

	// ��ǰ���ԭʼ����	
	iS32 CurrX = 0, CurrY = 0;
	// ��ǰ��ѡ��������
	iS32 RotateX = 0, RotateY = 0;
	// ��ɫbuff
	iU16 * ColorBuff = iNULL;
	iS32 DrawW = 0;
	iS32 DrawH = 0;
	iU16* TempReadDataBuff1 = iNULL;
	iS32 AddsDirectionY = 0;
	iS32 BuffPos = 0;
	iCOLOR TempColor = 0;
	// ��Alpha��Ϣ�����ݵ����
	iU16 TotalAlphaNumber = 0;
	// ���ļ��ж�ȡ�����ݳ���	
	iU32 LengthNeedReadData = 0;
	// ���տռ��ȡ�ļ�
	iU8  * ImageDataEntry = iNULL;
	// �������ж������ļ��ܳ���
	iU32 LengthTotalRead = 0;
	// �ļ�������������ֵ
	iU8   FlagReturn = iFALSE;
	iU16 * ScreenBuffer = iNULL;
	iU32 * LcdInfoArr = iNULL;
	iU16 ScreenWidth = 0;
	iU16 ScreenHeight= 0;

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
	DataEntry = ImageData;
	ImageDataEntry = (iU8*)DataEntry;
	ImageWidth = ImageDataEntry[9]|(ImageDataEntry[10]<<8);
	ImageHeight = ImageDataEntry[11]|(ImageDataEntry[12]<<8);
	LengthIamgeData = ImageDataEntry[5]|(ImageDataEntry[6]<<8)|(ImageDataEntry[7]<<16)|(ImageDataEntry[8]<<24);
	TotalAlphaNumber = ImageDataEntry[18]|(ImageDataEntry[19]<<8);

	DrawW = SrcRect->Width;
	DrawH = SrcRect->Height;
	SrcStartX = SrcRect->X;
	SrcStartY = SrcRect->Y;
	SrcEndX = SrcStartX +DrawW;
	SrcEndY = SrcStartY + DrawH;  

	DrawStartX = DesRect->X;
	DrawStartY = DesRect->Y;
	DrawEndX = DrawStartX + DesRect->Width - 1;
	DrawEndY = DrawStartY + DesRect->Height;


	DataEntry = (iU32)ImageDataEntry;
	LengthTotalReadData = 0X14;
	DataEntry += 0X14;

	while(1)
	{
		TempReadDataBuff1 = (iU16 *)(DataEntry);   
		CurrentImageX = TempReadDataBuff1[0];
		CurrentImageY = TempReadDataBuff1[1];
		CurrentImageWidth = TempReadDataBuff1[2];

		DataEntry += 6;
		LengthTotalReadData += 6 + (CurrentImageWidth<<1);

		if( (CurrentImageY < SrcStartY) || (CurrentImageX >= SrcEndX) )
		{
			DataEntry += ( CurrentImageWidth<<1 );
			continue;
		}

		if( CurrentImageY >= SrcEndY ) break;

		ColorBuff = (iU16*)DataEntry;
		DataEntry += ( CurrentImageWidth<<1 );
		LengthCopyData = CurrentImageWidth;

		if( CurrentImageX<SrcStartX )
		{
			LengthCopyData -= SrcStartX - CurrentImageX;
			CurrentImageX = SrcStartX;
			ColorBuff = &ColorBuff[SrcStartX - CurrentImageX];
			if( LengthCopyData<0 ) continue;
		}

		if( (CurrentImageX+LengthCopyData) > SrcEndX )
		{
			LengthCopyData = SrcEndX - CurrentImageX;
		}

		AddsDirectionY = CurrentImageY-SrcStartY+DrawW;
		CurrY = CurrentImageY - SrcStartY;
		CurrX = CurrentImageX - SrcStartX;
		for( iTemp=0; iTemp<LengthCopyData; iTemp++ )
		{
			RotateX = CurrX + ChangeData[CurrX][0] + ChangeData[AddsDirectionY][0] ;
			RotateY = CurrY + ChangeData[CurrX][1] + ChangeData[AddsDirectionY][1] ;
			CurrX++;
			if( RotateY<DrawStartY|| RotateY>=DrawEndY ||RotateX<DrawStartX||RotateX>=DrawEndX) continue;
			BuffPos = RotateY*ScreenWidth+ RotateX;
			ScreenBuffer[BuffPos] = ColorBuff[iTemp];
			ScreenBuffer[BuffPos+1] = ColorBuff[iTemp];
		}

		if( (CurrentImageY == SrcEndY) && ((CurrentImageX+LengthCopyData) >= SrcEndX)) break;

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

		DataEntry = (iU32)ImageDataEntry+CurrBuffPos + 12;

		for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
		{
			TempReadDataBuff1 = (iU16 *)(DataEntry);   
			AlphaPosX = TempReadDataBuff1[0];
			AlphaPosY = TempReadDataBuff1[1];
			AlphaNumber = (iALPHA)TempReadDataBuff1[2];
			AlphaColor = TempReadDataBuff1[3];
			DataEntry += 8;

			if(AlphaPosY>=SrcEndY) break;
			if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<SrcStartY) continue;				

			CurrX = AlphaPosX - SrcStartX;
			CurrY = AlphaPosY - SrcStartY;
			AddsDirectionY = AlphaPosY-SrcStartY+DrawW;
			RotateX =  CurrX + ChangeData[CurrX][0] + ChangeData[AddsDirectionY][0] ;
			RotateY = AlphaPosY - SrcStartY + ChangeData[CurrX][1] + ChangeData[AddsDirectionY][1] ;
			if( RotateY<DrawStartY|| RotateY>=DrawEndY ||RotateX<DrawStartX||RotateX>=DrawEndX) continue;

			LcdBuffPos =RotateY*ScreenWidth+RotateX;
			ScreenBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(ScreenBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
		}
	}

	return iTRUE;

}

iBOOL i51KitG2RotateImageEx(iU32 ImageData, iRECT* DesRect , iRECT* SrcRect,iS32 SrcMoveX , iS32 SrcMoveY, iS32 RotateCenterX , iS32 RotateCenterY , iU32 RotateType, iS32 Angle )
{
	// author : Otto
	// since : Aug 29 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	//ContainerHandle	������ͼƬ���������ľ��
	//ImageID	ͼƬID
	//DesRect	ͼƬ���Ƶ�Ŀ������
	//ImageHeight	ͼƬ���ƵĲü���
	//SrcMoveX	ͼƬ��תǰƽ�����㣨SrcMoveX,SrcMoveY��
	//SrcMoveY	ͼƬ��תǰƽ�����㣨SrcMoveX,SrcMoveY��
	//RotateCenterX ͼƬ��ת����X����
	//RotateCenterY ͼƬ��ת����Y����
	//RotateType	��ת����
	//Angle ��ת�Ƕ�

	// notes : ��˳ʱ�뷽����תһ���ǶȺ����ImageContainer�����ڱ��ΪImageID��ͼ��
	// SrcRect Ϊ��ʱ�ڵ�(DesRect->X, DesRect->Y)����ʼ��������ͼƬ
	// DesRect Ϊ������ƴ���Ļ���Ͻǿ�ʼ

#define ROTATE_MAX_GIRTH_HALF 600

	iU8 * ImageDataPoint = iNULL;
	iU32 ImageWidth = 0;
	iU32 ImageHeight = 0;
	// ͼƬ���뷽ʽ
	iU8   DecodeType = 0;
	iRECT ImageRect;
	iRECT DrawDesRect;
	iRECT DrawSrcRect;
	iU8 Ret = iFALSE;
	iS16 * GirthPoint = iNULL;
	iU16 TotalAlphaNumber = 0;
	iU32 LengthIamgeData = 0;
	iU16 ScreenWidth = 0;
	iU16 ScreenHeight= 0;

	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
	{
		return iFALSE;
	}

	ImageDataPoint = (iU8 *)ImageData;
	if( iNULL==ImageDataPoint ) return iFALSE;

	if( ImageDataPoint[0X11]!=0X02 )
	{
		iLog("�ļ���ʽ��������������ͼƬ�ļ�!");
		return iFALSE;
	}

	ImageWidth = ImageDataPoint[9]|(ImageDataPoint[10]<<8);
	ImageHeight = ImageDataPoint[11]|(ImageDataPoint[12]<<8);
	LengthIamgeData = ImageDataPoint[5]|(ImageDataPoint[6]<<8)|(ImageDataPoint[7]<<16)|(ImageDataPoint[8]<<24);
	TotalAlphaNumber = ImageDataPoint[18]|(ImageDataPoint[19]<<8);

	if( iNULL==SrcRect )
	{
		DrawSrcRect.X = 0;
		DrawSrcRect.Y = 0;
		DrawSrcRect.Width = ImageWidth;
		DrawSrcRect.Height = ImageHeight;
	}
	else
	{
		DrawSrcRect.X = SrcRect->X;
		DrawSrcRect.Y = SrcRect->Y;
		DrawSrcRect.Width = SrcRect->Width;
		DrawSrcRect.Height = SrcRect->Height;
	}

	if( iNULL==DesRect )
	{
		DrawDesRect.X = 0;
		DrawDesRect.Y = 0;
		DrawDesRect.Width = ScreenWidth;
		DrawDesRect.Height = ScreenHeight;
	}
	else
	{
		DrawDesRect.X = DesRect->X;
		DrawDesRect.Y = DesRect->Y;
		DrawDesRect.Width = DesRect->Width;
		DrawDesRect.Height = DesRect->Height;
	}


	ImageRect.X = 0;
	ImageRect.Y = 0;
	ImageRect.Width= ImageWidth;
	ImageRect.Height= ImageHeight;

	if(DrawSrcRect.X<0) { DrawSrcRect.Width+= DrawSrcRect.X; DrawSrcRect.X = 0;}
	//    else if( DrawSrcRect.X>=ScreenWidth|| DrawSrcRect.X>=ImageRect.Width) return iTRUE;
	else if( DrawSrcRect.X>=ImageRect.Width) return iTRUE;
	//    if( (DrawSrcRect.X+DrawSrcRect.Width) > ScreenWidth ) DrawSrcRect.Width = ScreenWidth -DrawSrcRect.X;
	if( (DrawSrcRect.X+DrawSrcRect.Width) > ImageRect.Width ) DrawSrcRect.Width = ImageRect.Width -DrawSrcRect.X;

	if(DrawSrcRect.Y<0) { DrawSrcRect.Height+= DrawSrcRect.Y; DrawSrcRect.Y = 0;}
	else if( DrawSrcRect.Y>=ImageRect.Height) return iTRUE;
	//    else if( DrawSrcRect.Y>=ScreenHeight|| DrawSrcRect.Y>=ImageRect.Height) return iTRUE;
	//    if( (DrawSrcRect.X+DrawSrcRect.Width) > ScreenWidth ) DrawSrcRect.Width = ScreenWidth -DrawSrcRect.X;
	if( (DrawSrcRect.Y+DrawSrcRect.Height) > ImageRect.Height) DrawSrcRect.Height= ImageRect.Height-DrawSrcRect.Y;

	if(DrawDesRect.X<0) { DrawDesRect.Width+= DrawDesRect.X; DrawDesRect.X = 0;}
	else if( DrawDesRect.X>=ScreenWidth) return iTRUE;
	if( (DrawDesRect.X+DrawDesRect.Width) > ScreenWidth ) DrawDesRect.Width = ScreenWidth -DrawDesRect.X;

	if(DrawDesRect.Y<0) { DrawDesRect.Height+= DrawDesRect.Y; DrawDesRect.Y = 0;}
	else if( DrawDesRect.Y>=ScreenHeight) return iTRUE;
	if( (DrawDesRect.Y+DrawDesRect.Height) > ScreenHeight) DrawDesRect.Height= ScreenHeight-DrawDesRect.Y;

	if( (DrawSrcRect.Height+DrawSrcRect.Width)>=ROTATE_MAX_GIRTH_HALF  )   return iFALSE;

	GirthPoint = (iS16*)(i51AdeOsMalloc((DrawSrcRect.Height+DrawSrcRect.Width)*4,__FILE__ , __LINE__ ));

	if(iNULL==GirthPoint) 
	{
		iLog("KIT - G2 : malloc rotate");
		return iFALSE;		
	}

	if( iNULL==i51KitG2GetGirthPointsEx(DrawSrcRect, DrawDesRect,(iS16(*)[2])GirthPoint, SrcMoveX, SrcMoveY, RotateCenterX, RotateCenterY,RotateType, Angle))
	{
		i51AdeOsFree(GirthPoint);
		GirthPoint = iNULL;
		return iTRUE;
	}	

	RotateRamLinearImageEx((iU32)ImageDataPoint, &(DrawDesRect),  &(DrawSrcRect), (iS16(*)[2])GirthPoint);
	i51AdeOsFree(GirthPoint);
	GirthPoint = iNULL;

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
	iLog( "G2DrawLine Start , %x, %x", Point, Color) ;
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
			iLog( "G2DrawLine Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
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
	iLog( "G2DrawLine Start , %x, %x", Point, Color) ;
#endif

	return iTRUE;

}

iU8 AlphaNeed = iTRUE;
iU8 AlphaSrc = 128;
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
	iLog( "G2CleanScreen Start , %x, %x", Color, Rect) ;
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

	if(!AlphaNeed)
	{
		for( TempX=0; TempX<CleanW ; TempX++)
		{
			OneRowColorBuf[TempX] = Color;
		}

		CleanW = (CleanW<<1);	
		LcdBufPos = CleanY*ScreenWidth + CleanX;
		for( TempY=0; TempY<CleanH; TempY++)
		{
			i51AdeStdMemcpy16(&(ScreenBuffer[LcdBufPos]), OneRowColorBuf, CleanW);
			LcdBufPos += ScreenWidth;
		}

		//if(OneRowColorBuf)
		//{
		//	i51AdeOsFree(OneRowColorBuf);
		//	OneRowColorBuf = iNULL;
		//}
	}
	else
	{
		for( TempY=0; TempY<CleanH; TempY++)
		{
			LcdBufPos = (CleanY+TempY)*ScreenWidth + CleanX;
			for( TempX=0; TempX<CleanW ; TempX++)
			{
				ScreenBuffer[LcdBufPos] = i51KitG2GetNewColorEx(ScreenBuffer[LcdBufPos], Color,AlphaSrc);
				LcdBufPos++;
			}
		}
	}

	if(OneRowColorBuf)
	{
		i51AdeOsFree(OneRowColorBuf);
		OneRowColorBuf = iNULL;
	}


#if i51KITG2_DEBUG
	iLog( "G2CleanScreen End , %x, %x", Color, Rect) ;
#endif
	return iTRUE;

}


//iBOOL i51KitG2CleanScreenEx(iCOLOR Color, iRECT *Rect)
//{
//	// author : Otto
//	// since : Sep 27 2011
//	// (C) 2011 , PKIG Tech. Co., Ltd.
//
//	// iCOLOR Color : �����Ļ����ɫ
//	//iRECT *Rect : �����Ļ������
//
//	// Notes : ��ָ������ɫ����Ļ����
//
//	iS32 CleanX, CleanY;
//	iS32 CleanW, CleanH;
//	iS32 LcdBufPos = 0;
//	iS32 TempX, TempY;
//	iU16 ScreenWidth;
//	iU16 ScreenHeight;
//	iU16 * ScreenBuffer = iNULL;
//	iU32 * LcdInfoArr = iNULL;
//	iU16 * OneRowColorBuf = iNULL;
//
//#if i51KITG2_DEBUG
//	iLog( "G2CleanScreen Start , %x, %x", Color, Rect) ;
//#endif
//
//	if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 )
//	{
//		return iFALSE;
//	}
//
//	LcdInfoArr =  i51AdeMmiGetPanel();
//	if( iNULL == LcdInfoArr )
//	{
//		return iFALSE;
//	}
//
//	ScreenBuffer = (iU16 *) LcdInfoArr[1] ;
//
//	if( Rect)
//	{
//		CleanX = Rect->X;
//		CleanY = Rect->Y;
//		CleanW = Rect->Width;
//		CleanH = Rect->Height;
//	}
//	else
//	{
//		CleanX = 0;
//		CleanY = 0;
//		CleanW = ScreenWidth;
//		CleanH = ScreenHeight;
//	}
//
//	if(CleanX<0)
//	{
//		CleanW += CleanX;
//		CleanX = 0;
//	}
//
//	if(CleanY<0)
//	{
//		CleanH += CleanH;
//		CleanY = 0;
//	}
//
//	if( (CleanX+CleanW)>ScreenWidth) CleanW = ScreenWidth-CleanX;
//	if( (CleanY+CleanH)>ScreenHeight) CleanH = ScreenHeight-CleanY;
//
//	if( ScreenHeight>ScreenWidth)
//	{
//		OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenHeight<<1),  __FILE__ , __LINE__ );
//	}
//	else
//	{
//		OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenWidth<<1),  __FILE__ , __LINE__ );
//	}
//
//	for( TempX=0; TempX<CleanW ; TempX++)
//	{
//		OneRowColorBuf[TempX] = Color;
//	}
//
//	CleanW = (CleanW<<1);	
//	LcdBufPos = CleanY*ScreenWidth + CleanX;
//	for( TempY=0; TempY<CleanH; TempY++)
//	{
//		i51AdeStdMemcpy16(&(ScreenBuffer[LcdBufPos]), OneRowColorBuf, CleanW);
//		LcdBufPos += ScreenWidth;
//	}
//
//	if(OneRowColorBuf)
//	{
//		i51AdeOsFree(OneRowColorBuf);
//		OneRowColorBuf = iNULL;
//	}
//
//#if i51KITG2_DEBUG
//	iLog( "G2CleanScreen End , %x, %x", Color, Rect) ;
//#endif
//	return iTRUE;
//
//}
