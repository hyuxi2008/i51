
#ifndef THE_i51KitP2_H
#define THE_i51KitP2_H

#include "i51.h"

#define COORDINATE_RATE	100
#define VELOCITY_RATE	11
#define FORCE_RATE	11

struct iShape;

// ��ά��������
typedef struct iVECT
{
	iS32 X;
	iS32 Y;
}iVECT;

// ����״̬
typedef enum iBodyState
{
	IBODY_STATE_SLEEP = 0,
	IBODY_STATE_AWAKE,
	IBODY_STATE_MOVE
} iBodyState;

// �����е�����
typedef struct iBody
{
	// �������������
	struct  iShape * Shape;
	// ����
	iS32 Mass;
	// ������С
	iVECT GravityForce;
	// ����λ��
	iVECT Centroid;
	// ����
//	iS32 Damping;
	// �ٶȡ�����ٶȡ��ٶ�����ֵ��Ϊ�˷�ֹ�˶�����䴩͸
	// v_static���˶������뾲ֹǽ��֮��ĵ����ٶ�
	// v_bias���˶�����֮��ĵ����ٶ�
	iVECT V, MAX_V, v_bias;//, v_static;
	// ����״̬
//	iBodyState BodyState;
	// ����
	iVECT ActingForce;
	// ����
//	iVECT ExternalForce;
	// ˮ���Ƿ���ԡ���еı��
	iU8 Flag;
}iBody;

// ������������
typedef enum iShapeType{
	I51_CIRCLE_SHAPE = 0,
	I51_SEGMENT_SHAPE,
	I51_POLY_SHAPE,
	I51_NUM_SHAPES
} iShapeType;

#define MAX_COE	10000
#define MAX_COF	10000
#define MAX_MASS	100000

typedef struct mwMapBlock 
{
	unsigned char BlockType;
	unsigned char ShapeType;
	signed short BlockX;
	signed short BlockY;
}mwMapBlock;

// �����е���������
typedef struct iShape
{
	// ��������
	iShapeType ShapeType;
	// ���ζ�Ӧ��Body
	iBody * Body;
	// Ħ��ϵ��,ȡֵ��Χ(0~10000)
//	iS32 cof;
	// ����ϵ��,ȡֵ��Χ(0~10000)
//	iS32 coe;
	// �Ƕ�
//	iS32 Angle;
	// ���ٶ�
//	iS32 AngularRate;
	// Ť��
//	iS32 Torsion;
	// �������ڵ��飬ͬһ���ڵ����β�������ײ��Ĭ��Ϊ0��������ͬһ������η�����ײ��һ���ֽڶ�Ӧһ���飬һ�����ο����ڶ����
//	iU32 Group;
	// �������ڵĲ㣬ͬһ���ڵ����βŻᷢ����ײ��ÿһ���ֽڶ�Ӧһ���㣬���֧��32���㣬һ�����ο����ڶ������
//	iU32 Layer;
	// �����Ƿ����ƶ��ı�ǣ�����󶨵�body�ǿռ��staticbody����StaticFlagΪ�棬���岻���ƶ�
	iS8 StaticFlag;  
	// �������ڵ��飬��Ҫ���ڻ��ش�
	iU32 ShapeGroup;
	mwMapBlock block;

}iShape;

typedef struct iShapeCircle
{
	// ����
	iShape Shape;
	// �뾶
	iS32 Radius;
	// ��ǰ��Բ������
	iVECT CentrePoint;
	// ��һ�����ڵ�Բ������
	iVECT LastPoint;
	iVECT LastPoint1;
	iVECT LastPoint2;
	iVECT LastPoint3;
	iVECT LastPoint4;
}iShapeCircle;

typedef struct iShapeSeg
{
	// ����
	iShape Shape;
	// �˵�����
	iVECT p1,p2;
	// �߶κ��
	iS32 Thickness;
	// ֱ��һ��ʽ��A,B,C
	// A*x+B*y+C=0;
	iS32 a,b,c;
}iShapeSeg;

typedef struct iShapePoly
{
	// ����
	iShape Shape;
	// �˵���
	iS32 PointNum;
	// �˵�����
	iVECT * Points;
}iShapePoly;

typedef struct iNode
{
	iHANDLE Handle;
	struct iNode * Next;
	struct iNode * Parent;
}iNode;

typedef struct Node
{
	// �Ƿ�ʹ�õı��
	iU8 Flag;
	iHANDLE Handle;
}Node;

typedef struct NodeContainer
{
	iS32 MaxNode;
	iS32 UseNode;
	iS32 IdlyNode;
	Node * NodeHandle;
}NodeContainer;

#define DEFAULT_BODY_NUM	50
#define DEFAULT_SHAPE_NUM	500

typedef struct iWorld
{
	// �����е������ٶ���m/s(��/��)
	// �����е�����������kg(ǧ��)
	// �����е�����ʱ�䵥λ����(s)
	// ������ÿһ�����ض�Ӧ1024��
	// ��������ϵ����Ļ����ϵ

	// ��������Ĵ�С
	iRECT WorldRect;
	// ����������С
	iVECT WorldGravity;
	// ����Body
	iBody StaticBody;
	// ���ε���i51KitP2Step()��ʱ����λ��(s)��Ĭ��Ϊ1
	iS32 Stamp;
	// ÿ����һ��i51KitP2Step()�������е�����������������Ĭ��Ϊ1
	// ���StepsΪ0���������������徲ֹ
	iS32 Steps;
	// ������Body����
	iS32 BodyNum;
	// ����������Body������
	NodeContainer BodyContainer;
	// ������shape����
	iS32 ShapeNum;
	// ����������Shape������
	NodeContainer ShapeContainer;
}iWorld;

#define AbsInt(X) ((X)>0?((X)):(-(X)))


extern void * i51KitP2Malloc(iU32 length);
extern iBOOL iVectInit1(iVECT * DesVect, iVECT v1);
extern iBOOL iVectInit2(iVECT * DesVect, iS32 X, iS32 Y);
extern iU32 SqrtInt(iU32 Num);
extern iHANDLE i51KitP2CreateWorld( iRECT * WorldRect, iVECT * Gravity);
extern iBOOL i51KitP2ReleaseWorld(iHANDLE Space);
extern iHANDLE i51KitP2CreateBody( iHANDLE Space, iVECT * Centroid, iS32 Mass);
extern iHANDLE i51KitP2CreateShapeSeg( iHANDLE Space, iHANDLE Body, iS32 Thickness, iVECT P1, iVECT P2);
extern iHANDLE i51KitP2CreateShapeCircle(iHANDLE Space, iHANDLE Body, iS32 Radius, iVECT * Centre);
extern iBOOL i51KitP2ReleaseBody(iHANDLE Space, iHANDLE Body);
extern iBOOL i51KitP2ReleaseShape(iHANDLE Space, iHANDLE Shape);
extern iBOOL i51KitP2ResetWorldTimeStamp(iHANDLE Space, iS32 Stamp, iS32 Steps);
extern iBOOL i51KitP2Step(iHANDLE Space);

#endif

