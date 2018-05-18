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
//������ĵ�����
template<class V, class Ref, class Ptr>//�ֱ�V V* V&
struct _RBTreeIterator
{
	typedef _RBTreeIterator<V, Ref, Ptr> Self;
	typedef RBTreeNode<V> Node;
	Node* _node;//������ͷ����ָ�룬��ʵ����������һ������ָ��ֻ�������Ĺ�����++��--����һЩ�����ö���
	_RBTreeIterator(Node* node)
		:_node(node){}//����ֻ��Ҫ ǳ�����Ϳ����ˣ��͵������Ĺ����й�
	_RBTreeIterator()
	{}
	Ref operator*()//��������
	{
		return _node->_v;//��map����pair����setֱ�ӷ���k�Ϳ�����
	}
	Ptr operator->()//����ָ��,ֻ����ģ���������ӵ����
	{
		return &(operator*());
	}

	Self operator++()//ǰ��++
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
	//typedef _RBTreeIterator< V, const V&, const V*> Iterator;//���ں�����п���ʹ�õ�����
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
		if (NULL == _pRoot)//���ڵ�Ϊ��ɫ  
		{
			_pRoot = new Node( value);
			_pRoot->_color = Black;
			return make_pair(Iterator(_pRoot),true);//make_pair<>����һ��pair���󷵻�ȥ
		}
		KeyOfValue kof;
		PNode cur = _pRoot;
		PNode pParent = NULL;
		while (cur)//�Ҳ����λ��  
		{
			if (kof(cur->_v)==kof(value))
				return make_pair(Iterator(cur),false);
			else if (kof(cur->_v)>kof(value))//�����÷º�������� ������ set��map��ֵ��ͬ������
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
		PNode pNewNode = new Node(value);//�²���Ľ�����ɫĬ���Ǻ��  
		PNode ret = pNewNode;
		//�ҵ������λ���Ժ�ʼ����  
		
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
		////������������Ժ����Ǿ�Ҫ���ݺ�������������жϽ���Ƿ����㣬�Ӷ��������  
		while (pParent)
		{
			if (pParent->_color == Black)//������Ǻڵ�,���õ���ֱ���˳�  
			{
				break;
			}
			//��¼�游���Ͷ��屣��������  
			PNode gparent = pParent->_pParent;
			PNode uncle = NULL;
			if (pParent == gparent->_pLeft)//�������ӽڵ���  
			{
				uncle = gparent->_pRight;
				if (uncle&& uncle->_color == Red)//�����������Ϊ�����3,4  
				{
					gparent->_color = Red;
					uncle->_color = pParent->_color = Black;
					pNewNode = gparent;//����gg���  
					pParent = gparent->_pParent;
					continue;
				}
				else if (uncle == NULL || uncle->_color == Black)//���岻���ڻ���Ϊ��  
				{
					if (pNewNode == pParent->_pLeft)//������  
					{
						_RBRolateR(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
					else//�ڲ����  
					{
						_RBRolateL(pParent);
						std::swap(pParent, pNewNode);//����pParent�Ͳ�����ָ���ֵ  
						_RBRolateR(gparent);
						gparent->_color = Red;
						pParent->_color = Black;
					}
				}
				break;//ֱ������ѭ��  
			}
			else//�ұߵ����  
			{
				uncle = gparent->_pLeft;
				if (uncle&&uncle->_color == Red)//������ڶ���Ϊ��ɫ  
				{
					gparent->_color = Red;
					uncle->_color = pParent->_color = Black;
					pNewNode = gparent;
					pParent = pNewNode->_pParent;
					continue;
				}
				else if (uncle == NULL || uncle->_color == Black)//���岻���ڻ���Ϊ��  
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
		if (_pRoot == NULL)//���ڵ� Ϊ���Ǻ����  
			return true;
		if (_pRoot->_color == Red)//���ڵ�Ϊ��ɫ�϶����Ǻ����  
			return false;
		int count = 0;//ͳ�ƺ�ɫ������Ŀ  
		PNode cur = _pRoot;
		while (cur)//����һ���ο�·���ĺ�ɫ������Ŀ  
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
	bool _IsRBTree(PNode pRoot, int& count, int k)//�����K���ܴ�����  
	{
		if (pRoot == NULL)
			return true;
		//�������������ĺ�ɫ�Ľ��  
		if (pRoot->_pParent&&pRoot->_color == Red&&pRoot->_pParent->_color == Red)
			return false;
		//����Ǻ�ɫ���k++  
		if (pRoot->_color == Black)
			k++;
		if (pRoot->_pLeft == NULL&&pRoot->_pRight == NULL)//�����Ҷ�ӽ��Ļ������ж�k��count  
		{
			if (k == count)
				return true;
			else
				return false;
		}
		return _IsRBTree(pRoot->_pLeft, count, k) && _IsRBTree(pRoot->_pRight, count, k);
	}
	//�ҵ���ת  
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
//struct SetKeyOfvalue//����Set�еķº���
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