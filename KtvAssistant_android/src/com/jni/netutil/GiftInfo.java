package com.jni.netutil;

public class GiftInfo {
    public int  id;					    //商品id
    public int order;					//排序
    public String name;             //商品名称
    public String image_name;  //图片文件名(含后缀)
    public String image_url;        //图片URL,NetUtilManager会把这个转换成本地具体的文件路径
    public int  cash;				     //金币
    @Deprecated
	public int  coin;				     //银币
	public String unit_name;		  //单位
	public int  effectId;               //礼物特效ID 0认为是没有特效
	public int gift_type;				  //礼物类别,幸运1，普通2，奢侈3
};
