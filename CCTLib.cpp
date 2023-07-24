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
	Circle circlel1(cv::Point(acct_info.size / 2, acct_info.size / 2), int(0.2 * acct_info.size), 1, cv::Scalar(0,0,0));//内圆周
	Circle circle_in_large(circlel1.center, int(0.2 * acct_info.size), -1, circlel1.color);//里面的大圆
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
	Flabellate flabellate(circlel1.center, int(0.3*acct_info.size), -1, circlel1.color);
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
	//绘制内小圆
	Circle circle_in_small(circlel1.center, int(0.1*acct_info.size), -1, circlel1.color);
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

bool IsIn(const cv::RotatedRect& temp_r, const vector<cv::RotatedRect>& ellipse_rects_c1)
{
	if (ellipse_rects_c1.size() == 0) return false;
	for (const auto& p : ellipse_rects_c1)
	{
		double x = p.center.x - temp_r.center.x;
		double y = p.center.y - temp_r.center.y;
		if (sqrt(pow(x, 2) + pow(y, 2)) <= 10)
		{
			return true;
		}
	}
	return false;
}

cv::Mat GetTransMatrix(const cv::Mat& src, const cv::Mat& dst)
{
	int m = src.rows;
	cv::Mat A(2 * m, 6, CV_32F, cv::Scalar(0));
	cv::Mat b(2 * m, 1, CV_32F, cv::Scalar(0));
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (j==0)
			{
				A.at<float>(2 * i + j, 0) = src.at<float>(i, 0);
				A.at<float>(2 * i + j, 1) = src.at<float>(i, 1);
				A.at<float>(2 * i + j, 4) = 1;
				b.at<float>(2 * i + j, 0) = dst.at<float>(i, 0);
			}
			if (j==1)
			{
				A.at<float>(2 * i + j, 2) = src.at<float>(i, 0);
				A.at<float>(2 * i + j, 3) = src.at<float>(i, 1);
				A.at<float>(2 * i + j, 5) = 1;
				b.at<float>(2 * i + j, 0) = dst.at<float>(i, 1);
			}
		}
	}
	cv::Mat A_transponse = A.t();
	cv::Mat AtA = A_transponse * A;
	double det = cv::determinant(AtA);
	if (det < 0.1) return cv::Mat();
	cv::Mat AtA_inverse;
	cv::invert(AtA, AtA_inverse, cv::DECOMP_LU);
	cv::Mat x = (AtA_inverse * A_transponse) * b;
	cv::Mat M = (cv::Mat_<float>(2, 3) <<
		x.at<float>(0, 0), x.at<float>(1, 0), x.at<float>(4, 0),
		x.at<float>(2, 0), x.at<float>(3, 0), x.at<float>(5, 0)
		);
	return M;
}

cv::Mat ReadImg(const string& file_path)
{
	//读取图片
	cv::Mat color_img = cv::imread(file_path);
	if (color_img.empty())
	{
		cerr << "图片：" << file_path
			<< "加载失败！" << endl;
		return cv::Mat();
	}
	return color_img;
}

cv::Mat TranImgPre(const cv::Mat& color_img)
{
	//图片前处理
	cv::Mat gray_img;
	cv::cvtColor(color_img, gray_img,
		cv::COLOR_BGR2GRAY);
	cv::Mat binary_img;
	cv::adaptiveThreshold(gray_img, binary_img, 1,
		cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY,
		11, 2);
	return binary_img;
}

vector<cv::RotatedRect> GetAllEs(const cv::Mat& binary_img)
{
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
	return ellipse_rects;
}

vector<cv::RotatedRect> Get1Es(const vector<cv::RotatedRect>& ellipse_rects)
{
	vector<cv::RotatedRect> ellipse_rects_c1;
	for (size_t i = 0; i < ellipse_rects.size(); i++)
	{
		//先判断是不是相同圆心的，是相同圆心的只取面积最小的
		vector<cv::RotatedRect> temp;
		temp.push_back(ellipse_rects[i]);//保存这一个
		for (size_t j = i + 1; j < ellipse_rects.size(); j++)
		{
			double x = ellipse_rects[i].center.x - ellipse_rects[j].center.x;
			double y = ellipse_rects[i].center.y - ellipse_rects[j].center.y;
			if (sqrt(pow(x, 2) + pow(y, 2)) <= 10)
			{
				temp.push_back(ellipse_rects[j]);//保存相同圆心的
			}
		}
		cv::RotatedRect temp_r = temp[0];//取出现在的这一个
		if (IsIn(temp_r, ellipse_rects_c1)) continue;//判断是不是已经处理了，已经在容器里了
		for (const auto& e : temp)
		{
			if (e.size.area() > temp_r.size.area())
			{
				temp_r = e;//找到该组同心圆中面积最小的一个
			}
		}
		ellipse_rects_c1.push_back(temp_r);	//添加到容器里				
	}
	return ellipse_rects_c1;
}

