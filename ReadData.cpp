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
	if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 81)//解析的是加速度包
	{
		if (CheakSum())
		{
			p = ParseByteBySpeed();
				// data.Clear();
		}
	}
	else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 82) //解析的是角速度包
	{
		if (CheakSum())
		{
			p = ParseByteByAngle_velocity();
			// data.Clear();
		}
	}
    else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 84)// 解析的是磁场
	{
		if (CheakSum())
		{
			p = ParseByteByMagnetic_field();
		}
	}
	else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 83)// 解析的是角度包
	{
		if (CheakSum())
		{
			p = ParseByteByAngle();
		}
	}
	



///////////////////////////////////complete///////////////////////////////////////////////

 //   if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 80)//解析的是时间
 //   {
	//	if (CheakSum())
	//	{
	//			//dataOut = ParseByteByDate(RD_buffer_winR);
	//	}		
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 81)//解析的是加速度包
	//{
	//	if (CheakSum())
	//	{
	//		p = ParseByteBySpeed();
	//			// data.Clear();
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 82) //解析的是角速度包
	//{
	//	if (CheakSum())
	//	{
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 83)// 解析的是角度包
	//{
	//	if (CheakSum())
	//	{
	//	}
	//	else
	//	{
	//		
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 84)// 解析的是磁场
	//{
	//	if (CheakSum())
	//	{
	//			
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 85)// 解析的是端口状态
	//{
	//	if (CheakSum())
	//	{
	//		
	//	}
	//		
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 86)// 解析的是气压、高度数据
	//{
	//	if (CheakSum())
	//	{	
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 87)// 解析的是经度、纬度数据
	//{
	//	if (CheakSum())
	//	{
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 88)// 解析的是地速数据
	//{
	//	if (CheakSum())
	//	{

	//	}
	//}

	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 89)// 解析的是四元素
	//{
	//	if (CheakSum())
	//	{
	//	}
	//}
	//else if (RD_buffer_win[0] == 85 && RD_buffer_win[1] == 90)// 解析的是卫星定位精度
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

	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 16;// * G; // m/s^2   x方向的加速度
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 16;// * G; // m/s^2   y方向的加速度
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 16;// * G; // m/s^2  z方向的加速度
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //温度  摄氏度
																	// Console.WriteLine("加速度：x方向" + wx.ToString() + "  y方向" + wy.ToString() + "  z方向" + wz.ToString());
	return temp;
}

double* ParseByteBySpeed()
{
	double* temp = new double[4];

	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 16 * CONSTANTS_ONE_G;// * G; // m/s^2   x方向的加速度
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 16 * CONSTANTS_ONE_G;// * G; // m/s^2   y方向的加速度
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 16 * CONSTANTS_ONE_G;// * G; // m/s^2  z方向的加速度
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //温度  摄氏度
																	//Console.WriteLine("加速度：x方向"+wx.ToString()+"  y方向"+ wy.ToString()+"  z方向"+ wz.ToString());
    temp[0] = 0;
	temp[1] = wx;
	temp[2] = wy;
	temp[3] = wz;
	
	return temp;
}




double* ParseByteByAngle_velocity()
{
	double* temp = new double[4];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 2000 * PI / 180.0; // rad/s   x方向的角速度
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 2000 * PI / 180.0; // rad/s   y方向的角速度
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 2000 * PI / 180.0; // rad/s   z方向的角速度
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //温度  摄氏度
	//Console.WriteLine("角速度：x方向角速度", wx, "y方向角速度", wy, "z方向角速度", wz);															
	//Console.WriteLine("角速度：x方向角速度", wx, "y方向角速度", wy, "z方向角速度", wz)
	temp[0] = 1;
	temp[1] = wx;
	temp[2] = wy;
	temp[3] = wz;
	return temp;
}

double* ParseByteByAngle()
{
	double* temp = new double[4];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // 度  x方向的角度
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // 度   y方向的角度
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // 度   z方向的角度
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //温度  摄氏度
	temp[0] = 3;
	temp[1] = wx;
	temp[2] = wy;
	temp[3] = wz;
	return temp;
}
double* ParseByteByMagnetic_field()
{
	double* temp = new double[4];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]); //x轴的磁场
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]); // y轴的磁场
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]); //  z轴的磁场
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //温度  摄氏度

	temp[0] = 2;
	temp[1] = wx;
	temp[2] = wy;
	temp[3] = wz;
	return temp;
}

double* ParseByteByPort_state()
{
	double* temp = new double[3];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // 度  x方向的角度
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // 度   y方向的角度
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // 度   z方向的角度
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //温度  摄氏度


	return temp;
}

double* ParseByteByPressure_height()
{
	double* temp = new double[3];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // 度  x方向的角度
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // 度   y方向的角度
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // 度   z方向的角度
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //温度  摄氏度

	return temp;
}

double*  ParseByteByLongitude_latitude()
{
	double* temp = new double[3];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // 度  x方向的角度
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // 度   y方向的角度
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // 度   z方向的角度
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //温度  摄氏度

	temp[0] = wx;
	temp[1] = wy;
	temp[2] = wz;
	return temp;
}

double* ParseByteByGround_velocity()
{
	double* temp = new double[3];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // 度  x方向的角度
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // 度   y方向的角度
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // 度   z方向的角度
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //温度  摄氏度

	temp[0] = wx;
	temp[1] = wy;
	temp[2] = wz;
	return temp;
}
/******************************************************************************
*解析数据用  判断数据的符号
*****************************************************************************/
double* ParseByteByFour_elements()
{
	double* temp = new double[3];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // 度  x方向的角度
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // 度   y方向的角度
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // 度   z方向的角度
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //温度  摄氏度

	temp[0] = wx;
	temp[1] = wy;
	temp[2] = wz;
	return temp;
}
/******************************************************************************
*解析数据用  卫星定位精度
*****************************************************************************/
double* ParseByteByAccuracy_satellitePositioning()
{
	double* temp = new double[3];
	double wx = MakeSign(RD_buffer_win[3], RD_buffer_win[2]) / 32768 * 180; // 度  x方向的角度
	double wy = MakeSign(RD_buffer_win[5], RD_buffer_win[4]) / 32768 * 180; // 度   y方向的角度
	double wz = MakeSign(RD_buffer_win[7], RD_buffer_win[6]) / 32768 * 180; // 度   z方向的角度
	double temperture = MakeSign(RD_buffer_win[9], RD_buffer_win[8]) / 340 + 36.53; //温度  摄氏度

	temp[0] = wx;
	temp[1] = wy;
	temp[2] = wz;
	return temp;
}
/******************************************************************************
*解析数据用  判断数据的符号
*****************************************************************************/
double MakeSign(uint8_t highByte, uint8_t lowByte)
{
	double outD = 0;
	if (highByte > 127) // 取补码
	{
		outD = -1 * (254 - highByte) * 256 - lowByte;
	}
	else//整数
	{
		outD = (highByte * 256 + lowByte);
	}
	return outD;
}

/******************************************************************************
*解析数据用 校验和
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
