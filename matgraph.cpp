/**
 * 使用邻接矩阵表示的有向图。
 */ 
#include <iostream>
#include <utility>
#include <limits>

/**
 * 二维数组的简单封装，使用连续空间，运算下标
 */
template <typename T>
class Matrix {
	int row, col;
	T* data;
public:
	Matrix(int row_, int col_);
	~Matrix();
	T& get(int i, int j);
	T* get_pointer(int i, int j);
	inline T* get_pointer_serial(int n) {
		return data + n;
	}
	inline const T& get(int i, int j)const {
		return const_cast<Matrix*>(this)->get(i, j);
	}
	inline int get_row()const {
		return row;
	}
	inline int get_col()const {
		return col;
	}
	void fill(const T& d);

	Matrix(const Matrix&) = delete;
	Matrix(Matrix&&) = delete;
	Matrix& operator=(const Matrix&) = delete;
	Matrix& operator=(Matrix&&) = delete;

	void show()const;
};

template<typename T>
Matrix<T>::Matrix(int row_, int col_) :row(row_), col(col_)
{
	data = new T[row * col];
}

template<typename T>
Matrix<T>::~Matrix()
{
	delete[] data;
	data = nullptr;
}

template<typename T>
T& Matrix<T>::get(int i, int j)
{
	return data[i * col + j];
}

template<typename T>
T* Matrix<T>::get_pointer(int i, int j)
{
	return data + i * col + j;
}

template<typename T>
void Matrix<T>::fill(const T& d)
{
	std::fill(data, data + row * col, d);
}

template<typename T>
void Matrix<T>::show() const
{
	std::cout << '[' << std::endl;
	for (int i = 0; i < row; i++) {
		std::cout << ' ';
		for (int j = 0; j < col; j++) {
			std::cout << get(i, j) << ' ';
		}
		std::cout << std::endl;
	}
	std::cout << ']' << std::endl;
}


/// <summary>
/// 2021.06.03
/// 邻接矩阵表示的图
/// </summary>
template <typename E>
class MatGraph {
	Matrix<E> mat;
	int size;
public:
	static constexpr E MAX = std::numeric_limits<E>::max()-1;
	MatGraph(int size_);
	
	//冲突时只保留最短边
	void add_edge(int u, int v, const E& data);
	void add_biedge(int u, int v, const E& data);

	inline int get_edata(int u, int v)const {
		return mat.get(u, v);
	}

	//-1表示不存在
	int first_neigh(int u)const;
	int next_neigh(int u, int v)const;

	inline int get_size()const {
		return size;
	}
};

template<typename E>
MatGraph<E>::MatGraph(int size_): size(size_), mat(size_, size_) 
{
	mat.fill(MAX);
	for (int i = 0; i < size; i++) {
		mat.get(i, i) = 0;
	}
}

template<typename E>
void MatGraph<E>::add_edge(int u, int v, const E& data)
{
	mat.get(u, v) = std::min(mat.get(u, v), data);
}

template<typename E>
void MatGraph<E>::add_biedge(int u, int v, const E& data)
{
	add_edge(u, v, data);
	add_edge(v, u, data);
}

template<typename E>
int MatGraph<E>::first_neigh(int u) const
{
	for (int j = 0; j < size; j++) {
		if (mat.get(u, j) < MAX)
			return j;
	}
	return -1;
}

template<typename E>
int MatGraph<E>::next_neigh(int u, int v) const
{
	if (v == -1)
		return -1;
	for (int j = v + 1; j < size; j++) {
		if (mat.get(u, j) < MAX)
			return j;
	}
	return -1;
}

using G = MatGraph<int16_t>;


inline int get_min(const bool* si, const int* dist, const int n) {
	int mi = -1, ms = G::MAX;
	for (int i = 0; i < n; i++) {
		if (!si[i]) {
			if (dist[i] < ms) {
				ms = dist[i];
				mi = i;
			}
		}
	}
	return mi;
}


/**
 * 基于邻接矩阵的Dijkstra算法的实现
 */ 
int sssp(const G& graph)
{

	int n = graph.get_size();
	bool* si = new bool[n];
	int* dist = new int[n];
	std::fill(si, si + n, false);
	std::fill(dist, dist + n, G::MAX);

	dist[0] = 0;

	for (int cnt = 0; cnt < n; cnt++) {
		int mi = get_min(si, dist, n);
		if (mi == -1 || dist[mi] == G::MAX) {
			//没有可达的了，再见
			break;
		}
		si[mi] = true;
		int mj = graph.first_neigh(mi);
		for (; mj!=-1; mj=graph.next_neigh(mi,mj)) {
			dist[mj] = std::min(dist[mj], dist[mi] + graph.get_edata(mi,mj));
		}
	}

	if (dist[n - 1] == G::MAX)
		return -1;
	else return dist[n - 1];
}