vector<vector<cv::RotatedRect>> Get2_3Es(const vector<cv::RotatedRect>& ellipse_rects_c1)
{
	vector<cv::RotatedRect> ellipse_rects_c3;//最大的椭圆0.3
	vector<cv::RotatedRect> ellipse_rects_c2;//中间的椭圆0.2
	for (const auto& e : ellipse_rects_c1)
	{
		ellipse_rects_c2.push_back(cv::RotatedRect(e.center,
			cv::Size2f(e.size.width * 2, e.size.height * 2),
			e.angle));
		ellipse_rects_c3.push_back(cv::RotatedRect(e.center,
			cv::Size2f(e.size.width * 3, e.size.height * 3),
			e.angle));
	}
	vector<vector<cv::RotatedRect>> e23;
	e23.push_back(ellipse_rects_c2);
	e23.push_back(ellipse_rects_c3);
	return e23;
}

vector<cv::Mat> CutImg(const vector<cv::RotatedRect>& e3, const cv::Mat& color_img)
{
	vector<cv::Mat> cct_imgs;
	for (const auto& e : e3)
	{
		//剪切图像：将最外层椭圆包裹的图像从原图像中剪切下来
		cv::Rect bounding_rect = e.boundingRect();
		cv::Mat cropped_img;
		cv::getRectSubPix(color_img, bounding_rect.size(), e.center, cropped_img);
		cct_imgs.push_back(cropped_img);
	}
	return cct_imgs;
}

vector<int> GetResult(const int&N,const CCTColor&color,const string&file_path,const cv::Mat&color_img,const vector<cv::RotatedRect>& ellipse_rects_c1, const vector<cv::RotatedRect>& ellipse_rects_c2, const vector<cv::RotatedRect>& ellipse_rects_c3, const vector<cv::Mat>& cct_imgs)
{
	vector<int> result;
	for (size_t i = 0; i < ellipse_rects_c3.size(); i++)
	{
		//仿射变换
		cv::Mat img = cct_imgs[i];
		cv::RotatedRect box1 = ellipse_rects_c1[i];
		cv::RotatedRect box3 = ellipse_rects_c3[i];
		cv::Mat eroded_img = TranImg(img,box1,box3);//这张剪切下的图片变换后的图像
		if (eroded_img.empty()) continue;
		//解码
		int a_result = Decode(N, color, eroded_img);
		if (a_result == 0)
			continue;
		cv::RotatedRect box2 = ellipse_rects_c2[i];
		result.push_back(a_result);
		//绘制
		DrawResult(a_result, color_img, file_path, box1, box2, box3);
	}
	return result;
}

cv::Mat TranImg(const cv::Mat& img, const cv::RotatedRect& box1, const cv::RotatedRect& box3)
{
	cv::Mat eroded_img;//这张图片变换后的图像
	//仿射变换
	cv::Mat min_rect;
	cv::boxPoints(box3, min_rect);
	float a = max(box3.size.height, box3.size.width);
	float dx = box1.center.x - a / 2;
	float dy = box1.center.y - a / 2;
	cv::Mat src = (cv::Mat_<float>(5, 2) <<
		min_rect.at<float>(0, 0) - dx, min_rect.at<float>(0, 1) - dy,
		min_rect.at<float>(1, 0) - dx, min_rect.at<float>(1, 1) - dy,
		min_rect.at<float>(2, 0) - dx, min_rect.at<float>(2, 1) - dy,
		min_rect.at<float>(3, 0) - dx, min_rect.at<float>(3, 1) - dy,
		box1.center.x - dx, box1.center.y - dy
		);
	cv::Mat dst = (cv::Mat_<float>(5, 2) <<
		box1.center.x - a / 2 - dx, box1.center.y - a / 2 - dy,
		box1.center.x + a / 2 - dx, box1.center.y - a / 2 - dy,
		box1.center.x + a / 2 - dx, box1.center.y + a / 2 - dy,
		box1.center.x - a / 2 - dx, box1.center.y + a / 2 - dy,
		box1.center.x - dx, box1.center.y - dy
		);
	cv::Mat M = GetTransMatrix(src, dst);
	if (M.empty()) return cv::Mat();
	cv::Mat transformed_img;
	cv::warpAffine(img, transformed_img, M, cv::Size(int(a), int(a)));
	//缩放为200*200
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
	return eroded_img;
}

