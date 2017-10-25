//
//  rc4.cpp
//  rc4
//
//  Created by ZYL-1 on 14-9-22.
//  Copyright (c) 2014Äê zyl. All rights reserved.
//

#include "rc4.h"
#include <string.h>
#include <fstream>

rc4_Encode::rc4_Encode(unsigned char *key_data_ptr,int nLen)
{
	prepare_key(key_data_ptr, nLen);
}

void rc4_Encode::prepare_key(unsigned char *key_data_ptr, int key_data_len)
{
	if (0 == key_data_len)
	{
		return;
	}
	unsigned char index1;
	unsigned char index2;
	unsigned char* state;
	short counter;
    
	state = &key.state[0];
    
	for(counter = 0; counter < KEYT_LEN; counter++)
		state[counter] = (unsigned char)counter;
    
	key.x = 0;
	key.y = 0;
	index1 = 0;
	index2 = 0;
    
	for(counter = 0; counter < KEYT_LEN; counter++)
	{
		index2 = (key_data_ptr[index1] + state[counter] + index2) % KEYT_LEN;
		swap_byte(&state[counter], &state[index2]);
		index1 = (index1 + 1) % key_data_len;
	}
}

void rc4_Encode::swap_byte(unsigned char *a, unsigned char *b)
{
	unsigned char x;
	x=*a;*a=*b;*b=x;
}

void rc4_Encode::rc4_decrypt(unsigned char *buffer_ptr, int buffer_len)
{
	unsigned char x;
	unsigned char y;
	unsigned char state[KEYT_LEN] = {0};
	unsigned char xorIndex;
	short counter;
    
	x = key.x;
	y = key.y;
    
	memcpy((void *)state, (void *)key.state, sizeof(key.state));
    
	for(counter = 0; counter < buffer_len; counter++)
	{
		x = (x + 1) % 256;
		y = (state[x] + y) % 256;
		swap_byte(&state[x], &state[y]);
		xorIndex = (state[x] + state[y]) % 256;
		buffer_ptr[counter] ^= state[xorIndex];
		buffer_ptr[counter] += 0x16;
	}
}

void rc4_Encode::rc4_encrypt(unsigned char *buffer_ptr, int buffer_len)
{
	unsigned char x;
	unsigned char y;
	unsigned char state[KEYT_LEN] = {0};
	unsigned char xorIndex;
	short counter;
    
	x = key.x;
	y = key.y;
    
	memcpy((void *)state, (void *)key.state, sizeof(key.state));
    
	for(counter = 0; counter < buffer_len; counter++)
	{
		x = (x + 1) % 256;
		y = (state[x] + y) % 256;
		swap_byte(&state[x], &state[y]);
		xorIndex = (state[x] + state[y]) % 256;
		buffer_ptr[counter] -= 0x16;
		buffer_ptr[counter] ^= state[xorIndex];
	}
}
