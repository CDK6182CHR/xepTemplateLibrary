/// <summary>
/// 2021.06.03  带附加头结点的单向链表
/// 参考std::forward_list 设计API
/// </summary>
template <typename T>
class ForwardList {
public:
	struct Node {
		T data;
		Node* next;
		Node(const T& d) :data(d), next(nullptr) {}
		Node() :data(), next(nullptr) {}
	};
private:
	Node head;
public:
	ForwardList() = default;
	ForwardList(const ForwardList& lst);
	ForwardList(ForwardList&& lst);
	~ForwardList();
	ForwardList& operator=(const ForwardList& lst);
	ForwardList& operator=(ForwardList&& lst);

	void push_front(const T& data);
	void remove_after(Node* node);
	void insert_after(Node* node, const T& data);
	void clear();
	int get_size()const;

	inline Node* before_begin() {
		return &head;
	}
	inline const Node* before_begin()const {
		return &head;
	}
	inline Node* begin() {
		return head.next;
	}
	inline const Node* begin()const {
		return head.next;
	}
	inline bool empty()const {
		return head.next == nullptr;
	}

	//返回一个删除了指定节点的副本
	ForwardList remove_copy(const Node* node)const;

	void show()const;
private:
	//附加头结点
	Node* copy_nodes()const;
};


template<typename T>
ForwardList<T>::ForwardList(const ForwardList& lst)
{
	head.next = lst.copy_nodes();
}

template<typename T>
ForwardList<T>::ForwardList(ForwardList&& lst)
{
	head.next = lst.head.next;
	lst.head.next = nullptr;
}

template<typename T>
ForwardList<T>::~ForwardList()
{
	clear();
}

template<typename T>
ForwardList<T>& ForwardList<T>::operator=(const ForwardList& lst)
{
	head.next = lst.copy_nodes();
	return *this;
}

template<typename T>
ForwardList<T>& ForwardList<T>::operator=(ForwardList&& lst)
{
	head.next = lst.head.next;
	lst.head.next = nullptr;
	return *this;
}

template<typename T>
void ForwardList<T>::push_front(const T& data)
{
	auto* node = new Node(data);
	node->next = head.next;
	head.next = node;
}

template<typename T>
void ForwardList<T>::remove_after(Node* node)
{
	if (!node || !(node->next))
		return;
	auto* tmp = node->next;
	node->next = node->next->next;
	delete tmp;
}

template<typename T>
void ForwardList<T>::insert_after(Node* node, const T& data)
{
	if (!node)
		return;
	auto* n = new Node(data);
	n->next = node->next;
	node->next = n;
}

template<typename T>
void ForwardList<T>::clear()
{
	if (!head.next)
		return;
	auto* p0 = head.next;
	auto* p1 = head.next->next;
	while (p1) {
		p1 = p1->next;
		delete p0;
		p0 = p1;
	}
	delete p0;
	head.next = nullptr;
}

template<typename T>
int ForwardList<T>::get_size() const
{
	auto* p = head.next;
	int n = 0;
	for (; p; p = p->next)
		n++;
	return n;
}

template<typename T>
ForwardList<T> ForwardList<T>::remove_copy(const Node* node)const
{
	ForwardList<T> res;
	Node* n = &(res.head);

	auto* p = head.next;

	for (; p; p = p->next) {
		if (p == node) {
			p = p->next;
			if (!p)break;
		}
		n->next = new Node(p->data);
		n = n->next;
	}
	return res;
}

template<typename T>
void ForwardList<T>::show() const
{
	std::cout << "[ ";
	for (auto* p = head.next; p; p = p->next) {
		std::cout << p->data << ", ";
	}
	std::cout << " ]" << std::endl;
}

//不包含头结点
template<typename T>
typename ForwardList<T>::Node* ForwardList<T>::copy_nodes() const
{
	if (!head.next)
		return nullptr;
	Node* n = new Node(head.next->data);
	Node* n0 = n;
	auto* p = head.next->next;
	for (; p; p = p->next) {
		n->next = new Node(p->data);
		n = n->next;
	}
	return n0;
}