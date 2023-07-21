// CCTLib.cpp : 定义静态库的函数。
//

#include "pch.h"
#include "framework.h"
#include "CCTLib.h"

using namespace std;


DetectCCTInfo::DetectCCTInfo(const string& file_path_,
	const CCTColor& color_,const int&N_) :file_path(file_path_), color(color_),N(N_)
{
}

DetectCCTInfo::DetectCCTInfo(const DetectCCTInfo& detect_cct_info_)
{
	this->color = detect_cct_info_.color;
	this->file_path = detect_cct_info_.file_path;
	this->N = detect_cct_info_.N;
}

DetectCCTInfo::DetectCCTInfo()
{
	this->color = black;
	this->file_path = "";
	this->N = 0;
}

cv::Mat DrawACCT(const ACCTInfo& acct_info)
{
	string dir_path = "";
	string file_path = "";
	//创建绘图画布
	cv::Mat image(acct_info.size, acct_info.size, CV_8UC3);
	Circle circlel1(cv::Point(acct_info.size / 2, acct_info.size / 2), 0.2 * acct_info.size, 1, cv::Scalar(0,0,0));//内圆周
	Circle circle_in_large(circlel1.center, 0.2 * acct_info.size, -1, circlel1.color);//里面的大圆
	switch (acct_info.color)
	{
	case black:
		file_path = string("E:/source/repos/main/CCT_Black_") + to_string(acct_info.N) + string("_/")
			+ to_string(acct_info.num) + string(".png");
		dir_path = string("E:/source/repos/main/CCT_Black_") + to_string(acct_info.N) + string("_/");
		//设置画布颜色
		image.setTo(cv::Scalar(255, 255, 255));
		circlel1.color = cv::Scalar(0, 0, 0);
		circle_in_large.color = cv::Scalar(255, 255, 255);
		break;
	case white:
		file_path = string("E:/source/repos/main/CCT_White_") + to_string(acct_info.N) + string("_/")
			+ to_string(acct_info.num) + string(".png");
		dir_path = string("E:/source/repos/main/CCT_White_") + to_string(acct_info.N) + string("_/");
		image.setTo(cv::Scalar(0, 0, 0));
		circlel1.color = cv::Scalar(255, 255, 255);
		circle_in_large.color = cv::Scalar(0, 0, 0);
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
			return cv::Mat();
		}
	}
	//绘制扇形
	Flabellate flabellate(circlel1.center, 0.4*acct_info.size, -1, circlel1.color);
	double unit_angle = 360.0 / acct_info.N;//单位角度
	int k = 0;
	vector<int> bin = IntToBin(acct_info.num,acct_info.N);
	for (int i : bin)
	{
		if (i == 1) cv::ellipse(image,
			flabellate.center,
			cv::Size(flabellate.radius,
				flabellate.radius),
			0, k * unit_angle, (k + 1) * unit_angle,
			flabellate.color,
			flabellate.thickness);
		k += 1;
	}
	//绘制内大圆
	cv::circle(image, circle_in_large.center,
		circle_in_large.radius,
		circle_in_large.color,
		circle_in_large.thickness);
	//绘制里面的圆周
	cv::circle(image, circlel1.center, circlel1.radius,
		circlel1.color, circlel1.thickness);
	//再绘制外面的圆周
	Circle circlel2(circlel1.center, 0.4 * acct_info.size, 1, circlel1.color);
	cv::circle(image, circlel2.center, circlel2.radius,
		circlel2.color, circlel2.thickness);
	//绘制内小圆
	Circle circle_in_small(circlel1.center, 0.1*acct_info.size, -1, circlel1.color);
	cv::circle(image, circle_in_small.center,
		circle_in_small.radius,
		circle_in_small.color,
		circle_in_small.thickness);
	cv::imwrite(file_path, image);
	return image;
}