int Decode(const int& N, const CCTColor& color, const cv::Mat& erode_img)
{
	cv::Mat eroded_img = erode_img;
	//采样解码
	cv::Point cct_center = cv::Point(100, 100);
	int unite_angle = 360 / N;
	vector<int>temp;
	for (size_t n = 0; n < N; n++)
	{
		vector<int> re_a;
		for (size_t i = 70; i < 100; i++)
		{
			int row = int(round(
				cct_center.y - i * sin(
					(n * unite_angle + 0.5 * unite_angle) * PI / 180
				))
			);
			int col = int(round(
				cct_center.x + i * cos(
					(n * unite_angle + 0.5 * unite_angle) * PI / 180
				))
			);
			uchar* data = eroded_img.ptr<uchar>(row);
			int intensity = data[col];
			re_a.push_back(
				intensity);
		}
		int sum = 0;
		for (const auto& r : re_a)
			sum += r;
		sum /= int(re_a.size());
		(sum > 125) ? sum = 0 : sum = 1;
		temp.push_back(sum);
	}
	if (color == white)
	{
		for (size_t k = 0; k < temp.size(); k++)
		{
			(temp[k] == 1) ? temp[k] = 0 : temp[k] = 1;
		}
	}
	int a_result = BinToInt(temp);
	return a_result;
}

void DrawResult(const int& a_result, const cv::Mat& color_img,
	const string& file_path,
	const cv::RotatedRect& box1,
	const cv::RotatedRect& box2,
	const cv::RotatedRect& box3)
{
	cv::ellipse(color_img, box1,
			cv::Scalar(0, 255, 0), 1);
	cv::ellipse(color_img, box2,
		cv::Scalar(0, 255, 0), 1);
	cv::ellipse(color_img, box3,
		cv::Scalar(0, 255, 0), 1);
	Text text(to_string(a_result), box1.center);
	cv::putText(color_img, text.text, 
		text.position, text.fontFace,
		text.fontScale, text.color, 
		text.thickness, text.lineType);
	cv::imwrite(file_path, color_img);
}

vector<int> DecodeCCT(const DetectCCTInfo& detect_cct_info)
{
	cout << "识别开始！" << endl;
	ProgressBar bar(6);
	//读取图片
	cv::Mat color_img = ReadImg(detect_cct_info.file_path);
	bar.update();
	//图片前处理
	cv::Mat binary_img = TranImgPre(color_img);
	bar.update();
	//提取所有合适的轮廓：ellipse_rects
	vector<cv::RotatedRect> ellipse_rects = GetAllEs(binary_img);
	bar.update();
	//提取里面的椭圆轮廓：ellipse_rects_c1
	vector<cv::RotatedRect> ellipse_rects_c1 = Get1Es(ellipse_rects);
	vector<vector<cv::RotatedRect>> e23 = Get2_3Es(ellipse_rects_c1);
	vector<cv::RotatedRect> ellipse_rects_c2 = e23[0];//中间的椭圆0.2
	vector<cv::RotatedRect> ellipse_rects_c3 = e23[1];//最大的0.3
	bar.update();
	//剪切的图像
	vector<cv::Mat> cct_imgs = CutImg(ellipse_rects_c3, color_img);
	bar.update();
	//保存这张原图像上提取到的所有的结果
	vector<int>result = GetResult(detect_cct_info.N,
		detect_cct_info.color, detect_cct_info.file_path, 
		color_img, ellipse_rects_c1,
		ellipse_rects_c2, ellipse_rects_c3, cct_imgs);
	bar.update();
	cout << "\n识别成功！" << endl;
	return result;
}

int BinToInt(const vector<int>& bin)
{
	vector<int> array_ = bin;
	int min_value = 1000000;
	int temp = 0;
	int N = int(array_.size());
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
	for (int i = 0; i < max; i++)
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
