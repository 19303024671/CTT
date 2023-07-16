#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<filesystem>
#include<opencv2/opencv.hpp>

using namespace std;
namespace fs = std::filesystem;

//���������ɫ
enum CCTColor
{
	black = 1,
	white = 0	
};
//ʶ�������������Ϣ
struct DetectCCTInfo
{
	string dir_path;//���������ڵ��ļ���·��
	vector<string> img_file_paths;//·��������ͼƬ�ļ�·��
	CCTColor color;//���������ɫ
public:
	void Init();
};
//����
struct Flabellate
{
	cv::Point center;//Բ��
	int radius;//�뾶
	double start_angle;//��ʼ�Ƕ�
	double end_angle;//��ֹ�Ƕ�
	int thickness = -1;//��ȣ�-1��ʾ���
	cv::Scalar color;//��ɫ(BGR˳��)
};
//Բ
struct Circle
{
	cv::Point center;//Բ��
	int radius;//�뾶
	int thickness = -1;//���
	cv::Scalar color;//��ɫ
};
//���ƻ������������Ϣ
struct CCTInfo
{
	string dir_path;//�ļ���·��
	int N;//�ȷ���
	CCTColor color;//��������ɫ
	int size;//ͼƬ�ߴ�
	int num;//���������
	int max_num;//Nλ�����Ʊ�ʾ���������
	vector<int> bin;//����Ķ�����vector
	Flabellate flabellate;//������Ϣ
	Circle circle_out;//��Բ�Σ�����Ϣ
	Circle circle_in;//��Բ�Σ�С����Ϣ

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
	//������ת�ɶ�����vector
	vector<int> IntToBin( int num,const int& N);
	//�������Ƶ�vectorת������
	int BinToInt(const vector<int>& bin, int N);
	//����һ��CCTͼ��
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



