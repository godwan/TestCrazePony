#include  <stdio.h>
#include  <math.h>
#include <iostream>
#include "imu.h"

# define RD_WINDLEN  11   // RD means variable using in read data
#define PI       3.14159265358979323846
static int RD_Count = 0;
static uint8_t RD_buffer_win[RD_WINDLEN] = {0};

bool CheakSum();
double* ParseByteBySpeed();
double MakeSign(uint8_t highByte, uint8_t lowByte);
double* checkData();
double* ParseByteByAngle_velocity();
double* ParseByteByMagnetic_field();
double* ParseByteByAngle();

double* ReadData(uint8_t data)
{
	double*p = NULL ;
	int i;
	RD_buffer_win[RD_Count] = data;
	if (++RD_Count == RD_WINDLEN) {
		p = checkData();

		for (i = 1; i < RD_Count; i++) 
		{
			RD_buffer_win[i - 1] = RD_buffer_win[i];
		}
		--RD_Count;
	}
	return p;
}


double* checkData() {
	double *p = NULL;
	if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 81)//�������Ǽ��ٶȰ�
	{
		if (CheakSum())
		{
			p = ParseByteBySpeed();
				// data.Clear();
		}
	}
	else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 82) //�������ǽ��ٶȰ�
	{
		if (CheakSum())
		{
			p = ParseByteByAngle_velocity();
			// data.Clear();
		}
	}
    else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 84)// �������Ǵų�
	{
		if (CheakSum())
		{
			p = ParseByteByMagnetic_field();
		}
	}
	else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 83)// �������ǽǶȰ�
	{
		if (CheakSum())
		{
			p = ParseByteByAngle();
		}
	}
	



///////////////////////////////////complete///////////////////////////////////////////////

 //   if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 80)//��������ʱ��
 //   {
	//	if (CheakSum())
	//	{
	//			//dataOut = ParseByteByDate(RD_buffer_winR);
	//	}		
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 81)//�������Ǽ��ٶȰ�
	//{
	//	if (CheakSum())
	//	{
	//		p = ParseByteBySpeed();
	//			// data.Clear();
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 82) //�������ǽ��ٶȰ�
	//{
	//	if (CheakSum())
	//	{
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 83)// �������ǽǶȰ�
	//{
	//	if (CheakSum())
	//	{
	//	}
	//	else
	//	{
	//		
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 84)// �������Ǵų�
	//{
	//	if (CheakSum())
	//	{
	//			
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 85)// �������Ƕ˿�״̬
	//{
	//	if (CheakSum())
	//	{
	//		
	//	}
	//		
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 86)// ����������ѹ���߶�����
	//{
	//	if (CheakSum())
	//	{	
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 87)// �������Ǿ��ȡ�γ������
	//{
	//	if (CheakSum())
	//	{
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 88)// �������ǵ�������
	//{
	//	if (CheakSum())
	//	{

	//	}
	//}

	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 89)// ����������Ԫ��
	//{
	//	if (CheakSum())
	//	{
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 90)// �����������Ƕ�λ����
	//{
	//	if (CheakSum())
	//	{
	//	}
	//}

	return p;
}



///////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////////
double* ParseByteByDate()
{
	double* temp = new double[3];

	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 16;// * G; // m/s^2   x����ļ��ٶ�
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 16;// * G; // m/s^2   y����ļ��ٶ�
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 16;// * G; // m/s^2  z����ļ��ٶ�
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //�¶�  ���϶�
																	// Console.WriteLine("���ٶȣ�x����" + wx.ToString() + "  y����" + wy.ToString() + "  z����" + wz.ToString());
	return temp;
}

double* ParseByteBySpeed()
{
	double* temp = new double[4];

	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 16 * CONSTANTS_ONE_G;// * G; // m/s^2   x����ļ��ٶ�
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 16 * CONSTANTS_ONE_G;// * G; // m/s^2   y����ļ��ٶ�
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 16 * CONSTANTS_ONE_G;// * G; // m/s^2  z����ļ��ٶ�
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //�¶�  ���϶�
																	//Console.WriteLine("���ٶȣ�x����"+wx.ToString()+"  y����"+ wy.ToString()+"  z����"+ wz.ToString());
    temp[0] = 0;
	temp[1] = wx;
	temp[2] = wy;
	temp[3] = wz;
	
	return temp;
}




