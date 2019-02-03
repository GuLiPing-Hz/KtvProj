#pragma  once

#include <vector>
#include <list>
#include <map>
#include <wchar.h>

typedef std::list<wchar_t> LISTWCHAR;
typedef std::map<wchar_t,int> MAPWCHARINT;
typedef std::list<float>										LISTSENTENCELINEVECT;
typedef std::list<int>										LISTINT;
typedef std::list<float>									LISTFLOAT;
typedef std::list<bool>									LISTBOOL;
typedef std::vector<ulong>							VECTULONG;
typedef std::vector<float>							VECTFLOAT;

typedef std::vector<LINEVERTEX>				VECTLINEVERTEX;


typedef std::vector<int>			VECTORINT;
class Image;
typedef std::vector<Image*>	VECTORIMAGE;
