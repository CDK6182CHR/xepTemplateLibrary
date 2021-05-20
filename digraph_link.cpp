#include <iostream>
#include <cstdio>
#include <utility>

/// <summary>
/// 有向图的邻接表表示。暂时只考虑插入的情况。
/// 只维护出边而不维护入边。
/// </summary>
/// <typeparam name="V"></typeparam>
/// <typeparam name="E"></typeparam>
template <typename V,typename E>
class Graph {
public:
	struct Edge;
	struct Vertex {
		V vdata;
		Edge* adj;
		Vertex():vdata(),adj(nullptr){}
	};
	struct Edge {
		E edata;
		int to;
		Edge* link;
		Edge(int to_) :edata(), to(to_), link(nullptr){}
		Edge(int to_,const E& data):edata(data),to(to_),link(nullptr){}
	};

	Graph(int size_);
	~Graph();
	
	Edge* add_edge(int from, int to, const E& data);
	//添加双向边
	void add_biedge(int a, int b, const E& data);

	Edge* first_edge(int v);

	inline int get_size()const {
		return size;
	}

	Graph(const Graph&) = delete;
	Graph(Graph&&) = delete;
	Graph& operator=(const Graph&) = delete;
	Graph& operator=(Graph&&) = delete;

protected:
	int size;
	Vertex* vertices;
};

template<typename V, typename E>
Graph<V, E>::Graph(int size_):size(size_)
{
	vertices = new Vertex[size];
}

template<typename V, typename E>
Graph<V, E>::~Graph()
{
	for (int i = 0; i < size; i++) {
		Edge* t = vertices[i].adj;
		if (t) {
			Edge* t1 = t->link;
			while (t1) {
				delete t;
				t = t1;
				t1 = t1->link;
			}
		}
		vertices[i].adj = nullptr;
	}
	delete[] vertices;
	vertices = nullptr;
}

// 注意 这是插入到第一个
template<typename V, typename E>
typename Graph<V,E>::Edge* Graph<V, E>::add_edge(int from, int to, const E& data)
{
	Edge* e = new Edge(to, data);
	e->link = vertices[from].adj;
	vertices[from].adj = e;
	return e;
}

template<typename V, typename E>
void Graph<V, E>::add_biedge(int a, int b, const E& data)
{
	add_edge(a, b, data);
	add_edge(b, a, data);
}

template<typename V, typename E>
typename Graph<V,E>::Edge* Graph<V, E>::first_edge(int v)
{
	return vertices[v].adj;
}