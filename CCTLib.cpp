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
//将整数转成二进制vector
vector<int> DrawCCT::IntTuBin(int num,const int& N)
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
