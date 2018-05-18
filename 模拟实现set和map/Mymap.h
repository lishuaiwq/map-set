#pragma once
#include"RBTree.h"
template<class K,class V>
struct MapKeyOfvalue
{
	const K& operator()(const pair<K, V>& kv)
	{
		return kv.first;
	}
};
template<class K, class V, class MapKeyOfvalue = MapKeyOfvalue<K,V> >
class Map
{
protected:
	RBTree</*K,*/ pair<K, V>,MapKeyOfvalue> _t;//定义红黑树的对象
public:
	typedef typename RBTree</*K,*/ pair<K, V>, MapKeyOfvalue>::Iterator Iterator;
	pair<Iterator, bool> Insert(const pair<K,V>& kv)
	{
		return _t.Insert(kv);
	}
	V& operator[](const K& key)
	{
		pair<Iterator, bool> ret = Insert(make_pair(key, V()));
		return (ret.first)->second;
	}
	Iterator Find(const K& key)
	{
		return _t.Find(key);
	}
	Iterator Begin()
	{
		return _t.Begin();
	}
	Iterator end()
	{
		return _t.End();
	}
};

void TestMyMap()
{
	Map<string, string> dict;
	dict["hehe"] = "haha";
	dict.Insert(make_pair("left", "左边"));

	Map<string, string>::Iterator it = dict.Begin();
	while (it != dict.end())
	{
		cout << (*it).first <<" "<<(*it).second<<endl;
		++it;
	}
}

