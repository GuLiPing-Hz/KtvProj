#ifndef SEQMAP__H__
#define SEQMAP__H__
/*
	注释添加以及修改于 2014-4-3 
	mender: glp

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
		virtual void Put(int id,T t) {m_clientmap.insert(std::make_pair(id,t));}
		virtual void Cover(int id,T t) {m_clientmap[id] = t;}
		virtual T* Get(int id)
		{
			typename _MapT::iterator it;
			it = m_clientmap.find(id);
			if(it == m_clientmap.end())
				return NULL;
			return &(it->second);
		}
		inline virtual void Del (int id) {m_clientmap.erase(id);}
		inline virtual unsigned int Size() {return m_clientmap.size();}
		inline virtual void Clear() { m_clientmap.clear();}
		virtual _MapT* GetMap() {return &m_clientmap;}
	private:
		_MapT m_clientmap;
	};

	template <class T,class Map = std::map<int,T> >
	class SeqMap_ThreadSafe : public SeqMap<T,Map>
	{
	public:
		SeqMap_ThreadSafe():m_pCSW(NULL)
        {
            m_pCSW = CriticalSectionWrapper::CreateCriticalSection();
            assert(m_pCSW!=NULL);
        }
		virtual ~SeqMap_ThreadSafe(){if(m_pCSW)delete m_pCSW;}

		virtual void Put(int id,T t){
            //加入临界区的保护，使之具有线程安全。
            CriticalSectionScoped g(m_pCSW);
            SeqMap<T,Map>::Put(id,t);
        }
		virtual void Cover(int id,T t){
            CriticalSectionScoped g(m_pCSW);
            SeqMap<T,Map>::Cover(id,t);
        }
		virtual T* Get(int id)	{
            CriticalSectionScoped g(m_pCSW);
            return SeqMap<T,Map>::Get(id);
        }
		inline virtual void Del(int id)	{
            CriticalSectionScoped g(m_pCSW);
            SeqMap<T,Map>::Del(id);
        }
		inline virtual unsigned int Size(){
            CriticalSectionScoped g(m_pCSW);
            return SeqMap<T,Map>::Size();
        }
		inline virtual void Clear(){
            CriticalSectionScoped g(m_pCSW);
            SeqMap<T,Map>::Clear();
        }
	private:
		CriticalSectionWrapper* m_pCSW;
	};
}

#endif//SEQMAP__H__
