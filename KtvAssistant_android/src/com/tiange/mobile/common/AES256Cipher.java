package com.tiange.mobile.common;

import java.io.UnsupportedEncodingException;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.spec.AlgorithmParameterSpec;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import assistant.task.PDataCache;

public class AES256Cipher {
	
	public static byte[] ivBytes = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	public static String AES_Encode(String str, String key) {
		
		byte[] textBytes;
		try {
			textBytes = str.getBytes("UTF-8");
			AlgorithmParameterSpec ivSpec = new IvParameterSpec(ivBytes);
		     SecretKeySpec newKey = new SecretKeySpec(key.getBytes("UTF-8"), "AES");
		     Cipher cipher = null;
		     cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
		     cipher.init(Cipher.ENCRYPT_MODE, newKey, ivSpec);
		
		     //android base64 after 2.1
//		     return Base64.encodeToString(cipher.doFinal(textBytes), 0);
		     byte[] encodeBytes = cipher.doFinal(textBytes);
		     if(null != encodeBytes){
//				return Base64Encoder.encode(encodeBytes);
		    	 return Base64.encodeToString(encodeBytes, 0);
		     }
		
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NoSuchPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InvalidKeyException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InvalidAlgorithmParameterException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalBlockSizeException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (BadPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("str: " + str);
			PDataCache.getInstance().setString("encodeerrorlog", str);
		}
		
		return null;
	}

	public static String AES_Decode(String str, String key) {
		
		try {
			if(null == str){
				return null;
			}
			byte[] textBytes =Base64.decode(str,0);
			AlgorithmParameterSpec ivSpec = new IvParameterSpec(ivBytes);
			SecretKeySpec newKey = new SecretKeySpec(key.getBytes("UTF-8"), "AES");
			Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
			cipher.init(Cipher.DECRYPT_MODE, newKey, ivSpec);
			return new String(cipher.doFinal(textBytes), "UTF-8");
			
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (NoSuchPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InvalidKeyException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (InvalidAlgorithmParameterException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalBlockSizeException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (BadPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("str: " + str);
			PDataCache.getInstance().setString("decodeerrorlog", str);
		}
		
		return null;
	}
}