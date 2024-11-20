#include <string>
#include <cstdlib>
#include "json/json.h"
#include "jsoncpp.cpp"
#include <iostream>
#include <fstream>
#include<cmath>
#include "io.h"  
#include <vector>  
#include <stdio.h>
#include<afx.h>
#include <WINDOWS.H>
using namespace std;

#define PI 3.14159265
#define file_read_path_0 "D:\\test\\testForAlgorithm_JSON\\*.json"
#define file_read_path "D:\\test\\testForAlgorithm_JSON\\"
#define file_read_path_clean "D:\\test\\testForAlgorithm_JSON\\*.json"//to clean


#define img_from_pi_path_0 "D:\\test\\imgFromPi\\*.jpg"
#define img_from_pi_path "D:\\test\\imgFromPi\\"
#define img_from_pi_path_clean "D:\\test\\imgFromPi\\*.jpg"//to clean


#define img_prepare_to_openpose_path "D:\\test\\inputTest\\"
#define img_prepare_to_openpose_path_clean "D:\\test\\inputTest\\*.jpg"//to clean


int readFileJson(string input_json_data, double point_data[][3]);//读取json 
double count_angle(double point_data[][3], int i, int j);//计算i 和 j 的角度
double count_angle2(double point_data[][3], int i, int j);
void processJson();//json处理程序
void clean();// 之前残留文件擦除
void clean2();// imgFromPi堆积文件擦除


int readFileJson(string input_json_data, double point_data[][3])//读取json 
{
	Json::Reader reader;
	Json::Value root;

	//确认文件读取状态
	ifstream in(input_json_data, ios::binary);

	if (!in.is_open())
	{
		cout << "Error opening file\n";
		return -2;
	}

	if (reader.parse(in, root))
	{

		string people = root["people"].toStyledString();
		int data_start_temp = people.find("pose_keypoints_2d");
		if (data_start_temp == -1)return -1;

		int data_start = people.find("[", data_start_temp);
		int data_end = people.find("]", data_start);
		string a = people.substr(data_start, data_end - data_start + 1);

		for (int i = 0, point_count = 0, count = 0;; i++)
		{

			if (i == a.length())break;

			string temp = "";
			for (; (a[i] == '.' || ('0' <= a[i] && a[i] <= '9')); i++)
			{
				temp += a[i];
			}
			if (temp == "")continue;

			point_data[point_count][count] = atof(temp.c_str());

			count++;
			if (count == 3)
			{
				count = 0;
				point_count++;
			}
		}
	}

	return 0;

}

double count_angle(double point_data[][3], int i, int j)
{

	double x_d = (point_data[i][0] - point_data[j][0]);
	double y_d = (point_data[i][1] - point_data[j][1]);

	double angle = (atan(x_d / y_d));//angle是弧度制
	double angle_2 = angle * 180 / PI;//angle_2角度制

	return angle_2;
}

double count_angle2(double point_data[][3], int i, int j)//  y/x
{

	double x_d = (point_data[i][0] - point_data[j][0]);
	double y_d = (point_data[i][1] - point_data[j][1]);

	double angle = (atan(y_d / x_d));//angle是弧度制
	double angle_2 = angle * 180 / PI;//angle_2角度制

	return angle_2;
}


void processJson()
{
	double point_data[25][3];
	long Handle;
	struct _finddata_t FileInfo;

	if ((Handle = _findfirst(file_read_path_0, &FileInfo)) == -1L)cout << "没有找到任何一个json文件\n";
	else   for (int i = 0; ; i++)
	{
		int img_have_people;
		img_have_people = readFileJson(string(file_read_path) + FileInfo.name, point_data);
		if (img_have_people == -1)
		{
			cout << "\n照片:" << string(FileInfo.name) + " 中没有检测到人\n";
			remove((string(file_read_path) + FileInfo.name).c_str());

			if (_findnext(Handle, &FileInfo) == -1)break;

			continue;
		}

		double neck_nose_angle = count_angle(point_data, 0, 1);
		double neck_midhip_angle = count_angle(point_data, 1, 8);
		double neck_Lear_angle = count_angle(point_data, 18, 1);
		double Lear_neck_midhip = neck_Lear_angle - neck_midhip_angle;

		//sending result
		string result = "";

		if (neck_midhip_angle > 13)result = "1";
		else if (neck_midhip_angle < -13)result = "2";
		else result = "0";

		if (Lear_neck_midhip > 12)result += "1";
		else result += "0";

		if (result != "00")
		{
			result = "..\\client.exe --" + result;
			system(result.c_str());
			Sleep(5000);//5000ms
		}

		remove((string(file_read_path) + FileInfo.name).c_str());


		if (_findnext(Handle, &FileInfo) == -1)break;

	}

	_findclose(Handle);

	return;
}



