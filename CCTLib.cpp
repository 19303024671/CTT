// CCTLib.cpp : 定义静态库的函数。
//

#include "pch.h"
#include "framework.h"
#include "CCTLib.h"

using namespace std;

DrawCCT::DrawCCT(const CCTInfo& cct_info)
{
	this->cct_infor = cct_info;
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
		num += i *pow(2,N-1);
		N -= 1;	
	}
	return num;
}