vector<int> MoveBit(const vector<int>& bin)
{
	vector<int> temp = bin;
	for (size_t i = 0; i < (bin.size()-1); i++)
	{
		temp[i] = bin[i + 1];
	}
	temp[bin.size()-1] = bin[0];
	return temp;
}

vector<int> DecodeCCT(const DetectCCTInfo& detect_cct_info)
{
	//读取图片
	cv::Mat color_img = cv::imread(
		detect_cct_info.file_path);
	if (color_img.empty())
	{
		cerr << "图片：" << detect_cct_info.file_path
			<< "加载失败！" << endl;
		return vector<int>( - 1);
	}
	//图片前处理
	cv::Mat gray_img;
	cv::cvtColor(color_img, gray_img,
		cv::COLOR_BGR2GRAY);
	cv::Mat binary_img;
	cv::adaptiveThreshold(gray_img, binary_img, 1,
		cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY,
		11, 2);
	//提取轮廓
	vector<vector<cv::Point>> contours;
	cv::findContours(binary_img, contours,
		cv::RETR_TREE,
		cv::CHAIN_APPROX_NONE);
	vector<cv::RotatedRect> ellipse_rects;
	for (size_t i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() >= 20)
		{
			cv::RotatedRect ellipse_rect = cv::fitEllipse(
				contours[i]
			);
			double area = cv::contourArea(contours[i]);
			double perimeter = cv::arcLength(contours[i], true);
			double circularuty = (4.0 * CV_PI * area)
				/ (perimeter * perimeter);
			if (circularuty < 0.8) continue;
			ellipse_rects.push_back(ellipse_rect);
		}
	}
	//提取最里面的椭圆轮廓
	vector<cv::RotatedRect> ellipse_rects_c1;
	for (const auto& e : ellipse_rects)
	{
		for (const auto& p : ellipse_rects)
		{
			double x = e.center.x - p.center.x;
			double y = e.center.y - p.center.y;
			if (sqrt(pow(x, 2) + pow(y, 2)) <= 5)
			{
				ellipse_rects_c1.push_back(
					(e.size.area() > p.size.area()) ?
					e : p
				);
			}
		}
	}
	//扩充轮廓，得到外面两层椭圆轮廓
	vector<cv::RotatedRect> ellipse_rects_c2;
	vector<cv::RotatedRect> ellipse_rects_c3;
	for (const auto& e : ellipse_rects_c1)
	{
		ellipse_rects_c2.push_back(cv::RotatedRect(e.center,
			cv::Size2f(e.size.width * 2, e.size.height * 2),
			e.angle));
		ellipse_rects_c3.push_back(cv::RotatedRect(e.center,
			cv::Size2f(e.size.width * 4, e.size.height * 4),
			e.angle));
	}
	vector<cv::Mat> cct_imgs;//剪切图像
	vector<cv::RotatedRect> ellipse_rects_c3_1;//剪切后的图像上的椭圆轮廓
	for (const auto& e : ellipse_rects_c3)
	{
		//剪切图像
		cv::Rect bounding_rect = e.boundingRect();
		cv::Mat cropped_img;
		cv::getRectSubPix(color_img, bounding_rect.size(), e.center, cropped_img);
		cct_imgs.push_back(cropped_img);
		//剪切后图像上的椭圆轮廓
		cv::RotatedRect temp = e;
		temp.center.x -= bounding_rect.x;
		temp.center.y -= bounding_rect.y;
		ellipse_rects_c3_1.push_back(temp);
	}
	vector<cv::Mat> cct_imgs_after_tran;
	vector<int>result;
	for (size_t i = 0; i < ellipse_rects_c3.size(); i++)
	{
		vector<int>temp;
		cv::Mat eroded_img;
		//前处理剪切下的图像
		{
			//仿射变换
			cv::RotatedRect temp_rotate_rect = ellipse_rects_c3_1[i];
			cv::Mat img = cct_imgs[i];
			float major_axis = temp_rotate_rect.size.width / 2.0f;
			float minor_axis = temp_rotate_rect.size.height / 2.0f;
			float angle = temp_rotate_rect.angle;
			cv::Point2f center = temp_rotate_rect.center;
			float scale = major_axis / minor_axis;
			cv::Mat rotation_matrix = cv::getRotationMatrix2D(
				center,
				-angle,
				1 / scale);
			cv::Mat transformed_img;
			cv::warpAffine(img, transformed_img, rotation_matrix,
				img.size());
			//缩放
			cv::Mat large_img;
			cv::resize(transformed_img, large_img, cv::Size(200, 200), cv::INTER_LANCZOS4);
			//灰度化
			cv::Mat gray_img;
			cv::cvtColor(large_img,
				gray_img,
				cv::COLOR_BGR2GRAY);
			//腐蚀
			cv::Mat kernel = cv::getStructuringElement(
				cv::MORPH_RECT,
				cv::Size(3, 3)
			);
			cv::erode(gray_img, eroded_img, kernel);
			cct_imgs_after_tran.push_back(eroded_img);
		}
		//采样解码
		cv::Point cct_center(100, 100);
		int unite_angle = 360 / detect_cct_info.N;
		for (size_t n = 0; n < detect_cct_info.N; n++)
		{
			vector<int> re_a;
			for (size_t i = 40; i < 81; i++)
			{
				int row = round(
					cct_center.y - i * sin(
						(n * unite_angle + 0.5 * unite_angle) * PI / 180
					)
				);
				int col = round(
					cct_center.x + i * cos(
						(n * unite_angle + 0.5 * unite_angle) * PI / 180
					)
				);
				uchar* data = eroded_img.ptr<uchar>(row);
				int intensity = data[col];
				re_a.push_back(
					intensity);
			}
			int sum = 0;
			for (const auto& r : re_a)
				sum += r;
			sum /= re_a.size();
			(sum > 125) ? sum = 0 : sum = 1;
			temp.push_back(sum);
		}
		reverse(temp.begin(), temp.end());
		if (detect_cct_info.color == white)
		{
			for (size_t k = 0; k < temp.size(); k++)
			{
				(temp[k] == 1) ? temp[k] = 0 : temp[k] = 1;
			}
		}
		//绘制
		int a_result = BinToInt(temp);
		if (a_result == 0)
			continue;
		cv::ellipse(color_img, ellipse_rects_c1[i], 
			cv::Scalar(0, 255, 0), 1);
		cv::ellipse(color_img, ellipse_rects_c2[i],
			cv::Scalar(0, 255, 0), 1);
		cv::ellipse(color_img, ellipse_rects_c3[i],
			cv::Scalar(0, 255, 0), 1);
		Text text(to_string(a_result), ellipse_rects_c1[i].center);
		cv::putText(color_img, text.text, text.position, text.fontFace, text.fontScale, text.color, text.thickness, text.lineType);
		result.push_back(BinToInt(temp));
	}
	cv::imwrite(detect_cct_info.file_path, color_img);
	return result;
}

