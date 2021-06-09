#include <iostream>
#include <utility>
#include <algorithm>


/// <summary>
/// 教材 p.362
/// 具有固定结点数的无向图的邻接多重表实现  或十字链表
/// 仍然采用int在内部表示结点
/// </summary>
template <typename V, typename E>
class UniGraph {
public:
	//表示从v1到v2的有向边
	struct Edge {
		E edata;
		int v1, v2;
		Edge* path1;   //出边表
		Edge* path2;   //入边表
		Edge() :edata(), v1(), v2(), path1(nullptr), path2(nullptr) {}
		Edge(int v1_, int v2_, const E& data) :edata(data), v1(v1_), v2(v2_),
			path1(nullptr), path2(nullptr) {}
	};
	struct Vertex {
		V vdata;
		Edge* outed;   //出边表
		Vertex() :vdata(), outed(nullptr) {}
	};

	UniGraph(int size_);
	~UniGraph()noexcept;

	Edge* add_edge(int from, int to, const E& data);
	void remove_edge(int from, int to);  //需要时实现

	inline const Edge* first_edge(int v)const {
		return vertices[v].outed;
	}
	inline Edge* first_edge(int v) {
		return vertices[v].outed;
	}

	Edge* next_edge(int v, const Edge* e);

	const Edge* next_edge(int v, const Edge* e)const {
		return const_cast<UniGraph*>(this)->next_edge(v, e);
	}

	inline int get_edge_neighbour(int u, const Edge* e)const {
		return u == e->v1 ? e->v2 : e->v1;
	}

	inline int get_size()const {
		return size;
	}

	Edge* get_edge(int u, int v);

	UniGraph(const UniGraph&) = delete;
	UniGraph(UniGraph&&) = delete;
	UniGraph& operator=(const UniGraph&) = delete;
	UniGraph& operator=(UniGraph&&) = delete;

protected:
	int size;    //顶点数
	Vertex* vertices;

	//path1或者path2指针域
	Edge*& next_edge_ref(int v, Edge* e);

	//来向的指针
	Edge* previous_edge(int v, Edge* e);
};

template<typename V, typename E>
UniGraph<V, E>::UniGraph(int size_) :size(size_)
{
	vertices = new Vertex[size];
}

template<typename V, typename E>
UniGraph<V, E>::~UniGraph() noexcept
{
	//先按照出边顺序清除表
	for (int i = 0; i < size; i++) {
		Edge* e = vertices[i].outed;
		if (e) {
			auto e1 = next_edge(i, e);
			while (e1) {
				if (e->v2 >= 0) {
					//用v2为-1作为已经访问过一次的判据
					//注意这个强烈依赖于判定next_edge时仅用了v1!!
					e->v2 = -1;
				}
				else {
					delete e;
				}

				e = e1;
				e1 = next_edge(i, e1);
			}
		}
	}
	delete[] vertices;
	vertices = nullptr;
}

//注意 插入到各个边的链表的第一位
template<typename V, typename E>
typename UniGraph<V, E>::Edge*
UniGraph<V, E>::add_edge(int from, int to, const E& data)
{
	auto e = new Edge(from, to, data);
	e->path1 = vertices[from].outed;
	vertices[from].outed = e;
	e->path2 = vertices[to].outed;
	vertices[to].outed = e;
	return e;
}

template<typename V, typename E>
typename UniGraph<V, E>::Edge* UniGraph<V, E>::next_edge(int v, const Edge* e)
{
	if (v == e->v1) {
		return e->path1;
	}
	else return e->path2;
}

template<typename V, typename E>
typename UniGraph<V, E>::Edge*& UniGraph<V, E>::next_edge_ref(int v, Edge* e)
{
	if (v == e->v1) {
		return e->path1;
	}
	else return e->path2;
}

template<typename V, typename E>
typename UniGraph<V, E>::Edge* UniGraph<V, E>::previous_edge(int v, Edge* e)
{
	if (v == e->v1) {
		return e->path2;
	}
	else return e->path1;
}

template<typename V, typename E>
typename UniGraph<V, E>::Edge* UniGraph<V, E>::get_edge(int u, int v)
{
	auto* e = first_edge(u);
	for (; e; e = next_edge(u, e)) {
		if ((e->v1 == v && e->v2 == u) || (e->v2 == v && e->v1 == u)) {
			return e;
		}
	}
	return nullptr;
}

