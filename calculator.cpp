/**
 * 经典问题：表达式求值
 * 整个OJ题
 */ 
#include <iostream>

#define ALLOC_FAIL \
do{std::cerr<<"Allocate failed! "<<std::endl;\
return false;\
}while(false)

template <typename T>
class DoubleLinkedList {
protected:
	class Node {
	public:
		T data;
		Node* prev, * next;
		Node(T data_) :data(data_), prev(nullptr), next(nullptr) {}
	};

	Node* head, * tail;
public:
	DoubleLinkedList() :head(nullptr), tail(nullptr) {}
	DoubleLinkedList(const DoubleLinkedList& lst);
	bool insert(Node* site, const T& data);
	bool prepend(const T& data);
	bool append(const T& data);
	bool remove(const T& data);
	bool remove(Node* nd);
	inline bool empty()const {
		return head == nullptr;
	}
	Node* getItem(int i);
	void clear();
	int size()const;
	DoubleLinkedList<T>& operator=(const DoubleLinkedList<T>& lst);
	DoubleLinkedList<T>& operator=(DoubleLinkedList<T>&& lst);
	~DoubleLinkedList() { clear(); }
};

template<typename T>
DoubleLinkedList<T>::DoubleLinkedList(const DoubleLinkedList& lst) :
	head(nullptr), tail(nullptr)
{
	auto p = lst.head;
	if (p) {
		while (p != lst.tail) {
			append(p->data);
			p = p->next;
		}
		append(p->data);
	}
}

template<typename T>
bool DoubleLinkedList<T>::prepend(const T& data)
{
	Node* p = new Node(data);
	if (!p)ALLOC_FAIL;
	if (!head) {
		p->prev = p->next = p;
		head = tail = p;
	}
	else {
		p->next = head;
		head->prev = p;
		p->prev = tail;
		tail->next = p;
		head = p;
	}
	return true;
}

template<typename T>
bool DoubleLinkedList<T>::append(const T& data)
{
	Node* node = new Node(data);
	if (!node)
		ALLOC_FAIL;
	if (head == nullptr) {
		//空表插入
		head = node;
		tail = node;
		node->prev = node;
		node->next = node;
	}
	else {
		tail->next = node;
		node->prev = tail;
		node->next = head;
		head->prev = node;
		tail = node;
	}
	return true;
}

template<typename T>
bool DoubleLinkedList<T>::remove(Node* nd)
{
	if (nd->next == nd) {
		//最后一个
		head = tail = nullptr;
		delete nd;
	}
	else {
		nd->prev->next = nd->next;
		nd->next->prev = nd->prev;
		if (head == nd)
			head = nd->next;
		if (tail == nd)
			tail = nd->prev;
		delete nd;
	}
	return true;
}

/// <summary>
/// 直接写循环，不考虑表长度
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="i">下标从0开始。</param>
/// <returns></returns>
template<typename T>
typename DoubleLinkedList<T>::Node* DoubleLinkedList<T>::getItem(int i)
{
	if (i < 0 || !head)return nullptr;
	int t = 0;
	Node* p = head;
	while (t < i) {
		p = p->next;
		t++;
	}
	return p;
}

template<typename T>
void DoubleLinkedList<T>::clear()
{
	while (!empty())
		remove(head);
	head = tail = nullptr;
}

template<typename T>
int DoubleLinkedList<T>::size() const
{
	if (!head)
		return 0;
	int t = 1;
	auto p = head;
	while (p != tail) {
		p = p->next; t++;
	}
	return t;
}

template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(const DoubleLinkedList<T>& lst)
{
	clear();
	auto p = lst.head;
	for (; p != lst.tail; p = p->next) {
		append(p->data);
	}
	append(lst.tail->data);
	return *this;
}

template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(DoubleLinkedList<T>&& lst)
{
	head = lst.head;
	tail = lst.tail;
	lst.head = lst.tail = nullptr;
	return *this;
}

