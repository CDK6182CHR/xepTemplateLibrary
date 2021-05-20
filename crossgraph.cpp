#include <iostream>
#include <utility>

/// <summary>
/// 教材 p.362
/// 具有固定结点数的有向图的邻接多重表实现  或十字链表
/// 仍然采用int在内部表示结点
/// </summary>
/// <typeparam name="V"></typeparam>
/// <typeparam name="E"></typeparam>
template <typename V,typename E>
class CrossGraph {
public:
	//表示从v1到v2的有向边
	struct Edge {
		E edata;
		int v1, v2;
		Edge* path1;   //出边表
		Edge* path2;   //入边表
		Edge():edata(),v1(),v2(),path1(nullptr),path2(nullptr){}
		Edge(int v1_,int v2_,const E& data):edata(data),v1(v1_),v2(v2_),
			path1(nullptr),path2(nullptr){}
	};
	struct Vertex {
		V vdata;
		Edge* ined;   //入边表
		Edge* outed;   //出边表
		Vertex():vdata(),ined(nullptr),outed(nullptr){}
	};

	CrossGraph(int size_);
	~CrossGraph()noexcept;

	Edge* add_edge(int from, int to, const E& data);
	void add_biedge(int x, int y, const E& data);
	void remove_edge(int from, int to);  //需要时实现

	inline Edge* first_out_edge(int v) {
		return vertices[v].outed;
	}
	inline Edge* next_out_edge(Edge* e) {
		return e->path1;
	}

	Edge* first_in_edge(int v) {
		return vertices[v].ined;
	}

	Edge* next_in_edge(Edge* e) {
		return e->path2;
	}

	inline int get_size()const {
		return size;
	}

	CrossGraph(const CrossGraph&) = delete;
	CrossGraph(CrossGraph&&) = delete;
	CrossGraph& operator=(const CrossGraph&) = delete;
	CrossGraph& operator=(CrossGraph&&) = delete;

protected:
	int size;    //顶点数
	Vertex* vertices;
};

template<typename V, typename E>
CrossGraph<V, E>::CrossGraph(int size_):size(size_)
{
	vertices = new Vertex[size];
}

template<typename V, typename E>
CrossGraph<V, E>::~CrossGraph() noexcept
{
	//先按照出边顺序清除表
	for (int i = 0; i < size; i++) {
		auto e = vertices[i].outed;
		if (e) {
			auto e1 = e->path1;
			while (e1) {
				delete e;
				e = e1;
				e1 = e1->path1;
			}
		}
	}
	delete[] vertices;
	vertices = nullptr;
}

//注意 插入到各个边的链表的第一位
template<typename V, typename E>
typename CrossGraph<V,E>::Edge* 
	CrossGraph<V, E>::add_edge(int from, int to, const E& data)
{
	auto e = new Edge(from, to, data);
	e->path1 = vertices[from].outed;
	vertices[from].outed = e;
	e->path2 = vertices[to].ined;
	vertices[to].ined = e;
	return e;
}

template<typename V, typename E>
void CrossGraph<V, E>::add_biedge(int x, int y, const E& data)
{
	add_edge(x, y, data);
	add_edge(y, x, data);
}
