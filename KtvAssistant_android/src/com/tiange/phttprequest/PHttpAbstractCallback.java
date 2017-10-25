package com.tiange.phttprequest;

import java.io.InputStream;

import org.apache.http.HttpResponse;

public interface PHttpAbstractCallback {

	public void requestFialed(HttpResponse httpResponse, int statusCode);
	public void requestReceiveBytes(InputStream inputStream);
	
}
