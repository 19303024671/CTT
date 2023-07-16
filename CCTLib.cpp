﻿// CCTLib.cpp : 定义静态库的函数。
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
	//计算N位二进制表示的最大十进制
	vector<int>max_bin;
	for (int i = 0; i < cct_infor.N; i++)
		max_bin.push_back(1);
	this->cct_infor.max_num = BinToInt(max_bin, cct_infor.N);
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

cv::Mat DrawCCT::DrawACCT()
{
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
	return image;
}

void DrawCCT::DrawaCCT()
{
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
	//保存绘制的图片
	cv::imwrite(file_path, DrawACCT());
}

vector<cv::Mat> DrawCCT::DrawcCTs()
{
	vector<cv::Mat> images;
	for (int i = 0; i < cct_infor.max_num; i++)
	{
		CCTInfo cct_info_temp = cct_infor;
		cct_info_temp.num = i;
		cct_info_temp.Init();
		DrawCCT draw_cct_temp(cct_info_temp);
		images.push_back(draw_cct_temp.DrawACCT());
	}
	return images;
}

void DrawCCT::DrawCCTs()
{
	//创建文件路径vector
	vector<string> file_paths;
	for (int i = 0; i < cct_infor.max_num; i++)
	{
		string file_name = to_string(i) +
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
		file_paths.push_back(file_path);
	}
	//循环绘制所有图片
	vector<cv::Mat>images = DrawcCTs();
	int k = 0;
	for (auto& file_path : file_paths)
	{
		cv::imwrite(file_path, images[k]);
		k += 1;
	}
}

void DrawCCT::DrawCCTsOnAPic()
{
	vector<cv::Mat> imgs = DrawcCTs();
	int pic_rows = int(sqrt(cct_infor.max_num)) + 1;
	int pic_cols = pic_rows;
	for (const auto& img : imgs)
	{
		if (img.empty())
		{
			cerr << "图片加载失败！" << endl;
			return;
		}
	}
	int pic_width = pic_cols * cct_infor.size;
	int pic_height = pic_rows * cct_infor.size;
	cv::Mat merged_img(pic_height, pic_width,
		imgs[0].type());
	int offsetX = 0;
	int offsetY = 0;
	for (const auto& img : imgs)
	{
		img.copyTo(merged_img(cv::Rect(offsetX, offsetY,
			img.cols, img.rows)));
		offsetX += img.cols;
		if (offsetX >= pic_width)
		{
			offsetY += img.rows;
			offsetX = 0;
		}
	}
	string merged_img_path = cct_infor.dir_path +
		string("merged_img_") + to_string(
			cct_infor.max_num) + string(".png");
	cout<<(cv::imwrite(merged_img_path, merged_img));
	return;
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
		dir_path = string("E:/source/repos/main/CCT_IMG_") + to_string(N)
			+ string("_Black/");	
		flabellate.color = cv::Scalar(0, 0, 0);
		circle_in.color = cv::Scalar(0, 0, 0);
		circle_out.color = cv::Scalar(255, 255, 255);
		break;
	case white:
		dir_path = string("E:/source/repos/main/CCT_IMG_") + to_string(N)
			+ string("_White/");
		flabellate.color = cv::Scalar(255, 255, 255);
		circle_in.color = cv::Scalar(255, 255, 255);
		circle_out.color = cv::Scalar(0, 0, 0);
		break;
	default:
		break;
	}
	//创建文件夹
	if (!fs::exists(dir_path))
	{
		if (!fs::create_directory(dir_path))
		{
			cerr << "创建文件夹：" << dir_path <<
				"失败！" << endl;
			return;
		}
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

void DetectCCTInfo::Init()
{
	for (const auto& file_path :
		fs::directory_iterator(dir_path))
	{
		if (fs::is_regular_file(file_path))
		{
			fs::path file(file_path);
			if (file.extension().string() == ".jpg" ||
				file.extension().string() == ".JPG" ||
				file.extension().string() == "png" ||
				file.extension().string() == "PNG")
			{
				img_file_paths.push_back(file_path.
					path().string());
			}
		}
	}
}
