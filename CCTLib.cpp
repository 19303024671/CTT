// CCTLib.cpp : 定义静态库的函数。
//

#include "pch.h"
#include "framework.h"
#include "CCTLib.h"

using namespace std;

DrawCCT::DrawCCT(const CCTInfo& cct_info_)
{
	this->cct_infor = cct_info_;
	this->cct_infor.Init();
	this->cct_infor.bin = IntToBin(cct_infor.num,
		cct_infor.N);
}

DrawCCT::~DrawCCT()
{
}
/*
* @brief 整数转二进制vector
* 
* @param num 待转整数
* @param N 转成的位数，不足用0补齐
*/
vector<int> DrawCCT::IntToBin(int num,const int& N)
{
	vector<int> bin;
	for (size_t i = 0; i < N; i++)
	{
		if (num>0)
		{
			bin.push_back(num % 2);
			num = num / 2;
		}
		else
		{
			bin.push_back(0);
		}
	}
	reverse(bin.begin(), bin.end());
	return bin;
}
/*
* @brief 将二进制转成整数
*/
int DrawCCT::BinToInt(const vector<int>& bin, int N)
{
	if (bin.size() != N) return 0;
	int num = 0;
	for (int i : bin)
	{
		num += i *int(pow(2,N-1));
		N -= 1;	
	}
	return num;
}

void DrawCCT::DrawACCT()
{
	//创建文件夹与文件路径
	if (!fs::exists(cct_infor.dir_path))
	{
		if (!fs::create_directory(cct_infor.dir_path))
		{
			cerr << "创建文件夹：" << cct_infor.dir_path <<
				"失败！" << endl;
			return;
		}
	}
	string file_name = to_string(cct_infor.num) +
		string(".png");
	string file_path = cct_infor.dir_path + file_name;
	if (fs::exists(file_path))
	{
		cout << "文件：" << file_path << 
			"已存在！\n是否覆盖：(y/n)\n";
		string temp;
		cin >> temp;
		if (temp == "n") return;
	}
	//创建绘图画布
	cv::Mat image(cct_infor.size, cct_infor.size, CV_8UC3);
	switch (cct_infor.color)
	{
	case black:
		//设置画布颜色
		image.setTo(cv::Scalar(255, 255, 255));
		break;
	case white:
		image.setTo(cv::Scalar(0, 0, 0));
		break;
	default:
		break;
	}
	//绘制一个圆周
	Circle circle = cct_infor.circle_in;
	circle.thickness = 2;
	circle.radius = cct_infor.flabellate.radius;
	cv::circle(image, circle.center, circle.radius,
		circle.color, circle.thickness);
	//绘制扇形
	double unit_angle = 360.0 / cct_infor.N;//单位角度
	int k = 0;
	for (int i : cct_infor.bin)
	{
		if (i == 1) cv::ellipse(image,
			cct_infor.flabellate.center,
			cv::Size(cct_infor.flabellate.radius,
				cct_infor.flabellate.radius),
			0, k * unit_angle, (k + 1) * unit_angle,
			cct_infor.flabellate.color,
			cct_infor.flabellate.thickness);
		k += 1;
	}
	//绘制内大圆
	cv::circle(image, cct_infor.circle_out.center,
		cct_infor.circle_out.radius,
		cct_infor.circle_out.color,
		cct_infor.circle_out.thickness);
	//绘制内小圆
	cv::circle(image, cct_infor.circle_in.center,
		cct_infor.circle_in.radius,
		cct_infor.circle_in.color,
		cct_infor.circle_in.thickness);
	//保存绘制的图片
	cv::imwrite(file_path, image);
}

void CCTInfo::Init()
{
	flabellate.center = cv::Point(size / 2, size / 2);
	flabellate.radius = int(0.4 * size);
	circle_in.center = cv::Point(size / 2, size / 2);
	circle_in.radius = int(0.1 * size);
	circle_out.center = cv::Point(size / 2, size / 2);
	circle_out.radius = int(0.2 * size);
	switch (color)
	{
	case black:
		dir_path = string("./CCT_IMG_") + to_string(N)
			+ string("_Black/");	
		flabellate.color = cv::Scalar(0, 0, 0);
		circle_in.color = cv::Scalar(0, 0, 0);
		circle_out.color = cv::Scalar(255, 255, 255);
		break;
	case white:
		dir_path = string("./CCT_IMG_") + to_string(N)
			+ string("_White/");
		flabellate.color = cv::Scalar(255, 255, 255);
		circle_in.color = cv::Scalar(255, 255, 255);
		circle_out.color = cv::Scalar(0, 0, 0);
		break;
	default:
		break;
	}
}

CCTInfo::CCTInfo(const int& N_, const CCTColor& color_, 
	const int& size_, const int& num_):
	N(N_),color(color_),size(size_),num(num_)
{
}

CCTInfo::CCTInfo()
{
}

