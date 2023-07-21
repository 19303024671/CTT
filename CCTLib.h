#pragma once
#define PI 3.1415926535

#include<iostream>
#include<string>
#include<vector>
#include<filesystem>
#include<opencv2/opencv.hpp>
#include"ProgressBar.hpp"


using namespace std;
namespace fs = std::filesystem;

//环形码的颜色
enum CCTColor
{
	black = 1,
	white = 0	
};
//文本
struct Text
{
	string text;//文本
	cv::Point position; // 文本位置
	int fontFace = cv::FONT_HERSHEY_SIMPLEX; // 字体类型
	double fontScale = 1.5; // 字体大小
	cv::Scalar color = cv::Scalar(0,255,0); // 文本颜色 (BGR格式)
	int thickness = 2; // 文本线宽
	int lineType = cv::LINE_AA; // 文本线型
public:
	Text(const string& text_, const cv::Point& position_);
	Text(const Text& text_);
	Text();
};
//识别环形码所需的信息
struct DetectCCTInfo
{
	string file_path;//环形码路径
	CCTColor color;//环形码的颜色
	int N;//编码位数
public:
	DetectCCTInfo(const string& file_path_, const CCTColor& color_ , const int&N_);
	DetectCCTInfo(const DetectCCTInfo&detect_cct_info_);
	DetectCCTInfo();
};
//扇形
struct Flabellate
{
	cv::Point center;//圆心
	int radius;//半径
	int thickness = -1;//厚度，-1表示填充
	cv::Scalar color;//颜色(BGR顺序)
public:
	Flabellate(const cv::Point& center_, const int& radius_, const int& thickness_, const cv::Scalar color_);
};
//圆
struct Circle
{
	cv::Point center;//圆心
	int radius;//半径
	int thickness = -1;//厚度
	cv::Scalar color;//颜色
public:
	Circle(const cv::Point& center_, const int& radius_, const int& thickness_, const cv::Scalar& color_);
};
//绘制一个环形码所需的信息
struct ACCTInfo
{
	int num;//编码的整数
	int N;//编码位数
	CCTColor color;//编码的颜色
	int size;//图片尺寸
public:
	ACCTInfo(const int& num_, const int& N_, const CCTColor& color_, const int& size_);
	ACCTInfo(const ACCTInfo& acct_info_);
	ACCTInfo();
};
//绘制一批环形码所需的信息
struct CCTInfo
{
	int N;//等分数
	CCTColor color;//环形码颜色
	int size;//图片尺寸
public:
	CCTInfo(const int& N_, const CCTColor& color_, const int& size_);
	CCTInfo(const CCTInfo& cct_info_);
	CCTInfo();
};

vector<int> DecodeCCT(const DetectCCTInfo& detect_cct_info);

int BinToInt(const vector<int>& bin);

cv::Mat DrawACCT(const ACCTInfo& acct_info);

vector<cv::Mat> DrawCCTs(const CCTInfo& cct_info);

vector<int> IntToBin(const int& num_,const int&N);

cv::Mat DrawCCTsOnAPic(const CCTInfo& cct_info);

vector<int> MoveBit(const vector<int>& bin);

bool IsIn(const cv::RotatedRect& temp_r,const vector<cv::RotatedRect> & ellipse_rects_c1);