#ifndef __PNG_H__
#define __PNG_H__

#include "i51.h"

//  author : yepan
//  since  : 2012-7-20
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			ImageHandle ����Դָ��
//			HandleType	0==����Դ���ļ����,1==����Դ���ڴ�ָ��
//			ImageWidth	�����ͼƬ���
//			ImageHeight �����ͼƬ�߶�
//
//  return :
//			iTRUE==�ɹ��� iFALSE==ʧ��
//  note   :
//			��ȡͼƬ��Ϣ
iBOOL png_get_attribute(iHANDLE ImageHandle, iU8 HandleType,  iU32 * ImageWidth, iU32 * ImageHeight);

//  author : yepan
//  since  : 2012-7-20
//  (C) PKIG Tech. Co., Ltd.
//  
//  Param  :
//			type				0==���룬 1==���벢����
//			ImageHandle			����Դָ��
//			ImageHandleType		0==����Դ���ļ����,1==����Դ���ڴ�ָ��
//			SrcRect				����Դ����
//			DesHandle			Ŀ�Ķ���ָ��
//			DesHandleType		0==Ŀ�Ķ������ļ����,1==Ŀ�Ķ������ڴ�ָ��
//			DesRect				Ŀ������
//  return :
//			iTRUE==�ɹ��� iFALSE==ʧ��
//  note   :
//			����pngͼƬ��
iBOOL png_decode(iS32 type, iU32 TransColor, iHANDLE ImageHandle, iU8 ImageHandleType, iU32 ImageFileSize, iRECT* SrcRect, iHANDLE DesHandle, iU8 DesHandleType, iRECT* DesRect);

#endif