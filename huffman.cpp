/**
 * Huffman树标准实现
 * 包括OJ题内容
 */
#include <iostream>
#include <utility>
#include <memory>
#include <cstring>

/// <summary>
/// 具有固定最大长度的最小堆
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class MinHeap {
	const int max_size;
	T* data;
	int size;
public:
	MinHeap(int size_);
	~MinHeap()noexcept;
	void push(const T& t);

	void emplace(T&& t);
	void pop();
	inline const T& top()const {
		return data[0];
	}
	inline T& top() {
		return data[0];
	}
private:
	void init_heap();   //这个没必要写

	void sift_down(int start);
	void sift_up(int start);

	
};

template<typename T>
MinHeap<T>::MinHeap(int size_):max_size(size_),size(0)
{
	data = new T[max_size];
}

template<typename T>
MinHeap<T>::~MinHeap()noexcept
{
	if (data) {
		delete[]data;
		data = nullptr;
	}
}

template<typename T>
void MinHeap<T>::sift_down(int start)
{
	int i = start;
	int ch = 2 * i + 1;   //指向子女中最小的那个
	while (ch < size) {
		if (ch + 1 < size && data[ch + 1] < data[ch]) {
			ch++;
		}
		if (data[ch] < data[i]) {
			std::swap(data[ch], data[i]);
			i = ch;
			ch = 2 * i + 1;
		}
		else break;
	}
}

template<typename T>
void MinHeap<T>::sift_up(int start)
{
	int i = start;   //int div  指向父节点
	while (i > 0) {
		int par = (i - 1) / 2;
		if (data[i] < data[par]) {
			std::swap(data[i], data[par]);
			i = par;
		}
		else break;
	}
}

template<typename T>
void MinHeap<T>::emplace(T&& t)
{
	if (size >= max_size)
		throw "OVERFLOWED";
	data[size++] = std::move(t);
	sift_up(size - 1);
}

template<typename T>
void MinHeap<T>::pop()
{
	if (size <= 0)
		throw "EMPTY HEAP";
	data[0] = std::move(data[--size]);  //直接覆盖！
	sift_down(0);
}

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

	BinTree(const BinTree<T>&) = delete;
	BinTree(BinTree<T>&& bt)noexcept;

	BinTree<T>& operator=(const BinTree<T>&) = delete;
	BinTree<T>& operator=(BinTree<T>&& bt);

	inline void set_root(BinNode* node) {
		root = node;
	}

	inline bool empty()const {
		return root == nullptr;
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

	BinNode* find(const T& data);

	BinNode* insert_left(BinNode* p, const T& data);
	BinNode* insert_right(BinNode* p, const T& data);
	~BinTree() { clear(root); }
	void get_depth_sum(int* depth, int* dpsum, const int n)const {
		get_depth_sum(depth, dpsum, n, root);
	}
private:
	void show_post(const BinNode* n)const;
	void show_pre(const BinNode* n)const;
	void show_in(const BinNode* n)const;
	BinNode* find(const T& data, BinNode* root);

	void clear(BinNode* b);
};

template <typename T>
int arfind(const T ar[], const T& val, const int n)
{
	for (int i = 0; i < n; i++)
		if (ar[i] == val)
			return i;
	return -1;
}

using namespace std;


template<typename T>
BinTree<T>::BinTree(BinTree<T>&& bt) noexcept:
	root(bt.root)
{
	bt.root = nullptr;
}

template<typename T>
BinTree<T>& BinTree<T>::operator=(BinTree<T>&& bt)
{
	root = bt.root;
	bt.root = nullptr;
	return *this;
}

template<typename T>
void BinTree<T>::show_post() const
{
	show_post(root);
	cout << endl;
}

template<typename T>
typename BinTree<T>::BinNode* BinTree<T>::find(const T& data)
{
	return find(data, root);
}

template<typename T>
typename BinTree<T>::BinNode* BinTree<T>::insert_left(BinNode* p, const T& data)
{
	if (p->left)
		throw "LEFT NOT EMPTY";
	p->left = new BinNode(data);
	return p->left;
}

template<typename T>
typename BinTree<T>::BinNode* BinTree<T>::insert_right(BinNode* p, const T& data)
{
	if (p->right)
		throw "RIGHT NOT EMPTY";
	p->right = new BinNode(data);
	return p->right;
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
	if (n == nullptr)
		return;
	cout << n->data << ' ';
	if (n->left)
		show_pre(n->left);
	if (n->right)
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
typename BinTree<T>::BinNode* BinTree<T>::find(const T& data, BinNode* root)
{
	if (!root)
		return nullptr;
	if (root->data == data)
		return root;
	auto* p1 = find(data, root->left);
	if (p1)
		return p1;
	auto* p2 = find(data, root->right);
	return p2;
}

template<typename T>
void BinTree<T>::clear(BinNode* b)
{
	if (!b)
		return;
	if (b->left)
		clear(b->left);
	if (b->right)
		clear(b->right);
	delete b;
}

struct HufData {
	int letter, weight;
};

class HufTree :public BinTree<HufData>
{
public:
	using Node = BinTree<HufData>::BinNode;
	HufTree() = default;
	HufTree(int letter, int weight);
	inline bool operator<(const HufTree& ht)const {
		return root->data.weight < ht.root->data.weight;
	}

	//在位的合并
	HufTree merge_with(HufTree& t);

	/// <summary>
	/// 找到每个字母对应的深度
	/// </summary>
	/// <param name="depth"></param>
	void get_letter_depth(int depth[]);
private:
	void get_letter_depth(int depth[], Node* rt, int h);
};


HufTree::HufTree(int letter,int weight)
{
	root = new Node({ letter,weight });
}

HufTree HufTree::merge_with(HufTree& t)
{
	auto* p = new Node({ -1,root->data.weight + t.root->data.weight });
	p->left = root;
	p->right = t.root;
	root = nullptr;
	t.root = nullptr;
	HufTree ht;
	ht.set_root(p);
	return ht;
}

void HufTree::get_letter_depth(int depth[])
{
	get_letter_depth(depth, root, 0);
}

void HufTree::get_letter_depth(int depth[], Node* rt, int h)
{
	if (rt == nullptr)
		return;
	auto t = rt->data;
	if (t.letter != -1)
		depth[t.letter] = h ;
	get_letter_depth(depth, rt->left, h + 1);
	get_letter_depth(depth, rt->right, h + 1);
}


constexpr int LEN = 10010;
constexpr int LETTERS = 26;

int main()
{
	char buffer[LEN];
	cin.getline(buffer, LEN);

	MinHeap<HufTree> heap(LETTERS * 2);

	//统计字母数量
	int counts[26]{ 0 };
	for (int i = 0; i < strlen(buffer); i++) {
		int t = buffer[i] - 'A';
		counts[t]++;
	}

	//统计存在的字母数量，建立堆
	int cnt = 0;
	for (int i = 0; i < 26; i++) {
		if (counts[i]) {
			heap.emplace(HufTree(i, counts[i]));
			cnt++;
		}
	}

	while (cnt > 1) {
		auto t1 = std::move(heap.top());
		heap.pop();
		auto t2 = std::move(heap.top());
		heap.pop();
		heap.emplace(t1.merge_with(t2));
		cnt--;
	}

	//最后只剩下一棵树了
	auto& ht = heap.top();

	int depth[26]{ 0 };

	ht.get_letter_depth(depth);

	int res = 0;
	for (int i = 0; i < 26; i++) {
		if (counts[i])
			res += counts[i] * depth[i];
	}
	cout << res << endl;

}
