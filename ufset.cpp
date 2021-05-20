#include <iostream>
#include <utility>


/// <summary>
/// 具有固定长度的UFSet 数组实现
/// 暂时不考虑保存数据的问题 下标就是数据
/// </summary>
class UFSet {
	int* data;
	int size;
public:
	UFSet(int size_);
	~UFSet();
	UFSet(const UFSet&) = delete;
	UFSet(UFSet&& s)noexcept;
	UFSet& operator=(const UFSet&) = delete;
	UFSet& operator=(UFSet&& s)noexcept;

	int find(int x)const;
	int find(int x);    //带路径折叠
	int merge(int x, int y);
};

UFSet::UFSet(int size_):size(size_)
{
	data = new int[size];
	std::fill(data, data + size, -1);
}

UFSet::~UFSet()
{
	if (data)
		delete[] data;
	data = nullptr;
}

UFSet::UFSet(UFSet&& s) noexcept:
	data(s.data),size(s.size)
{
	s.size = 0;
	s.data = nullptr;
}

UFSet& UFSet::operator=(UFSet&& s) noexcept
{
	size = s.size;
	data = s.data;
	s.size = 0;
	s.data = nullptr;
	return *this;
}

int UFSet::find(int x) const
{
	while (data[x] >= 0)
		x = data[x];
	return x;
}

//把路径上所有的都折叠上去
int UFSet::find(int x)
{
	int root = static_cast<const UFSet*>(this)->find(x);
	while (data[x] >= 0) {
		x = data[x];
		data[x] = root;
	}
	return root;
}

//不负责折叠路径
//只把y合并到x
//返回新的根节点
int UFSet::merge(int x, int y)
{
	int rx = find(x);
	int ry = find(y);
	data[rx] += data[ry];
	data[ry] = rx;
	return rx;
}
