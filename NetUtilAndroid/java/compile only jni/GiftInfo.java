package com.jni.netutil;

public class GiftInfo {
    public int  id;					    //��Ʒid
    public int order;					//����
    public String name;             //��Ʒ����
    public String image_name;  //ͼƬ�ļ���(����׺)
    public String image_url;        //ͼƬURL,NetUtilManager������ת���ɱ��ؾ�����ļ�·��
    public int  cash;				     //���
    @Deprecated
	public int  coin;				     //����
	public String unit_name;		  //��λ
	public int  effectId;               //������ЧID 0��Ϊ��û����Ч
	public int gift_type;				  //�������,����1����ͨ2���ݳ�3
};
