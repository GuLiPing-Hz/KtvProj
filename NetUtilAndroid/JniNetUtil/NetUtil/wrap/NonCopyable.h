#ifndef NONCOPYABLE__H__
#define NONCOPYABLE__H__

namespace NetworkUtil
{
	//禁用拷贝构造，=复制
	class NonCopyable
	{
	protected:
		NonCopyable() {}
		~NonCopyable() {}

	private:
		NonCopyable(const NonCopyable &);
		const NonCopyable & operator=(const NonCopyable &);
	};
}
#endif//NONCOPYABLE__H__