int BinToInt(const vector<int>& bin)
{
	vector<int> array_ = bin;
	int min_value = 1000000;
	int temp = 0;
	int N = array_.size();
	for (size_t i = 0; i < N; i++)
	{
		temp = 0;
		for (size_t j = 0; j < N; j++)
		{
			if (array_[j]==1)
			{
				temp += int(pow(2, j));
			}
		}
		if (temp<min_value)
		{
			min_value = temp;
		}
		array_ = MoveBit(array_);
	}
	for (size_t i = 0; i < N; i++)
	{
		temp = 0;
		for (size_t j = 0; j < N; j++)
		{
			if (array_[j]==1)
			{
				temp += int(pow(2, j));
			}
		}
		if (temp==min_value)
		{
			break;
		}
		array_ = MoveBit(array_);
	}
	return min_value;
}

Text::Text(const string& text_, 
	const cv::Point& position_):
	text(text_),position(position_)
{
}

Text::Text(const Text& text_)
{
	this->text = text_.text;
	this->position = text_.position;
}

Text::Text()
{
	this->position = cv::Point(0, 0);
	this->text = "";
}

ACCTInfo::ACCTInfo(const int& num_, const int& N_, const CCTColor& color_, const int& size_):
	num(num_),N(N_),color(color_),size(size_)
{
}

