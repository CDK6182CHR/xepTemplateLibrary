/// <summary>
/// 采用闭地址的Hash表，只允许插入和查找不允许删除。
/// 线性寻址。
/// </summary>
template <typename K, typename T>
class StaticHashMap {
	const int maxsize;
	int cursize;
	int m;   //取模的基底
public:
	struct HashNode {
		bool valid;
		K key;
		T value;
		HashNode() :valid(false), key(), value() {}
		HashNode(const K& k, const T& t) :valid(true), key(k), value(t) {}
	};
	StaticHashMap(int maxsize);
	~StaticHashMap();
	void insert(const K& k, const T& v);
	bool find(const K& k, T& t)const;

private:
	HashNode* nodes;
	int hash(K key)const;
};


template<typename K, typename T>
StaticHashMap<K, T>::StaticHashMap(int maxsize_) :
	maxsize(maxsize_), m(maxsize_), cursize(0)
{
	nodes = new HashNode[maxsize];
}

template<typename K, typename T>
StaticHashMap<K, T>::~StaticHashMap()
{
	if (nodes)
		delete[] nodes;
	nodes = nullptr;
}

template<typename K, typename T>
void StaticHashMap<K, T>::insert(const K& k, const T& v)
{
	if (cursize >= maxsize)
		throw "FULL";
	int t = hash(k);
	while (nodes[t].valid)
		t = (t + 1) % maxsize;
	nodes[t].key = k;
	nodes[t].value = v;
	nodes[t].valid = true;
	cursize++;
}

template<typename K, typename T>
bool StaticHashMap<K, T>::find(const K& k, T& t) const
{
	int p = hash(k);
	while (true) {
		if (!nodes[p].valid)
			return false;
		else if (nodes[p].key == k) {
			t = nodes[p].value;
			return true;
		}
		p = (p + 1) % maxsize;
	}
}

template<typename K, typename T>
int StaticHashMap<K, T>::hash(K key)const
{
	static_assert(sizeof(K) >= sizeof(unsigned), "The Key must no shorter than unigned!");
	return *(reinterpret_cast<unsigned*>(&key)) % m;
}