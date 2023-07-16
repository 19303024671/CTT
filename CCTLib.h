#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<filesystem>
#include<opencv2/opencv.hpp>

using namespace std;
namespace fs = std::filesystem;

//环形码的颜色
enum CCTColor
{
	black = 1,
	white = 0	
};
//识别环形码所需的信息
struct DetectCCTInfo
{
	string dir_path;//环形码所在的文件夹路径
	vector<string> img_file_paths;//路径下所有图片文件路径
	CCTColor color;//环形码的颜色
public:
	void Init();
};
//扇形
struct Flabellate
{
	cv::Point center;//圆心
	int radius;//半径
	double start_angle;//起始角度
	double end_angle;//终止角度
	int thickness = -1;//厚度，-1表示填充
	cv::Scalar color;//颜色(BGR顺序)
};
//圆
struct Circle
{
	cv::Point center;//圆心
	int radius;//半径
	int thickness = -1;//厚度
	cv::Scalar color;//颜色
};
//绘制环形码所需的信息
struct CCTInfo
{
	string dir_path;//文件夹路径
	int N;//等分数
	CCTColor color;//环形码颜色
	int size;//图片尺寸
	int num;//编码的整数
	int max_num;//N位二进制表示的最大整数
	vector<int> bin;//编码的二进制vector
	Flabellate flabellate;//扇形信息
	Circle circle_out;//外圆形（大）信息
	Circle circle_in;//内圆形（小）信息

public:
	void Init();
	CCTInfo(const int&N_,const CCTColor&color_,const int&size_,const int&num);
	CCTInfo();
};

class DrawCCT
{
public:
	DrawCCT(const CCTInfo& cct_info);
	~DrawCCT();
	//将整数转成二进制vector
	vector<int> IntToBin( int num,const int& N);
	//将二进制的vector转成整数
	int BinToInt(const vector<int>& bin, int N);
	//绘制一张CCT图像
	cv::Mat DrawACCT();
	void DrawaCCT();
	vector<cv::Mat> DrawcCTs();
	void DrawCCTs();
	void DrawCCTsOnAPic();
private:
	CCTInfo cct_infor;
};
class DetectCCT
{
public:
	DetectCCT();
	~DetectCCT();

private:

};



