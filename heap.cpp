#include <iostream>
#include <initializer_list>


/// <summary>
/// 具有固定最大长度的最小堆
/// 已经测试对存在相同数据的应该没问题
/// </summary>
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
	inline bool empty()const {
		return size == 0;
	}
private:
	void init_heap();   //这个没必要写

	void sift_down(int start);
	void sift_up(int start);


};

template<typename T>
MinHeap<T>::MinHeap(int size_) :max_size(size_), size(0)
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
void MinHeap<T>::push(const T& t)
{
	if (size >= max_size)
		throw "OVERFLOWED";
	data[size++] = t;
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