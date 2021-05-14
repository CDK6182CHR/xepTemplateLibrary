#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <sstream>
using namespace std;

//标准二岔链表实现二叉树
template <typename T>
class BinTree {
public:
	struct BinNode {
		T data;
		BinNode* left, * right;
		BinNode(const T& d) :data(d), left(nullptr), right(nullptr) {}
	};
	BinNode* root;

	BinTree() :root(nullptr) {}
	inline void set_root(BinNode* node) {
		root = node;
	}

	void show_post()const;

	inline void show_pre()const {
		show_pre(root);
	}

	inline void show_in()const {
		show_in(root);
	}

	inline const BinNode* get_root()const {
		return root;
	}

	inline void dump_in(T* a, int n)const {
		dump_in(a, n, 0, root);
	}
private:
	void show_post(const BinNode* n)const;
	void show_pre(const BinNode* n)const;
	void show_in(const BinNode* n)const;
	//返回：下一个要赋值的位置
	int dump_in(T* a, int n, int start, const BinNode* b)const;
};

template <typename T>
int arfind(const T ar[], const T& val, const int n)
{
	for (int i = 0; i < n; i++)
		if (ar[i] == val)
			return i;
	return -1;
}


template<typename T>
void BinTree<T>::show_post() const
{
	show_post(root);
	cout << endl;
}

template<typename T>
void BinTree<T>::show_post(const BinNode* n)const
{
	if (n == nullptr)
		return;
	if (n->left)
		show_post(n->left);
	if (n->right)
		show_post(n->right);
	cout << n->data << ' ';
}

template<typename T>
void BinTree<T>::show_pre(const BinNode* n) const
{
	if (n == nullptr) {
		cout << -1 << ' ';
		return;
	}
	cout << n->data << ' ';
	show_pre(n->left);
	show_pre(n->right);
}

template<typename T>
void BinTree<T>::show_in(const BinNode* n) const
{
	if (n == nullptr)
		return;
	if (n->left)
		show_in(n->left);
	cout << n->data << ' ';
	if (n->right)
		show_in(n->right);
}

template<typename T>
int BinTree<T>::dump_in(T* a, int n, int start, const BinNode* b) const
{
	if (b == nullptr)
		return start;
	start = dump_in(a, n, start, b->left);
	a[start++] = b->data;
	return dump_in(a, n, start, b->right);
}

using Node = BinTree<int>::BinNode;

Node* read_pre()
{
	int x;
	cin >> x;
	if (x == -1) {
		return nullptr;
	}
	auto* p = new Node(x);
	p->left = read_pre();
	p->right = read_pre();
	return p;
}

/// <summary>
/// 递归构建
/// https://blog.csdn.net/qq_39445165/article/details/93971171
/// </summary>
/// <param name="post"></param>
/// <param name="in"></param>
/// <param name="ni">根节点在post中的下标</param>
/// <returns></returns>
Node* construct(int* post, int* in, const int n)
{
	if (n == 0)
		return nullptr;
	//else if (n == 1) {
	//	return new Node(post[0]);
	//}
	int rt = post[n - 1];   //根节点的下标
	Node* p = new Node(rt);
	int nj = arfind(in, rt, n);

	int nl = nj;  //左子树的数目
	int nr = n - nj - 1;   //右子树的数目

	auto* pleft = construct(post, in, nl);
	auto* pright = construct(post + nl, in + nl + 1, nr);

	p->left = pleft;
	p->right = pright;
	return p;
}