void clean()
{
	//对inputtest文件夹擦除 
	long Handle_img;
	struct _finddata_t FileInfo_img;
	Handle_img = _findfirst(img_prepare_to_openpose_path_clean, &FileInfo_img);
	for (; Handle_img != -1L;)
	{
		remove((string(img_prepare_to_openpose_path) + FileInfo_img.name).c_str());

		Handle_img = _findfirst(img_prepare_to_openpose_path_clean, &FileInfo_img);

	}
	_findclose(Handle_img);

	//对imgFromPi文件夹擦除
	Handle_img = _findfirst(img_from_pi_path_clean, &FileInfo_img);
	for (; Handle_img != -1L;)
	{
		remove((string(img_from_pi_path) + FileInfo_img.name).c_str());

		Handle_img = _findfirst(img_from_pi_path_clean, &FileInfo_img);

	}
	_findclose(Handle_img);


	//对testForAlgorithm_JSON文件夹擦除 
	long Handle;
	struct _finddata_t FileInfo;
	Handle = _findfirst(file_read_path_clean, &FileInfo);
	for (; Handle != -1L;)
	{
		remove((string(file_read_path) + FileInfo.name).c_str());

		Handle = _findfirst(file_read_path_clean, &FileInfo);

	}
	_findclose(Handle);

	return;
}

void clean2()
{
	long Handle_img;
	struct _finddata_t FileInfo_img;

	string pre = "";//前一个文件路径
	Handle_img = _findfirst(img_from_pi_path_clean, &FileInfo_img);
	if (Handle_img == -1L)return;

	pre = string(img_from_pi_path) + FileInfo_img.name;
	for (; _findnext(Handle_img, &FileInfo_img) != -1;)
	{
		remove(pre.c_str());
		pre = string(img_from_pi_path) + FileInfo_img.name;

	}
	_findclose(Handle_img);

	return;
}


int main(int argc, char* argv[])
{
	// 之前残留文件擦除
	clean();

	//读取树莓派传输过来的图像文件 -预处理 
	long Handle_img;
	struct _finddata_t FileInfo_img;
	for (int i = 0; (Handle_img = _findfirst(img_from_pi_path_0, &FileInfo_img)) == -1L; i++)
	{
		std::cout << "没有找到任何一个jpg文件\n";
		Sleep(2000);//延迟2000ms

	}

	//大循环
	for (;;)
	{

		//读取树莓派传输过来的图像文件
		FILE* filepath;
		errno_t error;
		error = fopen_s(&filepath, (string(img_from_pi_path) + string(FileInfo_img.name)).c_str(), "rb+"); 
		
		if (error != 0)
		{
			cout << "using!!!\n";
			Sleep(20);//延迟20ms
			continue;
		}
		fclose(filepath);
		CString sourcefile = (string(img_from_pi_path) + string(FileInfo_img.name)).c_str();
		CString targetfile = (string(img_prepare_to_openpose_path) + string(FileInfo_img.name)).c_str();
		MoveFile(sourcefile, targetfile);

		//调用openpose
		system("..\\OpenPoseDemo.exe");

		//删除openpose处理过的图像文件
		remove((string(img_prepare_to_openpose_path) + FileInfo_img.name).c_str());

		//对openpose得到的json文件进行处理 
		processJson();

		//堆积文件擦除
		clean2();

		//读取下一张图片
		if (_findnext(Handle_img, &FileInfo_img) != -1)continue;
		for (int i = 0; (Handle_img = _findfirst(img_from_pi_path_0, &FileInfo_img)) == -1L; i++)
		{
			std::cout << "没有找到下一个jpg文件\n";
			Sleep(200);//延迟200ms
		}


	}

	_findclose(Handle_img);
	return 0;
}
