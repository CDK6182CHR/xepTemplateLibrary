#include <iostream>
#include <utility>

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
Matrix<T>::Matrix(int row_, int col_):row(row_),col(col_)
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