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

//���������ɫ
enum CCTColor
{
	black = 1,
	white = 0	
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

vector<int> DecodeCCT(const DetectCCTInfo& detect_cct_info);

int BinToInt(const vector<int>& bin);

cv::Mat DrawACCT(const ACCTInfo& acct_info);

vector<cv::Mat> DrawCCTs(const CCTInfo& cct_info);

vector<int> IntToBin(const int& num_,const int&N);

cv::Mat DrawCCTsOnAPic(const CCTInfo& cct_info);

vector<int> MoveBit(const vector<int>& bin);

bool IsIn(const cv::RotatedRect& temp_r,const vector<cv::RotatedRect> & ellipse_rects_c1);