#pragma once

#include<iostream>
#include<string>
#include<vector>

using namespace std;

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
	string file_path;//�ļ���
	int N;//�ȷ���
	CCTColor color;//��������ɫ
	double size;//ͼƬ�ߴ�
};

class DrawCCT
{
public:
	DrawCCT(const CCTInfo& cct_info);
	~DrawCCT();
	//������ת�ɶ����Ƶ�vector
	vector<int> IntTuBin( int num,const int& N);
private:
	CCTInfo cct_infor;
};