template <typename T>
class LinkedStack :private DoubleLinkedList<T> {
public:
	DoubleLinkedList<T>::empty;
	void push(const T& t);
	T pop();
	const T& top()const;
};

template<typename T>
void LinkedStack<T>::push(const T& t)
{
	this->append(t);
}

//要保证非空！！
template<typename T>
T LinkedStack<T>::pop()
{
	if (empty()) {
		throw "EMPTY STACK!!!";
	}
	T t = this->tail->data;
	this->remove(this->tail);
	return t;
}

template<typename T>
const T& LinkedStack<T>::top() const
{
	return this->tail->data;
}

using namespace std;

//表达式的一项，操作符或者操作数
class Item {
public:
	union {
		int n;
		char op;
	} data;
	bool isOperator;

	static Item makeNumItem(int t);
	static Item makeOprItem(char ch);
	friend ostream& operator<<(ostream& out, const Item& it);

	inline bool isEndOpr()const {
		return isOperator && data.op == '#';
	}
};


Item Item::makeNumItem(int t)
{
	Item it;
	it.isOperator = false;
	it.data.n = t;
	return it;
}

Item Item::makeOprItem(char ch)
{
	Item it;
	it.isOperator = true;
	it.data.op = ch;
	return it;
}


class Calc {
	LinkedStack<int> nums;
	LinkedStack<char> ops;
	DoubleLinkedList<Item> suffix;

public:
	void run();
private:
	void addOperator(char ch);
	static int icp(char ch);
	static int isp(char ch);
	static int step(char op, int n1, int n2);

};


void Calc::run()
{
	char ch;
	int x;
	//读取和转换
	ops.push('#');
	while ((ch = cin.get()) != '\n') {
		switch (ch) {
		case '+':case '-':case 'x':case '(':case ')': addOperator(ch); break;
		case '\n':case '\0':break;
		default:cin.putback(ch); cin >> x; suffix.append(Item::makeNumItem(x));  break;
		}
	}
	
	//最后的操作符清理出来
	while ((ch = ops.pop()) != '#') {
		suffix.append(Item::makeOprItem(ch));
	}
	suffix.append(Item::makeOprItem('#'));

	//auto q = suffix.getItem(0);
	//for (; !q->data.isEndOpr(); q = q->next) {
	//	cout << q->data;
	//}
	//cout << endl;

	//计算后缀表达式
	auto p = suffix.getItem(0);
	for (; !p->data.isEndOpr(); p = p->next) {
		if (p->data.isOperator) {
			int n2 = nums.pop(), n1 = nums.pop();
			nums.push(step(p->data.data.op, n1, n2));
		}
		else {   //操作数
			nums.push(p->data.data.n);
		}
	}
	cout << nums.top() << endl;
}

void Calc::addOperator(char ch)
{
	while (true) {
		char op = ops.top();
		if (icp(ch) > isp(op)) {
			ops.push(ch);
			return;
		}
		else if (icp(ch) < isp(op)) {
			ops.pop();
			suffix.append(Item::makeOprItem(op));
		}
		else {   //优先级相等  
			ops.pop();
			return;
		}
	}
}

int Calc::icp(char ch)
{
	switch (ch) {
	case '#':return 0;
	case '(':return 6;
	case 'x':return 4;
	case '+':case '-':return 2;
	case ')':return 1;
	}
}

int Calc::isp(char ch)
{
	switch (ch) {
	case '#':return 0;
	case '(':return 1;
	case 'x':return 5;
	case '+':case '-':return 3;
	case ')':return 6;
	}
}

int Calc::step(char op, int n1, int n2)
{
	switch (op) {
	case '+':return n1 + n2;
	case '-':return n1 - n2;
	case 'x':return n1 * n2;
	default:cerr << "INVALID OPERATOR: " << op << endl; return 0;
	}
}


int main()
{
	Calc calc;
	calc.run();
}

ostream& operator<<(ostream& out, const Item& it)
{
	if (it.isOperator)
		out << it.data.op;
	else
		out << it.data.n;
	return out;
}
