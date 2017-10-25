package com.tiange.phttprequest;

public interface PHttpUploadFileCallback {

	public void requestSendBytes(long totalSize, long num);
	public void reqeustProgress(float progress);
	
}
