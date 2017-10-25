#include "Utf.h"
#include <assert.h>

static inline bool isUnicodeNonCharacter(unsigned int ucs4)
{
	// Unicode has a couple of "non-characters" that one can use internally,
	// but are not allowed to be used for text interchange.
	//
	// Those are the last two entries each Unicode Plane (U+FFFE, U+FFFF,
	// U+1FFFE, U+1FFFF, etc.) as well as the entries between U+FDD0 and
	// U+FDEF (inclusive)

	return (ucs4 & 0xfffe) == 0xfffe
		|| (ucs4 - 0xfdd0U) < 16;
}

unsigned short* QUtf8::convertToUnicode(const char *chars, int len)
{
	if(!chars || len==0)
		return NULL;

	bool headerdone = false;
	unsigned short replacement = QChar::ReplacementCharacter;
	int need = 0;
	int error = -1;
	unsigned int uc = 0;
	unsigned int min_uc = 0;
/*
	if (state) {
		if (state->flags & QTextCodec::IgnoreHeader)
			headerdone = true;
		if (state->flags & QTextCodec::ConvertInvalidToNull)
			replacement = QChar::Null;
		need = state->remainingChars;
		if (need) {
			uc = state->state_data[0];
			min_uc = state->state_data[1];
		}
	}*/
	if (!headerdone && len > 3
		&& (unsigned char)chars[0] == 0xef && (unsigned char)chars[1] == 0xbb && (unsigned char)chars[2] == 0xbf) {
			// starts with a byte order mark
			chars += 3;
			len -= 3;
			headerdone = true;
	}

	//QString result(need + len + 1, Qt::Uninitialized); // worst case
	int newLen = need+len+2;
	unsigned short* result = new unsigned short[newLen];
	memset(result,0,sizeof(unsigned short)*newLen);
	unsigned short *qch = (unsigned short *)result;
	unsigned char ch;
	int invalid = 0;

	for (int i = 0; i < len; ++i) {
		ch = chars[i];
		if (need) {
			if ((ch&0xc0) == 0x80) {
				uc = (uc << 6) | (ch & 0x3f);
				--need;
				if (!need) {
					// utf-8 bom composes into 0xfeff code point
					bool nonCharacter;
					if (!headerdone && uc == 0xfeff) {
						// don't do anything, just skip the BOM
					} else if (!(nonCharacter = isUnicodeNonCharacter(uc)) && uc > 0xffff && uc < 0x110000) {
						// surrogate pair
						assert((qch - (unsigned short*)result) + 2 < newLen/*result.length()*/);
						*qch++ = QChar::highSurrogate(uc);
						*qch++ = QChar::lowSurrogate(uc);
					} else if ((uc < min_uc) || (uc >= 0xd800 && uc <= 0xdfff) || nonCharacter || uc >= 0x110000) {
						// error: overlong sequence, UTF16 surrogate or non-character
						*qch++ = replacement;
						++invalid;
					} else {
						*qch++ = uc;
					}
					headerdone = true;
				}
			} else {
				// error
				i = error;
				*qch++ = replacement;
				++invalid;
				need = 0;
				headerdone = true;
			}
		} else {
			if (ch < 128) {
				*qch++ = (unsigned short)(ch);
				headerdone = true;
			} else if ((ch & 0xe0) == 0xc0) {
				uc = ch & 0x1f;
				need = 1;
				error = i;
				min_uc = 0x80;
				headerdone = true;
			} else if ((ch & 0xf0) == 0xe0) {
				uc = ch & 0x0f;
				need = 2;
				error = i;
				min_uc = 0x800;
			} else if ((ch&0xf8) == 0xf0) {
				uc = ch & 0x07;
				need = 3;
				error = i;
				min_uc = 0x10000;
				headerdone = true;
			} else {
				// error
				*qch++ = replacement;
				++invalid;
				headerdone = true;
			}
		}
	}
	if (/*!state &&*/ need > 0) {
		// unterminated UTF sequence
		for (int i = error; i < len; ++i) {
			*qch++ = replacement;
			++invalid;
		}
	}
	result[qch-(unsigned short *)result] = 0;
	//result.truncate(qch - (unsigned short *)result);//末尾截断'\0'
/*
	if (state) {
		state->invalidChars += invalid;
		state->remainingChars = need;
		if (headerdone)
			state->flags |= QTextCodec::IgnoreHeader;
		state->state_data[0] = need ? uc : 0;
		state->state_data[1] = need ? min_uc : 0;
	}*/
	return result;
}

