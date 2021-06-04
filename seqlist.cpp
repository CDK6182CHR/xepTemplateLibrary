
/// <summary>
/// 2021.06.03
/// 具有固定运行时常量大小的数组
/// </summary>
template <typename T>
class SeqList {
	T* data;
	const int size;
public:
	SeqList(int sz) :size(sz) {
		data = new T[size];
	}
	SeqList(const SeqList& s) :size(s.size) {
		data = new T[size];
		std::copy(s.data, s.data + size, data);
	}
	SeqList(SeqList&& s) :size(s.size), data(s.data) {
		s.data = nullptr;
	}
	~SeqList() {
		if (data) {
			delete[] data;
			data = nullptr;
		}
	}
	inline T& operator[](int i) {
		return data[i];
	}
	inline const T& operator[](int i)const {
		return const_cast<SeqList*>(this)->operator[](i);
	}
	inline void fill(const T& t) {
		std::fill(data, data + size, t);
	}
	inline int get_size()const {
		return size;
	}
};

