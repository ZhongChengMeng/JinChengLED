/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-05-29 11:55:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\iodefine.h
 */
#ifndef __IODEFINE_H
#define __IODEFINE_H

/***************************************************************/
/*              板卡硬件口重命名(不需更改)                     */
/***************************************************************/

/* 输入口命名 */
#define 	X(n)					(n-1)

/* 输出口命名 */
#define 	Y(n)					(n-1)

/* 卡编号（0:固定为主卡 1开始为扩展卡） */
#define 	CARD(n) 				(n)     		

/***************************************************************/
/*              
		io口重命名，配合项目修改：
		作为以下接口参数传入识别主从卡：
			获取输入口： InGet();
			获取输出口： OutGet();
			设置输出口： OutSet();
*/
/***************************************************************/
/** 主卡输出口功能定义 **/
#define	Q_xxA		 	CARD(0),Y(1) 		//	Y1	
#define	Q_xxB			CARD(0),Y(2)		//	Y2	
#define	Q_xxC 			CARD(0),Y(3)		//	Y3	

/** 扩展输出卡1功能定义 **/
#define	Q_xxD			CARD(1),Y(1)		//	Y1 	
#define	Q_xxE			CARD(1),Y(2)		//	Y2	
#define	Q_xxF			CARD(1),Y(3)		//	Y3	


/** 扩展输出卡1功能定义 **/
#define	Q_xxG			CARD(2),Y(1)		//	Y1 	
#define	Q_xxH			CARD(2),Y(2)		//	Y2	
#define	Q_xxI			CARD(2),Y(3)		//	Y3	


/** 主卡输入功能定义 **/
#define	I_xxA			CARD(0),X(1)		//	X1
#define	I_xxB			CARD(0),X(2)		//	X2
#define	I_xxC			CARD(0),X(3)		//	X3



/** 扩展输入卡1功能定义 **/
#define	I_xxD			CARD(1),X(1)		//	X1
#define	I_xxE			CARD(1),X(2)		//	X2
#define	I_xxF			CARD(1),X(3)		//	X3	

/** 扩展输入卡2功能定义 **/
#define	I_xxG			CARD(2),X(1)		//	X1
#define	I_xxH			CARD(2),X(2)		//	X2
#define	I_xxI			CARD(2),X(3)		//	X3	


#endif
