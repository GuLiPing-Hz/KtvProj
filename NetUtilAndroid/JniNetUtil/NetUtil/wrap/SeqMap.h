#ifndef SEQMAP__H__
#define SEQMAP__H__
/*
	注释添加以及修改于 2014-4-3 

	封装一个模板类map和一个线程安全的模板类map
*/
#include <map>
#include <assert.h>
#include "NUtility.h"

class CriticalSectionWrapper;
namespace NetworkUtil
{
	template <class T,class Map = std::map<int,T> >
	class SeqMap
	{
	public:
		typedef SeqMap<T,Map> _MyT;
		typedef std::map<int,T> _MapT;
	public:
		virtual ~SeqMap(){}
		void Put(int id,T t) {m_clientmap.insert(std::make_pair(id,t));}
		void Cover(int id,T t) {m_clientmap[id] = t;}
		T* Get(int id)
		{
			typename _MapT::iterator it;
			it = m_clientmap.find(id);
			if(it == m_clientmap.end())
				return NULL;
			return &(it->second);
		}
		void Del (int id) {m_clientmap.erase(id);}
		unsigned int Size() {return m_clientmap.size();}
		void Clear() { m_clientmap.clear();}
		virtual _MapT* GetMap() {return &m_clientmap;}
	private:
		_MapT m_clientmap;
	};

	template <class T,class Map = std::map<int,T> >
	class SeqMap_ThreadSafe : public SeqMap<T,Map>
	{
		typedef SeqMap<T,Map> SeqMapTM;
	public:
		SeqMap_ThreadSafe()
		:m_pCSW(NULL)
        {
            m_pCSW = CriticalSectionWrapper::CreateCriticalSection();
            assert(m_pCSW!=NULL);
        }
		virtual ~SeqMap_ThreadSafe(){if(m_pCSW)delete m_pCSW;}

		void Put(int id,T t)
		{
            //加入临界区的保护，使之具有线程安全。
            CriticalSectionScoped g(m_pCSW);
            SeqMap<T,Map>::Put(id,t);
        }
		void Cover(int id,T t)
		{
            CriticalSectionScoped g(m_pCSW);
            SeqMap<T,Map>::Cover(id,t);
        }
		T* Get(int id)
		{
            CriticalSectionScoped g(m_pCSW);
            return SeqMap<T,Map>::Get(id);
        }
		void Del(int id)
		{
            CriticalSectionScoped g(m_pCSW);
            SeqMap<T,Map>::Del(id);
        }
		unsigned int Size()
		{
            CriticalSectionScoped g(m_pCSW);
            return SeqMap<T,Map>::Size();
        }
		void Clear()
		{
            CriticalSectionScoped g(m_pCSW);
            SeqMap<T,Map>::Clear();
        }
	private:
		CriticalSectionWrapper* m_pCSW;
	};

	template<class KeyT,class ValueT>
	class KVMap
	{
	private:
		typedef typename std::map<KeyT,ValueT> MapT;
		typedef typename std::map<KeyT,ValueT>::itearator Itearator;
		MapT value;
		CriticalSectionWrapper* _csw;

		bool _safe;
	public:
		KVMap(bool threadSafe);
		~KVMap();

		bool isThreadSafe(){return _safe;}
		void Put(KeyT id,ValueT t) {CriticalSectionScoped g(_csw);value.insert(std::make_pair(id,t));}
		void Cover(KeyT id,ValueT t) {CriticalSectionScoped g(_csw);value[id] = t;}
		ValueT* Get(KeyT id)
		{
			CriticalSectionScoped g(_csw);
			Itearator it = value.find(id);
			if(it == value.end())
				return NULL;
			return &(it->second);
		}
		void Del (KeyT id) {CriticalSectionScoped g(_csw);value.erase(id);}
		unsigned int Size() {CriticalSectionScoped g(_csw);return value.size();}
		void Clear() { CriticalSectionScoped g(_csw);value.clear();}
		MapT* GetMap() {CriticalSectionScoped g(_csw);return &value;}
	};
}

#endif//SEQMAP__H__
