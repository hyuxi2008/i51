
//#include "math.h"
#include "string.h"
#include "i51KitAde.h"
#include "i51KitG2.h"
#include "i51KitStd.h"

#include "i51KitG2Font.h"

i51KIT_DYNAMIC_KIT_DEF(i51KitStd);

// ���Կ���
#define DEBUG_I51KITG2 0
#define LCDBUFF_DEBUG  0
#define i51KITG2_DEBUG  0
#define ROTATE_SPECIAE 1

#if DEBUG_I51KITG2
#include "Image_Hex.h"
#endif

// ��X�ľ���ֵ
#define I51KITG2_ABS(X) (((X)>0)?(X):(-X))

// ͼƬ�����汾��
#define I51KITG2_IMAGE_CONTAINER_VERSION 0X02

// ͼƬ�����汾���������ļ��ڵ�ƫ����
#define I51KITG2_OFFSET_CONTAINER_VERSION 0X08
// ͼƬ�����ļ��汾���������ļ��ڵ�ƫ����
#define I51KITG2_OFFSET_FILE_VERSION 0X09
// ͼƬ�����е�һ��ͼƬ�����Ƿ�������ͼƬ����ͷ��ƫ����
#define I51KITG2_OFFSET_START_DATA  0X11
// ͼƬ������ͼƬ���ݳ����������ڵ�ƫ����
#define I51KITG2_OFFSET_DATALENGTH  0X05
// ͼƬ������ͼƬ����������ڵ�ƫ����
#define I51KITG2_OFFSET_IMAGEWIDTH  0X09
// ͼƬ������ͼƬ�߶��������ڵ�ƫ����
#define I51KITG2_OFFSET_IMAGEHEIGHT 0X0B
// ͼƬ������ͼƬ͸��ɫ�������ڵ�ƫ����
#define I51KITG2_OFFSET_COLORALPHA  0X0D
// ͼƬ������ͼƬ���뷽ʽ�������ڵ�ƫ����
#define I51KITG2_OFFSET_DECODETYPE  0X11
// ͼƬ������ͼƬ�Ƿ��д�Alpha�ĵ������������ڵ�ƫ����
#define I51KITG2_OFFSET_ALPHANUMER     0X12
// ͼƬ������ͼƬ������Ϣ�������ڵ�ƫ����
#define I51KITG2_OFFSET_PIXELDATA     0X14

// ͼƬ���뷽ʽΪ��������,�������ȴ洢ÿ�����ص���Ϣ
#define I51KITG2_DECODE_NORMAL         1
// ͼƬ���뷽ʽΪ����ѹ�����뷽ʽ
// ��ͼ��������Ϣ�������Ƚ������Ա��룬���ݸ�ʽ���£�
// x,y,w,RGB���ݣ�����x��y�Ƿ�͸��ɫ��������꣬w�������ж��ٸ���͸��ɫ��x��y�ֱ�ռ��1.5�ֽڣ�wռ�����ֽڡ�
#define I51KITG2_DECODE_LINEAR           2
// ͼƬ���뷽ʽΪ��ɢѹ����ʽ
// ��ͼ��������Ϣ�������Ƚ�����ɢ��ʽ���룬���ݸ�ʽ���£�
// Y,w,X1,X2����Xw��ͼ����Ϣ������Y���Ƿ�͸��ɫ��Y���꣬w����һ���ж��ٸ���͸��ɫ��
// X1,X2����Xw�ֱ���ÿһ����͸��ɫ��X���꣬Y��w��Xi��ռ1.5���ֽڣ���wΪ����ʱ��Xwռ�����ֽڡ�
#define I51KITG2_DECODE_SCATTER        3

// ��ʾͼƬʱ��ԭͼȫ����ʾ��Ŀ������ı��,Ҳ����ͼƬ���򴫿�ʱʹ��
//#define I51KITG2_DRAW_IMAGE_ALL        0XFFF

// ���ļ��ж�ȡͼƬ��Ϣ��Buffer��С
#define I51KITG2_LENGTH_DATA_BUFF     0X400

// ��ʾͼƬʱ������������ʾ
#define I51KITG2_DRAW_IAMGE_NORMAL    0X01
// ��ʾͼƬʱ����������ʾ
#define I51KITG2_DRAW_IAMGE_MIRROR    0X02
// ��ʾͼƬʱ������������ʾ
#define I51KITG2_DRAW_IAMGE_HANDSTAND 0X04
// ��ʾͼƬʱ����������ʽ��ʾ
#define I51KITG2_DRAW_IAMGE_MIRROR_HANDSTAND ( (I51KITG2_DRAW_IAMGE_MIRROR)|(I51KITG2_DRAW_IAMGE_HANDSTAND))
// ��ʾͼƬʱ������Ƕ���ת
#define I51KITG2_DRAW_IAMGE_ROTATE   0XEE

// �ֶ�ѹ����ͼƬ�ļ���ѹ��ÿ����󳤶�
#define I51KITG2_LENGTH_PART_UMCOMPRESS  32768

// Ԫ��������ͼƬ
#define I51KITG2_ELEMENT_TYPE_PICTURE    1
// Ԫ�������ǵ�
#define I51KITG2_ELEMENT_TYPE_POINT        2
// Ԫ��������ֱ��
#define I51KITG2_ELEMENT_TYPE_LINE           3
// Ԫ�������Ƕ����
#define I51KITG2_ELEMENT_TYPE_POLYGON   4
// Ԫ�������ǻ���
#define I51KITG2_ELEMENT_TYPE_ARC            5
// Ԫ��������Բ
#define I51KITG2_ELEMENT_TYPE_CIRCLE       6
// Ԫ���������������
#define I51KITG2_ELEMENT_TYPE_FILLPOLYGON  7
// Ԫ����������仡��
#define I51KITG2_ELEMENT_TYPE_FILLARC      8
// Ԫ�����������Բ��
#define I51KITG2_ELEMENT_TYPE_FILLCRICLE  9
// Ԫ������������
#define I51KITG2_ELEMENT_TYPE_WORDS  10
// Ԫ�����������ͼƬ
#define I51KITG2_ELEMENT_TYPE_FILLIMAGE  11

// ϵͳ����ܴ����������
#define I51KITG2_MAX_NUMBER_VIRTUAL_PANEL 10
// ͼƬ����������ŵ�ͼƬ����
#define I51KITG2_CONTAINER_NUMBER_IMAGE 256
// Բ����
//#define I51KITG2_PI 3.1416

// �ֿ��ļ��ļ����͵�ַ
#define i51KITG2_FONT_FILETYPE 4
// �ֿ��ļ���ģ���͵�ַ
#define i51KITG2_FONT_MODELTYPE 9
// �ֿ��ļ���ģ��ȵ�ַ
#define i51KITG2_FONT_MODELWIDTH 10
// �ֿ��ļ���ģ�߶ȵ�ַ
#define i51KITG2_FONT_MODELHEIGHT 11
// �ֿ��ļ����ú��ָ�����ַ
#define i51KITG2_FONT_NORMALWORDS_NUM 12
// �ֿ��ļ������ַ���ģƫ������ַ
#define i51KITG2_FONT_NORMALWORDS_OFFSET 14
// �ֿ��ļ������ַ���ģƫ������ַ
#define i51KITG2_FONT_ALLWORDS_OFFSET 18
// ���ָ���
#define i51KITG2_FONT_WORDS_NUMBER 8178

// ��ת�������Ƶ���ת
#define i51KITG2_ROTATE_POINT 0X01
// ��ת����X�᷽��
#define i51KITG2_ROTATE_XLINE 0X02
// ��ת����Y�᷽��
#define i51KITG2_ROTATE_YLINE 0X03

typedef struct
{
    // author : Otto
    // since : 2011.10.11
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // �ֿ���Դ�ļ����
    iFILE FontFileHandle;
    // �ֿ���ģ�ļ�����Դ�ļ��е�ID
    iFILE TypeheadID;
    // ��ģ�߶�	
    iU8   WordHeight;
    // ��ģ���	
    iU8   WordWidth;
    // ȡ��ģ����
    iU8  TypeheadType;
    // ���ú��ָ���	
    iS16  NormalUseWordsNum;	
    // ���ú����ڴ��ַ�������Cach��ΪiNULL
    iU32  NormalWordsEntry;
    // �����ַ���ģ���ļ��ڵĿ�ʼλ��ƫ����
    iU32  NormalWordsOffset;
    // ȫ���ַ���ģ���ļ��ڵĿ�ʼλ��ƫ����
    iU32  AllWordsOffset;
    // �ֿ��ļ�����Դ�ļ��еĿ�ʼλ��
    iU32  TypeheadOffset;
}i51KitG2Font;

// �ֿ�������ĸ���
#define MAX_TYPEHEAD_NUMBER 3
// �ֿ������
static i51KitG2Font i51KitG2FontManage[MAX_TYPEHEAD_NUMBER];
// һ���ֵ���ģ���ռ���ֽ���
// ���֧�������ָߺ��ֿ�ռ16λ,һ����ռ2*16,32���ֽ�
// Ŀǰ֧�ֵ���ģ�Ǵ������ң��������µ�˳��洢
#define NUMBER_TYPE_MATRIX           32

typedef struct
{

    // author : Otto
    // since : 2011.9.29
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // ���ֵĻ�����
    iU16 CharactersKeywords;
    // ���ֵľ�����ģ
    iU8   TypeMatrix[NUMBER_TYPE_MATRIX];
}i51KitFontWord;

//static i51KitFontWord * i51KitFontNormalWordStock = iNULL;

typedef struct
{

    // author : Otto
    // since : 2011.8.19
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes:  ����ͼƬ������һ��ͼƬ

    // �������Ƿ���ͼƬ���ݵı��
    iU8 FlagImageExist;
    // �ڴ����Ƿ���ͼƬ���ݵı��
    iU8 FlagCopyRAM;
	
    // ͼƬ���뷽ʽ
    // ͼƬ�����ֱ��뷽ʽ

    // 0X01���������룬�������ȴ洢ÿ�����ص���Ϣ��
    // 0X02������ѹ�����뷽ʽ����ͼ��������Ϣ�������Ƚ������Ա��룬���ݸ�ʽ���£�x,y,w,RGB���ݣ�
    //       ����x��y�Ƿ�͸��ɫ��������꣬w�������ж��ٸ���͸��ɫ��x��y�ֱ�ռ��1.5�ֽڣ�wռ�����ֽڡ�
    // 0X03����ɢѹ����ʽ����ͼ��������Ϣ�������Ƚ�����ɢ��ʽ���룬���ݸ�ʽ���£�Y,w,X1,X2����Xw��ͼ����Ϣ��
    //       ����Y���Ƿ�͸��ɫ��Y���꣬w����һ���ж��ٸ���͸��ɫ��X1,X2����Xw�ֱ���ÿһ����͸��ɫ��X���꣬Y��
    //       w��Xi��ռ1.5���ֽڣ���wΪ����ʱ��Xwռ�����ֽڡ�
    iU8 DecodeType;
    // ͼƬ�Ƿ������ݵ��Alpha�ı��
    iU16 AlphaPixelNum;
    // ͼƬ���	
    iU16 ImageWidth;	
    // ͼƬ�߶�
    iU16 ImageHeight;	
    // ͼƬ�����ܳ���	
    iU32 LengthTotalData;	
    // ͼƬ���ݻ����㳤��	
    iU32 LengthIamgeData;	
    // ͼƬ���ݵ����
    // ͼƬ�����������ļ��еĿ�ʼ��ַƫ����
    iU32 ImageEntry;
    // ͼƬ���ݵ��ڴ濪ʼ��ַ
    iU32 MemoryEntry;
    // ͼƬ�ǵڼ��μ��ؽ��ڴ�ı��
    iU32 SetMemoryCopyCount;
} i51ImageIndex ;

// ͼƬ�ǵڼ��μ��ؽ��ڴ�ļ���
static iU32 SetMemoryCopyCount = 0;
// ��ǰʹ�õ�ͼƬ���������
static iU32 CurrContainerManageNum = 0;
// ͼƬ���ؽ��ڴ��Ƿ�����resume״̬
static iU8  RESUNE_MEMORY_COPY = iFALSE;

typedef struct
{

    // author : Otto
    // since : 2011.8.19
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes:  ����ͼƬ����

    // �򿪵�ͼƬ�����ļ�ָ��
    iFILE FileHandle;
    // ������ͼƬ����
    iU16 ImageTotalNumber;
    // ������ͼƬ���ID
    iU16 ImageMaxId;
    // ����������ͼƬ״̬��������
    i51ImageIndex * ImageIndexTable;
} i51ImageContainer ;


typedef struct
{

    // author : Otto
    // since : 2011.9.3
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes:  ���⻭���е�Ԫ����Ϣ

    // Ԫ�ر��
    iU16 ElementNumber;
    // Ԫ������
    iU8 ElementType;
    // Ԫ��Alphaֵ
    iU8 ElemenAlpha;
    // Ԫ����Ϣ
    union
    {
        // ͼ��
        struct
        {
            // ����ͼƬ���������ľ��
            iHANDLE ContainerHandle;
            // ���Ʒ���
            iU8  DrawDirection;
            // ����ͼ����
            iU32 ImageHandle;
            // ��תͼƬ����ת�Ƕ�			
            iS32 DarwAngle    ;
            // ͼƬ��ԭʼ����
            iRECT SrcRect;
            // ͼƬ���Ƶ���Ļ�ϵ�����
            iRECT DesRect;
            // ��תͼƬ�ƶ���X����
            iS32 SrcMoveX;
            // ��תͼƬ�ƶ���Y����
            iS32 SrcMoveY;
            // ��תͼƬ����ת����X����
            iS32 RotateCenterX;
            // ��תͼƬ����ת����Y����
            iS32 RotateCenterY;
            // ��תͼƬ����ת����
            iU32 RotateType;
        }Image;

        // ��
        struct
        {
            // ���Ƶ��X����
            iS32 X;
            // ���Ƶ��Y����
            iS32 Y;
            // ���Ƶ����ɫֵ
            iCOLOR Color;
        }Point;

        // ֱ��
        struct
        {
            // ����ֱ�ߵ������˵�����
            iS32 Points[2][2];
            // ����ֱ�ߵ���ɫ
            iCOLOR Color;
        }Line;

        // �����
        struct
        {
            // ���ƶ���εĶ�����
            iU32 VerticeNumber;
            // ���ƶ���εĶ�������ָ��
            iU32 PointerPoints;
            // ���ƶ���ε���ɫ
            iCOLOR Color;
        }Polygon;

        // ����
        struct
        {
            // �����ϵ���㡢�յ�ͻ�����һ�������
             iS32 Points[3][2];
            // ���ƻ��ߵ���ɫ
             iCOLOR Color;
        }Arc;

        // Բ
        struct
        {
            // Բ��X����
            iS32 X;
            // Բ��Y����
            iS32 Y;
            // Բ�뾶
            iU32 R;
            // ����Բ����ɫ
            iCOLOR Color;
        }Circle;

        // �������
        struct
        {
            // ������εĶ�����
            iU32 VerticeNumber;
            // ������εĶ�������ָ��
            iU32 PointerPoints;
            // ������ε���ɫ
            iCOLOR Color;
        }FillPolygon;

        // ��仡��
        struct
        {
            // ��仡�ߵ���㡢�յ�ͻ�����һ�������
             iS32 Points[3][2];
            // ��仡�ߵ���ɫ
             iCOLOR Color;
        }FillArc;

        // ���Բ
        struct
        {
            // ���Բ��Բ��X����
            iS32 X;
            // ���Բ��Բ��Y����
            iS32 Y;
            // ���Բ�İ뾶
            iU32 R;
            // ���Բ����ɫ
            iCOLOR Color;
        }FillCircle;

        struct
        {
            // �ֿ�������Դ�ļ�Handle
            iFILE ReFileHandle;
            // �ֿ��ļ�����Դ�ļ��е�ID			
            iU32 TypeheadID;
            // ���־��			
            iU32 WordsHandle;
            // ���ֻ�������			
            iRECT   DrawRect;
            // ����ԭʼ����			
            iRECT   SrcRect;
            // ����������ɫ			
            iCOLOR fgcolor; 		
            // ���Ʊ�����ɫ			
            iCOLOR bgcolor;
            // �������ַ���			
            iU16 DrawDirect;
            // ����������Ч			
            iU16 Effect; 
            // �������ָ���			
            iU16 WordsNum;
            // ���������м��
            iS16 WordsRowGap;		
            // ����λ���м��
            iS16 WordsColumnGap;			
	}Words;
        // ͼ��
        struct
        {
            // ����ͼƬ���������ľ��
            iHANDLE ContainerHandle;
            // ����ͼ����
            iU32 ImageHandle;
            // ͼƬ��������ͼƬԭʼ����
            iRECT SrcRect;
            // ͼƬ����������Ļ����
            iRECT DesRect;
        }Fill;		
    }ElementData;

} i51KitG2VPElement;

typedef struct
{

    // author : Otto
    // since : 2011.9.5
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes:  һ�����⻭�����Ϣ

    // �����ڹ������еı��
    iU16 VP_ID;
    // ������
    iHANDLE VPHandle;
    // �����Ƿ��Ѿ�����
    iBOOL VpIsFull;
    // �����Ƿ��������ܻ���	
    iBOOL VpIsDraw;
    // �����Ƿ�ѭ��
    iBOOL VpIsLoop;
    // ���������Ԫ�ظ���
    iU16 VPMaxElement;
    // �����ڵ�ǰ�Ѿ�����Ԫ�ظ���
    iU16 VPCurrentElement;
    // ���������Ļ��Rect  
    iRECT VPActualRect;
    // ���Ƶ�����
    iRECT VPDrawRect;
    // Ԫ����Ϣ�б�
    i51KitG2VPElement * VPElementDetails;  
}i51KitG2VPStructure;

typedef struct
{

    // author : Otto
    // since : 2011.9.5
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes:  ���⻭�������

    // �Ƿ�ʹ�����⻭����
    iU8 UseVirtualPanel;
    // �����Ļ�������
    iU16 TotalVPNumber;
    // ��໭������
    iU16 MaxVPNumber;
    // ��ǰ�������
    iU16 ActivityVp_ID;
    // ��ǰ�����ָ��
    i51KitG2VPStructure * CurrentVpActivity;
    // ��ǰ���ƻ���ָ��
    i51KitG2VPStructure * CurrentVpRender;
    // ���⻭����Ϣ�б�
    i51KitG2VPStructure * VPDetails[I51KITG2_MAX_NUMBER_VIRTUAL_PANEL];  
} i51KitG2VPManage;

typedef struct
{
    // ��Դ�ļ�ָ��
    iFILE SrcFile;
    // ��������ļ�ָ��
    iFILE DesFilePointer;
    // ����󱣴������ļ���·�����ļ���	
    iU16   DesFileName[50];	
    // ͼƬID�����ַ
    iU32 ImageId;
    // ���ͼƬID
    iU16 MaxImageID;
    // ͼƬ����	
    iU16 TotallImage;	
    // �Ѿ�������ͼƬ����
    iU16 CompleteImage;	
    // ���ڽ����ͼƬID
    iU16 GoingImageID;	
    // ������Ȼص�����	
    void (*Process)(iU32  , iU32 , iHANDLE);
    // ������ɻص�����
    void (*Done)(iHANDLE);
	
}i51KitG2ZlibImage;

typedef struct i51ContainerManage
{
	// �򿪵��������
	i51ImageContainer * ContainerHandle;
	// ������ͼƬ���ؽ��ڴ������
	iS32 MemoryCopyImageNum;
	// ������ͼƬ���ؽ��ڴ��ͼƬID
	iU32 * ImageID;
	// ������ͼƬ�ǵڼ��α����ؽ��ڴ��
	iU32 * MemCopyCount;
	// �������Ƿ���еı��
	iU8 Flag;
}i51ContainerManage;

// ͼƬ�����������ĸ���
#define MAX_ContainerManage_NUM	10
// ͼƬ����������
i51ContainerManage ContainerManage[MAX_ContainerManage_NUM];

// ��ǰ��Ļ���
static iU16 ScreenWidth = 0;

// ��ǰ��Ļ�߶�
static iU16 ScreenHeight = 0;

// �Դ�
static iU16* LcdBuffer = 0;

// ��Ļ��Ϣ����
static iU32 * LcdInfoArr;

// ͼƬ����ָ��
//static i51ImageContainer CurrentImageContainer ;
//���ڽ���ͼƬ����ָ��
static i51ImageContainer * ZlibImageContainer ;

// ��ǰ���⻭�������
static i51KitG2VPManage i51CurrentVpManage;
// ��ǰ������⻭��
static i51KitG2VPStructure * i51CurrentVpActivity = iNULL;
// ��ǰ��Ҫ���Ƶ����⻭��
static i51KitG2VPStructure * i51CurrentVpRender = iNULL;
// �Ƿ�ʹ�����⻭����
static iU8 i51UseVirtualPanel = iFALSE;
// �Ƿ���Ƶ��������ı��
static iU8 i51DrawVirtualPanel = iFALSE;
  
// ͼƬ��ת��������
//static iS32 RotateCenterX = 120;
//static iS32 RotateCenterY = 160;

// ����ͼƬ��ʱ��ID
static iU32 TimeIdZlibDecodeImage = iNULL;
// ����ͼƬ���
static iU32 FlagDecodeImage = iNULL;
// ����ͼƬ���ݽṹ
static i51KitG2ZlibImage ManageZlibDecodeImage;

// һ�л���һ�п�ȵ���ɫbuf
static iU16 * OneRowColorBuf = iNULL;

//ϵͳ��ǰ��Alphaֵ
static iALPHA AlphaSys = i51KITG2_ALPHA_NONEED;
// ԭͼ��Alpha
static iALPHA AlphaSrc = i51KITG2_ALPHA_NONEED;
// Ŀ�ĵص�Alpha
static iALPHA AlphaDes = 0;

// sin0~90�Ĳ��ұ����������ǰ���ȷ���ݷŵ���16384����Ҳ����������14λ
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

#if DEBUG_I51KITG2
// �����ñ����������Ƿ��һ�����б��
 static iU8 FLAG_FIRSTRUN  = iTRUE;
iU8 DecodeComplete = iFALSE;
iU8    UseCach = iFALSE;
iHANDLE CurrentImageContainer = iNULL;
iFILE TypeheadHandle = iNULL;
iU32 ZoomRateX = 100;
iU32 ZoomRateY = 100;
static iHANDLE VP_BK = iNULL;
static iHANDLE VP_Road = iNULL;
static iHANDLE VP_Snidget  = iNULL;
static iHANDLE VP_Bee = iNULL;
static iU8 Flag = iTRUE;
#endif

i51KIT_STATIC_KIT_DEF(Adapter);

static iCOLOR i51KitG2GetNewColor (iCOLOR dpixel, iCOLOR spixel)
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
	
    if( AlphaSrc==0XFF ) return spixel;
    if( AlphaSrc==0 ) return dpixel;

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

static iCOLOR i51KitG2GetNewColorEx (iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha)
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

    if( SrcAlpha==0XFF ) return spixel;
    if( SrcAlpha==0 ) return dpixel;
	
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

static  iU16 * MMIAPICOM_Wstrcat( iU16* dst, const iU16 * src  )
{
    // author : Otto
    // since : 2011.8.19
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes:  �ַ������Ӻ���
  
    iU16 * cp = dst;
  
    if( iNULL== src || iNULL == dst )
    {
        return dst;
    }

    // find end of dst
    while( *cp )
    {
        cp++;
    }

     // copy src to end of dst
    while( iNULL != (*cp++ = *src++) ) ;   /* copy src to end of dst */

    // return dst
    return( dst );            
}

#if LCDBUFF_DEBUG
static iBOOL i51KitG2CheckLcdbuffer( iS32 StartBuffer , iS32 Length )
{
    iS32 Max =ScreenHeight*ScreenWidth;

    if( Length<0 )
    {
        iLog( "KIT - G2 : lcdbuffer Len"); 
        return iFALSE;
    }
	
    if( StartBuffer<0||StartBuffer>=Max ||(StartBuffer+Length)>Max)
    {
        iLog( "KIT - G2 : lcdbuffer "); 
        return iFALSE;
    }
    return iTRUE;
}
#endif

static iU16 CountWordsNum( iCHAR * Str)
{
    // ͳ����Ӣ�Ļ����ַ����ַ�����
    iU16 WordsNum = 0;
    iU8 Ch = 0;
    iU16 Temp = 0;
    iS32 StrLength = strlen(Str);
    for( Temp=0; Temp<StrLength; Temp++ )
    {
        WordsNum++;
        Ch = Str[Temp];
        if( Ch>0XA0) Temp++;
    }
    return WordsNum;
}

static iBOOL i51KitG2FontFindKey(iU16 Key, iU16 * KeyPos, i51KitG2Font * CurrTypehead)
{
    // ���ҳ����ֿ��еĺ��֣������ֿ��ǰ��պ��ֵĻ�����Ϊ�ؼ�����������
    // ��Ϊ���������У����Բ����㷨ʹ�õ��Ƕ��ֲ���
    iS16 StartWord =0;
    iS16 EndWord = CurrTypehead->NormalUseWordsNum-1;
    iS16 Temp;
    i51KitFontWord * NormalWordStock = iNULL;

    int Temp1 = 0;
	
    NormalWordStock = ( i51KitFontWord * )(CurrTypehead->NormalWordsEntry);
    if( !NormalWordStock )
    {
        return iFALSE;
    }
	
    while(StartWord<EndWord)
    {	
        Temp1 ++;
        Temp = ((StartWord+EndWord)>>1);
        if( NormalWordStock[Temp].CharactersKeywords>Key)
        {
            EndWord = Temp-1;
        }
        else if( NormalWordStock[Temp].CharactersKeywords<Key)
        {
            StartWord = Temp+1;
        }
        else
        {
            *KeyPos = Temp;
            return iTRUE;
        }
    }

    if( NormalWordStock[StartWord].CharactersKeywords==Key ) 
    {
        * KeyPos = StartWord;
        return iTRUE;
    }
	
    return iFALSE;

}

static iBOOL i51KitG2Create( void )
{ 
    // author : Otto
    // since : Aug 10 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.
  
    // notes : ��ʼ��i51KitG2ϵͳ

    iU32 Temp = 0;
#if i51KITG2_DEBUG
    iLog("Start i51KitG2Create");
#endif
    if( i51AdeMmiGetScreenScale ( &ScreenWidth , &ScreenHeight ) == 0 ) goto ERROR;

    LcdInfoArr =  i51AdeMmiGetPanel();
    if( iNULL == LcdInfoArr ) goto ERROR;

    LcdBuffer = (iU16 *) LcdInfoArr[1] ;
  
    i51CurrentVpManage.ActivityVp_ID = 0;
    i51CurrentVpManage.CurrentVpActivity = iNULL;
    i51CurrentVpManage.CurrentVpRender = iNULL;
    i51CurrentVpManage.MaxVPNumber = I51KITG2_MAX_NUMBER_VIRTUAL_PANEL;
    i51CurrentVpManage.TotalVPNumber = 0;
    i51CurrentVpManage.UseVirtualPanel = iFALSE;
    i51CurrentVpManage.VPDetails[0] = iNULL;

//    CurrentImageContainer.FileHandle  = iNULL;
//    CurrentImageContainer.ImageIndexTable = iNULL;
//    CurrentImageContainer.ImageMaxId = 0;
//    CurrentImageContainer.ImageTotalNumber = 0;

//    ZlibImageContainer.FileHandle  = iNULL;
//    ZlibImageContainer.ImageIndexTable = iNULL;
//    ZlibImageContainer.ImageMaxId = 0;
  //  ZlibImageContainer.ImageTotalNumber = 0;
    ZlibImageContainer = iNULL;

    for( Temp=0; Temp<MAX_TYPEHEAD_NUMBER; Temp++ )
    {
        i51KitG2FontManage[Temp].FontFileHandle = iNULL;
        i51KitG2FontManage[Temp].TypeheadID = iNULL;
        i51KitG2FontManage[Temp].NormalUseWordsNum = iNULL;
        i51KitG2FontManage[Temp].NormalWordsEntry = iNULL;
        i51KitG2FontManage[Temp].NormalWordsOffset = iNULL;
        i51KitG2FontManage[Temp].AllWordsOffset = iNULL;
        i51KitG2FontManage[Temp].TypeheadType = iNULL;
        i51KitG2FontManage[Temp].WordHeight = 0;
        i51KitG2FontManage[Temp].WordWidth = 0;
        i51KitG2FontManage[Temp].TypeheadOffset = 0;
    }

	FlagDecodeImage = iFALSE;

    memset ( ContainerManage, 0 , (sizeof(i51ContainerManage))*MAX_ContainerManage_NUM) ;

    SetMemoryCopyCount = 0;
    CurrContainerManageNum = 0;
    RESUNE_MEMORY_COPY = iFALSE;	
	
    if( ScreenHeight>ScreenWidth)
    {
        OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenHeight<<1),  __FILE__ , __LINE__ );
    }
    else
    {
        OneRowColorBuf = ( iU16 * )i51AdeOsMalloc( (ScreenWidth<<1),  __FILE__ , __LINE__ );
    }
    if( iNULL==OneRowColorBuf)
    {
        iLog("KIT - G2 :init malloc");
        goto ERROR;
    }
	
#if i51KITG2_DEBUG
    iLog("End i51KitG2Create");
#endif
    return iTRUE;
	


ERROR:
    iLog("KIT - G2 :init ");
    return iFALSE;
	
}

static iBOOL i51KitG2Release( void  )
{

    // author : Otto
    // since : Aug 10 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : �˳�i51KitG2ϵͳ

    ScreenWidth = 0;
    ScreenHeight = 0;
    LcdBuffer = 0;

    if(OneRowColorBuf)
    {
        i51AdeOsFree(OneRowColorBuf);
        OneRowColorBuf = iNULL;
    }
	
    return iTRUE;
	
}

static iBOOL i51KitG2CheckFileCRC ( iFILE handle )
{
    iU32 nCRC = 0;
    
    if(iNULL==i51AdeOsFileSetSeek(handle, 0, i51_OS_FL_SEEK_HEAD)) goto ERROR;
    if(iNULL==i51AdeOsFileReadInt32 ( handle , (iU32*)&nCRC)) goto ERROR ;

    if(i51AdeCnfGetEndian()&&nCRC == 9) return iTRUE;
    else if(nCRC==1) return iTRUE;
    else return iFALSE;


ERROR:
    iLog("KIT - G2 : RW check crc");
    return iFALSE;
}

static iBOOL i51KitG2WriteFileCRC ( iFILE handle )
{

    iU32 nCRC = 1 ;

    if(i51AdeCnfGetEndian()) nCRC = 9;
    else nCRC = 1;

    if(iNULL==i51AdeOsFileSetSeek(handle, 0, i51_OS_FL_SEEK_HEAD)) goto ERROR;
    i51AdeOsFileWriteInt32( handle, &nCRC);
    i51AdeOsFileFlush(handle);

    return iTRUE;

ERROR:
    iLog("KIT - G2 : RW write crc");
    return iFALSE;
	
}

static  void QuickSort1(iS16 pData[],iU16 left,iU16 right)
{

    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : ����pData����������
#if 0
    iS16 i,j;
    iS16 middle,iTemp;
    i = left;
    j = right;
    middle = pData[left];
    do{
      while( (pData[i]<middle) && (i<right) )//����ɨ��С����ֵ����
      i++;
      while( ( pData[j]>middle) && (j>left) )//����ɨ�������ֵ����
      j--;
      if(i<=j)//�ҵ���һ��ֵ
     {
      //����
      iTemp = pData[i];
      pData[i] = pData[j];
      pData[j] = iTemp;
      i++;
      j--;
     }
    }while(i<=j);//�������ɨ����±꽻��,��ֹͣ�����һ�Σ�
    //����߲�����ֵ(left<j),�ݹ�����
    if(left<j)
    QuickSort1(pData,left,j);
    //���ұ߲�����ֵ(right>i),�ݹ��Ұ��
    if(right>i)
    QuickSort1(pData,i,right);
#endif	
#if 1
// ѡ������
	iS16 i,j;
	iS16 MinData,MinPos;
	iS16 Temp;

	for(i=0;i<=right;i++)
	{
		MinData= pData[i];
		MinPos = i;
		for(j=i+1;j<=right;j++)
		{
			if(pData[j]<MinData)
			{
				MinData = pData[j];
				MinPos = j;
			}
		}
		
		if(MinPos==i) continue;
		
		Temp = pData[i];
		pData[i] = pData[MinPos];
		pData[MinPos] = Temp;		
	}
	
#endif
}

static  void QuickSort2(iS16 pData[][2],iU16 left,iU16 right)
{

	// author : Otto
	// since : Aug 15 2011
	// (C) 2011 , PKIG Tech. Co., Ltd.

	// notes : ����pData��pData[][1]��������
#if 0
	iS16 TempLeft = 0, TempRight = 0;
	iS16 MiddleData = 0, TempData;
	MiddleData = pData[left][1];
	TempLeft = left;
	TempRight = right;

	while(TempLeft<TempRight)
	{
		//����ɨ�������ֵ����
		while((pData[TempLeft][1]<=MiddleData)&&(TempLeft<right)) TempLeft++;

		//����ɨ��С����ֵ����
		while((pData[TempRight][1]>=MiddleData)&&(TempRight>left)) TempRight--;

		// ����
		if( TempLeft<TempRight)
		{
			TempData= pData[TempRight][0];
			pData[TempRight][0] = pData[TempLeft][0];
			pData[TempLeft][0] = TempData;
			
			TempData = pData[TempRight][1];
			pData[TempRight][1] = pData[TempLeft][1];
			pData[TempLeft][1] = TempData;

			TempLeft++;
			TempRight--;
		}
		
	}

	if(TempRight>left) 	QuickSort2(pData, left,TempRight);

	if(TempLeft<right) 	QuickSort2(pData, TempLeft,right);
#endif	
#if 1
// ѡ������
	iS16 i,j;
	iS16 MinData,MinPos;
	iS16 Temp;

	for(i=0;i<=right;i++)
	{
		MinData= pData[i][1];
		MinPos = i;
		for(j=i+1;j<=right;j++)
		{
			if(pData[j][1]<MinData)
			{
				MinData = pData[j][1];
				MinPos = j;
			}
		}
		
		if(MinPos==i) continue;
		
		Temp = pData[i][0];
		pData[i][0] = pData[MinPos][0];
		pData[MinPos][0] = Temp;
		
		Temp = pData[i][1];
		pData[i][1] = pData[MinPos][1];
		pData[MinPos][1] = Temp;
		
	}
#endif
}

static iS32 i51KitG2Sin( iS32 Angle)
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

static iS32 i51KitG2Cos( iS32 Angle)
{
    //iS32 Angle: ��Ҫ����ĽǶ�ֵ
    // notes : ���ؽǶ�Angle�������Ŵ�10000����ֵ
    return i51KitG2Sin(Angle+90);
}

static iBOOL i51KitDrawWaterLine( iS32 Point[][2], iRECT VPDrawRect, iRECT VPActualRect, iCOLOR*ColorBuf)
{
// ����һ��ˮƽ��
// DrawRect��Ҫ����Ļ�ϻ��Ƶ�����
// ActualRect,VP��ʵ�ʴ�С�������Ļ��ƫ��
//  Point[][2],ˮƽ�ߵ������յ�����
    iS32 DrawX1, DrawX2;
    // ��������	
    iS16 DrawStartX = 0;
    iS16 DrawStartY = 0;
    iS16 DrawWidth = 0;
    iS32 BuffPos = 0;
    iU32 iTemp = 0;
	
    DrawX1 = Point[0][0];
    DrawX2 = Point[1][0];

	if( DrawX2<DrawX1 ) return iFALSE;
   
    if(!i51UseVirtualPanel)
    {
        // ������ֱ���������
        DrawStartX = VPDrawRect.X;
        DrawStartY = Point[0][1];
    }
    else
    {
        // ��������VP��,����ͼ�νӿ�VP�Ƿ�ѭ��������ѭ������
        DrawStartX = VPDrawRect.X + VPActualRect.X;
        DrawStartY = Point[0][1]+ VPActualRect.Y;
    }

    DrawWidth = VPDrawRect.Width;
	
    // �����������Ļû�н���
    if( DrawStartY>=ScreenHeight  || DrawStartY>=(VPDrawRect.Y+VPDrawRect.Height)
    || DrawStartY< VPDrawRect.Y || DrawStartY< 0  || DrawStartX>=ScreenWidth ) return iTRUE;
    // ������ˮƽ�����������û�н���
    if( DrawX1>=(DrawStartX+DrawWidth) || DrawX2<DrawStartX)  return iTRUE;
    // ����ˮƽ�߽������ڻ��������ұ�
    if( DrawX2 > (DrawStartX+DrawWidth ) ) DrawX2 = DrawStartX + DrawWidth;
    // ����ˮƽ�߿�ʼ���ڻ����������
    if( DrawX1<DrawStartX )  DrawX1= DrawStartX;
    // ����ˮƽ�߿�ʼ������Ļ���
    if( DrawX1<0 )  DrawX1 = 0;

    // ����ˮƽ��		
    DrawWidth = DrawX2- DrawX1;
    if( DrawWidth<1 ) return iTRUE;
    BuffPos = ScreenWidth * DrawStartY+ DrawX1;		
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(BuffPos,DrawWidth);
#endif
    if(AlphaSys==i51KITG2_ALPHA_NONEED)
    {
        i51AdeStdMemcpy16(&LcdBuffer[BuffPos], ColorBuf, (DrawWidth<<1));
    }
    else
    {
        for( iTemp=0; iTemp<DrawWidth; iTemp++ )
        {
            LcdBuffer[BuffPos]=i51KitG2GetNewColor(LcdBuffer[BuffPos], ColorBuf[iTemp]);
            BuffPos++;
        }
    }
    return iTRUE;

}

static iBOOL i51KitDrawDottedLine( iS16 Y, iU16 Num, iS16 X[], iCOLOR * ColorBuf )
{

    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU16 Y : �������ߵ�Y����
    // iU16 Num : �������ߵĶ������
    // iU16 X[] : �������� ��X���꼯
    // iCOLOR Color : �������ߵ���ɫ
  
    // notes : ����ˮƽ����
  
    iU32 i = 0, j = 1;//, k = 0;
    // ��ǰ���Ƶ�Ԫ����VP�ϵ�ID
//    iU16 CurrDrawID;
    // ������VPʵ�ʴ�С	
    iRECT VPActualRect;
    // ��Ҫ���Ƶ�VP����	
    iRECT VPDrawRect;
    iS32 Point[2][2];	

    if( i51UseVirtualPanel )
    {
        VPActualRect.X = i51CurrentVpRender->VPActualRect.X;
        VPActualRect.Y = i51CurrentVpRender->VPActualRect.Y;
        VPActualRect.Width= i51CurrentVpRender->VPActualRect.Width;
        VPActualRect.Height= i51CurrentVpRender->VPActualRect.Height;

        VPDrawRect.X = i51CurrentVpRender->VPDrawRect.X;
        VPDrawRect.Y = i51CurrentVpRender->VPDrawRect.Y;
        VPDrawRect.Width = i51CurrentVpRender->VPDrawRect.Width;
        VPDrawRect.Height= i51CurrentVpRender->VPDrawRect.Height;
    }
    else
    {
        VPActualRect.X = 0;
        VPActualRect.Y = 0;
        VPActualRect.Width = ScreenWidth;
        VPActualRect.Height = ScreenHeight;

        VPDrawRect.X = 0;
        VPDrawRect.Y = 0;
        VPDrawRect.Width = ScreenWidth;
        VPDrawRect.Height = ScreenHeight;
    }
	
	
   QuickSort1( X, 0, Num-1);

#if 1
    for( i=1; i<Num; i++ )
    {
        if( X[i]!=X[j-1] )
        {
            X[j++] = X[i];
        }
    }
#endif

    Point[0][1] = Y;
    Point[1][1] = Y;
//j=Num;
    for( i=0; i<j-1; i+=2 )
    {
        Point[0][0] = (iS32)X[i];
        Point[1][0] = (iS32)X[i+1];
        i51KitDrawWaterLine(Point, VPDrawRect, VPActualRect, ColorBuf);		
    }
  
    if( (j%2) )
    {
// ʣ��һ��������Ҫ����
    }
	
    return iTRUE;
	
}

static iBOOL ZlibDecodeInit( void )
{
    iBOOL FlagReturn = iFALSE;
    iU8   FlagImageExist = 0;	
    iU8   ContainerVersion = 0;
    iU32  SaveFileOffset = 0;
    iU32  iTemp = 0;
    iU32  WrittenDataLength = 0;
//    iU32  NumerRam = 0;
    iU32 MaxImageID = 0;
    iU32 CRC = 0;
    iU32 FileVer = 0;
    iU16 TempWrite16 = 0;
    iU32 TempWrite32 = 0;

    if( iNULL==ManageZlibDecodeImage.SrcFile)
    {
        iLog("KIT - G2 : image resource decode init");
        return iFALSE;
    }

    MaxImageID = ManageZlibDecodeImage.MaxImageID;
    // ����ͼƬʱ��ʼ������  
    ManageZlibDecodeImage.DesFilePointer =  i51AdeOsFileOpen( ManageZlibDecodeImage.DesFileName, i51_OS_FL_WRITE |i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__ );
    if( ManageZlibDecodeImage.DesFilePointer == iNULL ) 
    {
        iLog("KIT - G2 :Open file decode init");
        return iFALSE;
    }

    TempWrite32 = CRC;
    FlagReturn = i51AdeOsFileWriteInt32( ManageZlibDecodeImage.DesFilePointer,  &TempWrite32);
    if( iFALSE == FlagReturn ) goto ERROR;
	
    FlagReturn =  i51AdeOsFileWrite(ManageZlibDecodeImage.DesFilePointer, "i51C", 4, &WrittenDataLength);
    if( iFALSE == FlagReturn ) goto ERROR;

    ContainerVersion  = I51KITG2_IMAGE_CONTAINER_VERSION;
    FlagReturn =  i51AdeOsFileWriteInt8(ManageZlibDecodeImage.DesFilePointer, &ContainerVersion);
    if( iFALSE == FlagReturn ) goto ERROR;
	
    TempWrite32 = FileVer;
    FlagReturn = i51AdeOsFileWriteInt32( ManageZlibDecodeImage.DesFilePointer,  &TempWrite32);
    if( iFALSE == FlagReturn ) goto ERROR;
	
    TempWrite16 = ManageZlibDecodeImage.TotallImage;
    FlagReturn = i51AdeOsFileWriteInt16( ManageZlibDecodeImage.DesFilePointer,  &TempWrite16);
    if( iFALSE == FlagReturn ) goto ERROR;
  
    TempWrite16 = ManageZlibDecodeImage.MaxImageID;
    FlagReturn = i51AdeOsFileWriteInt16( ManageZlibDecodeImage.DesFilePointer,  &TempWrite16);
    if( iFALSE == FlagReturn ) goto ERROR;
  
    SaveFileOffset = 0X0;
    FlagImageExist = 0;
    for( iTemp=0; iTemp<MaxImageID; iTemp++ )
    {
  
        FlagReturn = i51AdeOsFileWriteInt8( ManageZlibDecodeImage.DesFilePointer,  &FlagImageExist);
        if( iFALSE == FlagReturn ) goto ERROR;
  
        TempWrite32 = SaveFileOffset;
        FlagReturn = i51AdeOsFileWriteInt32( ManageZlibDecodeImage.DesFilePointer,  &TempWrite32);
        if( iFALSE == FlagReturn ) goto ERROR;
  
    }

    ZlibImageContainer = ( i51ImageContainer* )i51AdeOsMalloc( sizeof(i51ImageContainer), __FILE__ , __LINE__ );
    if( iNULL==ZlibImageContainer )
    {
        iLog("KIT - G2 : malloc ZlibImageContainer");
        goto ERROR;
    }
    memset(ZlibImageContainer, 0, sizeof(i51ImageContainer) );
	
    ZlibImageContainer->ImageTotalNumber= ManageZlibDecodeImage.TotallImage;
    ZlibImageContainer->FileHandle = ManageZlibDecodeImage.DesFilePointer;
    ZlibImageContainer->ImageMaxId = ManageZlibDecodeImage.MaxImageID;

    ZlibImageContainer->ImageIndexTable = ( i51ImageIndex * )i51AdeOsMalloc((sizeof(i51ImageIndex))*(MaxImageID), (__FILE__) , __LINE__ );
    if( iFALSE == ZlibImageContainer->ImageIndexTable )
    {
        iLog("KIT - G2 :malloc decode init");
        goto ERROR;
    }
    memset(ZlibImageContainer->ImageIndexTable, 0, sizeof(i51ImageIndex)*MaxImageID );
	
    for( iTemp = 0; iTemp<MaxImageID; iTemp++ )
    {
        ZlibImageContainer->ImageIndexTable[iTemp].FlagImageExist = 0;
        ZlibImageContainer->ImageIndexTable[iTemp].FlagCopyRAM = iFALSE;
    }

    i51AdeOsFileFlush(ManageZlibDecodeImage.DesFilePointer);

    iTemp = 0;	 
    while(1)
    {
        if( iFALSE==ContainerManage[iTemp].Flag )
        {
    	     ContainerManage[iTemp].Flag = iTRUE;
            ContainerManage[iTemp].ContainerHandle = ZlibImageContainer;
//iLog("open ContainerManage[%d].ContainerHandle:%d",iTemp,ContainerManage[iTemp].ContainerHandle);
            break;
        }
        iTemp++;
        if( iTemp>=MAX_ContainerManage_NUM )		
        {
            iLog("KIT - G2 : contain full");
            goto ERROR;			
        }
    }
	
    return iTRUE;

ERROR:
    if( ZlibImageContainer )
    {
        i51AdeOsFree(ZlibImageContainer->ImageIndexTable);
        ZlibImageContainer->ImageIndexTable = iNULL;
    	
        i51AdeOsFree(ZlibImageContainer);
        ZlibImageContainer = iNULL;
    }	
    iLog("KIT - G2 : RW decode init");
    return iFALSE;
	
}

static iBOOL ZlibDecodeOneImageFromFile( iFILE SrcFile , iU32 SrcSeek, iFILE DesFile, iU32 DesSeek, iHANDLE Container, iU32 ImageID )
{
    // author : Otto
    // since : Aug 31 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE SrcFile : ��Ҫ��������ݴ���ļ�ָ��
    // iU32 SrcSeek : ��Ҫ��������ݴ����ԭʼ�ļ��еĿ�ʼ��ַ
    // iFILE DesFile : ���������ݴ���ļ�ָ��
    // iU32 DesFile : ���������ݴ��Ŀ���ļ��еĿ�ʼ��ַ
    // iHANDLE Container : �������
    // iU32 ImageID : ��ǰ�����ͼƬ�������ļ��е�ͼƬID
    
    // notes : ���ļ�SrcFile�н�ѹһ��ͼƬ���ļ�DesFile��
    // SrcFile�ļ��д�ŵ�ͼƬ�����Ƿֶ���Zlibѹ����
    // �����ļ���ʽ��ο��ĵ�"The_i51_DC_dynamic_library_G2_i51_Image_Format.xls"

    // ���ļ��ж�ȡ��CRC
    iU32 CRC_Read = 0;
    // Ŀ�����ݳ��ȣ�����ѹ���ļ�����
    iU32 LengthDesData = 0;
    // ԭʼ���ݳ��ȣ���ѹ�����ļ�����
    iU32 LengthSrcData = 0;
    // ��ѹ��������,Ĭ������Ϊ32K
    iU32 LengthPartMax = 0;
    // ������
    iU16 NumberParts = 0;
    // ��ǰ�ν�ѹǰ���ݳ���
    iU32 LengthPartSrc = 0;
    // ��ǰ�ν�ѹ�����ݳ���
    iU32 LengthPartDes = 0;
    // ��������ֵ
    iBOOL FlagReturn = iFALSE;
    // ����ǰͼƬ���ݿ�ʼλ�� ����Դ�ļ��е�ƫ����
    iU32 SeekFileResource = 0;
    // �����ͼƬ���ݿ�ʼλ�� �������ļ��е�ƫ����
    iU32 SeekFileContainer = 0;
    // �ļ���ǰƫ����
//    iU32 FileSeekCureent = 0;
    iU32 LengFactReadData = 0;
    // ʵ��һ��д���ļ��е����ݳ���  
    iU32 LengFactWriteData = 0;
    iU32 iTemp = 0;
    iU32 GoingImageID = ImageID;	
    iU8 FlagImageExist = 1;

    // ��ѹǰ������buffer
    iU8  * DataSrcCurrentPart = iNULL;
    // ��ѹ�������buffer
    iU8  * DataDesCurrentPart = iNULL;
    iU32 DesTempPos = 0;	
    iU32 SrcTempPos = 0;	

    // ͼƬ����ɫ��
    iU8 PaletteImage = iFALSE;
    // ��ɫ������ɫ����
    iU16 PaletteColorNum = 0;
    // ��ɫ����ɫ������
    iCOLOR * PaletteColorMem = iNULL;
    // һ�����ݿ�ĳ���	:=6+���������
    iS32 BlockLength = 0;
    // ��ǰ���ݿ�λ��
    iS32 BlockPlace = 0;
    // ��ɫ��ʱ����	
//    iCOLOR TempColor = 0;
    // ��ɫָ���ַ
    iCOLOR * ColorPoint = iNULL;
    // ������ͷ	
    iU8 TempHead[6];
    iS32 Temp = 0;	

    i51ImageContainer * ImageContainer = (i51ImageContainer *)Container;


    // �ƶ��ļ���ָ�뵽�����ļ�DesSeekλ��
    SeekFileContainer = DesSeek;
    FlagReturn = i51AdeOsFileSetSeek( DesFile, DesSeek, i51_OS_FL_SEEK_HEAD) ;
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }

    // ��õ�ǰͼƬ������ԭʼ�ļ��е�ƫ����	
    SeekFileResource =  SrcSeek;

    // �ƶ��ļ���ָ�뵽�ض���Դ���ݿ�ʼ��
    FlagReturn = i51AdeOsFileSetSeek( SrcFile , SeekFileResource, i51_OS_FL_SEEK_HEAD ) ;
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }

    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &CRC_Read);
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }

    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &LengthDesData);
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }

    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &LengthSrcData);
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }
  
  
    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &LengthPartMax);
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }
  
    FlagReturn = i51AdeOsFileReadInt16( SrcFile, (iU16 *)(&NumberParts));
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }

    if( NumberParts==1 && (LengthDesData+514)<=I51KITG2_LENGTH_PART_UMCOMPRESS )
    {
        // LengthDesData�ǽ���������ļ����ȣ����i51G��8λͼ����LengthDesData�ǽ��������16λͼ�ĳ��ȣ�Ҳ����ȥ���˵�ɫ����Ϣ���±��Ѿ��滻������ɫֵ
        // ��ɫ����󳤶�Ϊ2+256*2=514���ֽ�
        DataDesCurrentPart = i51AdeOsMalloc( LengthDesData+514, __FILE__ , __LINE__ );
    }
    else
    {
        DataDesCurrentPart = i51AdeOsMalloc(I51KITG2_LENGTH_PART_UMCOMPRESS, __FILE__ , __LINE__ );
    }
    if( iFALSE == DataDesCurrentPart  ) 
    {
        iLog("KIT - G2 : malloc src decode one ");
        goto ERROR;
    }

    for( iTemp=0; iTemp<NumberParts; iTemp++ )
    {
        DesTempPos = 0;
		
        FlagReturn = i51AdeOsFileReadInt32( SrcFile, (iU32 *)(&LengthPartSrc));
        if( iFALSE == FlagReturn )
        {
            goto ERROR;
        }

        if( iNULL==DataSrcCurrentPart)
        {
            if( NumberParts==1 && (LengthDesData+514)<=I51KITG2_LENGTH_PART_UMCOMPRESS*2 )
            {
                // LengthDesData�ǽ���������ļ����ȣ����i51G��8λͼ����LengthDesData�ǽ��������16λͼ�ĳ��ȣ�Ҳ����ȥ���˵�ɫ����Ϣ���±��Ѿ��滻������ɫֵ
                // ��ɫ����󳤶�Ϊ2+256*2=514���ֽ�
                if( LengthPartSrc>LengthDesData)
                {
                    DataSrcCurrentPart = i51AdeOsMalloc(LengthPartSrc+514, __FILE__ , __LINE__ );
                }
                else
                {
                    DataSrcCurrentPart = i51AdeOsMalloc(LengthDesData+514, __FILE__ , __LINE__ );
                }
            }
            else
            {
                DataSrcCurrentPart = i51AdeOsMalloc(I51KITG2_LENGTH_PART_UMCOMPRESS*2, __FILE__ , __LINE__ );
            }
            if( iNULL == DataSrcCurrentPart )
            {
                iLog("KIT - G2 : malloc des decode one");
                goto ERROR;
            }
        }
			

        FlagReturn = i51AdeOsFileRead( SrcFile, DataSrcCurrentPart, LengthPartSrc, &LengFactReadData);
        if( iFALSE == FlagReturn )
        {
            goto ERROR;
        }

        LengthPartDes = I51KITG2_LENGTH_PART_UMCOMPRESS;
        FlagReturn = i51AdeStdUncompress( DataDesCurrentPart, &LengthPartDes, DataSrcCurrentPart, LengthPartSrc);
        if( iFALSE == FlagReturn )
        {
            iLog("KIT - G2 : Uncompress");
            goto ERROR;
        }

        if( iTemp==0 )
        {
            DesTempPos = I51KITG2_OFFSET_PIXELDATA;
            if(DataDesCurrentPart[0]!='i'||DataDesCurrentPart[1]!='5'||DataDesCurrentPart[2]!='1'||DataDesCurrentPart[3]!='G')
            {
                iLog("KIT - G2 : file form");
                goto ERROR;
            }

            if( DataDesCurrentPart[4]>0X40)
            {
                PaletteImage = iTRUE;
                PaletteColorNum = (DataDesCurrentPart[I51KITG2_OFFSET_PIXELDATA]) | (DataDesCurrentPart[I51KITG2_OFFSET_PIXELDATA+1]<<8); 
                PaletteColorMem = i51AdeOsMalloc(PaletteColorNum*2, __FILE__ , __LINE__ );
                if ( iNULL==PaletteColorMem )
                {
                    iLog("KIT - G2 : malloc PaletteColor" );
                    goto ERROR;
                }
                i51AdeStdMemcpy16(PaletteColorMem, &(DataDesCurrentPart[I51KITG2_OFFSET_PIXELDATA+2]), PaletteColorNum*2 );
				
            }
			
            if( iNULL==ImageContainer )
            {
                iLog("KIT - G2 : ZlibImageContainer null" );
		   goto ERROR;
            }
            ImageContainer->ImageIndexTable[GoingImageID].DecodeType = DataDesCurrentPart[I51KITG2_OFFSET_DECODETYPE];
            if(ImageContainer->ImageIndexTable[GoingImageID].DecodeType!=I51KITG2_DECODE_LINEAR)
            {
                iLog("KIT - G2 : ����ͼƬ��ʽ");
                goto ERROR;				
            }
            ImageContainer->ImageIndexTable[GoingImageID].ImageHeight= (DataDesCurrentPart[I51KITG2_OFFSET_IMAGEHEIGHT])|(DataDesCurrentPart[I51KITG2_OFFSET_IMAGEHEIGHT+1]<<8);
            ImageContainer->ImageIndexTable[GoingImageID].ImageWidth= (DataDesCurrentPart[I51KITG2_OFFSET_IMAGEWIDTH])|(DataDesCurrentPart[I51KITG2_OFFSET_IMAGEWIDTH+1]<<8);
            ImageContainer->ImageIndexTable[GoingImageID].AlphaPixelNum= (DataDesCurrentPart[I51KITG2_OFFSET_ALPHANUMER])|(DataDesCurrentPart[I51KITG2_OFFSET_ALPHANUMER+1]<<8);
            ImageContainer->ImageIndexTable[GoingImageID].LengthIamgeData =
   		 	(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH])|(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH+1]<<8)
   		     |(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH+2]<<16)|(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH+3]<<24);
            ImageContainer->ImageIndexTable[GoingImageID].LengthTotalData = ImageContainer->ImageIndexTable[GoingImageID].LengthIamgeData ;
            if( ImageContainer->ImageIndexTable[GoingImageID].AlphaPixelNum>0)
            {
                ImageContainer->ImageIndexTable[GoingImageID].LengthTotalData+=ImageContainer->ImageIndexTable[GoingImageID].AlphaPixelNum*8 + 12;
            }

        }

        if(PaletteImage)
        {
            //���������ͼƬ��Ҫ�Ȱ�����תΪ��ɫ
            if( iTemp==0 )
            {
//                ZlibImageContainer->ImageIndexTable[GoingImageID].LengthIamgeData = 0;
                i51AdeStdMemcpy16(DataSrcCurrentPart, DataDesCurrentPart, I51KITG2_OFFSET_ALPHANUMER);
                DataSrcCurrentPart[I51KITG2_OFFSET_ALPHANUMER] = DataDesCurrentPart[I51KITG2_OFFSET_ALPHANUMER+1];
                DataSrcCurrentPart[I51KITG2_OFFSET_ALPHANUMER+1] = DataDesCurrentPart[I51KITG2_OFFSET_ALPHANUMER];
                DesTempPos = I51KITG2_OFFSET_PIXELDATA + PaletteColorNum*2 +2;
                SrcTempPos = I51KITG2_OFFSET_PIXELDATA;
                BlockLength = 0;
                BlockPlace = 0;
            }
            else
            {
                DesTempPos = 0;
                SrcTempPos = 0;
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
                        BlockLength = DataDesCurrentPart[DesTempPos+4] | (DataDesCurrentPart[DesTempPos+5]<<8);
                        i51AdeStdMemcpy16(&DataSrcCurrentPart[SrcTempPos], &DataDesCurrentPart[DesTempPos], 6);
                        i51AdeStdMemcpy16(TempHead, &DataDesCurrentPart[DesTempPos], 6);
                        DesTempPos += 6;
                        SrcTempPos += 6;
                    }
                    else
                    {
                        // ��ʣ������С�ڿ�ͷ���ȣ���ʣ�����ݱ���
                        BlockPlace = LengthPartDes-DesTempPos;
                        for( Temp = 0; Temp < BlockPlace; Temp++ )
                        {
                            TempHead[Temp] = DataDesCurrentPart[DesTempPos+Temp];
                        }
                        break; 						
                    }
                }
                else if( BlockPlace<6 )				
                {
                    for( Temp = BlockPlace; Temp < 6; Temp++ )
                    {
                        TempHead[Temp] = DataDesCurrentPart[DesTempPos+Temp-BlockPlace];
                    }
                    i51AdeStdMemcpy16(&DataSrcCurrentPart[SrcTempPos], TempHead, 6);
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    DesTempPos += 6-BlockPlace;
                    SrcTempPos += 6;
                    BlockPlace = 6;
                }
                else
                {
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    BlockLength -= BlockPlace - 6;
                }

                ColorPoint = (iCOLOR*)(((iU32)DataSrcCurrentPart)+SrcTempPos);

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
                    ColorPoint[Temp] = PaletteColorMem[DataDesCurrentPart[DesTempPos]];
                    DesTempPos++;
                    SrcTempPos += 2;
                }

            }

            if(i51AdeCnfGetEndian()==1)
            {
                // ��˵Ļ�����Ҫ����ɫֵ������ֵ�ĸߵ�λ����
                if( iTemp==0 )
                {
                	Temp = I51KITG2_OFFSET_PIXELDATA;
                }
                else
                {
                	Temp = 0;
                }
				
                while(Temp<SrcTempPos)
                {
                    DataSrcCurrentPart[Temp] += DataSrcCurrentPart[Temp+1];
                    DataSrcCurrentPart[Temp+1] = DataSrcCurrentPart[Temp] - DataSrcCurrentPart[Temp+1];
                    DataSrcCurrentPart[Temp] = DataSrcCurrentPart[Temp] - DataSrcCurrentPart[Temp+1];
                    Temp += 2;
                }
            }
            FlagReturn = i51AdeOsFileWrite( DesFile, DataSrcCurrentPart, SrcTempPos, &LengFactWriteData );
            if( iFALSE == FlagReturn ) goto ERROR;
			
        }
        else
        {
            if(i51AdeCnfGetEndian()==1)
            {
                // ��˵Ļ�����Ҫ����ɫֵ������ֵ�ĸߵ�λ����
                while(DesTempPos<LengthPartDes)
                {
                    DataDesCurrentPart[DesTempPos] += DataDesCurrentPart[DesTempPos+1];
                    DataDesCurrentPart[DesTempPos+1] = DataDesCurrentPart[DesTempPos] - DataDesCurrentPart[DesTempPos+1];
                    DataDesCurrentPart[DesTempPos] = DataDesCurrentPart[DesTempPos] - DataDesCurrentPart[DesTempPos+1];
                    DesTempPos += 2;
                }
            }
            FlagReturn = i51AdeOsFileWrite( DesFile, DataDesCurrentPart, LengthPartDes, &LengFactWriteData );
            if( iFALSE == FlagReturn ) goto ERROR;
        }
    }

    ImageContainer->ImageIndexTable[GoingImageID].FlagImageExist = 1;
    ImageContainer->ImageIndexTable[GoingImageID].FlagCopyRAM = iFALSE;
    ImageContainer->ImageIndexTable[GoingImageID].ImageEntry = SeekFileContainer;

    FlagReturn = i51AdeOsFileSetSeek( DesFile , ( I51KITG2_OFFSET_START_DATA + ImageID*5 ), i51_OS_FL_SEEK_HEAD ) ;
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }

    FlagImageExist = 1;
    FlagReturn = i51AdeOsFileWriteInt8( DesFile, &FlagImageExist);
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }

    FlagReturn = i51AdeOsFileWriteInt32( DesFile, &SeekFileContainer );
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }
		
    FlagReturn = i51AdeOsFileSetSeek( DesFile , 0, i51_OS_FL_SEEK_TAIL) ;
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }

    i51AdeOsFileFlush( DesFile );

    if(PaletteColorMem)
    {
        i51AdeOsFree(PaletteColorMem);
        PaletteColorMem = iNULL;
    }
    i51AdeOsFree(DataDesCurrentPart);
    DataDesCurrentPart = iNULL;
    i51AdeOsFree(DataSrcCurrentPart);
    DataSrcCurrentPart = iNULL;

    return iTRUE;

    ERROR:
        iLog("KIT - G2 : decode one : %d",GoingImageID);
        if(PaletteColorMem)
        {
            i51AdeOsFree(PaletteColorMem);
            PaletteColorMem = iNULL;
        }
        i51AdeOsFree(DataDesCurrentPart);
        DataDesCurrentPart = iNULL;
        i51AdeOsFree(DataSrcCurrentPart);
        DataSrcCurrentPart = iNULL;
        return iFALSE;

}

static iBOOL ZlibDecodeOneImageFromMemory( iHANDLE SrcData, iFILE DesFile, iU32 DesSeek, iHANDLE Container, iU32 ImageID )
{
    // author : Otto
    // since : Aug 31 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE SrcData : ��Ҫ������������ڴ��еĵ�ַ
    // iFILE DesFile : ���������ݴ���ļ�ָ��
    // iU32 DesFile : ���������ݴ��Ŀ���ļ��еĿ�ʼ��ַ
    // iHANDLE Container : �������
    // iU32 ImageID : ��ǰ�����ͼƬ�������ļ��е�ͼƬID
    
    // notes : ���ļ�SrcFile�н�ѹһ��ͼƬ���ļ�DesFile��
    // SrcFile�ļ��д�ŵ�ͼƬ�����Ƿֶ���Zlibѹ����
    // �����ļ���ʽ��ο��ĵ�"The_i51_DC_dynamic_library_G2_i51_Image_Format.xls"

    // ���ļ��ж�ȡ��CRC
//    iU32 CRC_Read = 0;
    // Ŀ�����ݳ��ȣ�����ѹ���ļ�����
    iU32 LengthDesData = 0;
    // ԭʼ���ݳ��ȣ���ѹ�����ļ�����
    iU32 LengthSrcData = 0;
    // ��ѹ��������,Ĭ������Ϊ32K
//    iU32 LengthPartMax = 0;
    // ������
    iU16 NumberParts = 0;
    // ��ǰ�ν�ѹǰ���ݳ���
    iU32 LengthPartSrc = 0;
    // ��ǰ�ν�ѹ�����ݳ���
    iU32 LengthPartDes = 0;
    // ��������ֵ
    iBOOL FlagReturn = iFALSE;
    // ����ǰͼƬ���ݿ�ʼλ�� ����Դ�ļ��е�ƫ����
//    iU32 SeekFileResource = 0;
    // �����ͼƬ���ݿ�ʼλ�� �������ļ��е�ƫ����
    iU32 SeekFileContainer = 0;
    // �ļ���ǰƫ����
//    iU32 FileSeekCureent = 0;
//    iU32 LengFactReadData = 0;
    // ʵ��һ��д���ļ��е����ݳ���  
    iU32 LengFactWriteData = 0;
    iU32 iTemp = 0;
    iU32 GoingImageID = ImageID;	
    iU8 FlagImageExist = 1;

    // ��ѹ�������buffer
    iU8  * DataDesCurrentPart = iNULL;
    // ��ѹ���λͼ����תΪ16λbuff
    iU8  * DataDesTempPart = iNULL;
    iU32 DesTempPos = 0;	
//    iU32 SrcTempPos = 0;	

    // ͼƬ����ɫ��
    iU8 PaletteImage = iFALSE;
    // ��ɫ������ɫ����
    iU16 PaletteColorNum = 0;
    // ��ɫ����ɫ������
    iCOLOR * PaletteColorMem = iNULL;
    // һ�����ݿ�ĳ���	:=6+���������
    iS32 BlockLength = 0;
    // ��ǰ���ݿ�λ��
    iS32 BlockPlace = 0;
    // ��ɫ��ʱ����	
//    iCOLOR TempColor = 0;
    // ��ɫָ���ַ
    iCOLOR * ColorPoint = iNULL;
    // ������ͷ	
    iU8 TempHead[6];
    iS32 Temp = 0;	
    iU32 SrcPos = 0;
    iU32 DesPos = 0;	
    iU8 * SrcDataPoint = iNULL;

    i51ImageContainer * ImageContainer = (i51ImageContainer *)Container;
    if( iNULL ==ImageContainer )
    {
        iLog("KIT - G2 : decode F mem Container");
        goto ERROR;		
    }

    if( iNULL ==SrcData)
    {
        iLog("KIT - G2 : decode F mem SrcData");
        goto ERROR;		
    }

    SrcDataPoint = (iU8*)SrcData;
	
    // �ƶ��ļ���ָ�뵽�����ļ�DesSeekλ��
    SeekFileContainer = DesSeek;
    FlagReturn = i51AdeOsFileSetSeek( DesFile, DesSeek, i51_OS_FL_SEEK_HEAD) ;
    if( iFALSE == FlagReturn )
    {
		goto ERROR;
    }

    LengthDesData = SrcDataPoint[4]|(SrcDataPoint[5]<<8)|(SrcDataPoint[6]<<16)|(SrcDataPoint[7]<<24);
    LengthSrcData = SrcDataPoint[8]|(SrcDataPoint[9]<<8)|(SrcDataPoint[10]<<16)|(SrcDataPoint[11]<<24);
    NumberParts = SrcDataPoint[16]|(SrcDataPoint[17]<<8);

    if( NumberParts==1 && (LengthDesData+514)<=I51KITG2_LENGTH_PART_UMCOMPRESS )
    {
        // LengthDesData�ǽ���������ļ����ȣ����i51G��8λͼ����LengthDesData�ǽ��������16λͼ�ĳ��ȣ�Ҳ����ȥ���˵�ɫ����Ϣ���±��Ѿ��滻������ɫֵ
        // ��ɫ����󳤶�Ϊ2+256*2=514���ֽ�
        DataDesCurrentPart = i51AdeOsMalloc( LengthDesData+514, __FILE__ , __LINE__ );
    }
    else
    {
        DataDesCurrentPart = i51AdeOsMalloc(I51KITG2_LENGTH_PART_UMCOMPRESS, __FILE__ , __LINE__ );
    }
    if( iFALSE == DataDesCurrentPart  ) 
    {
        iLog("KIT - G2 : malloc src decode one ");
        goto ERROR;
    }

    SrcPos = 18;
    DesPos = 0;
    for( iTemp=0; iTemp<NumberParts; iTemp++ )
    {
        DesTempPos = 0;
		
        LengthPartSrc = SrcDataPoint[SrcPos]|(SrcDataPoint[SrcPos+1]<<8)|(SrcDataPoint[SrcPos+2]<<16)|(SrcDataPoint[SrcPos+3]<<24);
        SrcPos += 4;

        if( iNULL==DataDesTempPart)
        {
            if( NumberParts==1 && (LengthDesData+514)<=I51KITG2_LENGTH_PART_UMCOMPRESS*2 )
            {
                // LengthDesData�ǽ���������ļ����ȣ����i51G��8λͼ����LengthDesData�ǽ��������16λͼ�ĳ��ȣ�Ҳ����ȥ���˵�ɫ����Ϣ���±��Ѿ��滻������ɫֵ
                // ��ɫ����󳤶�Ϊ2+256*2=514���ֽ�
                if( LengthPartSrc>LengthDesData)
                {
                    DataDesTempPart = i51AdeOsMalloc(LengthPartSrc+514, __FILE__ , __LINE__ );
                }
                else
                {
                    DataDesTempPart = i51AdeOsMalloc(LengthDesData+514, __FILE__ , __LINE__ );
                }
            }
            else
            {
                DataDesTempPart = i51AdeOsMalloc(I51KITG2_LENGTH_PART_UMCOMPRESS*2, __FILE__ , __LINE__ );
            }
            if( iNULL == DataDesTempPart )
            {
                iLog("KIT - G2 : malloc des decode one");
                goto ERROR;
            }
        }
			
        LengthPartDes = 32768;

        FlagReturn = i51AdeStdUncompress( DataDesCurrentPart, &LengthPartDes, &SrcDataPoint[SrcPos], LengthPartSrc);
        SrcPos += LengthPartSrc;	
        if( iFALSE == FlagReturn ) goto ERROR;

        if( iTemp==0 )
        {
            DesTempPos = I51KITG2_OFFSET_PIXELDATA;
            if(DataDesCurrentPart[0]!='i'||DataDesCurrentPart[1]!='5'||DataDesCurrentPart[2]!='1'||DataDesCurrentPart[3]!='G')
            {
                iLog("KIT - G2 : file form");
                goto ERROR;
            }

            if( DataDesCurrentPart[4]>0X40)
            {
                PaletteImage = iTRUE;
                PaletteColorNum = (DataDesCurrentPart[I51KITG2_OFFSET_PIXELDATA]) | (DataDesCurrentPart[I51KITG2_OFFSET_PIXELDATA+1]<<8); 
                PaletteColorMem = i51AdeOsMalloc(PaletteColorNum*2, __FILE__ , __LINE__ );
                if ( iNULL==PaletteColorMem )
                {
                    iLog("KIT - G2 : malloc PaletteColor" );
                    goto ERROR;
                }
                i51AdeStdMemcpy16(PaletteColorMem, &(DataDesCurrentPart[I51KITG2_OFFSET_PIXELDATA+2]), PaletteColorNum*2 );
				
            }

            ImageContainer->ImageIndexTable[GoingImageID].DecodeType = DataDesCurrentPart[I51KITG2_OFFSET_DECODETYPE];
            if(ImageContainer->ImageIndexTable[GoingImageID].DecodeType!=I51KITG2_DECODE_LINEAR)
            {
                iLog("KIT - G2 : ����ͼƬ��ʽ");
                goto ERROR;				
            }
            ImageContainer->ImageIndexTable[GoingImageID].ImageHeight= (DataDesCurrentPart[I51KITG2_OFFSET_IMAGEHEIGHT])|(DataDesCurrentPart[I51KITG2_OFFSET_IMAGEHEIGHT+1]<<8);
            ImageContainer->ImageIndexTable[GoingImageID].ImageWidth= (DataDesCurrentPart[I51KITG2_OFFSET_IMAGEWIDTH])|(DataDesCurrentPart[I51KITG2_OFFSET_IMAGEWIDTH+1]<<8);
            ImageContainer->ImageIndexTable[GoingImageID].AlphaPixelNum= (DataDesCurrentPart[I51KITG2_OFFSET_ALPHANUMER])|(DataDesCurrentPart[I51KITG2_OFFSET_ALPHANUMER+1]<<8);
            ImageContainer->ImageIndexTable[GoingImageID].LengthIamgeData =
   		 	(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH])|(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH+1]<<8)
   		     |(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH+2]<<16)|(DataDesCurrentPart[I51KITG2_OFFSET_DATALENGTH+3]<<24);
            ImageContainer->ImageIndexTable[GoingImageID].LengthTotalData = ImageContainer->ImageIndexTable[GoingImageID].LengthIamgeData ;
            if( ImageContainer->ImageIndexTable[GoingImageID].AlphaPixelNum>0)
            {
                ImageContainer->ImageIndexTable[GoingImageID].LengthTotalData+=ImageContainer->ImageIndexTable[GoingImageID].AlphaPixelNum*8 + 12;
            }

        }

        if(PaletteImage)
        {
            //���������ͼƬ��Ҫ�Ȱ�����תΪ��ɫ
            if( iTemp==0 )
            {
//                ZlibImageContainer->ImageIndexTable[GoingImageID].LengthIamgeData = 0;
                i51AdeStdMemcpy16(DataDesTempPart, DataDesCurrentPart, I51KITG2_OFFSET_PIXELDATA);
                DesPos = I51KITG2_OFFSET_PIXELDATA + PaletteColorNum*2 +2;
                DesTempPos = I51KITG2_OFFSET_PIXELDATA;
                BlockLength = 0;
                BlockPlace = 0;
            }
            else
            {
                DesPos = 0;
                DesTempPos = 0;
            }

            while(DesPos<LengthPartDes)
            {
                if( BlockPlace==0 )
                {
                    // �����¿�ʼ
                    if( (LengthPartDes-DesPos)>=6 )
                    {
                        // ��ʣ�����ݳ��ȴ��ڵ��ڿ�ͷ����
                        BlockPlace = 6;
                        BlockLength = DataDesCurrentPart[DesPos+4] | (DataDesCurrentPart[DesPos+5]<<8);
                        i51AdeStdMemcpy16(&DataDesTempPart[DesTempPos], &DataDesCurrentPart[DesPos], 6);
                        i51AdeStdMemcpy16(TempHead, &DataDesCurrentPart[DesPos], 6);
                        DesTempPos += 6;
                        DesPos += 6;
                    }
                    else
                    {
                        // ��ʣ������С�ڿ�ͷ���ȣ���ʣ�����ݱ���
                        BlockPlace = LengthPartDes-DesPos;
                        for( Temp = 0; Temp < BlockPlace; Temp++ )
                        {
                            TempHead[Temp] = DataDesCurrentPart[DesPos+Temp];
                        }
                        break; 						
                    }
                }
                else if( BlockPlace<6 )				
                {
                    for( Temp = BlockPlace; Temp < 6; Temp++ )
                    {
                        TempHead[Temp] = DataDesCurrentPart[DesPos+Temp-BlockPlace];
                    }
                    i51AdeStdMemcpy16(&DataDesTempPart[DesTempPos], TempHead, 6);
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    DesPos += 6-BlockPlace;
                    DesTempPos += 6;
                    BlockPlace = 6;
                }
                else
                {
                    BlockLength = TempHead[4] | (TempHead[5]<<8);
                    BlockLength -= BlockPlace - 6;
                }

                ColorPoint = (iCOLOR*)(((iU32)DataDesTempPart)+DesTempPos);

                if( DesPos+BlockLength > LengthPartDes)
                {
                    BlockLength = LengthPartDes - DesPos;
                    BlockPlace = 6 + BlockLength;
                }
                else				
                {
                    BlockPlace = 0;
                }
				
                for( Temp=0; Temp<BlockLength; Temp++ )
                {
                    ColorPoint[Temp] = PaletteColorMem[DataDesCurrentPart[DesPos]];
                    DesPos++;
                    DesTempPos += 2;
                }

            }

            if(i51AdeCnfGetEndian()==1)
            {
                // ��˵Ļ�����Ҫ����ɫֵ������ֵ�ĸߵ�λ����
                if( iTemp==0 )
                {
                	Temp = I51KITG2_OFFSET_PIXELDATA;
                }
                else
                {
                	Temp = 0;
                }
				
                while(Temp<DesTempPos)
                {
                    DataDesTempPart[Temp] += DataDesTempPart[Temp+1];
                    DataDesTempPart[Temp+1] = DataDesTempPart[Temp] - DataDesTempPart[Temp+1];
                    DataDesTempPart[Temp] = DataDesTempPart[Temp] - DataDesTempPart[Temp+1];
                    Temp += 2;
                }
            }
            FlagReturn = i51AdeOsFileWrite( DesFile, DataDesTempPart, DesTempPos, &LengFactWriteData );
            if( iFALSE == FlagReturn ) goto ERROR;
			
        }
        else
        {
            if(i51AdeCnfGetEndian()==1)
            {
                // ��˵Ļ�����Ҫ����ɫֵ������ֵ�ĸߵ�λ����
                while(DesTempPos<LengthPartDes)
                {
                    DataDesCurrentPart[DesTempPos] += DataDesCurrentPart[DesTempPos+1];
                    DataDesCurrentPart[DesTempPos+1] = DataDesCurrentPart[DesTempPos] - DataDesCurrentPart[DesTempPos+1];
                    DataDesCurrentPart[DesTempPos] = DataDesCurrentPart[DesTempPos] - DataDesCurrentPart[DesTempPos+1];
                    DesTempPos += 2;
                }
            }
            FlagReturn = i51AdeOsFileWrite( DesFile, DataDesCurrentPart, LengthPartDes, &LengFactWriteData );
            if( iFALSE == FlagReturn ) goto ERROR;
        }
    }

    ImageContainer->ImageIndexTable[GoingImageID].FlagImageExist = 1;
    ImageContainer->ImageIndexTable[GoingImageID].FlagCopyRAM = iFALSE;
    ImageContainer->ImageIndexTable[GoingImageID].ImageEntry = SeekFileContainer;

    FlagReturn = i51AdeOsFileSetSeek( DesFile , ( I51KITG2_OFFSET_START_DATA + ImageID*5 ), i51_OS_FL_SEEK_HEAD ) ;
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }

    FlagImageExist = 1;
    FlagReturn = i51AdeOsFileWriteInt8( DesFile, &FlagImageExist);
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }

    FlagReturn = i51AdeOsFileWriteInt32( DesFile, &SeekFileContainer );
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }
		
    FlagReturn = i51AdeOsFileSetSeek( DesFile , 0, i51_OS_FL_SEEK_TAIL) ;
    if( iFALSE == FlagReturn )
    {
        goto ERROR;
    }

    i51AdeOsFileFlush( DesFile );

    if(PaletteColorMem)
    {
        i51AdeOsFree(PaletteColorMem);
        PaletteColorMem = iNULL;
    }
    i51AdeOsFree(DataDesCurrentPart);
    DataDesCurrentPart = iNULL;
    i51AdeOsFree(DataDesTempPart);
    DataDesTempPart = iNULL;

    return iTRUE;

    ERROR:
        iLog("KIT - G2 : decode one : %d",GoingImageID);
        if(PaletteColorMem)
        {
            i51AdeOsFree(PaletteColorMem);
            PaletteColorMem = iNULL;
        }
        i51AdeOsFree(DataDesCurrentPart);
        DataDesCurrentPart = iNULL;
        i51AdeOsFree(DataDesTempPart);
        DataDesTempPart = iNULL;
        return iFALSE;

}

static void  ZlibDecodeAllImage( iU32 ID)
{
    // author : Otto
    // since : Sep 8 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE file : i51�ļ�ָ��
    // iU32 TotallImage : i51�ļ�����
    //  iU16* path �������ļ�����·��ָ��

    // notes : ����һ��ͼƬ�����ļ�,����������ָ��

    // �����ļ���ʽ���£�
    // 1.ͼƬ���� �ĸ��ֽ�
    // 2.�ڴ��п�����ͼƬ���� �ĸ��ֽ�
    // 3.ͼƬ�Ƿ���Ҫ�������ڴ�ı�� һ���ֽ�
    // 4.ͼƬ�������ļ��е�ƫ���� �ĸ��ֽ�
    // 5.ͼƬ���
    // 6.ͼƬ�߶�
    // 7.���뷽ʽ
    // 8.͸��ɫ
    // 9.ͼƬRGB����
    // 3,4��������ҪTotallImage��
    // 5,6,7,8,9��������ҪTotallImage��

    iU32 * ImageId ;
    iS32 iTemp = 0;	

    iFILE SrcFile = iNULL;
    iFILE DesFile = iNULL;
    iHANDLE Container = iNULL;
    iU32 DesSeek = 0;
    iU32 SrcSeek = 0;
    iU32 ImageID = 0;
    iU8 FlagReturn = iFALSE;

    // �����һ��ͼƬʱ��Ҫ������������ʼ������
    if( ManageZlibDecodeImage.CompleteImage == 0 )
    {
        if( iFALSE == ZlibDecodeInit() )
        {
            iLog("KIT - G2 : ZlibDecodeInit");
            goto ERROR;
        }
    }

    {
        SrcFile = ManageZlibDecodeImage.SrcFile;
        DesFile = ManageZlibDecodeImage.DesFilePointer;
        Container = (iHANDLE)(ZlibImageContainer);
        // �ƶ��ļ���ָ�뵽�����ļ�ĩβ
        FlagReturn = i51AdeOsFileSetSeek( DesFile, 0, i51_OS_FL_SEEK_TAIL) ;
        if( iFALSE == FlagReturn )
        {
    		goto ERROR;
        }
    	
        // ��õ�ǰ�����ļ���ָ��λ��
        FlagReturn = i51AdeOsFileGetSeek( DesFile , (iS32 *)(&DesSeek), i51_OS_FL_SEEK_HEAD ) ;
        if( iFALSE == FlagReturn )
        {
    		goto ERROR;
        }

        // ���ָ��ID�ŵ���Դ����Դ�ļ���ƫ����	
        ImageID = ManageZlibDecodeImage.GoingImageID;
        SrcSeek =  i51KitStdResGetContentEntry (  ManageZlibDecodeImage.SrcFile, ImageID) ;
        if( iFALSE == FlagReturn )
        {
            iLog("KIT - G2 :offset decode one");
            goto ERROR;
        }

        if( iFALSE == ZlibDecodeOneImageFromFile( SrcFile , SrcSeek, DesFile, DesSeek, Container, ImageID ) )
        {
            goto ERROR;
        }
    }

    ManageZlibDecodeImage.CompleteImage++;

    if( ManageZlibDecodeImage.CompleteImage < ManageZlibDecodeImage.TotallImage )	
    {
        ImageId = (iU32*)ManageZlibDecodeImage.ImageId;
        ManageZlibDecodeImage.GoingImageID = ImageId[ManageZlibDecodeImage.CompleteImage];

        ManageZlibDecodeImage.Process(   ManageZlibDecodeImage.CompleteImage, ManageZlibDecodeImage.TotallImage, ZlibImageContainer);

        return ;

    }

    i51AdeOsTimerStop( TimeIdZlibDecodeImage );         
    TimeIdZlibDecodeImage = iNULL;
    FlagDecodeImage = iFALSE;

//    i51AdeOsFileClose(ManageZlibDecodeImage.DesFilePointer);

    i51AdeOsFree((void *)ManageZlibDecodeImage.ImageId);
    ManageZlibDecodeImage.ImageId = iNULL;
	
//    ManageZlibDecodeImage.DesFilePointer = iNULL;

    i51KitG2WriteFileCRC(ManageZlibDecodeImage.DesFilePointer);

    i51AdeOsFileFlush( ManageZlibDecodeImage.DesFilePointer);
	
     ManageZlibDecodeImage.Process(   ManageZlibDecodeImage.CompleteImage, ManageZlibDecodeImage.TotallImage, ZlibImageContainer);
    ManageZlibDecodeImage.Done( ZlibImageContainer );

    return ;
	
    ERROR:

    iTemp = 0;	 
    while(1)
    {
        if(ZlibImageContainer &&  ContainerManage[iTemp].Flag  && ContainerManage[iTemp].ContainerHandle==ZlibImageContainer )
        {
    	     ContainerManage[iTemp].Flag = iFALSE;
            break;
        }
        iTemp++;
        if( iTemp>=MAX_ContainerManage_NUM )		
        {
            iLog("KIT - G2 : decode all zlib");
		break;
        }
    }		
	
        i51AdeOsFileClose( ManageZlibDecodeImage.DesFilePointer );
        i51AdeOsFree((void *)ManageZlibDecodeImage.ImageId);
        ManageZlibDecodeImage.ImageId = iNULL;
        if( ZlibImageContainer )
        {
	        i51AdeOsFree((void *)ZlibImageContainer->ImageIndexTable);
	        ZlibImageContainer->ImageIndexTable = iNULL;
	        i51AdeOsFree((void *)ZlibImageContainer);
	        ZlibImageContainer = iNULL;
        }

        if( i51AdeOsFileIsExist(ManageZlibDecodeImage.DesFileName) )
        {
            i51AdeOsFileDelete(ManageZlibDecodeImage.DesFileName);
        }
        i51AdeOsTimerStop( TimeIdZlibDecodeImage );         
        TimeIdZlibDecodeImage = iNULL;
        FlagDecodeImage = iFALSE;
        ManageZlibDecodeImage.Done(iNULL);
        iLog("KIT - G2 :decode all");

        return ;		

}

static iBOOL i51KitG2GetActualRect(iRECT * ImageRect, iRECT * DesRect, iRECT *  SrcRect,  iRECT * DrawDesRect,  iRECT * DrawSrcRect, iU8 Direction )
{
 // ������������ͼ��û����Ҫ���Ƶ������򷵻�iFALSE�����򷵻�iTURE
    // ͼƬ���Ƶ�����
    iS16 SrcStartX = 0;
    iS16 SrcStartY = 0;
    iS16 SrcWidth = 0;
    iS16 SrcHeight = 0;

    // ԭʼͼ��ĳ���	
    iS16 ImageWidth = 0;
    iS16 ImageHeight = 0;
	
     // ���Ƶ���Ļ�ϵ�����
    iS16 DesStartX = 0;
    iS16 DesStartY = 0;
    iS16 DesEndX = 0;
    iS16 DesEndY = 0;
    iS16 DesWidth = 0;
    iS16 DesHeight = 0;

    // ����������Ƶ���Ļ�ϵĸ߿�
    iS16 DrawWidth = 0;
    iS16 DrawHeight = 0;	
	
    // VP��λ��RECT
    iRECT * VPActualRect = iNULL;
    // VP��Ҫ���������RECT
    iRECT * VPDrawRect = iNULL;

    if(i51UseVirtualPanel)
    {
        VPActualRect = &(i51CurrentVpRender->VPActualRect);
        VPDrawRect = &(i51CurrentVpRender->VPDrawRect);
    }
	
    ImageWidth = ImageRect->Width;
    ImageHeight = ImageRect->Height;
	
    if( SrcRect)
    {
        SrcStartX = SrcRect->X;
        SrcStartY = SrcRect->Y;
        SrcWidth = SrcRect->Width;
        SrcHeight = SrcRect->Height;
    }
    else
    {
        SrcStartX = 0;
        SrcStartY = 0;
        SrcWidth = ImageRect->Width;
        SrcHeight = ImageRect->Height;
    }
	
    if( i51UseVirtualPanel == iFALSE )
    {
        if( DesRect)
        {
            DesStartX = DesRect->X;
            DesStartY = DesRect->Y;
            DesWidth = DesRect->Width;
            DesHeight = DesRect->Height;
        }
        else
        {
            DesStartX = 0;
            DesStartY = 0;
            DesWidth = ScreenWidth;
            DesHeight = ScreenHeight;
        }
    }
    else
    {
        if( DesRect)
        {
            DesStartX = DesRect->X + VPActualRect->X;
            DesStartY = DesRect->Y + VPActualRect->Y;
            DesWidth = DesRect->Width;
            DesHeight = DesRect->Height;
        }
        else
        {
            DesStartX = VPActualRect->X;
            DesStartY = VPActualRect->Y;
            DesWidth = VPActualRect->Width;
            DesHeight = VPActualRect->Height;
        }
		
        // ����ʹ��VPʱ���Ƶ���Ļ����
        DesEndX = DesStartX + DesWidth;
        DesEndY = DesStartY + DesHeight;
        if( DesStartX<VPDrawRect->X )
	{
		SrcWidth -= VPDrawRect->X - DesStartX ;
		SrcStartX += VPDrawRect->X - DesStartX ;
		DesStartX = VPDrawRect->X;
	}
        if( DesStartY<VPDrawRect->Y )
	{
		SrcHeight -= VPDrawRect->Y - DesStartY ; 
		SrcStartY += VPDrawRect->Y - DesStartY ; 
		DesStartY = VPDrawRect->Y;
	}
        if( DesEndX>(VPDrawRect->X+VPDrawRect->Width) ) DesEndX = VPDrawRect->X+VPDrawRect->Width;
        if( DesEndY>(VPDrawRect->Y+VPDrawRect->Height) ) DesEndY = VPDrawRect->Y+VPDrawRect->Height;
        DesWidth = DesEndX - DesStartX;
        DesHeight = DesEndY - DesStartY;
        if( DesWidth<=0 || DesHeight<=0 ) return iFALSE;
    }
	
    // ͼƬ��ʼ���Ƶ������Ѿ�����ͼƬ��С��ֱ�ӷ���
    if( SrcStartX>=ImageWidth || SrcStartY>=ImageHeight) return iFALSE;
	
    // ͼƬ���������Ŀ�ĵ�����߿�ѡ�񽻼�
    if( SrcWidth<DesWidth)	
        DrawWidth = SrcWidth;
    else
        DrawWidth = DesWidth;
	
    if( SrcHeight<DesHeight)	
        DrawHeight= SrcHeight;
    else
        DrawHeight = DesHeight;

    // ԴͼƬ�ü�λ�ÿ�ʼ��ͼƬ�����ƫ������
    if( SrcStartX < 0 )
    {
        DrawWidth += SrcStartX;
        DesStartX -= SrcStartX;
        SrcStartX = 0;			
    }
	
    // ԴͼƬ�ü�λ�ÿ�ʼ��ͼƬ�Ϸ���ƫ������
    if( SrcStartY<0)
    {
        DrawHeight += SrcStartY;
        DesStartY -= SrcStartY;
        SrcStartY = 0;
    }

    // ���ƿ�ʼ��������Ļ���
    if( DesStartX<0 )
    {
        DrawWidth += DesStartX;
        if( Direction!=I51KITG2_DRAW_IAMGE_MIRROR )
        {
            SrcStartX -= DesStartX;
        }
        DesStartX = 0;			
    }

    // ���ƿ�ʼ��������Ļ�Ϸ�
    if( DesStartY<0 )
    {
        DrawHeight += DesStartY;
        SrcStartY -= DesStartY;
        DesStartY = 0;			
    }

    ImageWidth = ImageRect->Width;
    ImageHeight = ImageRect->Height;

    // ͼƬ�������򳬳�ͼƬ��С����С��������
    if( (SrcStartX+DrawWidth) > ImageWidth )
    {
        DrawWidth = ImageWidth - SrcStartX;
    }

    if( (SrcStartY+DrawHeight) > ImageHeight )
    {
        DrawHeight = ImageHeight - SrcStartY;
    }

    // ��Ļ�������򳬳���Ļ��С����С��������
    if( (DesStartX+DrawWidth) > ScreenWidth)
    {
        if( Direction==I51KITG2_DRAW_IAMGE_MIRROR )
        {
        	SrcStartX += DesStartX+DrawWidth- ScreenWidth;
        }
        DrawWidth = ScreenWidth - DesStartX;
    }

    if( (DesStartY+DrawHeight) > ScreenHeight)
    {
        DrawHeight = ScreenHeight - DesStartY;
    }

    if( (DrawHeight<=0) || ( DrawWidth<=0 ) ) return iFALSE;

    DrawSrcRect->X = SrcStartX;
    DrawSrcRect->Y = SrcStartY;
    DrawSrcRect->Width= DrawWidth;
    DrawSrcRect->Height= DrawHeight;

    DrawDesRect->X = DesStartX;
    DrawDesRect->Y = DesStartY;
    DrawDesRect->Width = DrawWidth;
    DrawDesRect->Height= DrawHeight;

    return iTRUE;

}
static iBOOL DrawRAMLinearImage  ( iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect , iU8 Direction)
{
    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 ImageID : ����ͼ���ID��
    // iRECT* DesRect : ����ͼ����Ļ�ϵ�����
    // iRECT* SrcRect : ����ͼ�������
    // iU8 Direction : ����ͼ��ķ���
  
    // notes : ��ͼ��������ID��ΪImageID��ͼƬSrcRect����
    // ����Direction�ķ�����Ƶ���Ļ��DesRect����
    // ͼƬ���������ڴ����б��ݣ���Ҫ�����ݴ��ڴ��ж�ȡ
    // ����ͼƬ�ı��뷽ʽΪI51KITG2_DECODE_LINEAR 

    // ͼƬ������ַ���
    i51ImageContainer * ImageContainer = iNULL;
    // ͼƬ�������ַ
    i51ImageIndex * ImageIndexTable = iNULL;
    // ͼƬ��Ⱥ͸߶�
    iU32 ImageWidth = 0;
    iU32 ImageHeight = 0;
    // �����ļ�ָ���ַ
    iFILE File = iNULL;
    // �ļ�������������ֵ
//    iU8   FlagReturn = iFALSE;
    // ��ȡ�ļ��Ļ�����
//    iU8   ReadDataBuff[6] = {0};
    iU16 * TempReadDataBuff = iNULL;
    // ͼƬ�����ļ���ʼ��ַ
    iU32 DataEntry = 0;
    iU32 CopyDataEntry = 0;
    // ��ȡ�������ݳ���
//    iU32  LengthReadData = 0;
    // �ۼƶ�ȡ�������ݳ���
    iU32  LengthTotalReadData = 0;
    // ��Ҫ��ȡ�����ݳ���
//    iU32  LengthNeedReadData = 0;
    // һ����Ҫ����������������ɫ����
    iS32 LengthCopyColor = 0;
    // ͼƬ�ļ��ܳ���
    iU32 LengthIamgeData = 0;

    // ���Ƶ���ͼƬʱͼƬ�߶���ʼλ��
    iS16 FillStartY = 0;
    iS16 FillEndY = 0;

    // ��ǰ���ݶ�����Ļ�ϵĿ�ʼ����λ��
    iS16 TempDrawX = 0;
    iS16 TempDrawY = 0;
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
    // ʵ�ʻ��Ƶ�ͼƬ���ȺͿ��  
    iS16 DrawWidth = 0;
    iS16 DrawHeight = 0;
//    iU16 iTempColor = 0;
    iS32 iTemp = 0;

    // ImageX
    iS32 CurrentImageX = 0;
    iS32 CurrentImageY = 0;
    iS32 CurrentImageWidth = 0;


    iU32 LcdBufferPostion = 0;

    // ������һ�������׵�ַ
    iU16 * TempReadDataBuff1 = iNULL;
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
    iRECT DrawDesRect , DrawSrcRect, ImageRect;

    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = (i51ImageContainer *)ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL==ImageIndexTable || iNULL==File ) return iFALSE;

    ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    ImageHeight = ImageIndexTable[ImageID].ImageHeight;
    LengthIamgeData = ImageIndexTable[ImageID].LengthIamgeData;
    TotalAlphaNumber = ImageIndexTable[ImageID].AlphaPixelNum;

    ImageRect.Width = ImageWidth;
    ImageRect.Height = ImageHeight;
    if(iFALSE==i51KitG2GetActualRect(&ImageRect, DesRect, SrcRect, &DrawDesRect, &DrawSrcRect, Direction))
    {
        return iTRUE;
    }

    DrawWidth = DrawSrcRect.Width;
    DrawHeight= DrawSrcRect.Height;

    SrcStartX = DrawSrcRect.X;
    SrcStartY = DrawSrcRect.Y;
    SrcEndX = DrawSrcRect.X + DrawWidth;
    SrcEndY = DrawSrcRect.Y + DrawHeight;

    DrawStartX = DrawDesRect.X;
    DrawStartY = DrawDesRect.Y;
    DrawEndX = DrawDesRect.X + DrawWidth;
    DrawEndY = DrawDesRect.Y + DrawHeight;

    if( Direction&(I51KITG2_DRAW_IAMGE_HANDSTAND) )
    {
        FillStartY = ImageHeight - SrcStartY - DrawHeight;
        FillEndY = FillStartY + DrawHeight;
    }
    else
    {
        FillStartY = SrcStartY;
        FillEndY = FillStartY + DrawHeight;
    }

	if( 0)
//	if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR)  )
    {
        SrcStartX = ImageWidth - SrcStartX - DrawWidth;
        SrcEndX = SrcStartX + DrawWidth;
    }

    if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
    {
        DataEntry = ImageIndexTable[ImageID].ImageEntry;
        FlagReturn = i51AdeOsFileSetSeek( File, (DataEntry), i51_OS_FL_SEEK_HEAD );
        if( iFALSE == FlagReturn ) goto ERROR;                      
    
        LengthNeedReadData =ImageIndexTable[ImageID].LengthTotalData;
    
        ImageDataEntry = i51AdeOsMalloc(LengthNeedReadData, __FILE__ , __LINE__ );
        if( iNULL == ImageDataEntry ) 
        {
            iLog("KIT - G2 : malloc linear normal");
            return iFALSE;
        }
    	
        i51AdeOsFileRead( File, ImageDataEntry, LengthNeedReadData, &LengthTotalRead);
        if( iFALSE == FlagReturn || LengthNeedReadData!=LengthTotalRead) 
        {
            i51AdeOsFree(ImageDataEntry);
            ImageDataEntry = iNULL;
            goto ERROR;                
        }
    }
    else
    {
        ImageDataEntry = (iU8*)ImageIndexTable[ImageID].MemoryEntry;
    }

    DataEntry = (iU32)ImageDataEntry;
    LengthTotalReadData = I51KITG2_OFFSET_PIXELDATA;
    DataEntry += I51KITG2_OFFSET_PIXELDATA;

    if(AlphaSys!=i51KITG2_ALPHA_NONEED) goto ALPHA_NEED;
    {
        while(1)
        {
            TempReadDataBuff1 = (iU16 *)(DataEntry);   
            CurrentImageX = TempReadDataBuff1[0];
            CurrentImageY = TempReadDataBuff1[1];
            CurrentImageWidth = TempReadDataBuff1[2];
            DataEntry += 6;
            CopyDataEntry = DataEntry;
            LengthCopyColor =  CurrentImageWidth;

            LengthTotalReadData += 6 + (CurrentImageWidth<<1);
            DataEntry += ( CurrentImageWidth<<1 );
			
            // ͼƬ��ǰ���ݶ�X��ʼ������ͼƬ���������ұ߻���Y������ͼƬ���������Ϸ�            
            if( (CurrentImageY < FillStartY) || (CurrentImageX > SrcEndX) || (CurrentImageY>=FillEndY) )
            {
                if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                continue;
            }
            
            // ͼƬ��Ҫ������ʾ��ӳ��X����
            if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR) )
            {
//				CurrentImageX = ImageWidth - CurrentImageX - CurrentImageWidth;
            }

            // ͼƬ��Ҫ������ʾ��ӳ��Y����
            if( Direction&(I51KITG2_DRAW_IAMGE_HANDSTAND) )
            {
                // ͼ��߶Ƚ�����������ͼ��߶�-1
                CurrentImageY = ImageHeight -1 - CurrentImageY ;
            }

            // ��ǰ���ݶγ��ȳ���ͼƬ���������ұ�
            if( (CurrentImageX+CurrentImageWidth) >SrcEndX )
            {
                LengthCopyColor -= ((CurrentImageX+CurrentImageWidth-SrcEndX));
            }
			
            // ͼƬ��ǰ���ݶ�X��ʼ������ͼƬ�����������            
            if( CurrentImageX<SrcStartX )
            {
//                if( (iNULL)==(Direction&(I51KITG2_DRAW_IAMGE_MIRROR)) )
                {
                    CopyDataEntry += ((SrcStartX - CurrentImageX)<<1);
                }

                LengthCopyColor -= ((SrcStartX - CurrentImageX));
                CurrentImageX = SrcStartX;
                if( LengthCopyColor<=0 ) 
                {
                    if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                    continue;
                }
            }

             // ���㵱ǰ���ݶ�����Ļ�Ͽ�ʼ��ʾ��λ��
             TempDrawX = DrawStartX;
             TempDrawY = DrawStartY + CurrentImageY - SrcStartY;

            // ͼƬ��ǰ���ݶ�X��ʼ������ͼƬ��ʼ���ƻ��������ұ�
            // ��Ҫ�ƶ���Ļ��ʼ��������
            if( CurrentImageX>SrcStartX )
            {
                TempDrawX += ((CurrentImageX-SrcStartX));
            }

            if( TempDrawX >DrawEndX )
            {
                if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                continue;
            }

            if( TempDrawY > DrawEndY ) break;
			
            
            TempReadDataBuff = (iU16*)CopyDataEntry;
            if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR) )
            {
                LcdBufferPostion = TempDrawY*ScreenWidth+DrawStartX+DrawWidth - CurrentImageX + SrcStartX-1;
//                LcdBufferPostion = TempDrawY*ScreenWidth+TempDrawX + LengthCopyColor-1;
//                 i51AdeStdMemcpy16((iU16 *)ReadDataBuff, (void *)(CopyDataEntry), (LengthCopyColor<<1));
                for( iTemp = 0; iTemp <LengthCopyColor; iTemp++ )
                {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(LcdBufferPostion,1);
#endif
                     LcdBuffer[LcdBufferPostion--] = TempReadDataBuff[iTemp];
                }
            }
            else
            {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(TempDrawY*ScreenWidth+TempDrawX,LengthCopyColor);
#endif
                i51AdeStdMemcpy16( &LcdBuffer[ TempDrawY*ScreenWidth+TempDrawX], TempReadDataBuff, (LengthCopyColor<<1));
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
			
            DataEntry = (iU32)ImageDataEntry+CurrBuffPos + 12;
			
            for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
            {
                TempReadDataBuff1 = (iU16 *)(DataEntry);   
                AlphaPosX = TempReadDataBuff1[0];
                AlphaPosY = TempReadDataBuff1[1];
                AlphaNumber = (iALPHA)TempReadDataBuff1[2];
                AlphaColor = TempReadDataBuff1[3];
                DataEntry += 8;
				
                // ͼƬ��Ҫ������ʾ��ӳ��X����
                if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR) )
                {
                    AlphaPosX = TempWidth - AlphaPosX ;
                }
                if(AlphaPosY>=FillEndY) break;
                if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<FillStartY ) continue;				
    
                // ͼƬ��Ҫ������ʾ��ӳ��Y����
                if( Direction&(I51KITG2_DRAW_IAMGE_HANDSTAND) )
                {
                    // ͼ��߶Ƚ�����������ͼ��߶�-1
                    AlphaPosY = TempHeight - AlphaPosY ;
                }
                LcdBuffPos = (AlphaPosY-SrcStartY+DrawStartY)*ScreenWidth+AlphaPosX-SrcStartX+DrawStartX;
                LcdBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(LcdBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
            }
        }
    }
     goto FUN_RETURN;
	 
ALPHA_NEED:
    {
        while(1)
        {
            TempReadDataBuff1 = (iU16 *)(DataEntry);   
            CurrentImageX = TempReadDataBuff1[0];
            CurrentImageY = TempReadDataBuff1[1];
            CurrentImageWidth = TempReadDataBuff1[2];
            DataEntry += 6;
            CopyDataEntry = DataEntry;
            LengthCopyColor =  CurrentImageWidth;

            LengthTotalReadData += 6 + (CurrentImageWidth<<1);
            DataEntry += ( CurrentImageWidth<<1 );
			
            // ͼƬ��ǰ���ݶ�X��ʼ������ͼƬ���������ұ߻���Y������ͼƬ���������Ϸ�            
            if( (CurrentImageY < FillStartY) || (CurrentImageX > SrcEndX) || (CurrentImageY>=FillEndY) )
            {
                if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                continue;
            }
            
            // ͼƬ��Ҫ������ʾ��ӳ��X����
            if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR) )
            {
                CurrentImageX = ImageWidth - CurrentImageX - CurrentImageWidth;
            }

            // ͼƬ��Ҫ������ʾ��ӳ��Y����
            if( Direction&(I51KITG2_DRAW_IAMGE_HANDSTAND) )
            {
                // ͼ��߶Ƚ�����������ͼ��߶�-1
                CurrentImageY = ImageHeight -1 - CurrentImageY ;
            }

            // ��ǰ���ݶγ��ȳ���ͼƬ���������ұ�
            if( (CurrentImageX+CurrentImageWidth) >SrcEndX )
            {
                LengthCopyColor -= ((CurrentImageX+CurrentImageWidth-SrcEndX));
            }
			
            // ͼƬ��ǰ���ݶ�X��ʼ������ͼƬ�����������            
            if( CurrentImageX<SrcStartX )
            {
                if( (iNULL)==(Direction&(I51KITG2_DRAW_IAMGE_MIRROR)) )
                {
                    CopyDataEntry += ((SrcStartX - CurrentImageX)<<1);
                }

                LengthCopyColor -= ((SrcStartX - CurrentImageX));
                if( LengthCopyColor<=0 ) 
                {
                    if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                    continue;
                }
            }

             // ���㵱ǰ���ݶ�����Ļ�Ͽ�ʼ��ʾ��λ��
             TempDrawX = DrawStartX;
             TempDrawY = DrawStartY + CurrentImageY - SrcStartY;

            // ͼƬ��ǰ���ݶ�X��ʼ������ͼƬ��ʼ���ƻ��������ұ�
            // ��Ҫ�ƶ���Ļ��ʼ��������
            if( CurrentImageX>SrcStartX )
            {
                TempDrawX += ((CurrentImageX-SrcStartX));
            }

            if( TempDrawX >DrawEndX )
            {
                if( LengthTotalReadData> (LengthIamgeData-3) ) break;
                continue;
            }

            if( TempDrawY > DrawEndY ) break;
			
            
            TempReadDataBuff = (iU16*)CopyDataEntry;
            if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR) )
            {
                LcdBufferPostion = TempDrawY*ScreenWidth+TempDrawX + LengthCopyColor;
//                 i51AdeStdMemcpy16((iU16 *)ReadDataBuff, (void *)(CopyDataEntry), (LengthCopyColor<<1));
                for( iTemp = 0; iTemp <LengthCopyColor; iTemp++ )
                {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(LcdBufferPostion,1);
#endif
//                     LcdBuffer[LcdBufferPostion--] = TempReadDataBuff[iTemp];
                     LcdBuffer[ LcdBufferPostion] = i51KitG2GetNewColor(LcdBuffer[ LcdBufferPostion], TempReadDataBuff[iTemp]);
                     LcdBufferPostion--;
                }
            }
            else
            {
                LcdBufferPostion = TempDrawY*ScreenWidth+TempDrawX;
                for( iTemp = 0; iTemp <LengthCopyColor; iTemp++ )
                {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(LcdBufferPostion,1);
#endif
                    LcdBuffer[ LcdBufferPostion] = i51KitG2GetNewColor(LcdBuffer[ LcdBufferPostion], TempReadDataBuff[iTemp]);
                    LcdBufferPostion++;
                }
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
			
            DataEntry = (iU32)ImageDataEntry+CurrBuffPos + 12;
			
            for( CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber;CurrAlphaNum++)
            {
                TempReadDataBuff1 = (iU16 *)(DataEntry);   
                AlphaPosX = TempReadDataBuff1[0];
                AlphaPosY = TempReadDataBuff1[1];
                AlphaNumber = (iALPHA)TempReadDataBuff1[2];
                AlphaColor = TempReadDataBuff1[3];
                DataEntry += 8;
				
                // ͼƬ��Ҫ������ʾ��ӳ��X����
                if( Direction&(I51KITG2_DRAW_IAMGE_MIRROR) )
                {
                    AlphaPosX = TempWidth - AlphaPosX ;
                }
                if(AlphaPosY>=FillEndY) break;
                if( AlphaPosX>=SrcEndX || AlphaPosX<SrcStartX || AlphaPosY<FillStartY ) continue;				
    
                // ͼƬ��Ҫ������ʾ��ӳ��Y����
                if( Direction&(I51KITG2_DRAW_IAMGE_HANDSTAND) )
                {
                    // ͼ��߶Ƚ�����������ͼ��߶�-1
                    AlphaPosY = TempHeight - AlphaPosY ;
                }
                LcdBuffPos = (AlphaPosY-SrcStartY+DrawStartY)*ScreenWidth+AlphaPosX-SrcStartX+DrawStartX;
                LcdBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(LcdBuffer[LcdBuffPos], AlphaColor, AlphaNumber*AlphaSrc/255);
            }
        }    
    }

FUN_RETURN:	
    if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
    {
        i51AdeOsFree(ImageDataEntry);
        ImageDataEntry = iNULL;
    }
    return iTRUE;

ERROR:
    iLog("KIT - G2 : RW file linear");
    if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
    {
        i51AdeOsFree(ImageDataEntry);
        ImageDataEntry = iNULL;
    }
    return iFALSE;
		
}

static iBOOL i51KitG2RotatePoint(iS32 SrcX, iS32 SrcY, iS32 RotateCenterX,  iS32 RotateCenterY ,iS32 * DesX, iS32 * DesY, iS32 Angle)
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
    SinB = i51KitG2Sin(Angle);
    CosB = i51KitG2Cos(Angle);

    SinC = SinA*CosB - CosA*SinB;
    CosC = CosA*CosB + SinA*SinB;

    *DesY = ((SinC+(CenterY<<14))>>14);
    *DesX = ((CosC+(CenterX<<14))>>14);

//    *DesX = (((SrcX-CenterX)*CosB+(SrcY-CenterY)*SinB)>>14) + CenterX;
//    *DesY = (((CenterX-SrcX)*SinB+(SrcY-CenterY)*CosB)>>14) + CenterY;
    return iTRUE;
	
}

static iBOOL i51KitG2GetGirthPoints(iRECT SrcRect, iRECT DesRect, iS16 PointS[][2], iS32 SrcMoveX, iS32 SrcMoveY,
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
	
    i51KitG2RotatePoint(UpLeftX, UpLeftY, RotateCenterX, RotateCenterY, &EdgePoints[0][0], &EdgePoints[0][1], Angle);
    i51KitG2RotatePoint(UpLeftX+SrcW, UpLeftY, RotateCenterX, RotateCenterY, &EdgePoints[1][0], &EdgePoints[1][1], Angle);
    i51KitG2RotatePoint(UpLeftX, UpLeftY+SrcH, RotateCenterX, RotateCenterY, &EdgePoints[2][0], &EdgePoints[2][1], Angle);
    i51KitG2RotatePoint(UpLeftX+SrcW, UpLeftY+SrcH, RotateCenterX, RotateCenterY, &EdgePoints[3][0], &EdgePoints[3][1], Angle);

    if( EdgePoints[0][0]<DesStartX&&EdgePoints[1][0]<DesStartX&&EdgePoints[2][0]<DesStartX&&EdgePoints[3][0]<DesStartX) return iFALSE;
    if( EdgePoints[0][1]<DesStartY&&EdgePoints[1][1]<DesStartY&&EdgePoints[2][1]<DesStartY&&EdgePoints[3][1]<DesStartY) return iFALSE;
    if( EdgePoints[0][0]>DesEndX&&EdgePoints[1][0]>DesEndX&&EdgePoints[2][0]>DesEndX&&EdgePoints[3][0]>DesEndX) return iFALSE;
    if( EdgePoints[0][1]>DesEndY&&EdgePoints[1][1]>DesEndY&&EdgePoints[2][1]>DesEndY&&EdgePoints[3][1]>DesEndY) return iFALSE;

    if( RotateType==i51KITG2_ROTATE_XLINE) goto XLINE;

    SrcX = UpLeftX;
    SrcY = UpLeftY;

//iLog("Y:");
    for ( Location = 0; Location <SrcW; Location++)
    {
        i51KitG2RotatePoint(SrcX, SrcY, RotateCenterX, RotateCenterY, &DesX, &DesY, Angle);
        PointS[Location][0] = DesX-SrcX;
        PointS[Location][1] = DesY-SrcY;
//iLog("%d\t%d",PointS[Location][0],PointS[Location][1]);
        SrcX++;
    }	

    if( RotateType==i51KITG2_ROTATE_YLINE) goto CHECK;

XLINE:
    SrcH = SrcW + SrcH;
    SrcX = UpLeftX;
    SrcY = UpLeftY;
    i51KitG2RotatePoint(SrcX, SrcY, RotateCenterX,  RotateCenterY , &DesX, &DesY, Angle);
    SrcX1 = DesX;
    SrcY1 = DesY;
#if IMAGE_STRETCH	
    OldX = DesX;
    OldY = DesY;
#endif
//iLog("X:")
    for ( Location = SrcW; Location <SrcH; Location++)
    {
        i51KitG2RotatePoint(SrcX, SrcY, RotateCenterX, RotateCenterY,&DesX, &DesY, Angle);
#if IMAGE_STRETCH		
DesPoints[0][0] = OldX;
DesPoints[0][1] = OldY;
DesPoints[1][0] = OldX-1;
DesPoints[1][1] = OldY;
DesPoints[2][0] = OldX;
DesPoints[2][1] = OldY-1;
DesPoints[3][0] = OldX+1;
DesPoints[3][1] = OldY;
DesPoints[4][0] = OldX;
DesPoints[4][1] = OldY+1;

Min = I51KITG2_ABS(DesPoints[0][0]-DesX) + I51KITG2_ABS(DesPoints[0][1]-DesY) ;
Num = 0;
for( iTemp=1; iTemp<5; iTemp++ )
{
    Temp = I51KITG2_ABS(DesPoints[iTemp][0]-DesX) + I51KITG2_ABS(DesPoints[iTemp][1]-DesY) ;
    if( Temp<Min)
    {
        Min = Temp;
        Num = iTemp;
    }
}
DesX = DesPoints[Num][0];
DesY = DesPoints[Num][1];
OldX = DesX;
OldY = DesY;
#endif
        PointS[Location][0] = DesX-SrcX1;
        PointS[Location][1] = DesY-SrcY1 ;
//iLog("%d\t%d",PointS[Location][0],PointS[Location][1]);
        SrcY++;
        SrcY1++;		
    }	

CHECK:
    for ( Location = 0; Location <SrcW; Location++)
    {
        PointS[Location][0] += SrcMoveX;
        PointS[Location][1] += SrcMoveY;
#if DEBUG_I51KITG2
//		 iLog("PointS[%d][0]=%d,PointS[%d][0]=%d",Location,PointS[Location][0],Location,PointS[Location][1]);
#endif
    }	

    return iTRUE;
	
}

static iBOOL RotateRamLinearImage  ( iHANDLE ContainerHandle,iU32 ImageID , iRECT* DesRect , iRECT* SrcRect , iS16 ChangeData[][2])
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

   // ͼƬ������ַ���
    i51ImageContainer * ImageContainer = iNULL;
    // ͼƬ�������ַ
    i51ImageIndex * ImageIndexTable = iNULL;
    // ͼƬ��Ⱥ͸߶�
    iU32 ImageWidth = 0;
    iU32 ImageHeight = 0;
    // �����ļ�ָ���ַ
    iFILE File = iNULL;
    // �ļ�������������ֵ
//    iU8   FlagReturn = iFALSE;
    // ��ȡ�ļ��Ļ�����
    iU8  * ReadDataBuff = iNULL;
    // ��ReadDataBuff�������Դ��������ʼ��ַ
    iU32 AddStartCopy = 0;
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
	
    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = ( i51ImageContainer * )ContainerHandle;

    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL==ImageIndexTable || iNULL==File ) return iFALSE;

    DataEntry = ImageIndexTable[ImageID].MemoryEntry;
    ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    ImageHeight = ImageIndexTable[ImageID].ImageHeight;
    LengthIamgeData = ImageIndexTable[ImageID].LengthIamgeData;
    TotalAlphaNumber = ImageIndexTable[ImageID].AlphaPixelNum;

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
#if DEBUG_I51KITG2
//	 iLog("SrcStartX=%d,SrcStartY=%d,SrcEndX=%d,SrcEndY=%d",SrcStartX,SrcStartY,SrcEndX,SrcEndY);
#endif

    if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
    {
        DataEntry = ImageIndexTable[ImageID].ImageEntry;
        FlagReturn = i51AdeOsFileSetSeek( File, (DataEntry), i51_OS_FL_SEEK_HEAD );
        if( iFALSE == FlagReturn ) goto ERROR;                      
    
        LengthNeedReadData =ImageIndexTable[ImageID].LengthTotalData;
    
        ImageDataEntry = i51AdeOsMalloc(LengthNeedReadData, __FILE__ , __LINE__ );
        if( iNULL == ImageDataEntry ) 
        {
            iLog("KIT - G2 : malloc linear normal");
            return iFALSE;
        }
    	
        i51AdeOsFileRead( File, ImageDataEntry, LengthNeedReadData, &LengthTotalRead);
        if( iFALSE == FlagReturn || LengthNeedReadData!=LengthTotalRead) 
        {
            i51AdeOsFree(ImageDataEntry);
            ImageDataEntry = iNULL;
            goto ERROR;                
        }
    }
    else
    {
        ImageDataEntry = (iU8*)ImageIndexTable[ImageID].MemoryEntry;
    }

    DataEntry = (iU32)ImageDataEntry;
    LengthTotalReadData = I51KITG2_OFFSET_PIXELDATA;
    DataEntry += I51KITG2_OFFSET_PIXELDATA;
  
    if(AlphaSys==i51KITG2_ALPHA_NONEED)
    {
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
            AddStartCopy = (iU32)ReadDataBuff;
            
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
    #if LCDBUFF_DEBUG
    i51KitG2CheckLcdbuffer(RotateY*ScreenWidth+ RotateX,1);
    #endif
#if DEBUG_I51KITG2
// iLog("CurrX=%d,CurrY=%d,RotateX=%d,RotateY=%d",CurrX,CurrY,RotateX,RotateY);
#endif
                BuffPos = RotateY*ScreenWidth+ RotateX;
                LcdBuffer[BuffPos] = ColorBuff[iTemp];
                LcdBuffer[BuffPos+1] = ColorBuff[iTemp];
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
//            iU32 TempHeight = ImageHeight -1;
//            iU32 TempWidth = ImageWidth-1;
			
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
                LcdBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(LcdBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
            }
        }
    }
    else
    {
        while(1)
        {
            TempReadDataBuff1 = (iU16 *)(DataEntry);   
            CurrentImageX = TempReadDataBuff1[0];
            CurrentImageY = TempReadDataBuff1[1];
            CurrentImageWidth = TempReadDataBuff1[2];
            
            DataEntry += 6;
            LengthTotalReadData += 6 + (CurrentImageWidth<<1);

            if( (CurrentImageY < SrcStartY) || (CurrentImageX > SrcEndX) )
            {
                DataEntry += ( CurrentImageWidth<<1 );
                continue;
            }
            
            if( CurrentImageY > SrcEndY ) break;
            ColorBuff = (iU16*)DataEntry;
            DataEntry += ( CurrentImageWidth<<1 );
            LengthCopyData = CurrentImageWidth;
            AddStartCopy = (iU32)ReadDataBuff;
            
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
    #if LCDBUFF_DEBUG
    i51KitG2CheckLcdbuffer(RotateY*ScreenWidth+ RotateX,1);
    #endif
                BuffPos = RotateY*ScreenWidth+ RotateX;
//                BuffPos = RotateY*ScreenWidth+ RotateX;
                TempColor = i51KitG2GetNewColor(LcdBuffer[BuffPos], ColorBuff[iTemp]);
                LcdBuffer[BuffPos] = TempColor;
                LcdBuffer[BuffPos+1] = TempColor;
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
//            iU32 TempHeight = ImageHeight -1;
//            iU32 TempWidth = ImageWidth-1;
			
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
                LcdBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(LcdBuffer[LcdBuffPos], AlphaColor, AlphaNumber*AlphaSrc/255);
            }
        }
    }
#if DEBUG_I51KITG2
//iLog("Rotate OK" );
#endif	
    if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
    {
        i51AdeOsFree(ImageDataEntry);
        ImageDataEntry = iNULL;
    }
    return iTRUE;

ERROR:
    iLog("KIT - G2 : RW rotate");
    if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
    {
        i51AdeOsFree(ImageDataEntry);
        ImageDataEntry = iNULL;
    }
    return iFALSE;
	
}

#if 0
static iBOOL i51KitG2ImageDecode2File( iFILE SrcFile, iFILE DesFile)
{
    // author : Otto
    // since : Aug 31 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE SrcFile : ��Ҫ��������ݴ���ļ�ָ��
    // iFILE DesFile : ���������ݴ���ļ�ָ��

    // notes : ���ļ�SrcFile�н�ѹһ��ͼƬ���ļ�DesFile��
    // SrcFile�ļ��д�ŵ�ͼƬ�����Ƿֶ���Zlibѹ����
    // �����ļ���ʽ��ο��ĵ�"The_i51_DC_dynamic_library_G2_i51_Image_Format.xls"

    // ���ļ��ж�ȡ��CRC
    iU32 CRC_Read = 0;
    // ��ȡ�ļ���Ϣ������õ���CRC
    iU32 CRC_Calculate = 0;
    // Ŀ�����ݳ��ȣ�����ѹ���ļ�����
    iU32 LengthDesData = 0;
    // ԭʼ���ݳ��ȣ���ѹ��ǰ�ļ�����
    iU32 LengthSrcData = 0;
    // ��ѹ��������,Ĭ������Ϊ32K
    iU32 LengthPartMax = 0;
    // ������
    iU32 NumberParts = 0;
    // ��ǰ�ν�ѹǰ���ݳ���
    iU64 LengthPartSrc = 0;
    // ��ǰ�ν�ѹ�����ݳ���
    iU64 LengthPartDes = 0;
    // ��������ֵ
    iBOOL FlagReturn = iFALSE;
    // ͼƬ���ݿ�ʼλ��ƫ����
    iU32 FileSeekStart = 0;
    // �ļ���ǰƫ����
    iU32 FileSeekCureent = 0;
    // ��ѹǰ������buffer
    iU8  * DataSrcCurrentPart = iNULL;
    // ��ѹ�������buffer
    iU8  * DataDesCurrentPart = iNULL;
    // ʵ�ʴ��ļ���һ�ζ�ȡ�������ݳ���  
    iU32 LengFactReadData = 0;
    // ʵ��һ��д���ļ��е����ݳ���  
    iU32 LengFactWriteData = 0;
    iU32 iTemp = 0;
    iU32 FlagFirst = iTRUE;

    DataSrcCurrentPart = i51AdeOsMalloc(I51KITG2_LENGTH_PART_UMCOMPRESS, __FILE__ , __LINE__ );
    if( iNULL == DataSrcCurrentPart )	return iFALSE;
	
    DataDesCurrentPart = i51AdeOsMalloc(I51KITG2_LENGTH_PART_UMCOMPRESS, __FILE__ , __LINE__ );
    if( iNULL == DataDesCurrentPart )
    {
        i51AdeOsFree(DataSrcCurrentPart);
        DataSrcCurrentPart = iNULL;		
        return iFALSE;
    }

    FlagReturn = i51AdeOsFileGetSeek( SrcFile, (iS32*)(&FileSeekStart), i51_OS_FL_SEEK_HEAD );
    if( iFALSE == FlagReturn ) return iFALSE;

    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &CRC_Read);
    if( iFALSE == FlagReturn ) return iFALSE;
  
    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &LengthSrcData);
    if( iFALSE == FlagReturn ) return iFALSE;
  
    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &LengthDesData);
    if( iFALSE == FlagReturn ) return iFALSE;
  
    FlagReturn = i51AdeOsFileReadInt32( SrcFile, &LengthPartMax);
    if( iFALSE == FlagReturn ) return iFALSE;
  
    FlagReturn = i51AdeOsFileReadInt16( SrcFile, (iU16*)(&NumberParts));
    if( iFALSE == FlagReturn ) return iFALSE;

    for( iTemp=0; iTemp<NumberParts; iTemp++ )
    {
        FlagReturn = i51AdeOsFileReadInt32( SrcFile, (iU32*)(&LengthPartSrc));
        if( iFALSE == FlagReturn ) return iFALSE;

        FlagReturn = i51AdeOsFileRead( SrcFile, DataSrcCurrentPart, LengthPartSrc, &LengFactReadData);
        if( (iFALSE == FlagReturn) || (LengthPartSrc!=LengFactReadData)  ) return iFALSE;

        LengthPartDes = I51KITG2_LENGTH_PART_UMCOMPRESS;
        FlagReturn = i51AdeStdUncompress( DataDesCurrentPart, &LengthPartDes, DataSrcCurrentPart, LengthPartSrc);
        if( iFALSE == FlagReturn ) return iFALSE;

        if( LengthPartDes < I51KITG2_LENGTH_PART_UMCOMPRESS)
        {
           FlagFirst = iFALSE;
        }
        FlagReturn = i51AdeOsFileWrite( DesFile, DataDesCurrentPart, LengthPartDes, &LengFactWriteData );
        if( (iFALSE == FlagReturn) || (LengthPartDes!=LengFactWriteData)  ) return iFALSE;
    }

    i51AdeOsFileFlush( DesFile );

    return iTRUE;
  
}
#endif

#if DEBUG_I51KITG2

static iBOOL i51KitG2DrawLineEx( iS32 Point[][2], iCOLOR Color)
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

    // ��������	
    iS16 DrawRectStartX = 0;
    iS16 DrawRectStartY = 0;
    iS16 DrawRectEndX = ScreenWidth;
    iS16 DrawRectEndY = ScreenHeight;
    iU32 BuffPos = 0;
    iS32 Temp1 = 0;

	
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
    for( Temp1=0; Temp1<2; Temp1++ )
    {
        if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
         ||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
        )
        {
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
              ScreenBuffer[ (ScreenWidth*TempY1 + TempX1)] = Color;
        }

         if( TempX2>=DrawRectStartX && TempX2<DrawRectEndX && TempY2>=DrawRectStartY&& TempY2<DrawRectEndY )
        {
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

    return iTRUE;

}

static iU32 i51KitG2GetImageAttrEx(iU32  SrcData, iU32 * ImageWidth, iU32 * ImageHeight)
{
    // ǰ���ĸ��ֽ��Ǳ����ͼƬ��Ⱥ͸߶�	
    iU8 * SrcDataPoint = (iU8 * )(SrcData);
    iU16 ImageW = 0;
    iU16 ImageH = 0;	

    if( SrcData==iNULL ) return iFALSE;

    ImageW = 	SrcDataPoint[0]|(SrcDataPoint[1]<<8);
    ImageH = 	SrcDataPoint[2]|(SrcDataPoint[3]<<8);
//    i51AdeStdMemcpy16(&ImageW, &SrcDataPoint[0], 2);
//    i51AdeStdMemcpy16(&ImageH, &SrcDataPoint[2], 2);

    *ImageWidth = ImageW;
    *ImageHeight = ImageH;
	
    return iTRUE;

}

static iU32 i51KitG2ImageCreate( iU32  SrcData)
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

static iBOOL i51KitG2ImageDestory( iU32  SrcData)
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


static iCOLOR i51KitG2GetNewColorEx1 (iCOLOR dpixel, iCOLOR spixel, iALPHA SrcAlpha)
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

static iBOOL i51KitG2DrawImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect)
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
	
//iLog("DesStartX=%d",DesStartX);
//iLog("DesStartY=%d",DesStartY);
//iLog("DesEndX=%d",DesEndX);
//iLog("DesEndY=%d",DesEndY);
	
    while(1)
    {
        TempReadDataBuff1 = (iU16 *)(ImageDataPoint);   
        CurrentImageX = TempReadDataBuff1[0];
        CurrentImageY = TempReadDataBuff1[1];
        CurrentImageWidth = TempReadDataBuff1[2];
//CurrentImageX = ImageDataPoint[1]|(ImageDataPoint[0]<<8);
//CurrentImageY = ImageDataPoint[3]|(ImageDataPoint[2]<<8);
//CurrentImageWidth = ImageDataPoint[5]|(ImageDataPoint[4]<<8);
//iLog("CurrentImageX=%d",CurrentImageX);
//iLog("CurrentImageY=%d",CurrentImageY);
//iLog("CurrentImageWidth=%d",CurrentImageWidth);
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
    return iTRUE;
	
}

static iBOOL i51KitG2MirrorImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect)
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


static iBOOL i51KitG2FlipImageEx(iU32 ImageData, iRECT * SrcRect, iRECT * DesRect)
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

static iS32 i51KitG2SinEx( iS32 Angle)
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

static iS32 i51KitG2CosEx( iS32 Angle)
{
    //iS32 Angle: ��Ҫ����ĽǶ�ֵ
    // notes : ���ؽǶ�Angle�������Ŵ�10000����ֵ
    return i51KitG2Sin(Angle+90);
}

static iBOOL i51KitG2RotatePointEx(iS32 SrcX, iS32 SrcY, iS32 RotateCenterX,  iS32 RotateCenterY ,iS32 * DesX, iS32 * DesY, iS32 Angle)
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

static iBOOL i51KitG2GetGirthPointsEx(iRECT SrcRect, iRECT DesRect, iS16 PointS[][2], iS32 SrcMoveX, iS32 SrcMoveY,
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


static iBOOL RotateRamLinearImageEx  ( iU32 ImageData , iRECT* DesRect , iRECT* SrcRect , iS16 ChangeData[][2])
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

static iBOOL i51KitG2RotateImageEx(iU32 ImageData, iRECT* DesRect , iRECT* SrcRect, 
	iS32 SrcMoveX , iS32 SrcMoveY, iS32 RotateCenterX , iS32 RotateCenterY , iU32 RotateType, iS32 Angle )
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
	
    if( iNULL==i51KitG2GetGirthPoints(DrawSrcRect, DrawDesRect,(iS16(*)[2])GirthPoint, SrcMoveX, SrcMoveY, RotateCenterX, RotateCenterY,RotateType, Angle))
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

#endif

iKIT iBOOL i51KitG2SetAlpha( iALPHA Alpha )
{
    // ����ϵͳ͸����
    AlphaSys = Alpha;
    AlphaSrc = Alpha;
    AlphaDes = 0XFF-Alpha;
	 
    return iTRUE;
}

iKIT iBOOL i51KitG2CleanScreen(iCOLOR Color, iRECT *Rect)
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

 #if i51KITG2_DEBUG
    iLog( "G2CleanScreen Start , %x, %x", Color, Rect) ;
#endif

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

    if(AlphaSys==i51KITG2_ALPHA_NONEED)
    {
        for( TempX=0; TempX<CleanW ; TempX++)
        {
            OneRowColorBuf[TempX] = Color;
        }
    
        CleanW = (CleanW<<1);	
        LcdBufPos = CleanY*ScreenWidth + CleanX;
        for( TempY=0; TempY<CleanH; TempY++)
        {
            i51AdeStdMemcpy16(&(LcdBuffer[LcdBufPos]), OneRowColorBuf, CleanW);
            LcdBufPos += ScreenWidth;
        }
    }
    else
    {
        for( TempY=0; TempY<CleanH; TempY++)
        {
            LcdBufPos = (CleanY+TempY)*ScreenWidth + CleanX;
            for( TempX=0; TempX<CleanW ; TempX++)
            {
                LcdBuffer[LcdBufPos] = i51KitG2GetNewColor(LcdBuffer[LcdBufPos], Color);
                LcdBufPos++;
            }
        }
    }
	
  #if i51KITG2_DEBUG
    iLog( "G2CleanScreen End , %x, %x", Color, Rect) ;
#endif
    return iTRUE;
	
}

iKIT iBOOL i51KitG2DrawPoint( iS32 X, iS32 Y, iCOLOR Color )
{

    // author : Otto
    // since : Aug 10 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iS32 X : ��Ҫ���Ƶ��X����
    // iS32 Y : ��Ҫ���Ƶ��Y����
    // iCOLOR Color : ���Ƶ���ɫ

    // notes : ������(X, Y)����һ��Color��ɫ�ĵ�

    // VP�ϵ�ǰ���Ƶ�Ԫ�ر��
    iU16 CurrDrawID;
    // ����Ļ�ϻ��Ƶ�����	
    iS32 DrawX = 0, DrawY = 0;   
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
    iS16 DrawRectEndX = ScreenWidth;
    iS16 DrawRectEndY = ScreenHeight;
    iU32 BuffPos = 0;

 #if i51KITG2_DEBUG
    iLog( "G2DrawPoint Start , %x, %x, %x", X, Y, Color) ;
#endif
  
    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_POINT;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Point.X = X;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Point.Y = Y;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Point.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    if( i51UseVirtualPanel )
    {
        VPActualRect = &(i51CurrentVpRender->VPActualRect);
        VPDrawRect = &(i51CurrentVpRender->VPDrawRect);
  
        VP_TOP_LEFT_X = VPActualRect->X;
        VP_TOP_LEFT_Y = VPActualRect->Y;
        VP_LOVER_RIGHT_X = VP_TOP_LEFT_X + VPActualRect->Width;
        VP_LOVER_RIGHT_Y = VP_TOP_LEFT_Y + VPActualRect->Height;

        // ����Ļ�ϵĻ�������
        DrawRectStartX = VPDrawRect->X;
        DrawRectStartY = VPDrawRect->Y;
        DrawRectEndX = DrawRectStartX + VPDrawRect->Width;
        DrawRectEndY = DrawRectStartY + VPDrawRect->Height;

        if( DrawRectStartX < 0 ) DrawRectStartX = 0;
        if( DrawRectStartY < 0 ) DrawRectStartX = 0;
        if( DrawRectEndX >= ScreenWidth ) DrawRectEndX = ScreenWidth;
        if( DrawRectEndY >= ScreenHeight) DrawRectEndY = ScreenHeight;

        // ��Ҫ���Ƶĵ�����Ļ�ϵ�����
        DrawX = X + VPActualRect->X;
        DrawY = Y + VPActualRect->Y;
    }
    else
    {
        DrawX = X;
        DrawY = Y;
    }
	
    if( DrawX >= DrawRectEndX) return iTRUE;
    if( DrawY >= DrawRectEndY) return iTRUE;
    if( DrawX < DrawRectStartX ) return iTRUE;
    if( DrawY < DrawRectStartX ) return iTRUE;

#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth * DrawY + DrawX,1);
#endif
    if(AlphaSys==i51KITG2_ALPHA_NONEED)
    {
        LcdBuffer[ (ScreenWidth * DrawY + DrawX) ] = Color;
    }
    else
    {
        BuffPos = ScreenWidth * DrawY + DrawX;
        LcdBuffer[ BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
    }
		

 #if i51KITG2_DEBUG
    iLog( "G2DrawPoint End , %x, %x, %x", X, Y, Color) ;
#endif

    return iTRUE;

}

iKIT iBOOL i51KitG2DrawLine( iS32 Point[][2], iCOLOR Color)
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
    iS16 DrawRectEndX = ScreenWidth;
    iS16 DrawRectEndY = ScreenHeight;
    iU32 BuffPos = 0;
    iS32 Temp1 = 0;
	
 #if i51KITG2_DEBUG
    iLog( "G2DrawLine Start , %x, %x", Point, Color) ;
#endif

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

    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_LINE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Line.Points[0][0] = Point[0][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Line.Points[0][1] = Point[0][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Line.Points[1][0] = Point[1][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Line.Points[1][1] = Point[1][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Line.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    if( i51UseVirtualPanel )
    {
        VPActualRect = &(i51CurrentVpRender->VPActualRect);
        VPDrawRect = &(i51CurrentVpRender->VPDrawRect);
  
        VP_TOP_LEFT_X = VPActualRect->X;
        VP_TOP_LEFT_Y = VPActualRect->Y;
        VP_LOVER_RIGHT_X = VP_TOP_LEFT_X + VPActualRect->Width;
        VP_LOVER_RIGHT_Y = VP_TOP_LEFT_Y + VPActualRect->Height;

        // ����Ļ�ϵĻ�������
        DrawRectStartX = VPDrawRect->X;
        DrawRectStartY = VPDrawRect->Y;
        DrawRectEndX = DrawRectStartX + VPDrawRect->Width;
        DrawRectEndY = DrawRectStartY + VPDrawRect->Height;

        if( DrawRectStartX < 0 ) DrawRectStartX = 0;
        if( DrawRectStartY < 0 ) DrawRectStartX = 0;
        if( DrawRectEndX >= ScreenWidth ) DrawRectEndX = ScreenWidth;
        if( DrawRectEndY >= ScreenHeight) DrawRectEndY = ScreenHeight;

        StartX = Point[0][0] +  VPActualRect->X;
        StartY = Point[0][1] +  VPActualRect->Y;
        EndX = Point[1][0] +  VPActualRect->X;
        EndY = Point[1][1] +  VPActualRect->Y;
    }
    else
    {
        DrawRectStartX = 0;
        DrawRectStartY = 0;
        DrawRectEndX = ScreenWidth;
        DrawRectEndY = ScreenHeight;
        StartX = Point[0][0];
        StartY = Point[0][1];
        EndX = Point[1][0];
        EndY = Point[1][1];
    }

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
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY1 + TempX1)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY1 + TempX1;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
        }

         if( TempX2>=DrawRectStartX && TempX2<DrawRectEndX && TempY2>=DrawRectStartY&& TempY2<DrawRectEndY )
        {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY2 + TempX2,1);
#endif
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY2 + TempX2)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY2 + TempX2;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
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

iKIT iBOOL i51KitG2DrawPolygon( iU32 VerticeNumber, iS32 Point[][2], iCOLOR Color)
{

    // author : Otto
    // since : Aug 11 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 VerticeNumber : ����ζ������,������ڵ���3
    // iS32 Point[][2] :
    //    Point[0][0]����Ҫ���ƶ���εĵ�һ������X����
    //    Point[0][1]����Ҫ���ƶ���εĵ�һ������Y����
    //    Point[Num-1][0]����Ҫ���ƶ���ε����һ������X����
    //    Point[Num-1][1]����Ҫ���ƶ���ε����һ������X����
    // iCOLOR Color : ���ƶ���ε���ɫ

    // notes : ���������ǻ�����VerticeNumber������,����������Point��ŵĶ����
    //    Point�����д�ŵĶ������������ģ������������߶�AB��
    //    �߶�BC���߶�CD���߶�DE���߶�EA������ɣ�
    //    ��Point[0][0]��Ŷ���A��X����,Point[0][1]��Ŷ���A��Y����
    //    ��Point[1][0]��Ŷ���B��X����,Point[1][1]��Ŷ���B��Y����
    //    ��Point[2][0]��Ŷ���C��X����,Point[2][1]��Ŷ���C��Y����
    //    ��Point[3][0]��Ŷ���D��X����,Point[3][1]��Ŷ���D��Y����
    //    ��Point[4][0]��Ŷ���E��X����,Point[4][1]��Ŷ���E��Y����

    iU32 CurrentSide = 0;
    iS32 CurrentPoint[2][2] = {0};
    iU32 CopyLength = 0;
	
    // ��ǰ���Ƶ�Ԫ����VP�ϵ�ID
    iU16 CurrDrawID;
    iU32 PonterPoints;
    iS32 Temp1 = 0;

 #if i51KITG2_DEBUG
    iLog( "G2DrawPolygon Start , %x, %x, %x", VerticeNumber, Point,Color) ;
#endif

    if( VerticeNumber < 3 ) return iFALSE;

    for( Temp1=0; Temp1<VerticeNumber; Temp1++ )
    {
        if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
         ||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
        )
        {
 #if i51KITG2_DEBUG
    iLog( "G2DrawPolygon Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
#endif
            return iFALSE;
        }
    }     	
    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        CopyLength = sizeof(iS32) * 2*VerticeNumber;
        PonterPoints = ( iU32 )i51AdeOsMalloc( CopyLength,  __FILE__ , __LINE__ );
        if( !PonterPoints) return iFALSE;
        i51AdeStdMemcpy16( (void *)PonterPoints, (void*)Point, CopyLength);  

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_POLYGON;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Polygon.VerticeNumber = VerticeNumber;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Polygon.PointerPoints = PonterPoints;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Polygon.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    CopyLength = sizeof(iU32) * 4;

    for( CurrentSide=0; CurrentSide<VerticeNumber-1; CurrentSide++ )
    {
          i51AdeStdMemcpy16( CurrentPoint, &Point[CurrentSide][0], CopyLength);  
         i51KitG2DrawLine( CurrentPoint, Color);
    }

    CurrentPoint[0][0] = Point[0][0];
    CurrentPoint[0][1] = Point[0][1];
    CurrentPoint[1][0] = Point[VerticeNumber-1][0];
    CurrentPoint[1][1] = Point[VerticeNumber-1][1];
    i51KitG2DrawLine( CurrentPoint, Color);

 #if i51KITG2_DEBUG
    iLog( "G2DrawPolygon End , %x, %x, %x", VerticeNumber, Point,Color) ;
#endif
    return iTRUE;
	
}

iKIT iBOOL i51KitG2DrawArc( iS32 Point[][2], iCOLOR Color )
{

    // author : Otto
    // since : Aug 11 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iS32 Point[][2] :
    //    Point[0][0]����Ҫ���ƻ��ߵ����X����
    //    Point[0][1]����Ҫ���ƻ��ߵ����Y����
    //    Point[1][0]�Ǿ���������������X����
    //    Point[1][1]�Ǿ���������������Y����
    //    Point[2][0]����Ҫ���ƻ��ߵ��յ�X����
    //    Point[2][1]����Ҫ���ƻ��ߵ��յ�Y����
    // iCOLOR Color : ���ƻ��ߵ���ɫ

    // notes : ������(Point[0][0], Point[0][1])Ϊ���,(Point[2][0],Point[2][1])Ϊ�յ�,������(Point[1][0],Point[1][1])�Ļ���
    // ��֪ABC����,���ABC���ԲԲ�ĵĹ�ʽΪ��
    //    CenterX=(Qy+K1Px-K2Qx-Py)/(K2-K1)
    //    CenterY=(K1K2Qx+K2Py-K1Qy-K1K2Px)/(K2-K1)
    //    ����(Px, Py)���߶�AB�е�����, (Qx, Qy)���߶�BC���е�����, K1���߶�AB���д���б��, K2���߶�BC���д���б��

    //����X�ľ���ֵ
    #define ABSOLUTE(X) (X) > 0 ? (X) :(-(X))
    #define CLOCK_WISE    1
    #define ANTI_CLOCK_WISE 2

    iS32 OldX = 0;
    iS32 OldY = 0;
    iS32 CenterX = 0;
    iS32 CenterY = 0;
    iS32 Molecular = 0;
    iS32 Denominator = 0;
    iS32 CurrentX = 0;
    iS32 CurrentY = 0;
    iS16 RegulationY = 1;
    iS16 RegulationX = 1;
    iS32 SquareR = 0;
    iS32 Xa, Xb, Xc, Ya, Yb, Yc;
    iS32 PerpendicularIntersectionY = 0;
    iS32 SquareDifferential[3] = {0};
    iU8  AirDirection = CLOCK_WISE;

    iS32 TempX, TempY;
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
    iS16 DrawRectEndX = ScreenWidth;
    iS16 DrawRectEndY = ScreenHeight;
    iU32 BuffPos = 0;
    iS32 Temp1 = 0;
	
 #if i51KITG2_DEBUG
    iLog( "G2DrawArc Start , %x, %x", Point,Color) ;
#endif
    for( Temp1=0; Temp1<3; Temp1++ )
    {
        if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
         ||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
        )
        {
 #if i51KITG2_DEBUG
    iLog( "G2DrawArc Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
#endif
            return iFALSE;
        }
    }     	
	
    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_ARC;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Points[0][0] = Point[0][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Points[0][1] = Point[0][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Points[1][0] = Point[1][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Points[1][1] = Point[1][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Points[2][0] = Point[2][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Points[2][1] = Point[2][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Arc.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;

        return iTRUE;

    }

    if( i51UseVirtualPanel )
    {
        VPActualRect = &(i51CurrentVpRender->VPActualRect);
        VPDrawRect = &(i51CurrentVpRender->VPDrawRect);
  
        VP_TOP_LEFT_X = VPActualRect->X;
        VP_TOP_LEFT_Y = VPActualRect->Y;
        VP_LOVER_RIGHT_X = VP_TOP_LEFT_X + VPActualRect->Width;
        VP_LOVER_RIGHT_Y = VP_TOP_LEFT_Y + VPActualRect->Height;

        // ����Ļ�ϵĻ�������
        DrawRectStartX = VPDrawRect->X;
        DrawRectStartY = VPDrawRect->Y;
        DrawRectEndX = DrawRectStartX + VPDrawRect->Width;
        DrawRectEndY = DrawRectStartY + VPDrawRect->Height;

        if( DrawRectStartX < 0 ) DrawRectStartX = 0;
        if( DrawRectStartY < 0 ) DrawRectStartX = 0;
        if( DrawRectEndX >= ScreenWidth ) DrawRectEndX = ScreenWidth;
        if( DrawRectEndY >= ScreenHeight) DrawRectEndY = ScreenHeight;
		
        Xa = Point[0][0] + VP_TOP_LEFT_X;
        Ya = Point[0][1] + VP_TOP_LEFT_Y;
        Xb = Point[1][0] + VP_TOP_LEFT_X;
        Yb = Point[1][1] + VP_TOP_LEFT_Y;
        Xc = Point[2][0] + VP_TOP_LEFT_X;
        Yc = Point[2][1] + VP_TOP_LEFT_Y;
    }
    else
    {
        DrawRectStartX = 0;
        DrawRectStartY = 0;
        DrawRectEndX = ScreenWidth;
        DrawRectEndY = ScreenHeight;
        Xa = Point[0][0];
        Ya = Point[0][1];
        Xb = Point[1][0];
        Yb = Point[1][1];
        Xc = Point[2][0];
        Yc = Point[2][1];
    }
	


    //����ͬһֱ�����������ȷ��һ��Բ,����������������غ����޷�ȷ��Բ
    if( Xa==Xb&&Ya==Yb ) return iFALSE;
    if( Xa==Xc&&Ya==Yc ) return iFALSE;
    if( Xb==Xc&&Yb==Yc ) return iFALSE;

    //������ͬһֱ�����޷�ȷ��Բ
    if( Xa!=Xc && Xa!=Xb )
    {
         if( (((Ya-Yc)*100)/(Xa-Xc)) ==(((Ya-Yb)*100)/(Xa-Xb)) ) return iFALSE;
    }
    if( Xa==Xb && Xa==Xc ) return iFALSE;
    if( Ya==Yb && Ya==Yc ) return iFALSE;

    //������ߵĶ���ΪԲ�������
    if( Xa > Xc )
    {
        Xa += Xc;
        Xc = Xa - Xc;
        Xa = Xa - Xc;
  
        Ya += Yc;
        Yc = Ya - Yc;
        Ya = Ya - Yc;
    }

    //����Բ��X����
    Molecular = (Yc-Ya)*(Yc-Yb)*(Yb-Ya) + ( Xa*Xa-Xb*Xb)*(Yc-Yb) + (Xc*Xc - Xb*Xb)*(Yb-Ya);
    Denominator = ( (Xb-Xc)*(Ya-Yb) + (Xb-Xa)*(Yb-Yc) ) << 1;
    CenterX = ((Molecular*10)/Denominator+5)/10;

    //����Բ��Y����
    Molecular = (Xb-Xa)*(Xc-Xb)*(Xc-Xa) + (Xb-Xc)*(Ya+Yb)*(Yb-Ya) + (Xb-Xa)*(Yb+Yc)*(Yc-Yb);
    Denominator = ( (Xb-Xc)*(Yb-Ya) + (Xb-Xa)*(Yc-Yb) ) << 1;
    CenterY = ((Molecular*10)/Denominator + 5 ) / 10;


    //����Բ�뾶��ƽ��
    SquareR = ( Xc - CenterX ) * ( Xc - CenterX ) + ( Yc - CenterY ) * ( Yc - CenterY );


    //���㾭����B����ֱ��AC��ֱ��ֱ����ֱ��AC�Ľ���X����
    //  Molecular = (Yb-Ya)*(Xc-Xa)*(Ya-Yc) - (Xc-Xa)*(Xc-Xa)*Xb - (Yc-Ya)*(Yc-Ya) * Xa;
    //  Denominator = (Yc-Ya)*(Ya-Yc) - (Xc-Xa)*(Xc-Xa);
    Molecular = (Yc-Ya)*(Ya*Yb-Yb*Yc+Xa*Xb-Xc*Xb) + (Xa-Xc)*(Ya*Xc-Yc*Xa);
    Denominator = (Yc-Ya)*(Ya-Yc) - (Xc-Xa)*(Xc-Xa);
    PerpendicularIntersectionY =  ((Molecular*10)/Denominator + 5 ) / 10;

    //�жϻ�Բ������,���B���ڴ����·���Բ������Ϊ��ʱ��,����Ϊ˳ʱ��
    if( Yb > PerpendicularIntersectionY )
    {
        AirDirection = ANTI_CLOCK_WISE;
    }
    else if( Yb==PerpendicularIntersectionY )
    {
        if(  Xb<CenterX )
        {
             AirDirection = ANTI_CLOCK_WISE;
        }
    }

    //�жϻ�Բ������Ϊ˳ʱ��ʱX,Y�ı仯����,���ݶԳ���,���Ϊ��ʱ���������Ƿ���
    if ( Ya < CenterY )
    {
        RegulationX = 1;
    }
    else if ( Ya > CenterY )
    {
        RegulationX = -1;
    }

    if ( Xa < CenterX )
    {
        RegulationY = -1;
    }
    else if ( Xa > CenterX )
    {
        RegulationY = 1;
    }

    if( AirDirection == ANTI_CLOCK_WISE )
    {
        RegulationX = -RegulationX;
        RegulationY = -RegulationY;
    }

    if( Ya==CenterY)
    {
        if( Xa > CenterX )
        {
             RegulationX = -1;
        }
        else
        {
            RegulationX = 1;
        }
    }

    if( Xa==CenterX )
    {
        if( Ya > CenterY )
        {
              RegulationY = -1;
        }
        else
        {
             RegulationY = 1;
        }
    }

    CurrentX = Xa;
    CurrentY = Ya;
    TempX = CurrentX;
    TempY = CurrentY;

    if( TempX>=DrawRectStartX&&TempX<DrawRectEndX&&TempY>=DrawRectStartY&&TempY<DrawRectEndY )
    {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY+ TempX,1);
#endif
        if(AlphaSys==i51KITG2_ALPHA_NONEED)
        {
            LcdBuffer[ (ScreenWidth*TempY + TempX)] = Color;
        }
        else
        {
            BuffPos = ScreenWidth*TempY + TempX;
            LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
        }
    }

    while(1)
    {
        if( CurrentX==Xc&&CurrentY==Yc) break;

  
        OldX = CurrentX;
        OldY = CurrentY;

        SquareDifferential[0] = ABSOLUTE( (CurrentX+RegulationX-CenterX)*(CurrentX+RegulationX-CenterX)\
                                                         +(CurrentY-CenterY)*(CurrentY-CenterY) - SquareR);
        SquareDifferential[1] = ABSOLUTE( (CurrentY +RegulationY -CenterY)*(CurrentY +RegulationY -CenterY)\
                                                         +(CurrentX-CenterX)*(CurrentX-CenterX) - SquareR);
        SquareDifferential[2] = ABSOLUTE( (CurrentY +RegulationY -CenterY)*(CurrentY +RegulationY -CenterY)\
                                                         +(CurrentX+RegulationX-CenterX)*(CurrentX+RegulationX-CenterX) - SquareR);
        if( SquareDifferential[0]<SquareDifferential[1] )
        {
            if( SquareDifferential[0]<SquareDifferential[2] )
            {
                CurrentX += RegulationX ;
            }
            else
            {
                CurrentX += RegulationX ;
                CurrentY += RegulationY ;
            }
        }
        else
        {
            if( SquareDifferential[1]<SquareDifferential[2] )
            {
                CurrentY += RegulationY ;
            }
            else
            {
                CurrentX += RegulationX ;
                CurrentY += RegulationY ;
            }
        }
  
        TempX = CurrentX;
        TempY = CurrentY;
  
        if( TempX>=DrawRectStartX&&TempX<DrawRectEndX&&TempY>=DrawRectStartY&&TempY<DrawRectEndY )
        {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY+ TempX,1);
#endif
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY + TempX)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY + TempX;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
        }
  
        if( CurrentX==CenterX && OldX!=CurrentX )
        {
            RegulationY = -RegulationY;
        }

        if( CurrentY==CenterY && OldY!=CurrentY )
        {
            RegulationX = -RegulationX;
        }
    }

  #undef ABSOLUTE
  #undef CLOCK_WISE
  #undef ANTI_CLOCK_WISE

 #if i51KITG2_DEBUG
    iLog( "G2DrawArc End , %x, %x", Point,Color) ;
#endif

    return iTRUE;

}

iKIT iBOOL i51KitG2DrawCircle( iS32 X, iS32 Y, iU32 R, iCOLOR Color )
{

    // author : Otto
    // since : Aug 12 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iS32 X : Բ��X����
    // iS32 Y : Բ��Y����
    // iU32 R : Բ�뾶
    // iCOLOR Color : ���ƻ��ߵ���ɫ

    // notes : ������(X, Y)ΪԲ��,�뾶ΪR��Բ
    // ����Բ�ĶԳ��ԣ������ҵ�Բ��һ�����ȷ����֮X��Գơ�Y��Գ��Ѿ����ĶԳƵ���������
    // ���Լ���ʱֻ��Ҫͨ������ȷ��һ��������Բ�ϵĵ�,�����������޵ĵ���Ը��ݶԳ���ֱ�ӻ��

    #define ABSOLUTE(X) (X) > 0 ? (X) :(-(X))

    iS32 SquareR = 0;
    iS32 OldX = 0;
    iS32 OldY = 0;
    iS32 EndX = 0;
    iS32 EndY = 0;
    iS32 CenterX = X;
    iS32 CenterY = Y;
    iS32 Center2X = X+X;
    iS32 Center2Y = Y+Y;
    iS32 CurrentX = 0;
    iS32 CurrentY = 0;
    iS32 SymmetryPoints[3][2] ;
    iS32 SquareDifferential[3] = {0};

    iS32 DrawX = 0, DrawY = 0;   
    iS32 TempX1, TempY1;	
    iS32 TempX2, TempY2;	
    iS32 TempX3, TempY3;	
    iS32 TempX4, TempY4;	

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
    iS16 DrawRectEndX = ScreenWidth;
    iS16 DrawRectEndY = ScreenHeight;
    iU32 BuffPos = 0;
    iS32 Temp1 = 0;
	
 #if i51KITG2_DEBUG
    iLog( "G2DrawCircle start , %x, %x, %x, %x", X,Y,R,Color) ;
#endif
    if( X>2000||X<-1500 ||Y>2000||Y<-1500||R>500 )
    {
#if i51KITG2_DEBUG
iLog( "G2DrawCircle: %X,%X,%X", X,Y,R) ;
#endif
        return iFALSE;
    }

    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_CIRCLE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Circle.X = X;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Circle.Y = Y;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Circle.R = R;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Circle.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    if( i51UseVirtualPanel )
    {
        VPActualRect = &(i51CurrentVpRender->VPActualRect);
        VPDrawRect = &(i51CurrentVpRender->VPDrawRect);
  
        VP_TOP_LEFT_X = VPActualRect->X;
        VP_TOP_LEFT_Y = VPActualRect->Y;
        VP_LOVER_RIGHT_X = VP_TOP_LEFT_X + VPActualRect->Width;
        VP_LOVER_RIGHT_Y = VP_TOP_LEFT_Y + VPActualRect->Height;

        // ����Ļ�ϵĻ�������
        DrawRectStartX = VPDrawRect->X;
        DrawRectStartY = VPDrawRect->Y;
        DrawRectEndX = DrawRectStartX + VPDrawRect->Width;
        DrawRectEndY = DrawRectStartY + VPDrawRect->Height;

        if( DrawRectStartX < 0 ) DrawRectStartX = 0;
        if( DrawRectStartY < 0 ) DrawRectStartX = 0;
        if( DrawRectEndX >= ScreenWidth ) DrawRectEndX = ScreenWidth;
        if( DrawRectEndY >= ScreenHeight) DrawRectEndY = ScreenHeight;

        DrawX = X + VP_TOP_LEFT_X;
        DrawY = Y +VP_TOP_LEFT_Y;
    }
    else
    {
        DrawRectStartX = 0;
        DrawRectStartY = 0;
        DrawRectEndX = ScreenWidth;
        DrawRectEndY = ScreenHeight;

        DrawX = X;
        DrawY = Y;
    }

    SquareR = R * R;

    CurrentX = DrawX - R;
    CurrentY = DrawY;

    EndX = DrawX;
    EndY = DrawY - R;

    CenterX = DrawX;
    CenterY = DrawY;
    Center2X = DrawX+DrawX;
    Center2Y = DrawY+DrawY;

    while(1)
    {
        OldX = CurrentX;
        OldY = CurrentY;

        SymmetryPoints[0][0] = Center2X - CurrentX;
        SymmetryPoints[0][1] = CurrentY;
        SymmetryPoints[1][0] = SymmetryPoints[0][0];
        SymmetryPoints[1][1] = Center2Y - CurrentY;
        SymmetryPoints[2][0] = CurrentX;
        SymmetryPoints[2][1] = SymmetryPoints[1][1];

        TempX1 = CurrentX;
        TempY1 = CurrentY;
        TempX2 = SymmetryPoints[0][0];
        TempY2 = SymmetryPoints[0][1];
        TempX3 = SymmetryPoints[1][0];
        TempY3 = SymmetryPoints[1][1];
        TempX4 = SymmetryPoints[2][0];
        TempY4 = SymmetryPoints[2][1];
		
         if( TempX1>=DrawRectStartX && TempX1<DrawRectEndX && TempY1>=DrawRectStartY&& TempY1<DrawRectEndY )
        {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY1,1);
#endif
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY1 + TempX1)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY1 + TempX1;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
        }
	
         if( TempX2>=DrawRectStartX && TempX2<DrawRectEndX && TempY2>=DrawRectStartY&& TempY2<DrawRectEndY )
        {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY2 + TempX2,1);
#endif
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY2 + TempX2)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY2 + TempX2;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
        }
	
         if( TempX3>=DrawRectStartX && TempX3<DrawRectEndX && TempY3>=DrawRectStartY&& TempY3<DrawRectEndY )
        {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY3 + TempX3,1);
#endif
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY3 + TempX3)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY3 + TempX3;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
         }
	
         if( TempX4>=DrawRectStartX && TempX4<DrawRectEndX && TempY4>=DrawRectStartY&& TempY4<DrawRectEndY )
        {
#if LCDBUFF_DEBUG
i51KitG2CheckLcdbuffer(ScreenWidth*TempY4 + TempX4,1);
#endif
            if(AlphaSys==i51KITG2_ALPHA_NONEED)
            {
                LcdBuffer[ (ScreenWidth*TempY4 + TempX4)] = Color;
            }
            else
            {
                BuffPos = ScreenWidth*TempY4 + TempX4;
                LcdBuffer[BuffPos] = i51KitG2GetNewColor(LcdBuffer[BuffPos], Color);
            }
        }
	
        if( CurrentX==EndX && CurrentY==EndY )
        {
            break;
        }

        SquareDifferential[0] = ABSOLUTE( (CurrentX+1-CenterX)*(CurrentX+1-CenterX)\
                                                         +(CurrentY-CenterY)*(CurrentY-CenterY) - SquareR);
        SquareDifferential[1] = ABSOLUTE( (CurrentY - 1 -CenterY)*(CurrentY - 1 -CenterY)\
                                                         +(CurrentX-CenterX)*(CurrentX-CenterX) - SquareR);
        SquareDifferential[2] = ABSOLUTE( (CurrentY -1  -CenterY)*(CurrentY -1 -CenterY)\
                                                         +(CurrentX+1-CenterX)*(CurrentX+1-CenterX) - SquareR);
        if( SquareDifferential[0]<SquareDifferential[1] )
        {
            if( SquareDifferential[0]<SquareDifferential[2] )
            {
                CurrentX ++ ;
            }
            else
            {
                CurrentX ++ ;
                CurrentY -- ;
            }
        }
        else
        {
            if( SquareDifferential[1]<SquareDifferential[2] )
            {
                CurrentY -- ;
            }
            else
            {
                CurrentX ++ ;
                CurrentY -- ;
            }
        }

       }

    #undef ABSOLUTE

 #if i51KITG2_DEBUG
    iLog( "G2DrawCircle end , %x, %x, %x, %x", X,Y,R,Color) ;
#endif

    return iTRUE;
}


iKIT iBOOL i51KitG2FillPolygon( iS32 VerticeNumber, iS32 Point[][2], iCOLOR Color)
{ 	

    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 VerticeNumber : ����ζ������,������ڵ���3
    // iS32 Point[][2] :
    //    Point[0][0]����Ҫ������εĵ�һ������X����
    //    Point[0][1]����Ҫ������εĵ�һ������Y����
    //    Point[Num-1][0]����Ҫ������ε����һ������X����
    //    Point[Num-1][1]����Ҫ������ε����һ������X����
    // iCOLOR Color : ������ε���ɫ

    // notes : ���������������Num������,����������Point��ŵĶ����
    //    Point�����д�ŵĶ������������ģ������������߶�AB��
    //    �߶�BC���߶�CD���߶�DE���߶�EA������ɣ�
    //    ��Point[0][0]��Ŷ���A��X����,Point[0][1]��Ŷ���A��Y����
    //    ��Point[1][0]��Ŷ���B��X����,Point[1][1]��Ŷ���B��Y����
    //    ��Point[2][0]��Ŷ���C��X����,Point[2][1]��Ŷ���C��Y����
    //    ��Point[3][0]��Ŷ���D��X����,Point[3][1]��Ŷ���D��Y����
    //    ��Point[4][0]��Ŷ���E��X����,Point[4][1]��Ŷ���E��Y����

    #define MAX_POINTS 2048
    #define MAX_LINE_POINTS 50
    //��Ե���꼯
    iS16 * IntersectionCoordinates = iNULL;
    //һ���еı�Ե�����꼯
    iS16 IntersectionX[MAX_LINE_POINTS];
    //һ���еı�Ե���
    iU16 IntersectionNumberX = 0;
    //��ǰɨ�赽������
    iU16 CurrentLine = 0;
    //��ǰ��Ե��
    iU16 CurrentIntersection = 0;
    //��Ե�����
    iU16 IntersectionNum = 0;

    iS32 EndOldY = -1;
    iS32 EndOldX = -1;
    iS32 StartOldY = -1;
    iS32 StartOldX = -1;
    iS32 EndX = Point[1][0];
    iS32 EndY = Point[1][1];
    iS32 StartX = Point[0][0];
    iS32 StartY = Point[0][1];

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

    iU16 CurrDrawID;
//    iU16 DrawRectStartX = 0;
//    iU16 DrawRectStartY = 0;
//    iU16 DrawRectEndX = 0;
//    iU16 DrawRectEndY = 0;
    iU32 PonterPoints = 0;  
    iU32 CopyLength = 0; 
    iCOLOR * ColorBuf = iNULL;
    iRECT * VPActualRect;
    iS16 Temp1 = 0, Temp2 = 0;
	
    if( VerticeNumber < 3 ) return iFALSE;

 #if i51KITG2_DEBUG
    iLog( "G2FillPolygon start , %x, %x, %x", VerticeNumber,Point,Color) ;
#endif

    for( Temp1=0; Temp1<VerticeNumber; Temp1++ )
    {
        if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
         ||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
        )
        {
 #if i51KITG2_DEBUG
    iLog( "G2FillPolygon Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
#endif
            return iFALSE;
        }
    }
    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        CopyLength = sizeof(iS32) * 2*VerticeNumber;
        PonterPoints = ( iU32)(i51AdeOsMalloc( CopyLength,  __FILE__  , __LINE__ ));
        if( PonterPoints==0 ) return iFALSE;		
         i51AdeStdMemcpy16( (void *)(PonterPoints), (void *)(Point), CopyLength);  

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_FILLPOLYGON;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillPolygon.VerticeNumber = VerticeNumber;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillPolygon.PointerPoints = PonterPoints;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillPolygon.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

IntersectionCoordinates = (iS16*)(i51AdeOsMalloc(MAX_POINTS*4,__FILE__ , __LINE__ ));
    //ͳ�ƶ���α�Ե
    VPActualRect = &(i51CurrentVpRender->VPActualRect);
    for( CurrentLine=0; CurrentLine<VerticeNumber; CurrentLine++ )
    {
        if( i51UseVirtualPanel )
        {
            StartX = Point[CurrentLine][0] + VPActualRect->X;
            StartY = Point[CurrentLine][1] + VPActualRect->Y;
            EndX = Point[(CurrentLine+1)%VerticeNumber][0] + VPActualRect->X;
            EndY = Point[(CurrentLine+1)%VerticeNumber][1] + VPActualRect->Y;
        }
        else
        {
            StartX = Point[CurrentLine][0];
            StartY = Point[CurrentLine][1];
            EndX = Point[(CurrentLine+1)%VerticeNumber][0];
            EndY = Point[(CurrentLine+1)%VerticeNumber][1];
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
            if( CurrentIntersection>=MAX_POINTS-2 ) goto ERROR;
            //ͬһֱ��ͬһYֵֻȡһ��X
            if( (StartOldY!=StartY&&EndOldY!=StartY)||Location==0)
            {
                StartOldY = StartY;
                StartOldX = StartX;
                IntersectionCoordinates[CurrentIntersection<<1] = StartX;
                IntersectionCoordinates[(CurrentIntersection<<1)+1]= StartY;
                CurrentIntersection++;				
            }

            //ͬһֱ��ͬһYֵֻȡһ��X
            if( (EndOldY != EndY  && EndY!=StartY) || Location==0 )
            {
                EndOldY = EndY;
                EndOldX = EndX;
                IntersectionCoordinates[CurrentIntersection<<1] = EndX;
                IntersectionCoordinates[(CurrentIntersection<<1)+1]= EndY;
                CurrentIntersection++;				
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
    }

    if(ScreenHeight>ScreenWidth) Temp2 = ScreenHeight;
    else Temp2 = ScreenWidth;
    for( Temp1=0; Temp1<Temp2; Temp1++)
    {
        OneRowColorBuf[Temp1] = Color;
    }
    ColorBuf = OneRowColorBuf;
	
    IntersectionNum = CurrentIntersection;

    //��Ե���갴Y��������
   QuickSort2( (iS16(*)[2])IntersectionCoordinates , 0, CurrentIntersection-1);

    memset ( IntersectionX, 0 , MAX_LINE_POINTS*2 ) ;

    IntersectionNumberX = 0;
    StartY = IntersectionCoordinates[1];
    IntersectionX[0] = IntersectionCoordinates[0];
    for( CurrentIntersection=0; CurrentIntersection<IntersectionNum; CurrentIntersection++ )
    {
        if( StartY != IntersectionCoordinates[(CurrentIntersection<<1)+1] )
        {
            i51KitDrawDottedLine( StartY, IntersectionNumberX, IntersectionX, ColorBuf);
            IntersectionNumberX = 0;
            StartY = IntersectionCoordinates[(CurrentIntersection<<1)+1];
        }
        IntersectionX[IntersectionNumberX++] = IntersectionCoordinates[CurrentIntersection<<1];
        if( IntersectionNumberX>=MAX_LINE_POINTS) goto ERROR;
    }

    i51KitDrawDottedLine( StartY, IntersectionNumberX, IntersectionX, ColorBuf);

    i51AdeOsFree(IntersectionCoordinates);

 #if i51KITG2_DEBUG
    iLog( "G2FillPolygon end , %x, %x, %x", VerticeNumber,Point,Color) ;
#endif

    return iTRUE;
	
    ERROR:
       i51AdeOsFree(IntersectionCoordinates);
       IntersectionCoordinates = iNULL;
       return iFALSE;
    
}

iKIT iBOOL i51KitG2FillArc( iS32 Point[][2], iCOLOR Color )
{

    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iS32 Point[][2] :
    //    Point[0][0]����Ҫ��仡�ߵ����X����
    //    Point[0][1]����Ҫ��仡�ߵ����Y����
    //    Point[1][0]�Ǿ���������������X����
    //    Point[2][1]�Ǿ���������������Y����
    //    Point[2][0]����Ҫ��仡�ߵ��յ�X����
    //    Point[2][1]����Ҫ��仡�ߵ��յ�Y����
    // iCOLOR Color : ��仡�ߵ���ɫ

    // notes : �����(Point[0][0], Point[0][1])Ϊ���,(Point[2][0],Point[2][1])Ϊ�յ�,������(Point[1][0],Point[1][1])�Ļ���
    // ��֪ABC����,���ABC���ԲԲ�ĵĹ�ʽΪ��
    //    CenterX=(Qy+K1Px-K2Qx-Py)/(K2-K1)
    //    CenterY=(K1K2Qx+K2Py-K1Qy-K1K2Px)/(K2-K1)
    //    ����(Px, Py)���߶�AB�е�����, (Qx, Qy)���߶�BC���е�����, K1���߶�AB���д���б��, K2���߶�BC���д���б��

    //����X�ľ���ֵ
    #define ABSOLUTE(X) (X) > 0 ? (X) :(-(X))
    #define CLOCK_WISE    1
    #define ANTI_CLOCK_WISE 2

    iS32 OldX = 0;
    iS32 OldY = 0;
    iS32 CenterX = 0;
    iS32 CenterY = 0;
    iS32 SquareR = 0;
    iS32 CurrentX = 0;
    iS32 CurrentY = 0;
    iS32 Molecular = 0;
    iS32 Denominator = 0;
    iS16 RegulationY = 1;
    iS16 RegulationX = 1;
    iS32 Xa, Xb, Xc, Ya, Yb, Yc;
    iS32 SegmentEndpoints[2][2];
    iS32 PerpendicularIntersectionY = 0;
    iS32 SquareDifferential[3] = {0};
    iU8  AirDirection = CLOCK_WISE;

    // ��ǰ���Ƶ�Ԫ����VP�ϵ�ID
    iU16 CurrDrawID;
    iS32 Temp1 = 0;
	
 #if i51KITG2_DEBUG
    iLog( "G2FillArc start , %x, %x", Point,Color) ;
#endif

    for( Temp1=0; Temp1<3; Temp1++ )
    {
        if( Point[Temp1][0]>2000||Point[Temp1][0]<-1500
         ||Point[Temp1][1]>2000||Point[Temp1][1]<-1500		
        )
        {
 #if i51KITG2_DEBUG
    iLog( "G2FillArc Point[%X]:%X , %x", Temp1,Point[Temp1][0],Point[Temp1][1]) ;
#endif
            return iFALSE;
        }
    }     	

    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_FILLARC;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Points[0][0] = Point[0][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Points[0][1] = Point[0][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Points[1][0] = Point[1][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Points[1][1] = Point[1][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Points[2][0] = Point[2][0];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Points[2][1] = Point[2][1];
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillArc.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }


    Xa = Point[0][0];
    Ya = Point[0][1];
    Xb = Point[1][0];
    Yb = Point[1][1];
    Xc = Point[2][0];
    Yc = Point[2][1];
	

    //����ͬһֱ�����������ȷ��һ��Բ,����������������غ����޷�ȷ��Բ
    if( Xa==Xb&&Ya==Yb ) return iFALSE;
    if( Xa==Xc&&Ya==Yc ) return iFALSE;
    if( Xb==Xc&&Yb==Yc ) return iFALSE;

    //������ͬһֱ�����޷�ȷ��Բ
    if( Xa!=Xc && Xa!=Xb )
    {
        if( (((Ya-Yc)*100)/(Xa-Xc)) ==(((Ya-Yb)*100)/(Xa-Xb)) ) return iFALSE ;
    }
    if( Xa==Xb && Xa==Xc ) return iFALSE;
    if( Ya==Yb && Ya==Yc ) return iFALSE;

    //������ߵĶ���ΪԲ�������
    if( Xa > Xc )
    {
        Xa += Xc;
        Xc = Xa - Xc;
        Xa = Xa - Xc;
  
        Ya += Yc;
        Yc = Ya - Yc;
        Ya = Ya - Yc;
    }

    //����Բ��X����
    Molecular = (Yc-Ya)*(Yc-Yb)*(Yb-Ya) + ( Xa*Xa-Xb*Xb)*(Yc-Yb) + (Xc*Xc - Xb*Xb)*(Yb-Ya);
    Denominator = ( (Xb-Xc)*(Ya-Yb) + (Xb-Xa)*(Yb-Yc) ) << 1;
    CenterX = ((Molecular*10)/Denominator+5)/10;

    //����Բ��Y����
    Molecular = (Xb-Xa)*(Xc-Xb)*(Xc-Xa) + (Xb-Xc)*(Ya+Yb)*(Yb-Ya) + (Xb-Xa)*(Yb+Yc)*(Yc-Yb);
    Denominator = ( (Xb-Xc)*(Yb-Ya) + (Xb-Xa)*(Yc-Yb) ) << 1;
    CenterY = ((Molecular*10)/Denominator + 5 ) / 10;


    //����Բ�뾶��ƽ��
    SquareR = ( Xc - CenterX ) * ( Xc - CenterX ) + ( Yc - CenterY ) * ( Yc - CenterY );


    //���㾭����B����ֱ��AC��ֱ��ֱ����ֱ��AC�Ľ���X����
    //  Molecular = (Yb-Ya)*(Xc-Xa)*(Ya-Yc) - (Xc-Xa)*(Xc-Xa)*Xb - (Yc-Ya)*(Yc-Ya) * Xa;
    //  Denominator = (Yc-Ya)*(Ya-Yc) - (Xc-Xa)*(Xc-Xa);
    Molecular = (Yc-Ya)*(Ya*Yb-Yb*Yc+Xa*Xb-Xc*Xb) + (Xa-Xc)*(Ya*Xc-Yc*Xa);
    Denominator = (Yc-Ya)*(Ya-Yc) - (Xc-Xa)*(Xc-Xa);
    PerpendicularIntersectionY =  ((Molecular*10)/Denominator + 5 ) / 10;

    //�жϻ�Բ������,���B���ڴ����·���Բ������Ϊ��ʱ��,����Ϊ˳ʱ��
    if( Yb > PerpendicularIntersectionY )
    {
        AirDirection = ANTI_CLOCK_WISE;
    }
    else if( Yb==PerpendicularIntersectionY )
    {
        if(  Xb<CenterX )
        {
            AirDirection = ANTI_CLOCK_WISE;
        }
    }

    //�жϻ�Բ������Ϊ˳ʱ��ʱX,Y�ı仯����,���ݶԳ���,���Ϊ��ʱ���������Ƿ���
    if ( Ya < CenterY )
    {
        RegulationX = 1;
    }
    else if ( Ya > CenterY )
    {
        RegulationX = -1;
    }

    if ( Xa < CenterX )
    {
        RegulationY = -1;
    }
    else if ( Xa > CenterX )
    {
        RegulationY = 1;
    }

    if( AirDirection == ANTI_CLOCK_WISE )
    {
        RegulationX = -RegulationX;
        RegulationY = -RegulationY;
    }

    if( Ya==CenterY)
    {
        if( Xa > CenterX )
        {
            RegulationX = -1;
        }
        else
        {
            RegulationX = 1;
        }
    }

    if( Xa==CenterX )
    {
        if( Ya > CenterY )
        {
            RegulationY = -1;
        }
        else
        {
            RegulationY = 1;
       }
    }

    CurrentX = Xa;
    CurrentY = Ya;

    SegmentEndpoints[0][0] = Xa;
    SegmentEndpoints[0][1] = Ya;

    while(1)
    {
        if( CurrentX==Xc&&CurrentY==Yc) break;

  
        OldX = CurrentX;
        OldY = CurrentY;

        SquareDifferential[0] = ABSOLUTE( (CurrentX+RegulationX-CenterX)*(CurrentX+RegulationX-CenterX)\
                                                         +(CurrentY-CenterY)*(CurrentY-CenterY) - SquareR);
        SquareDifferential[1] = ABSOLUTE( (CurrentY +RegulationY -CenterY)*(CurrentY +RegulationY -CenterY)\
                                                         +(CurrentX-CenterX)*(CurrentX-CenterX) - SquareR);
        SquareDifferential[2] = ABSOLUTE( (CurrentY +RegulationY -CenterY)*(CurrentY +RegulationY -CenterY)\
                                                         +(CurrentX+RegulationX-CenterX)*(CurrentX+RegulationX-CenterX) - SquareR);
        if( SquareDifferential[0]<SquareDifferential[1] )
       {
            if( SquareDifferential[0]<SquareDifferential[2] )
            {
                CurrentX += RegulationX ;
            }
            else
            {
                CurrentX += RegulationX ;
                CurrentY += RegulationY ;
            }
        }
        else
        {
            if( SquareDifferential[1]<SquareDifferential[2] )
            {
                CurrentY += RegulationY ;
            }
            else
            {
                CurrentX += RegulationX ;
                CurrentY += RegulationY ;
            }
        }

        SegmentEndpoints[1][0] = CurrentX;
        SegmentEndpoints[1][1] = CurrentY;
        i51KitG2DrawLine( SegmentEndpoints, Color );

        if( CurrentX==CenterX && OldX!=CurrentX )
        {
            RegulationY = -RegulationY;
        }

        if( CurrentY==CenterY && OldY!=CurrentY )
        {
            RegulationX = -RegulationX;
        }
    }

    #undef ABSOLUTE
    #undef CLOCK_WISE
    #undef ANTI_CLOCK_WISE
 #if i51KITG2_DEBUG
    iLog( "G2FillArc end , %x, %x", Point,Color) ;
#endif

    return iTRUE;

}

iKIT iBOOL i51KitG2FillCircle( iS32 X, iS32 Y, iU32 R, iCOLOR Color )
{

    // author : Otto
    // since : Aug 15 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iS32 X : Բ��X����
    // iS32 Y : Բ��Y����
    // iU32 R : Բ�뾶
    // iCOLOR Color : ���Բ����ɫ

    // notes : �����(X, Y)ΪԲ��,�뾶ΪR��Բ
    // ����Բ�ĶԳ��ԣ������ҵ�Բ��һ�����ȷ����֮X��Գơ�Y��Գ��Ѿ����ĶԳƵ���������
    // ���Լ���ʱֻ��Ҫͨ������ȷ��һ��������Բ�ϵĵ�,�����������޵ĵ���Ը��ݶԳ���ֱ�ӻ��
    // ���㷨���õ��ǿռ������㷨

    #define ABSOLUTE(X) (X) > 0 ? (X) :(-(X))

    iS32 SquareR = 0;
    iS32 OldX = 0;
    iS32 OldY = 0;
    iS32 EndX = 0;
    iS32 EndY = 0;
    iS32 CenterX = X;
    iS32 CenterY = Y;
    iS32 Center2X = X+X;
    iS32 Center2Y = Y+Y;
    iS32 CurrentX = 0;
    iS32 CurrentY = 0;
    iS32 SymmetryPoints[3][2] ;
    iS32 SquareDifferential[3] = {0};
    iS32 Point[2][2];

    iS32 DrawX = 0, DrawY = 0;  
	
    // ��ǰ���Ƶ�Ԫ����VP�ϵ�ID
    iU16 CurrDrawID;
    // ������VPʵ�ʴ�С	
    iRECT VPActualRect;
    // ��Ҫ���Ƶ�VP����	
    iRECT VPDrawRect;

    iCOLOR * ColorBuf = iNULL;
    iS16 Temp1 = 0;//, Temp2 = 0;
 #if i51KITG2_DEBUG
    iLog( "G2FillCircle start , %x, %x, %x, %x",X,Y, R,Color) ;
#endif

    if( X>2000||X<-1500 ||Y>2000||Y<-1500||R>500 )
    {
#if i51KITG2_DEBUG
iLog( "G2FillCircle: %X,%X,%X", X,Y,R) ;
#endif
        return iFALSE;
    }


    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_FILLCRICLE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillCircle.X = X;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillCircle.Y = Y;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillCircle.R = R;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.FillCircle.Color= Color;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    if( i51UseVirtualPanel )
    {
        VPActualRect.X = i51CurrentVpRender->VPActualRect.X;
        VPActualRect.Y = i51CurrentVpRender->VPActualRect.Y;
        VPActualRect.Width= i51CurrentVpRender->VPActualRect.Width;
        VPActualRect.Height= i51CurrentVpRender->VPActualRect.Height;

        VPDrawRect.X = i51CurrentVpRender->VPDrawRect.X;
        VPDrawRect.Y = i51CurrentVpRender->VPDrawRect.Y;
        VPDrawRect.Width = i51CurrentVpRender->VPDrawRect.Width;
        VPDrawRect.Height= i51CurrentVpRender->VPDrawRect.Height;
  
        DrawX = X + VPActualRect.X;
        DrawY = Y + VPActualRect.Y;
    }
    else
    {
        VPActualRect.X = 0;
        VPActualRect.Y = 0;
        VPActualRect.Width = ScreenWidth;
        VPActualRect.Height = ScreenHeight;

        VPDrawRect.X = 0;
        VPDrawRect.Y = 0;
        VPDrawRect.Width = ScreenWidth;
        VPDrawRect.Height = ScreenHeight;
		
        DrawX = X;
        DrawY = Y;
    }

//    Temp2 = R + R ;
	if(Color!=OneRowColorBuf[0])
	{
	    for( Temp1=0; Temp1<ScreenWidth; Temp1++)
	    {
	        OneRowColorBuf[Temp1] = Color;
	    }
	}
    ColorBuf = OneRowColorBuf;

    SquareR = R * R;

    CurrentX = DrawX - R;
    CurrentY = DrawY;

    EndX = DrawX;
    EndY = DrawY - R;

    CenterX = DrawX;
    CenterY = DrawY;
    Center2X = DrawX+DrawX;
    Center2Y = DrawY+DrawY;

    //������X��ƽ�е�ֱ��
    Point[0][0] = CurrentX;
    Point[0][1] = CurrentY;
    Point[1][0] = CurrentX + R + R;
    Point[1][1] = CurrentY;
    i51KitDrawWaterLine(Point, VPDrawRect, VPActualRect, ColorBuf);

    while(1)
    {
        if( CurrentX==EndX && CurrentY==EndY )
        {
            break;
        }

        OldX = CurrentX;
        OldY = CurrentY;

        SquareDifferential[0] = ABSOLUTE( (CurrentX+1-CenterX)*(CurrentX+1-CenterX)\
                                                         +(CurrentY-CenterY)*(CurrentY-CenterY) - SquareR);
        SquareDifferential[1] = ABSOLUTE( (CurrentY - 1 -CenterY)*(CurrentY - 1 -CenterY)\
                                                         +(CurrentX-CenterX)*(CurrentX-CenterX) - SquareR);
        SquareDifferential[2] = ABSOLUTE( (CurrentY -1  -CenterY)*(CurrentY -1 -CenterY)\
                                                         +(CurrentX+1-CenterX)*(CurrentX+1-CenterX) - SquareR);
        if( SquareDifferential[0]<SquareDifferential[1] )
        {
            if( SquareDifferential[0]<SquareDifferential[2] )
            {
                CurrentX ++ ;
            }
            else
            {
                CurrentX ++ ;
                CurrentY -- ;
            }
        }
        else
        {
            if( SquareDifferential[1]<SquareDifferential[2] )
            {
                CurrentY -- ;
            }
            else
            {
                CurrentX ++ ;
                CurrentY -- ;
            }
        }

        SymmetryPoints[0][0] = Center2X - CurrentX;
        SymmetryPoints[0][1] = CurrentY;
        SymmetryPoints[1][0] = SymmetryPoints[0][0];
        SymmetryPoints[1][1] = Center2Y - CurrentY;
        SymmetryPoints[2][0] = CurrentX;
        SymmetryPoints[2][1] = SymmetryPoints[1][1];

        //����һ�����޵�ͼ��
        Point[0][0] = CurrentX;
        Point[0][1] = CurrentY;
        Point[1][0] = SymmetryPoints[0][0];
        Point[1][1] = CurrentY;
        i51KitDrawWaterLine(Point, VPDrawRect, VPActualRect, ColorBuf);


        //���ƶ������޵�ͼ��
        Point[0][0] = SymmetryPoints[2][0];
        Point[0][1] = SymmetryPoints[2][1];
        Point[1][0] = SymmetryPoints[1][0];
        Point[1][1] = SymmetryPoints[1][1];
        i51KitDrawWaterLine(Point, VPDrawRect, VPActualRect, ColorBuf);


    }

    #undef ABSOLUTE
 #if i51KITG2_DEBUG
    iLog( "G2FillCircle end , %x, %x, %x, %x",X,Y, R,Color) ;
#endif
    return iTRUE;
}

iKIT iBOOL i51KitG2CreateImageContainer ( iFILE File , iU32* ImageId , iU32 TotallImage , iU16* ContainerPathName, 
	void(*Process)( iU32 , iU32  , iHANDLE) , void (*Done)(iHANDLE) )
{

    // File		�ļ�ָ��
    // ImageId		ͼ����ԴID����
    // TotallImage		ͼ����ԴID����
    // ContainerPathName �����������ļ������ļ���
    // Process		�ص�����
    // Done		�ص�����

    // ���ͼƬID
    iU32 MaxImageID=0;
    iU32 Temp = 0;

 #if i51KITG2_DEBUG
    iLog( "G2CreateImageContainer start , %x, %x, %x, %x, %x, %x",
    File,ImageId, TotallImage,ContainerPathName, Process, Done) ;
#endif

    if( iNULL != TimeIdZlibDecodeImage) return iTRUE;

    if(iNULL==File||iNULL==ImageId||TotallImage==0||iNULL==ContainerPathName||iNULL==Process||iNULL==Done)
    {
        iLog("KIT - G2 : Creat container par");
        return iFALSE;
    }

    ManageZlibDecodeImage.DesFileName[0] = '\0';
    MMIAPICOM_Wstrcat( ManageZlibDecodeImage.DesFileName, ContainerPathName);
    ManageZlibDecodeImage.CompleteImage = 0;
    ManageZlibDecodeImage.Done = Done;
    ManageZlibDecodeImage.GoingImageID = ImageId[0];
    ManageZlibDecodeImage.ImageId = (iU32 )i51AdeOsMalloc( sizeof(iU32)*TotallImage,   __FILE__ , __LINE__ );
    if( ManageZlibDecodeImage.ImageId == 0) return iFALSE;
    i51AdeStdMemcpy16( (void *)(ManageZlibDecodeImage.ImageId), ImageId, sizeof(iU32)*TotallImage);
    ManageZlibDecodeImage.Process = Process;
    ManageZlibDecodeImage.SrcFile = File;
    ManageZlibDecodeImage.TotallImage = TotallImage;

    for( Temp = 0; Temp<TotallImage; Temp++ )
    {
        if( ImageId[Temp]>MaxImageID)
        {
            MaxImageID = ImageId[Temp];
        }
    }
    ManageZlibDecodeImage.MaxImageID = MaxImageID+1;

    // �������붨ʱ��	
    TimeIdZlibDecodeImage = i51AdeOsTimerCreate( 1,1);
     FlagDecodeImage = iTRUE;
     ZlibImageContainer = iNULL;
	 
    i51AdeOsTimerStart( TimeIdZlibDecodeImage, ZlibDecodeAllImage, 30);

  #if i51KITG2_DEBUG
    iLog( "G2CreateImageContainer end , %x, %x, %x, %x, %x, %x",
    File,ImageId, TotallImage,ContainerPathName, Process, Done) ;
#endif
   return iTRUE;
}
	
iKIT iBOOL i51KitG2ContainerImageIdExist( iHANDLE ContainerHandle, iU32 ImageId)
{
    // author : Otto
    // since : 2011.12.29
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : �ж�ָ����ID���������ļ����Ƿ��������ڵ�ͼƬ
    // ���ڷ���iTRUE�����򷵻�iFALSE

    // ͼƬ������ַ���
    i51ImageContainer * ImageContainer = iNULL;

    ImageContainer = (i51ImageContainer * )ContainerHandle;
	
    if( (iNULL==ImageContainer) || (ImageId>=ImageContainer->ImageMaxId) ) return iFALSE;

    if(ImageContainer->ImageIndexTable[ImageId].FlagImageExist ) return iTRUE;
    else return iFALSE;
	
}

iKIT iBOOL i51KitG2ContainerDeleteImage( iHANDLE ContainerHandle, iU32 ImageId)
{
    // author : Otto
    // since : 2011.12.29
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : ɾ�������ļ���ָ��ID�ŵ�ͼƬ�ļ�

    // ͼƬ������ַ���
    i51ImageContainer * ImageContainer = iNULL;
    iFILE FileHandle = iNULL;
    iU8 Data[5] = {0};	

    ImageContainer = (i51ImageContainer * )ContainerHandle;
    FileHandle = ImageContainer->FileHandle;
	
    if( (iNULL==ImageContainer) || (iNULL==FileHandle) ||(ImageId>=ImageContainer->ImageMaxId) ) return iFALSE;

    if(iTRUE== ImageContainer->ImageIndexTable[ImageId].FlagImageExist )
    {
        memset(Data,0,5);
        if( iFALSE==i51AdeOsFileSetSeek( FileHandle, I51KITG2_OFFSET_START_DATA+ImageId*5, i51_OS_FL_SEEK_HEAD) )
        {
            return iFALSE;
        }

        if( iFALSE==i51AdeOsFileWrite(FileHandle, Data, 5, iNULL))
        {
            return iFALSE;
        }

        ImageContainer->ImageIndexTable[ImageId].FlagImageExist = iFALSE;
    }

    return iTRUE;
	
}

iKIT iBOOL i51KitG2UpdataContainerFromImage ( iHANDLE ContainerHandle , void * ImageMess, iU8 MessType, iU32 DesImageId)
{
    // author : Otto
    // since : 2011.12.29
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : ���������ļ��е�ͼƬ


    // ͼƬ������ַ���
    i51ImageContainer * ImageContainer = iNULL;

    iFILE SrcFile = iNULL;
    iFILE DesFile = iNULL;
    iU32 DesSeek = 0;
    iU32 SrcSeek = 0;
    iU8 FlagReturn = iFALSE;

    if(iNULL==(iU32)ImageMess) return iFALSE;

    ImageContainer = (i51ImageContainer * )ContainerHandle;
	
    if( (iNULL==ImageContainer) || (DesImageId>=ImageContainer->ImageMaxId) ) return iFALSE;

    DesFile = ImageContainer->FileHandle;
	
    // �ƶ��ļ���ָ�뵽�����ļ�ĩβ
    FlagReturn = i51AdeOsFileSetSeek( DesFile, 0, i51_OS_FL_SEEK_TAIL) ;
    if( iFALSE == FlagReturn ) goto ERROR;
	
    // ��õ�ǰ�����ļ���ָ��λ��
    FlagReturn = i51AdeOsFileGetSeek( DesFile , (iS32 *)(&DesSeek), i51_OS_FL_SEEK_HEAD ) ;
    if( iFALSE == FlagReturn ) goto ERROR;

    if( MessType==i51KitG2_MESSAGE_TYPE_FILE )
    {
        SrcFile = (iFILE)(ImageMess);
        SrcSeek =  0;

        if( iFALSE == ZlibDecodeOneImageFromFile( SrcFile , SrcSeek, DesFile, DesSeek, ContainerHandle, DesImageId ) );
        {
		goto ERROR;
        }
    }
    else
    {
        if( iFALSE==ZlibDecodeOneImageFromMemory(ImageMess, DesFile, DesSeek, ContainerHandle, DesImageId) )
        {
            goto ERROR;
        }
    }

    return iTRUE;

ERROR:
    iLog("KIT - G2 : updata C I");
    return iFALSE;
	
}

iKIT iBOOL i51KitG2UpdataContainerFromRe ( iHANDLE ContainerHandle , void * ReMess, iU32 SrcImageId , iU8 MessType, iU32 DesImageId)
{
    // author : Otto
    // since : 2011.12.29
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : ���������ļ��е�ͼƬ

    // ͼƬ������ַ���
    i51ImageContainer * ImageContainer = iNULL;


    iFILE SrcFile = iNULL;
    iFILE DesFile = iNULL;
    iU32 DesSeek = 0;
    iU32 SrcSeek = 0;
    iU8 FlagReturn = iFALSE;
    iU32 CurrDataPos = 0;
    iU8 * SrcData = iNULL;	
    iU32 TextCount = 0 ;
    iU32 BinCount = 0 ;
	
    if(iNULL==(iU32)ReMess) return iFALSE;

    ImageContainer = (i51ImageContainer * )ContainerHandle;

    if( (iNULL==ImageContainer) || (DesImageId>=ImageContainer->ImageMaxId) ) return iFALSE;

    DesFile = ImageContainer->FileHandle;

    // �ƶ��ļ���ָ�뵽�����ļ�ĩβ
    FlagReturn = i51AdeOsFileSetSeek( DesFile, 0, i51_OS_FL_SEEK_TAIL) ;
    if( iFALSE == FlagReturn ) goto ERROR;
	
    // ��õ�ǰ�����ļ���ָ��λ��
    FlagReturn = i51AdeOsFileGetSeek( DesFile , (iS32 *)(&DesSeek), i51_OS_FL_SEEK_HEAD ) ;
    if( iFALSE == FlagReturn ) goto ERROR;

    if( MessType==i51KitG2_MESSAGE_TYPE_FILE )
    {
        SrcFile = (iFILE)(ReMess);
        SrcSeek =  i51KitStdResGetContentEntry(SrcFile,SrcImageId);
		
        if( iFALSE == ZlibDecodeOneImageFromFile( SrcFile , SrcSeek, DesFile, DesSeek, ContainerHandle, DesImageId ) )
        {
            goto ERROR;
        }
		
    }
    else
    {
        CurrDataPos = 14;
        
        SrcData= (iU8*)((iU8*)ReMess+CurrDataPos);
       
        TextCount = SrcData[0] | (SrcData[1]<<8)| (SrcData[2]<<16)| (SrcData[3]<<24);
        BinCount = SrcData[4] | (SrcData[5]<<8)| (SrcData[6]<<16)| (SrcData[7]<<24);
        
        if ( SrcImageId+ 1 > BinCount  )  goto ERROR;

        CurrDataPos = 22 + 40 * (TextCount+ SrcImageId )+ 32;

        SrcData= (iU8*)((iU8*)ReMess+CurrDataPos);
        CurrDataPos= SrcData[0] | (SrcData[1]<<8)| (SrcData[2]<<16)| (SrcData[3]<<24);

        SrcData= (iU8*)((iU8*)ReMess+CurrDataPos);
        if( iFALSE==ZlibDecodeOneImageFromMemory(SrcData, DesFile, DesSeek, ContainerHandle, DesImageId) )
        {
            goto ERROR;
        }
    }
	

    return iTRUE;
	
    ERROR:
        iLog("KIT - G2 : updata C R");
        return iFALSE;
    
}

iKIT iBOOL i51KitG2GetContainerAttr(iHANDLE ContainerHandle, iU32 * TotalCapacity, iU32 * UsedCapacity, iU32 * ResidueCapacity, iU32 * UsableMinID )
{
    // author : Otto
    // since : 2011.12.29
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : �����������

    // ͼƬ������ַ���
    i51ImageContainer * ImageContainer = iNULL;
    iU32 TotalCapacityNum = 0;
    iU32 UsedCapacityNum = 0;
    iU32 ResidueCapacityNum = 0;
    iU32 UsableMinIDNum = 0;
    iU32 Temp = 0, UsedTemp = 0, ResidueTemp = 0 ;	
    i51ImageIndex * ImageIndex = iNULL;
	
    ImageContainer = (i51ImageContainer * )ContainerHandle;
	
    if( (iNULL==ImageContainer)) return iFALSE;

    ImageIndex = (i51ImageIndex *)ImageContainer->ImageIndexTable;
    if( (iNULL==ImageIndex)) return iFALSE;
	
    TotalCapacityNum = ImageContainer->ImageMaxId;
    UsedCapacityNum = ImageContainer->ImageTotalNumber;
    ResidueCapacityNum = TotalCapacityNum - UsedCapacityNum;

    for( Temp=0; Temp<TotalCapacityNum; Temp++ )
    {
        if(ImageIndex[Temp].FlagImageExist ) 
        {
            UsedTemp ++;
        }
        else
        {
            if(ResidueTemp==0)
            {
                UsableMinIDNum = Temp;
            }
            ResidueTemp++;
        }
    }

    if( (UsedCapacityNum!=UsedTemp) || (ResidueCapacityNum!=ResidueTemp) ) return iFALSE;

    if(TotalCapacity) *TotalCapacity = TotalCapacityNum;
    if(UsedCapacity) *UsedCapacity = UsedCapacityNum;
    if(ResidueCapacity) *ResidueCapacity = ResidueCapacityNum;
    if(UsableMinID&&ResidueCapacityNum) *UsableMinID = UsableMinIDNum;

    return iTRUE;
	 
}

iKIT iBOOL i51KitG2CloseImageContainer ( iHANDLE Handle)
{

    // author : Otto
    // since : Aug 20 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE ImageContainer �������ļ�ָ��

    // notes : �ͷ������ļ�ռ����Դ���ر�����

    // ͼƬ������ַ���
    i51ImageContainer * CloseImageContainer = iNULL;
    // �ļ�������������ֵ
    iU8   FlagReturn = iFALSE;
    // ͼƬ�������ַ
    i51ImageIndex * ImageIndexTable = iNULL;
    iU32 AllImageNum = 0;
    iU32 Temp;

 #if i51KITG2_DEBUG
    iLog( "G2CloseImageContainer start , %x",Handle);
#endif

    CloseImageContainer = (i51ImageContainer*)Handle;

    if( CloseImageContainer==iNULL ) return iFALSE;

    Temp = 0;
    while(1)
    {
        if( CloseImageContainer==ContainerManage[Temp].ContainerHandle && ContainerManage[Temp].Flag )
        {
//iLog("close ContainerManage[%d].ContainerHandle:%d",Temp,ContainerManage[Temp].ContainerHandle);
    	     ContainerManage[Temp].Flag = iFALSE;
            ContainerManage[Temp].ContainerHandle = iNULL;
            if ( ContainerManage[Temp].ImageID )
            {
                i51AdeOsFree( ContainerManage[Temp].ImageID );
                ContainerManage[Temp].ImageID = iNULL;
            }

            if ( ContainerManage[Temp].MemCopyCount)
            {
                i51AdeOsFree( ContainerManage[Temp].MemCopyCount);
                ContainerManage[Temp].MemCopyCount= iNULL;

            }

            break;
        }
        Temp++;
        if( Temp>=MAX_ContainerManage_NUM )		
        {
            iLog("KIT - G2 : close handle err:%X",Handle);
            return iFALSE;			
        }
    }
	
    ImageIndexTable = CloseImageContainer->ImageIndexTable;
    if( iNULL==ImageIndexTable ) return iFALSE;
	
    AllImageNum = 	CloseImageContainer->ImageMaxId;

    for( Temp=0; Temp<AllImageNum; Temp++ )
    {
        if( ImageIndexTable[Temp].FlagImageExist==0 ) continue;
        if( ImageIndexTable[Temp].FlagCopyRAM )
        {
            FlagReturn = i51AdeOsFree( (void *)(ImageIndexTable[Temp].MemoryEntry));
            ImageIndexTable[Temp].MemoryEntry = iNULL;
            ImageIndexTable[Temp].FlagCopyRAM = iFALSE;
            if( iFALSE == FlagReturn ) return iFALSE;
        }
    }
	
    if( CloseImageContainer->ImageIndexTable!=iNULL )
    {
        FlagReturn = i51AdeOsFree( CloseImageContainer->ImageIndexTable );
        CloseImageContainer->ImageIndexTable = iNULL;
        if( iFALSE == FlagReturn ) goto ERROR;;
    }

    if( CloseImageContainer->FileHandle )
    {
        FlagReturn = i51AdeOsFileClose( CloseImageContainer->FileHandle );
        if( iFALSE == FlagReturn )  goto ERROR;
        CloseImageContainer->FileHandle = iNULL;
    }

    i51AdeOsFree(CloseImageContainer);
    CloseImageContainer = iNULL;

	if(FlagDecodeImage && Handle==(iHANDLE)(ZlibImageContainer) )
	{
           if( i51AdeOsFileIsExist(ManageZlibDecodeImage.DesFileName) )
           {
               i51AdeOsFileDelete(ManageZlibDecodeImage.DesFileName);
           }

	    if( ManageZlibDecodeImage.ImageId )
	    {
	    	i51AdeOsFree((void*)ManageZlibDecodeImage.ImageId );
		ManageZlibDecodeImage.ImageId  = iNULL;
	    }
   			
	}
 #if i51KITG2_DEBUG
    iLog( "G2CloseImageContainer end , %x",Handle);
#endif
    return iTRUE;

ERROR:
    iLog("KIT - G2 : close container");
    return iFALSE;
}

iKIT iBOOL i51KitG2CheckImageContainer ( iU16* ContainerPathName )
{
    // author : Otto
    // since : Aug 20 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iU16* path �������ļ�����·��ָ��
    //  iU16 * Name : �����ļ���

    // notes : �ж������ļ��Ƿ�Ϸ�

    // �򿪵�i51IMIC�ļ�ָ��
    iFILE Fp_i51IMIC = iNULL;
    // i51IMIC�����ļ�·�����ļ���
    iU16 Name_i51IMIC[50] = { 0 } ;

 #if i51KITG2_DEBUG
    iLog( "G2CheckImageContainer start , %x",ContainerPathName);
#endif

	
    MMIAPICOM_Wstrcat( Name_i51IMIC, ContainerPathName);

    // �����Ҫ�򿪵������ļ��Ƿ����
    if( !i51AdeOsFileIsExist(Name_i51IMIC) )   return iFALSE;

    // ���Ѿ����ڵ������ļ�
    Fp_i51IMIC =  i51AdeOsFileOpen( Name_i51IMIC , i51_OS_FL_READ | i51_OS_FL_EXISTING_OPEN, __FILE__ , __LINE__ );
    if( Fp_i51IMIC==0 )
    {
        iLog("KIT - G2 : open check container");
        return iFALSE;
    }

    if( !i51KitG2CheckFileCRC(Fp_i51IMIC)) 
    {
        i51AdeOsFileClose(Fp_i51IMIC);
        return iFALSE;
    }

    if( !i51AdeOsFileClose(Fp_i51IMIC)) return iFALSE;

 #if i51KITG2_DEBUG
    iLog( "G2CheckImageContainer end , %x",ContainerPathName);
#endif
    return iTRUE;

}

static iHANDLE i51KitG2CreatEmptyImageContainer( iU16* ContainerPathName)
{
    // author : Otto
    // since : Aug 20 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iU16* path �������ļ�����·��ָ��
    //  iU16 * Name : �����ļ���

    // notes : ��ͼ�������ļ�
    // �ɹ��򿪷��ظ������ļ���ָ��,���򷵻�iNULL

    // �򿪵�i51IMIC�ļ�ָ��
    iFILE Fp_i51IMIC = iNULL;
    // ͼƬ�������ַ
    i51ImageIndex * ImageIndexTable = iNULL;
    // ͼƬ������ַ���
    i51ImageContainer * ImageContainerEntry = iNULL;
    // �ļ�������������ֵ
    iU8   FlagReturn = iFALSE;
    // ����ļ���ʽ����ʱ����
    iU8   FlagFile[4] = {'i','5','1','C'};
    // ���ļ��ж�ȡ������ʵ�ʳ���
//    iU32 LengthReadData = 0;
//    iU32 HeadDataPos = 0;	
//    iU32 iTemp = 0;
    iU32 CRC = 1;
    iU8 ContainerVer = I51KITG2_IMAGE_CONTAINER_VERSION;
    iU32 FileVer = 0;
//    iU8   DataModle = 0;

    // �����Ҫ�򿪵������ļ��Ƿ����
    if( i51AdeOsFileIsExist(ContainerPathName) ) 
    {
        return iFALSE;
    }

    // ���Ѿ����ڵ������ļ�
    Fp_i51IMIC =  i51AdeOsFileOpen( ContainerPathName, i51_OS_FL_WRITE| i51_OS_FL_ALWAYS_CREATE, __FILE__ , __LINE__ );
    if( Fp_i51IMIC==0 ) goto ERROR;

    // дCRC
    FlagReturn = i51AdeOsFileWriteInt32( Fp_i51IMIC, &(CRC) );
    if( (iFALSE == FlagReturn))    goto ERROR;

    // д���ļ���ʾ��
    FlagReturn = i51AdeOsFileWrite( Fp_i51IMIC, FlagFile, 4, iNULL);   
    if( (iFALSE == FlagReturn))    goto ERROR;
	
    // �����ڴ�������������ʼ������  
    ImageContainerEntry = ( i51ImageContainer * )i51AdeOsMalloc( sizeof(i51ImageContainer),   __FILE__ , __LINE__ );
    if( iFALSE == ImageContainerEntry ) goto ERROR;

    ImageContainerEntry->FileHandle = Fp_i51IMIC;
    ImageContainerEntry->ImageIndexTable = iNULL;
    ImageContainerEntry->ImageTotalNumber = 0;
    ImageContainerEntry->ImageMaxId = 256;

    FlagReturn = i51AdeOsFileWriteInt8( Fp_i51IMIC, &ContainerVer );
    if( iFALSE == FlagReturn) goto ERROR;

    FlagReturn = i51AdeOsFileWriteInt32( Fp_i51IMIC, &FileVer );
    if( iFALSE == FlagReturn  ) goto ERROR;

    FlagReturn = i51AdeOsFileWriteInt16( Fp_i51IMIC, &(ImageContainerEntry->ImageTotalNumber) );
    if( iFALSE == FlagReturn ) goto ERROR;

    FlagReturn = i51AdeOsFileWriteInt16( Fp_i51IMIC, &(ImageContainerEntry->ImageMaxId) );
    if( iFALSE == FlagReturn ) goto ERROR;

    // ���������������ڴ�ռ�
    ImageIndexTable = ( i51ImageIndex * )i51AdeOsMalloc( (sizeof(i51ImageIndex))*(ImageContainerEntry->ImageMaxId), __FILE__ , __LINE__ );
    if( iFALSE == ImageIndexTable ) goto ERROR;

    memset(ImageIndexTable,0,((sizeof(i51ImageIndex))*(ImageContainerEntry->ImageMaxId)));

    FlagReturn = i51AdeOsFileWrite( Fp_i51IMIC, ImageIndexTable, (sizeof(i51ImageIndex))*(ImageContainerEntry->ImageMaxId), 0);
    if( iFALSE == FlagReturn ) goto ERROR;

    ImageContainerEntry->ImageIndexTable = ImageIndexTable;

    i51AdeOsFileFlush(Fp_i51IMIC);
	
    return ImageContainerEntry;
	
    ERROR:

        i51AdeOsFileClose( Fp_i51IMIC );
        i51AdeOsFree(ImageContainerEntry);
        ImageContainerEntry = iNULL;
        i51AdeOsFree(ImageIndexTable);
        ImageIndexTable = iNULL;
        iLog("KIT - G2 : RW empty contain");
        return iFALSE;

}

iKIT iHANDLE i51KitG2OpenImageContainer ( iU16* ContainerPathName)
{
    // author : Otto
    // since : Aug 20 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iU16* path �������ļ�����·��ָ��
    //  iU16 * Name : �����ļ���

    // notes : ��ͼ�������ļ�
    // �ɹ��򿪷��ظ������ļ���ָ��,���򷵻�iNULL

    // �򿪵�i51IMIC�ļ�ָ��
    iFILE Fp_i51IMIC = iNULL;
    // i51IMIC�����ļ�·�����ļ���
    iU16 Name_i51IMIC[50] = { 0 } ;
    // ͼƬ�������ַ
    i51ImageIndex * ImageIndexTable = iNULL;
    // ͼƬ������ַ���
    i51ImageContainer * ImageContainerEntry = iNULL;
    // �ļ�������������ֵ
    iU8   FlagReturn = iFALSE;
    // ����ļ���ʽ����ʱ����
    iU8   FlagFile[4] = {0};
    // �ļ�ͷ��ǰƫ��λ��
//    iU32 FileHeadOffset = 0;
    // ��ǰͼƬ�ļ������ݳ���
//    iU32 CurrentDataLength = 0;
    // ���ļ��ж�ȡ������ʵ�ʳ���
    iU32 LengthReadData = 0;
    iU32 HeadDataPos = 0;	
    iU32 iTemp = 0;
    iU32 CRC = 0;
    iU8 ContainerVer = 0;
    iU32 FileVer = 0;
//    iU8   DataModle = 0;
    iU8 * HeadDataBuff = iNULL;
    iU8 * ImageDataBuff = iNULL;

 #if i51KITG2_DEBUG
    iLog( "G2OpenImageContainer start , %x",ContainerPathName);
#endif
	
    MMIAPICOM_Wstrcat( Name_i51IMIC, ContainerPathName);
    // �����Ҫ�򿪵������ļ��Ƿ����
    if( !i51AdeOsFileIsExist(Name_i51IMIC) ) 
    {
        ImageContainerEntry = i51KitG2CreatEmptyImageContainer(Name_i51IMIC);
        if( ImageContainerEntry )
        {
            iTemp = 0;	 
            while(1)
            {
                if( iFALSE==ContainerManage[iTemp].Flag )
                {
                    ContainerManage[iTemp].Flag = iTRUE;
                    ContainerManage[iTemp].ContainerHandle = ImageContainerEntry;
                    break;
                }
                iTemp++;
                if( iTemp>=MAX_ContainerManage_NUM )		
                {
                    iLog("KIT - G2 : contain full");
                    i51KitG2CloseImageContainer((iHANDLE)(ImageContainerEntry));
                    return iFALSE;
                }
            }
            return ImageContainerEntry;
        }
        else return iFALSE;
    }

    // ���Ѿ����ڵ������ļ�
    Fp_i51IMIC =  i51AdeOsFileOpen( Name_i51IMIC , i51_OS_FL_WRITE| i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ );
    if( Fp_i51IMIC==0 ) goto ERROR;

    // ��CRC
    FlagReturn = i51AdeOsFileReadInt32( Fp_i51IMIC, &(CRC) );
    if( (iFALSE == FlagReturn))    goto ERROR;

    // ���򿪵����������ŵ��ļ��Ƿ�Ϊ"i51G"����"i51C"��ʽ
    FlagReturn = i51AdeOsFileRead( Fp_i51IMIC, FlagFile, 4, &LengthReadData);   
    if( (iFALSE == FlagReturn) || (LengthReadData!=4) || (FlagFile[0]!='i')
	|| (FlagFile[1]!='5') || (FlagFile[2]!='1') ||(FlagFile[3]!='G'&&FlagFile[3]!='C') )
    goto ERROR;
	
    // �����ڴ�������������ʼ������  
    ImageContainerEntry = ( i51ImageContainer * )i51AdeOsMalloc( sizeof(i51ImageContainer),   __FILE__ , __LINE__ );
    if( iFALSE == ImageContainerEntry ) goto ERROR;

    ImageContainerEntry->FileHandle = Fp_i51IMIC;
    ImageContainerEntry->ImageIndexTable = iNULL;
    ImageContainerEntry->ImageTotalNumber = 0;
    ImageContainerEntry->ImageMaxId = 0;

    FlagReturn = i51AdeOsFileReadInt8( Fp_i51IMIC, &ContainerVer );
    if( iFALSE == FlagReturn && ContainerVer <= I51KITG2_IMAGE_CONTAINER_VERSION ) goto ERROR;

    FlagReturn = i51AdeOsFileReadInt32( Fp_i51IMIC, &FileVer );
    if( iFALSE == FlagReturn  ) goto ERROR;

    FlagReturn = i51AdeOsFileReadInt16( Fp_i51IMIC, &(ImageContainerEntry->ImageTotalNumber) );
    if( iFALSE == FlagReturn ) goto ERROR;

    FlagReturn = i51AdeOsFileReadInt16( Fp_i51IMIC, &(ImageContainerEntry->ImageMaxId) );
    if( iFALSE == FlagReturn ) goto ERROR;

    // ���������������ڴ�ռ�
    ImageIndexTable = ( i51ImageIndex * )i51AdeOsMalloc( (sizeof(i51ImageIndex))*(ImageContainerEntry->ImageMaxId), __FILE__ , __LINE__ );
    if( iFALSE == ImageIndexTable ) goto ERROR;

    HeadDataBuff = (iU8*)i51AdeOsMalloc((ImageContainerEntry->ImageMaxId*5),  __FILE__ , __LINE__ );
    if( iFALSE == HeadDataBuff) goto ERROR;

    FlagReturn = i51AdeOsFileRead( Fp_i51IMIC, HeadDataBuff, (ImageContainerEntry->ImageMaxId*5), iNULL);
    if( iFALSE == FlagReturn ) goto ERROR;

    ImageDataBuff = (iU8*)i51AdeOsMalloc(I51KITG2_OFFSET_PIXELDATA,  __FILE__ , __LINE__ );
    if( iFALSE == ImageDataBuff) goto ERROR;

    // ��ʼ������������
    HeadDataPos = 0;
    for( iTemp=0; iTemp<ImageContainerEntry->ImageMaxId; iTemp++ )
    {
        ImageIndexTable[iTemp].FlagCopyRAM = iFALSE;

        ImageIndexTable[iTemp].FlagImageExist = HeadDataBuff[HeadDataPos];

        if(ImageIndexTable[iTemp].FlagImageExist==0)
        {
            HeadDataPos += 5;
            continue;
        }

        ImageIndexTable[iTemp].ImageEntry = (HeadDataBuff[HeadDataPos+1])|(HeadDataBuff[HeadDataPos+2]<<8)
			|(HeadDataBuff[HeadDataPos+3]<<16)|(HeadDataBuff[HeadDataPos+4]<<24);

        FlagReturn = i51AdeOsFileSetSeek( Fp_i51IMIC, (ImageIndexTable[iTemp].ImageEntry), i51_OS_FL_SEEK_HEAD );
        if( iFALSE == FlagReturn ) goto ERROR;

        FlagReturn = i51AdeOsFileRead( Fp_i51IMIC, ImageDataBuff, I51KITG2_OFFSET_PIXELDATA, iNULL);
        if( iFALSE == FlagReturn ) goto ERROR;

//        ImageIndexTable[iTemp].LengthIamgeData= 
//			(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH]) |(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH+1]<<8)\
//			|(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH+2]<<16)|(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH+3]<<24); 
        ImageIndexTable[iTemp].ImageWidth = (ImageDataBuff[I51KITG2_OFFSET_IMAGEWIDTH]) |(ImageDataBuff[I51KITG2_OFFSET_IMAGEWIDTH+1]<<8); 
        ImageIndexTable[iTemp].ImageHeight= (ImageDataBuff[I51KITG2_OFFSET_IMAGEHEIGHT]) |(ImageDataBuff[I51KITG2_OFFSET_IMAGEHEIGHT+1]<<8); 
        ImageIndexTable[iTemp].DecodeType= (ImageDataBuff[I51KITG2_OFFSET_DECODETYPE]); 
        ImageIndexTable[iTemp].AlphaPixelNum= (ImageDataBuff[I51KITG2_OFFSET_ALPHANUMER])|(ImageDataBuff[I51KITG2_OFFSET_ALPHANUMER+1]<<8);
        ImageIndexTable[iTemp].LengthIamgeData =
        	(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH])|(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH+1]<<8)
           |(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH+2]<<16)|(ImageDataBuff[I51KITG2_OFFSET_DATALENGTH+3]<<24);
        ImageIndexTable[iTemp].LengthTotalData = ImageIndexTable[iTemp].LengthIamgeData ;
        if(ImageIndexTable[iTemp].AlphaPixelNum>0)
        {
            ImageIndexTable[iTemp].LengthTotalData+=ImageIndexTable[iTemp].AlphaPixelNum*8 + 12;
        }
//        DataModle= (ImageDataBuff[I51KITG2_OFFSET_DATAMODEL]); 
//        if( DataModle!=i51AdeCnfGetEndian() ) goto ERROR;
        HeadDataPos += 5;
		
  }

    ImageContainerEntry->ImageIndexTable = ImageIndexTable;

//    CurrentImageContainer.FileHandle  = ImageContainerEntry->FileHandle;
//    CurrentImageContainer.ImageIndexTable = ImageContainerEntry->ImageIndexTable;
//    CurrentImageContainer.ImageMaxId = 0;
//    CurrentImageContainer.ImageTotalNumber = ImageContainerEntry->ImageTotalNumber;

 #if i51KITG2_DEBUG
    iLog( "G2OpenImageContainer end , %x",ContainerPathName);
#endif

    i51AdeOsFree(HeadDataBuff);
    i51AdeOsFree(ImageDataBuff);
    HeadDataBuff = iNULL;
    ImageDataBuff = iNULL;

    iTemp = 0;	 
    while(1)
    {
        if( iFALSE==ContainerManage[iTemp].Flag )
        {
    	     ContainerManage[iTemp].Flag = iTRUE;
            ContainerManage[iTemp].ContainerHandle = ImageContainerEntry;
//iLog("open ContainerManage[%d].ContainerHandle:%d",iTemp,ContainerManage[iTemp].ContainerHandle);
            break;
        }
        iTemp++;
        if( iTemp>=MAX_ContainerManage_NUM )		
        {
            iLog("KIT - G2 : contain full");
            goto ERROR;			
        }
    }

    return ImageContainerEntry;
	
    ERROR:

        i51AdeOsFree(HeadDataBuff);
        i51AdeOsFree(ImageDataBuff);
        i51AdeOsFree(ImageIndexTable);
        i51AdeOsFree(ImageContainerEntry);
        HeadDataBuff = iNULL;
        ImageDataBuff = iNULL;
        ImageIndexTable = iNULL;
        ImageContainerEntry = iNULL;
		
//        i51KitG2CloseImageContainer((iHANDLE)(ZlibImageContainer));
//		ZlibImageContainer = iNULL;

        i51AdeOsFileClose( Fp_i51IMIC );
        Fp_i51IMIC = 0;

        iLog("KIT - G2 : RW open contain");
        return iFALSE;

}

iKIT iBOOL i51KitG2DestroyImageContainer ( iHANDLE ImageContainer, iU16* ContainerPathName )
{

    // author : Otto
    // since : Aug 20 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE ImageContainer �������ļ�ָ��

    // notes : �ͷ������ļ�ռ����Դ���ر�����
    //            ɾ��·��Path�е������ļ�,����ļ�����
  
    // i51IMIC�����ļ�·�����ļ���
    iU16 Name_i51IMIC[50] = { 0 } ;
    // �ļ�������������ֵ
    iU8   FlagReturn = iFALSE;

 #if i51KITG2_DEBUG
    iLog( "G2DestroyImageContainer start , %x,%x",ImageContainer,ContainerPathName);
#endif
    FlagReturn = i51KitG2CloseImageContainer(ImageContainer);
    if( iFALSE == FlagReturn )  return iFALSE;

    MMIAPICOM_Wstrcat( Name_i51IMIC, ContainerPathName);

    if( i51AdeOsFileIsExist(Name_i51IMIC) )
    {
        FlagReturn = i51AdeOsFileDelete(Name_i51IMIC);
        if( iFALSE == FlagReturn )  return iFALSE;
    }

 #if i51KITG2_DEBUG
    iLog( "G2DestroyImageContainer end , %x,%x",ImageContainer,ContainerPathName);
#endif
    return iTRUE;

}

iKIT iBOOL i51KitG2SetContainerContentVersion( iHANDLE Handle, iU32 Version)
{
    // author : Otto
    // since : 2011.10.10
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE handle: �������
    //  iU32 Version: �������ݰ汾��
    // notes : �����������ݰ汾��

    iU8  Flag = iFALSE;
    iFILE File = ((i51ImageContainer * )Handle)->FileHandle;
 #if i51KITG2_DEBUG
    iLog( "G2SetContainerContentVersion start , %x,%x",Handle,Version);
#endif

    Flag = i51AdeOsFileSetSeek( File, I51KITG2_OFFSET_FILE_VERSION, i51_OS_FL_SEEK_HEAD );
    if( iNULL==Flag ) goto ERROR;

    Flag = i51AdeOsFileWriteInt32( File, &Version);
    if( iNULL==Flag ) goto ERROR;

    Flag = i51AdeOsFileFlush(File);
    if( iNULL==Flag ) goto ERROR;

    Flag = i51KitG2WriteFileCRC(File);
    if( iNULL==Flag ) goto ERROR;

 #if i51KITG2_DEBUG
    iLog( "G2SetContainerContentVersion end , %x,%x",Handle,Version);
#endif

    return iTRUE;

ERROR:
    iLog("KIT - G2 : RW set version");
    return iFALSE;
	
}
	
iKIT iBOOL i51KitG2GetContainerContentVersion( iHANDLE Handle, iU32 *Version)
{
    // author : Otto
    // since : 2011.10.10
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE handle: �������
    //  iU32 Version: �������ݰ汾��
    // notes : ����������ݰ汾��

    iU8  Flag = iFALSE;
    iFILE File = ((i51ImageContainer * )Handle)->FileHandle;
	
 #if i51KITG2_DEBUG
    iLog( "G2GetContainerContentVersion start , %x,%x",Handle,Version);
#endif

    if( iNULL==Handle || iNULL==File ) return iFALSE;
	
    Flag = i51AdeOsFileSetSeek( File, I51KITG2_OFFSET_FILE_VERSION, i51_OS_FL_SEEK_HEAD );
    if( iNULL==Flag ) goto ERROR;
	
    Flag = i51AdeOsFileReadInt32( File, Version);
    if( iNULL==Flag ) goto ERROR;

 #if i51KITG2_DEBUG
    iLog( "G2GetContainerContentVersion end , %x,%x",Handle,Version);
#endif

    return iTRUE;

ERROR:
    iLog("KIT - G2 : RW get version");
    return iFALSE;
}
	
iKIT iU32 i51KitG2SetImageMemoryCopy( iHANDLE Handle, iU32 TotalImage, iU32 ImageId[])
{
    // author : Otto
    // since : Sep 27 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE handle: �������
    //  iU32 TotalImage: ��Ҫ���õ�ͼƬ����
    //  iU32 ImageId[] : ͼƬID

    // return iU32: �����ɹ���ͼƬ����    
    // notes : ����������ͼƬ�������ڴ��У����ImageIdΪ����ȫ������

    // ������ͼƬ����
    iU32 AllImageNum = 0;
    // ���óɹ���ͼƬ����	
    iU32 SetOkNum = 0;
    // ��Ҫ���õ�ͼƬ����	
    iU32 NeedSetNum = 0;
    // ͼƬ�������ַ
    i51ImageIndex * ImageIndexTable = iNULL;
    // ͼƬ������ַ���
    i51ImageContainer * ImageContainerEntry = iNULL;
    // ͼƬ�����ļ�
    iFILE ImageContainerFile = iNULL;
    // �ļ���������ֵ���	
    iBOOL FlagReturn =iFALSE;
    // ��ǰ������ͼƬID
    iU32 CurrCopyID = 0 ;	
    // ��ǰͼƬ�ļ������ݳ���
    iU32 CurrentDataLength = 0;
    // ���ļ��ж�ȡ������ʵ�ʳ���
    iU32 LengthReadData = 0;
    iS32 Temp1 = 0;

 #if i51KITG2_DEBUG
    iLog( "G2SetImageMemoryCopy start , %x,%x, %x",Handle,TotalImage, ImageId);
#endif
	
    if( iNULL == Handle ) return iFALSE;

//iLog("Handle:0X%X",Handle);

    ImageContainerEntry = (i51ImageContainer * )Handle;

//iLog("S1");
    ImageIndexTable = ImageContainerEntry->ImageIndexTable;

//iLog("S2");
    AllImageNum = ImageContainerEntry->ImageMaxId;

//iLog("AllImageNum:%d",AllImageNum);
    ImageContainerFile = ImageContainerEntry->FileHandle;

    if( AllImageNum<TotalImage ) NeedSetNum = AllImageNum;
    else NeedSetNum = TotalImage;

    if( iNULL==ImageId )  NeedSetNum = AllImageNum;

//iLog("NeedSetNum:%d",NeedSetNum);
    if( iFALSE == RESUNE_MEMORY_COPY )
    {
        SetMemoryCopyCount ++;
    }
//iLog("SetMemoryCopyCount:%d",SetMemoryCopyCount);

    for( Temp1 = 0; Temp1 < NeedSetNum; Temp1++)	
    {
//iLog("Temp1:%d",Temp1);
        if( ImageId)
        {
            CurrCopyID = ImageId[Temp1];
        }
        else
        {
            CurrCopyID = Temp1;
        }

//iLog("T1");
        if( ImageIndexTable[CurrCopyID].FlagImageExist==0) continue;
		
        if( CurrCopyID>AllImageNum ) continue;
		
//iLog("T2");
        if( ImageIndexTable[CurrCopyID].FlagCopyRAM ) continue;

        // �ļ���ʽǰ��λ���ļ����ͱ�ʾ��������λ�汾�ţ���λ��ʼ��������λ��ͼƬ�����ܳ���
//        FlagReturn = i51AdeOsFileSetSeek( ImageContainerFile, (ImageIndexTable[CurrCopyID].ImageEntry+5), i51_OS_FL_SEEK_HEAD );
//        if( iFALSE == FlagReturn )  goto ERROR;

           CurrentDataLength = ImageIndexTable[CurrCopyID].LengthTotalData;
//        FlagReturn = i51AdeOsFileReadInt32( ImageContainerFile, &CurrentDataLength);
//        if( iFALSE == FlagReturn )  goto ERROR;

        // �ļ���ʽǰ��λ���ļ����ͱ�ʾ��������λ�汾�ţ���λ��ʼ��������λ��ͼƬ�����ܳ���
        FlagReturn = i51AdeOsFileSetSeek( ImageContainerFile, (ImageIndexTable[CurrCopyID].ImageEntry), i51_OS_FL_SEEK_HEAD );

//iLog("T3");
        ImageIndexTable[CurrCopyID].MemoryEntry = ( iU32)(i51AdeOsMalloc( CurrentDataLength,  __FILE__ , __LINE__ ));
        if( iFALSE == ImageIndexTable[CurrCopyID].MemoryEntry ) 
        {
            iLog("KIT - G2 : malloc set memory");
            return SetOkNum;
        }

        FlagReturn = i51AdeOsFileRead( ImageContainerFile, (void *)(ImageIndexTable[CurrCopyID].MemoryEntry), CurrentDataLength,&LengthReadData);
        if( iFALSE == FlagReturn || ( LengthReadData!= (CurrentDataLength) ) ) goto ERROR;

        ImageIndexTable[CurrCopyID].FlagCopyRAM = iTRUE;
        if( RESUNE_MEMORY_COPY )
        {
            ImageIndexTable[CurrCopyID].SetMemoryCopyCount = ContainerManage[CurrContainerManageNum].MemCopyCount[Temp1];
        }
        else
        {
            ImageIndexTable[CurrCopyID].SetMemoryCopyCount = SetMemoryCopyCount;
        }
        SetOkNum ++;
    }

 #if i51KITG2_DEBUG
    iLog( "G2SetImageMemoryCopy end , %x,%x, %x",Handle,TotalImage, ImageId);
#endif

    RESUNE_MEMORY_COPY = 0;

    return SetOkNum;

ERROR:
    i51AdeOsFree( (void *)ImageIndexTable[CurrCopyID].MemoryEntry);
    ImageIndexTable[CurrCopyID].MemoryEntry = iNULL;
    iLog("KIT - G2 : RW set memory");
    return SetOkNum;
	
}

iKIT iU32 i51KitG2UnsetImageMemoryCopy( iHANDLE Handle, iU32 TotalImage, iU32 ImageId[])
{
    // author : Otto
    // since : Sep 27 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE handle: �������
    //  iU32 TotalImage: ��Ҫ���õ�ͼƬ����
    //  iU32 ImageId[] : ͼƬID

    // return iU32: ȡ�������ɹ���ͼƬ����    
    // notes : ȡ��������ͼƬ�������ڴ��У����ImageIdΪ����ȫ��ȡ��
    
    // ������ͼƬ����
    iU32 AllImageNum = 0;
    // ���óɹ���ͼƬ����	
    iU32 SetOkNum = 0;
    // ��Ҫ���õ�ͼƬ����	
    iU32 NeedSetNum = 0;
    // ͼƬ�������ַ
    i51ImageIndex * ImageIndexTable = iNULL;
    // ͼƬ������ַ���
    i51ImageContainer * ImageContainerEntry = iNULL;
    // ��������ֵ���	
    iBOOL FlagReturn =iFALSE;
    // ��ǰ������ͼƬID
    iU32 CurrCopyID = 0 ;	
    iS32 Temp1 = 0;

 #if i51KITG2_DEBUG
    iLog( "G2UnsetImageMemoryCopy start , %x,%x, %x",Handle,TotalImage, ImageId);
#endif
	
    if( iNULL == Handle ) return iFALSE;
	
    ImageContainerEntry = (i51ImageContainer * )Handle;

    ImageIndexTable = ImageContainerEntry->ImageIndexTable;

    AllImageNum = ImageContainerEntry->ImageMaxId;

    if( AllImageNum<TotalImage ) NeedSetNum = AllImageNum;
    else NeedSetNum = TotalImage;

    if( iNULL==ImageId )  NeedSetNum = AllImageNum;

    for( Temp1 = 0; Temp1 < NeedSetNum; Temp1++)	
    {
        if( ImageId)
        {
            CurrCopyID = ImageId[Temp1];
        }
        else
        {
            CurrCopyID = Temp1;
        }
		
        if( CurrCopyID>AllImageNum ) continue;
		
        if( ImageIndexTable[CurrCopyID].FlagImageExist==0) continue;
		
        if( ImageIndexTable[CurrCopyID].FlagCopyRAM == iFALSE ) continue;
		
        FlagReturn = i51AdeOsFree( (void *)(ImageIndexTable[CurrCopyID].MemoryEntry));
        ImageIndexTable[CurrCopyID].MemoryEntry = iNULL;
        if( iFALSE == FlagReturn ) return iFALSE;

        ImageIndexTable[CurrCopyID].FlagCopyRAM = iFALSE;
        ImageIndexTable[CurrCopyID].MemoryEntry = iNULL;
        SetOkNum ++;
    }
	
 #if i51KITG2_DEBUG
    iLog( "G2UnsetImageMemoryCopy end , %x,%x, %x",Handle,TotalImage, ImageId);
#endif
    return SetOkNum;
	
}

iKIT iBOOL i51KitG2StopDecode( iHANDLE Handle)
{
	// ֹͣ����
	if(TimeIdZlibDecodeImage)
	{
		if( iNULL==ZlibImageContainer || ZlibImageContainer->FileHandle != (iFILE)(Handle) )
		{
			iLog("KIT - G2 : stop decode handle:%d",(iU32)Handle);
			return iFALSE;
		}
		
		if( iFALSE== i51AdeOsTimerStop( TimeIdZlibDecodeImage ))
		{
			iLog("KIT - G2 : stop decode time:%d",TimeIdZlibDecodeImage);
			return iFALSE;
		}
		TimeIdZlibDecodeImage = iNULL;
	}

	if(FlagDecodeImage && ZlibImageContainer )
	{
		i51KitG2CloseImageContainer(ZlibImageContainer);
	}

	return iTRUE;
}

iKIT iBOOL i51KitG2GetImageAttr ( iHANDLE ContainerHandle,iU32 ImageID , iU32 * ImageWidth, iU32 * ImageHeight, iU8 *MemoryCopy)
{
    //  iHANDLE handle: �������
    //  iU32 TotalImage: ��Ҫ���õ�ͼƬ����
    //  iU32 ImageId : ͼƬID
    //  iU32 * ImageWidth: ��õ�ͼƬ���
    //  iU32 * ImageHeight:��õ�ͼƬ�߶�
    
    // ͼƬ������ַ���
    i51ImageContainer * ImageContainer = iNULL;
    // ͼƬ�������ַ
    i51ImageIndex * ImageIndexTable = iNULL;

 #if i51KITG2_DEBUG
    iLog( "G2GetImageAttr start , %x,%x, %x,%x",ContainerHandle,ImageID, ImageWidth,ImageHeight);
#endif

    if( iNULL==ContainerHandle ) return iFALSE;
    ImageContainer =  (i51ImageContainer * )ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    if( iNULL == ImageIndexTable )  return iFALSE;
    if(ImageIndexTable[ImageID].FlagImageExist==0) return iFALSE;    

    if(ImageWidth) *ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    if(ImageHeight) *ImageHeight = ImageIndexTable[ImageID].ImageHeight;
    if(MemoryCopy) *MemoryCopy = ImageIndexTable[ImageID].FlagCopyRAM;
	
 #if i51KITG2_DEBUG
    iLog( "G2GetImageAttr end , %x,%x, %x,%x",ContainerHandle,ImageID, ImageWidth,ImageHeight);
#endif
    return iTRUE;
  
}

iKIT iBOOL i51KitG2DrawImage ( iHANDLE ContainerHandle,iU32 ImageID , iRECT* DesRect , iRECT* SrcRect )
{
    // author : Otto
    // since : Aug 29 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE ImageContainer �������ļ�ָ��
    //  iU16 ImageID  ��������ͼƬ�ļ�ID
    //  iRECT* DesRect :   ����Ŀ�ĵ�����
    //  iRECT* SrcRect :    ����ͼƬԭʼ����
    
    // notes : ����ImageContainer�����ڱ��ΪImageID��ͼ��
    // SrcRect Ϊ��ʱ�ڵ�(DesRect->X, DesRect->Y)����ʼ��������ͼƬ
    // DesRect Ϊ������ƴ���Ļ���Ͻǿ�ʼ

    // ͼƬ������ַ���
    i51ImageContainer * ImageContainer = iNULL;
    // ͼƬ�������ַ
    i51ImageIndex * ImageIndexTable = iNULL;
    // �����ļ�ָ���ַ
    iFILE File = iNULL;
     // ͼƬ���뷽ʽ
    iU8   DecodeType = 0;
    iU16 CurrDrawID = 0;
    iU8 Ret = iFALSE;
    iS32 ImageWidth = 0;
    iS32 ImageHeight = 0;	

 #if i51KITG2_DEBUG
    iLog( "G2DrawImage start , %x,%x, %x,%x",ContainerHandle,ImageID, DesRect,SrcRect);
#endif
	
    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = (i51ImageContainer *)ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL == ImageIndexTable || iNULL==File)  return iFALSE;

    if(ImageIndexTable[ImageID].FlagImageExist==0 ) return iFALSE;
	
    DecodeType = ImageIndexTable[ImageID].DecodeType;
    ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    ImageHeight = ImageIndexTable[ImageID].ImageHeight;

    if(DecodeType!=I51KITG2_DECODE_LINEAR)
    {
    	iLog("�ļ���ʽ��������������ͼƬ�ļ�!");
    	return iFALSE;
    }

    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_PICTURE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        if( SrcRect )
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = SrcRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = SrcRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = SrcRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = SrcRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = ImageWidth;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = ImageHeight;
        }
		
        if( DesRect )		
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X = DesRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y = DesRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = DesRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = DesRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = i51CurrentVpActivity->VPActualRect.Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = i51CurrentVpActivity->VPActualRect.Height;
        }

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DrawDirection = I51KITG2_DRAW_IAMGE_NORMAL;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ImageHandle = ImageID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ContainerHandle = ContainerHandle;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DarwAngle = 0;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }
 
    if( ImageIndexTable[ImageID].FlagCopyRAM )
    {
        // ����ѹ�����뷽ʽ
        // ��ͼ��������Ϣ�������Ƚ������Ա��룬���ݸ�ʽ���£�
        // x,y,w,RGB���ݣ�����x��y�Ƿ�͸��ɫ��������꣬w�������ж��ٸ���͸��ɫ��x��y�ֱ�ռ��1.5�ֽڣ�wռ�����ֽڡ�
        Ret = DrawRAMLinearImage( ContainerHandle, ImageID,  DesRect, SrcRect, I51KITG2_DRAW_IAMGE_NORMAL);
    }
    else
    {  
        // ��ʾ����ѹ������ͼƬ
        // ��ͼ��������Ϣ�������Ƚ������Ա��룬���ݸ�ʽ���£�
        // x,y,w,RGB���ݣ�����x��y�Ƿ�͸��ɫ��������꣬w�������ж��ٸ���͸��ɫ��x��y�ֱ�ռ��1.5�ֽڣ�wռ�����ֽڡ�
        Ret = DrawRAMLinearImage( ContainerHandle, ImageID, DesRect, SrcRect, I51KITG2_DRAW_IAMGE_NORMAL);
    }

 #if i51KITG2_DEBUG
    iLog( "G2DrawImage end , %x,%x, %x,%x",ContainerHandle,ImageID, DesRect,SrcRect);
#endif
	
    return Ret;
}


iKIT iBOOL  i51KitG2MirrorImage  (  iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect )
{
    // author : Otto
    // since : Aug 29 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE ImageContainer �������ļ�ָ��
    //  iU16 ImageID  ��������ͼƬ�ļ�ID
    //  iRECT* DesRect :   ����Ŀ�ĵ�����
    //  iRECT* SrcRect :    ����ͼƬԭʼ����

    // notes : ��������ʾ��ʽ����ImageContainer�����ڱ��ΪImageID��ͼ��
    // SrcRect Ϊ��ʱ�ڵ�(DesRect->X, DesRect->Y)����ʼ��������ͼƬ
    // DesRect Ϊ������ƴ���Ļ���Ͻǿ�ʼ

    // ͼƬ������ַ���
    i51ImageContainer * ImageContainer = iNULL;
    // ͼƬ�������ַ
    i51ImageIndex * ImageIndexTable = iNULL;
    // �����ļ�ָ���ַ
    iFILE File = iNULL;
     // ͼƬ���뷽ʽ
    iU8   DecodeType = 0;
    iU16 CurrDrawID = 0;
    iU8 Ret = iFALSE;

    iS32 ImageWidth = 0;
    iS32 ImageHeight = 0;	

 #if i51KITG2_DEBUG
    iLog( "G2MirrorImage start , %x,%x, %x,%x",ContainerHandle,ImageID, DesRect,SrcRect);
#endif
	

    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = (i51ImageContainer * )ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL == ImageIndexTable || iNULL==File)  return iFALSE;
	
    if(ImageIndexTable[ImageID].FlagImageExist==0 ) return iFALSE;

    DecodeType = ImageIndexTable[ImageID].DecodeType;
    ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    ImageHeight = ImageIndexTable[ImageID].ImageHeight;

    if(DecodeType!=I51KITG2_DECODE_LINEAR)
    {
    	iLog("�ļ���ʽ��������������ͼƬ�ļ�!");
    	return iFALSE;
    }

  
    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_PICTURE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        if( SrcRect )
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = SrcRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = SrcRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = SrcRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = SrcRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = ImageWidth;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = ImageHeight;
        }
		
        if( DesRect )		
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X = DesRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y = DesRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = DesRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = DesRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = i51CurrentVpActivity->VPActualRect.Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = i51CurrentVpActivity->VPActualRect.Height;
        }

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DrawDirection = I51KITG2_DRAW_IAMGE_MIRROR;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ImageHandle = ImageID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ContainerHandle = ContainerHandle;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DarwAngle = 0;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }
 
    if( ImageIndexTable[ImageID].FlagCopyRAM )
    {
        // ����ѹ�����뷽ʽ
        // ��ͼ��������Ϣ�������Ƚ������Ա��룬���ݸ�ʽ���£�
        // x,y,w,RGB���ݣ�����x��y�Ƿ�͸��ɫ��������꣬w�������ж��ٸ���͸��ɫ��x��y�ֱ�ռ��1.5�ֽڣ�wռ�����ֽڡ�
        Ret = DrawRAMLinearImage( ContainerHandle, ImageID,  DesRect, SrcRect, I51KITG2_DRAW_IAMGE_MIRROR);
    }
    else
    {  
        // ��ʾ����ѹ������ͼƬ
        // ��ͼ��������Ϣ�������Ƚ������Ա��룬���ݸ�ʽ���£�
        // x,y,w,RGB���ݣ�����x��y�Ƿ�͸��ɫ��������꣬w�������ж��ٸ���͸��ɫ��x��y�ֱ�ռ��1.5�ֽڣ�wռ�����ֽڡ�
        Ret = DrawRAMLinearImage( ContainerHandle, ImageID, DesRect,  SrcRect, I51KITG2_DRAW_IAMGE_MIRROR);
    }

 #if i51KITG2_DEBUG
    iLog( "G2MirrorImage end , %x,%x, %x,%x",ContainerHandle,ImageID, DesRect,SrcRect);
#endif
	
    return Ret;
	
}

iKIT iBOOL   i51KitG2FlipImage   (  iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect )
{
    // author : Otto
    // since : Aug 29 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE ImageContainer �������ļ�ָ��
    //  iU16 ImageID  ��������ͼƬ�ļ�ID
    //  iRECT* DesRect :   ����Ŀ�ĵ�����
    //  iRECT* SrcRect :    ����ͼƬԭʼ����

    // notes : �����¶��ϵ���ʾ��ʽ����ImageContainer�����ڱ��ΪImageID��ͼ��
    // SrcRect Ϊ��ʱ�ڵ�(DesRect->X, DesRect->Y)����ʼ��������ͼƬ
    // DesRect Ϊ������ƴ���Ļ���Ͻǿ�ʼ

    // ͼƬ������ַ���
    i51ImageContainer * ImageContainer = iNULL;
    // ͼƬ�������ַ
    i51ImageIndex * ImageIndexTable = iNULL;
    // �����ļ�ָ���ַ
    iFILE File = iNULL;
     // ͼƬ���뷽ʽ
    iU8   DecodeType = 0;
    iU16 CurrDrawID = 0;
    iU8 Ret = 0;

    iS32 ImageWidth = 0;
    iS32 ImageHeight = 0;	

 #if i51KITG2_DEBUG
    iLog( "G2FlipImage start , %x,%x, %x,%x",ContainerHandle,ImageID, DesRect,SrcRect);
#endif
	
    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = (i51ImageContainer *)ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL == ImageIndexTable || iNULL==File)  return iFALSE;
	
    if(ImageIndexTable[ImageID].FlagImageExist==0 ) return iFALSE;

    DecodeType = ImageIndexTable[ImageID].DecodeType;
    ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    ImageHeight = ImageIndexTable[ImageID].ImageHeight;
  
    if(DecodeType!=I51KITG2_DECODE_LINEAR)
    {
    	iLog("�ļ���ʽ��������������ͼƬ�ļ�!");
    	return iFALSE;
    }

    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_PICTURE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        if( SrcRect )
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = SrcRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = SrcRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = SrcRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = SrcRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = ImageWidth;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = ImageHeight;
        }
		
        if( DesRect )		
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X = DesRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y = DesRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = DesRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = DesRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = i51CurrentVpActivity->VPActualRect.Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = i51CurrentVpActivity->VPActualRect.Height;

        }

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DrawDirection = I51KITG2_DRAW_IAMGE_HANDSTAND;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ImageHandle = ImageID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ContainerHandle = ContainerHandle;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DarwAngle = 0;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }
 
    if( ImageIndexTable[ImageID].FlagCopyRAM )
    {
        // ����ѹ�����뷽ʽ
        // ��ͼ��������Ϣ�������Ƚ������Ա��룬���ݸ�ʽ���£�
        // x,y,w,RGB���ݣ�����x��y�Ƿ�͸��ɫ��������꣬w�������ж��ٸ���͸��ɫ��x��y�ֱ�ռ��1.5�ֽڣ�wռ�����ֽڡ�
        Ret = DrawRAMLinearImage( ContainerHandle, ImageID,  DesRect, SrcRect, I51KITG2_DRAW_IAMGE_HANDSTAND);
    }
    else
    {  
        // ��ʾ����ѹ������ͼƬ
        // ��ͼ��������Ϣ�������Ƚ������Ա��룬���ݸ�ʽ���£�
        // x,y,w,RGB���ݣ�����x��y�Ƿ�͸��ɫ��������꣬w�������ж��ٸ���͸��ɫ��x��y�ֱ�ռ��1.5�ֽڣ�wռ�����ֽڡ�
        Ret = DrawRAMLinearImage( ContainerHandle, ImageID, DesRect,  SrcRect, I51KITG2_DRAW_IAMGE_HANDSTAND);
    }

 #if i51KITG2_DEBUG
    iLog( "G2FlipImage end , %x,%x, %x,%x",ContainerHandle,ImageID, DesRect,SrcRect);
#endif
	
    return Ret;
	
}

#if ROTATE_SPECIAE

static iBOOL   i51G2DrawBasePixByAngle( iU16 *SrcBuf, iU32 SrcBufLen, iS32 SrcMoveX , iS32 SrcMoveY,	
iS32 CenterX , iS32 CenterY ,iRECT* inDesRect, iRECT* inSrcRect, iS32 Angle)
{
	iS32 i = 0, j = 0;
	iU16 *SrcTmp = iNULL, *DrawTmp = iNULL;
	iU16 *SrcBufPtr = iNULL;
	iS16 CurrentImageX = 0,  CurrentImageY = 0, CurrentImageWidth = 0;
	iS16 StartX=0, EndX=0, Width = 0;
	iS16 SrcStartX=0, SrcStartY=0, SrcEndX=0, SrcEndY=0;
	iS16 StartY = 0;
	iU32 DataSize = 0;
	iS32 TempX = 0;
	iS32 TempY = 0;
	iS32 OffSetX = 0;
	iS32 OffSetY = 0;
	iS32 DesStartX = 0, DesStartY = 0, DesEndX = 0, DesEndY = 0;
	iU8 Flag = iTRUE;
	
	DesStartX = inDesRect->X;
	DesStartY = inDesRect->Y;
	DesEndX = DesStartX + inDesRect->Width;
	DesEndY = DesStartY + inDesRect->Height;

 	switch( Angle )
	{
		case 0:
			// ��A(x,y)��ת0����Ӧ�����:
			// B(x+ SrcMoveX -inSrcRect->X, y+ SrcMoveY  -inSrcRect->Y )
			// ��Ϊ:B(y+OffSetX,x+OffSetY);
			OffSetX = SrcMoveX - inSrcRect->X ;
			OffSetY = SrcMoveY - inSrcRect->Y ;
			break;
		case 90:
			// ��A(x,y)��ת90����Ӧ�����:
			// B(inSrcRect->Y - SrcMoveY + CenterY + CenterX-y, SrcMoveX - inSrcRect->X + CenterY - CenterX+x)
			// ��Ϊ:B(y+OffSetX,x+OffSetY);
			OffSetY = SrcMoveX - inSrcRect->X + CenterY - CenterX;
			OffSetX = inSrcRect->Y - SrcMoveY + CenterY + CenterX;
			break;
		case 180:	
			// ��A(x,y)��ת180����Ӧ�����:
			// B(2*CenterX+inSrcRect->X-SrcMoveX-x,)
			// ��Ϊ:B(OffSetX-x,OffSetY-y,2*CenterY+inSrcRect->Y-SrcMoveY);
			OffSetY = 2*CenterY+inSrcRect->Y-SrcMoveY;
			OffSetX = 2*CenterX+inSrcRect->X-SrcMoveX;
			break;
		case 270:	
			// ��A(x,y)��ת270����Ӧ�����:
			// B(CenterX - CenterY  + SrcMoveY - inSrcRect->Y +y, CenterY + CenterX - SrcMoveX + inSrcRect->X - x )
			// ��Ϊ:B(OffSetX+x,OffSetY+y);
			OffSetY = CenterY + CenterX - SrcMoveX + inSrcRect->X;
			OffSetX = CenterX - CenterY  + SrcMoveY - inSrcRect->Y ;
			break;
		default:
			iLog(" KIT - G2 : angle %d", Angle);
			return iFALSE;
	}

 	//��λ��ͼƬ���ݵĵ�һ��
	SrcBufPtr = SrcBuf;   
	CurrentImageX = SrcBufPtr[0];
	CurrentImageY = SrcBufPtr[1];
	CurrentImageWidth = SrcBufPtr[2];
	while (CurrentImageY < inSrcRect->Y)
	{
		DataSize += (6 + CurrentImageWidth*2);

		SrcBufPtr += (3 + CurrentImageWidth);
		CurrentImageX = SrcBufPtr[0];
		CurrentImageY = SrcBufPtr[1];
		CurrentImageWidth = SrcBufPtr[2];
		if (DataSize >= SrcBufLen)
		{
			return iFALSE;
		}
	}

	SrcStartX = inSrcRect->X;
	SrcEndX   =  SrcStartX + inSrcRect->Width;
	SrcStartY = inSrcRect->Y;
	SrcEndY   = SrcStartY + inSrcRect->Height;

 //	if (0xFF == AlphaSys)
	{
		Flag = iTRUE;
		while (Flag)
		{
			CurrentImageX = SrcBufPtr[0];
			CurrentImageY = SrcBufPtr[1];
			CurrentImageWidth = SrcBufPtr[2];

			DataSize += (6 + CurrentImageWidth*2);
			if (DataSize > SrcBufLen || CurrentImageY >= SrcEndY)
			{
				break;
			}

			if ( CurrentImageX >= SrcEndX)
			{
				SrcBufPtr += (3 + CurrentImageWidth); 
				continue;
			}

			if( CurrentImageX<SrcStartX )
			{
			
				SrcBufPtr += 3+(SrcStartX-CurrentImageX);
				CurrentImageWidth -= (SrcStartX-CurrentImageX);
				CurrentImageX = SrcStartX;
			}
			else
			{
				SrcBufPtr += 3; 
			}


			switch (Angle)
			{
				case 0:
					StartX = OffSetX + CurrentImageX;
					StartY = OffSetY + CurrentImageY;
					if( StartY>=DesStartY && StartY< DesEndY )
					{
						Width = CurrentImageWidth;
						SrcTmp = SrcBufPtr;
						if( StartX<DesStartX )
						{
							Width -= DesStartX - StartX;
							SrcTmp += DesStartX - StartX;
							StartX = DesStartX;
						}
						if( StartX+Width>DesEndX )
						{
							Width = DesEndX - StartX;
						}
					}
					else if( StartY>=DesEndY )
					{
						Flag = iFALSE;
						break;
					}
					else
					{
						break;
					}
					
					DrawTmp = LcdBuffer + ScreenWidth*StartY + StartX;
					for (j=0; j<Width; j++)
					{
						*DrawTmp = i51KitG2GetNewColorEx(*DrawTmp,*SrcTmp,AlphaSys);
						SrcTmp++;
						DrawTmp ++;
					}			
					break;
				case 90:
					StartX = OffSetX - CurrentImageY;
					StartY = OffSetY + CurrentImageX;
					if( StartX>=DesStartX && StartX< DesEndX && StartY <DesEndY )
					{
						Width = CurrentImageWidth;
						SrcTmp = SrcBufPtr;
						if( StartY<DesStartY )
						{
							Width -= DesStartY - StartY;
							SrcTmp += DesStartY - StartY;
							StartY = DesStartY;
						}
						if( StartY+Width>DesEndY )
						{
							Width = DesEndY - StartY;
						}
					}
					else if( StartX<DesStartX )
					{
						Flag = iFALSE;
						break;
					}
					else
					{
						break;
					}
					
					DrawTmp = LcdBuffer + ScreenWidth*StartY + StartX;
					for (j=0; j<Width; j++)
					{
						*DrawTmp = i51KitG2GetNewColorEx(*DrawTmp,*SrcTmp,AlphaSys);
						SrcTmp++;
						DrawTmp += ScreenWidth;
					}			
					break;
				case 180:
					StartX = OffSetX - CurrentImageX;
					StartY = OffSetY - CurrentImageY;
					if( StartY>=DesStartY && StartY< DesEndY && StartX>= DesStartX )
					{
						Width = CurrentImageWidth;
						SrcTmp = SrcBufPtr;
						if( StartX>=DesEndX )
						{
							Width -= StartX - DesEndX +1;
							SrcTmp += StartX - DesEndX +1;
							StartX = DesEndX-1;
						}
						if( StartX-Width<DesStartX )
						{
							Width = StartX-DesStartX+1;
						}
					}
					else if( StartY<DesStartY )
					{
						Flag = iFALSE;
						break;
					}
					else 
					{
 						break;
					}
					
					DrawTmp = LcdBuffer + ScreenWidth*StartY + StartX;
					for (j=0; j<Width; j++)
					{
//						*DrawTmp = *SrcTmp;
						*DrawTmp = i51KitG2GetNewColorEx(*DrawTmp,*SrcTmp,AlphaSys);
						SrcTmp++;
						DrawTmp --;
					}			
					break;
				case 270:
					StartX = OffSetX + CurrentImageY;
					StartY = OffSetY - CurrentImageX;
					if( StartX>=DesStartX && StartX< DesEndX && StartY >=DesStartY )
					{
						Width = CurrentImageWidth;
						SrcTmp = SrcBufPtr;
						if( StartY>=DesEndY )
						{
							Width -= StartY- DesEndY -1;
							SrcTmp += StartY- DesEndY -1;
							StartY = DesEndY -1 ;
						}
						if( StartY-Width<DesStartY )
						{
							Width = StartY - DesStartY +1;
						}
					}
					else if( StartX>=DesEndX )
					{
						Flag = iFALSE;
						break;
					}
					else
					{
						break;
					}
					
					DrawTmp = LcdBuffer + ScreenWidth*StartY + StartX;
					for (j=0; j<Width; j++)
					{
//						*DrawTmp = *SrcTmp;
						*DrawTmp = i51KitG2GetNewColorEx(*DrawTmp,*SrcTmp,AlphaSys);
						SrcTmp++;
						DrawTmp -= ScreenWidth;
					}			
					break;
				default:
					return iFALSE;
					break;
			}

			SrcBufPtr += CurrentImageWidth; 

			if (DataSize > SrcBufLen-3) break;

		}
	}
 
	return iTRUE;

}

static iBOOL   i51KitG2RotateImageEx1( iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect, 
									 iS32 SrcMoveX , iS32 SrcMoveY, iS32 RotateCenterX , iS32 RotateCenterY , iU32 RotateType, iS32 Angle )
{
	// author : yepan
	// since : 2012-05-30
	// (C) 2012 , PKIG Tech. Co., Ltd.

	//ContainerHandle	������ͼƬ���������ľ��
	//ImageID	ͼƬID
	//DesRect	ͼƬ���Ƶ�Ŀ������
	//SrcRect	ͼƬ���ƵĲü���
	//SrcMoveX	ͼƬ��תǰƽ�����㣨SrcMoveX,SrcMoveY��
	//SrcMoveY	ͼƬ��תǰƽ�����㣨SrcMoveX,SrcMoveY��
	//RotateCenterX	ͼƬ��ת����X����
	//RotateCenterY	ͼƬ��ת����Y����
	//RotateType	��ת����
	//Angle	��ת�Ƕ� ֻ����90��180��270����Ƕ�

	// notes : ��˳ʱ�뷽����תһ���ǶȺ����ContainerHandle�����ڱ��ΪImageID��ͼ��

	iBOOL	Ret = iTRUE;

	// ͼƬ������ַ���
	i51ImageContainer * ImageContainer = iNULL;
	// ͼƬ�������ַ
	i51ImageIndex * ImageIndexTable = iNULL;
	// �����ļ�ָ���ַ
	iFILE File = iNULL;

	// ͼƬ��Ⱥ͸߶�
	iU32 ImageWidth = 0, ImageHeight = 0;
	//ͼƬ�����ܳ���/ͼƬ�������ݳ���
	iU32 LengthTotalData = 0, LengthIamgeData = 0;
	iU32 ReserveOffset = 0x0c;
	// ��Alpha��Ϣ�����ݵ����
	iU16 TotalAlphaNumber = 0;

	// ͼƬ�����ļ���ʼ��ַ
	iU32 DataEntry = 0;
	//ͼƬ���ݻ���ָ��
	iU8  *ImageDataEntry = iNULL;

	//��ȡ�����ݳ���
	iU32	ReadLenght = 0;	

	//�Ƕ�ת����0~360
	if( (Angle%90)!=0 )
	{
		iLog("KIT - G2 : angle no special");
		return iFALSE;
	}

	ImageContainer = (i51ImageContainer *)ContainerHandle;
	if ( iNULL == ImageContainer )
	{
		return iFALSE;
	}

	ImageIndexTable = ImageContainer->ImageIndexTable;
	File = ImageContainer->FileHandle;
	if ( iNULL == ImageIndexTable || iNULL == File)
	{
		return iFALSE;
	}

	DataEntry = ImageIndexTable[ImageID].MemoryEntry;
	LengthTotalData = ImageIndexTable[ImageID].LengthTotalData;
	LengthIamgeData = ImageIndexTable[ImageID].LengthIamgeData;
	TotalAlphaNumber = ImageIndexTable[ImageID].AlphaPixelNum;
	ImageWidth = ImageIndexTable[ImageID].ImageWidth;
	ImageHeight = ImageIndexTable[ImageID].ImageHeight;

	//����ͼƬ���ݵ��ڴ�
	if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM )
	{
		DataEntry = ImageIndexTable[ImageID].ImageEntry;
		if( iFALSE == i51AdeOsFileSetSeek( File, DataEntry, i51_OS_FL_SEEK_HEAD ) )
		{
			return iFALSE;
		}

		ImageDataEntry = i51AdeOsMalloc(LengthTotalData, __FILE__ , __LINE__ );
		if( iNULL == ImageDataEntry ) 
		{
			iLog("KIT - G2 : i51KitG2RotateImageEx malloc err");
			return iFALSE;
		}

		Ret = i51AdeOsFileRead( File, ImageDataEntry, LengthTotalData, &ReadLenght);
		if( iFALSE == Ret || LengthTotalData!=ReadLenght) 
		{
			i51AdeOsFree(ImageDataEntry);
			ImageDataEntry = iNULL;
			iLog(" KIT - G2 : rotate read data");
			return iFALSE;             
		}
	}
	else
	{
		ImageDataEntry = (iU8*)ImageIndexTable[ImageID].MemoryEntry;
	}

	DataEntry = (iU32)ImageDataEntry;
	DataEntry += I51KITG2_OFFSET_PIXELDATA;

	//����������
 	if ( iFALSE == i51G2DrawBasePixByAngle((iU16 *)DataEntry, (iU32)(LengthIamgeData-I51KITG2_OFFSET_PIXELDATA),
						SrcMoveX, SrcMoveY, RotateCenterX, RotateCenterY,DesRect, SrcRect,  Angle) )
	{
		if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM  && ImageDataEntry )
		{
			i51AdeOsFree(ImageDataEntry);
			ImageDataEntry = iNULL;
		}
		return iFALSE;
	}

	//��alpha����
	if(TotalAlphaNumber>0)
	{
		iS32 CurrAlphaNum = 0;
		iS32 LcdBuffPos = 0;
		iU16 *ReadDataBuf = iNULL;
		iALPHA AlphaNumber = 0;
		iS16 AlphaColor = 0;

		iS16 SrcAlphaX = 0, SrcAlphaY = 0, DesAlphaX=0, DesAlphaY=0;
		iS16 OffSetX=0, OffSetY=0;

		iS32 DesStartX = 0, DesStartY = 0, DesEndX = 0, DesEndY = 0;
		iS32 SrcStartX = 0, SrcStartY = 0, SrcEndX = 0, SrcEndY = 0;

		DesStartX = DesRect->X;
		DesStartY = DesRect->Y;
		DesEndX = DesStartX + DesRect->Width;
		DesEndY = DesStartY + DesRect->Height;

		SrcStartX = SrcRect->X;
		SrcStartY = SrcRect->Y;
		SrcEndX = SrcStartX + SrcRect->Width;
		SrcEndY = SrcStartY + SrcRect->Height;

		DataEntry = (iU32)ImageDataEntry;
		DataEntry += (LengthIamgeData+ReserveOffset);
		ReadDataBuf = (iU16 *)DataEntry;

		switch( Angle )
		{
			case 0:
				// ��A(x,y)��ת0����Ӧ�����:
				// B(x+ SrcMoveX -inSrcRect->X, y+ SrcMoveY  -inSrcRect->Y )
				// ��Ϊ:B(y+OffSetX,x+OffSetY);
				OffSetX = SrcMoveX - SrcRect->X ;
				OffSetY = SrcMoveY - SrcRect->Y ;
				break;
			case 90:
				// ��A(x,y)��ת90����Ӧ�����:
				// B(SrcRect->Y - SrcMoveY + RotateCenterY + RotateCenterX-y, SrcMoveX - SrcRect->X + RotateCenterY - RotateCenterX+x)
				// ��Ϊ:B(y+OffSetX,x+OffSetY);
				OffSetY = SrcMoveX - SrcRect->X + RotateCenterY - RotateCenterX;
				OffSetX = SrcRect->Y - SrcMoveY + RotateCenterY + RotateCenterX;
				break;
			case 180:	
				// ��A(x,y)��ת180����Ӧ�����:
				// B(2*RotateCenterX+SrcRect->X-SrcMoveX-x,)
				// ��Ϊ:B(OffSetX-x,OffSetY-y,2*RotateCenterY+SrcRect->Y-SrcMoveY);
				OffSetY = 2*RotateCenterY+SrcRect->Y-SrcMoveY;
				OffSetX = 2*RotateCenterX+SrcRect->X-SrcMoveX;
				break;
			case 270:	
				// ��A(x,y)��ת270����Ӧ�����:
				// B(RotateCenterX - RotateCenterY  + SrcMoveY - SrcRect->Y +y, RotateCenterY + RotateCenterX - SrcMoveX + SrcRect->X - x )
				// ��Ϊ:B(OffSetX+x,OffSetY+y);
				OffSetY = RotateCenterY + RotateCenterX - SrcMoveX + SrcRect->X;
				OffSetX = RotateCenterX - RotateCenterY  + SrcMoveY - SrcRect->Y ;
				break;
			default:
				iLog(" KIT - G2 : angle %d", Angle);
				return iFALSE;
		}

		for (CurrAlphaNum=0; CurrAlphaNum<TotalAlphaNumber; CurrAlphaNum++)
		{
			SrcAlphaX   = ReadDataBuf[0];
			SrcAlphaY   = ReadDataBuf[1];
			AlphaNumber	  = (iALPHA)ReadDataBuf[2];
			AlphaColor    = ReadDataBuf[3];
			ReadDataBuf   += 4;

			if (SrcAlphaX < SrcStartX || SrcAlphaX >= SrcEndX ||
				SrcAlphaY < SrcStartY || SrcAlphaY >= SrcEndY )
			{
				continue;
			}


			switch (Angle)
			{
				case 0:
					DesAlphaX = OffSetX + SrcAlphaX;
					DesAlphaY = OffSetY + SrcAlphaY;
					break;
				case 90:
					DesAlphaX = OffSetX - SrcAlphaY;
					DesAlphaY = OffSetY + SrcAlphaX;
					break;
				case 180:
					DesAlphaX = OffSetX - SrcAlphaX;
					DesAlphaY = OffSetY - SrcAlphaY;

					break;
				case 270:
					DesAlphaX = OffSetX + SrcAlphaY;
					DesAlphaY = OffSetY - SrcAlphaX;
					break;
				default:
					if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM  && ImageDataEntry )
					{
						i51AdeOsFree(ImageDataEntry);
						ImageDataEntry = iNULL;
					}
					return iFALSE;
					break;
			}

			if (	DesAlphaX < DesStartX || DesAlphaX >= DesEndX 
			||	DesAlphaY < DesStartY || DesAlphaY >= DesEndY )
			{
				continue;
			}
			LcdBuffPos = ScreenWidth*DesAlphaY+ DesAlphaX;
			LcdBuffer[LcdBuffPos] = i51KitG2GetNewColorEx(LcdBuffer[LcdBuffPos], AlphaColor, AlphaNumber);
		}
	}

	if( iFALSE==ImageIndexTable[ImageID].FlagCopyRAM  && ImageDataEntry )
	{
		i51AdeOsFree(ImageDataEntry);
		ImageDataEntry = iNULL;
	}

	return iTRUE;

}
#endif

iKIT iBOOL   i51KitG2RotateImage   ( iHANDLE ContainerHandle, iU32 ImageID , iRECT* DesRect , iRECT* SrcRect, 
	iS32 SrcMoveX , iS32 SrcMoveY, iS32 RotateCenterX , iS32 RotateCenterY , iU32 RotateType, iS32 Angle )
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
    //RotateCenterX	ͼƬ��ת����X����
    //RotateCenterY	ͼƬ��ת����Y����
    //RotateType	��ת����
    //Angle	��ת�Ƕ�
    
    // notes : ��˳ʱ�뷽����תһ���ǶȺ����ImageContainer�����ڱ��ΪImageID��ͼ��
    // SrcRect Ϊ��ʱ�ڵ�(DesRect->X, DesRect->Y)����ʼ��������ͼƬ
    // DesRect Ϊ������ƴ���Ļ���Ͻǿ�ʼ

#define ROTATE_MAX_GIRTH_HALF 600

    // ͼƬ������ַ���
    i51ImageContainer * ImageContainer = iNULL;
    // ͼƬ�������ַ
    i51ImageIndex * ImageIndexTable = iNULL;
    // �����ļ�ָ���ַ
    iFILE File = iNULL;
     // ͼƬ���뷽ʽ
    iU8   DecodeType = 0;
    iU16 CurrDrawID;
    iRECT * VPActualRect = iNULL;
    iRECT ImageRect;
    iRECT DrawDesRect;
    iRECT DrawSrcRect;
    iU8 Ret = iFALSE;
    iS16 * GirthPoint = iNULL;

 #if i51KITG2_DEBUG
    iLog( "G2RotateImage start , %x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
    ,ContainerHandle,ImageID,DesRect,SrcRect,SrcMoveX,SrcMoveY,RotateCenterX,RotateCenterY,RotateType,Angle);
#endif

    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = (i51ImageContainer * )ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL == ImageIndexTable || iNULL==File)  return iFALSE;
	
    if(ImageIndexTable[ImageID].FlagImageExist==0 ) return iFALSE;

    DecodeType = ImageIndexTable[ImageID].DecodeType;
    if(DecodeType!=I51KITG2_DECODE_LINEAR)
    {
    	iLog("�ļ���ʽ��������������ͼƬ�ļ�!");
    	return iFALSE;
    }


    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_PICTURE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        if( SrcRect )
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = SrcRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = SrcRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = SrcRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = SrcRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.X = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Y = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Width = ImageIndexTable[ImageID].ImageWidth;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcRect.Height = ImageIndexTable[ImageID].ImageHeight;
        }
		
        if( DesRect )		
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X = DesRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y = DesRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = DesRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = DesRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.X =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Y =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Width = ScreenWidth;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DesRect.Height = ScreenHeight;
        }
		
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DrawDirection = I51KITG2_DRAW_IAMGE_ROTATE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ImageHandle = ImageID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.ContainerHandle = ContainerHandle;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.RotateCenterX = RotateCenterX;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.RotateCenterY = RotateCenterY;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.RotateType = RotateType;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcMoveX = SrcMoveX;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.SrcMoveY = SrcMoveY;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Image.DarwAngle = Angle;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    if(i51UseVirtualPanel) VPActualRect = &(i51CurrentVpRender->VPActualRect);
	
    if( iNULL==SrcRect )
    {
        DrawSrcRect.X = 0;
        DrawSrcRect.Y = 0;
        DrawSrcRect.Width = ImageIndexTable[ImageID].ImageWidth;
        DrawSrcRect.Height = ImageIndexTable[ImageID].ImageHeight;
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
        if( i51UseVirtualPanel )
        {
            DrawDesRect.X = VPActualRect->X;
            DrawDesRect.Y = VPActualRect->Y;
            DrawDesRect.Width = VPActualRect->Width;
            DrawDesRect.Height = VPActualRect->Height;
        }
        else
        {
            DrawDesRect.X = 0;
            DrawDesRect.Y = 0;
            DrawDesRect.Width = ScreenWidth;
            DrawDesRect.Height = ScreenHeight;
        }
    }
    else
    {
        DrawDesRect.X = DesRect->X;
        DrawDesRect.Y = DesRect->Y;
        DrawDesRect.Width = DesRect->Width;
        DrawDesRect.Height = DesRect->Height;
        if( i51UseVirtualPanel )
        {
            if( (DrawDesRect.X+DrawDesRect.Width)>(VPActualRect->Width))
            {
                DrawDesRect.Width = VPActualRect->Width - DrawDesRect.X;
            }
			
            if( (DrawDesRect.Y+DrawDesRect.Height)>(VPActualRect->Height))
            {
                DrawDesRect.Height= VPActualRect->Height- DrawDesRect.Y;
            }
			
            DrawDesRect.X += VPActualRect->X;
            DrawDesRect.Y += VPActualRect->Y;
        }
    }
    

    ImageRect.X = 0;
    ImageRect.Y = 0;
    ImageRect.Width= ImageIndexTable[ImageID].ImageWidth;
    ImageRect.Height= ImageIndexTable[ImageID].ImageHeight;

    if(DrawSrcRect.X<0) { DrawSrcRect.Width+= DrawSrcRect.X; DrawSrcRect.X = 0;}
    else if( DrawSrcRect.X>=ImageRect.Width) return iTRUE;
    if( (DrawSrcRect.X+DrawSrcRect.Width) > ImageRect.Width ) DrawSrcRect.Width = ImageRect.Width -DrawSrcRect.X;

    if(DrawSrcRect.Y<0) { DrawSrcRect.Height+= DrawSrcRect.Y; DrawSrcRect.Y = 0;}
    else if( DrawSrcRect.Y>=ImageRect.Height) return iTRUE;
    if( (DrawSrcRect.Y+DrawSrcRect.Height) > ImageRect.Height) DrawSrcRect.Height= ImageRect.Height-DrawSrcRect.Y;

    if(DrawDesRect.X<0) { DrawDesRect.Width+= DrawDesRect.X; DrawDesRect.X = 0;}
    else if( DrawDesRect.X>=ScreenWidth) return iTRUE;
    if( (DrawDesRect.X+DrawDesRect.Width) > ScreenWidth ) DrawDesRect.Width = ScreenWidth -DrawDesRect.X;
		
    if(DrawDesRect.Y<0) { DrawDesRect.Height+= DrawDesRect.Y; DrawDesRect.Y = 0;}
    else if( DrawDesRect.Y>=ScreenHeight) return iTRUE;
    if( (DrawDesRect.Y+DrawDesRect.Height) > ScreenHeight) DrawDesRect.Height= ScreenHeight-DrawDesRect.Y;

    if( DrawSrcRect.Width<=0 || DrawSrcRect.Height<=0 || DrawDesRect.Width<=0 || DrawDesRect.Height<=0 )
    {
    	return iTRUE;
    }
		
    DecodeType = ImageIndexTable[ImageID].DecodeType;

	//�Ƕ�ת����0~360
	Angle = Angle%360;
	if( Angle<0 ) Angle += 360;
	
//	while ( Angle < 0 ){Angle += 360;}
//	while ( Angle >= 360 ){Angle -= 360;}

#if ROTATE_SPECIAE
	if ((Angle%90)==0 )
	{
		return i51KitG2RotateImageEx1(ContainerHandle, ImageID , &DrawDesRect , &DrawSrcRect, 
			SrcMoveX , SrcMoveY, RotateCenterX , RotateCenterY , RotateType, Angle );
	}
#endif

    if( (DrawSrcRect.Height+DrawSrcRect.Width)>=ROTATE_MAX_GIRTH_HALF  )   return iFALSE;
	
    GirthPoint = (iS16*)(i51AdeOsMalloc((DrawSrcRect.Height+DrawSrcRect.Width)*4,__FILE__ , __LINE__ ));

    if(iNULL==GirthPoint) 
    {
        iLog("KIT - G2 : malloc rotate");
        return iFALSE;		
    }
	
    if( iNULL==i51KitG2GetGirthPoints(DrawSrcRect, DrawDesRect,(iS16(*)[2])GirthPoint, SrcMoveX, SrcMoveY, RotateCenterX, RotateCenterY,RotateType, Angle))
    {
        i51AdeOsFree(GirthPoint);
        GirthPoint = iNULL;
        return iTRUE;
    }

    if( ImageIndexTable[ImageID].FlagCopyRAM )
    {
            // ����ѹ�����뷽ʽ
            // ��ͼ��������Ϣ�������Ƚ������Ա��룬���ݸ�ʽ���£�
            // x,y,w,RGB���ݣ�����x��y�Ƿ�͸��ɫ��������꣬w�������ж��ٸ���͸��ɫ��x��y�ֱ�ռ��1.5�ֽڣ�wռ�����ֽڡ�
            Ret = RotateRamLinearImage(ContainerHandle, ImageID, &(DrawDesRect),  &(DrawSrcRect), (iS16(*)[2])GirthPoint);
    }
    else
    {  
            // ��ʾ����ѹ������ͼƬ
            // ��ͼ��������Ϣ�������Ƚ������Ա��룬���ݸ�ʽ���£�
            // x,y,w,RGB���ݣ�����x��y�Ƿ�͸��ɫ��������꣬w�������ж��ٸ���͸��ɫ��x��y�ֱ�ռ��1.5�ֽڣ�wռ�����ֽڡ�
            Ret = RotateRamLinearImage(ContainerHandle, ImageID, &(DrawDesRect),  &(DrawSrcRect), (iS16(*)[2])GirthPoint);
    }
	
#undef ROTATE_MAX_GIRTH_HALF
    i51AdeOsFree(GirthPoint);
    GirthPoint = iNULL;

 #if i51KITG2_DEBUG
    iLog( "G2RotateImage start , %x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
    ,ContainerHandle,ImageID,DesRect,SrcRect,SrcMoveX,SrcMoveY,RotateCenterX,RotateCenterY,RotateType,Angle);
#endif

    return Ret;

}


iKIT iBOOL i51KitG2ImageFillRect(iHANDLE ContainerHandle,iU32 ImageID , iRECT* DesRect , iRECT* SrcRect )
{
    // author : Otto
    // since : 2011.11.7
    // (C) 2011 , PKIG Tech. Co., Ltd.

    //  iHANDLE ImageContainer �������ļ�ָ��
    //  iU16 ImageID  ��������ͼƬ�ļ�ID
    //  iRECT* DesRect :   ����Ŀ�ĵ�����
    //  iRECT* SrcRect :    ����ͼƬԭʼ����
    
    // notes : ����ImageContainer�����ڱ��ΪImageID��ͼ��
    // SrcRect Ϊ��ʱ�ڵ�(DesRect->X, DesRect->Y)����ʼ��������ͼƬ
    // DesRect Ϊ������ƴ���Ļ���Ͻǿ�ʼ

    // ͼƬ������ַ���
    i51ImageContainer * ImageContainer = iNULL;
    // ͼƬ�������ַ
    i51ImageIndex * ImageIndexTable = iNULL;
    // �����ļ�ָ���ַ
    iFILE File = iNULL;
     // ͼƬ���뷽ʽ
    iU8   DecodeType = 0;
//    iRECT DrawSrcRect;
    iRECT CurrFillRect;
    iU16 CurrDrawID;
    iS32 ImageWidth = 0;
    iS32 ImageHeight = 0;	

     // ���Ƶ���Ļ�ϵ�����
    iS16 DesStartX = 0;
    iS16 DesStartY = 0;
    iS16 DesEndX = 0;
    iS16 DesEndY = 0;
    iS16 DesWidth = 0;
    iS16 DesHeight = 0;

     // ����ͼƬ������
    iS16 SrcStartX = 0;
    iS16 SrcStartY = 0;
    iS16 SrcWidth = 0;
    iS16 SrcHeight = 0;

    // VP��λ��RECT
    iRECT * VPActualRect = iNULL;
    // VP��Ҫ���������RECT
    iRECT * VPDrawRect = iNULL;

 #if i51KITG2_DEBUG
    iLog( "G2ImageFillRect start , %x,%x,%x,%x"
    ,ContainerHandle,ImageID,DesRect,SrcRect);
#endif

    if( iNULL== ContainerHandle) return iFALSE;
    ImageContainer = (i51ImageContainer *)ContainerHandle;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);
    File = ImageContainer->FileHandle;
    if( iNULL == ImageIndexTable || iNULL==File)  return iFALSE;

    if(ImageIndexTable[ImageID].FlagImageExist==0 ) return iFALSE;
	
    DecodeType = ImageIndexTable[ImageID].DecodeType;
    ImageWidth = ImageIndexTable[ImageID].ImageWidth;
    ImageHeight = ImageIndexTable[ImageID].ImageHeight;

    if(DecodeType!=I51KITG2_DECODE_LINEAR)
    {
    	iLog("�ļ���ʽ��������������ͼƬ�ļ�!");
    	return iFALSE;
    }

    if(i51UseVirtualPanel)
    {
        VPActualRect = &(i51CurrentVpRender->VPActualRect);
        VPDrawRect = &(i51CurrentVpRender->VPDrawRect);
    }
	
	
    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;
      
        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_FILLIMAGE;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElemenAlpha = AlphaSys;
        if( SrcRect )
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.X = SrcRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.Y = SrcRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.Width = SrcRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.Height = SrcRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.X = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.Y = 0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.Width = ImageWidth;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.SrcRect.Height = ImageHeight;
        }
		
        if( DesRect )		
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.X = DesRect->X;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.Y = DesRect->Y;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.Width = DesRect->Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.Height = DesRect->Height;
        }
        else
        {
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.X =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.Y =0;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.Width = i51CurrentVpActivity->VPActualRect.Width;
            i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.DesRect.Height = i51CurrentVpActivity->VPActualRect.Height;
        }

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.ImageHandle = ImageID;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Fill.ContainerHandle = ContainerHandle;
        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;
        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    if( SrcRect )
    {
        SrcStartX= SrcRect->X;
        SrcStartY= SrcRect->Y;
        SrcWidth= SrcRect->Width;
        SrcHeight= SrcRect->Height;
    }
    else
    {
        SrcStartX= 0;
        SrcStartY= 0;
        SrcWidth= ImageWidth;
        SrcHeight= ImageHeight;
    }
    if( SrcStartX>=ImageWidth || SrcStartY>=ImageHeight) return iTRUE;
    if( SrcStartX< 0 ) { SrcWidth+= SrcStartX; SrcStartX= 0; }
    if( SrcStartY< 0 ) { SrcHeight+= SrcStartY; SrcStartY= 0; }

    if( i51UseVirtualPanel == iFALSE )
    {
        if( DesRect)
        {
            DesStartX = DesRect->X;
            DesStartY = DesRect->Y;
            DesWidth = DesRect->Width;
            DesHeight = DesRect->Height;
        }
        else
        {
            DesStartX = 0;
            DesStartY = 0;
            DesWidth = ScreenWidth;
            DesHeight = ScreenHeight;
        }
    }
    else
    {
        if( DesRect)
        {
            DesStartX = DesRect->X ;
            DesStartY = DesRect->Y ;
            DesWidth = DesRect->Width;
            DesHeight = DesRect->Height;
        }
        else
        {
            DesStartX = 0;
            DesStartY = 0;
            DesWidth = VPActualRect->Width;
            DesHeight = VPActualRect->Height;
        }
#if 0		
        // ����ʹ��VPʱ���Ƶ���Ļ����
        DesEndX = DesStartX + DesWidth;
        DesEndY = DesStartY + DesHeight;
        if( DesStartX<VPDrawRect->X )  DesStartX = VPDrawRect->X;
        if( DesStartY<VPDrawRect->Y ) DesStartY = VPDrawRect->Y;
        if( DesEndX>(VPDrawRect->X+VPDrawRect->Width) ) DesEndX = VPDrawRect->X+VPDrawRect->Width;
        if( DesEndY>(VPDrawRect->Y+VPDrawRect->Height) ) DesEndY = VPDrawRect->Y+VPDrawRect->Height;
        DesWidth = DesEndX - DesStartX;
        DesHeight = DesEndY - DesStartY;
#endif
    }	
#if 0
    if( DesStartX>=ScreenWidth|| DesStartY>=ScreenHeight ) return iTRUE;
    if( DesStartX < 0 ) { DesWidth += DesStartX; DesStartX= 0; }
    if( DesStartY < 0 ) { DesHeight += DesStartY; DesStartY= 0; }
    if( DesWidth<=0 || DesHeight<=0 ) return iTRUE;
#endif
    DesEndX = DesStartX + DesWidth;
    DesEndY = DesStartY + DesHeight;
	
    CurrFillRect.X = DesStartX;
    CurrFillRect.Y = DesStartY;
    CurrFillRect.Width= DesWidth;
    CurrFillRect.Height= DesHeight;     

    if( ImageIndexTable[ImageID].FlagCopyRAM )
    {
        while(1)
        {
            // ����ѹ�����뷽ʽ
            // ��ͼ��������Ϣ�������Ƚ������Ա��룬���ݸ�ʽ���£�
            // x,y,w,RGB���ݣ�����x��y�Ƿ�͸��ɫ��������꣬w�������ж��ٸ���͸��ɫ��x��y�ֱ�ռ��1.5�ֽڣ�wռ�����ֽڡ�
            DrawRAMLinearImage( ContainerHandle, ImageID,  &(CurrFillRect), SrcRect, I51KITG2_DRAW_IAMGE_NORMAL);
            CurrFillRect.X += SrcWidth;
            CurrFillRect.Width -= SrcWidth;
            if( (CurrFillRect.X >= DesEndX) || (CurrFillRect.X>=ScreenWidth) || (CurrFillRect.Width<=0) )
            {
                CurrFillRect.X = DesStartX;
                CurrFillRect.Y += SrcHeight;
                CurrFillRect.Width= DesWidth;
                CurrFillRect.Height -= SrcHeight;
            }
            if( (CurrFillRect.Y >= DesEndY) || (CurrFillRect.Y>=ScreenHeight) || ( CurrFillRect.Height<=0))
            {
                break;
            }
        }
    }
    else
    {  
        while(1)
        {
            // ��ʾ����ѹ������ͼƬ
            // ��ͼ��������Ϣ�������Ƚ������Ա��룬���ݸ�ʽ���£�
            // x,y,w,RGB���ݣ�����x��y�Ƿ�͸��ɫ��������꣬w�������ж��ٸ���͸��ɫ��x��y�ֱ�ռ��1.5�ֽڣ�wռ�����ֽڡ�
            DrawRAMLinearImage( ContainerHandle, ImageID, &(CurrFillRect),  SrcRect, I51KITG2_DRAW_IAMGE_NORMAL);
            CurrFillRect.X += SrcWidth;
            CurrFillRect.Width -= SrcWidth;
            if( (CurrFillRect.X >= DesEndX) || (CurrFillRect.X>=ScreenWidth) || (CurrFillRect.Width<=0) )
            {
                CurrFillRect.X = DesStartX;
                CurrFillRect.Y += SrcHeight;
                CurrFillRect.Width= DesWidth;
                CurrFillRect.Height -= SrcHeight;
            }
            if( (CurrFillRect.Y >= DesEndY) || (CurrFillRect.Y>=ScreenHeight) || ( CurrFillRect.Height<=0))
            {
                break;
            }
        }

    }
 #if i51KITG2_DEBUG
    iLog( "G2ImageFillRect end , %x,%x,%x,%x"
    ,ContainerHandle,ImageID,DesRect,SrcRect);
#endif

    return iTRUE;
	
}

iKIT iBOOL i51KitG2ScreenZoom( iRECT * DesRect, iRECT * SrcRect, iU32 ZoomRateX, iU32 ZoomRateY )
{
    // author : Otto
    // since : 2011.12.6
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iRECT DesRect: Ӱ�����ŵ���Ŀ������
    // iRECT SrcRect : Ӱ�����ŵ�ԭʼ����
    // iU32 ZoomRateX : Ӱ��X�������ŵı���
    // iU32 ZoomRateY : Ӱ��Y�������ŵı���
    
    // notes: ������Buffer��������
    
    iS16 * NewPosX = iNULL;
    iS16 * NewPosY = iNULL;
    iU32 iTemp =0;	
    iS32 SrcStartX = 0;
    iS32 SrcStartY = 0;
    iS32 SrcEndX = 0;
    iS32 SrcEndY = 0;
    iS32 DesStartX = 0;
    iS32 DesStartY = 0;
    iS32 DesEndX = 0;
    iS32 DesEndY = 0;
    iS32 SrcCurrX = 0;
    iS32 SrcCurrY = 0;
    iS32 DesCurrX = 0;
    iS32 DesCurrY = 0;

    iU32 NewLcdBuffer = 0;
    iU32 OldLcdBuffer = 0;
    iU16 * CurrScreenBuffer = iNULL;
	
    if(ZoomRateX==100&&ZoomRateY==100) return iTRUE;

    if(ZoomRateX==0||ZoomRateY==0) return iFALSE;
	
    if(DesRect)
    {
        DesStartX = DesRect->X;
        DesStartY = DesRect->Y;
        DesEndX = DesRect->Width + DesStartX;
        DesEndX = DesRect->Height + DesStartY;

        if( DesStartX<0 ) DesStartX = 0;
        if( DesStartY<0 ) DesStartY = 0;
        if( DesEndX>ScreenWidth ) DesEndX = ScreenWidth;
        if( DesStartY>ScreenHeight ) DesEndY = ScreenHeight;
    	
        if( DesStartX>=ScreenWidth ) return iTRUE;
        if( DesStartY>=ScreenHeight) return iTRUE;
        if( DesEndX<0 ) return iTRUE;
        if( DesEndY<0 ) return iTRUE;
	
    }
    else
    {
        DesStartX = 0;
        DesStartY = 0;
        DesEndX = ScreenWidth;
        DesEndY = ScreenHeight;
    }

    if( SrcRect )
    {
        SrcStartX = SrcRect->X;
        SrcStartY = SrcRect->Y;
        SrcEndX = SrcRect->Width + SrcStartX;
        SrcEndY = SrcRect->Height + SrcStartY;

        if( SrcStartX<0 ) SrcStartX = 0;
        if( SrcStartY<0 ) SrcStartY = 0;
        if( SrcEndX >ScreenWidth ) SrcEndX = ScreenWidth;
        if( SrcEndY >ScreenHeight ) SrcEndY = ScreenHeight;
    	
        if( SrcStartX>=ScreenWidth ) return iTRUE;
        if( SrcStartY>=ScreenHeight) return iTRUE;
        if( SrcEndX <0 ) return iTRUE;
        if( SrcEndY <0 ) return iTRUE;

    }
    else
    {
        SrcStartX = 0;
        SrcStartY = 0;
        SrcEndX = ScreenWidth;
        SrcEndY = ScreenHeight;
    }

    NewPosX = (iS16*) i51AdeOsMalloc((ScreenWidth<<1),   __FILE__ , __LINE__ );
    if( iNULL==NewPosX ) goto ERROR;
    for( iTemp=0; iTemp<ScreenWidth; iTemp++ )
    {
        NewPosX[iTemp] = (iTemp*100)/ZoomRateX;
    }

    NewPosY = (iS16*) i51AdeOsMalloc((ScreenHeight<<1),   __FILE__ , __LINE__ );
    if( iNULL==NewPosX ) goto ERROR;
    for( iTemp=0; iTemp<ScreenHeight; iTemp++ )
    {
        NewPosY[iTemp] = (iTemp*100)/ZoomRateY;
    }

    CurrScreenBuffer = (iU16*) i51AdeOsMalloc(((ScreenHeight*ScreenWidth)<<1),   __FILE__ , __LINE__ );
    if( iNULL==CurrScreenBuffer ) goto ERROR;
    i51AdeStdMemcpy16(CurrScreenBuffer,LcdBuffer,((ScreenHeight*ScreenWidth)<<1));
	
    NewLcdBuffer = DesStartY*ScreenWidth + DesStartY;

    for( DesCurrY=DesStartY; DesCurrY<DesEndY; DesCurrY++ )
    {
        SrcCurrY = NewPosY[DesCurrY];
        if( SrcCurrY>=SrcEndY ) break;
		
        for( DesCurrX=DesStartX,iTemp = 0; DesCurrX<DesEndX;DesCurrX++,iTemp++)
        {
            SrcCurrX = NewPosX[DesCurrX];
            if( SrcCurrX>=SrcEndX ) break;
            OldLcdBuffer = SrcCurrY*ScreenWidth + SrcCurrX;
			
            LcdBuffer[NewLcdBuffer+iTemp] = CurrScreenBuffer[OldLcdBuffer];
        }
		
        NewLcdBuffer += ScreenWidth;
    }

    i51AdeOsFree(CurrScreenBuffer);
    i51AdeOsFree(NewPosX);
    i51AdeOsFree(NewPosY);

    CurrScreenBuffer = iNULL;
    NewPosX = iNULL;
    NewPosY = iNULL;

    return iTRUE;

ERROR:
    iLog("KIT - G2 : Zoom malloc");

    i51AdeOsFree(CurrScreenBuffer);
    i51AdeOsFree(NewPosX);
    i51AdeOsFree(NewPosY);

    CurrScreenBuffer = iNULL;
    NewPosX = iNULL;
    NewPosY = iNULL;

    return iFALSE;
	
}
	
static iBOOL i51KitG2FontSetUse ( iFILE ReFile, iU32 TypeheadID , i51KitG2Font * CurrTypehead ) 
{
    // author : Otto
    // since : 2011.10.11
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE ReFile: �ֿ��ļ����ڵ���Դ�ļ����
    // iU32 TypeheadID : �ֿ��ļ�����Դ�ļ��е�ID

    // �ļ�������������ֵ
    iU8   FlagReturn = iFALSE;
    // ����ļ���ʽ����ʱ����
    iU8   FlagFile[4] = {0};
    // ���ļ��ж�ȡ������ʵ�ʳ���
    iU32 LengthReadData = 0;
    //  ȡ��ģ����	
    iU8  FontModelType = 0;
    //  ��ģ���	
    iU8  FontModelWidth = 0;	
    //  ��ģ�߶�
    iU8  FontModelHeight = 0;	
    //  �����ָ���
    iU16 FontNormalWordsNum = 0;
    //  ��������ģƫ����
    iU32 FontNormalWordsModelOffset = 0;	
    //  ��������ģƫ����
    iU32 FontAllWordsModelOffset = 0;	
    // �ֿ��ļ�����Դ�ļ��еĿ�ʼλ��
    iU32 TypeheadOffset = 0;
	int Temp = 0;

//iLog("Start set use");
//    i51KitStdResSetContext(ReFile);
    if(iNULL==ReFile) return iFALSE;

	for( Temp=0; Temp<MAX_TYPEHEAD_NUMBER; Temp++ )
	{
        if( (ReFile == (i51KitG2FontManage[Temp].FontFileHandle)) 
		&& (TypeheadID== (i51KitG2FontManage[Temp].TypeheadID))  )
        {
			CurrTypehead->NormalWordsEntry = i51KitG2FontManage[Temp].NormalWordsEntry;
			CurrTypehead->FontFileHandle = i51KitG2FontManage[Temp].FontFileHandle;
			CurrTypehead->TypeheadID= i51KitG2FontManage[Temp].TypeheadID;
			CurrTypehead->WordHeight = i51KitG2FontManage[Temp].WordHeight;
			CurrTypehead->WordWidth = i51KitG2FontManage[Temp].WordWidth;
			CurrTypehead->TypeheadType = i51KitG2FontManage[Temp].TypeheadType;
			CurrTypehead->NormalUseWordsNum = i51KitG2FontManage[Temp].NormalUseWordsNum;	
			CurrTypehead->NormalWordsOffset = i51KitG2FontManage[Temp].NormalWordsOffset;
			CurrTypehead->AllWordsOffset = i51KitG2FontManage[Temp].AllWordsOffset;
			CurrTypehead->TypeheadOffset = i51KitG2FontManage[Temp].TypeheadOffset;
//			iLog("font in cache");
        	return iTRUE;
        }
		
	}
	
    TypeheadOffset = i51KitStdResGetContentEntry(ReFile, TypeheadID);
    FlagReturn = i51AdeOsFileSetSeek( ReFile, TypeheadOffset, i51_OS_FL_SEEK_HEAD );
    if( iFALSE == FlagReturn ) goto ERROR;                      

    // ǰ�ĸ��ֽ���CRCУ����
    FlagReturn = i51AdeOsFileSetSeek( ReFile, TypeheadOffset+i51KITG2_FONT_FILETYPE, i51_OS_FL_SEEK_HEAD );
    if( iFALSE == FlagReturn ) goto ERROR;                      

    // ���򿪵����������ŵ��ļ��Ƿ�Ϊ"i51F"��ʽ
    FlagReturn = i51AdeOsFileRead( ReFile, FlagFile, 4, &LengthReadData);   
    if( (iFALSE == FlagReturn) || (LengthReadData!=4) 
    || (FlagFile[0]!='i') || (FlagFile[1]!='5') || (FlagFile[2]!='1') ||(FlagFile[3]!='F') )
    {
        iLog("KIT - G2 : file type font set use");
        goto ERROR;
    }

    FlagReturn = i51AdeOsFileSetSeek( ReFile, TypeheadOffset+i51KITG2_FONT_MODELTYPE, i51_OS_FL_SEEK_HEAD );
    if( iFALSE == FlagReturn ) goto ERROR;          

    FlagReturn = i51AdeOsFileRead( ReFile, &FontModelType, 1, &LengthReadData);   
    if( (iFALSE == FlagReturn) || (LengthReadData!=1) )
    {
       goto ERROR;
    }

    FlagReturn = i51AdeOsFileRead( ReFile, &FontModelWidth, 1, &LengthReadData);   
    if( (iFALSE == FlagReturn) || (LengthReadData!=1) )
    {
       goto ERROR;
    }

    FlagReturn = i51AdeOsFileRead( ReFile, &FontModelHeight, 1, &LengthReadData);   
    if( (iFALSE == FlagReturn) || (LengthReadData!=1) )
    {
       goto ERROR;
    }

    FlagReturn = i51AdeOsFileReadInt16( ReFile, &FontNormalWordsNum);   
    if( (iFALSE == FlagReturn) )       goto ERROR;
 
    FlagReturn = i51AdeOsFileReadInt32( ReFile, &FontNormalWordsModelOffset);   
    if( (iFALSE == FlagReturn))      goto ERROR;

    FlagReturn = i51AdeOsFileReadInt32( ReFile, &FontAllWordsModelOffset);   
    if( (iFALSE == FlagReturn))       goto ERROR;

    CurrTypehead->NormalWordsEntry = iNULL;
    CurrTypehead->FontFileHandle = ReFile;
    CurrTypehead->TypeheadID= TypeheadID;
    CurrTypehead->WordHeight = FontModelHeight;
    CurrTypehead->WordWidth = FontModelWidth;
    CurrTypehead->TypeheadType = FontModelType;
    CurrTypehead->NormalUseWordsNum = FontNormalWordsNum;	
    CurrTypehead->NormalWordsOffset = FontNormalWordsModelOffset+TypeheadOffset;
    CurrTypehead->AllWordsOffset = FontAllWordsModelOffset + TypeheadOffset;
    CurrTypehead->TypeheadOffset = TypeheadOffset;
//	iLog("font in file");
    return iTRUE;

ERROR:
    iLog("KIT - G2 : RW font set use");
    return iFALSE;
	
}

iKIT iBOOL i51KitG2FontCachEnable ( iFILE ReFile, iU32 TypeheadID  ) 
{
    // author : Otto
    // since : 2011.10.11
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE ReFile : �ֿ�������Դ�ļ����
    // iU32 TypeheadID  :�ֿ�����Դ�ļ��е�ID
    
    // notes: �ѳ����ּ��ؽ��ڴ�	

    // �򿪵��ļ����	
    iFILE FileHandle = iNULL;
    // �ļ�������������ֵ
    iU8   FlagReturn = iFALSE;
    // ���ļ��ж�ȡ������ʵ�ʳ���
    iU32 LengthReadData = 0;
    //  ��ģ���	
    iU8  FontModelWidth = 0;	
    //  ��ģ�߶�
    iU8  FontModelHeight = 0;	
    //  �����ָ���
    iS16 FontNormalWordsNum = 0;
    //  ��������ģƫ����
    iS32 FontNormalWordsModelOffset = 0;	
    //  ��Ҫ���ؽ��ڴ��������
    iU32 NeedReadData = 0;	
    iS16 i = 0;
    // �����ֿ����	
    i51KitFontWord * NormalWordStock = iNULL;
    iU16 TempKeywords = 0;
    iU32 Temp = 0;
    iU32 UseTypeheadId = 0;
    	
 #if i51KITG2_DEBUG
    iLog( "G2FontCachEnable start , %x,%x"
    ,ReFile,TypeheadID);
#endif

    if( iNULL==ReFile ) return iFALSE;
	
    for( Temp=0; Temp<MAX_TYPEHEAD_NUMBER; Temp++ )
    {
        if( (ReFile == (i51KitG2FontManage[Temp].FontFileHandle)) 
		&& (TypeheadID== (i51KitG2FontManage[Temp].TypeheadID))  )
        {
        	return iTRUE;
        }
    }
	
    for( Temp=0; Temp<MAX_TYPEHEAD_NUMBER; Temp++ )
    {
        if( iNULL == (i51KitG2FontManage[Temp].FontFileHandle)  )
        {
            UseTypeheadId = Temp;
            break;
        }
    }
	
    if( Temp==MAX_TYPEHEAD_NUMBER)
    {
        return iFALSE;
    }

    if( i51KitG2FontManage[UseTypeheadId].NormalWordsEntry ) return iTRUE;

    i51KitG2FontSetUse(ReFile, TypeheadID, &i51KitG2FontManage[UseTypeheadId]);
    FileHandle = i51KitG2FontManage[UseTypeheadId].FontFileHandle;
    FontModelWidth =  i51KitG2FontManage[UseTypeheadId].WordWidth;
    FontModelHeight =  i51KitG2FontManage[UseTypeheadId].WordHeight;
    FontNormalWordsNum =  i51KitG2FontManage[UseTypeheadId].NormalUseWordsNum;
    FontNormalWordsModelOffset =  i51KitG2FontManage[UseTypeheadId].NormalWordsOffset;

    NeedReadData = (sizeof(i51KitFontWord) )* FontNormalWordsNum;

    FlagReturn = i51AdeOsFileSetSeek( FileHandle, FontNormalWordsModelOffset, i51_OS_FL_SEEK_HEAD );
    if( iFALSE == FlagReturn ) goto ERROR;                      

    NormalWordStock= (i51KitFontWord *)i51AdeOsMalloc( NeedReadData,  __FILE__ , __LINE__ );
    if( !NormalWordStock )
    {
        iLog("KIT - G2 : malloc font cach enable");
        return iFALSE;
    }

    NeedReadData = FontModelWidth*FontModelHeight+2;
    for( i=0; i<FontNormalWordsNum;i++)
    {
        FlagReturn = i51AdeOsFileRead( FileHandle, &NormalWordStock[i], NeedReadData, &LengthReadData);   
        if( (iFALSE == FlagReturn) || (LengthReadData!=NeedReadData) )
        {
           i51AdeOsFree(NormalWordStock);
           NormalWordStock = iNULL;
           goto ERROR;
        }
        if(i51AdeCnfGetEndian())
        {
            TempKeywords = NormalWordStock[i].CharactersKeywords;
            NormalWordStock[i].CharactersKeywords = ((TempKeywords<<8)&0XFF00)|((TempKeywords>>8)&0XFF);
        }
    }
    i51KitG2FontManage[UseTypeheadId].NormalWordsEntry = (iU32)NormalWordStock;
	
 #if i51KITG2_DEBUG
    iLog( "G2FontCachEnable end , %x,%x"
    ,ReFile,TypeheadID);
#endif

    return iTRUE;

ERROR:
    i51AdeOsFree(NormalWordStock);
    NormalWordStock = iNULL;
    iLog("KIT - G2 : RW font cach enable");
    return iFALSE;
}

iKIT iBOOL i51KitG2FontCachDisable(  iFILE ReFile, iU32 TypeheadID  ) 
{
    // author : Otto
    // since : 2011.10.11
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE ReFile : �ֿ�������Դ�ļ����
    // iU32 TypeheadID  :�ֿ�����Դ�ļ��е�ID

    // notes: �����ֲ����ؽ��ڴ�

    iU32 Temp = 0;
    iU32 UseTypeheadId = 0;

 #if i51KITG2_DEBUG
    iLog( "G2FontCachDisable start , %x,%x"
    ,ReFile,TypeheadID);
#endif

    	
    for( Temp=0; Temp<MAX_TYPEHEAD_NUMBER; Temp++ )
    {
        if( i51KitG2FontManage[Temp].TypeheadID==TypeheadID && i51KitG2FontManage[Temp].FontFileHandle == ReFile )
        {
            UseTypeheadId = Temp;
            break;
        }
    }
    if( Temp==MAX_TYPEHEAD_NUMBER)
    {
        return iFALSE;
    }

    if(!i51KitG2FontManage[Temp].NormalWordsEntry) return iTRUE;
	
    if(i51AdeOsFree( (void *)i51KitG2FontManage[Temp].NormalWordsEntry))
    {
        i51KitG2FontManage[Temp].NormalWordsEntry = iNULL;

		i51KitG2FontManage[Temp].FontFileHandle = 0;
        return iTRUE;
    }

 #if i51KITG2_DEBUG
    iLog( "G2FontCachDisable end , %x,%x"
    ,ReFile,TypeheadID);
#endif

    return iFALSE;
	
}

iKIT iBOOL i51KitG2FontGetDemension ( iFILE ReFile, iU32 TypeheadID, iU32 WordsNum, iU32 RowWords,
	iU8 WordsDirection , iS16 RowGap, iS16 ColumnGap , iU32 * DrawWidth, iU32 * DrawHeight) 
{
    // author : Otto
    // since : 2011.10.11
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE ReFile : �ֿ�������Դ�ļ����
    // iU32 TypeheadID:�ֿ�����Դ�ļ��е�ID
    // iU32 WordsNum: ��Ҫ���Ƶ����ָ���
    // iU32 RowWords: ÿһ�л��Ƶ����ָ���
    // iU8 WordsDirection : �������ֵķ���
    // iS16 RowGap : �������ֵ��о�
    // iS16 ColumnGap : �������ֵ��о�
    // iU32 * DrawWidth : �������ֻ���������
    // iU32 * DrawHeight : �������ֻ�������߶�
    
    // notes: ����һ�����ֻ�����Ҫ�ľ�������ĳ��Ϳ�
    
    //  ��ģ�߶�
    iU8  FontModelHeight = 0;	
    //  ��Ҫ���Ƶ������ж�����	
    iU8  DrawRows = 0;
    iU32 TempDrawWidth = 0;
    iU32 TempDrawHeight = 0;	

//    iU32 Temp = 0;

	i51KitG2Font CurrTypehead;
    	
 #if i51KITG2_DEBUG
    iLog( "G2FontGetDemension start , %x,%x,%x,%x,%x,%x,%x,%x,%x"
    ,ReFile,TypeheadID,WordsNum,RowWords,WordsDirection,RowGap,ColumnGap,DrawWidth,DrawHeight);
#endif

	i51KitG2FontSetUse( ReFile, TypeheadID, &CurrTypehead);

    FontModelHeight = CurrTypehead.WordHeight;


    if( RowWords<WordsNum ) RowWords = WordsNum;

    if(RowWords==0) return iFALSE;	
    DrawRows = 	WordsNum/RowWords;

    TempDrawWidth = RowWords * ( FontModelHeight + ColumnGap );
    TempDrawHeight = DrawRows * ( FontModelHeight + RowGap );

    if( WordsDirection<i51KITG2_FONT_WORDS_ROTATE_90)
    { 
        // ���Ǻ���	
        *DrawWidth = TempDrawWidth;
        *DrawHeight = TempDrawHeight;
    }
    else
    {
        // ��������	
        *DrawHeight= TempDrawWidth;
        *DrawWidth = TempDrawHeight;
    }

 #if i51KITG2_DEBUG
    iLog( "G2FontGetDemension end , %x,%x,%x,%x,%x,%x,%x,%x,%x"
    ,ReFile,TypeheadID,WordsNum,RowWords,WordsDirection,RowGap,ColumnGap,DrawWidth,DrawHeight);
#endif
    return iTRUE;
	
}

iKIT iBOOL i51KitG2FontDraw (iFILE Handle, iU32 TypeheadID, void* text ,  iU16 WordsNum, 
	iRECT* DrawRect , iRECT * SrcRect, iU16 DrawDirect, iS16 WordsRowGap,   iS16 WordsColumnGap,  
	iCOLOR fgcolor , iCOLOR bgcolor, iU16 Effect )
{
    // author : Otto
    // since : 2011.10.11
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iFILE ReFile : �ֿ�������Դ�ļ����
    // iU32 TypeheadID:�ֿ�����Դ�ļ��е�ID
    // void* text : �ַ���ָ��
    // iU32 WordsNum : �������ֵĸ��������Ϊ����ȫ������
    // iRECT* DrawRect : ���ֻ�����ʾ����
    // iRECT* SrcRect : ���ֻ��Ƶ�����
    // iU32 DrawDirect : �������ֵ����к����ֵ���ת����
    // iS16 WordsRowGap : �������ֵ��о�
    // iS16 WordsColumnGap : �������ֵ��о�
    // iCOLOR fgcolor : �������ֵ���ɫ
    // iCOLOR bgcolor : ������������ı���ɫ
    // iU32 Effect : �������ֵ���Ч�������Ƿ���Ҫ�»��ߡ��Ƿ񱳾����

    // notes: �����ض��Ĺ������һ�����ֵ���Ļ��

    // ��ǰ�������ֵĻ�����
    iU16 WordKey = 0;
    // ��ǰ���������ڳ����ֿ��еı��
    iU16 FindWordKey = 0;
    // ��ʱ����	
    iS32 Temp=0,Temp1=0, Temp2=0;
    // ��ǰ���Ƶ��ǵڼ�������	
    iS32  CurrWords = 0;
    // ��ǰ�������ֵ���ģ��ʼ��ַ
    iU8   * TypeMatrix = iNULL;
    // ȡ��ģ	��һ���ֽڴ��λ��
    iU8    CurrTypeMatrix1 = 0;
    iU8    CurrTypeMatrix2 = 0;

    // �ַ�������
    iU16 StrLength = 0;
    // �ַ���ָ��
    iU8 * Words = (iU8 *)text;
    // ����VPʱ�����ڴ汣���ַ���
    iU32  WordsHandle = 0;

    //  ��ģ���	
    iU8  TypeheadWidth = 0;	
    //  ��ģ�߶�
    iU8  TypeheadHeight = 0;	
    //  ����������ཻʱ��Ҫ���Ƶ���ģ�߶�
    iU8  TempTypeheadHeight = 0;	
    //  ����������ཻʱ��Ҫ���Ƶ���ģ����
    iU16  TempTypeheadMask = 0;	
    iU16  TempTypeheadMask1 = 0;	
    //  ����������ཻʱ��Ҫ���Ƶ���ģλ��λ��
    iU8  TempTypeheadMove = 0;	
    // �ַ����ݳ���
    iU8  DoubleTypeheadHeight = 0;	

    // ���ļ��ж�ȡ��һ���ַ���ģ����
    iU8  ReadWords[32] = {0};
    // ����һ���ַ���Ҫ���ļ��ж�����ģ���ݳ���
    iU32 NeedReadLongth = 0;
    // ʵ�ʴ��ļ��ж�ȡ�������ݳ���	
    iU32 LengthReadData = 0;
    // �����ַ����ֿ��ļ��ڵ�ƫ����	
    iU32 AllWordsOffset = 0;
    // ��ǰ��Ҫ���Ƶ��������ֿ��ڵ�ƫ����
    iU32 CurrWordsOffset = 0;
    // �ֿ��ļ�ָ��
    iFILE FontFileHandle = iNULL;
    // �ļ���������ֵ
    iU8   FlagReturn = iFALSE;
    // ���������Ƿ���Ҫ�»���
    iU32 NeedUnderLine  = iFALSE;
    // ���������Ƿ���Ҫ����ɫ
    iU32 NeedBKColor  = iFALSE;

    // ����������ʾ����
    iS32 DrawWordsStartX = 0;	
    iS32 DrawWordsStartY = 0;	
    iS32 DrawUnderlinesStartX = 0;	
    iS32 DrawUnderlinesStartY = 0;	
    iS32 DrawWidth = 0;	
    iS32 DrawHeight = 0;	
    // �������ֵ�ԭʼ����	
    iS32 SrcStartX = 0;
    iS32 SrcStartY = 0;
    iS32 SrcWidth = 0;
    iS32 SrcHeight = 0;	
    // ��ǰ�л��ƵĿ��	
    iS32 TempDrawWidth = 0;	
    // ���Ƶĸ߶�	
    iS32 TempDrawHeight = 0;	
    // ��ǰ��ԭʼ�Ŀ��	
    iS32 TempSrcWidth = 0;	
    // ԭʼ�ĸ߶�	
    iS32 TempSrcHeight = 0;	
    // һ�������ƺ��ָ���	
//    iS32 ChinaMaxColumn = 0;
    // Ӣ���ֿ��
    iS32 AscllWidth = 0;

    // �������ֵ�ǰ�е���Ļ��ʼλ��	
    iS32 LcdBufferTypeheadPos = 0;
    // ��������������ģ֮������
    iS16 LcdBufferTypeheadRowAdds = 0;	
    // �������ֵ�ǰ�е���Ļ��ʼλ��
    iS32 LcdBuffColumnPos = 0;
    // ���ƹ�������������֮�������
    iS32 LcdBuffColumnChinaAdds = 0;
    // ����ASCLL����������֮�������
    iS32 LcdBuffColumnAscllAdds = 0;
    // �������ֵ�ǰ�е���Ļ��ʼλ��
    iS32 LcdBuffRowPos = 0;
    // ������������֮�������
    iS32 LcdBuffRowAdds = 0;
	
    iU16 CurrDrawID;
    iRECT * VPActualRect = iNULL;
    iRECT VPDrawRect;
    // ��Ҫ���Ƶ��»�������	
    iS16 UnderLineNumber =0; 	
    // ��Ҫ���Ƶ��»��ߵĳ���	
//    iS16 UnderLineLength =0; 	
    // ���������Ҫ���Ƶ��»��ߵĳ���	
//    iS16 NormalUnderLineLength =0; 	
    // ���һ����Ҫ���Ƶ��»��ߵĳ���	
//    iS32 LastUnderLineLength =0; 	
    // ��Ҫ���Ƶ����ָ���
    iU32 DrawWordsNum = 0;
    // ���������»���LcdBuf����
    iS32 UnderLinesAdds = 0;
    // ���������м��	
    iS32 RowGap;
    // ���������ּ��	
    iS32 ColumnGap;
    iS32 LcdBufferPos = 0;
    // ���Ƶ��������Ƿ����ASCLL�ı��	
//    iU8 DrawIncludeAscll = iFALSE;
    // ��ģ��ÿ��λ��Ե�ǰ��ģ�е�ƫ����	
    iS16 PointAdd0 = 0;
    iS16 PointAdd1 = 0;
    iS16 PointAdd2 = 0;
    iS16 PointAdd3 = 0;
    iS16 PointAdd4 = 0;
    iS16 PointAdd5 = 0;
    iS16 PointAdd6 = 0;
    iS16 PointAdd7 = 0;
    iS16 PointAdd8 = 0;
    iS16 PointAdd9 = 0;
    iS16 PointAdd10 = 0;
    iS16 PointAdd11 = 0;
    iS16 PointAdd12 = 0;
    iS16 PointAdd13 = 0;
    iS16 PointAdd14 = 0;
    iS16 PointAdd15 = 0;

    iU8 TypeheadMask = 0XFF;
	
    // Ӣ������ģ
    iS32  WordsTypehead = iNULL;
    // ��ǰʹ�õ��ֿ��ļ����ֿ�������еı��	
//    iU32  UseTypeheadId = 0;
    // ��ǰʹ�õ��ֿ������	
    i51KitG2Font CurrTypehead;
    // ��ǰʹ�õ��ֿⳣ���ֹ�����	
    i51KitFontWord * NormalWordStock = iNULL;
    // Ӣ���ֵ�ǰ��ĸ��Ҫ�жϻ���
    iU8  LineFeed = iTRUE;
    // һ��Ӣ�����ɶ�����ĸ���
    iU8  LetterNum = 0;

    // ԭʼ���������������߿�Ȳ�
    iS32 MulLeftWidth = 0;	
    // ԭʼ��������������ұ߿�Ȳ�
    iS32 MulRightWidth  = 0;	
    // ԭʼ��������������ϱ߿�Ȳ�
    iS32 MulTopWidth  = 0;	
    // ԭʼ��������������±߿�Ȳ�
    iS32 MulBottomWidth  = 0;	
 #if i51KITG2_DEBUG
    iLog( "G2FontDraw start , %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
    ,Handle,TypeheadID,text,WordsNum,DrawRect,SrcRect,DrawDirect,WordsRowGap,WordsColumnGap,fgcolor,bgcolor,Effect);
#endif

    // �ַ���Ϊ��ֱ�ӷ���
    if( iNULL == text ) return iTRUE;

    // ���ֻ���Ϊ��ֱ�ӷ���
    if( iNULL == DrawRect ) return iFALSE;


    // ����������ϻ��ƣ�ʵ����ֻ�ǽ���һЩ��ֵ����
    if( i51UseVirtualPanel && i51DrawVirtualPanel )
    {
        if( i51CurrentVpManage.TotalVPNumber == 0 ) return iFALSE;

        if( iNULL == i51CurrentVpActivity )	return iFALSE;
		
        // �����Ѿ�������ֱ�ӷ���
        if(  i51CurrentVpActivity->VpIsFull ) return iFALSE;

        StrLength = strlen( (iCHAR*)text)+2;
        if(StrLength%2) StrLength++;
        WordsHandle = (iU32)i51AdeOsMalloc( StrLength,  __FILE__ , __LINE__ );
        if( iNULL == WordsHandle ) return iFALSE;
        memset((void *)WordsHandle,0,StrLength);
        i51AdeStdMemcpy16( (void *)WordsHandle, (void *)text, strlen( (iCHAR*)text));

        CurrDrawID = i51CurrentVpActivity->VPCurrentElement;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementType = I51KITG2_ELEMENT_TYPE_WORDS;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementNumber = CurrDrawID;

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.WordsHandle = WordsHandle;

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.DrawRect.X= DrawRect->X;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.DrawRect.Y = DrawRect->Y;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.DrawRect.Width= DrawRect->Width;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.DrawRect.Height = DrawRect->Height;

	if( SrcRect )
	{
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.X= SrcRect->X;
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.Y = SrcRect->Y;
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.Width= SrcRect->Width;
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.Height = SrcRect->Height;
	}
	else
	{
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.X= DrawRect->X;
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.Y = DrawRect->Y;
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.Width= DrawRect->Width;
	        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.SrcRect.Height = DrawRect->Height;
	}

        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.fgcolor = fgcolor;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.bgcolor = bgcolor;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.DrawDirect = DrawDirect;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.WordsNum = WordsNum;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.Effect = Effect;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.WordsRowGap= WordsRowGap;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.WordsColumnGap = WordsColumnGap;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.ReFileHandle = Handle;
        i51CurrentVpActivity->VPElementDetails[CurrDrawID].ElementData.Words.TypeheadID = TypeheadID;

        i51CurrentVpActivity->VPCurrentElement =  CurrDrawID +1;

        if( (CurrDrawID+1) >= i51CurrentVpActivity->VPMaxElement )	i51CurrentVpActivity->VpIsFull = iTRUE;
        return iTRUE;
    }

    DrawWordsStartX = DrawRect->X;
    DrawWordsStartY = DrawRect->Y;
    DrawWidth= DrawRect->Width;
    DrawHeight= DrawRect->Height;

    if( SrcRect )
    {
        SrcStartX = SrcRect->X;
        SrcStartY = SrcRect->Y;
        SrcWidth = SrcRect->Width;
        SrcHeight = SrcRect->Height;
    }
    else
    {
        SrcStartX = DrawRect->X;
        SrcStartY = DrawRect->Y;
        SrcWidth = DrawRect->Width;
        SrcHeight = DrawRect->Height;
    }

    if( DrawWordsStartX<SrcStartX )
    {
        DrawWidth -= SrcStartX-DrawWordsStartX; 
        DrawWordsStartX = SrcStartX;
    }

    if( (DrawWordsStartX+DrawWidth)>(SrcStartX+SrcWidth) )
    {
        DrawWidth = SrcStartX + SrcWidth - DrawWordsStartX;
    }

    if( DrawWordsStartY<SrcStartY)
    {
        DrawHeight -= SrcStartY -DrawWordsStartY;
        DrawWordsStartY = SrcStartY;
    }

    if( (DrawWordsStartY+DrawHeight) > (SrcStartY+SrcHeight) )
    {
        DrawHeight = SrcStartY+SrcHeight-DrawWordsStartY;
    }

    VPDrawRect.X = 0;
    VPDrawRect.Y = 0;
    VPDrawRect.Width= ScreenWidth;
    VPDrawRect.Height= ScreenHeight;
	
    if ( i51UseVirtualPanel )
    {
        VPActualRect = &(i51CurrentVpActivity->VPActualRect);

        VPDrawRect.X= (i51CurrentVpActivity->VPDrawRect).X;
        VPDrawRect.Y= (i51CurrentVpActivity->VPDrawRect).Y;
        VPDrawRect.Width= (i51CurrentVpActivity->VPDrawRect).Width;
        VPDrawRect.Height= (i51CurrentVpActivity->VPDrawRect).Height;

        DrawWordsStartX += VPActualRect->X;
        DrawWordsStartY += VPActualRect->Y;

        SrcStartX += VPActualRect->X;
        SrcStartY += VPActualRect->Y;
       		
    }
	
    if( DrawWordsStartX<VPDrawRect.X )
    {
        DrawWidth -= (VPDrawRect.X - DrawWordsStartX);
        DrawWordsStartX = VPDrawRect.X;
    }
	
    if( DrawWordsStartY<VPDrawRect.Y )
    {
        DrawHeight -= VPDrawRect.Y - DrawWordsStartY;
        DrawWordsStartY= VPDrawRect.Y;
    }

    if( (DrawWordsStartX+DrawWidth)> (VPDrawRect.X+VPDrawRect.Width) )
    {
        DrawWidth =  VPDrawRect.X+VPDrawRect.Width - DrawWordsStartX;
    }
		
    if( (DrawWordsStartY+DrawHeight)> (VPDrawRect.Y+VPDrawRect.Height) )
    {
        DrawHeight =  VPDrawRect.Y+VPDrawRect.Height- DrawWordsStartY;
    }

    if( DrawWordsStartX<0 )
    {
        DrawWidth += DrawWordsStartX;
        DrawWordsStartX = 0;
    }
	
    if( DrawWordsStartY<0 )
    {
        DrawHeight+= DrawWordsStartY;
        DrawWordsStartY = 0;
    }

#if 0	
    for( Temp=0; Temp<MAX_TYPEHEAD_NUMBER; Temp++ )
    {
        if( (i51KitG2FontManage[Temp].TypeheadID==TypeheadID && i51KitG2FontManage[Temp].FontFileHandle == Handle)
            ||i51KitG2FontManage[Temp].FontFileHandle == 0)
        {
            UseTypeheadId = Temp;
            break;
        }
    }
    if( Temp==MAX_TYPEHEAD_NUMBER || i51KitG2FontManage[Temp].FontFileHandle == 0 )
    {
        if( Temp==MAX_TYPEHEAD_NUMBER ) UseTypeheadId = 0;
		iLog("Temp:%d,UseTypeheadId:%d",Temp,UseTypeheadId);
        i51KitG2FontSetUse(Handle, TypeheadID, UseTypeheadId);
    }
#endif

//    CurrTypehead = &i51KitG2FontManage[UseTypeheadId];
	i51KitG2FontSetUse(Handle, TypeheadID, &CurrTypehead);
    TypeheadWidth = CurrTypehead.WordWidth;
    TypeheadHeight = CurrTypehead.WordHeight;
    AllWordsOffset = CurrTypehead.AllWordsOffset;
    FontFileHandle = CurrTypehead.FontFileHandle;
    NormalWordStock  = (i51KitFontWord *)(CurrTypehead.NormalWordsEntry);
		
    if( TypeheadHeight !=12 && TypeheadHeight !=14 && TypeheadHeight !=16 ) return iFALSE;

 //   if( TypeheadHeight>DrawHeight ) return iTRUE;
//    if( TypeheadHeight>DrawWidth ) return iTRUE;
//    if( DrawWordsStartX>(VPDrawRect.X+VPDrawRect.Width-TypeheadHeight)) return iTRUE;
//    if( DrawWordsStartY>(VPDrawRect.Y+VPDrawRect.Height-TypeheadHeight) ) return iTRUE;
    if( 0>=DrawHeight ) return iTRUE;
    if( 0>=DrawWidth ) return iTRUE;
    if( DrawWordsStartX>(VPDrawRect.X+VPDrawRect.Width)) return iTRUE;
    if( DrawWordsStartY>(VPDrawRect.Y+VPDrawRect.Height) ) return iTRUE;

    NeedUnderLine = Effect&i51KITG2_FONT_UNDERLINE;
    NeedBKColor = Effect&i51KITG2_FONT_BKCOLOR;

    RowGap = WordsRowGap;
    ColumnGap = WordsColumnGap;
	
    // ����о�Ϊ������Ҫ�»��ߣ�������һ�л��»���
    if( RowGap==0 && NeedUnderLine ) RowGap = 1;

    DoubleTypeheadHeight = TypeheadHeight<<1;
	
    NeedReadLongth = TypeheadWidth * TypeheadHeight;

    StrLength = CountWordsNum( (iCHAR *)text);
    if( (WordsNum)<StrLength && WordsNum>0 ) StrLength = WordsNum;
    DrawWordsNum = StrLength;

     LcdBufferPos = DrawWordsStartY*ScreenWidth+DrawWordsStartX;
	
    // ������򱳾�ɫ
    if( NeedBKColor)
    {
        i51KitG2CleanScreen(bgcolor, DrawRect);
    }

    // ���㲻ͬ�����з��������
    if( DrawDirect<i51KITG2_FONT_WORDS_ROTATE_90)
    {

        MulLeftWidth = DrawWordsStartX - SrcStartX;
        MulRightWidth = (SrcStartX + SrcWidth) - (DrawWordsStartX + DrawWidth) ;
        MulTopWidth = DrawWordsStartY - SrcStartY;
        MulBottomWidth = (SrcStartY + SrcHeight) - (DrawWordsStartY + DrawHeight) ; 
		
        DrawUnderlinesStartX = DrawWordsStartX;
        DrawUnderlinesStartY = DrawWordsStartY;
        LcdBufferTypeheadRowAdds= ScreenWidth;
        LcdBuffColumnChinaAdds = TypeheadHeight + ColumnGap;
        LcdBuffColumnAscllAdds = (TypeheadHeight>>1) + ((ColumnGap+1)>>1);
        LcdBuffRowAdds = ScreenWidth*(TypeheadHeight+RowGap);
        PointAdd0 = 0;
        PointAdd1 = 1;
        PointAdd2 = 2;
        PointAdd3 = 3;
        PointAdd4 = 4;
        PointAdd5 = 5;
        PointAdd6 = 6;
        PointAdd7 = 7;
        PointAdd8 = 8;
        PointAdd9 = 9;
        PointAdd10 = 10;
        PointAdd11 = 11;
        PointAdd12 = 12;
        PointAdd13 = 13;
        PointAdd14 = 14;
        PointAdd15 = 15;
    }
    else
    {

        MulLeftWidth = DrawWordsStartY - SrcStartY;
        MulRightWidth = (SrcStartY + SrcHeight) - (DrawWordsStartY + DrawHeight) ;
        MulTopWidth = (SrcStartX+SrcWidth) - (DrawWordsStartX+DrawWidth);
        MulBottomWidth = DrawWordsStartX - SrcStartX; 

        DrawUnderlinesStartX = DrawWordsStartX + DrawWidth - TypeheadHeight -1;
        DrawUnderlinesStartY = DrawWordsStartY;
        LcdBufferTypeheadRowAdds= -1;
        LcdBuffColumnChinaAdds = (TypeheadHeight + ColumnGap)*ScreenWidth;
        LcdBuffColumnAscllAdds = ((TypeheadHeight>>1) + ((ColumnGap+1)>>1))*ScreenWidth;
        LcdBuffRowAdds = -(TypeheadHeight+RowGap);
        DrawWordsStartX += DrawWidth - 1;
		
        DrawWidth = DrawWidth+DrawHeight;
        DrawHeight = DrawWidth - DrawHeight;
        DrawWidth = DrawWidth -DrawHeight;
		
        SrcWidth = SrcWidth+SrcHeight;
        SrcHeight = SrcWidth - SrcHeight;
        SrcWidth = SrcWidth -SrcHeight;
		
        PointAdd0 = 0;
        PointAdd1 = ScreenWidth;
        PointAdd2 = 2*ScreenWidth;
        PointAdd3 = 3*ScreenWidth;
        PointAdd4 = 4*ScreenWidth;
        PointAdd5 = 5*ScreenWidth;
        PointAdd6 = 6*ScreenWidth;
        PointAdd7 = 7*ScreenWidth;
        PointAdd8 = 8*ScreenWidth;
        PointAdd9 = 9*ScreenWidth;
        PointAdd10 = 10*ScreenWidth;
        PointAdd11 = 11*ScreenWidth;
        PointAdd12 = 12*ScreenWidth;
        PointAdd13 = 13*ScreenWidth;
        PointAdd14 = 14*ScreenWidth;
        PointAdd15 = 15*ScreenWidth;
    }

	
    // ��ʼ��������
    Temp = MulTopWidth%(TypeheadHeight+RowGap);
    if( RowGap>0 && Temp)
    {
    	if( (TypeheadHeight+RowGap - Temp )<=RowGap )
    	{
    		DrawWordsStartY += TypeheadHeight+RowGap - Temp;
			DrawHeight -= TypeheadHeight+RowGap - Temp;
    	}
    }

    Temp = MulLeftWidth%(TypeheadWidth+ColumnGap);
    if( ColumnGap>0 && Temp)
    {
    	if( (TypeheadWidth+ColumnGap - Temp )<=ColumnGap )
    	{
    		DrawWordsStartX += TypeheadWidth+ColumnGap - Temp;
			DrawWidth -= TypeheadWidth+ColumnGap - Temp;
    	}
    }
	
	if( DrawWidth<=0 || DrawHeight<=0 ) return iTRUE;
	
    LcdBufferTypeheadPos = DrawWordsStartY*ScreenWidth+DrawWordsStartX;
    LcdBuffColumnPos = LcdBufferTypeheadPos;
    LcdBuffRowPos = LcdBufferTypeheadPos;

    UnderLineNumber = 0;
    TempDrawWidth = 0;  
    AscllWidth = (TypeheadHeight>>1);	
	
    switch( TypeheadHeight)	
    {
        case 12 : WordsTypehead = (iU32)ASCLL_WORDS_12; TypeheadMask = 0XF0; break;
        case 14 : WordsTypehead = (iU32)ASCLL_WORDS_14; TypeheadMask = 0XFC;break;
        case 16 : WordsTypehead = (iU32)ASCLL_WORDS_16; TypeheadMask = 0XFF;break;
        default : return iFALSE;
    }
	
//	iLog("Start draw word");
    Temp = 0;	
    for(CurrWords=0;CurrWords<DrawWordsNum; CurrWords++ )
    {
        // Ӣ����
        if(Words[Temp]<0XA0)
        {
            if( (LineFeed==iFALSE) && ( (Words[Temp]>='a'&&Words[Temp]<='z')
            || (Words[Temp]>='A'&&Words[Temp]<='Z') || (Words[Temp]>='0'&&Words[Temp]<='9') ))
            {
                LineFeed = iTRUE;

            }
				
            if( LineFeed && ( (Words[Temp]>='a'&&Words[Temp]<='z') 
            || (Words[Temp]>='A'&&Words[Temp]<='Z') || (Words[Temp]>='0'&&Words[Temp]<='9') ))
            {
                LineFeed = iFALSE;
                LetterNum = 0;
                while(1)
                {
                    if(((Words[Temp+LetterNum]>='a'&&Words[Temp+LetterNum]<='z')
                   || (Words[Temp+LetterNum]>='A'&&Words[Temp+LetterNum]<='Z') 
                   || (Words[Temp+LetterNum]>='0'&&Words[Temp+LetterNum]<='9'))
                   && (CurrWords+LetterNum<DrawWordsNum))
                    {
                        LetterNum++;
                    }
                    else
                    {
                        break;
                    }
                }
                if( (TempSrcWidth+LetterNum*AscllWidth)>SrcWidth)
                {
                    TempSrcWidth = SrcWidth;
                }
            }

            if( (TempSrcWidth+AscllWidth)>SrcWidth)
            {
                // ����
                if(Words[Temp]==' '&&Words[Temp+1]!=' ') 
                {
                    Temp++;
                    continue;
                }
			   
                if( TempSrcHeight>MulTopWidth)
                {
                    LcdBufferTypeheadPos = LcdBuffRowPos+LcdBuffRowAdds;
                    LcdBuffColumnPos = LcdBufferTypeheadPos;
                    LcdBuffRowPos = LcdBufferTypeheadPos;
                    TempDrawHeight += TypeheadHeight + RowGap; 
                    // ��Ҫ���Ƶ������ڻ��������·�
                    if( TempDrawHeight>=DrawHeight ) break;					
                    UnderLineNumber++;				
                }
                else if( (TempSrcHeight+TypeheadHeight)>MulTopWidth )
                {
                    TempDrawHeight = TypeheadHeight -( MulTopWidth - TempSrcHeight ) + RowGap;
                    LcdBufferTypeheadPos = LcdBuffRowPos+LcdBuffRowAdds*(TempDrawHeight)/(TypeheadHeight+RowGap);
                    LcdBuffColumnPos = LcdBufferTypeheadPos;
                    LcdBuffRowPos = LcdBufferTypeheadPos;
                    // ��Ҫ���Ƶ������ڻ��������·�
                    if( TempDrawHeight>=DrawHeight ) break;					
                    if( DrawDirect<i51KITG2_FONT_WORDS_ROTATE_90)
                    {
                        DrawUnderlinesStartY -= MulTopWidth - TempSrcHeight;
                    }
                    else
                    {
                        DrawUnderlinesStartX += MulTopWidth - TempSrcHeight;
                    }
                    UnderLineNumber++;				
                }
                TempSrcHeight += TypeheadHeight + RowGap;
                TempSrcWidth = 0;
                TempDrawWidth = 0;			   
            }

            if( (TempSrcWidth+AscllWidth)<=MulLeftWidth) 
            {
                // ��Ҫ���Ƶ������ڻ����������
                TempSrcWidth += AscllWidth;
                Temp++;
                continue; 				
            }
			
            if( (TempSrcHeight+TypeheadHeight)<=MulTopWidth )
            {
                // ��Ҫ���Ƶ������ڻ��������Ϸ�
                TempSrcWidth += AscllWidth;
                Temp++;
                continue;
            }

            if(TempDrawWidth>=DrawWidth)
            {
                // ��Ҫ���Ƶ������ڻ��������ұ�
                TempSrcWidth += AscllWidth;
                Temp++;
                continue;
            }
			
            if( (TempSrcWidth>=MulLeftWidth)&&((TempDrawWidth+AscllWidth)<DrawWidth) 
            &&(TempSrcHeight>=MulTopWidth)&& ((TempDrawHeight+TypeheadHeight)<DrawHeight))
            {
                // ��Ҫ���Ƶ������ڻ�������֮��
                TempSrcWidth += AscllWidth;
                TempDrawWidth += AscllWidth;
                Temp2 = (Words[Temp]-0X20);			
                TypeMatrix = (iU8 *)(WordsTypehead+Temp2*TypeheadHeight);
                for(Temp1=0; Temp1<TypeheadHeight; Temp1++)
                {
                    CurrTypeMatrix1 = TypeMatrix[Temp1];
                    if(CurrTypeMatrix1&(0X80)) LcdBuffer[LcdBufferTypeheadPos+PointAdd0] = fgcolor;
                    if(CurrTypeMatrix1&(0X40)) LcdBuffer[LcdBufferTypeheadPos+PointAdd1] = fgcolor;
                    if(CurrTypeMatrix1&(0X20)) LcdBuffer[LcdBufferTypeheadPos+PointAdd2] = fgcolor;
                    if(CurrTypeMatrix1&(0X10)) LcdBuffer[LcdBufferTypeheadPos+PointAdd3] = fgcolor;
                    if(CurrTypeMatrix1&(0X08)) LcdBuffer[LcdBufferTypeheadPos+PointAdd4] = fgcolor;
                    if(CurrTypeMatrix1&(0X04)) LcdBuffer[LcdBufferTypeheadPos+PointAdd5] = fgcolor;
                    if(CurrTypeMatrix1&(0X02)) LcdBuffer[LcdBufferTypeheadPos+PointAdd6] = fgcolor;
                    if(CurrTypeMatrix1&(0X01)) LcdBuffer[LcdBufferTypeheadPos+PointAdd7] = fgcolor;
    
                    LcdBufferTypeheadPos += LcdBufferTypeheadRowAdds;
                }
                Temp ++;
                LcdBufferTypeheadPos = LcdBuffColumnPos + LcdBuffColumnAscllAdds;
                LcdBuffColumnPos = LcdBufferTypeheadPos;
                continue;			
            }
            else
            {
                // ��Ҫ���Ƶ��������������߽��ཻ
                iS32 WidthMul = AscllWidth;
                TempTypeheadHeight = TypeheadHeight;
                Temp1=0;
                if( TempDrawHeight==0 && TempSrcHeight<MulTopWidth)
                {
                    // ���ཻ
                    Temp1 = MulTopWidth - TempSrcHeight;
                }
                if( (TempDrawHeight+TypeheadHeight)>DrawHeight)
                {
                    // ���ཻ
                    TempTypeheadHeight = DrawHeight - TempDrawHeight + Temp1;
                    if(TempTypeheadHeight>TypeheadHeight) 	 TempTypeheadHeight = TypeheadHeight;			
                }

                TempTypeheadMove = 0;
                TempTypeheadMask = 0XFF;
                if( TempDrawWidth==0 && TempSrcWidth<MulLeftWidth)
                {
                    // ���ཻ
                    TempTypeheadMove = MulLeftWidth - TempSrcWidth;
//                    TempDrawWidth += TempSrcWidth + AscllWidth- MulLeftWidth;
                    WidthMul = TempSrcWidth + AscllWidth- MulLeftWidth;
                }
                if( (TempDrawWidth+AscllWidth)>DrawWidth)
                {
                    // ���ཻ
                    TempTypeheadMask = (TempTypeheadMask)>>(8 - DrawWidth + TempDrawWidth);
                    TempTypeheadMask = (TempTypeheadMask)<<(8 - DrawWidth + TempDrawWidth);
//                    TempDrawWidth += AscllWidth;
                }
//                else
                {
                    TempDrawWidth += WidthMul;
                }
                
                TempSrcWidth += AscllWidth;
                Temp2 = (Words[Temp]-0X20);			
                TypeMatrix = (iU8 *)(WordsTypehead+Temp2*TypeheadHeight);
                for( ;Temp1<TempTypeheadHeight; Temp1++)
                {
                    CurrTypeMatrix1 = ((TypeMatrix[Temp1]<<TempTypeheadMove)&TempTypeheadMask);
                    if(CurrTypeMatrix1&(0X80)) LcdBuffer[LcdBufferTypeheadPos+PointAdd0] = fgcolor;
                    if(CurrTypeMatrix1&(0X40)) LcdBuffer[LcdBufferTypeheadPos+PointAdd1] = fgcolor;
                    if(CurrTypeMatrix1&(0X20)) LcdBuffer[LcdBufferTypeheadPos+PointAdd2] = fgcolor;
                    if(CurrTypeMatrix1&(0X10)) LcdBuffer[LcdBufferTypeheadPos+PointAdd3] = fgcolor;
                    if(CurrTypeMatrix1&(0X08)) LcdBuffer[LcdBufferTypeheadPos+PointAdd4] = fgcolor;
                    if(CurrTypeMatrix1&(0X04)) LcdBuffer[LcdBufferTypeheadPos+PointAdd5] = fgcolor;
                    if(CurrTypeMatrix1&(0X02)) LcdBuffer[LcdBufferTypeheadPos+PointAdd6] = fgcolor;
                    if(CurrTypeMatrix1&(0X01)) LcdBuffer[LcdBufferTypeheadPos+PointAdd7] = fgcolor;
    
                    LcdBufferTypeheadPos += LcdBufferTypeheadRowAdds;
                }
                Temp ++;
                LcdBufferTypeheadPos = LcdBuffColumnPos + LcdBuffColumnAscllAdds*(AscllWidth-TempTypeheadMove)/AscllWidth;
                LcdBuffColumnPos = LcdBufferTypeheadPos;
                continue;			
            }
        }
		
        LineFeed = iTRUE;


        if( (TempSrcWidth+TypeheadHeight)>SrcWidth)
        {
            // ����
            if( TempSrcHeight>MulTopWidth)
            {
                LcdBufferTypeheadPos = LcdBuffRowPos+LcdBuffRowAdds;
                LcdBuffColumnPos = LcdBufferTypeheadPos;
                LcdBuffRowPos = LcdBufferTypeheadPos;
                TempDrawHeight += TypeheadHeight + RowGap; 
                // ��Ҫ���Ƶ������ڻ��������·�
                if( TempDrawHeight>=DrawHeight ) break;					
                UnderLineNumber++;				
            }
            else if( (TempSrcHeight+TypeheadHeight)>MulTopWidth )
            {
                TempDrawHeight = TypeheadHeight -( MulTopWidth - TempSrcHeight ) + RowGap;
                LcdBufferTypeheadPos = LcdBuffRowPos+LcdBuffRowAdds*(TempDrawHeight)/(TypeheadHeight+RowGap);
                LcdBuffColumnPos = LcdBufferTypeheadPos;
                LcdBuffRowPos = LcdBufferTypeheadPos;
                // ��Ҫ���Ƶ������ڻ��������·�
                if( TempDrawHeight>=DrawHeight ) break;					
                if( DrawDirect<i51KITG2_FONT_WORDS_ROTATE_90)
                {
                    DrawUnderlinesStartY -= MulTopWidth - TempSrcHeight;
                }
                else
                {
                    DrawUnderlinesStartX += MulTopWidth - TempSrcHeight;
                }
                UnderLineNumber++;				
            }
            TempSrcHeight += TypeheadHeight + RowGap;
            TempSrcWidth = 0;
            TempDrawWidth = 0;			   
        }

        if( (TempSrcWidth+TypeheadHeight)<=MulLeftWidth) 
        {
            // ��Ҫ���Ƶ������ڻ����������
            TempSrcWidth += TypeheadHeight+ColumnGap;
            Temp+=2;
            continue; 				
        }
		
		if( (TempSrcHeight+TypeheadHeight)<=MulTopWidth )
        {
            // ��Ҫ���Ƶ������ڻ��������Ϸ�
            TempSrcWidth += TypeheadHeight+ColumnGap;
            Temp+=2;
            continue;
        }

        if(TempDrawWidth>=DrawWidth)
        {
            // ��Ҫ���Ƶ������ڻ��������ұ�
            TempSrcWidth += TypeheadHeight+ColumnGap;
            Temp+=2;
            continue;
        }

        // ���ҵ�ǰ���Ƶ�������ģ�Ƿ����ڴ���
        WordKey = (Words[Temp]<<8)|Words[Temp+1];
        if(i51KitG2FontFindKey(WordKey, &FindWordKey, &CurrTypehead))		
        {
            TypeMatrix = NormalWordStock[FindWordKey].TypeMatrix;
        }
        else
        {
            // ���ļ��ж�ȡ��ǰ������ģ
            CurrWordsOffset = AllWordsOffset + ((Words[Temp]-0XA1)*94 + (Words[Temp+1]-0XA1))*NeedReadLongth;
            FlagReturn = i51AdeOsFileSetSeek( FontFileHandle, CurrWordsOffset, i51_OS_FL_SEEK_HEAD );
            if( iFALSE == FlagReturn )
            {
                LcdBufferTypeheadPos = LcdBuffColumnPos + LcdBuffColumnAscllAdds;
                LcdBuffColumnPos = LcdBufferTypeheadPos;
                continue;
            }

            FlagReturn = i51AdeOsFileRead( FontFileHandle, ReadWords, NeedReadLongth, &LengthReadData);   
            if( (iFALSE == FlagReturn) || (LengthReadData!=NeedReadLongth) ) 
            {
                LcdBufferTypeheadPos = LcdBuffColumnPos + LcdBuffColumnAscllAdds;
                LcdBuffColumnPos = LcdBufferTypeheadPos;
                continue;
            }

            TypeMatrix = ReadWords;
        }
		
        if( (TempSrcWidth>=MulLeftWidth)&&((TempDrawWidth+TypeheadHeight)<DrawWidth) 
        &&(TempSrcHeight>=MulTopWidth)&& ((TempDrawHeight+TypeheadHeight)<DrawHeight))
        {
            // ����һ������
            for(Temp1=0; Temp1<DoubleTypeheadHeight; )
            {
                CurrTypeMatrix1 = TypeMatrix[Temp1++];
                CurrTypeMatrix2 = TypeMatrix[Temp1++]&TypeheadMask;
                if(CurrTypeMatrix1&(0X80)) LcdBuffer[LcdBufferTypeheadPos+PointAdd0] = fgcolor;
                if(CurrTypeMatrix1&(0X40)) LcdBuffer[LcdBufferTypeheadPos+PointAdd1] = fgcolor;
                if(CurrTypeMatrix1&(0X20)) LcdBuffer[LcdBufferTypeheadPos+PointAdd2] = fgcolor;
                if(CurrTypeMatrix1&(0X10)) LcdBuffer[LcdBufferTypeheadPos+PointAdd3] = fgcolor;
                if(CurrTypeMatrix1&(0X08)) LcdBuffer[LcdBufferTypeheadPos+PointAdd4] = fgcolor;
                if(CurrTypeMatrix1&(0X04)) LcdBuffer[LcdBufferTypeheadPos+PointAdd5] = fgcolor;
                if(CurrTypeMatrix1&(0X02)) LcdBuffer[LcdBufferTypeheadPos+PointAdd6] = fgcolor;
                if(CurrTypeMatrix1&(0X01)) LcdBuffer[LcdBufferTypeheadPos+PointAdd7] = fgcolor;
                if(CurrTypeMatrix2&(0X80)) LcdBuffer[LcdBufferTypeheadPos+PointAdd8] = fgcolor;
                if(CurrTypeMatrix2&(0X40)) LcdBuffer[LcdBufferTypeheadPos+PointAdd9] = fgcolor;
                if(CurrTypeMatrix2&(0X20)) LcdBuffer[LcdBufferTypeheadPos+PointAdd10] = fgcolor;
                if(CurrTypeMatrix2&(0X10)) LcdBuffer[LcdBufferTypeheadPos+PointAdd11] = fgcolor;
                if(CurrTypeMatrix2&(0X08)) LcdBuffer[LcdBufferTypeheadPos+PointAdd12] = fgcolor;
                if(CurrTypeMatrix2&(0X04)) LcdBuffer[LcdBufferTypeheadPos+PointAdd13] = fgcolor;
                if(CurrTypeMatrix2&(0X02)) LcdBuffer[LcdBufferTypeheadPos+PointAdd14] = fgcolor;
                if(CurrTypeMatrix2&(0X01)) LcdBuffer[LcdBufferTypeheadPos+PointAdd15] = fgcolor;
    
                LcdBufferTypeheadPos += LcdBufferTypeheadRowAdds;
    			
            }        

            Temp += 2;
            TempDrawWidth += ColumnGap + TypeheadHeight;
            TempSrcWidth += ColumnGap + TypeheadHeight;
            LcdBufferTypeheadPos = LcdBuffColumnPos + LcdBuffColumnChinaAdds;
            LcdBuffColumnPos = LcdBufferTypeheadPos;
			
        }		
        else
        {
            iS32 WidthMul = TypeheadHeight;
            // ��Ҫ���Ƶ��������������߽��ཻ
            TempTypeheadHeight = TypeheadHeight<<1;
            Temp1=0;
            if( TempDrawHeight==0 && TempSrcHeight<MulTopWidth)
            {
                // ���ཻ
                Temp1 = ((MulTopWidth - TempSrcHeight)<<1);
            }
            if( (TempDrawHeight+TypeheadHeight)>DrawHeight)
            {
                // ���ཻ
                TempTypeheadHeight = ((DrawHeight - TempDrawHeight)<<1) + Temp1;
                if( TempTypeheadHeight>(TypeheadHeight<<1)) TempTypeheadHeight = TypeheadHeight<<1;
            }

            TempTypeheadMove = 0;
            TempTypeheadMask = 0XFFFF;
            if( TempDrawWidth==0 && TempSrcWidth<MulLeftWidth)
            {
                // ���ཻ
                TempTypeheadMove = MulLeftWidth - TempSrcWidth;
//                TempDrawWidth += TempSrcWidth + TypeheadHeight- MulLeftWidth+ColumnGap;
                WidthMul = TempSrcWidth + TypeheadHeight- MulLeftWidth+ColumnGap;
            }
            if( (TempDrawWidth+TypeheadHeight)>DrawWidth)
            {
                // ���ཻ
                TempTypeheadMask = (TempTypeheadMask)>>(16 - DrawWidth + TempDrawWidth);
                TempTypeheadMask = (TempTypeheadMask)<<(16 - DrawWidth + TempDrawWidth);
            }
            {
                TempDrawWidth += WidthMul;
            }
			
            // ����һ������
            for(; Temp1<TempTypeheadHeight; )
            {
                CurrTypeMatrix1 = TypeMatrix[Temp1++];
                CurrTypeMatrix2 = TypeMatrix[Temp1++]&TypeheadMask;
                TempTypeheadMask1 = ((( (CurrTypeMatrix1<<8)|CurrTypeMatrix2)<<TempTypeheadMove)&TempTypeheadMask);
                CurrTypeMatrix1 = (TempTypeheadMask1>>8)&0XFF;
                CurrTypeMatrix2 = (TempTypeheadMask1)&TypeheadMask;
                
                if(CurrTypeMatrix1&(0X80)) LcdBuffer[LcdBufferTypeheadPos+PointAdd0] = fgcolor;
                if(CurrTypeMatrix1&(0X40)) LcdBuffer[LcdBufferTypeheadPos+PointAdd1] = fgcolor;
                if(CurrTypeMatrix1&(0X20)) LcdBuffer[LcdBufferTypeheadPos+PointAdd2] = fgcolor;
                if(CurrTypeMatrix1&(0X10)) LcdBuffer[LcdBufferTypeheadPos+PointAdd3] = fgcolor;
                if(CurrTypeMatrix1&(0X08)) LcdBuffer[LcdBufferTypeheadPos+PointAdd4] = fgcolor;
                if(CurrTypeMatrix1&(0X04)) LcdBuffer[LcdBufferTypeheadPos+PointAdd5] = fgcolor;
                if(CurrTypeMatrix1&(0X02)) LcdBuffer[LcdBufferTypeheadPos+PointAdd6] = fgcolor;
                if(CurrTypeMatrix1&(0X01)) LcdBuffer[LcdBufferTypeheadPos+PointAdd7] = fgcolor;
                if(CurrTypeMatrix2&(0X80)) LcdBuffer[LcdBufferTypeheadPos+PointAdd8] = fgcolor;
                if(CurrTypeMatrix2&(0X40)) LcdBuffer[LcdBufferTypeheadPos+PointAdd9] = fgcolor;
                if(CurrTypeMatrix2&(0X20)) LcdBuffer[LcdBufferTypeheadPos+PointAdd10] = fgcolor;
                if(CurrTypeMatrix2&(0X10)) LcdBuffer[LcdBufferTypeheadPos+PointAdd11] = fgcolor;
                if(CurrTypeMatrix2&(0X08)) LcdBuffer[LcdBufferTypeheadPos+PointAdd12] = fgcolor;
                if(CurrTypeMatrix2&(0X04)) LcdBuffer[LcdBufferTypeheadPos+PointAdd13] = fgcolor;
                if(CurrTypeMatrix2&(0X02)) LcdBuffer[LcdBufferTypeheadPos+PointAdd14] = fgcolor;
                if(CurrTypeMatrix2&(0X01)) LcdBuffer[LcdBufferTypeheadPos+PointAdd15] = fgcolor;
    
                LcdBufferTypeheadPos += LcdBufferTypeheadRowAdds;
    			
            }      

            Temp += 2;
            TempSrcWidth += TypeheadHeight+ColumnGap;
            LcdBufferTypeheadPos = LcdBuffColumnPos + LcdBuffColumnChinaAdds*(TypeheadHeight-TempTypeheadMove)/TypeheadHeight;;
            LcdBuffColumnPos = LcdBufferTypeheadPos;
			
        }


    }            

    // �����»��ߣ������Ҫ
    if( NeedUnderLine )
    {
        if(DrawHeight<TypeheadHeight)
        {
            if( DrawDirect<i51KITG2_FONT_WORDS_ROTATE_90)
            {
                DrawUnderlinesStartY -=TypeheadHeight - ( MulTopWidth %(TypeheadHeight+RowGap));
            }
            else
            {
                DrawUnderlinesStartX +=TypeheadHeight - ( MulTopWidth %(TypeheadHeight+RowGap));
            }
        }
        if( (TempDrawHeight+TypeheadHeight+1)<DrawHeight&&(CurrWords==DrawWordsNum))
        {
            UnderLineNumber++;
        }
        // ���㲻ͬ�����з��������
        if(DrawDirect<i51KITG2_FONT_WORDS_ROTATE_90)
        {
            UnderLinesAdds = (TypeheadHeight+RowGap)*ScreenWidth;
            LcdBufferPos = (DrawUnderlinesStartY+TypeheadHeight)*ScreenWidth + DrawUnderlinesStartX;
            for( Temp1=0; Temp1<DrawWidth; Temp1++ )
            {
                OneRowColorBuf[Temp1] = fgcolor;
            }
			
            for( Temp=0; Temp<UnderLineNumber; Temp++ )
            {
                i51AdeStdMemcpy16(&LcdBuffer[LcdBufferPos], OneRowColorBuf,(DrawWidth<<1));
                LcdBufferPos += UnderLinesAdds;
           }
        }
        else
        {
            UnderLinesAdds = -(TypeheadHeight+RowGap);
            LcdBufferPos = (DrawUnderlinesStartY)*ScreenWidth + DrawUnderlinesStartX;
            for( Temp=0; Temp<UnderLineNumber; Temp++ )
            {
                Temp1= LcdBufferPos;
                for( Temp2=0; Temp2<DrawWidth; Temp2++ )
                {
                    LcdBuffer[Temp1] = fgcolor;
                    Temp1 +=ScreenWidth;
                }
                LcdBufferPos +=  UnderLinesAdds;
            }
        }

    }

 #if i51KITG2_DEBUG
    iLog( "G2FontDraw end , %x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x,%x"
    ,Handle,TypeheadID,text,WordsNum,DrawRect,SrcRect,DrawDirect,WordsRowGap,WordsColumnGap,fgcolor,bgcolor,Effect);
#endif
    return iTRUE;

}

#if DEBUG_I51KITG2
iKIT iBOOL i51KitG2FontDrawEx1
(
  void * TypeheadWords, iU8 * TypeheadData, iU32 TypeheadNum, 
  iU32 EnglishWidth, iU32 EnglishHeight, iU32 ChineseWidth, iU32 ChineseHeight,
  iS16 WordsRowGap,  iS16 WordsColumnGap, iCOLOR Color, iRECT * DesRect
)
{
    // author : Otto
    // since : 2011.10.19
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // void * TypeheadWords : �ַ���ָ��
    // iU16 * TypeheadData : ��ģ����
    // iU32 TypeheadNum : ��Ҫ���Ƶ���ģ����
    // iU32 EnglishWidth : Ӣ������ģ���
    // iU32 EnglishHeight : Ӣ������ģ�߶�
    // iU32 ChineseWidth : ��������ģ���
    // iU32 ChineseHeight : ��������ģ�߶�
    // iS16 WordsRowGap : ���о�
    // iS16 WordsColumnGap : �ּ��    
    // iCOLOR Color : ������ɫ
    // iRECT * DesRect : ��������
 
    // notes: ������ģ����Ļ�ϣ����Ƶķ�������������
    iU8 TempTypehead1 = 0;
    iU8 TempTypehead2 = 0;
    iU16 ScreenWidth = 0;
    iU16 ScreenHeight= 0;
    iU16 * ScreenBuffer = iNULL;
    iU32 * LcdInfoArr = iNULL;
    iU32 CurrDrawWord = 0;
    iU32 CurrTypeheadPos = 0;
    iU32 CurrWordsPos = 0;
    iU32 CurrScreenBufferPos = 0;
    iU32 OldScreenBufferPos = 0;
    iS32 Temp = 0;
    iS16 StartX = 0, StartY = 0, EndX = 0, EndY = 0;	
    iS16 CurrX = 0, CurrY = 0;		
    // �ַ���ָ��
    iU8 * Words = (iU8 *)TypeheadWords;
    iU32 CurrWidth = 0;
    iU32 CurrHeight = 0;	
    iS32 StrLength = 0;
    iU16 WordsNum = 0;
    iU16 DrawNum = 0;
    iU16 LetterNum = 0; 	

    iU8 Ch = 0;

    // ͳ����Ҫ���Ƶ��ַ�����	
    StrLength = strlen((iCHAR*)Words);
    for( Temp=0; Temp<StrLength; Temp++ )
    {
        WordsNum++;
        Ch = Words[Temp];
        if( Ch>0XA0) Temp++;
    }

    if( TypeheadNum==0 ) DrawNum = WordsNum;
    else if( TypeheadNum>WordsNum ) DrawNum = WordsNum;
    else DrawNum = TypeheadNum;
	
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

    if( DesRect==iNULL ) 
    {
        StartX = 0;
        StartY = 0;	
        EndX = ScreenWidth;
        EndY = ScreenHeight;
    }
    else
    {
        StartX = DesRect->X;
        StartY = DesRect->Y;
        EndX = DesRect->Width + StartX;
        EndY = DesRect->Height + StartY;
    }

    if(StartX<0) StartX = 0;
    if(StartY<0) StartY = 0;
    if(EndX>ScreenWidth) EndX = ScreenWidth;
    if(EndY>ScreenHeight) EndY = ScreenHeight;

    if( StartX>= ScreenWidth ) return iTRUE;
    if( StartY>= ScreenHeight ) return iTRUE;
    if( EndX<0) return iTRUE;
    if( EndY<0) return iTRUE;

    CurrX = StartX;
    CurrY = StartY;
    CurrScreenBufferPos = CurrY*ScreenWidth + CurrX;
    OldScreenBufferPos = CurrScreenBufferPos;
	
    for( CurrDrawWord=0; CurrDrawWord<DrawNum; CurrDrawWord++)
    {
        LetterNum = 1;			
        if( Words[CurrWordsPos]< 0XA0 )
        {
            CurrWidth = EnglishWidth;
            CurrHeight = EnglishHeight;

            // ͳ��Ӣ�ĵ�������ĸ����
            while(1)
            {
                if((Words[CurrWordsPos+LetterNum]>='a'&&Words[CurrWordsPos+LetterNum]<='z')
               || (Words[CurrWordsPos+LetterNum]>='A'&&Words[CurrWordsPos+LetterNum]<='Z') 
               || (Words[CurrWordsPos+LetterNum]>='0'&&Words[CurrWordsPos+LetterNum]<='9') )
                {
                    LetterNum++;
                }
                else
                {
                    break;
                }
            }
            CurrWordsPos += 1;
        }
        else
        {
             CurrWordsPos += 2;
        	CurrWidth = ChineseWidth;
             CurrHeight = ChineseHeight;
        }

        // �ж��Ƿ���
        if( CurrX+LetterNum*CurrWidth >= EndX )
        {
            if( Words[CurrWordsPos-1]< 0XA0 )
            {
                if(Words[CurrWordsPos-1]==' ' && Words[CurrWordsPos]!=' ')
                {
                    CurrWordsPos ++;
                    CurrDrawWord ++;
                    CurrTypeheadPos += CurrHeight;		
                    if(CurrDrawWord>=DrawNum) return iTRUE;
                }
            }
            if(ChineseHeight>0)
            {
                CurrY += ChineseHeight+WordsRowGap;
                if( CurrY+ChineseHeight>=EndY ) return iTRUE;
            }
            else 
            {
                CurrY += EnglishHeight+WordsRowGap;
                if( CurrY+EnglishHeight>=EndY ) return iTRUE;
            }

            CurrX = StartX;				
            CurrScreenBufferPos = CurrY*ScreenWidth + CurrX;
            OldScreenBufferPos = CurrScreenBufferPos;
        }
			
        for( Temp =0; Temp < CurrHeight; Temp++ ) 
        {
            TempTypehead1 = TypeheadData[CurrTypeheadPos++];
            if(TempTypehead1&(0X80)) ScreenBuffer[CurrScreenBufferPos] = Color;
            if(TempTypehead1&(0X40)) ScreenBuffer[CurrScreenBufferPos+1] = Color;
            if(TempTypehead1&(0X20)) ScreenBuffer[CurrScreenBufferPos+2] = Color;
            if(TempTypehead1&(0X10)) ScreenBuffer[CurrScreenBufferPos+3] = Color;
            if(TempTypehead1&(0X08)) ScreenBuffer[CurrScreenBufferPos+4] = Color;
            if(TempTypehead1&(0X04)) ScreenBuffer[CurrScreenBufferPos+5] = Color;
            if(TempTypehead1&(0X02)) ScreenBuffer[CurrScreenBufferPos+6] = Color;
            if(TempTypehead1&(0X01)) ScreenBuffer[CurrScreenBufferPos+7] = Color;
            if( CurrWidth<9 ) goto LOOP;			
            TempTypehead2 = TypeheadData[CurrTypeheadPos++];
            if(TempTypehead2&(0X80)) ScreenBuffer[CurrScreenBufferPos+8] = Color;
            if(TempTypehead2&(0X40)) ScreenBuffer[CurrScreenBufferPos+9] = Color;
            if(TempTypehead2&(0X20)) ScreenBuffer[CurrScreenBufferPos+10] = Color;
            if(TempTypehead2&(0X10)) ScreenBuffer[CurrScreenBufferPos+11] = Color;
            if(TempTypehead2&(0X08)) ScreenBuffer[CurrScreenBufferPos+12] = Color;
            if(TempTypehead2&(0X04)) ScreenBuffer[CurrScreenBufferPos+13] = Color;
            if(TempTypehead2&(0X02)) ScreenBuffer[CurrScreenBufferPos+14] = Color;
            if(TempTypehead2&(0X01)) ScreenBuffer[CurrScreenBufferPos+15] = Color;
            LOOP:
            CurrScreenBufferPos += ScreenWidth;
        }

        if(CurrWidth==ChineseWidth)
        {
            CurrScreenBufferPos = OldScreenBufferPos + CurrWidth + WordsColumnGap;
            CurrX += CurrWidth+WordsColumnGap;
        }
        else
        {
            CurrScreenBufferPos = OldScreenBufferPos + CurrWidth;
            CurrX += CurrWidth;
        }
        OldScreenBufferPos = CurrScreenBufferPos;
		
    }

    return iTRUE;
	
}

iKIT iBOOL i51KitG2FontDrawEx2
(
  void * TypeheadWords, iU8 * TypeheadData, iU32 TypeheadNum, 
  iU32 EnglishWidth, iU32 EnglishHeight, iU32 ChineseWidth, iU32 ChineseHeight,
  iS16 WordsRowGap,  iS16 WordsColumnGap, iCOLOR Color, iRECT * DesRect
)
{
    // author : Otto
    // since : 2011.10.19
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // void * TypeheadWords : �ַ���ָ��
    // iU16 * TypeheadData : ��ģ����
    // iU32 TypeheadNum : ��Ҫ���Ƶ���ģ����
    // iU32 EnglishWidth : Ӣ������ģ���
    // iU32 EnglishHeight : Ӣ������ģ�߶�
    // iU32 ChineseWidth : ��������ģ���
    // iU32 ChineseHeight : ��������ģ�߶�
    // iS16 WordsRowGap : ���о�
    // iS16 WordsColumnGap : �ּ��    
    // iCOLOR Color : ������ɫ
    // iRECT * DesRect : ��������
 
    // notes: ������ģ����Ļ�ϣ����Ƶķ�����ѡ��90��
    iU8 TempTypehead1 = 0;
    iU8 TempTypehead2 = 0;
    iU16 ScreenWidth = 0;
    iU16 ScreenHeight= 0;
    iU16 * ScreenBuffer = iNULL;
    iU32 * LcdInfoArr = iNULL;
    iU32 CurrDrawWord = 0;
    iU32 CurrTypeheadPos = 0;
    iU32 CurrWordsPos = 0;
    iU32 CurrScreenBufferPos = 0;
    iU32 OldScreenBufferPos = 0;
    iS32 Temp = 0;
    iS16 StartX = 0, StartY = 0, EndX = 0, EndY = 0;	
    iS16 CurrX = 0, CurrY = 0;		
    // �ַ���ָ��
    iU8 * Words = (iU8 *)TypeheadWords;
    iU32 CurrWidth = 0;
    iU32 CurrHeight = 0;	
    iS32 StrLength = 0;
    iU16 WordsNum = 0;
    iU16 DrawNum = 0;
    iU16 LetterNum = 0; 	

    iU8 Ch = 0;

    // ͳ����Ҫ���Ƶ��ַ�����	
    StrLength = strlen((iCHAR*)Words);
    for( Temp=0; Temp<StrLength; Temp++ )
    {
        WordsNum++;
        Ch = Words[Temp];
        if( Ch>0XA0) Temp++;
    }

    if( TypeheadNum==0 ) DrawNum = WordsNum;
    else if( TypeheadNum>WordsNum ) DrawNum = WordsNum;
    else DrawNum = TypeheadNum;
	
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

    if( DesRect==iNULL ) 
    {
        StartX = 0;
        StartY = 0;	
        EndX = ScreenWidth;
        EndY = ScreenHeight;
    }
    else
    {
        StartX = DesRect->X;
        StartY = DesRect->Y;
        EndX = DesRect->Width + StartX;
        EndY = DesRect->Height + StartY;
    }

    if(StartX<0) StartX = 0;
    if(StartY<0) StartY = 0;
    if(EndX>ScreenWidth) EndX = ScreenWidth;
    if(EndY>ScreenHeight) EndY = ScreenHeight;

    if( StartX>= ScreenWidth ) return iTRUE;
    if( StartY>= ScreenHeight ) return iTRUE;
    if( EndX<0) return iTRUE;
    if( EndY<0) return iTRUE;

    StartX += EndX;
    EndX = StartX - EndX;
    StartX = StartX - EndX;	
		
    CurrX = StartX;
    CurrY = StartY;
    CurrScreenBufferPos = CurrY*ScreenWidth + CurrX;
    OldScreenBufferPos = CurrScreenBufferPos;
	
    for( CurrDrawWord=0; CurrDrawWord<DrawNum; CurrDrawWord++)
    {
        LetterNum = 1;			
        if( Words[CurrWordsPos]< 0XA0 )
        {
            CurrWidth = EnglishWidth;
            CurrHeight = EnglishHeight;

            // ͳ��Ӣ�ĵ�������ĸ����
            while(1)
            {
                if((Words[CurrWordsPos+LetterNum]>='a'&&Words[CurrWordsPos+LetterNum]<='z')
               || (Words[CurrWordsPos+LetterNum]>='A'&&Words[CurrWordsPos+LetterNum]<='Z') 
               || (Words[CurrWordsPos+LetterNum]>='0'&&Words[CurrWordsPos+LetterNum]<='9') )
                {
                    LetterNum++;
                }
                else
                {
                    break;
                }
            }
            CurrWordsPos += 1;
        }
        else
        {
             CurrWordsPos += 2;
        	CurrWidth = ChineseWidth;
             CurrHeight = ChineseHeight;
        }

        // �ж��Ƿ���
        if( CurrY+LetterNum*CurrWidth >= EndY )
        {
            if( Words[CurrWordsPos-1]< 0XA0 )
            {
                if(Words[CurrWordsPos-1]==' ' && Words[CurrWordsPos]!=' ')
                {
                    CurrWordsPos ++;
                    CurrDrawWord ++;
                    CurrTypeheadPos += CurrHeight;		
                    if(CurrDrawWord>=DrawNum) return iTRUE;
                }
            }
            if(ChineseHeight>0)
            {
                CurrX -= ChineseHeight+WordsRowGap;
                if( CurrX-ChineseHeight<EndX ) return iTRUE;
            }
            else 
            {
                CurrX -= EnglishHeight+WordsRowGap;
                if( CurrX-EnglishHeight<EndX) return iTRUE;
            }

            CurrY = StartY;				
            CurrScreenBufferPos = CurrY*ScreenWidth + CurrX;
            OldScreenBufferPos = CurrScreenBufferPos;
        }
			
        for( Temp =0; Temp < CurrHeight; Temp++ ) 
        {
            TempTypehead1 = TypeheadData[CurrTypeheadPos++];
            if(TempTypehead1&(0X80)) ScreenBuffer[CurrScreenBufferPos] = Color;
            if(TempTypehead1&(0X40)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth] = Color;
            if(TempTypehead1&(0X20)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*2] = Color;
            if(TempTypehead1&(0X10)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*3] = Color;
            if(TempTypehead1&(0X08)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*4] = Color;
            if(TempTypehead1&(0X04)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*5] = Color;
            if(TempTypehead1&(0X02)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*6] = Color;
            if(TempTypehead1&(0X01)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*7] = Color;
            if( CurrWidth<9 ) goto LOOP;			
            TempTypehead2 = TypeheadData[CurrTypeheadPos++];
            if(TempTypehead2&(0X80)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*8] = Color;
            if(TempTypehead2&(0X40)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*9] = Color;
            if(TempTypehead2&(0X20)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*10] = Color;
            if(TempTypehead2&(0X10)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*11] = Color;
            if(TempTypehead2&(0X08)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*12] = Color;
            if(TempTypehead2&(0X04)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*13] = Color;
            if(TempTypehead2&(0X02)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*14] = Color;
            if(TempTypehead2&(0X01)) ScreenBuffer[CurrScreenBufferPos+ScreenWidth*15] = Color;
            LOOP:
            CurrScreenBufferPos --;
        }

        if(CurrWidth==ChineseWidth)
        {
            CurrScreenBufferPos = OldScreenBufferPos + (CurrWidth + WordsColumnGap)*ScreenWidth;
            CurrY += CurrWidth+WordsColumnGap;
        }
        else
        {
            CurrScreenBufferPos = OldScreenBufferPos + CurrWidth*ScreenWidth;
            CurrY += CurrWidth;
        }
        OldScreenBufferPos = CurrScreenBufferPos;
		
    }

    return iTRUE;
	
}

#endif

iKIT iBOOL i51KitG2VPEnable( void )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // ���ô˺���������G2�ĺ������������⻭���ϻ���
 #if i51KITG2_DEBUG
    iLog( "G2VPEnable start");
#endif

    i51UseVirtualPanel = iTRUE;
    i51CurrentVpManage.UseVirtualPanel = iTRUE;
    i51DrawVirtualPanel = iTRUE;

 #if i51KITG2_DEBUG
    iLog( "G2VPEnable end");
#endif
    return iTRUE;
}

iKIT iBOOL  i51KitG2VPDisable( void )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // ���ô˺�������ʹ�û���

 #if i51KITG2_DEBUG
    iLog( "G2VPDisable start");
#endif

    i51UseVirtualPanel = iFALSE;
    i51CurrentVpManage.UseVirtualPanel = iFALSE;
  
 #if i51KITG2_DEBUG
    iLog( "G2VPDisable end");
#endif
    return iTRUE;
}

iKIT iHANDLE i51KitG2VPCreate ( iU16 TotalElement , iRECT * Rect )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iU32 TotalElement : ���⻭����ͬʱ���װ�ص�Ԫ�ظ���
    // iRECT Rect : ���⻭������ԣ��������ĳ����Լ������Ļ��ƫ����
    // iALPHA Alpha : ���⻭���͸����

    // notes :  ����һ�����⻭��

    iU16 iTemp;

    i51KitG2VPStructure * NewVp = iNULL;
    
 #if i51KITG2_DEBUG
    iLog( "G2VPCreate start, %x,%x", TotalElement,Rect);
#endif
  
	// ��������Ļ��������������������»���
    if( i51CurrentVpManage.TotalVPNumber >= i51CurrentVpManage.MaxVPNumber ) return iNULL;

    if( iNULL == Rect ) return iFALSE;

    if( (Rect->Width)<=0 || (Rect->Height)<=0 ) return iFALSE;

    // ���仭���ڴ�  
    NewVp = ( i51KitG2VPStructure * )i51AdeOsMalloc((sizeof(i51KitG2VPStructure)), __FILE__ , __LINE__ );
    if( iNULL==NewVp ) return iNULL;

   // ���仭����Ԫ���ڴ�
    NewVp->VPElementDetails = iNULL;
    NewVp->VPElementDetails = ( i51KitG2VPElement * )i51AdeOsMalloc( ((sizeof(i51KitG2VPElement)) * TotalElement ), __FILE__ ,
__LINE__ );
    if( iNULL==NewVp->VPElementDetails )
    {
        i51AdeOsFree(NewVp);
        NewVp = iNULL;
        return iNULL;
    }

    // ������װ��Ԫ����������  
    NewVp->VPMaxElement = TotalElement;

    // ��ǰ�Ѿ�װ�ص�Ԫ�ظ���  
    NewVp->VPCurrentElement = 0;

    // ���⻭����
    NewVp->VPHandle = (iHANDLE)(NewVp);

    // ���⻭������
    NewVp->VPActualRect.X = Rect->X;
    NewVp->VPActualRect.Y = Rect->Y;
    NewVp->VPActualRect.Width= Rect->Width;
    NewVp->VPActualRect.Height= Rect->Height;

    NewVp->VpIsFull = iFALSE;

    NewVp->VpIsLoop = iFALSE;
	
    // �������⻭���ڹ������е�ID
    for( iTemp=0; iTemp<I51KITG2_MAX_NUMBER_VIRTUAL_PANEL; iTemp++ )
    {
        if( iNULL == (i51CurrentVpManage.VPDetails[iTemp]) )
        {
            NewVp->VP_ID= iTemp;
            i51CurrentVpManage.VPDetails[iTemp] = NewVp;
            break;
        }
    }

    if( iTemp>=I51KITG2_MAX_NUMBER_VIRTUAL_PANEL )
    {
        i51AdeOsFree(NewVp->VPElementDetails);
        NewVp->VPElementDetails = iNULL;
        i51AdeOsFree(NewVp);
        NewVp = iNULL;
        return iNULL;
    }

    NewVp->VpIsDraw = iTRUE;
	
    // VP����Ļ���
    if((Rect->X+(Rect->Width))<0) NewVp->VpIsDraw = iFALSE;
    
    // VP����Ļ�ұ�		
    if((Rect->X) >= ScreenWidth) NewVp->VpIsDraw = iFALSE;
    
    // VP����Ļ�·�
    if((Rect->Y)>=ScreenHeight) NewVp->VpIsDraw = iFALSE;
    
    // VP����Ļ�Ϸ�
    if( (Rect->Y+Rect->Height)<0) NewVp->VpIsDraw = iFALSE;

    if( i51CurrentVpManage.TotalVPNumber==0 )
    {
        i51CurrentVpActivity = NewVp;
    }
  
    i51CurrentVpManage.TotalVPNumber++;  

  #if i51KITG2_DEBUG
    iLog( "G2VPCreate end, %x,%x", TotalElement,Rect);
#endif

    return NewVp;

}

iKIT iBOOL i51KitG2VPDestroy ( iHANDLE vpanel )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : ���������⻭����

    // notes :  ����һ�����⻭��

    iU16 iTemp = 0;
	
    i51KitG2VPStructure * DestoryVp = ( i51KitG2VPStructure * )vpanel;

 #if i51KITG2_DEBUG
    iLog( "G2VPDestroy start, %x", vpanel);
#endif

    if( iNULL==DestoryVp ) return iFALSE;

    if( i51CurrentVpManage.TotalVPNumber<1) return iFALSE;

    for( iTemp=0; iTemp<DestoryVp->VPCurrentElement; iTemp++ )
    {
        if( DestoryVp->VPElementDetails[iTemp].ElementType==I51KITG2_ELEMENT_TYPE_POLYGON )
        {
            if( iFALSE==i51AdeOsFree((void *)( DestoryVp->VPElementDetails[iTemp].ElementData.Polygon.PointerPoints) ))
            {
                DestoryVp->VPElementDetails[iTemp].ElementData.Polygon.PointerPoints = iNULL;
                return iFALSE;
            }
            DestoryVp->VPElementDetails[iTemp].ElementData.Polygon.PointerPoints = iNULL;
        }
        else if( DestoryVp->VPElementDetails[iTemp].ElementType==I51KITG2_ELEMENT_TYPE_FILLPOLYGON )
        {
            if( iFALSE==i51AdeOsFree( (void *)( DestoryVp->VPElementDetails[iTemp].ElementData.FillPolygon.PointerPoints) ))
            {
                DestoryVp->VPElementDetails[iTemp].ElementData.FillPolygon.PointerPoints = iNULL;
                return iFALSE;
            }
            DestoryVp->VPElementDetails[iTemp].ElementData.FillPolygon.PointerPoints = iNULL;
        }
        else if( DestoryVp->VPElementDetails[iTemp].ElementType==I51KITG2_ELEMENT_TYPE_WORDS )
        {
            if( iFALSE==i51AdeOsFree( (void *)(DestoryVp->VPElementDetails[iTemp].ElementData.Words.WordsHandle)) )
            {
                DestoryVp->VPElementDetails[iTemp].ElementData.Words.WordsHandle = iNULL;
                return iFALSE;
            }
            DestoryVp->VPElementDetails[iTemp].ElementData.Words.WordsHandle = iNULL;
        }
    }
	
    if( (i51AdeOsFree(DestoryVp->VPElementDetails)) != iTRUE )
    {
       DestoryVp->VPElementDetails = iNULL;
       return iFALSE;
    }
   
    i51CurrentVpManage.VPDetails[DestoryVp->VP_ID] = iNULL;

    if( (i51AdeOsFree(DestoryVp)) != iTRUE )
    {
       DestoryVp = iNULL;
	return iFALSE;
    }
    i51CurrentVpManage.TotalVPNumber--;

  #if i51KITG2_DEBUG
    iLog( "G2VPDestroy end, %x", vpanel);
#endif
    return iTRUE;
  
}

iKIT iBOOL i51KitG2VPSetCoord ( iHANDLE vpanel , iS32 X, iS32 Y)
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : ���ƶ����⻭����
    // iS32 X : �������X����
    // iS32 Y : �������Y����

    // notes :  �ƶ�һ��VP,����ƶ���VP����Ļ�޽������ƶ����ɹ�

    i51KitG2VPStructure * MoveVp = ( i51KitG2VPStructure * )vpanel;
  #if i51KITG2_DEBUG
    iLog( "G2VPSetCoord start, %x,%x,%x", vpanel,X,Y);
#endif

    if( iNULL==MoveVp ) return iFALSE;

    MoveVp->VpIsDraw = iTRUE;
	
    // VP����Ļ���
    if((X+(MoveVp->VPActualRect.Width))<0) MoveVp->VpIsDraw = iFALSE;
    
    // VP����Ļ�ұ�		
    if((X)>=ScreenWidth) MoveVp->VpIsDraw = iFALSE;
    
    // VP����Ļ�·�
    if((Y)>=ScreenHeight) MoveVp->VpIsDraw = iFALSE;
    
    // VP����Ļ�Ϸ�
    if( (Y+((MoveVp->VPActualRect.Height)))<0) MoveVp->VpIsDraw = iFALSE;

    MoveVp->VPActualRect.X = X;
    MoveVp->VPActualRect.Y = Y;

 #if i51KITG2_DEBUG
    iLog( "G2VPSetCoord end, %x,%x,%x", vpanel,X,Y);
#endif
    return iTRUE;
  
}

iKIT iBOOL i51KitG2VPSetActivity ( iHANDLE vpanel )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : �����õ����⻭����

    // notes :  ���û���vpanelΪ��ǰ�����

    i51KitG2VPStructure * ActivityVp = ( i51KitG2VPStructure * )vpanel;
 #if i51KITG2_DEBUG
    iLog( "G2VPSetActivity start, %x", vpanel);
#endif
    if( iNULL==ActivityVp ) return iFALSE;

    if( i51CurrentVpManage.VPDetails[ActivityVp->VP_ID]!=ActivityVp ) return iFALSE;

    i51CurrentVpManage.ActivityVp_ID = ActivityVp->VP_ID;
    i51CurrentVpManage.CurrentVpActivity = ActivityVp;
    i51CurrentVpActivity = ActivityVp;

 #if i51KITG2_DEBUG
    iLog( "G2VPSetActivity end, %x", vpanel);
#endif
    return iTRUE;
  
}

iKIT iHANDLE i51KitG2VPGetActivity ( void )

{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // notes : ���ص�ǰ�����ľ��

    return    i51CurrentVpActivity;
}

iKIT iBOOL i51KitG2VPClean ( iHANDLE vpanel )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : ����յ����⻭����
    // notes : ������⻭��

    iU16 iTemp = 0;
  
    i51KitG2VPStructure * ClearVp = ( i51KitG2VPStructure * )vpanel;

 #if i51KITG2_DEBUG
    iLog( "G2VPClean start, %x", vpanel);
#endif
    if( iNULL==ClearVp )     return iFALSE;

    if( i51CurrentVpManage.VPDetails[ClearVp->VP_ID]!=ClearVp ) return iFALSE;

    for( iTemp=0; iTemp<ClearVp->VPCurrentElement; iTemp++ )
    {
        if( ClearVp->VPElementDetails[iTemp].ElementType==I51KITG2_ELEMENT_TYPE_POLYGON )
        {
            if( iFALSE==i51AdeOsFree( (void *)(ClearVp->VPElementDetails[iTemp].ElementData.Polygon.PointerPoints) ))
            {
                return iFALSE;
            }
            ClearVp->VPElementDetails[iTemp].ElementData.Polygon.PointerPoints = iNULL;
        }
        else if( ClearVp->VPElementDetails[iTemp].ElementType==I51KITG2_ELEMENT_TYPE_FILLPOLYGON )
        {
            if( iFALSE==i51AdeOsFree( (void *)(ClearVp->VPElementDetails[iTemp].ElementData.FillPolygon.PointerPoints)) )
            {
                return iFALSE;
            }
            ClearVp->VPElementDetails[iTemp].ElementData.FillPolygon.PointerPoints = iNULL;
        }
        else if( ClearVp->VPElementDetails[iTemp].ElementType==I51KITG2_ELEMENT_TYPE_WORDS )
        {
            if( iFALSE==i51AdeOsFree( (void *)(ClearVp->VPElementDetails[iTemp].ElementData.Words.WordsHandle)) )
            {
                return iFALSE;
            }
            ClearVp->VPElementDetails[iTemp].ElementData.Words.WordsHandle = iNULL;
        }
    }

    ClearVp->VpIsFull = iFALSE;
    ClearVp->VPCurrentElement = 0 ;

 #if i51KITG2_DEBUG
    iLog( "G2VPClean end, %x", vpanel);
#endif

    return iTRUE;
  
}

iKIT iBOOL i51KitG2VPGetAtrribute ( iHANDLE vpanel , iRECT * Rect , iU16 * ResidueSpace  )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : ��������Ե����⻭����
    // iRECT * Rect : �����С��������Ϣ
    // iU16 * ResidueSpace : ����ʣ��ռ�
    
    // notes : ���һ�����⻭�����Ϣ

    i51KitG2VPStructure * VpTemp = ( i51KitG2VPStructure * )vpanel;

#if i51KITG2_DEBUG
    iLog( "G2VPGetAtrribute start, %x,%x,%x,%x", vpanel,Rect,ResidueSpace );
#endif

    if( iNULL==VpTemp ) return iFALSE;

    if( i51CurrentVpManage.VPDetails[VpTemp->VP_ID]!=VpTemp ) return iFALSE;

    if(Rect)
    {
        Rect->X = VpTemp->VPActualRect.X;
        Rect->Y = VpTemp->VPActualRect.Y;
        Rect->Width= VpTemp->VPActualRect.Width;
        Rect->Height= VpTemp->VPActualRect.Height;
    }

    if(ResidueSpace)
    {
        *ResidueSpace = VpTemp->VPMaxElement - VpTemp->VPCurrentElement;
    }

#if i51KITG2_DEBUG
    iLog( "G2VPGetAtrribute end, %x,%x,%x,%x", vpanel,Rect,ResidueSpace );
#endif
    return iTRUE;
  
}

iKIT iBOOL i51KitG2VPiSFull( iHANDLE vpanel )
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : ��������Ե����⻭����
    // notes : �жϻ����Ƿ��Ѿ�����
#if i51KITG2_DEBUG
    iLog( "G2VPiSFull start, %x", vpanel);
#endif
    if( iNULL==vpanel ) return iFALSE;
#if i51KITG2_DEBUG
    iLog( "G2VPiSFull end, %x", vpanel);
#endif
    return (((i51KitG2VPStructure *)vpanel)->VpIsFull);

}

iKIT iBOOL i51KitG2VPRender ( iHANDLE vpanel , iRECT * DrawRect)
{
    // author : Otto
    // since : Sep 5 2011
    // (C) 2011 , PKIG Tech. Co., Ltd.

    // iHANDLE vpanel : ��Render�����⻭����
    // iRECT * DrawRect : ���Ƶ��������ϵ�����
    // notes :  ����������ϵ���Ϣ���Ƶ����������
  
    iU16 iTemp = 0;
    iU16 NumberDrawImage     = 0;
    iU8   SysAlpha = 0;
	
    i51KitG2VPStructure * VpRender = ( i51KitG2VPStructure * )vpanel;

    i51KitG2VPElement * VPElement = iNULL;

#if i51KITG2_DEBUG
    iLog( "G2VPRender start, %x,%d", vpanel,DrawRect);
#endif
    if( iFALSE == i51UseVirtualPanel ) return iFALSE;
	
    if( iNULL==VpRender ) return iFALSE;

    if( i51CurrentVpManage.VPDetails[VpRender->VP_ID]!=VpRender ) return iFALSE;

    if( iFALSE == VpRender->VpIsDraw ) return iTRUE;
	
    i51DrawVirtualPanel = iFALSE;

    if(DrawRect)
    {
        VpRender->VPDrawRect.X = DrawRect->X;
        VpRender->VPDrawRect.Y= DrawRect->Y;
        VpRender->VPDrawRect.Width = DrawRect->Width;
        VpRender->VPDrawRect.Height = DrawRect->Height;
    }
    else
    {
        VpRender->VPDrawRect.X = 0;
        VpRender->VPDrawRect.Y= 0;
        VpRender->VPDrawRect.Width = ScreenWidth;
        VpRender->VPDrawRect.Height = ScreenHeight;
    }

    if( VpRender->VPDrawRect.X < 0  )
    {
        VpRender->VPDrawRect.Width += VpRender->VPDrawRect.X;
        VpRender->VPDrawRect.X = 0;
    }
	
    if( VpRender->VPDrawRect.Y < 0  )
    {
        VpRender->VPDrawRect.Height+= VpRender->VPDrawRect.Y;
        VpRender->VPDrawRect.Y = 0;
    }

    if( VpRender->VPDrawRect.X + VpRender->VPDrawRect.Width> ScreenWidth) 
    {
        VpRender->VPDrawRect.Width=  ScreenWidth- VpRender->VPDrawRect.X;	
    }
	
    if( VpRender->VPDrawRect.Y + VpRender->VPDrawRect.Height > ScreenHeight) 
    {
        VpRender->VPDrawRect.Height =  ScreenHeight- VpRender->VPDrawRect.Y;	
    }
	
    // ���������򳤶Ȼ��߿��С�ڵ���0
    if( VpRender->VPDrawRect.Height<=0 || VpRender->VPDrawRect.Width<=0 ) return iFALSE;
	
    // ���ƿ�ʼ������VP�ұ�
    if( VpRender->VPDrawRect.X > (VpRender->VPActualRect.X + VpRender->VPActualRect.Width) )  return iFALSE;

    // ���ƽ���������VP���
    if( (VpRender->VPDrawRect.X + VpRender->VPDrawRect.Width) < VpRender->VPActualRect.X  )  return iFALSE;

    // ���ƿ�ʼ������VP�Ϸ�
    if( (VpRender->VPDrawRect.Y+ VpRender->VPDrawRect.Height) < VpRender->VPActualRect.Y )  return iFALSE;
	
    // ���ƽ���������VP�·�
    if( VpRender->VPDrawRect.Y > (VpRender->VPActualRect.Y + VpRender->VPActualRect.Height))  return iFALSE;
	
    i51CurrentVpManage.CurrentVpRender = VpRender;
    i51CurrentVpRender = VpRender;

    SysAlpha = AlphaSys;
    NumberDrawImage = VpRender->VPCurrentElement;
    for( iTemp=0; iTemp<NumberDrawImage; iTemp++ )
    {
        VPElement = &(VpRender->VPElementDetails[iTemp]);
        i51KitG2SetAlpha(VPElement->ElemenAlpha);
        switch( VPElement->ElementType )
        {
            case I51KITG2_ELEMENT_TYPE_PICTURE :
                switch( VPElement->ElementData.Image.DrawDirection)
                {
                    case I51KITG2_DRAW_IAMGE_NORMAL:
                        i51KitG2DrawImage(VPElement->ElementData.Image.ContainerHandle,
							VPElement->ElementData.Image.ImageHandle,
							&(VPElement->ElementData.Image.DesRect),
							&(VPElement->ElementData.Image.SrcRect) );
                        break;
                    case I51KITG2_DRAW_IAMGE_MIRROR :
                        i51KitG2MirrorImage(VPElement->ElementData.Image.ContainerHandle,
							VPElement->ElementData.Image.ImageHandle,
							&(VPElement->ElementData.Image.DesRect),
							&(VPElement->ElementData.Image.SrcRect) );
                        break;						
                    case I51KITG2_DRAW_IAMGE_HANDSTAND :
                        i51KitG2FlipImage(VPElement->ElementData.Image.ContainerHandle,
							VPElement->ElementData.Image.ImageHandle,
							&(VPElement->ElementData.Image.DesRect),
							&(VPElement->ElementData.Image.SrcRect) );
                        break;						
                    case I51KITG2_DRAW_IAMGE_ROTATE:
                        i51KitG2RotateImage(VPElement->ElementData.Image.ContainerHandle,
							VPElement->ElementData.Image.ImageHandle,
							&(VPElement->ElementData.Image.DesRect),
							&(VPElement->ElementData.Image.SrcRect),
							VPElement->ElementData.Image.SrcMoveX,
							VPElement->ElementData.Image.SrcMoveY,
							VPElement->ElementData.Image.RotateCenterX,
							VPElement->ElementData.Image.RotateCenterY,
							VPElement->ElementData.Image.RotateType,
							(VPElement->ElementData.Image.DarwAngle ) );
                        break;						
                    default :
                       goto ERROR;
                        break;						
                }
                break;
            case I51KITG2_ELEMENT_TYPE_POINT :
                i51KitG2DrawPoint(VPElement->ElementData.Point.X,
    				VPElement->ElementData.Point.Y,
    				VPElement->ElementData.Point.Color );
                break;
            case I51KITG2_ELEMENT_TYPE_LINE :
                i51KitG2DrawLine( VPElement->ElementData.Line.Points,
    				VPElement->ElementData.Line.Color );
                break;	
            case I51KITG2_ELEMENT_TYPE_POLYGON :
                i51KitG2DrawPolygon( VPElement->ElementData.Polygon.VerticeNumber,
    				(iS32(*)[2])VPElement->ElementData.Polygon.PointerPoints,
    				VPElement->ElementData.Polygon.Color );
                break;	
            case I51KITG2_ELEMENT_TYPE_ARC:
                i51KitG2DrawArc( VPElement->ElementData.Arc.Points ,
    				VPElement->ElementData.Arc.Color );
                break;	
            case I51KITG2_ELEMENT_TYPE_CIRCLE:
                i51KitG2DrawCircle( VPElement->ElementData.Circle.X  ,
    				VPElement->ElementData.Circle.Y,
    				VPElement->ElementData.Circle.R,
    				VPElement->ElementData.Circle.Color);
                break;	
            case I51KITG2_ELEMENT_TYPE_FILLPOLYGON :
                i51KitG2FillPolygon( VPElement->ElementData.FillPolygon.VerticeNumber,
    				(iS32(*)[2])(VPElement->ElementData.FillPolygon.PointerPoints),
    				VPElement->ElementData.FillPolygon.Color );
                break;	
            case I51KITG2_ELEMENT_TYPE_FILLARC :
                i51KitG2FillArc(VPElement->ElementData.FillArc.Points ,
    				VPElement->ElementData.FillArc.Color );
                break;	
            case I51KITG2_ELEMENT_TYPE_FILLCRICLE :
                i51KitG2FillCircle( VPElement->ElementData.FillCircle.X  ,
    				VPElement->ElementData.FillCircle.Y,
    				VPElement->ElementData.FillCircle.R,
    				VPElement->ElementData.FillCircle.Color);
                break;	
            case I51KITG2_ELEMENT_TYPE_WORDS :
                i51KitG2FontDraw(
					VPElement->ElementData.Words.ReFileHandle,
					VPElement->ElementData.Words.TypeheadID,
					(void *)VPElement->ElementData.Words.WordsHandle,
					VPElement->ElementData.Words.WordsNum,
					&(VPElement->ElementData.Words.DrawRect),
					&(VPElement->ElementData.Words.SrcRect),
					VPElement->ElementData.Words.DrawDirect,
					VPElement->ElementData.Words.WordsRowGap,
					VPElement->ElementData.Words.WordsColumnGap,
					VPElement->ElementData.Words.fgcolor,
					VPElement->ElementData.Words.bgcolor,
					VPElement->ElementData.Words.Effect);
				break;
            case I51KITG2_ELEMENT_TYPE_FILLIMAGE :
                i51KitG2ImageFillRect( VPElement->ElementData.Fill.ContainerHandle,
					VPElement->ElementData.Fill.ImageHandle,
					&(VPElement->ElementData.Fill.DesRect),
					&(VPElement->ElementData.Fill.SrcRect) );
                break;
				
            default :
                goto ERROR;
                break;
        }		
    }
  
    i51DrawVirtualPanel = iTRUE;

#if i51KITG2_DEBUG
    iLog( "G2VPRender end, %x,%d", vpanel,DrawRect);
#endif

   i51KitG2SetAlpha(SysAlpha);
   return iTRUE;
ERROR:
    iLog( "KIT - G2 : ElementData");
    i51KitG2SetAlpha(SysAlpha);
    return iFALSE;
	
}

iKIT iBOOL i51KitG2Update( void )
{
    // ʵ�־ֲ�ˢ����Ҫ������������
    i51AdeMmiUpdateScreen();
    return iTRUE;
}

#if DEBUG_I51KITG2
#if 0
enum{
	BG,
	RUN_LEFT,
	RUN_RIGHT,
	WALL,
	BEE,
	LEAF,
	ORANGE,
	PINEAPPLE,
	STRAWBERRY,
	WATERMELON,
	APPLE,
	BOMB,
	FRUIT_BG1,
	FRUIT_BG2,
	REDBLUE_FISH
};
#endif
enum{
	BG,	
	RUN_LEFT,	
	RUN_RIGHT,	
	WALL,	
	BEE,	
	SPRITE,	
	prizenum,	
	shayu,
	test1,
	LEAF,
	ORANGE,
	PINEAPPLE,
	STRAWBERRY,
	WATERMELON,
	APPLE,
	BOMB,
	FRUIT_BG1,
	FRUIT_BG2,
	REDBLUE_FISH
};


void TestDrawLine( void )
{

  static iS32 VertexPosition = 1;
  static iS32 Point[2][2] = {-10,-10,250,-10};

  iCOLOR Color = 0X00;

  Color = i51KitG2COLOR( 0XFF, 0XFF, 0 );

    memset ( LcdBuffer , 0 , 240*320*2 ) ;
  
    i51KitG2DrawLine( Point, Color);
    i51AdeMmiUpdateScreen  () ;

    switch(VertexPosition)
    {
    case 1:
      if( Point[1][1]<ScreenHeight+10 )
      {
        Point[1][1]++;
      }
      else
      {
        Point[1][0]--;
        if( Point[1][0]<-10 )
        {
          VertexPosition = 2;
          Point[1][0] = 0;
          Point[1][1] = ScreenHeight +10;
        }
      }
      break;
    case 2:
      if( Point[0][0]<ScreenWidth+10 )
      {
        Point[0][0]++;
      }
      else
      {
        Point[0][1]++;
        if(Point[0][1]>ScreenHeight+10)
        {
          VertexPosition = 3;
          Point[0][0] = ScreenWidth +10;
          Point[0][1] = ScreenHeight +10;
        }
      }
      break;
    case 3:
      if( Point[1][1]>-10 )
      {
        Point[1][1]--;
      }
      else
      {
        Point[1][0]++;
        if(Point[1][0]>ScreenWidth+10)
        {
          VertexPosition = 4;
          Point[1][0] = ScreenWidth +10;
          Point[1][1] = -10;
        }
      }
      break;
    case 4:
      if( Point[0][0]>-10 )
      {
        Point[0][0]--;
      }
      else
      {
        Point[0][1]--;
        if(Point[0][1]<-10)
        {
          VertexPosition = 1;
          Point[0][0] = -10;
          Point[0][1] = -10;
        }
      }
      break;
    default :
      break;
  }
}

void TestDrawPolygon( void )
{

  iU32 VerticeNumber =0;
  iS32 Point[10][2];
  iCOLOR Color;

  memset ( LcdBuffer , 0 , 240*320*2 ) ;

  VerticeNumber = 3;
  Point[0][0] = 40;
  Point[0][1] = 10;
  Point[1][0] = 20;
  Point[1][1] = 30;
  Point[2][0] = 60;
  Point[2][1] = 30;
  Color =  i51KitG2COLOR(0XFF, 0, 0);
  i51KitG2DrawPolygon( VerticeNumber, Point, Color);

  VerticeNumber = 4;
  Point[0][0] = 80;
  Point[0][1] = 10;
  Point[1][0] = 110;
  Point[1][1] = 15;
  Point[2][0] = 140;
  Point[2][1] = 50;
  Point[3][0] = 110;
  Point[3][1] = 50;
  Color =  i51KitG2COLOR(0XFF, 0XFF, 0);
  i51KitG2DrawPolygon( VerticeNumber, Point, Color);

  VerticeNumber = 5;
  Point[0][0] = 10;
  Point[0][1] = 100;
  Point[1][0] = 20;
  Point[1][1] = 90;
  Point[2][0] = 60;
  Point[2][1] = 120;
  Point[3][0] = 150;
  Point[3][1] = 160;
  Point[4][0] = 30;
  Point[4][1] = 120;
  Color =  i51KitG2COLOR(0XFF, 0XFF, 0XFF);
  i51KitG2DrawPolygon( VerticeNumber, Point, Color);

  i51AdeMmiUpdateScreen  () ;

}

void TestDrawArc( void )
{
  iS32 Point[3][2];
  iCOLOR Color;
  iS32 i,j;
  iS8 RegulationY = 1, RegulationX = 1;
  iS8 RegulationY1 = 1, RegulationX1= 1;

  Point[0][0] = 10;
  Point[0][1] = 100;
  Point[1][0] = 150;
  Point[1][1] = 30;
  Point[2][0] = 182;
  Point[2][1] = 76;

  Point[0][0] = 54;
  Point[0][1] = 133;
  Point[1][0] = 150;
  Point[1][1] = 30;
  Point[2][0] = 178;
  Point[2][1] = 226;
  Color =  i51KitG2COLOR(0XFF, 0, 0);

  while(1)
  {
    memset ( LcdBuffer , 0 , 240*320*2 ) ;

    Color =  i51KitG2COLOR(0XFF, 0, 0);
    i51KitG2DrawArc( Point, Color );

    Color =  i51KitG2COLOR(0XFF, 0XFF, 0XFF);
    LcdBuffer[ (ScreenWidth * Point[1][1] + Point[1][0])  ] = Color ;
//     i51AdeStdMemcpy16( &LcdBuffer[ (ScreenWidth * Point[1][1] + Point[1][0])  ], &Color, 2);

    if( Point[2][0]<0)
    {
      RegulationX = 1;

       Point[0][0] += RegulationX1;
      if( Point[0][0]>ScreenWidth )  RegulationX1 = -1;
      if( Point[0][0]<0) RegulationX1 = 1;

    }

    if( Point[2][1]<0)
    {

      RegulationY = 1;
       Point[0][1] += RegulationY1;
      if( Point[0][1]>ScreenHeight)  RegulationY1 = -1;
      if( Point[0][1]<0) RegulationY1 = 1;
    }

    if( Point[2][0]>ScreenWidth )
    {
       Point[0][0] += RegulationX1;
      if( Point[0][0]>ScreenWidth )  RegulationX1 = -1;
      if( Point[0][0]<0) RegulationX1 = 1;
    
      RegulationX = -1;
    }
    if( Point[2][1]>ScreenHeight)
    {
      Point[0][1] += RegulationY1;
      if( Point[0][1]>ScreenHeight)  RegulationY1 = -1;
      if( Point[0][1]<0) RegulationY1 = 1;

      RegulationY = -1;
    }
    
    Point[2][0] += RegulationX;
    Point[2][1] += RegulationY;
    i51AdeMmiUpdateScreen  () ;
    for( i=0;i<1000;i++ )
      for( j=0;j<500;j++);
  };
}

void TestDarwCircle( void )
{
  iS32 CenterX = 110;
  iS32 CenterY = 110;
  iU32 CircleR = 10;
  iS32 i,j;
  iCOLOR Color =  i51KitG2COLOR(0XFF, 0, 0);

  iS8 RegulationY = 1, RegulationX = 1, RegulationR = 1;

  while(1)
  {

    memset ( LcdBuffer , 0 , 240*320*2 ) ;

    i51KitG2DrawCircle( CenterX, CenterY, CircleR, Color);
  
    i51AdeMmiUpdateScreen  () ;
  
    CircleR += RegulationR;
    CenterX += RegulationX;
    CenterY += RegulationY;

    if( CircleR<10 )
    {
      RegulationR = 1;
    }
    else if( CircleR > 100 )
    {
      RegulationR = -1;
    }

    if( CenterX<0 )
    {
      RegulationX = 1;
    }
    else if( CenterX > ScreenWidth )
    {
      RegulationX = -1;
    }

    if( CenterY<0 )
    {
      RegulationY = 1;
    }
    else if( CenterY > ScreenWidth )
    {
      RegulationY = -1;
    }
    for( i=0;i<1000;i++ )
      for( j=0;j<500;j++);
  }
}

void TestFillPolygon( void )
{
  iU32 VerticeNumber =0;
  iS32 Point[10][2];
  iCOLOR Color;
  iRECT DesRect;

  Point[0][0] = 9;
  Point[0][1] = -8;
  Point[1][0] = 209;
  Point[1][1] = -8;
  Point[2][0] = 209;
  Point[2][1] = 192;
  Point[3][0] = 9;
  Point[3][1] = 192;
  i51KitG2CleanScreen(0XAAAA,iNULL);
  Color =  i51KitG2COLOR(0XFF, 0, 0);
  i51KitG2FillPolygon( 4, Point, Color);
  i51KitG2Update();
//return ;

  DesRect.X =100;
  DesRect.Y = 100;
  DesRect.Width = 100;
  DesRect.Height = 100;
  i51KitG2CleanScreen(0XAAAA,iNULL);
  VerticeNumber = 3;
  Point[0][0] = 40;
  Point[0][1] = 10;
  Point[1][0] = 20;
  Point[1][1] = 30;
  Point[2][0] = 60;
  Point[2][1] = 30;
  Color =  i51KitG2COLOR(0XFF, 0, 0);
  i51KitG2FillPolygon( VerticeNumber, Point, Color);

  VerticeNumber = 4;
  Point[0][0] = 80;
  Point[0][1] = 10;
  Point[1][0] = 110;
  Point[1][1] = 15;
  Point[2][0] = 140;
  Point[2][1] = 50;
  Point[3][0] = 110;
  Point[3][1] = 50;
  Color =  i51KitG2COLOR(0XFF, 0XFF, 0);
  i51KitG2FillPolygon( VerticeNumber, Point, Color);
  i51KitG2DrawPolygon( VerticeNumber, Point, Color);

  VerticeNumber = 7;
  Point[0][0] = 120;
  Point[0][1] = 70;
  Point[1][0] = 220;
  Point[1][1] = 50;
  Point[2][0] = 250;
  Point[2][1] = 170;
  Point[3][0] = 120;
  Point[3][1] = 150;
  Point[4][0] = 200;
  Point[4][1] = 130;
  Point[5][0] = 120;
  Point[5][1] = 110;
  Point[6][0] = 200;
  Point[6][1] = 90;
  Color =  i51KitG2COLOR(0XF0, 0X0F, 0XF0);
  i51KitG2FillPolygon( VerticeNumber, Point, Color);
  i51KitG2DrawPolygon( VerticeNumber, Point, Color);

  VerticeNumber = 7;
  Point[0][0] = 10;
  Point[0][1] = 300;
  Point[1][0] = 40;
  Point[1][1] = 200;
  Point[2][0] = 70;
  Point[2][1] = 250;
  Point[3][0] = 90;
  Point[3][1] = 201;
  Point[4][0] = 110;
  Point[4][1] = 251;
  Point[5][0] = 130;
  Point[5][1] = 205;
  Point[6][0] = 150;
  Point[6][1] = 330;
  Color =  i51KitG2COLOR(0XFF, 0XFF, 0XFF);
  i51KitG2FillPolygon( VerticeNumber, Point, Color);
  i51KitG2DrawPolygon( VerticeNumber, Point, Color);

  i51AdeMmiUpdateScreen  () ;

}

void TestFillArc( void )
{
  iS32 i,j;
  iCOLOR Color;
  iS32 Point[3][2];
  iS8 RegulationY = 1, RegulationX = 1;
  iS8 RegulationY1 = 1, RegulationX1= 1;

  Point[0][0] = 10;
  Point[0][1] = 100;
  Point[1][0] = 150;
  Point[1][1] = 30;
  Point[2][0] = 182;
  Point[2][1] = 76;

  Point[0][0] = 54;
  Point[0][1] = 133;
  Point[1][0] = 150;
  Point[1][1] = 30;
  Point[2][0] = 178;
  Point[2][1] = 226;
  Color =  i51KitG2COLOR(0XFF, 0, 0);
//   i51AdeStdMemcpy16( &LcdBuffer[ (ScreenWidth * Point[1][1] + Point[1][0]) << 1 ], &Color, 2);


  while(1)
  {
    memset ( LcdBuffer , 0 , 240*320*2 ) ;

    Color =  i51KitG2COLOR(0XFF, 0, 0);
    i51KitG2FillArc( Point, Color );
    Color =  i51KitG2COLOR(0XFF, 0XFF, 0XFF);
     i51AdeStdMemcpy16( &LcdBuffer[ (ScreenWidth * Point[1][1] + Point[1][0]) ], &Color, 2);
    i51AdeMmiUpdateScreen  () ;

    if( Point[2][0]<0)
    {
      RegulationX = 1;

       Point[0][0] += RegulationX1;
      if( Point[0][0]>ScreenWidth )  RegulationX1 = -1;
      if( Point[0][0]<0) RegulationX1 = 1;

    }

    if( Point[2][1]<0)
    {

      RegulationY = 1;
       Point[0][1] += RegulationY1;
      if( Point[0][1]>ScreenHeight)  RegulationY1 = -1;
      if( Point[0][1]<0) RegulationY1 = 1;
    }

    if( Point[2][0]>ScreenWidth )
    {
       Point[0][0] += RegulationX1;
      if( Point[0][0]>ScreenWidth )  RegulationX1 = -1;
      if( Point[0][0]<0) RegulationX1 = 1;
    
      RegulationX = -1;
    }
    if( Point[2][1]>ScreenHeight)
    {
      Point[0][1] += RegulationY1;
      if( Point[0][1]>ScreenHeight)  RegulationY1 = -1;
      if( Point[0][1]<0) RegulationY1 = 1;

      RegulationY = -1;
    }
    
    Point[2][0] += RegulationX;
    Point[2][1] += RegulationY;
    for( i=0;i<1000;i++ )
      for( j=0;j<100;j++);
  }
}
void TestFillCircle( void )
{
  iS32 CenterX = 120;
  iS32 CenterY = 160;
  iU32 CircleR = 120;
  iS32 i,j;
static  iCOLOR Color =  i51KitG2COLOR(0XFF, 0, 0);

  iS8 RegulationY = 1, RegulationX = 1, RegulationR = 1;


    memset ( LcdBuffer , 0 , 480*320*2 ) ;
for(i=0;i<16;i++)
for(j=0;j<24;j++)
{
    i51KitG2FillCircle( 10+20*i, 10+20*j, 10, Color);
}
  Color+=5000;
    i51AdeMmiUpdateScreen  () ;

}

void Process(iU32 Step , iU32 Totall , iHANDLE Handle)
{
   // i51KitG2UseImageContainer(Handle);
//   CurrentImageContainer = Handle;
	if(Handle)
	{
		DecodeComplete = iFALSE;
    memset ( LcdBuffer , 0 , 240*320*2 ) ;
		i51KitG2DrawImage(Handle, Step-1, iNULL, iNULL);
		i51KitG2Update();
	}
	else
		DecodeComplete = iTRUE;
}

void Done( iHANDLE Handle)
{
	if(Handle)
	{
		CurrentImageContainer = Handle;
		DecodeComplete = iTRUE;
//		i51KitG2CloseImageContainer(Handle);
//		CurrentImageContainer = i51KitG2OpenImageContainer(L"i51KitG2\\i51IMIC");
//             i51KitG2SetImageMemoryCopy(CurrentImageContainer,0,iNULL);
	}
	else
		DecodeComplete = iFALSE;
}

void TestCreateImageContainer( void )
{
static iS32 ImageId = 0;
if( FLAG_FIRSTRUN )
{
	iU16 * Path = L"i51KitG2\\i51IMIC";
	iFILE  Test = i51KitStdResOpen ( L"i51KitG2\\i51KitG2.re" ) ;
	iU32 ImageId[11] = {0,1,2,3,4,5,6,7,8};

    if(i51KitG2CreateImageContainer ( Test , ImageId , 9 , Path , Process , Done ))
    {
    #if DEBUG_I51KITG2
        iLog("���������ɹ�.\n");
    #endif
    }
FLAG_FIRSTRUN = iFALSE;
}
if(CurrentImageContainer)
{
	i51KitG2DrawImage(CurrentImageContainer, (ImageId/30)%9, iNULL, iNULL);
	i51KitG2Update();
	ImageId++;
}
}

void TestOpenImageContainer( void )
{
  iU16 * Path = L"i51KitG2\\i51IMIC";
  if(i51KitG2OpenImageContainer(Path))
  {
  #if DEBUG_I51KITG2
        iLog("�������ɹ�.\n");
  #endif
  }
}

void TestCloseImageContainer( void )
{
//    iU16 * Path = L"i51KitG2\\i51IMIC";
//    CurrentImageContainer = i51KitG2OpenImageContainer(Path);

#if DEBUG_I51KITG2
//    if( CurrentImageContainer ) iLog("�������ɹ�.\n");
#endif

    if( i51KitG2CloseImageContainer( (iHANDLE)(&CurrentImageContainer) ))
    {
    #if DEBUG_I51KITG2
        iLog("�ر������ɹ�.\n");
    #endif
    }
}

void TestDestroyImageContainer( void )
{
    iU16 * Path = L"i51KitG2\\i51IMIC";
    iHANDLE ImageContainer;

    if( (ImageContainer=i51KitG2OpenImageContainer(Path))!=iFALSE)
    {
       iLog("�������ɹ�.\n");
    }
    if( i51KitG2DestroyImageContainer( ImageContainer, Path) )
    {
       iLog(" ���������ɹ�.\n");
    }
}

void TestDrawImage ( void )
{
	iU16 * Path = L"i51KitG2\\i51IMIC";
	static iFILE  Test ;
//	iU32 ImageId[5] = {BG,RUN_LEFT,RUN_RIGHT,WALL,BEE};
	iU32 ImageId[1] = {BEE};
       iU32 Ver = 0;

	if( FLAG_FIRSTRUN)
	{
		FLAG_FIRSTRUN = iFALSE;
		if( i51AdeOsFileIsExist(L"i51KitG2\\i51IMIC") )
		{
		       if(iFALSE==i51KitG2CheckImageContainer(L"i51KitG2\\i51IMIC")) return;
			   
			CurrentImageContainer = i51KitG2OpenImageContainer(Path);
			i51KitG2SetImageMemoryCopy(CurrentImageContainer,0,iNULL);
			DecodeComplete = iTRUE;
		}
		else
		{
			Test = i51KitStdResOpen ( L"i51KitG2\\i51KitG2.re" ) ;
			i51KitG2CreateImageContainer ( Test , ImageId , 1 , Path ,  Process , Done );
		}
	}
	
    memset ( LcdBuffer , 0XFF , 240*320*2 ) ;

	if(DecodeComplete)
	{
		i51KitG2VPDisable();
		i51KitG2DrawImage(CurrentImageContainer ,BEE, iNULL, iNULL);
	}
	i51KitG2Update();

}

#define TestMirror 	10
#define nin_l_j	11

void TestRotateImage ( void )
{
    //    iU16 * Path = L"E:\\i51\\i51APP\\i51KitG2\\";
    #define MAX_FRUIT 10
    iU16 * Path = L"i51KitG2\\i51IMIC";
    iRECT SrcRect, SrcRect1, DesRect1;
    static iS32 Angle = 0;
    static iS32 SrcMoveX[MAX_FRUIT] = {0,80,160,0,80,160,0,80,160,90};
    static iS32 SrcMoveY[MAX_FRUIT] = {10,10,10,90,90,90,170,170,170,250};
    static iS32 Xadds[MAX_FRUIT] = {0};
    static iS32 Yadds[MAX_FRUIT] = {0};
    static iS32 Aadds[MAX_FRUIT] = {15,-18,15,-15,10,-15,15,-10,18,-15};
    static iS32 MaxX[MAX_FRUIT] = {0};
	static iS32 CurrImage[MAX_FRUIT] = {0};
    iU32 ImageId[2] = {nin_l_j};
    static iFILE  Test ;
    i51ImageIndex * ImageIndexTable = iNULL;
    i51ImageContainer * ImageContainer = iNULL;
    iS32 iTemp =0;	
	if( FLAG_FIRSTRUN)
	{
		FLAG_FIRSTRUN = iFALSE;
		if( i51AdeOsFileIsExist(L"i51KitG2\\i51IMIC") )
		{
//		       if(iFALSE==i51KitG2CheckImageContainer(L"i51KitG2\\i51IMIC")) return;
			   
			CurrentImageContainer = i51KitG2OpenImageContainer(Path);
//			if(ScreenWidth>240){ ImageId[12] = ImageId[13]; ImageId[13] = ImageId[14];}
			i51KitG2SetImageMemoryCopy(CurrentImageContainer,0,iNULL);
			
			DecodeComplete = iTRUE;
		}
		else
		{
			Test = i51KitStdResOpen ( L"i51KitG2\\i51KitG2.re" ) ;
			i51KitG2CreateImageContainer ( Test , ImageId , 1, Path ,  Process , Done );
		}

	}

if(DecodeComplete)
{
    if( iNULL== CurrentImageContainer) return;
    ImageContainer = (i51ImageContainer *)CurrentImageContainer;
    ImageIndexTable = ( i51ImageIndex *  )(ImageContainer->ImageIndexTable);

SrcRect.X = 10;
SrcRect.Y= 10;
SrcRect.Width = 80;
SrcRect.Height = 80;

DesRect1.X = 100;
DesRect1.Y = 100;
DesRect1.Width= 100;
DesRect1.Height= 100;

Angle++;
i51KitG2CleanScreen(0XFFFF, iNULL);
i51KitG2RotateImage( CurrentImageContainer, 0, 0, 0,100,100,120,160,0,270);
//i51KitG2DrawImage(CurrentImageContainer, TestMirror, iNULL, iNULL);
//i51KitG2MirrorImage(CurrentImageContainer, nin_l_j, iNULL, iNULL);
}
    i51AdeMmiUpdateScreen  () ;

}


void TestDrawGraph( void )
{
	iS32 Point[10][2];
	iS32 VerticeNumber = 0;

	iCOLOR Color = 0XFFFF;

//	if( FLAG_FIRSTRUN )
	{
//		FLAG_FIRSTRUN = iFALSE;
		Point[0][0] = 0;
		Point[0][1] = 0;
		Point[1][0] = 239;
		Point[1][1] = 0;
		Point[2][0] = 239;
		Point[2][1] = 319;
		Point[3][0] = 0;
		Point[3][1] =  319;
		Color = i51KitG2COLOR( 0, 0, 0 );
//		i51KitG2FillPolygon( 4, Point, Color);
		
		Color = i51KitG2COLOR( 0XFF, 0, 0 );
		i51KitG2DrawPoint( 105, 80, Color)  ;
		i51KitG2DrawPoint( 110, 80, Color)  ;
		i51KitG2DrawPoint( 115, 80, Color)  ;
		i51KitG2DrawPoint( 120, 80, Color)  ;

		Point[0][0] = 15;
		Point[0][1] = 12;
		Point[1][0] = 115;
		Point[1][1] = 112;
		Color = i51KitG2COLOR( 0, 0XFF, 0 );
		i51KitG2DrawLine( Point, Color);
		
		VerticeNumber = 7;
		Point[0][0] = 120;
		Point[0][1] = 70;
		Point[1][0] = 220;
		Point[1][1] = 50;
		Point[2][0] = 220;
		Point[2][1] = 170;
		Point[3][0] = 120;
		Point[3][1] = 150;
		Point[4][0] = 200;
		Point[4][1] = 130;
		Point[5][0] = 120;
		Point[5][1] = 110;
		Point[6][0] = 200;
		Point[6][1] = 90;
		Color =  i51KitG2COLOR(0XF0, 0X0F, 0XF0);
		i51KitG2FillPolygon( VerticeNumber, Point, Color);
		
		VerticeNumber = 7;
		Point[0][0] = 10;
		Point[0][1] = 300;
		Point[1][0] = 40;
		Point[1][1] = 200;
		Point[2][0] = 70;
		Point[2][1] = 250;
		Point[3][0] = 90;
		Point[3][1] = 201;
		Point[4][0] = 110;
		Point[4][1] = 251;
		Point[5][0] = 130;
		Point[5][1] = 205;
		Point[6][0] = 150;
		Point[6][1] = 300;
		Color =  i51KitG2COLOR(0XF0, 0X0F, 0X0F);
		i51KitG2DrawPolygon( VerticeNumber, Point, Color);

		Point[0][0] = 70;
		Point[0][1] = 130;
		Point[1][0] = 120;
		Point[1][1] = 210;
		Point[2][0] = 170;
		Point[2][1] = 130;
		Color =  i51KitG2COLOR(0X0F, 0X0F, 0XFF);
		i51KitG2FillArc( Point, Color);
		i51KitG2DrawArc( Point, 0);

		Color =  i51KitG2COLOR(0XFF, 0X0F, 0X0F);
		i51KitG2DrawCircle( 50, 100, 30, Color);
		Color =  i51KitG2COLOR(0X0F, 0XFF, 0X0F);
		i51KitG2FillCircle( 50, 160, 30, Color);
	}		
}


void TestInit(void)
{
	iU16 * Path = L"i51KitG2\\i51IMIC";
	static iFILE  Test ;
//	iU32 ImageId[15] = {BG,RUN_LEFT,RUN_RIGHT,WALL,BEE,LEAF,
//	ORANGE,PINEAPPLE,STRAWBERRY,WATERMELON,APPLE,	BOMB,FRUIT_BG1,FRUIT_BG2,REDBLUE_FISH
//	};
	iU32 ImageId[9] = {BG,RUN_LEFT,RUN_RIGHT,WALL,BEE,SPRITE,prizenum,shayu,test1};
	
	if( FLAG_FIRSTRUN)
	{
		FLAG_FIRSTRUN = iFALSE;
		if( i51AdeOsFileIsExist(L"i51KitG2\\i51IMIC") )
		{
		       if(iFALSE==i51KitG2CheckImageContainer(L"i51KitG2\\i51IMIC")) return;
			   
			CurrentImageContainer = i51KitG2OpenImageContainer(Path);
//			i51KitG2SetImageMemoryCopy(CurrentImageContainer, iNULL, iNULL);
//			if(ScreenWidth>240) ImageId[12] = ImageId[13];
			DecodeComplete = iTRUE;
		}
		else
		{
			Test = i51KitStdResOpen ( L"i51KitG2\\i51KitG2.re" ) ;
			i51KitG2CreateImageContainer ( Test , ImageId , 9 , Path ,  Process , Done );
		}
//i51KernelSetCache(L"i51KitG2");
	}
}

void TestImage( void )
{
	iU16 * Path = L"i51KitG2\\i51IMIC";
	static iFILE  Test ;
	iRECT DesRect;
	iRECT SrcRect;

	if(FLAG_FIRSTRUN)    TestInit();

	if(DecodeComplete)
	{
//		i51KitG2DrawImage(CurrentImageContainer,BG, iNULL, iNULL);
//		i51KitG2SetAlpha(128);
//		i51KitG2DrawImage(CurrentImageContainer,shayu, iNULL, iNULL);
//		i51KitG2SetAlpha(0XFF);
//		i51KitG2MirrorImage(CurrentImageContainer,shayu, iNULL, iNULL);
//		i51KitG2FlipImage(CurrentImageContainer,shayu, iNULL, iNULL);
i51KitG2DrawImage( CurrentImageContainer, 0, 0, 0);
SrcRect.X = -20;
SrcRect.Y = 100;
SrcRect.Width = 240;
SrcRect.Height = 150;
i51KitG2DrawImage( CurrentImageContainer, 0, &SrcRect, 0);
SrcRect.Y = 150;
i51KitG2MirrorImage( CurrentImageContainer, 0, &SrcRect, 0);
SrcRect.X = 260;
SrcRect.Y = 200;
SrcRect.Width = 240;
SrcRect.Height = 150;
i51KitG2DrawImage( CurrentImageContainer, 0, &SrcRect, 0);
SrcRect.Y = 250;
i51KitG2MirrorImage( CurrentImageContainer, 0, &SrcRect, 0);
//		i51KitG2RotateImage(CurrentImageContainer,shayu, iNULL, &SrcRect,50,85,120,160,0,45);
//		i51KitG2SetAlpha(0XFF);
//		i51KitG2FlipImage(CurrentImageContainer,3, iNULL, iNULL);
//		i51KitG2MirrorImage(CurrentImageContainer,3, iNULL, iNULL);
		DesRect.X = 42;
		DesRect.Y = 50;
		DesRect.Width = 156;
		DesRect.Height = 196;
//		i51KitG2MirrorImage(CurrentImageContainer,1, &DesRect, iNULL);
//		i51KitG2MirrorImage(CurrentImageContainer,7, iNULL, iNULL);
//		i51KitG2DrawImage(CurrentImageContainer,CurrentImageContainer,2, &DesRect, iNULL);

		DesRect.X = 80;
		DesRect.Y = 0;
		DesRect.Width= ScreenWidth;
		DesRect.Height= ScreenHeight;
//		i51KitG2FlipImage(CurrentImageContainer,7, &DesRect, iNULL);
//		i51KitG2FlipImage(CurrentImageContainer,7,iNULL, iNULL);

		DesRect.X = 0;
		DesRect.Y = 280;
//		i51KitG2SetRotateCenter(DesRect.X, DesRect.Y);
//		i51KitG2RotateImage(CurrentImageContainer,1, &DesRect , iNULL, 45);
//i51KitG2DrawImage(CurrentImageContainer, test1, iNULL, iNULL);
		
		i51AdeMmiUpdateScreen  () ;
//		DecodeComplete = iFALSE;

	}
}

void TestVP( void)
{
	iRECT Rect, SrcRect, DesRect;
	static iS32 VP_X1 =0, VP_Y1 = 0;
	static iS32 VP_X2 =0, VP_Y2 = 0;
	static iS32 VP_X3 =0, VP_Y3 = 0;
	static iS16 SpikeX = 5, SpikeY = 5;
	static iS16 DrawImage = iFALSE;
	static iS16 SymbolNumber1= 0;
	static iS16 SymbolNumber2= 0;
	static iS16 BeeDir= 0;
	i51ImageIndex * ImageIndexTable = iNULL;
	i51ImageContainer * ImageContainer = iNULL;
	iS32 iTemp =0;	

	if(FLAG_FIRSTRUN)    TestInit();
	
	memset ( LcdBuffer , 0XFF , 240*320*2 ) ;

	if(DecodeComplete)
	{
		i51KitG2VPEnable();
		Rect.X = 0;
		Rect.Y = 0;
		Rect.Width = 240;
		Rect.Height = 640;
		VP_BK = i51KitG2VPCreate(2, &Rect);
		i51KitG2VPSetActivity(VP_BK);
//		i51KitG2SetVPiSLoop(VP_BK, iTRUE);
//    i51KitG2SetAlpha(0);
		i51KitG2DrawImage(CurrentImageContainer ,BG, iNULL, iNULL);
		DesRect.X = 0;
		DesRect.Y = 320;
		DesRect.Width = 240;
		DesRect.Height = 320;
		i51KitG2DrawImage(CurrentImageContainer ,BG, &DesRect, iNULL);

//    i51KitG2SetAlpha(150);
		Rect.X = 0;
		Rect.Y = 0;
		Rect.Width = 240;
		Rect.Height = 480;
		VP_Road = i51KitG2VPCreate(6, &Rect);
		i51KitG2VPSetActivity(VP_Road);
//		i51KitG2SetVPiSLoop(VP_Road, iTRUE);
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = 24;
		SrcRect.Height = 160;
		DesRect.X = 0;
		DesRect.Y = 0;
		DesRect.Width = 24;
		DesRect.Height = 160;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		DesRect.Y = 160;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		DesRect.Y = 320;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		SrcRect.X = 24;
		DesRect.X = 216;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		DesRect.Y = 160;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);
		DesRect.Y = 0;
		i51KitG2DrawImage(CurrentImageContainer ,WALL, &DesRect, &SrcRect);

		VP_Snidget = i51KitG2VPCreate(2, &Rect);
		i51KitG2VPSetActivity(VP_Snidget);
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = 25;
		SrcRect.Height = 39;
		DesRect.X = 24;
		DesRect.Y = 250;
		DesRect.Width = 25;
		DesRect.Height = 39;
		i51KitG2DrawImage(CurrentImageContainer ,RUN_LEFT, &DesRect, &SrcRect);
		DesRect.X = 191;
		i51KitG2DrawImage(CurrentImageContainer ,RUN_RIGHT, &DesRect, &SrcRect);

		Rect.X = 70;
		Rect.Y = 319;
		Rect.Width = 111;
		Rect.Height = 71;
		VP_Y3 = 319;
		VP_X3 = 70;
		VP_Bee= i51KitG2VPCreate(2, &Rect);
		i51KitG2VPSetActivity(VP_Bee);
		SrcRect.X = 0;
		SrcRect.Y = 0;
		SrcRect.Width = 111;
		SrcRect.Height = 71;
		DesRect.X = 0;
		DesRect.Y = 0;
		DesRect.Width = 25;
		DesRect.Height = 39;
		i51KitG2DrawImage(CurrentImageContainer ,BEE, &DesRect, &SrcRect);

		i51KitG2VPRender( VP_BK,iNULL);
		FLAG_FIRSTRUN = iFALSE;
		DecodeComplete = iFALSE;
		DrawImage = iTRUE;
	}
if(DrawImage)
{
//    i51KitG2SetAlpha(0);
	DesRect.X = 0;
	DesRect.Y= 0;
	DesRect.Width= 240;
	DesRect.Height= 320;
#if 1	
	i51KitG2VPSetCoord(VP_BK, VP_X1, VP_Y1);
	i51KitG2VPRender( VP_BK,&DesRect);
	VP_Y1 += 2;
	if( (VP_Y1>=0) || VP_Y1<-320)
	{
//		SpikeY= -SpikeY;
		VP_Y1 = -320;
	}
#endif
#if 1
	i51KitG2VPSetCoord(VP_Road, VP_X2, VP_Y2);
	i51KitG2VPRender( VP_Road,&DesRect);

	VP_Y2 += 10;
	if( (VP_Y2>=0) || VP_Y2<-160)
	{
//		SpikeY= -SpikeY;
		VP_Y2 = -160;
	}
//  i51KitG2SetAlpha(150);
	i51KitG2VPRender( VP_Snidget,&DesRect);
	i51KitG2VPClean(VP_Snidget);
	i51KitG2VPSetActivity(VP_Snidget);
	SymbolNumber1 = (SymbolNumber1+1)%4;
	SrcRect.X = 0;
	SrcRect.Y = SymbolNumber1*39;
	SrcRect.Width = 25;
	SrcRect.Height = 39;
	DesRect.X = 24;
	DesRect.Y = 250;
	DesRect.Width = 25;
	DesRect.Height = 39;
	i51KitG2DrawImage(CurrentImageContainer ,RUN_LEFT, &DesRect, &SrcRect);
	DesRect.X = 191;
	i51KitG2DrawImage(CurrentImageContainer ,RUN_RIGHT, &DesRect, &SrcRect);

	DesRect.X = VP_X3;
	DesRect.Y = VP_Y3;
	DesRect.Width = 111;
	DesRect.Height = 71;
	i51KitG2VPRender( VP_Bee,&DesRect);
	i51KitG2VPClean(VP_Bee);
	i51KitG2VPSetActivity(VP_Bee);
	SymbolNumber2 = (SymbolNumber2+1)%2;
	SrcRect.X = 0;
	SrcRect.Y = SymbolNumber2*71;
	SrcRect.Width = 111;
	SrcRect.Height = 71;
	i51KitG2VPSetCoord(VP_Bee, VP_X3, VP_Y3);
	if(BeeDir)
	{
		i51KitG2FlipImage(CurrentImageContainer ,4, iNULL, &SrcRect);
		VP_Y3 += 8;
	}
	else
	{
		i51KitG2DrawImage(CurrentImageContainer ,4, iNULL, &SrcRect);
		VP_Y3 -= 8;
	}
	
	if( VP_Y3>=320 )
	{
		VP_Y3 -= 8;
		BeeDir = 0;
	}
	if(VP_Y3<= -71)
	{
		VP_Y3 += 8;
		BeeDir = 1;
	}
#endif
	i51KitG2Update();
}
}

 void MGdrawChars(char* data, int offset, int length, int x, int y, int style) {
  iRECT drawrec;
 // DDRect desrect,tmp;
//   DDFont font=getFontStyle(style);
  drawrec.X=x;
  drawrec.Y=y;
  drawrec.Width=ScreenWidth;
  drawrec.Height=ScreenHeight;
/*
  GetCollideRect(&tmp,&rectClip,&desrect);
  CopyMyRectToI51Rect(&desrect,& drawrec);*/


 
	i51KitG2FontDraw(TypeheadHandle,0,(void *)(data+offset), length,&drawrec, &drawrec,
		0,0,0, 0XFF00, 0, 0);


}

void TestFontDraw(void)
{
char * Str = "VRFFFFFFFFFFFHJJAJJUKRRDTWWRRRHHHHHHHHHHHHHHHHHHH";
int i;
i51KitG2CleanScreen(0, 0);
for( i=0; i<strlen(Str); i++ )
{
	MGdrawChars(Str, i, 1, i*14, (i/15)*16, 0);
}
i51KitG2Update();

#if 0
    static iCOLOR Color=0;
    iRECT DrawRect, SrcRect;
    iRECT * RectTemp = iNULL;
    static iHANDLE VP1 = iNULL;
    static iS16 X = 0,Y = 240;
    static iU32 OldTime = 0;	
    static iU8 State = 4;	
//    iU16 * Str = (iU16 *)"���ڶ�������Ƽ�";
//    iU16 * Str = (iU16 *)"�ҵ�һ���ڲ����к������д�Ϊ�ϸ���";
//    iU16 * Str = (iU16 *)"���οƼ����οƼ����οƼ�";
//    iU16 * Str = (iU16 *)"��һ���ڲ����к������д�Ϊ�ϸ�������Ҫ��ʱ���������������ڳ��ͷֶԳɻ�������궯ͬ��Ҳ���¹���˵�����������ඨ��ѧ������þ�ʮ��֮���ŵȲ��ȼҵ�������ˮ�����Զ�����С����ʵ�����������ƻ���ʹ���ҵ��ȥ���Ժ�Ӧ�����ϻ�������ЩȻǰ������������������ƽ����ȫ�������ظ��������������ķ�������ԭ��ô���Ȼ�������������˱���ֻû������⽨�¹���ϵ������������������ͨ����ֱ�⵳��չ�������Ա��λ�볣���ܴ�Ʒʽ���輰���ؼ�������ͷ���ʱ���·����ͼɽͳ��֪�Ͻ�����Ʊ����ֽ��ڸ�����ũָ������ǿ�ž�����������ս�Ȼ�����ȡ�ݴ����ϸ�ɫ���ż����α���ٹ������ߺ��ڶ�����ѹ־���������ý���˼����������ʲ������Ȩ��֤���强���ٲ�ת�������д�׽��ٻ������������������ÿĿ�����߻�ʾ��������������뻪��ȷ�ſ�������ڻ�������Ԫ�����´�����Ⱥ��ʯ������н������ɽ��Ҿ���Խ֯װӰ��ͳ������鲼���ݶ�����̷����������ѽ���ǧ��ί�ؼ��������ʡ��ϰ��Լ֧��ʷ���ͱ����������п˺γ���������̫׼��ֵ������ά��ѡ��д���ë�׿�Ч˹Ժ�齭�����������������ɲ�Ƭʼȴר״������ʶ����Բ����ס�����ؾ��ղκ�ϸ������������";
//    iU16 * Str = (iU16 *)"��һ���ڲ����к������д�Ϊ�ϸ�������Ҫ��ʱ���������������ڳ��ͷֶԳɻ�������궯ͬ��Ҳ���¹���˵�����������ඨ��ѧ������þ�ʮ��֮���ŵȲ��ȼҵ�������ˮ�����Զ�����С����ʵ�����������ƻ���ʹ���ҵ��ȥ���Ժ�Ӧ�����ϻ�������ЩȻǰ������������������ƽ����ȫ�������ظ��������������ķ�������ԭ��ô���Ȼ�������������˱���ֻû������⽨�¹���ϵ������������������ͨ����ֱ�⵳��չ�������Ա��λ�볣���ܴ�Ʒʽ���輰���ؼ�������ͷ���ʱ���·����ͼɽͳ��֪�Ͻ�����Ʊ����ֽ��ڸ�����ũָ������ǿ�ž�����������ս�Ȼ�����ȡ�ݴ����ϸ�ɫ���ż����α���ٹ������ߺ��ڶ�����ѹ־���������ý���˼����������ʲ������Ȩ��֤���强���ٲ�ת�������д�׽��ٻ������������������ÿĿ�����߻�ʾ��������������뻪��ȷ�ſ�������ڻ�������Ԫ�����´�����Ⱥ��ʯ������н������ɽ��Ҿ���Խ֯װӰ��ͳ������鲼���ݶ�����̷����������ѽ���ǧ��ί�ؼ��������ʡ��ϰ��Լ֧��ʷ���ͱ����������п˺γ���������̫׼��ֵ������ά��ѡ��д���ë�׿�Ч˹Ժ�齭�����������������ɲ�Ƭʼȴר״������ʶ����Բ����ס�����ؾ��ղκ�ϸ������������";
iU16 * Str = (iU16 *)"  1967��,�������ĵ�һ����Ŀ��multics,\
����һ������ϵͳ��Ŀ,������ڴ��ͼ��������������,\
������������Ŀ������Ӵ�,�ۺ���̫�������,multics�����ܺܵ�,\
������ʧ�ܶ�����.multics��Ŀ�Ŀ�����֮һ��.����ѷ�����Ϊ\
ge-5�������,�����ձ�д��һ���²���ϵͳ��Ŀ,\
�ܹ�֧��ͬʱ�Ķ��û�����,Ҳ���Ǻ�����unix.";
//   iCHAR * Str = "6.strlen�Ľ��Ҫ�����е�ʱ����ܼ������,�����������ַ����ĳ���,��������ռ�ڴ�Ĵ�С.";
//    iU16 * Str = (iU16 *)"�������ĵ�һ����Ŀ!!!!!!!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
//    iU16 * Str = (iU16 *)"�������ĵ�һ����Ŀ!!!!!!!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
//    iU16 * Str = (iU16 *)"Designed to give you the finest document-formatting tools, Word 2010 also helps you easily organize and write your documents more efficiently, and stay within reach so you can capture your best ideas whenever and wherever they occur.";
    iU8 i;	

if(FLAG_FIRSTRUN)
{
//    i51KitG2FontSetUse(i51KitStdResOpen(L"Test\\i51KitG2.re"), 1);
    FLAG_FIRSTRUN = iFALSE;
}
//    i51KitG2FontCreat((void *)Str, L"Fonts\\", L"SmallFive.i51F",L"Fonts\\", L"Creat.i51F");
//    i51AdeMmiUpdateScreen();
//    return ;
    DrawRect.X = 10;
    DrawRect.Y = 20;
    DrawRect.Width = 120;
    DrawRect.Height= 220;

    SrcRect.X = 10;
    SrcRect.Y = 220;
    SrcRect.Width= 120;
    SrcRect.Height= 220;
	
#if 0
if(VP1==iNULL)
{
    VP1 = i51KitG2VPCreate(4, &DrawRect);
    i51KitG2VPSetActivity(VP1);
    DrawRect.Y = 20;
    DrawRect.Height= 55;
    i51KitG2FontDraw(TypeheadHandle,0,(void *)Str, 0, &DrawRect, 0,2,2, 0XF000, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 80;
    DrawRect.Height= 75;
    i51KitG2FontDraw(TypeheadHandle,1,(void *)Str, 0, &DrawRect, 0,2,2, 0X0F00, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 160;
    DrawRect.Height= 85;
    i51KitG2FontDraw(TypeheadHandle,2,(void *)Str, 0, &DrawRect, 0,2,2, 0X00F0, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 250;
    i51KitG2FontDraw(TypeheadHandle,2,(void *)Str, 0, &DrawRect, 0,4,4, 0X00F0, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
}


    DrawRect.Y = 20;
//    i51KitG2FontDraw((void *)Str, 66, &DrawRect, 0, Color+500, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    DrawRect.Y = 40;
//    i51KitG2FontDraw( TypeheadHandle , 0,(void *)Str, 0, &DrawRect, 2,2,0, Color+500, 0XFF00, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
    memset ( LcdBuffer , 0XFFFF , 240*320*2 ) ;
    i51KitG2VPRender(VP1, iNULL);
i51KitG2VPSetCoord(VP1,X,Y);
X+= 10;
Y+= 10;
if(X>220) X =0;
if(Y>300) Y =0;
#endif
i51KitG2VPDisable();
RectTemp = &SrcRect;
if(OldTime==0||(i51AdeOsGetTick()-OldTime>=10))
{
	OldTime = i51AdeOsGetTick();
	switch(State)
	{
		case 0:
			Y -= 1;
			if( Y<-180 )
			{
				Y = 20;

				X = 130;	
				State = 1;
			}
			break;
		case 1:	
			X -= 1;
			if( X<-110 )
			{
				Y = -180;

				X = 10;	
				State = 2;
			}
			break;
		case 2:	
			Y += 1;
			if( Y>240 )
			{
				Y = 20;

				X = -110;	
				State = 3;
			}
			break;
		case 3:	
			X += 1;
			if( X>110 )
			{
				X = 110;
				Y = 240;
				State = 5;
			}
			break;
		case 4:
			RectTemp = iNULL;
			X += 1;
			if( X>110 )
			{
				Y =240;

				X = 10;	
				State = 0;
			}
			break;
		case 5:
			X--;
			Y--;
			if(X<-110)
			{
				X = -110;
				Y = -180;
				State = 6;
			}
			break;
		case 6:
			X++;
			Y++;
			if(X>110)
			{
				X = -110;
				Y = 240;
				State = 7;
			}
			break;
		case 7:
			X++;
			Y--;
			if(X>110)
			{
				X = 110;
				Y = -180;
				State = 8;
			}
			break;
		case 8:
			X--;
			Y++;
			if(X<10)
			{
				X = 0;
				State = 4;
			}
			break;
		default : break;	
			
	}
SrcRect.X = X;
SrcRect.Y = Y;
DrawRect.X = 0;
DrawRect.Y = 0;
DrawRect.Width = 120;
DrawRect.Height = 160;
i51KitG2CleanScreen(i51KitG2COLOR(255,0,0),&DrawRect);
DrawRect.X = 120;
i51KitG2CleanScreen(i51KitG2COLOR(0,255,0),&DrawRect);
DrawRect.X = 0;
DrawRect.Y = 160;
i51KitG2CleanScreen(31,&DrawRect);
DrawRect.X = 120;
i51KitG2CleanScreen(i51KitG2COLOR(0,0,240),&DrawRect);

i51KitG2CleanScreen(30,0);

//i51KitG2CleanScreen(0X0,0);
//i51KitG2FontDraw(TypeheadHandle,0,(void *)Str, 0, &DrawRect,RectTemp, i51KITG2_FONT_WORDS_ROTATE_0,0,0, 0XFF00, 31, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
//i51KitG2FontDraw(TypeheadHandle,0,(void *)Str, 0, &DrawRect,RectTemp, i51KITG2_FONT_WORDS_ROTATE_0,0,0, 0XFF00, 31, i51KITG2_FONT_UNDERLINE|i51KITG2_FONT_BKCOLOR);
i51KitG2Update();

}
#endif	
}


void TestDrawTypehead( void )
{
    iU8 Typehead[224] = 
    {
    /*--  ����:  ��  --*/
    /*--  ����11;  �������¶�Ӧ�ĵ���Ϊ����x��=14x14   --*/
    /*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=16x14  --*/
    0x8F,0xF8,0x68,0x08,0x29,0x48,0x82,0x20,0x44,0x90,0x00,0x80,0x2F,0xF8,0x41,0xC0,
    0x42,0xA0,0x84,0x90,0x98,0x88,0x80,0x80,0x80,0x80,0x00,0x00,
    
    /*--  ����:  ��  --*/
    /*--  ����11;  �������¶�Ӧ�ĵ���Ϊ����x��=14x14   --*/
    /*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=16x14  --*/
    0x24,0x10,0x24,0x90,0x24,0x90,0x24,0x90,0xFC,0x90,0x24,0x90,0x24,0x90,0x24,0x90,
    0x3C,0x90,0xE4,0x90,0x08,0x90,0x08,0x10,0x10,0x10,0x00,0x00,
    
    /*--  ����:  ��  --*/
    /*--  ����11;  �������¶�Ӧ�ĵ���Ϊ����x��=14x14   --*/
    /*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=16x14  --*/
    0x7F,0xF0,0x00,0x00,0x00,0x00,0x3F,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0x3F,0xE0,
    0x00,0x00,0x10,0x40,0x08,0x80,0x08,0x80,0xFF,0xF8,0x00,0x00,
    
    /*--  ����:  ��  --*/
    /*--  ����11;  �������¶�Ӧ�ĵ���Ϊ����x��=14x14   --*/
    /*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=16x14  --*/
    0x48,0x40,0x24,0x78,0x1E,0x80,0x89,0x00,0x48,0xF8,0x0E,0x10,0x1A,0x20,0x2B,0xF8,
    0xCA,0x20,0x4A,0x20,0x52,0x20,0x52,0xA0,0x64,0x40,0x00,0x00,
    
    /*--  ����:  ��  --*/
    /*--  ����11;  �������¶�Ӧ�ĵ���Ϊ����x��=14x14   --*/
    /*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=16x14  --*/
    0x00,0x00,0x7F,0xF0,0x42,0x30,0x63,0x50,0x55,0x50,0x59,0x50,0x48,0x90,0x55,0x50,
    0x55,0x30,0x65,0x10,0x62,0x10,0x44,0x50,0x40,0x20,0x00,0x00,
    
    /*--  ����:  ��  --*/
    /*--  ����11;  �������¶�Ӧ�ĵ���Ϊ����x��=14x14   --*/
    /*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=16x14  --*/
    0x21,0x00,0x41,0xF0,0x42,0x20,0x95,0x20,0xF1,0x40,0x20,0x80,0x41,0x60,0x96,0x18,
    0xE3,0xF0,0x02,0x10,0x32,0x10,0xC3,0xF0,0x02,0x10,0x00,0x00,
    
    /*--  ����:  ��  --*/
    /*--  ����11;  �������¶�Ӧ�ĵ���Ϊ����x��=14x14   --*/
    /*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=16x14  --*/
    0x1C,0x20,0xF1,0x20,0x10,0xA0,0x10,0x20,0xFD,0x20,0x10,0xA0,0x38,0x38,0x57,0xE0,
    0x50,0x20,0x90,0x20,0x10,0x20,0x10,0x20,0x10,0x20,0x00,0x00,
    
    /*--  ����:  ��  --*/
    /*--  ����11;  �������¶�Ӧ�ĵ���Ϊ����x��=14x14   --*/
    /*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=16x14  --*/
    0x20,0x80,0x20,0x80,0x27,0xF8,0xF8,0x80,0x20,0x80,0x27,0xF0,0x32,0x10,0x62,0x20,
    0xA1,0x40,0x20,0x80,0x21,0x40,0xA2,0x20,0x4C,0x18,0x00,0x00,
    };

iU8 NumberTypehead[]=
{
/*--  ����:  0  --*/
/*--  ������9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
0x00,0x00,0x70,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00,

/*--  ����:  1  --*/
/*--  ������9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
0x00,0x00,0x20,0x60,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00,

/*--  ����:  2  --*/
/*--  ������9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
0x00,0x00,0x70,0x88,0x88,0x10,0x20,0x40,0x80,0xF8,0x00,0x00,

/*--  ����:  3  --*/
/*--  ������9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
0x00,0x00,0x70,0x88,0x08,0x30,0x08,0x08,0x88,0x70,0x00,0x00,

/*--  ����:  4  --*/
/*--  ������9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
0x00,0x00,0x10,0x30,0x50,0x50,0x90,0x78,0x10,0x18,0x00,0x00,

/*--  ����:  5  --*/
/*--  ������9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
0x00,0x00,0xF8,0x80,0x80,0xF0,0x08,0x08,0x88,0x70,0x00,0x00,

/*--  ����:  6  --*/
/*--  ������9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
0x00,0x00,0x70,0x90,0x80,0xF0,0x88,0x88,0x88,0x70,0x00,0x00,

/*--  ����:  7  --*/
/*--  ������9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
0x00,0x00,0xF8,0x90,0x10,0x20,0x20,0x20,0x20,0x20,0x00,0x00,

/*--  ����:  8  --*/
/*--  ������9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
0x00,0x00,0x70,0x88,0x88,0x70,0x88,0x88,0x88,0x70,0x00,0x00,

/*--  ����:  9  --*/
/*--  ������9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
0x00,0x00,0x70,0x88,0x88,0x88,0x78,0x08,0x48,0x70,0x00,0x00,

};

iU8 CutFruit_logo[] = {
	/*--  ����:  A  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x20,0x20,0x30,0x50,0x50,0x78,0x48,0xCC,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  s  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  ����:  i  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  m  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0xA8,0xA8,0xA8,0xA8,0x00,0x00,

	/*--  ����:  p  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0x70,0x40,0xE0,

	/*--  ����:  l  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  c  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x38,0x48,0x40,0x40,0x38,0x00,0x00,

	/*--  ����:  a  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x38,0x48,0x3C,0x00,0x00,

	/*--  ����:  s  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  a  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x38,0x48,0x3C,0x00,0x00,

	/*--  ����:  l  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  g  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x3C,0x48,0x30,0x40,0x78,0x44,0x38,

	/*--  ����:  a  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x38,0x48,0x3C,0x00,0x00,

	/*--  ����:  m  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0xA8,0xA8,0xA8,0xA8,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:  .  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  T  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xF8,0xA8,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  h  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:  r  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  i  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  s  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  n  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  ����:  l  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  ����:  y  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xEC,0x48,0x50,0x30,0x20,0x20,0xC0,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  n  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  p  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0x70,0x40,0xE0,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  r  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  ����:  p  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0x70,0x40,0xE0,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  s  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  -  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  -  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  c  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x38,0x48,0x40,0x40,0x38,0x00,0x00,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  f  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  ����:  r  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  i  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:  s  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  ����:  !  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  T  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xF8,0xA8,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  h  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  s  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  ����:  c  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x38,0x48,0x40,0x40,0x38,0x00,0x00,

	/*--  ����:  r  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:  n  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  w  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xA8,0xA8,0x70,0x50,0x50,0x00,0x00,

	/*--  ����:  i  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  l  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  ����:  l  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  c  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x38,0x48,0x40,0x40,0x38,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  n  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:  i  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  n  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  j  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x10,0x00,0x00,0x30,0x10,0x10,0x10,0x10,0x10,0xE0,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  m  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0xA8,0xA8,0xA8,0xA8,0x00,0x00,

	/*--  ����:  p  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0x70,0x40,0xE0,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  d  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x18,0x08,0x08,0x38,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  i  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  f  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  ����:  f  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:  r  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:  n  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:  s  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  f  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  ����:  r  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  i  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:  s  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  ����:  .  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  B  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xF0,0x48,0x48,0x70,0x48,0x48,0x48,0xF0,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:  f  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  r  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:  h  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  f  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  ����:  r  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  i  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:  s  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  d  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x18,0x08,0x08,0x38,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  r  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  p  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0x70,0x40,0xE0,

	/*--  ����:  p  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0x70,0x40,0xE0,

	/*--  ����:  i  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  n  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  ����:  g  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x3C,0x48,0x30,0x40,0x78,0x44,0x38,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  f  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  ����:  f  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  ����:  ,  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x80,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  Y  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xD8,0x50,0x50,0x20,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  m  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0xA8,0xA8,0xA8,0xA8,0x00,0x00,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  s  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  f  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  l  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  ����:  l  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xE0,0x20,0x20,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  c  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x38,0x48,0x40,0x40,0x38,0x00,0x00,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  f  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  ����:  f  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x1C,0x20,0x20,0x78,0x20,0x20,0x20,0x78,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  .  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  Y  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xD8,0x50,0x50,0x20,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  d  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x18,0x08,0x08,0x38,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  n  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  ����:  '  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x40,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  c  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x38,0x48,0x40,0x40,0x38,0x00,0x00,

	/*--  ����:  u  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x48,0x48,0x48,0x3C,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:  h  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  b  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0x70,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  m  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0xA8,0xA8,0xA8,0xA8,0x00,0x00,

	/*--  ����:  b  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0x70,0x00,0x00,

	/*--  ����:  !  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  O  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x70,0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00,0x00,

	/*--  ����:  t  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x20,0x20,0x70,0x20,0x20,0x20,0x18,0x00,0x00,

	/*--  ����:  h  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0xC0,0x40,0x40,0x70,0x48,0x48,0x48,0xEC,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:  r  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  ����:  w  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xA8,0xA8,0x70,0x50,0x50,0x00,0x00,

	/*--  ����:  i  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x20,0x00,0x00,0x60,0x20,0x20,0x20,0x70,0x00,0x00,

	/*--  ����:  s  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x78,0x40,0x30,0x08,0x78,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:  ,  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x80,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  G  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x38,0x48,0x80,0x80,0x9C,0x88,0x48,0x30,0x00,0x00,

	/*--  ����:  a  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x38,0x48,0x3C,0x00,0x00,

	/*--  ����:  m  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xF0,0xA8,0xA8,0xA8,0xA8,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:     --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

	/*--  ����:  o  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x30,0x00,0x00,

	/*--  ����:  v  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xEC,0x48,0x50,0x30,0x20,0x00,0x00,

	/*--  ����:  e  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38,0x00,0x00,

	/*--  ����:  r  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0xD8,0x60,0x40,0x40,0xE0,0x00,0x00,

	/*--  ����:  .  --*/
	/*--  ����9;  �������¶�Ӧ�ĵ���Ϊ����x��=6x12   --*/
	/*--  ��Ȳ���8�ı������ֵ���Ϊ�����x�߶�=8x12  --*/
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,
};
iU8 * Str = (iU8*)"A simple casual game. There is only one purpose -- to cut fruits! The screen will continue to jump out differents fruits. Before the fruits dropping off, You must full cut off . You don't cut the bomb! Otherwise, Game over.";

//    i51KitG2FontDrawEx1( NumberTypehead, 10, 6, 12, 0 ,0XFF00, 0, 100);
//    i51KitG2FontDrawEx1( Str,CutFruit_logo, 0, 6, 12, 0, 0, 0, 0, 0XFF00, iNULL);
    i51KitG2FontDrawEx2( Str,CutFruit_logo, 0, 6, 12, 0, 0, 0, 0, 0XFF00, iNULL);
    i51AdeMmiUpdateScreen();

}

void TestDrawi51Image( void )
{
    iU32 i51Image;
    iRECT SrcRect, DesRect;	
    iU32 W = 0, H = 0;	
    static iS32 Angle = 0;

    iU8 * TestArr[9];// = {pvz_warning_Hex};
    static	iS32 ImageId = 0; 
    SrcRect.X = 0;
    SrcRect.Y = 184;
    SrcRect.Width = 96;
    SrcRect.Height = 184;

    DesRect.X = 100;
    DesRect.Y = 60;
    DesRect.Width = 96;
    DesRect.Height = 184;

//    if(FLAG_FIRSTRUN)    TestInit();
//    i51KitG2DrawImage(CurrentImageContainer,prizenum,&DesRect,iNULL);

	ImageId++;ImageId = 0;
    i51KitG2GetImageAttrEx((iU32)TestArr[(ImageId/50)%9],&W,&H);
    i51Image = i51KitG2ImageCreate( (iU32)TestArr[(ImageId/50)%9] );
//    i51KitG2DrawImageEX(i51Image, &SrcRect, &DesRect);
    i51KitG2DrawImageEx(i51Image, iNULL, iNULL);
    i51KitG2DrawImageEx(i51Image, &SrcRect, &DesRect);
	//i51KitG2MirrorImageEx(i51Image, iNULL, iNULL);
//	i51KitG2FlipImageEx(i51Image, iNULL, iNULL);
	//i51KitG2RotateImageEx(i51Image, iNULL, iNULL, -120, 140, 120, 160, 0, Angle);
	Angle++;
	Angle %=360;
	i51KitG2ImageDestory(i51Image);
    i51AdeMmiUpdateScreen();
}

void TestFillImage ( void )
{
    iU16 * Path = L"i51KitG2\\i51IMIC";
    iRECT SrcRect, SrcRect1, DesRect, DesRect1;
	static iHANDLE CurrentVpActivity1 = iNULL;
static iS32 X =0, Y = 0;

if( FLAG_FIRSTRUN )
{
    CurrentImageContainer =  i51KitG2OpenImageContainer(Path);
    SrcRect.X = 0;   
    SrcRect.Y = 0;   
    SrcRect.Width= 240;   
    SrcRect.Height= 320;   
    i51KitG2VPEnable();
    CurrentVpActivity1 = i51KitG2VPCreate(1,&SrcRect);
    memset ( LcdBuffer , 0XFFFF , 240*320*2 ) ;
    FLAG_FIRSTRUN = iFALSE;
    i51KitG2VPSetActivity(CurrentVpActivity1);
    i51KitG2ImageFillRect( CurrentImageContainer,WALL, iNULL, iNULL);
}  
    memset ( LcdBuffer , 0XFFFF , 240*320*2 ) ;
i51KitG2VPRender(CurrentVpActivity1,iNULL);
X++;
Y++;
if(X>=ScreenWidth) X = 0;
if(Y>=ScreenHeight) Y = 0;
i51KitG2VPSetCoord(CurrentVpActivity1,X,Y);
//    i51KitG2ImageFillRect( CurrentImageContainer,W4ALL, iNULL, iNULL);
    i51AdeMmiUpdateScreen  () ;

}
void TestFillImage1 ( void )
{
    int i;
	static int nloop = 0;

    iU16 * Path = L"i51KitG2\\i51IMIC";
    iRECT SrcRect, SrcRect1, DesRect, DesRect1;

	static iHANDLE vps[4] = {iNULL};

static iS32 X =0, Y = 0;

if( FLAG_FIRSTRUN )
{
    CurrentImageContainer =  i51KitG2OpenImageContainer(Path);
    SrcRect.X = 0;   
    SrcRect.Y = 0;   
    SrcRect.Width= 240;   
    SrcRect.Height= 320;   
    memset ( LcdBuffer , 0XFFFF , 240*320*2 ) ;
    FLAG_FIRSTRUN = iFALSE;
}  
if (nloop++ % 100 ==0)
{
    SrcRect.X = 0;   
    SrcRect.Y = 0;   
    SrcRect.Width= 240;   
    SrcRect.Height= 320;   
    for (i = 0; i < 4; ++i)
    {
        if (vps[i])
			i51KitG2VPDestroy(vps[i]);
        vps[i] = i51KitG2VPCreate(10,&SrcRect);
    }
    for (i = 0; i < 4; ++i)
    {
        iRECT DesRect={0,0,20,20};
        iRECT SrcRect={0,0,20,20};
        i51KitG2VPSetActivity(vps[i]);
        i51KitG2ImageFillRect( CurrentImageContainer,1, &DesRect, &SrcRect);
    }
}
    memset ( LcdBuffer , 0XFFFF , 240*320*2 ) ;

    for (i = 0; i < 4; ++i)
    {
        i51KitG2VPSetCoord(vps[i],20*i,20*i);
        i51KitG2VPRender(vps[i],iNULL);
    }
    i51AdeMmiUpdateScreen  () ;

}

void TestScreenZoom( void )
{
i51KitG2CleanScreen(0XAAAA,iNULL);
i51KitG2CleanScreen(0XAAAA,iNULL);
i51KitG2Update();
return;

	if(FLAG_FIRSTRUN)    TestInit();
	
	memset ( LcdBuffer , 0XFF , 240*320*2 ) ;

	if(DecodeComplete)
	{
		i51KitG2DrawImage(CurrentImageContainer,BG,iNULL,iNULL);
		i51KitG2ScreenZoom(iNULL, iNULL, ZoomRateX,ZoomRateY);
		i51KitG2Update();
	}
}

void TestUpdataContainer( void )
{
	static iU32 Temp = 0 ;
	iU32 Temp1 = 0;
	iHANDLE  ImageContainer = iNULL;
	iFILE ImageFile = 0;
	iU16 FileName[10][40] =
	{
		L"i51KitG2\\test1.i51G",L"i51KitG2\\test2.i51G",L"i51KitG2\\test3.i51G",
		L"i51KitG2\\test4.i51G",L"i51KitG2\\test5.i51G",L"i51KitG2\\test6.i51G",
		L"i51KitG2\\test7.i51G",L"i51KitG2\\test8.i51G",L"i51KitG2\\test9.i51G"
	};
	iS32 ImageDataLength = 0;
	iU32 ReadDataLength = 0;
	iU8 *ImageDataBuff = iNULL;
	iFILE ReFile = iNULL;
	iRECT CleanRect;
	iHANDLE  TempContainer = iNULL;
	
	if(Flag)
	{
//		Flag = iFALSE;
		Temp ++;
		ImageContainer = i51KitG2OpenImageContainer(L"i51KitG2\\NewMic");
//		ImageFile = i51AdeOsFileOpen((FileName[(Temp/30)%9]), i51_OS_FL_WRITE| i51_OS_FL_ALWAYS_OPEN, __FILE__ , __LINE__ );
//		i51AdeOsFileGetSize(ImageFile, &ImageDataLength);
//		i51AdeOsFileSetSeek(ImageFile, 0, i51_OS_FL_SEEK_HEAD);
//		ImageDataBuff = i51AdeOsMalloc(ImageDataLength, __FILE__ , __LINE__ );
//		i51AdeOsFileRead(ImageFile, ImageDataBuff, ImageDataLength, 0);
		ReFile = i51KitStdResOpen(L"i51KitG2\\i51KitG2.re");
		i51AdeOsFileGetSize(ReFile, &ImageDataLength);
		ImageDataBuff = i51AdeOsMalloc(ImageDataLength, __FILE__ , __LINE__ );
		i51AdeOsFileSetSeek(ReFile, 0, i51_OS_FL_SEEK_HEAD);
		i51AdeOsFileRead(ReFile, ImageDataBuff, ImageDataLength, &ReadDataLength);
			
//		i51KitG2UpdataContainerFromImage( ImageContainer, (void *)ImageFile, i51KitG2_MESSAGE_TYPE_FILE, 0);
//		i51KitG2UpdataContainerFromImage( ImageContainer, ImageDataBuff, i51KitG2_MESSAGE_TYPE_MEMORY, 0);
		i51KitG2UpdataContainerFromRe( ImageContainer, (void *)ReFile,(Temp/30)%9, i51KitG2_MESSAGE_TYPE_FILE, 0);
//		i51KitG2UpdataContainerFromRe( ImageContainer, (void *)ImageDataBuff,Temp, i51KitG2_MESSAGE_TYPE_MEMORY, 0);

		i51AdeOsFree(ImageDataBuff);
		ImageDataBuff = iNULL;

//		TempContainer = i51KitG2OpenImageContainer(L"i51KitG2\\i51IMIC");
//		i51KitG2GetContainerAttr(TempContainer,0,0,0,0);
//		i51KitG2CloseImageContainer(TempContainer);

		i51KitStdResClose(ReFile);
		ReFile = iNULL;
		
//		Temp = (Temp+1)%5;
//		Temp1 =  i51KitStdResGetContentEntry(ReFile,Temp);

		i51KitG2DrawImage(ImageContainer,0,iNULL,iNULL);
//		i51AdeOsFileClose(ImageFile);
		i51KitG2DestroyImageContainer(ImageContainer,L"i51KitG2\\NewMic");

		i51KitG2Update();
	}
}
#endif

static iBOOL i51KitG2Pause( void )
{
	// ͼƬ�������ַ
	i51ImageIndex * ImageIndexTable = iNULL;
	// ͼƬ������ַ���
	i51ImageContainer * ImageContainerEntry = iNULL;
	iS32 Temp = 0;
	iS32 MemCopyNum = 0;
	iU32 * ImageID = iNULL;
	iU32 * MemCopyCount = iNULL;
	iU32 TempID = 0;
	iU16 AllImageNum = 0;
	iS32 i,j,k;

//iLog("1");
	if(TimeIdZlibDecodeImage)
	{
		i51AdeOsTimerStop( TimeIdZlibDecodeImage );       
		TimeIdZlibDecodeImage = iNULL;
	}

//iLog("2");
	ImageID = i51AdeOsMalloc((sizeof(iU32))*300, __FILE__ , __LINE__ );
	if( iNULL==ImageID )
	{
		iLog( "KIT - G2 : pause malloc");
		goto ERROR;
	}

//iLog("3");
	MemCopyCount = i51AdeOsMalloc((sizeof(iU32))*300, __FILE__ , __LINE__ );
	if( iNULL==MemCopyCount )
	{
		iLog( "KIT - G2 : pause malloc mem");
		goto ERROR;
	}

//iLog("4");
	for( Temp=0;Temp<MAX_ContainerManage_NUM; Temp++ )
	{
		if( ContainerManage[Temp].Flag )
		{
//iLog("Temp:%d",Temp);

			ImageContainerEntry = ContainerManage[Temp].ContainerHandle;
			if( iNULL==ImageContainerEntry )
			{
				iLog( "KIT - G2 : pause %X", Temp);
				continue;
			}
			ImageIndexTable = ImageContainerEntry->ImageIndexTable;
			AllImageNum = ImageContainerEntry->ImageMaxId;
			MemCopyNum = 0;
			for( TempID=0; TempID<AllImageNum; TempID++ )
			{
				if( ImageIndexTable[TempID].FlagImageExist==0) continue;
				if( ImageIndexTable[TempID].FlagCopyRAM )
				{
					ImageID[MemCopyNum] = TempID;
					MemCopyCount[MemCopyNum] = ImageIndexTable[TempID].SetMemoryCopyCount;
					MemCopyNum++;
					if( MemCopyNum>=300 )
					{
						iLog( "KIT - G2 : pause MemCopyNum %X", MemCopyNum);
						goto ERROR;
					}
				}
			}

			if(MemCopyNum>0)
			{
				for( i=0; i<MemCopyNum; i++ )
				{
					k = i;
					for( j=i+1; j<MemCopyNum; j++ )
					{
						if(MemCopyCount[j]<MemCopyCount[k])
						{
							k = j;
						}
					}
					
					if( k!=i)
					{
						j = MemCopyCount[i];
						MemCopyCount[i] = MemCopyCount[k];
						MemCopyCount[k] = j;
					
						j = ImageID[i];
						ImageID[i] = ImageID[k];
						ImageID[k] = j;
					
					}
				}
			
				ContainerManage[Temp].ImageID = i51AdeOsMalloc( (sizeof(iU32))*MemCopyNum, __FILE__, __LINE__ );
				if( iNULL==ContainerManage[Temp].ImageID )
				{
					iLog( "KIT - G2 : pause malloc ImagID");
					goto ERROR;
				}

				ContainerManage[Temp].MemCopyCount= i51AdeOsMalloc( (sizeof(iU32))*MemCopyNum, __FILE__, __LINE__ );
				if( iNULL==ContainerManage[Temp].MemCopyCount )
				{
					iLog( "KIT - G2 : pause malloc count");
					goto ERROR;
				}

				i51AdeStdMemcpy16(ContainerManage[Temp].ImageID, ImageID, (sizeof(iU32))*MemCopyNum);
				i51AdeStdMemcpy16(ContainerManage[Temp].MemCopyCount, MemCopyCount, (sizeof(iU32))*MemCopyNum);

			}
			else
			{
				ContainerManage[Temp].ImageID = iNULL;
				ContainerManage[Temp].MemCopyCount = iNULL;
			}
			
			ContainerManage[Temp].MemoryCopyImageNum = MemCopyNum;		
			i51KitG2UnsetImageMemoryCopy(ContainerManage[Temp].ContainerHandle, 0, 0);

		}
	}

//iLog("5");

	i51AdeOsFree(MemCopyCount);
	i51AdeOsFree(ImageID);

	MemCopyCount = iNULL;
	ImageID = iNULL;
//iLog("6");

	return iTRUE;

ERROR:
	if(MemCopyCount) i51AdeOsFree(MemCopyCount);
	if(ImageID) i51AdeOsFree(ImageID);
	MemCopyCount = iNULL;
	ImageID = iNULL;

	if( ContainerManage[Temp].ImageID ) i51AdeOsFree(ContainerManage[Temp].ImageID);
	if( ContainerManage[Temp].MemCopyCount ) i51AdeOsFree(ContainerManage[Temp].MemCopyCount);
	ContainerManage[Temp].ImageID = iNULL;
	ContainerManage[Temp].MemCopyCount = iNULL;

	return iFALSE;

}

static iBOOL i51KitG2Resume( void )
{

	iS32 Temp = 0;

	if(FlagDecodeImage)
	{
		TimeIdZlibDecodeImage = i51AdeOsTimerCreate( 1,1);
		i51AdeOsTimerStart( TimeIdZlibDecodeImage, ZlibDecodeAllImage, 30);
	}

	RESUNE_MEMORY_COPY = iTRUE;

	for( Temp=0;Temp<MAX_ContainerManage_NUM; Temp++ )
	{
		if( ContainerManage[Temp].Flag && ContainerManage[Temp].ImageID )
		{
			RESUNE_MEMORY_COPY = iTRUE;
			CurrContainerManageNum = Temp;
			i51KitG2SetImageMemoryCopy(
				ContainerManage[Temp].ContainerHandle, 
				ContainerManage[Temp].MemoryCopyImageNum, 
				ContainerManage[Temp].ImageID);
			if( ContainerManage[Temp].ImageID )
			{
				i51AdeOsFree( ContainerManage[Temp].ImageID );
			}
			ContainerManage[Temp].ImageID  = iNULL;

			if( ContainerManage[Temp].MemCopyCount )
			{
				i51AdeOsFree( ContainerManage[Temp].MemCopyCount);
			}
			ContainerManage[Temp].MemCopyCount= iNULL;
			
			ContainerManage[Temp].MemoryCopyImageNum = 0;
		}
	}
	
	RESUNE_MEMORY_COPY = iFALSE;

	return iTRUE;
}

static iBOOL i51KitG2Exit( void )
{
	iS32 Temp = 0;
	
	for( Temp=0;Temp<MAX_ContainerManage_NUM; Temp++ )
	{
		if( ContainerManage[Temp].Flag )
		{
			if( ContainerManage[Temp].ImageID )
			{
				i51AdeOsFree( ContainerManage[Temp].ImageID );
			}
			ContainerManage[Temp].ImageID  = iNULL;

			if( ContainerManage[Temp].MemCopyCount )
			{
				i51AdeOsFree( ContainerManage[Temp].MemCopyCount);
			}
			ContainerManage[Temp].MemCopyCount= iNULL;

//			ContainerManage[Temp].Flag = iFALSE;
//			ContainerManage[Temp].ContainerHandle = 0;
		}
	}

	if(TimeIdZlibDecodeImage)
	{
		i51AdeOsTimerStop( TimeIdZlibDecodeImage );       
		TimeIdZlibDecodeImage = iNULL;
	}

	if( FlagDecodeImage && ZlibImageContainer)
	{
		i51KitG2CloseImageContainer(ZlibImageContainer);
		ZlibImageContainer = iNULL;
	}

	i51KitG2Release();
	i51KIT_DYNAMIC_KIT_UNLOAD(i51KitStd);

	return iTRUE;

}

iKIT iBOOL i51KitMain ( iS32 aid , iU32 message , void* parameter )
{

    //    TODO :    ��Ϣѭ��
#if DEBUG_I51KITG2  
    iU32 TestType = 15;
#endif

    switch ( message )
    {

#if DEBUG_I51KITG2
	case i51_MSG_PADUP:
        case i51_MSG_KEYUP :
			Flag = iTRUE;
//		ZoomRate = ((iU32)parameter)*25;
		if ( UseCach)
		{
			UseCach = iFALSE;
//			i51KitG2FontCachEnable(TypeheadHandle, 0);
//	             i51KitG2SetImageMemoryCopy(CurrentImageContainer,0,iNULL);
		}
		else
		{
			UseCach = iTRUE;
//	             i51KitG2UnsetImageMemoryCopy(CurrentImageContainer,0,iNULL);
//			i51KitG2FontCachDisable(TypeheadHandle, 0);
		}
             break;
#endif			 
        case i51_MSG_PAINT :

#if DEBUG_I51KITG2
              switch( TestType){
	            case 1:
	              TestDrawLine();
	              break;
	            case 2:
	              TestDrawPolygon();
	              break;
	            case 3:
	              TestDrawArc();
	              break;
	            case 4 :
	              TestDarwCircle();
	              break;
	            case 5:
	              TestFillPolygon();
	              break;
	            case 6 :
	              TestFillArc();
	              break;
	            case 7 :
	              TestFillCircle();
	              break;
	            case 8 :
	              TestCreateImageContainer();
	              break;
	            case 9:
	              TestOpenImageContainer();
	              break;
	            case 10 :
	              TestCloseImageContainer();
	              break;
	           case 11 :
	              TestDestroyImageContainer();
	              break;
	          case 12 :
	              TestDrawImage();
	               break;
	          case 13 :
	              TestRotateImage();
	               break;
	          case 14 :
	              TestVP();
	               break;	
              case 15:
			  TestImage();
			  break;
		case 17:
			TestFontDraw();
			break;
		case 18:
			TestDrawTypehead();
			break;
		case 19:
			TestDrawi51Image();
			break;
		case 20:
			TestFillImage();
			break;
		case 21 :
			TestScreenZoom();
			break;
		case 22:
			TestUpdataContainer();
			break;
	       default :
	              break;
	      }
#endif
             break ;
	case i51_MSG_RUN:
	{
		break;
	}
        case i51_MSG_INIT :
        {
  
		i51KIT_STATIC_KIT_SET(Adapter,parameter);
		i51KIT_STATIC_KIT_ASSERT(Adapter);

//iLog("load Adapter");
             //	����i51KitStd��̬��
             i51KIT_DYNAMIC_KIT_LOAD(i51KitStd,L"PKiGAPI\\i51KitStd.kit");

        
             if ( iNULL == i51KIT_DYNAMIC_KIT_PROTOTYPE(i51KitStd) ) 
             {
                 return 0 ;
             }

//iLog("load std");
            if( iNULL == i51KitG2Create() ) return 0;
//iLog("i51KitG2Create");

		//i51KernelSetCache(L"i51KitG2");
					
#if DEBUG_I51KITG2
		i51AdeOsScreenAlwaysLight(1);
//		TypeheadHandle = i51KitStdResOpen(L"Fonts\\Typehead.re");
		TypeheadHandle = i51KitStdResOpen(L"Fonts\\fzlthjw_14.re");
		i51KitG2FontCachEnable(TypeheadHandle, 0);
		//    i51KitG2VPEnable();
		//    i51KitG2SetAlpha(150);
//		    memset ( LcdBuffer , 0 , 240*320*2 ) ;
i51KitG2CleanScreen(0, iNULL);
			i51KitG2Update();
		iLog( "Init OK");
		//i51KernelSetCache(L"i51KitG2");
#endif
        }
iLog(" KIT - G2 : init OK ");
        break ;

        case i51_MSG_PAUSE :
//iLog("��֮ͣǰʣ���ڴ� : %d", i51AdeOsGetTotallMemoryLeft());
iLog(" KIT - G2 : pause start ");
		i51KitG2Pause();
iLog(" KIT - G2 : pause end ");
//iLog("��֮ͣ��ʣ���ڴ�: %d", i51AdeOsGetTotallMemoryLeft());
            break ;

        case i51_MSG_RESUME:
//iLog("�ָ�֮ǰʣ���ڴ�: %d", i51AdeOsGetTotallMemoryLeft());
iLog(" KIT - G2 : resume start ");
		i51KitG2Resume();
iLog(" KIT - G2 : resume end ");
//iLog("�ָ�֮��ʣ���ڴ�: %d", i51AdeOsGetTotallMemoryLeft());
            break;
            
        case i51_MSG_EXIT :
#if DEBUG_I51KITG2
	i51KitG2FontCachDisable(TypeheadHandle, 0);
	TypeheadHandle = iNULL;

	i51KitG2CloseImageContainer(CurrentImageContainer);
	CurrentImageContainer = iNULL;
	
	i51KitG2VPDestroy(VP_Bee);
	i51KitG2VPDestroy(VP_BK);
	i51KitG2VPDestroy(VP_Road);
	i51KitG2VPDestroy(VP_Snidget);
	VP_Bee = iNULL;
	VP_BK = iNULL;
	VP_Road = iNULL;
	VP_Snidget = iNULL;
#endif
//iLog("G2 exit");
iLog(" KIT - G2 : exit start ");
i51KitG2Exit();
iLog(" KIT - G2 : exit end ");
        break ;
    }

    return iTRUE ;

}


iKIT iU32* i51KitPort ()
{

    //    TODO : Initialize API Port for external layer

    i51KIT_DYNAMIC_KIT_MAPING_BEGIN(i51KitG2)
    
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2CleanScreen)

    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DrawPoint)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DrawLine)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DrawPolygon)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DrawArc)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DrawCircle)
    
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FillPolygon)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FillArc)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FillCircle)
    
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2CreateImageContainer)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2CloseImageContainer)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2CheckImageContainer)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2OpenImageContainer)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DestroyImageContainer)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2SetContainerContentVersion)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2GetContainerContentVersion)
    
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2SetImageMemoryCopy)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2UnsetImageMemoryCopy)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2GetImageAttr)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2DrawImage)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2MirrorImage)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FlipImage)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2RotateImage)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2ImageFillRect)

    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FontCachEnable )
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FontCachDisable)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FontGetDemension )
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2FontDraw )

    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPEnable)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPDisable)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPCreate)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPDestroy)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPSetCoord)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPSetActivity)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPGetActivity)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPClean)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPGetAtrribute)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPiSFull)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2VPRender)  
    
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2Update)  
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2SetAlpha)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2ScreenZoom)

    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2UpdataContainerFromImage )
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2UpdataContainerFromRe )
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2ContainerImageIdExist)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2GetContainerAttr)
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2ContainerDeleteImage)

    // start add by otto 2012.6.11
    i51KIT_DYNAMIC_KIT_MAPING(i51KitG2StopDecode)
    // end add by otto 2012.6.11

   i51KIT_DYNAMIC_KIT_MAPING_END

    return i51KIT_DYNAMIC_KIT_GET_MAPING(i51KitG2) ;

}




