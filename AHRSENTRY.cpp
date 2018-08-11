/**************************************************************************************
Filename:	AHRSENTRY.c
Author:		JeromeWan  from SmartCare
Description:
***************************************************************************************/
# include "AHRSEntry.h"
# include "imu.h"
# include "IMUSO3.h"
/**************************************************************************************
函数名：void SmCare_AHRS_Entry(double *p,int init) 
描  述：姿态解算融合主框架函数
***************************************************************************************/
void SmCare_AHRS_Entry(double *p,int init) 
{
	//算法内部初始化
	if (init) 
	{
		calculatingAttitudeAngle(p, init);
	}

	if (imu.ready) {
		//processing
		/*
		1.根据标定后的数据来解析当前姿态角
		2.根据姿态角及传感器数据来提取步态参数
		3.步态参数的群体分析
		4.步态的多参数分析。
		*/
        calculatingAttitudeAngle(p, init);
	}
	else 
	{
		// preprocess
		/*
		1.对数据的要求： 前面预留一段时间的数据，实际工作时要求受试预先静止段时间
		2.检测数据的准确性
		3.抓取一段数据用来标定/校准传感器数据，方法可以用六面体模型来标定
		4.输出校准/标定后的传感器数据。
		*/
	    IMU_Pre(p, init);
	}
	
	
	
	//return;
	/*
	1.返回步态中间参数 用作辅助检测
	2.返回步态参数，用作输出。
	*/
}