ACCTInfo::ACCTInfo(const ACCTInfo& acct_info_)
{
	this->color = acct_info_.color;
	this->N = acct_info_.N;
	this->num = acct_info_.num;
	this->size = acct_info_.size;
}

ACCTInfo::ACCTInfo()
{
	this->color = black;
	this->N = 0;
	this->num = 0;
	this->size = 0;
}

CCTInfo::CCTInfo(const int& N_, const CCTColor& color_, const int& size_):
	N(N_),color(color_),size(size_)
{
}

CCTInfo::CCTInfo(const CCTInfo& cct_info_)
{
	this->color = cct_info_.color;
	this->N = cct_info_.N;
	this->size = cct_info_.size;
}

CCTInfo::CCTInfo()
{
	this->color = black;
	this->N = 0;
	this->size = 0;
}

Circle::Circle(const cv::Point& center_, const int& radius_, const int& thickness_, const cv::Scalar& color_):
	center(center_),radius(radius_),thickness(thickness_),color(color_)
{
}

vector<int> IntToBin(const int& num_,const int&N)
{
	vector<int> bin;
	int num = num_;
	for (size_t i = 0; i < N; i++)
	{
		if (num > 0)
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

Flabellate::Flabellate(const cv::Point& center_, const int& radius_, const int& thickness_, const cv::Scalar color_):
	center(center_),radius(radius_),thickness(thickness_),color(color_)
{
}

vector<cv::Mat> DrawCCTs(const CCTInfo& cct_info)
{
	//计算该位数可以表示的最大的数字
	vector<int> temp;
	for (size_t i = 0; i < cct_info.N; i++)
		temp.push_back(1);
	int max = BinToInt(temp);
	//开始绘制
	vector<cv::Mat> ccts;
	for (size_t i = 0; i < max; i++)
	{
		ACCTInfo acct_info(i, cct_info.N, cct_info.color, cct_info.size);
		ccts.push_back(DrawACCT(acct_info));
	}
	return ccts;
}

cv::Mat DrawCCTsOnAPic(const CCTInfo& cct_info)
{
	vector<cv::Mat> ccts = DrawCCTs(cct_info);
	int pic_rows = int(sqrt(ccts.size())) + 1;
	int pic_cols = pic_rows;
	int pic_width = pic_cols * cct_info.size;
	int pic_height = pic_rows * cct_info.size;
	cv::Mat merged_img(pic_height, pic_width,
		ccts[0].type());
	int offsetX = 0;
	int offsetY = 0;
	for (const auto& img : ccts)
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
	string file_path = "";
	string dir_path = "";
	switch (cct_info.color)
	{
	case black:
		file_path = string("E:/source/repos/main/CCT_Black_") + to_string(cct_info.N) + string("_MeragedImg / ") +
			to_string(ccts.size()) + string(".png");
		dir_path = string("E:/source/repos/main/CCT_Black_") + to_string(cct_info.N) + string("_MeragedImg / ");
		break;
	case white:
		file_path = string("E:/source/repos/main/CCT_White_") + to_string(cct_info.N) + string("_MeragedImg / ") +
			to_string(ccts.size()) + string(".png");
		dir_path = string("E:/source/repos/main/CCT_White_") + to_string(cct_info.N) + string("_MeragedImg / ");
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
			return cv::Mat();
		}
	}
	cv::imwrite(file_path, merged_img);
	return merged_img;
}
