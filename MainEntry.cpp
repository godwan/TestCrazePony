#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <windows.h>

#include "AHRSEntry.h"
#include "imu.h"

using std::string;
using namespace std;

double*  ReadData(uint8_t idata);
//float* calculatingAttitudeAngle(double *p);
//int gaitNum(double[][14]);
//http://www.crazepony.com/wiki/software-algorithm.html
void  main(int argc, char *args[])
{
	static int InitFlag = 1; //  ��ʼ��
	//string Path = "D:\\Wan_Project\\ForYWProject\\�������\\����Ь\\jy901\\programAndData\\data\\gait\\sps100\\jo\\0622\\left1.txt";
	// sps = 20
	//char* Path =  "D:\\Wan_Project\\ForZHJKProject\\����Ь\\jy901\\programAndData\\data\\gait\\sps20\\data4.txt";
	// sps = 100
	char* Path = "D:\\Wan_Project\\ForZHJKProject\\����Ь\\jy901\\programAndData\\data\\gait\\sps100\\zqj\\0607\\left1.txt";

	string attitude = "C:\\Users\\asus\\Desktop\\attitude\\attitude.txt"; //�����̬��
	static ofstream  file1;
	file1.open(attitude);



	errno_t err; // �жϴ��ļ��ɹ���񣬲��ɹ�����0������Ϊ1
	static int x[11] = { 0 };
	static int xCount = 0;
	FILE *fid;
	char c;
	errno_t s32Err;
	//ifstream fin(Path);
	char buffer[1024];
	double* p = NULL;

	int numofGait = 0;
	int i, j;
	//fid = fopen(Path,"rb");
	s32Err = fopen_s(&fid,Path,"rt");

	if (s32Err != 0)
	{
		printf("read data error");
		return;
	}
	else 
	{
		printf("file was opened");
	}
	//��ȡ�ļ���С
	fseek(fid, 0, SEEK_END);
	long lSize = ftell(fid);
	rewind(fid);
	//���ٴ洢�ռ�
	int num = lSize / sizeof(uint8_t);
	uint8_t *pos = (uint8_t*)malloc(sizeof(uint8_t)*num);
	if (pos == NULL)
	{
		printf("���ٿռ����");
		return;
	}
	fread(pos, sizeof(uint8_t), num, fid);


	double dataWin[9][14] = {0};
	double data_imu_out[12] = {0};
	int counterWin = 0;
	

	for (i = 0; i < num; i++){
		//printf("%d\n", pos[i]);

		double* p = ReadData(pos[i]);
		
		if (p != NULL) {
			if (p[0] == 0)
			{
				for (j = 0; j < 3; j++)
					data_imu_out[j] = p[j + 1];
			}
			else if(p[0] == 1)
			{
				for (j = 0; j < 3; j++)
					data_imu_out[j + 3] = p[j + 1];
			}
			else if (p[0] == 2)//�������ų�����
			{
				for (j = 0; j < 3; j++)
					data_imu_out[j + 6] = p[j + 1];

				if (!InitFlag)
				{
					SmCare_AHRS_Entry(data_imu_out, 0);
				}
				else
				{
					SmCare_AHRS_Entry(data_imu_out, 1);
					InitFlag = 0;
				}
				cout << "*****************************jy901*****************************" << endl;
				cout << "������" << data_imu_out[9] << "��ת��" << data_imu_out[10] << "ƫ����" << data_imu_out[11] << endl;
				cout << "*****************************�����˲�*****************************" << endl;
				cout << "������" << imu.pitch << "��ת��" << imu.roll << "ƫ����" << imu.yaw << endl;
				file1 <<  imu.pitch << "," << imu.roll << "," << imu.yaw << endl;

				dataWin[0][counterWin] = data_imu_out[0];
				dataWin[1][counterWin] = data_imu_out[1];
				dataWin[2][counterWin] = data_imu_out[2];
				dataWin[3][counterWin] = data_imu_out[3];
				dataWin[4][counterWin] = data_imu_out[4];
				dataWin[5][counterWin] = data_imu_out[5];
				dataWin[6][counterWin] = data_imu_out[6];
				dataWin[7][counterWin] = data_imu_out[7];
				dataWin[8][counterWin] = data_imu_out[8];
				if (++counterWin == 14) 
				{
					/*if (gaitNum(dataWin) == 1) 
					{
						numofGait++;
						cout << "we have " << numofGait <<"steps be check��"<<endl;
					}*/
					counterWin = 0;
				}

			}
			else if (p[0] == 3)
			{
				for (j = 0; j < 3; j++)
					data_imu_out[j + 9] = p[j + 1];
			}
			
		}
		free(p);
		//sleep(5);
	}
	free(pos);     //�ͷ��ڴ�
	file1.close();
}

