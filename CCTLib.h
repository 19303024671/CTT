#pragma once

#include<iostream>
#include<string>
#include<vector>

using namespace std;

//环形码的颜色
enum CCTColor
{
	black = 1,
	white = 0	
};

//绘制环形码所需的信息
struct CCTInfo
{
	string dir_path;//文件夹路径
	string file_path;//文件名
	int N;//等分数
	CCTColor color;//环形码颜色
	double size;//图片尺寸
	int num;//编码的整数
	vector<int> bin;//编码的二进制vector
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
	//将整数转成二进制vector
	vector<int> IntToBin( int num,const int& N);
	//将二进制的vector转成整数
	int BinToInt(const vector<int>& bin, int N);
	//绘制CCT图像
	void Draw();
private:
	CCTInfo cct_infor;
};

