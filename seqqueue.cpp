/* 
 * 数组实现固定大小的循环队列
 */
template <typename T>
class SeqQueue {
	T* data;
	int maxsize;
	int head, tail;
public:
	SeqQueue() = delete;
	SeqQueue(const SeqQueue&) = delete;
	SeqQueue(SeqQueue&&) = delete;
	SeqQueue(int maxsize_);
	SeqQueue& operator=(const SeqQueue&) = delete;
	~SeqQueue();
	void push(const T& t);
	void pop();   //必须保证非空
	const T& front()const;  //必须保证非空
	inline bool full()const {
		return tail == ((head + maxsize - 1) % maxsize);
	}
	inline bool empty()const {
		return head == tail;
	}
};


template<typename T>
SeqQueue<T>::SeqQueue(int maxsize_):maxsize(maxsize_),head(0),tail(0)
{
	data = new T[maxsize];
	assert(data);
}

template<typename T>
SeqQueue<T>::~SeqQueue()
{
	delete[] data;
	data = nullptr;
}

template<typename T>
void SeqQueue<T>::push(const T& t)
{
	if (full())
		throw "FULL";
	data[tail] = t;
	tail = (tail + 1) % maxsize;
}

template<typename T>
void SeqQueue<T>::pop()
{
	if (empty()) {
		throw "EMPTY";
	}
	head = (head + 1) % maxsize;
}

template<typename T>
const T& SeqQueue<T>::front() const
{
	if (empty())
		throw "EMPTY";
	return data[head];
}
