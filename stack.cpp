
/// <summary>
/// 单链表实现版本 链式栈
/// </summary>
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
