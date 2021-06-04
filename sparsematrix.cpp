#include <iostream>

/// <summary>
/// 固定数组大小的SparseMatrix
/// 采用三元组的标准实现
/// </summary>
template <typename T>
class SparseMatrix {
	struct Item {
		int row, col;
		T data;
		Item() = default;
		Item(int row_,int col_,T data_):row(row_),col(col_),data(data_){}
		inline void set(int row_, int col_, T data_) {
			row = row_, col = col_, data = data_;
		}
	};
	const int nrow, ncol;
	Item* terms;
	int nterm;
public:
	SparseMatrix(int nrow, int ncol);
	~SparseMatrix();

	SparseMatrix(const SparseMatrix<T>&) = delete;
	SparseMatrix(SparseMatrix<T>&&);

	SparseMatrix<T>& operator=(const SparseMatrix<T>&) = delete;
	SparseMatrix<T>& operator=(SparseMatrix<T>&&) = delete;

	void append(int row, int col, T data);
	inline const Item& getTerm(int t)const {
		return terms[t];
	}
	SparseMatrix<T> operator*(const SparseMatrix<T>& m)const;

	template <typename R>
	friend std::ostream& operator<<(std::ostream& out, const SparseMatrix<R>& m);
};

template<typename T>
SparseMatrix<T>::SparseMatrix(int nrow_, int ncol_):
	nrow(nrow_),ncol(ncol_),nterm(0)
{
	terms = new Item[MAX_TERMS];
}

template<typename T>
SparseMatrix<T>::~SparseMatrix()
{
	if (terms) {
		delete[] terms;
		terms = nullptr;
	}
}
	

template<typename T>
SparseMatrix<T>::SparseMatrix(SparseMatrix<T>&& m):
	nrow(m.nrow),ncol(m.ncol),nterm(m.nterm),terms(m.terms)
{
	m.terms = nullptr;
}

template<typename T>
void SparseMatrix<T>::append(int row, int col, T data)
{
	terms[nterm++].set(row, col, data);
}

template<typename T>
SparseMatrix<T> SparseMatrix<T>::operator*(const SparseMatrix<T>& m) const
{
	if (ncol != m.nrow)
		throw "INVALID SIZES";
	SparseMatrix<T> res(nrow, m.ncol);

	int* rowSize = new int[m.nrow];
	int* rowStart = new int[m.nrow + 1];   //多一个——结束位置

	for (int i = 0; i < m.nrow; i++) {
		rowSize[i] = 0;
	}
	for (int i = 0; i < m.nterm; i++) {
		rowSize[m.getTerm(i).row]++;
	}
	rowStart[0] = 0;

	for (int i = 1; i < m.nrow+1; i++) {
		rowStart[i] = rowStart[i - 1] + rowSize[i - 1];
	}

	//正式开始做乘法...
	int* ss = new int[m.ncol];
	clear(ss, m.ncol);

	int arow = 0;  //当前A所在的行数
	for (int i = 0; i < nterm; i++) {
		const auto& p = terms[i];
		if (p.row != arow) {
			//扫描到了新的一行。先把旧的复制了
			for (int t = 0; t < m.ncol; t++) {
				if (ss[t]) {
					res.append(arow, t, ss[t]);
					ss[t] = 0;
				}
			}
			arow = p.row;
		}
		//对B中所有在指定行的
		for (int t = rowStart[p.col]; t < rowStart[p.col + 1]; t++) {
			const auto& q = m.terms[t];
			ss[q.col] += p.data * q.data;
		}
	}
	//对最后一行的补充处理
	for (int t = 0; t < m.ncol; t++) {
		if (ss[t]) {
			res.append(arow, t, ss[t]);
			ss[t] = 0;
		}
	}
	return res;
}

template <typename T>
std::ostream&  operator<<(std::ostream& out, const SparseMatrix<T>& m)
{
	int t = 0;  //下一个要输出的term
	for (int i = 0; i < m.nrow; i++) {
		for (int j = 0; j < m.ncol; j++) {
			if (t<m.nterm && i == m.getTerm(t).row && j == m.getTerm(t).col) {
				out << m.getTerm(t++).data << ' ';
			}
			else {
				out << "0 ";
			}
		}
		out << std::endl;
	}
	return out;
}
