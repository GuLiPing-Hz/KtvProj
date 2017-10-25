﻿#ifndef DATABLOCK__H__
#define DATABLOCK__H__

/*
	注释添加以及修改于 2014-4-2 

	封装一个对数据存储处理的类 DataBlock
	每个函数已经给出简单的释义。
*/
namespace NetworkUtil
{

#define	DEFAULT_BLOCK_SIZE  1024
#define	MAX_BLOCK_SIZE  1048576/*1024*1024*/

	class DataBlock
	{
	public:
		DataBlock(unsigned int size = DEFAULT_BLOCK_SIZE);
		virtual ~DataBlock() { if(m_buf)delete m_buf;}
		//加到buffer的末尾
		/*返回当前添加进去的字节数*/
		int append(const char *buf,unsigned int buflen) {return copy(m_pos,buf,buflen);}
		//拷贝数据到指定位置
		/*返回当前拷贝进去的字节数*/
		int copy(unsigned int pos,const char *buf,unsigned int buflen);
		//获取整个buffer
		char* getBuf() const {return m_buf;}
		//获取当前位置
		unsigned int getPos() const {return m_pos;}
		//清空
		void initPos() {m_pos = 0;}
		//获取大小
		unsigned int getSize()const{return m_size;}
	private:
		char *m_buf;
		unsigned int m_pos;
		unsigned int m_size;
	};
}
#endif//DATABLOCK__H__
