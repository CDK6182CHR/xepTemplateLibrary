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
	inline T& get_serial(int n) {
		return *(data + n);
	}
	inline int get_size()const {
		return row * col;
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
	inline int index_to_serial(int i, int j)const {
		return i * col + j;
	}
	void index_to_rowcol(int n, int& i, int& j)const;
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
void Matrix<T>::index_to_rowcol(int n, int& i, int& j) const
{
	j = n % col;
	i = n / col;   //int div
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


//以下：四邻域判定

using MI = Matrix<int>;
//using MD = Matrix<std::array<int, 4>>;
using namespace std;


//方向指标：0,1,2,3  上下左右

inline void get_dir(int dir, int& dx, int& dy) {
	switch (dir) {
	case 0:dx = 0; dy = 1; return;
	case 1:dx = 0, dy = -1; return;
	case 2:dx = -1; dy = 0; return;
	case 3:dx = 1; dy = 0; return;
	default:return;
	}
}

inline bool valid(const MI& mat, int si, int sj) {
	return 0 <= si && si < mat.get_row() && 0 <= sj && sj < mat.get_col();
}