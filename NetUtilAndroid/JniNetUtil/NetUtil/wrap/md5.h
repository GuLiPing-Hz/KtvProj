#ifndef MD5_H
#define MD5_H

#include <string>
#include <fstream>

using std::string;
using std::ifstream;

/* MD5 declaration. */
class MD5 {
public:
	//返回大写的MD5值
	static string FileDigest(const string& file);

	MD5();
	MD5(const void* input, size_t length);
	MD5(const string& str);
	MD5(ifstream& in);
	void update(const void* input, size_t length);
	void update(const string& str);
	void update(ifstream& in);
	const unsigned char* digest();
	string toString();
	void reset();

private:
	void update(const unsigned char* input, size_t length);
	void final();
	void transform(const unsigned char block[64]);
	void encode(const unsigned int* input, unsigned char* output, size_t length);
	void decode(const unsigned char* input, unsigned int* output, size_t length);
	string bytesToHexString(const unsigned char* input, size_t length);

	/* class uncopyable */
	MD5(const MD5&);
	MD5& operator=(const MD5&);

private:
	unsigned int _state[4];	/* state (ABCD) */
	unsigned int _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
	unsigned char _buffer[64];	/* input buffer */
	unsigned char _digest[16];	/* message digest */
	bool _finished;		/* calculate finished ? */

	static const unsigned char PADDING[64];	/* padding for calculate */
	static const char HEX[16];
	enum { BUFFER_SIZE = 1024 };
};

#endif /*MD5_H*/
