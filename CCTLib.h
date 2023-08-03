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

//���������ɫ
enum CCTColor
{
	black = 1,
	white = 0	
};
//��ķ��ؽ��
struct Result
{
	int index;
	cv::Point pos;
public:
	Result(const int& index_, const cv::Point& pos_);
	Result(const Result& re_);
	Result();
};
//�ı�
struct Text
{
	string text;//�ı�
	cv::Point position; // �ı�λ��
	int fontFace = cv::FONT_HERSHEY_SIMPLEX; // ��������
	double fontScale = 1.5; // �����С
	cv::Scalar color = cv::Scalar(0,255,0); // �ı���ɫ (BGR��ʽ)
	int thickness = 2; // �ı��߿�
	int lineType = cv::LINE_AA; // �ı�����
public:
	Text(const string& text_, const cv::Point& position_);
	Text(const Text& text_);
	Text();
};
//ʶ�������������Ϣ
struct DetectCCTInfo
{
	string file_path;//������·��
	CCTColor color;//���������ɫ
	int N;//����λ��
public:
	DetectCCTInfo(const string& file_path_, const CCTColor& color_ , const int&N_);
	DetectCCTInfo(const DetectCCTInfo&detect_cct_info_);
	DetectCCTInfo();
};
//����
struct Flabellate
{
	cv::Point center;//Բ��
	int radius;//�뾶
	int thickness = -1;//��ȣ�-1��ʾ���
	cv::Scalar color;//��ɫ(BGR˳��)
public:
	Flabellate(const cv::Point& center_, const int& radius_, const int& thickness_, const cv::Scalar color_);
};
//Բ
struct Circle
{
	cv::Point center;//Բ��
	int radius;//�뾶
	int thickness = -1;//���
	cv::Scalar color;//��ɫ
public:
	Circle(const cv::Point& center_, const int& radius_, const int& thickness_, const cv::Scalar& color_);
};
//����һ���������������Ϣ
struct ACCTInfo
{
	int num;//���������
	int N;//����λ��
	CCTColor color;//�������ɫ
	int size;//ͼƬ�ߴ�
public:
	ACCTInfo(const int& num_, const int& N_, const CCTColor& color_, const int& size_);
	ACCTInfo(const ACCTInfo& acct_info_);
	ACCTInfo();
};
struct DrawCCTOnP
{
	int num;//���������
	int N;//����λ��
	CCTColor color;//�������ɫ
	cv::Mat img;//�����Ƶ�ͼƬ
	cv::Point pos;//���Ƶ�λ��
public:
	DrawCCTOnP(const int& num_, const int& N_,
		const CCTColor& color_, const cv::Mat& img_, const cv::Point& pos_);
	DrawCCTOnP(const DrawCCTOnP& info_);
	DrawCCTOnP();
};
//����һ���������������Ϣ
struct CCTInfo
{
	int N;//�ȷ���
	CCTColor color;//��������ɫ
	int size;//ͼƬ�ߴ�
public:
	CCTInfo(const int& N_, const CCTColor& color_, const int& size_);
	CCTInfo(const CCTInfo& cct_info_);
	CCTInfo();
};
//ʶ������ܺ���
vector<Result> DecodeCCT(const DetectCCTInfo& detect_cct_info);
//������תʮ����
int BinToInt(const vector<int>& bin);
//��һ��CCT
cv::Mat DrawACCT(const ACCTInfo& acct_info);
//��һ��CCT
vector<cv::Mat> DrawCCTs(const CCTInfo& cct_info);
//ʮ����ת������
vector<int> IntToBin(const int& num_,const int&N);
//��һ��ͼ�ϻ�һ��CCT
cv::Mat DrawCCTsOnAPic(const CCTInfo& cct_info);
//��������λ�����ڶ�����ת��Сʮ������
vector<int> MoveBit(const vector<int>& bin);
//�ж��ǲ����Ѿ���ͬ�ĵ���Բ����Բ�������ˣ����ڴ���������ȡ��ͬ����С����Բ��������
bool IsIn(const cv::RotatedRect& temp_r,const vector<cv::RotatedRect> & ellipse_rects_c1);
//�������任���󣬳���Python����
cv::Mat GetTransMatrix(const cv::Mat& src, const cv::Mat& dst);
//��ȡͼ��
cv::Mat ReadImg(const string& file_path);
//ͼƬǰ���������ҶȻ����ֵ��
cv::Mat TranImgPre(const cv::Mat& color_img);
//��ȡ���к��ʵ�������ellipse_rects
vector<cv::RotatedRect> GetAllEs(const cv::Mat& binary_img);
//��ȡ�������Բ������ellipse_rects_c1
vector<cv::RotatedRect> Get1Es(const vector<cv::RotatedRect>& ellipse_rects);
//��ȡ2��3
vector<vector<cv::RotatedRect>> Get2_3Es(const vector<cv::RotatedRect>& ellipse_rects_c1);
//����ͼ��
vector<cv::Mat> CutImg(const vector<cv::RotatedRect>& e3, const cv::Mat& color_img);
//�õ���ͼ������н��
vector<Result> GetResult(const int& N, const CCTColor& color, 
	const string& file_path,
	const cv::Mat& color_img, 
	const vector<cv::RotatedRect>& ellipse_rects_c1,
	const vector<cv::RotatedRect>& ellipse_rects_c2, 
	const vector<cv::RotatedRect>& ellipse_rects_c3, 
	const vector<cv::Mat>&cct_imgs);
//����任+������е�ͼ��
cv::Mat TranImg(const cv::Mat& img,
	const cv::RotatedRect& box1, 
	const cv::RotatedRect& box3);
//����
int Decode(const int&N,const CCTColor&color,const cv::Mat &erode_img,
	const cv::RotatedRect& box2,
	const cv::RotatedRect& box3);
//����
void DrawResult(const int& a_result, const cv::Mat& color_img,
	const cv::RotatedRect& box1,
	const cv::RotatedRect& box2,
	const cv::RotatedRect& box3);
int Decode2(const int& N, const CCTColor& color, const cv::Mat& erode_img);

void DrawIpadImg(const int& width, const int& height);

cv::Mat DrawACCTOnPic(const DrawCCTOnP& info);

bool IsOk(const int& re, const vector<int>& tem);

vector<int> GetTemp(const int& N, const int& a, const int& b, const cv::Mat& img, const cv::Point& center);