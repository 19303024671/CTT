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

//���ƻ������������Ϣ
struct CCTInfo
{
	string dir_path;//�ļ���·��
	int N;//�ȷ���
	CCTColor color;//��������ɫ
	double size;//ͼƬ�ߴ�
	int num;//���������
	vector<int> bin;//����Ķ�����vector
public:
	void Init();
	CCTInfo(const int&N_,const CCTColor&color_,const double&size_,const int&num);
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
	void DrawACCT();
private:
	CCTInfo cct_infor;
};

