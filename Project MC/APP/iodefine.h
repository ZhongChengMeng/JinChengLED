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
/*              �忨Ӳ����������(�������)                     */
/***************************************************************/

/* ��������� */
#define 	X(n)					(n-1)

/* ��������� */
#define 	Y(n)					(n-1)

/* ����ţ�0:�̶�Ϊ���� 1��ʼΪ��չ���� */
#define 	CARD(n) 				(n)     		

/***************************************************************/
/*              
		io���������������Ŀ�޸ģ�
		��Ϊ���½ӿڲ�������ʶ�����ӿ���
			��ȡ����ڣ� InGet();
			��ȡ����ڣ� OutGet();
			��������ڣ� OutSet();
*/
/***************************************************************/
/** ��������ڹ��ܶ��� **/
#define	Q_xxA		 	CARD(0),Y(1) 		//	Y1	
#define	Q_xxB			CARD(0),Y(2)		//	Y2	
#define	Q_xxC 			CARD(0),Y(3)		//	Y3	

/** ��չ�����1���ܶ��� **/
#define	Q_xxD			CARD(1),Y(1)		//	Y1 	
#define	Q_xxE			CARD(1),Y(2)		//	Y2	
#define	Q_xxF			CARD(1),Y(3)		//	Y3	


/** ��չ�����1���ܶ��� **/
#define	Q_xxG			CARD(2),Y(1)		//	Y1 	
#define	Q_xxH			CARD(2),Y(2)		//	Y2	
#define	Q_xxI			CARD(2),Y(3)		//	Y3	


/** �������빦�ܶ��� **/
#define	I_xxA			CARD(0),X(1)		//	X1
#define	I_xxB			CARD(0),X(2)		//	X2
#define	I_xxC			CARD(0),X(3)		//	X3



/** ��չ���뿨1���ܶ��� **/
#define	I_xxD			CARD(1),X(1)		//	X1
#define	I_xxE			CARD(1),X(2)		//	X2
#define	I_xxF			CARD(1),X(3)		//	X3	

/** ��չ���뿨2���ܶ��� **/
#define	I_xxG			CARD(2),X(1)		//	X1
#define	I_xxH			CARD(2),X(2)		//	X2
#define	I_xxI			CARD(2),X(3)		//	X3	


#endif
