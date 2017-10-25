package com.jni.netutil.out;

import com.jni.netutil.ResultData_HBDetail;

public class HongBaoDetailInfoToUi {
	public int idx;//红包发送idx
	public byte hbtype;//红包类型 0-个人  1房间手气 2-房间均分  3-大厅手气 4-大厅均分
	public String remark;//红包备注
	public int totalgold;//总金额
	public int totalnum;//红包总个数
	public int sendtime;//红包发送时间
	public ResultData_HBDetail[] list;//已经领取的红包列表
	
	public String name = "匿名";
	public String photo = "";
}
