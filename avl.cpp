#include <iostream>
#include <functional>

/// <summary>
/// 单链表实现版本 链式栈
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class Stack {
public:
	struct Node {
		T data;
		Node* next;
		Node() = delete;
		Node(const T& data_) :data(data_), next(nullptr) {}
	};
private:
	Node* head, * tail;
	int _count;
public:
	Stack() :head(nullptr), tail(nullptr), _count(0) {}
	~Stack();
	Stack(const Stack&) = delete;
	Stack(Stack&&) = delete;
	Stack& operator=(const Stack&) = delete;
	Stack& operator=(Stack&&) = delete;
	void clear();
	void push(const T& t);
	void pop();
	T& top();
	inline int count()const { return _count; }
	inline bool empty()const { return head == nullptr; }
};

template<typename T>
Stack<T>::~Stack()
{
	clear();
}

template<typename T>
void Stack<T>::clear()
{
	while (!empty())
		pop();
}

template<typename T>
void Stack<T>::push(const T& t)
{
	Node* p = new Node(t);
	if (!head)
		head = p;
	else {
		p->next = head;
		head = p;
	}
	_count++;
}

template<typename T>
void Stack<T>::pop()
{
	if (!head)
		return;
	auto* p = head;
	if (head->next) {
		head = head->next;
	}
	else {
		head = tail = nullptr;
	}
	delete p;
	_count--;
}

template<typename T>
T& Stack<T>::top()
{
	if (empty()) {
		throw "EMPTY (FRONT)";
	}
	return head->data;
}


/// <summary>
/// AVL树标准实现
/// </summary>
/// <typeparam name="K"></typeparam>
/// <typeparam name="V"></typeparam>
template <typename K,typename V>
class AVLTree {
public:
	struct AVLNode {
		K key;
		V value;
		int bf;  //平衡因子
		AVLNode* left, * right;
		AVLNode(const K& k,const V& v):key(k),value(v),bf(0),
			left(nullptr),right(nullptr){}
	};
private:
	AVLNode* root;
	void rotateL(AVLNode*& ptr);
	void rotateR(AVLNode*& ptr);
	void rotateRL(AVLNode*& ptr);
	void rotateLR(AVLNode*& ptr);
	void clear(AVLNode*& rt);
	void show_in(const AVLNode* rt)const;
	void show_pre(const AVLNode* rt)const;
	AVLNode* find_first_greater(const K& k, AVLNode* rt);
	AVLNode* find_last_less(const K& k, AVLNode* rt);
	AVLNode* find(const K& k, AVLNode* rt);
public:
	AVLTree();
	~AVLTree();
	void insert(const K& k, const V& v);
	void remove(const K& k);
	AVLNode* find_first_greater(const K& k);
	AVLNode* find_last_less(const K& k);
	inline AVLNode* find(const K& k) {
		return find(k, root);
	}
	inline const AVLNode* find(const K& k)const {
		return const_cast<AVLTree*>(this)->find(k);
	}

	inline const AVLNode* get_root()const {
		return root;
	}
	inline AVLNode* get_root() {
		return root;
	}
	inline void set_root(AVLNode* root_) {
		root = root_;
	}
	inline void show_in()const {
		show_in(root);
		std::cout << std::endl;
	}
	inline void show_pre()const {
		show_pre(root);
		std::cout << std::endl;
	}
};

template<typename K, typename V>
void AVLTree<K, V>::rotateL(AVLNode*& ptr)
{
	//A<B<C，注意一定非空
	auto pa = ptr, pb = ptr->right, pc = ptr->right->right;
	auto pbl = pb->left;
	pb->left = pa;
	pb->right = pc;
	pa->right = pbl;
	ptr = pb;
	//注意：删除的情况，bf没这么简单
	//pb->bf = 0;
	//pa->bf = 0;
	//pc的bf不变
	if (pb->bf == 0) {   //remove
		pa->bf = 1;
		pb->bf = -1;
	}
	else {   //insert; pb->bf==1
		pa->bf = 0;
		pb->bf = 0;
	}
}

