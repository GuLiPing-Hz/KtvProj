/*
	@Author : GLP

	用于做兼容版本，兼容以后使用xml进行聊天
*/

#ifndef CHATCOMPATIBILITY__H__
#define CHATCOMPATIBILITY__H__

class CChatCompatibility
{
public:
	/*
	@param out[out] : 输出字符串
	@param len[in out] : out的长度，输出实际写入长度
	@param in[in] : 输入字符串

	@return : 
		返回0，不存在问题，正常转换;  如果转换过程中发现版本不对，会把信息写入输入字符串 "聊天协议版本异常，请更新"
		返回-1，不需要转换成xml
		返回-2，UTF8转换失败

		下面两种错误通过代码的编写可以避免
		返回-3，输出字符串大小不够
		返回-4，参数错误
	*/
	static int ConvertXml2Normal(char* out,int& len,const char* in);
};

#endif//CHATCOMPATIBILITY__H__
