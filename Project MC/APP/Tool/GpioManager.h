#ifndef	_GPIOMANAGER_H_
#define	_GPIOMANAGER_H_

	//����������
	//CardId:�忨�ţ�0������>0��չ��
	//idx	:�忨�������˿ӡ�ţ���1��ʼ
	#define	GPIO(idx) (idx-1)
	#define	EGPIO(CardId,idx) (CardId*1000+idx-1)

	typedef enum
	{
		ON=0,
		OFF=1,
		ONR=1,
		OFFR=0,
	}GpioSta;
	
	typedef enum
	{
		I_Origin1		=	GPIO(1),	//��λ1ԭ��
		I_Origin2		=	GPIO(2),	//��λ2ԭ��
		I_Origin3		=	GPIO(3),	//װ��ԭ��
		I_Origin4		=	GPIO(4),	//͹��ԭ��
		I_Origin5		=	GPIO(5),	//
		I_Origin6		=	GPIO(6),	//
		I_StartBtn		=	GPIO(7),	//����
		I_StopBtn		=	GPIO(8),	//��ͣ
		I_ScramBtn		=	GPIO(9),	//��ͣ
		I_ResetBtn		=	GPIO(10),	//��λ
		I_FlatLack		=	GPIO(11),	//ֱ��ȱ��
		I_RoundLack		=	GPIO(12),	//Բ��ȱ��
		I_FlatInp		=	GPIO(13),	//ֱ��λ
		I_PackCameraEot	=	GPIO(14),	//װ��Ӱ��EOT
		I_PackCameraOk	=	GPIO(15),	//װ��Ӱ��OK/NG
		I_DownCameraEot	=	GPIO(16),	//�ײ�Ӱ��EOT
		I_DownCameraOk	=	GPIO(17),	//�ײ�Ӱ��OK/NG
		I_CollectBtn	=	GPIO(18),	//�մ���ť
		I_TempErr		=	GPIO(19),	//�¿����쳣
		I_PressureErr	=	GPIO(20),	//��ѹ���
		I_VacuoErr		=	GPIO(21),	//��ѹ���
		I_ElecTest2Ret	=	GPIO(22),	//��վ���Խ��
		I_DetectForPack	=	GPIO(23),	//װ��ǰ���
		I_ChkPack		=   GPIO(24),	//װ���Ӧ
		I_DetectTakeOut	=   GPIO(25),	//����ȡ�ϼ��
		I_ChkBelt		=   GPIO(26),	//�ϴ���Ӧ
		I_ChkFilm		=	GPIO(27),	//��Ĥ��Ӧ

		Q_TurnCtr1		=	GPIO(1),	//��ת���п���1
		Q_TurnCtr2		=	GPIO(2),	//��ת���п���2
		Q_BeltSupply	=	GPIO(3),	//�Ŵ����
		Q_BeltCollect	=	GPIO(4),	//�մ����
		Q_FeedVacuo1	=	GPIO(5),	//�������1
		Q_FeedVacuo2	=	GPIO(6),	//�������2
		Q_FeedVacuo3	=	GPIO(7),	//�������3
		Q_FeedBlow		=	GPIO(8),	//���봵��
		Q_PackVacuo		=	GPIO(9),	//װ��λ���
		Q_PlateClean	=	GPIO(10),	//��������
		Q_DiscardBlow1	=	GPIO(11),	//����1
		Q_DiscardBlow2	=	GPIO(12),	//����2
		Q_DiscardBlow3	=	GPIO(13),	//����3
		Q_DiscardBlow4	=	GPIO(14),	//����4
		Q_PressUpCyd	=   GPIO(15),   //���������
		Q_PressDownCyd	=	GPIO(16),   //���������
		Q_CutCyd		=   GPIO(17),   //�д�����
		Q_FeederHopper	=	GPIO(18),   //�϶�����
		Q_PackCameraSot	=	GPIO(19),	//װ��Ӱ��SOT
		Q_DownCameraSot	=	GPIO(20),	//�ײ�Ӱ��SOT
		Q_ElecTest2Sot	=	GPIO(21),	//��վ����SOT
		Q_ElecTest2Dir	=	GPIO(22),	//��վ���Է���
		Q_LightIn		=	GPIO(23),	//���Դ
		Q_LightOut		=	GPIO(24),	//���Դ
		Q_VacuoPump		=	GPIO(25),	//��ձ�		
		Q_FeederFlat	=   GPIO(26),   //ֱ��
		Q_Green			=	GPIO(27),   //�̵�
		Q_Yellow		=   GPIO(28),   //�Ƶ�
		Q_Red			=	GPIO(29),   //���
		Q_Beep			=	GPIO(30),	//������
		Q_FeederRound	=	GPIO(31),   //Բ��
		
	//BCD Output
	BCD_Q_ETestSOT		=	GPIO(38),	//BCD_OUT 0  //���Բ��Դ���SOT
	//BCD Input
	BCD_I_ETestRet_0	=   GPIO(31),   //BCD_IN_0  2��0��
	BCD_I_ETestRet_180	=	GPIO(32),   //BCD_IN_1  2��180��
	BCD_I_ETestRet_NG	=   GPIO(33),   //BCD_IN_2  2����Բ���
	
	BCD_I_A				=  	GPIO(31),   //BCD_IN_0  4��bit0
	BCD_I_B				=	GPIO(32),   //BCD_IN_1  4��bit1
	BCD_I_C				=  	GPIO(33),   //BCD_IN_2  4��bit2
		
		
		
		
	}Gpio;

	extern GpioSta I_Get(Gpio idx);
	extern void Q_Set(Gpio idx, GpioSta val);
	extern GpioSta Q_Get(Gpio idx);
	

#endif
