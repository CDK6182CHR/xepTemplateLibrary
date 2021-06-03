
/// <summary>
/// 链式队列的最简实现
/// </summary>
template <typename T>
class Queue {
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
	Queue() :head(nullptr), tail(nullptr), _count(0) {}
	~Queue();
	Queue(const Queue&) = delete;
	Queue(Queue&&) = delete;
	Queue& operator=(const Queue&) = delete;
	Queue& operator=(Queue&&) = delete;
	void clear();
	void push(const T& t);
	void pop();
	T& front();
	
	inline int count()const { return _count; }
	inline bool empty()const { return head == nullptr; }
};

template<typename T>
Queue<T>::~Queue()
{
	clear();
}

template<typename T>
void Queue<T>::clear()
{
	while (!empty())
		pop();
}

template<typename T>
void Queue<T>::push(const T& t)
{
	Node* p = new Node(t);
	if (!tail) {
		//插入第一个元素
		head = tail = p;
	}
	else {
		tail->next = p;
		tail = p;
	}
	_count++;
}

template<typename T>
void Queue<T>::pop()
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
T& Queue<T>::front()
{
	if (empty()) {
		throw "EMPTY (FRONT)";
	}
	return head->data;
}