template<typename K, typename V>
void AVLTree<K, V>::rotateR(AVLNode*& ptr)
{
	//仍然采用a<b<c的设定
	auto pc = ptr, pb = ptr->left, pa = ptr->left->left;
	auto pbr = pb->right;
	pb->left = pa;
	pb->right = pc;
	pc->left = pbr;
	ptr = pb;
	if (pb->bf == 0) {
		pc->bf = -1;
		pb->bf = 1;
	}
	else {  //pb->bf == -1
		pb->bf = 0;
		pc->bf = 0;
	}
	
}

template<typename K, typename V>
void AVLTree<K, V>::rotateRL(AVLNode*& ptr)
{
	auto pa = ptr, pc = ptr->right, pb = ptr->right->left;
	auto pbl = pb->left, pbr = pb->right;
	pb->left = pa;
	pb->right = pc;
	pa->right = pbl;
	pc->left = pbr;

	if (pb->bf == 0) {
		//好像有个特殊情况？如果B是新加的结点，好像也成立。
		//这样大家都是0
		pa->bf = pc->bf = 0;
	}
	else if (pb->bf > 0) {
		//新的结点加在pbr上面
		pa->bf = -1;
		pc->bf = 0;
	}
	else {
		//pb->bf < 0
		pa->bf = 0;
		pc->bf = 1;
	}
	pb->bf = 0;
	ptr = pb;
}

template<typename K, typename V>
void AVLTree<K, V>::rotateLR(AVLNode*& ptr)
{
	auto pc = ptr, pa = ptr->left, pb = ptr->left->right;
	auto pbl = pb->left, pbr = pb->right;
	pb->left = pa;
	pb->right = pc;
	pa->right = pbl;
	pc->left = pbr;
	if (pb->bf == 0) {
		//特殊情况：B是新加结点。参见RL
		pc->bf = pa->bf = 0;
	}
	else if (pb->bf > 0) {
		//新增结点属于pbr
		pc->bf = 0;
		pa->bf = -1;
	}
	else {
		pc->bf = 1;
		pa->bf = 0;
	}
	pb->bf = 0;
	ptr = pb;
}

template<typename K, typename V>
void AVLTree<K, V>::clear(AVLNode*& rt)
{
	if (rt == nullptr)
		return;
	clear(rt->left);
	clear(rt->right);
	delete rt;
	rt = nullptr;
}

template<typename K, typename V>
void AVLTree<K, V>::show_in(const AVLNode* rt) const
{
	if (!rt)
		return;
	show_in(rt->left);
	std::cout << '(' << rt->key << ',' << rt->bf << ") ";
	show_in(rt->right);
}

template<typename K, typename V>
void AVLTree<K, V>::show_pre(const AVLNode* rt) const
{
	if (!rt) {
		std::cout << "-1 ";
		return;
	}
	std::cout << rt->value << ' ';
	show_pre(rt->left);
	show_pre(rt->right);
}

template<typename K, typename V>
typename AVLTree<K,V>::AVLNode* AVLTree<K, V>::find_first_greater(const K& k, 
	typename AVLTree<K,V>::AVLNode* rt)
{
	if (!rt)
		return nullptr;
	if (k >= rt->key) {
		//如果恰好有相等的，返回中序遍历的直接后继
		return find_first_greater(k, rt->right);
	}
	else  { // (k < rt->key)
		//它其实可能是解。现在要找左子树里，有没有更合适的解。
		auto solleft = find_first_greater(k, rt->left);
		if (solleft && solleft->key < rt->key) {
			return solleft;
		}
		else
			return rt;
	}
}

