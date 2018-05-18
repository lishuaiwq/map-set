#pragma once

#include "RBTree.h"


//template<class K>
template<typename K>
class Set
{
public:
	struct SetKeyOfValue
	{
		const K& operator()(const K& key)
		{
			return key;
		}
	};
	typedef typename RBTree< K, SetKeyOfValue>::Iterator Iterator;
	pair<Iterator, bool> Insert(const K& key)
	{
		return _t.Insert(key);
	}

	Iterator Find(const K& key)
	{
		return _t.Find(key);
	}

	Iterator Begin()
	{
		return _t.Begin();
	}

	Iterator End()
	{
		return _t.End();
	}

protected:

	RBTree<K, SetKeyOfValue> _t;
};

void TestMySet()
{
	Set<string> s;
	s.Insert("sort");
	s.Insert("insert");
	s.Insert("set");
	s.Insert("map");
	s.Insert("iterator");
	s.Insert("value");
	s.Insert("value");
	Set<string>::Iterator it = s.Begin();
	while (it != s.End())
	{
		cout << *it << "  ";
		++it;
	}
	cout << endl;
}
