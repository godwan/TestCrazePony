/**************************************************************************************
Filename:	imu.c
Author:		JeromeWan
***************************************************************************************/
//#include "config.h"
//#include "DMP.h"
#include "imu.h"
#include "filter.h"
#include "SysConfig.h"
imu_t imu= {0};
int imuCaliFlag=0;
/**************************************************************************************
函数名：IMU_Pre(double *p)
描  述：姿态解算融合前准备函数
***************************************************************************************/
void IMU_Pre(double *p,int init) 
{
		IMU_Calibrate(p,init);
}
/**************************************************************************************
函数名：IMU_Calibrate(double* p)
描  述：should place to a level surface and keep it stop for 1~2 second return 1 when finish
***************************************************************************************/
void IMU_Calibrate(double* p,int init)
{
    //3s
    static float accSum[3]= {0,0,0};
    static float gyroSum[3]= {0,0,0};
    static int cnt=0;
    int ret=0;
    int i=0;
    int dt=0,now=0;
	dt = 10;

    if(init)
    {
        for(i=0; i<3; i++)
        {
            accSum[i]=0;
            gyroSum[i]=0;
            cnt=0;
            imu.ready=0;
        }
    }

    if(cnt<300)
    {
        for(i=0; i<3; i++)
        {
            accSum[i] += (float)p[i];
            gyroSum[i] += (float)p[i+3];
        }
        cnt++;
    }
    else
    {
        for(i=0; i<3; i++)
        {
            imu.accOffset[i] = accSum[i]/(float)cnt;
            imu.gyroOffset[i] = gyroSum[i]/(float)cnt;
        }

        imu.accOffset[2] = imu.accOffset[2] - CONSTANTS_ONE_G;
        imu.ready = 1;
    }
}


//#define SENSOR_MAX_G 8.0f		//constant g		// tobe fixed to 8g. but IMU need to  correct at the same time
//#define SENSOR_MAX_W 2000.0f	//deg/s
//#define ACC_SCALE  (SENSOR_MAX_G/32768.0f)
//#define GYRO_SCALE  (SENSOR_MAX_W/32768.0f)
//void ReadIMUSensorHandle(void)
//{
//    int i;
//
//    //read raw
//    MPU6050AccRead(imu.accADC);
//    MPU6050GyroRead(imu.gyroADC);
//    //tutn to physical
//    for(i=0; i<3; i++)
//    {
//        imu.accRaw[i]= (float)imu.accADC[i] *ACC_SCALE * CONSTANTS_ONE_G ;
//        imu.gyroRaw[i]=(float)imu.gyroADC[i] * GYRO_SCALE * M_PI_F /180.f;		//deg/s
//    }
//
//    imu.accb[0]=LPF2pApply_1(imu.accRaw[0]-imu.accOffset[0]);
//    imu.accb[1]=LPF2pApply_2(imu.accRaw[1]-imu.accOffset[1]);
//    imu.accb[2]=LPF2pApply_3(imu.accRaw[2]-imu.accOffset[2]);
//#ifdef IMU_SW
//    imu.gyro[0]=LPF2pApply_4(imu.gyroRaw[0]);
//    imu.gyro[1]=LPF2pApply_5(imu.gyroRaw[1]);
//    imu.gyro[2]=LPF2pApply_6(imu.gyroRaw[2]);
//#else
//    imu.gyro[0]=LPF2pApply_4(imu.gyroRaw[0]-imu.gyroOffset[0]);
//    imu.gyro[1]=LPF2pApply_5(imu.gyroRaw[1]-imu.gyroOffset[1]);
//    imu.gyro[2]=LPF2pApply_6(imu.gyroRaw[2]-imu.gyroOffset[2]);
//#endif
//
//}
#define ACCZ_ERR_MAX  0.05		//m/s^2
#define CHECK_TIME 5
/**************************************************************************************
函数名：int IMUCheck(double *p)
描  述：检测IMU是否ready，校准好,需要将四轴放水平
***************************************************************************************/
int IMUCheck(double *p)
{
    float accZSum=0;
    int i;
    float accZb=0;

    for(i=0; i<CHECK_TIME; i++)
    {
        accZSum += (float)p[2];
    }
    imu.accRaw[2]= (float)(accZSum /(float)CHECK_TIME);
    accZb=imu.accRaw[2]-imu.accOffset[2];

    if((accZb > CONSTANTS_ONE_G-ACCZ_ERR_MAX ) && (accZb < CONSTANTS_ONE_G + ACCZ_ERR_MAX))
        imu.caliPass=1;
    else
        imu.caliPass=0;

    return imu.caliPass;

}
/**************************************************************************************
函数名：void eular2DCM(float DCM[3][3],float roll,float pitch,float yaw)
描  述：in standard sequence , roll-pitch-yaw , x-y-z
angle in rad  ,get DCM for ground to body
***************************************************************************************/
void eular2DCM(float DCM[3][3],float roll,float pitch,float yaw)
{
    float cosx, sinx, cosy, siny, cosz, sinz;
    float coszcosx, coszcosy, sinzcosx, coszsinx, sinzsinx;

    cosx = cosf(roll * M_PI_F/180.0f);
    sinx = sinf(roll * M_PI_F/180.0f);
    cosy = cosf(pitch * M_PI_F/180.0f);
    siny = sinf(pitch * M_PI_F/180.0f);
    cosz = cosf(yaw * M_PI_F/180.0f);
    sinz = sinf(yaw * M_PI_F/180.0f);

    coszcosx = cosz * cosx;
    coszcosy = cosz * cosy;
    sinzcosx = sinz * cosx;
    coszsinx = sinx * cosz;
    sinzsinx = sinx * sinz;

    DCM[0][0] = coszcosy;
    DCM[0][1] = cosy * sinz;
    DCM[0][2] = -siny;
    DCM[1][0] = -sinzcosx + (coszsinx * siny);
    DCM[1][1] = coszcosx + (sinzsinx * siny);
    DCM[1][2] = sinx * cosy;
    DCM[2][0] = (sinzsinx) + (coszcosx * siny);
    DCM[2][1] = -(coszsinx) + (sinzcosx * siny);
    DCM[2][2] = cosy * cosx;

}

