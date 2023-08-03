#pragma once
#define PI 3.1415926535

#include<iostream>
#include<string>
#include<vector>
#include<filesystem>
#include<opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include"ProgressBar.hpp"


using namespace std;
namespace fs = std::filesystem;

//环形码的颜色
enum CCTColor
{
	black = 1,
	white = 0	
};
//库的返回结果
struct Result
{
	int index;
	cv::Point pos;
public:
	Result(const int& index_, const cv::Point& pos_);
	Result(const Result& re_);
	Result();
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
struct DrawCCTOnP
{
	int num;//编码的整数
	int N;//编码位数
	CCTColor color;//编码的颜色
	cv::Mat img;//待绘制的图片
	cv::Point pos;//绘制的位置
public:
	DrawCCTOnP(const int& num_, const int& N_,
		const CCTColor& color_, const cv::Mat& img_, const cv::Point& pos_);
	DrawCCTOnP(const DrawCCTOnP& info_);
	DrawCCTOnP();
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
//识别编码总函数
vector<Result> DecodeCCT(const DetectCCTInfo& detect_cct_info);
//二进制转十进制
int BinToInt(const vector<int>& bin);
//画一张CCT
cv::Mat DrawACCT(const ACCTInfo& acct_info);
//画一批CCT
vector<cv::Mat> DrawCCTs(const CCTInfo& cct_info);
//十进制转二进制
vector<int> IntToBin(const int& num_,const int&N);
//在一张图上画一批CCT
cv::Mat DrawCCTsOnAPic(const CCTInfo& cct_info);
//二进制移位，用在二进制转最小十进制上
vector<int> MoveBit(const vector<int>& bin);
//判断是不是已经有同心的椭圆在椭圆容器里了，用在从轮廓中提取到同心最小的椭圆轮廓容器
bool IsIn(const cv::RotatedRect& temp_r,const vector<cv::RotatedRect> & ellipse_rects_c1);
//计算仿射变换矩阵，抄的Python代码
cv::Mat GetTransMatrix(const cv::Mat& src, const cv::Mat& dst);
//读取图像
cv::Mat ReadImg(const string& file_path);
//图片前处理，包括灰度化与二值化
cv::Mat TranImgPre(const cv::Mat& color_img);
//提取所有合适的轮廓：ellipse_rects
vector<cv::RotatedRect> GetAllEs(const cv::Mat& binary_img);
//提取里面的椭圆轮廓：ellipse_rects_c1
vector<cv::RotatedRect> Get1Es(const vector<cv::RotatedRect>& ellipse_rects);
//获取2与3
vector<vector<cv::RotatedRect>> Get2_3Es(const vector<cv::RotatedRect>& ellipse_rects_c1);
//剪切图像
vector<cv::Mat> CutImg(const vector<cv::RotatedRect>& e3, const cv::Mat& color_img);
//得到该图像的所有结果
vector<Result> GetResult(const int& N, const CCTColor& color, 
	const string& file_path,
	const cv::Mat& color_img, 
	const vector<cv::RotatedRect>& ellipse_rects_c1,
	const vector<cv::RotatedRect>& ellipse_rects_c2, 
	const vector<cv::RotatedRect>& ellipse_rects_c3, 
	const vector<cv::Mat>&cct_imgs);
//仿射变换+处理剪切的图像
cv::Mat TranImg(const cv::Mat& img,
	const cv::RotatedRect& box1, 
	const cv::RotatedRect& box3);
//解码
int Decode(const int&N,const CCTColor&color,const cv::Mat &erode_img,
	const cv::RotatedRect& box2,
	const cv::RotatedRect& box3);
//绘制
void DrawResult(const int& a_result, const cv::Mat& color_img,
	const cv::RotatedRect& box1,
	const cv::RotatedRect& box2,
	const cv::RotatedRect& box3);
int Decode2(const int& N, const CCTColor& color, const cv::Mat& erode_img);

void DrawIpadImg(const int& width, const int& height);

cv::Mat DrawACCTOnPic(const DrawCCTOnP& info);

bool IsOk(const int& re, const vector<int>& tem);

vector<int> GetTemp(const int& N, const int& a, const int& b, const cv::Mat& img, const cv::Point& center);