double* ParseByteByAngle_velocity()
{
	double* temp = new double[4];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 2000 * PI / 180.0; // rad/s   x����Ľ��ٶ�
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 2000 * PI / 180.0; // rad/s   y����Ľ��ٶ�
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 2000 * PI / 180.0; // rad/s   z����Ľ��ٶ�
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //�¶�  ���϶�
	//Console.WriteLine("���ٶȣ�x������ٶ�", wx, "y������ٶ�", wy, "z������ٶ�", wz);															
	//Console.WriteLine("���ٶȣ�x������ٶ�", wx, "y������ٶ�", wy, "z������ٶ�", wz)
	temp[0] = 1;
	temp[1] = wx;
	temp[2] = wy;
	temp[3] = wz;
	return temp;
}

double* ParseByteByAngle()
{
	double* temp = new double[4];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // ��  x����ĽǶ�
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // ��   y����ĽǶ�
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // ��   z����ĽǶ�
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //�¶�  ���϶�
	temp[0] = 3;
	temp[1] = wx;
	temp[2] = wy;
	temp[3] = wz;
	return temp;
}
double* ParseByteByMagnetic_field()
{
	double* temp = new double[4];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]); //x��Ĵų�
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]); // y��Ĵų�
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]); //  z��Ĵų�
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //�¶�  ���϶�

	temp[0] = 2;
	temp[1] = wx;
	temp[2] = wy;
	temp[3] = wz;
	return temp;
}

double* ParseByteByPort_state()
{
	double* temp = new double[3];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // ��  x����ĽǶ�
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // ��   y����ĽǶ�
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // ��   z����ĽǶ�
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //�¶�  ���϶�


	return temp;
}

double* ParseByteByPressure_height()
{
	double* temp = new double[3];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // ��  x����ĽǶ�
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // ��   y����ĽǶ�
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // ��   z����ĽǶ�
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //�¶�  ���϶�

	return temp;
}

double*  ParseByteByLongitude_latitude()
{
	double* temp = new double[3];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // ��  x����ĽǶ�
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // ��   y����ĽǶ�
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // ��   z����ĽǶ�
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //�¶�  ���϶�

	temp[0] = wx;
	temp[1] = wy;
	temp[2] = wz;
	return temp;
}

double* ParseByteByGround_velocity()
{
	double* temp = new double[3];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // ��  x����ĽǶ�
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // ��   y����ĽǶ�
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // ��   z����ĽǶ�
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //�¶�  ���϶�

	temp[0] = wx;
	temp[1] = wy;
	temp[2] = wz;
	return temp;
}
/******************************************************************************
*����������  �ж����ݵķ���
*****************************************************************************/
double* ParseByteByFour_elements()
{
	double* temp = new double[3];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // ��  x����ĽǶ�
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // ��   y����ĽǶ�
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // ��   z����ĽǶ�
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //�¶�  ���϶�

	temp[0] = wx;
	temp[1] = wy;
	temp[2] = wz;
	return temp;
}
/******************************************************************************
*����������  ���Ƕ�λ����
*****************************************************************************/
double* ParseByteByAccuracy_satellitePositioning()
{
	double* temp = new double[3];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // ��  x����ĽǶ�
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // ��   y����ĽǶ�
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // ��   z����ĽǶ�
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //�¶�  ���϶�

	temp[0] = wx;
	temp[1] = wy;
	temp[2] = wz;
	return temp;
}
/******************************************************************************
*����������  �ж����ݵķ���
*****************************************************************************/
double MakeSign(uint8_t highByte, uint8_t lowByte)
{
	double outD = 0;
	if (highByte > 127) // ȡ����
	{
		outD = -1 * (254 - highByte) * 256 - lowByte;
	}
	else//����
	{
		outD = (highByte * 256 + lowByte);
	}
	return outD;
}

/******************************************************************************
*���������� У���
*****************************************************************************/
bool CheakSum()
{
	int sum = 0;
	bool flag = false;
	for (int i = 0; i < RD_WINDLEN - 1; i++)
	{
		sum += RD_buffer_win[i];
	}

	if (sum % 256 == RD_buffer_win[RD_WINDLEN - 1])
	{
		flag = true;
	}
	return flag;
}
