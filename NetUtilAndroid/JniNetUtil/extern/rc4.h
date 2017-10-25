//
//  Header.h
//  rc4
//
//  Created by ZYL-1 on 14-9-22.
//  Copyright (c) 2014Äê zyl. All rights reserved.
//


#ifndef RC4_H__
#define RC4_H__


const unsigned int KEYT_LEN	= 512;
typedef struct rc4_key
{
	unsigned char state[KEYT_LEN];
	unsigned char x;
	unsigned char y;
}rc4_key;

class rc4_Encode
{
public:
	rc4_Encode(){}
	rc4_Encode(unsigned char *key_data_ptr,int nLen);
	void rc4_decrypt(unsigned char *buffer_ptr, int buffer_len);
	void rc4_encrypt(unsigned char *buffer_ptr, int buffer_len);
    
	void prepare_key(unsigned char *key_data_ptr, int key_data_len);
	void swap_byte(unsigned char *a, unsigned char *b);
    
private:
	rc4_key key;
};

#endif//RC4_H__