char* QUtf8::convertFromUnicode(const unsigned short *uc, int len)
{
	if(!uc || len==0)
		return NULL;
	unsigned char replacement = '?';
	int rlen = 3*len+1;
	int surrogate_high = -1;
/*
	if (state) {
		if (state->flags & QTextCodec::ConvertInvalidToNull)
			replacement = 0;
		if (!(state->flags & QTextCodec::IgnoreHeader))
			rlen += 3;
		if (state->remainingChars)
			surrogate_high = state->state_data[0];
	}*/

	char* rstr = new char[rlen];
	memset(rstr,0,rlen);
	//QByteArray rstr;
	//rstr.resize(rlen);
	unsigned char* cursor = (unsigned char*)rstr;
	const unsigned short *ch = uc;
	int invalid = 0;
/*
	if (state && !(state->flags & QTextCodec::IgnoreHeader)) {
		*cursor++ = 0xef;
		*cursor++ = 0xbb;
		*cursor++ = 0xbf;
	}*/

	const unsigned short *end = ch + len;
	while (ch < end) {
		unsigned int u = *ch;
		if (surrogate_high >= 0) {
			if (QChar::isLowSurrogate((unsigned short)(*ch))) {
				u = QChar::surrogateToUcs4(surrogate_high, u);
				surrogate_high = -1;
			} else {
				// high surrogate without low
				*cursor = replacement;
				++ch;
				++invalid;
				surrogate_high = -1;
				continue;
			}
		} else if (/*ch->isLowSurrogate()*/QChar::isLowSurrogate((unsigned short)(*ch))) {
			// low surrogate without high
			*cursor = replacement;
			++ch;
			++invalid;
			continue;
		} else if (/*ch->isHighSurrogate()*/QChar::isHighSurrogate((unsigned short)(*ch))) {
			surrogate_high = u;
			++ch;
			continue;
		}

		if (u < 0x80) {
			*cursor++ = (unsigned char)u;
		} else {
			if (u < 0x0800) {
				*cursor++ = 0xc0 | ((unsigned char) (u >> 6));
			} else {
				// is it one of the Unicode non-characters?
				if (isUnicodeNonCharacter(u)) {
					*cursor++ = replacement;
					++ch;
					++invalid;
					continue;
				}

				if (u > 0xffff) {
					*cursor++ = 0xf0 | ((unsigned char) (u >> 18));
					*cursor++ = 0x80 | (((unsigned char) (u >> 12)) & 0x3f);
				} else {
					*cursor++ = 0xe0 | (((unsigned char) (u >> 12)) & 0x3f);
				}
				*cursor++ = 0x80 | (((unsigned char) (u >> 6)) & 0x3f);
			}
			*cursor++ = 0x80 | ((unsigned char) (u&0x3f));
		}
		++ch;
	}

	rstr[cursor-(unsigned char*)rstr] = 0;
	//rstr.resize(cursor - (const unsigned char*)rstr.constData());
/*
	if (state) {
		state->invalidChars += invalid;
		state->flags |= QTextCodec::IgnoreHeader;
		state->remainingChars = 0;
		if (surrogate_high >= 0) {
			state->remainingChars = 1;
			state->state_data[0] = surrogate_high;
		}
	}*/
	return rstr;
}


//需要自己释放 返回的指针 len = strlen(src);
unsigned short* QUtf8Codec::convertToUnicode(const char * src, int len) const
{
	if(!src || !len)
		return NULL;
	return QUtf8::convertToUnicode(src,len);
}
//需要自己释放 返回的指针
char* QUtf8Codec::convertFromUnicode(const unsigned short *src, int len) const
{
	if(!src || !len)
		return NULL;
	return QUtf8::convertFromUnicode(src,len);
}

