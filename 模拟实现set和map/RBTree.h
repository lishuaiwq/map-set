#pragma once
#include<iostream>
#include<string>
using namespace std;


enum Color
{
	Red,
	Black
};
template<class V>
struct RBTreeNode
{
	RBTreeNode< V>* _pLeft;
	RBTreeNode< V>* _pRight;
	RBTreeNode< V>* _pParent;
	V _v;
	Color _color;
	RBTreeNode( const V& value)
		:_pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
		, _v(value)
		, _color(Red){}
};
//红黑树的迭代器
template<class V, class Ref, class Ptr>//分别传V V* V&
struct _RBTreeIterator
{
	typedef _RBTreeIterator<V, Ref, Ptr> Self;
	typedef RBTreeNode<V> Node;
	Node* _node;//用来接头结点的指针，其实迭代器就是一个智能指针只不过他的功能是++和--还有一些解引用而已
	_RBTreeIterator(Node* node)
		:_node(node){}//这里只需要 浅拷贝就可以了，和迭代器的功能有关
	_RBTreeIterator()
	{}
	Ref operator*()//返回引用
	{
		return _node->_v;//是map返回pair，是set直接返回k就可以了
	}
	Ptr operator->()//返回指针,只是用模板来看更加的灵活
	{
		return &(operator*());
	}

	Self operator++()//前置++
	{
		if (_node->_pRight)
		{
			Node* subLeft = _node->_pRight;
			while (subLeft->_pLeft)
			{
				subLeft = subLeft->_pLeft;
			}
			_node = subLeft;
		}
		else
		{
			Node* cur = _node;
			Node* parent = cur->_pParent;
			while (parent&&parent->_pRight == cur)
			{
				cur = parent;
				parent = cur->_pParent;
			}
			_node = parent;
		}
		return *this;
	}
	Self operator++(int)
	{
		Self it(*this);
		++(*this);
		return it;
	}
	bool operator!=(const Self& s)
	{
		return _node != s._node;
	}
	Self operator--()
	{
		if (_node->_pLeft != NULL)
		{
			Node* subright = _node->_pLeft;
			while (subright->_pRight)
			{
				subright = subright->_pRight;
			}
			_node = subright;
		}
		else
		{
			Node* cur = _node;
			Node* parent = cur->_pParent;
			while (parent&&cur == parent->_pLeft)
			{
				cur = parent;
				parent = cur->_pParent;
			}
			_node = parent;
		}
		return *this;
	}
	Self operator--(int)
	{
		Self tmp(*this);
		--(*this);
		return tmp;
	}
};
//Myset->RBtree<K,K>
//Myset->RBtree<K,pair<K,V>>
template< class V,class KeyOfValue>
class RBTree
{
public:
	typedef RBTreeNode<V> Node;
	typedef Node* PNode;
	typedef _RBTreeIterator<V, V&, V*> Iterator;
	//typedef _RBTreeIterator< V, const V&, const V*> Iterator;//让在红黑树中可以使用迭代器
private:
	PNode _pRoot;
public:
	RBTree()
		:_pRoot(NULL){}