/// <summary>
/// 寻找表中第一个比所给key来的小的
/// </summary>
template<typename K, typename V>
typename AVLTree<K,V>::AVLNode* AVLTree<K, V>::find_last_less(const K& k,
	typename AVLTree<K,V>::AVLNode* rt)
{
	if (!rt)
		return nullptr;
	if (k <= rt->key) {
		//第一个比它来的小的
		return find_last_less(k, rt->left);
	}
	else {//(k > rt->key) 
		//有可能是解
		auto rightsol = find_last_less(k, rt->right);
		if (rightsol && rightsol->key > rt->key)
			return rightsol;
		return rt;
	}
}

template<typename K, typename V>
typename AVLTree<K,V>::AVLNode* AVLTree<K, V>::find(const K& k, 
	typename AVLTree<K,V>::AVLNode* rt)
{
	if (!rt)
		return nullptr;
	if (k == rt->key)
		return rt;
	else if (k < rt->key)
		return find(k, rt->left);
	else return find(k, rt->right);
}

template<typename K, typename V>
AVLTree<K, V>::AVLTree():root(nullptr)
{
}

template<typename K, typename V>
AVLTree<K, V>::~AVLTree()
{
	clear(root);
}

//直接书写非递归算法，参考教材写法
template<typename K, typename V>
void AVLTree<K, V>::insert(const K& k, const V& v)
{
	if (!root) {
		//第一个结点直接再见
		root = new AVLNode(k, v);
		return;
	}
	//注意栈里面放的是left/right那个变量的引用！！
	Stack<std::reference_wrapper<AVLNode*>> st;
	st.push(std::ref(root));

	AVLNode* pr = root;
	AVLNode* p = new AVLNode(k, v);
	//伪递归找插入点  注意保证pr非空
	while (true) {
		if (k == pr->key) {
			//相等插入的情况，暂不做任何处理
			return;
		}
		else if (k < pr->key) {
			if (pr->left == nullptr) {
				pr->left = p;
				break;
			}
			else {
				st.push(std::ref(pr->left));
				pr = pr->left;
			}
		}
		else {  //k > pr->key
			if (pr->right == nullptr) {
				pr->right = p;
				break;
			}
			else {
				st.push(std::ref(pr->right));
				pr = pr->right;
			}
		}
	}
	//上个循环结束时：p为新插入结点，pr为p的父节点 也在栈顶

	//loop invariant: pr为p的父节点。
	while (!st.empty()) {
		auto& pr = st.top().get();
		pr->bf += (p == pr->right ? 1 : -1);

		if (pr->bf == 0)
			break;

		if (pr->bf == 2) {
			if ((pr->bf > 0) == (p->bf > 0))
				rotateL(pr);
			else
				rotateRL(pr);
			break;
		}
		else if (pr->bf == -2) {
			if ((pr->bf > 0) == (p->bf > 0))
				rotateR(pr);
			else
				rotateLR(pr);
			break;
		}
		p = pr;
		st.pop();
	}
}

