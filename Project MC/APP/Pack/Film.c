#include "Film.h"

void File_operator()
{
	if(FSM_STA == STOP)
	{
		return;
	}
	char *name_;
	int number = 0;
	if(GUW.DisplayData.Data.File_Chiose)
	{
		// char *p =NULL;
		// ret = 0;
		// name_ = File_List("");
		// p = name_;
		// int i = 0;
		// int filenum = 0;//��¼�ļ�����
		// p += 27;//ֱ��ƫ��27���ַ�
		// while(1)
		// {
		// 	u8 val1 = *p; 
		// 	u8 val2 = *(p+1);
			
		// 	if((val1 == 255 && val2 == 255 ) || filenum >7)
		// 	{
		// 		break;
		// 	}
		// 	if((val1 == 0x0D && val2 == 0x0A) || i > 19)
		// 	{
		// 		filenum ++;
		// 		i = 0;
		//		p += 2;
		// 	}
		// 	GUR.FileName.Data.FileName[filenum][i] = *p;
		// 	i++;
		// 	p++;
		// }
		char *p = NULL;
		memset( &GUR.FileName.Data.FileName[0][0], 0 ,sizeof(GUR.FileName.Data.FileName[0][0])*8*20 );//����ļ���
		GUW.DisplayData.Data.File_Chiose = 0;
		name_ = File_List("");
		if(name_ == 0)
		{
			AlarmSetBit(2,0,29);//u�̶�ȡд�����
			return;//�˳�
		}
		else
		{
			AlarmClearBit(2,0,29);
		}
		p = name_;
		p += 27;				//���˵�ͷ��Ϣ
		char *p2 = p;			//��ǰ�ļ�����ͷָ��
		int filecount = 0;		//�ļ�����
		int filenameLength = 0; //�ļ�������

		while (strlen(p))
		{
			u8 val1 = *p; 
			u8 val2 = *(p+1);

			if ( (val1 == 255 && val2 == 255) || filecount == 8   )//0xff 0xff Ϊ���� ���洢�˸��ļ� strlen(p) == 0
			{
				break;
			}

			if (val1 == 0x0d && val2 == 0x0a)//һ���ļ��������� 0x0d 0x0a
			{
				strncpy(&GUR.FileName.Data.FileName[filecount][0], p2, filenameLength < 20 ? filenameLength : 19);
				//���ݸ���
				filenameLength = 0;
				p+=2;
				p2 = p;
				filecount++;
			}else{
				p++;
				filenameLength++;
			}
		}
	}
	if(GUW.DisplayData.Data.File_Write)//�ļ�д�뱾��
	{
		GUW.DisplayData.Data.File_Write = 0;
		number = File_Read(GUS.TransWatch.Data.FileName_save,&GUS,0, sizeof GUS,0);	//�����̶�ȡ
		if(number != 0)
		{
			AlarmSetBit(2,0,29);//u�̶�ȡд�����
		}
		else
		{
			AlarmClearBit(2,0,29);
		}
	}
	if(GUW.DisplayData.Data.File_Read)//����д������
	{
		GUW.DisplayData.Data.File_Read = 0;
		number = File_Write(GUS.TransWatch.Data.FileName_save,&GUS,0, sizeof GUS,0);	//д������
		if(number != 0)
		{
			AlarmSetBit(2,0,29);//u�̶�ȡд�����
		}
		else
		{
			AlarmClearBit(2,0,29);
		}
	}
}

