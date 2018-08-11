/**************************************************************************************
Filename:	AHRSENTRY.c
Author:		JeromeWan  from SmartCare
Description:
***************************************************************************************/
# include "AHRSEntry.h"
# include "imu.h"
# include "IMUSO3.h"
/**************************************************************************************
��������void SmCare_AHRS_Entry(double *p,int init) 
��  ������̬�����ں�����ܺ���
***************************************************************************************/
void SmCare_AHRS_Entry(double *p,int init) 
{
	//�㷨�ڲ���ʼ��
	if (init) 
	{
		calculatingAttitudeAngle(p, init);
	}

	if (imu.ready) {
		//processing
		/*
		1.���ݱ궨���������������ǰ��̬��
		2.������̬�Ǽ���������������ȡ��̬����
		3.��̬������Ⱥ�����
		4.��̬�Ķ����������
		*/
        calculatingAttitudeAngle(p, init);
	}
	else 
	{
		// preprocess
		/*
		1.�����ݵ�Ҫ�� ǰ��Ԥ��һ��ʱ������ݣ�ʵ�ʹ���ʱҪ������Ԥ�Ⱦ�ֹ��ʱ��
		2.������ݵ�׼ȷ��
		3.ץȡһ�����������궨/У׼���������ݣ�����������������ģ�����궨
		4.���У׼/�궨��Ĵ��������ݡ�
		*/
	    IMU_Pre(p, init);
	}
	
	
	
	//return;
	/*
	1.���ز�̬�м���� �����������
	2.���ز�̬���������������
	*/
}