/// <summary>
/// 参考教材的非递归算法
/// </summary>
template<typename K, typename V>
void AVLTree<K, V>::remove(const K& k)
{
	Stack<std::reference_wrapper<AVLNode*>> st;
	AVLNode* p = root;
	st.push(std::ref(root));

	bool shorter = false;

	//目标：把栈顶元素搞成被删除元素的父节点，
	//这样方便算好bf。
	while (p) {
		if (p->key == k) {
			//remove this!!!
			//此时栈顶元素是要（逻辑上）删除的那个节点
			shorter = true;
			if (!p->left && !p->right) {
				//直接把p摘掉就好了
				auto& pthis = st.top().get();
				st.pop();
				auto& pr = st.top().get();
				pr->bf += (p == pr->right ? -1 : 1);
				pthis = nullptr;
				delete p;
			}
			else if (p->left && p->right) {
				//都非空 找右子树的最左
				st.push(std::ref(p->right));
				AVLNode* q = p->right;
				while (q->left) {
					st.push(std::ref(q->left));
					q = q->left;
				}
				//现在：q的左子树是空的；q是栈顶元素。
				//把q的东西挪到p那里，然后把q删了
				auto& pthis = st.top().get();
				p->key = std::move(q->key);
				p->value = std::move(q->value);
				pthis = q->right;
				st.pop();
				auto& pr = st.top().get();
				pr->bf += (q == pr->right ? -1 : 1);			
				delete q;
			}
			else if (p->left) {
				//左侧非空，右侧空的
				auto& pthis = st.top().get();
				pthis = p->left;
				st.pop();
				auto& pr = st.top().get();
				pr->bf += (pr->right == p ? -1 : 1);
				delete p;
			}
			else {
				//右侧非空
				auto& pthis = st.top().get();
				pthis = p->right;
				auto& pr = st.top().get();
				pr->bf += (p == pr->right ? -1 : 1);
				delete p;
			}
			break;
		}
		else if (k < p->key) {
			st.push(std::ref(p->left));
			p = p->left;
		}
		else {
			st.push(std::ref(p->right));
			p = p->right;
		}
	}
	if (!shorter) {
		//没有删除
		return;
	}
	//现在：栈顶元素是（实际）被删除元素的父节点
	//它的bf已经调整好了；但是栈里面其他元素的bf还没调整好
	p = nullptr;

	//loop invariant: p的bf已经调好了；pr是p的父节点
	while (!st.empty() && shorter) {
		AVLNode*& pr = st.top().get();
		if (p) {
			//注意：只有循环的第一次，p才会是nullptr
			//这时候pr的bf已经在上面的if里面调整过了，不能再次调整
			pr->bf += (p == pr->right ? -1 : 1);
		}
		//重新来
		if (pr->bf == 1 || pr->bf == -1) {
			//只能是原来为0的情况。此时子树高度不变，再见
			shorter = false;
			break;
		}
		else if (pr->bf == 0) {
			//只能是原来较高子树被缩短的情况。此时子树被缩短了，要往上
		}
		else if (pr->bf == 2) {
			//右子树高一点
			auto q = pr->right;
			if (q->bf == 0) {
				rotateL(pr);
				break;
			}
			else if (q->bf > 0) {
				rotateL(pr);
				//此时子树高度-1，继续往上找
			}
			else {  //q->bf < 0
				rotateRL(pr);
			}
		}
		else {  // pr->bf == -2
			auto q = pr->left;
			if (q->bf == 0) {
				rotateR(pr);
				break;
			}
			else if (q->bf < 0) {
				rotateR(pr);
			}
			else {
				rotateLR(pr);
			}
		}

		p = pr;
		st.pop();
	}
}


/// <summary>
/// 类似std::map::upper_bound()
/// 找第一个比k来的大的元素
/// 不包括k本身
template<typename K, typename V>
typename AVLTree<K,V>::AVLNode* AVLTree<K, V>::find_first_greater(const K& k)
{
	return find_first_greater(k, root);
}

template<typename K, typename V>
typename AVLTree<K,V>::AVLNode* AVLTree<K, V>::find_last_less(const K& k)
{
	return find_last_less(k, root);
}




using namespace std;



int main()
{
	auto lst = { 1,3,5,45,23,866,346,555,1040 };
	AVLTree<int, int> avl;
	for (auto t : lst) {
		avl.insert(t, t);
		avl.show_in();
		avl.show_pre();
	}
	auto a = { 1, 5,4,500,114514 };
	for (auto t : a) {
		auto s = avl.find_first_greater(t);
		cout << "first_greater(" << t << ")=";
		if (s) {
			cout << s->key << endl;
		}
		else {
			cout << "null" << endl;
		}
		s = avl.find_last_less(t);
		cout << "last_less(" << t << ")=";
		if (s) {
			cout << s->key << endl;
		}
		else {
			cout << "null" << endl;
		}
	}
	auto b = { 45,23,1040,346 };
	for (auto t : b) {
		avl.remove(t);
		avl.show_in();
		avl.show_pre();
	}
}