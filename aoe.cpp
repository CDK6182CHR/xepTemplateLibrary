/**
 * AOE网络原型实现
 * 基于有向图的邻接表表示
 * 计算最早开始时间Ve，最晚开始时间Vl
 */ 
#include <iostream>
#include <cstdio>
#include <utility>
#include <algorithm>

/// <summary>
/// 有向图的邻接表表示。暂时只考虑插入的情况。
/// 只维护出边而不维护入边。
/// </summary>
/// <typeparam name="V"></typeparam>
/// <typeparam name="E"></typeparam>
template <typename V, typename E>
class Graph {
public:
	struct Edge;
	struct Vertex {
		V vdata;
		Edge* adj;
		Vertex() :vdata(), adj(nullptr) {}
	};
	struct Edge {
		E edata;
		int to;
		Edge* link;
		Edge(int to_) :edata(), to(to_), link(nullptr) {}
		Edge(int to_, const E& data) :edata(data), to(to_), link(nullptr) {}
	};

	Graph(int size_);
	~Graph();

	Edge* add_edge(int from, int to, const E& data);
	//添加双向边
	void add_biedge(int a, int b, const E& data);

	Edge* first_edge(int v);
	inline const Edge* first_edge(int v)const {
		return const_cast<Graph*>(this)->first_edge(v);
	}

	inline int get_size()const {
		return size;
	}

	void get_indegrees(int* indeg)const;

	Graph(const Graph&) = delete;
	Graph(Graph&&) = delete;
	Graph& operator=(const Graph&) = delete;
	Graph& operator=(Graph&&) = delete;

protected:
	int size;
	Vertex* vertices;
};

template<typename V, typename E>
Graph<V, E>::Graph(int size_) :size(size_)
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
typename Graph<V, E>::Edge* Graph<V, E>::add_edge(int from, int to, const E& data)
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
typename Graph<V, E>::Edge* Graph<V, E>::first_edge(int v)
{
	return vertices[v].adj;
}

template<typename V, typename E>
void Graph<V, E>::get_indegrees(int* indeg) const
{
	std::fill(indeg, indeg + size, 0);
	for (int i = 0; i < size; i++) {
		auto* e = vertices[i].adj;
		while (e) {
			indeg[e->to]++;
			e = e->link;
		}
	}
}

/// <summary>
/// precondition:  indeg[source] == 0
/// i.e. all vertices `before` source has been processed.
/// </summary>
/// <typeparam name="E">used as weight</typeparam>
/// <param name="ae">the earliest start time of source node</param>
/// <param name="ve">earliest finish time</param>
/// <param name="vl">latest finish time</param>
/// <param name="flags">DFS标志位 0 1 2 </param>
template <typename V,typename E>
void __aoe_ve_rec(const Graph<V, E>& graph, int source,E* ve, int* indeg)
{
	//int top;   //used as pseudo-stack top pointer
	////pseudo-pop
	//top = indeg[source];
	auto e = graph.first_edge(source);
	E vei = ve[source];

	for (; e; e = e->link) {
		int j = e->to;
		E vej = vei + e->edata;

		if (indeg[j] == 0) {
			//访问到了indeg已经是0的，也就是已经搞定的结点
			continue;
		}

		indeg[j]--;
		if (vej > ve[j]) {
			ve[j] = vej;
		}
		if (indeg[j] == 0) {
			////pseudo-push
			//indeg[j] = top; top = j;
			//好像不用压栈，直接递归就好？
			//这里递归之前并没有处理完毕indeg，不知道有无影响？
			__aoe_ve_rec(graph, j,  ve, indeg);
		}
	}
}

// 返回：source结点的vl
template <typename V,typename E>
void __aoe_vl_dfs(const Graph<V, E>& graph, int source, E* ve, E* vl)
{
	auto* e = graph.first_edge(source);
	E vli;
	if (!e) {
		vli = ve[source];
	}
	else {
		vli = (E)INFINITY;
		for (; e; e = e->link) {
			int j = e->to;
			if (vl[j] == (E)(-1)) {
				__aoe_vl_dfs(graph, j, ve, vl);   //recurse!!
			}
			E vlj = vl[j] - e->edata;
			if (vlj < vli)
				vli = vlj;
		}
	}
	vl[source] = vli;
}


/// <summary>
/// 2021.05.24
/// 教材 8.7 AOE网络算法的原型实现
/// 计算各节点（事件）的最早开始时间ve，最晚开始时间vl，以及判定关键路径。
/// precondition: 只有唯一源点，由source给定；只有唯一汇点，不必给出。
/// 不要求事先实行topo排序。
/// 采用递归算法。
/// </summary>
template <typename V,typename E>
void aoe_protype(const Graph<V, E>& graph, int source)
{
	int n = graph.get_size();
	E* ve = new E[n], * vl = new E[n];
	std::fill(ve, ve + n, (E)0);
	std::fill(vl, vl + n, (E)(-1));

	int* indeg = new int[n];   //各个点的入度表，兼做topo排序的stack
	graph.get_indegrees(indeg);

	if (indeg[source] != 0) {
		std::cout << "INVALID source node!!!" << std::endl;
		return;
	}
	for (int i = 0; i < n; i++) {
		ve[i] = (E)0;
	}
	__aoe_ve_rec(graph, source, ve, indeg);

	if (std::find_if(indeg, indeg + n, [](int x) {return x != 0; }) != indeg + n) {
		std::cout << "NOT DAG!!!" << std::endl;
		return;
	}

	__aoe_vl_dfs(graph, source, ve, vl);

	for (int i = 0; i < n; i++) {
		std::cout << "node: " << i << ", Ve: " << ve[i] << ", Vl: " << vl[i] << std::endl;
	}
}


#include <initializer_list>
#include <tuple>
using CGraph = Graph<int, double>;
using namespace std;


void test1()
{
	//test case 1: 教材 p. 388 图8.30
	CGraph g(9);
	std::initializer_list<std::tuple<int, int, double>> s{
		{0,1,6},
		{0,2,4},
		{0,3,5},
		{1,4,1},
		{2,4,1},
		{3,5,2},
		{4,6,9},
		{4,7,7},
		{5,7,4},
		{6,8,2},
		{7,8,4},
		//{6,1,10}  //for test circle!!
	};
	for (const auto& t : s) {
		g.add_edge(get<0>(t), get<1>(t), get<2>(t));
	}
	aoe_protype(g, 0);
}

void test2()
{
	//test case 2: 课件 p.129
	CGraph g(8);
	std::initializer_list<std::tuple<int, int, double>> s{
		{1,2,8},
		{1,3,12},
		{2,4,14},
		{3,4,10},
		{3,6,28},
		{4,6,8},
		{4,5,6},
		{5,7,18},
		{6,7,6},
		{7,8,12}
	};
	for (const auto& t : s) {
		g.add_edge(get<0>(t)-1, get<1>(t)-1, get<2>(t));
	}
	aoe_protype(g, 0);
}

int main()
{
	test2();
}