	Iterator Begin()
	{
		Node* left=_pRoot;
		while (left->_pLeft)
		{
			left = left->_pLeft;
		}
		return Iterator(left);
	}
	Iterator End()
	{
		return NULL;
	}
	pair<Iterator,bool> Insert(const V& value)
	{
		if (NULL == _pRoot)//根节点为黑色  
		{
			_pRoot = new Node( value);
			_pRoot->_color = Black;
			return make_pair(Iterator(_pRoot),true);//make_pair<>生成一个pair对象返回去
		}
		KeyOfValue kof;
		PNode cur = _pRoot;
		PNode pParent = NULL;
		while (cur)//找插入的位置  
		{
			if (kof(cur->_v)==kof(value))
				return make_pair(Iterator(cur),false);
			else if (kof(cur->_v)>kof(value))//这里用仿函数巧妙的 处理了 set和map中值不同的问题
			{
				pParent = cur;
				cur = cur->_pLeft;
			}
			else
			{
				pParent = cur;
				cur = cur->_pRight;
			}
		}
		PNode pNewNode = new Node(value);//新插入的结点的颜色默认是红的  
		PNode ret = pNewNode;
		//找到插入的位置以后开始插入  
		
		if (kof(pParent->_v)>kof(value))
		{
			pParent->_pLeft = pNewNode;
			pNewNode->_pParent = pParent;
		}
		else
		{
			pParent->_pRight = pNewNode;
			pNewNode->_pParent = pParent;
		}
		////当结点插入好了以后我们就要根据红黑树的性质来判断结点是否满足，从而调整结点  
		while (pParent)
		{
			if (pParent->_color == Black)//父结点是黑的,不用调整直接退出  
			{
				break;
			}
			//记录祖父结点和定义保存叔叔结点  
			PNode gparent = pParent->_pParent;
			PNode uncle = NULL;
			if (pParent == gparent->_pLeft)//在其左子节点上  
			{
				uncle = gparent->_pRight;
				if (uncle&& uncle->_color == Red)//叔叔结点存在切为红情况3,4  
				{
					gparent->_color = Red;
					uncle->_color = pParent->_color = Black;
					pNewNode = gparent;//保存gg结点  
					pParent = gparent->_pParent;
					continue;
				}
				else if (uncle == NULL || uncle->_color == Black)//叔叔不存在或者为黑  
				{
					if (pNewNode == pParent->_pLeft)//外侧插入  
					{
						_RBRolateR(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
					else//内测插入  
					{
						_RBRolateL(pParent);
						std::swap(pParent, pNewNode);//交换pParent和插入结点指针的值  
						_RBRolateR(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
				}
				break;//直接跳出循环  
			}
			else//右边的情况  
			{
				uncle = gparent->_pLeft;
				if (uncle&&uncle->_color == Red)//叔叔存在而且为红色  
				{
					gparent->_color = Red;
					uncle->_color = pParent->_color = Black;
					pNewNode = gparent;
					pParent = pNewNode->_pParent;
					continue;
				}
				else if (uncle == NULL || uncle->_color == Black)//叔叔不存在或者为黑  
				{
					if (pNewNode == pParent->_pRight)
					{
						_RBRolateL(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
					else
					{
						_RBRolateR(pParent);
						std::swap(pParent, pNewNode);
						_RBRolateL(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
				}
				break;
			}
		}
		_pRoot->_color = Black;
		return make_pair(Iterator(ret), true);
	}
	void InOrder()
	{
		_InOrder(_pRoot);
	}
	bool IsRBTree()
	{
		if (_pRoot == NULL)//根节点 为空是红黑树  
			return true;
		if (_pRoot->_color == Red)//根节点为红色肯定不是红黑树  
			return false;
		int count = 0;//统计黑色结点的数目  
		PNode cur = _pRoot;
		while (cur)//根出一条参考路径的黑色结点的数目  
		{
			if (cur->_color == Black)
				++count;
			cur = cur->_pLeft;
		}
		int k = 0;
		return _IsRBTree(_pRoot, count, k);
	}
	Iterator Find(const V& key)
	{
		KeyOfValue kov;
		PNode cur = _pRoot;
		while (cur)
		{
			if (kov(cur->_v > key))
			{
				cur = cur->_pLeft;
			}
			else if (kov(cur->_v) < key)
			{
				cur = cur->_pRight;
			}
			else
			{
				//return cur;
				return Iterator(cur);
			}
		}
	}
private:
	bool _IsRBTree(PNode pRoot, int& count, int k)//这里的K不能传引用  
	{
		if (pRoot == NULL)
			return true;
		//出现两个连续的红色的结点  
		if (pRoot->_pParent&&pRoot->_color == Red&&pRoot->_pParent->_color == Red)
			return false;
		//如果是黑色结点k++  
		if (pRoot->_color == Black)
			k++;
		if (pRoot->_pLeft == NULL&&pRoot->_pRight == NULL)//如果是叶子结点的话进行判断k和count  
		{
			if (k == count)
				return true;
			else
				return false;
		}
		return _IsRBTree(pRoot->_pLeft, count, k) && _IsRBTree(pRoot->_pRight, count, k);
	}
	//右单旋转  
	void _RBRolateR(PNode parent)
	{
		if (NULL == parent)
			return;
		PNode SubL = parent->_pLeft;// 
		
		PNode SubLR = SubL->_pRight;

		parent->_pLeft = SubLR;
		if (SubLR != NULL)
			SubLR->_pParent = parent;
		PNode pParent = parent->_pParent;
		SubL->_pRight = parent;
		parent->_pParent = SubL;
		if (pParent == NULL)
		{
			_pRoot = SubL;
			SubL->_pParent = NULL;
		}
		else
		{
			if (pParent->_pLeft == parent)
			{
				pParent->_pLeft = SubL;
				SubL->_pParent = parent;
			}
			else
			{
				pParent->_pRight = SubL;
				SubL->_pParent = parent;
			}
		}
	}
	void _RBRolateL(PNode parent)
	{
		if (NULL == parent)
			return;
		PNode SubR = parent->_pRight;
		PNode SubRL = SubR->_pLeft;
		parent->_pRight = SubRL;
		if (SubRL)
		{
			SubRL->_pParent = parent;
		}
		PNode pParent = parent->_pParent;
		SubR->_pLeft = parent;
		parent->_pParent = SubR;
		if (pParent == NULL)
		{
			_pRoot = SubR;
			SubR->_pParent = NULL;
		}
		else
		{
			if (pParent->_pLeft == parent)
			{
				pParent->_pLeft = SubR;
				SubR->_pParent = pParent;
			}
			else
			{
				pParent->_pRight = SubR;
				SubR->_pRight = pParent;
			}
		}
	}
	void _InOrder(PNode pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_pLeft);
		//	cout << pRoot->_key << " ";
			_InOrder(pRoot->_pRight);
		}
	}
};
//template<class K>
//struct SetKeyOfvalue//定义Set中的仿函数
//{
//	const K& operator()(const K& k)
//	{
//		return k;
//	}
//};
//void TestRBTree()
//{
//	int a[] = { 5, 3, 7, 1, 4, 6, 13, 8, 15, 12 };
//	RBTree<int, int, SetKeyOfvalue<int>> t;
//	for (size_t i = 0; i < sizeof(a) / sizeof(*a); ++i)
//		t.Insert(a[i]);
//	RBTree<int, int, SetKeyOfvalue<int>>::Iterator it = t.Begin();
//	while (it != t.End())
//	{
//		cout << *it <<" ";
//		++it;
//	}
//}