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
		// int filenum = 0;//记录文件个数
		// p += 27;//直接偏移27个字符
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
		memset( &GUR.FileName.Data.FileName[0][0], 0 ,sizeof(GUR.FileName.Data.FileName[0][0])*8*20 );//清空文件名
		GUW.DisplayData.Data.File_Chiose = 0;
		name_ = File_List("");
		if(name_ == 0)
		{
			AlarmSetBit(2,0,29);//u盘读取写入错误
			return;//退出
		}
		else
		{
			AlarmClearBit(2,0,29);
		}
		p = name_;
		p += 27;				//过滤掉头信息
		char *p2 = p;			//当前文件名的头指针
		int filecount = 0;		//文件数量
		int filenameLength = 0; //文件名长度

		while (strlen(p))
		{
			u8 val1 = *p; 
			u8 val2 = *(p+1);

			if ( (val1 == 255 && val2 == 255) || filecount == 8   )//0xff 0xff 为结束 最多存储八个文件 strlen(p) == 0
			{
				break;
			}

			if (val1 == 0x0d && val2 == 0x0a)//一个文件名结束是 0x0d 0x0a
			{
				strncpy(&GUR.FileName.Data.FileName[filecount][0], p2, filenameLength < 20 ? filenameLength : 19);
				//数据复制
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
	if(GUW.DisplayData.Data.File_Write)//文件写入本机
	{
		GUW.DisplayData.Data.File_Write = 0;
		number = File_Read(GUS.TransWatch.Data.FileName_save,&GUS,0, sizeof GUS,0);	//从优盘读取
		if(number != 0)
		{
			AlarmSetBit(2,0,29);//u盘读取写入错误
		}
		else
		{
			AlarmClearBit(2,0,29);
		}
	}
	if(GUW.DisplayData.Data.File_Read)//本机写入优盘
	{
		GUW.DisplayData.Data.File_Read = 0;
		number = File_Write(GUS.TransWatch.Data.FileName_save,&GUS,0, sizeof GUS,0);	//写入优盘
		if(number != 0)
		{
			AlarmSetBit(2,0,29);//u盘读取写入错误
		}
		else
		{
			AlarmClearBit(2,0,29);
		}
	}
}

