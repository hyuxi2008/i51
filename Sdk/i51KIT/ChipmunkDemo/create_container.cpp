#include "create_container.h"

decode_struct decode;
char g_image_nums[GAME_RESOURCE_NUM]   = {0};

void decode_one_image_finish(iU32 Step, iU32 Totall, iHANDLE handle)
{
	i51AdeOsLog(0, "���� total = %d, step = %d", Totall, Step);
	decode.num[decode.order] = Step;  //��1��ʼ
	decode.percent = 10000 * decode.order / decode.resource_num + Step * 10000 / decode.resource_num / g_image_nums[decode.order];
	decode.container_handle = handle;
}

void decode_result( iHANDLE Handle)
{
	decode.doing[decode.order] = iFALSE;
	container_exit();   //�ر���Դ�ļ�
	if(Handle)  //decode success
	{
		i51KitG2SetContainerContentVersion(Handle, IMAGE_RESOURCE_VERSION);   //�����м���汾
		i51AdeOsLog(0, "�رս�����Դǰ");
		i51KitG2CloseImageContainer(Handle);  //�ر������ļ�
		i51AdeOsLog(0, "�������");
		if(decode.order == decode.resource_num - 1)
		{
			decode.state = DECODE_FINISH;
		}
	}
	else
	{
		i51AdeOsLog(0, "����ʧ��");
		decode.state = DECODE_FALSE;
	}
}

void create_image_container(char *dir, char *resource_name, char *container_name, iU32 image_num)
{
	iU32 i;
	iU16 app_path[66];
	iU16 file_name[66];
	static iU32 ImageId[256];

	for(i=0; i<image_num; i++)
	{
		ImageId[i] = i;
	}

	i51AdeStdWSprintf(file_name, 66*2, "%s\\%s", dir, resource_name);
	decode.res_fd = i51KitStdResOpen (file_name) ;
	i51AdeOsLog(0, "����Դ�ļ�fd = %d", decode.res_fd);
	if(decode.res_fd == 0)
	{
		decode.state = DECODE_FALSE;
		return;
	}
	i51AdeStdWSprintf(app_path, 66*2, "%s\\%s", dir, container_name);
	i51KitG2CreateImageContainer ( decode.res_fd, ImageId, image_num, app_path, decode_one_image_finish, decode_result); 
}

void create_image_container_init(void)
{
	memset(&decode, 0, sizeof(decode_struct));
}

iBOOL check_image_container(char (*res_name)[33], char (*ctn_name)[33], iU32 image_num[], iU32 resource_num)
{
	iU32 i;
	iU16 file_name[66];

	decode.resource_num = resource_num;
	for(i=0; i<resource_num; i++)
	{
		iHANDLE fd;

		if(decode.finish[i])
			continue;
		else if(decode.doing[i])
			break;
		else if(decode.state == DECODE_FALSE)
			break;

		g_image_nums[i] = image_num[i];
		i51AdeStdWSprintf(file_name, 66*2, "%s\\%s", GAME_DIR, ctn_name[i]);
		if(i51KitG2CheckImageContainer(file_name))   // �������������
		{
			iU32 version;

			fd = i51KitG2OpenImageContainer(file_name);
			i51KitG2GetContainerContentVersion(fd, &version);
			if(fd)
			{
				i51KitG2CloseImageContainer(fd);  //�ر������ļ�
			}

			if(version == IMAGE_RESOURCE_VERSION)      //���汾
			{
				decode.finish[i] = iTRUE;
				decode.order++;
				if(i == resource_num - 1)
				{
					decode.state   = DECODE_FINISH;
					decode.percent = 10000;
					return iTRUE;
				}
				else
				{
					decode.state = DECODE_DOING;
					decode.res_fd  = 0;
				}
				continue;
			}
		}
		
		/* ��Ҫ���½��� */
		pfunc->public_delect_file(GAME_DIR, ctn_name[i]);  //ɾ�������ļ�
		decode.doing[decode.order] = iTRUE;
		create_image_container(GAME_DIR, res_name[i], ctn_name[i], image_num[i]);
		return iFALSE;
	}

	return iTRUE;
}

void container_exit(void)
{
	if(decode.res_fd)
	{
		i51AdeOsLog(0, "�ر����ڽ����re�ļ�");
		i51KitStdResClose(decode.res_fd);   //�ر���Դ�ļ�
		decode.res_fd = 0;
	}
}