

/// <summary>
/// 单链表的实现
/// </summary>
template <typename T>
class List {
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
	List() :head(nullptr), tail(nullptr), _count(0) {}
	~List();
	List(const List&) = delete;
	List(List&&) = delete;
	List& operator=(const List&) = delete;
	List& operator=(List&&) = delete;
	void clear();
	void push_back(const T& t);
	void pop_front();
	void remove(const T& t);
	T& back();
	T& front();
	inline const Node* get_head()const { return head; }
	inline const Node* get_tail()const { return tail; }
	inline int count()const { return _count; }
	inline bool empty()const { return head == nullptr; }
};

template<typename T>
List<T>::~List()
{
	clear();
}

template<typename T>
void List<T>::clear()
{
	while (!empty())
		pop_front();
}

template<typename T>
void List<T>::push_back(const T& t)
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
void List<T>::pop_front()
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
void List<T>::remove(const T& t)
{
	if (!head) {
		throw "EMPTY REMOVE!!!";
	}
	//case 1: 如果直接是head
	if (head->data == t) {
		auto* tmp = head;
		head = head->next;
		if (!head) {
			//最后一个结点...
			tail = nullptr;
		}
		delete tmp;
	}
	else {
		//case 2: 不是head,两个指针走
		auto* p0 = head, * p1 = head->next;
		while (p1 && p1->data != t) {
			p0 = p1;
			p1 = p1->next;
		}
		if (!p1) {
			throw "VALUE NOT FOUND";
		}
		//现在：p0是前一个，p1是要删除的结点
		p0->next = p1->next;
		if (!p0->next) {
			//同时也是最后一个结点
			tail = p0;
		}
		delete p1;
	}
}

template<typename T>
T& List<T>::back()
{
	if (empty())
		throw "EMPTY (BACK)";
	return tail->data;
}

template<typename T>
T& List<T>::front()
{
	if (empty()) {
		throw "EMPTY (FRONT)";
	}
	return head->data;
}