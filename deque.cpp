
/// <summary>
/// 双向（不循环）链表实现双端队列。
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class DeQue {
public:
	struct Node {
		T data;
		Node* prev, * next;
		Node() = delete;
		Node(const T& data_) :data(data_), prev(nullptr), next(nullptr) {}
	};
private:
	Node* head, * tail;
	int _count;
public:
	DeQue() :head(nullptr), tail(nullptr), _count(0) {}
	~DeQue();
	DeQue(const DeQue&) = delete;
	DeQue(DeQue&&) = delete;
	DeQue& operator=(const DeQue&) = delete;
	DeQue& operator=(DeQue&&) = delete;
	void clear();
	void push_back(const T& t);
	void push_front(const T& t);
	void pop_back();
	void pop_front();
	T& back();
	T& front();
	inline const Node* get_head()const { return head; }
	inline const Node* get_tail()const { return tail; }
	inline int count()const { return _count; }
	inline bool empty()const { return head == nullptr; }
};

template<typename T>
DeQue<T>::~DeQue()
{
	clear();
}

template<typename T>
void DeQue<T>::clear()
{
	while (!empty())
		pop_back();
}

template<typename T>
void DeQue<T>::push_back(const T& t)
{
	Node* p = new Node(t);
	if (!tail) {
		//插入第一个元素
		head = tail = p;
	}
	else {
		tail->next = p;
		p->prev = tail;
		tail = p;
	}
	_count++;
}

template<typename T>
void DeQue<T>::push_front(const T& t)
{
	Node* p = new Node(t);
	if (!head) {
		head = tail = p;
	}
	else {
		head->prev = p;
		p->next = head;
		head = p;
	}
	_count++;
}

template<typename T>
void DeQue<T>::pop_back()
{
	if (!tail)
		return;
	auto* p = tail;
	if (tail->prev) {
		//不是唯一结点
		tail->prev->next = nullptr;
		tail = tail->prev;
	}
	else {
		head = tail = nullptr;
	}
	delete p;
	_count--;
}

template<typename T>
void DeQue<T>::pop_front()
{
	if (!head)
		return;
	auto* p = head;
	if (head->next) {
		head->next->prev = nullptr;
		head = head->next;
	}
	else {
		head = tail = nullptr;
	}
	delete p;
	_count--;
}

template<typename T>
T& DeQue<T>::back()
{
	if (empty())
		throw "EMPTY (BACK)";
	return tail->data;
}

template<typename T>
T& DeQue<T>::front()
{
	if (empty()) {
		throw "EMPTY (FRONT)";
	}
	return head->data;
}
