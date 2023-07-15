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
	int num;//���������
	vector<int> bin;//����Ķ�����vector
public:
	CCTInfo(const int&N_,const CCTColor&color_,
		const double&size_,const int&num);
	CCTInfo(const int& N_, const CCTColor& color_,
		const double& size_, const vector<int>&bin);
	CCTInfo();
	void Init();
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
	//����CCTͼ��
	void Draw();
private:
	CCTInfo cct_infor;
};

