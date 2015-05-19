/*
//商品信息
*/

#ifndef __FXPlugin_ProductInfoHelper_H__
#define __FXPlugin_ProductInfoHelper_H__


#include "cocos2d.h"

#include <string>
#include <vector>
#include <map>
using namespace cocos2d;
using std::string;
using std::vector;
using std::map;


//游戏内产品id   (productID.txt中对应每个渠道的productID)
enum ProductID
{
	ProductID_None 					= 0,
	ProductID_SuperTool_One 		= 1,
	ProductID_SuperTool_Ten 		= 2,
	ProductID_OfficialSolution,
};


struct ProductInfo
{
	int ID;
	string name;
	string describe;
	int price;  //部分sdk不支持浮点，所以必须全部为整型（分，美分）
	map<string,string> channelProduct;  // channelID : productID
};

class ProductInfoHelper
{
private:
	static ProductInfoHelper* _productInfoHelper;
	ProductInfoHelper();
	bool initData();
public:
	static ProductInfoHelper* getHelper();

	string getChannel(){return m_ChannelId;}  //获取当前版本是什么渠道
	const ProductInfo* getOneProductInfo(ProductID productID);
	
private:
	vector<ProductInfo> m_productList;
	string m_ChannelId;  //当前版本的channelid
};

#endif /* define(__FXPlugin_ProductInfoHelper_H__) */