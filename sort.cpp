#include <iostream>
#include <utility>

/// <summary>
/// 快速排序标准实现
/// 排序范围为 [left, right)
/// 利用 operator lt
/// </summary>
template <typename T>
void quick_sort(T* x, int left, int right)
{
	if (right - left <= 1)
		return;
	T pivot = x[left];   //copy construct
	int p = left;
	for (int i = left + 1; i < right; i++) {
		if (x[i] < pivot) {
			p++;
			std::swap(x[p], x[i]);
		}
	}
	std::swap(x[left], x[p]);
	quick_sort(x, left, p);
	quick_sort(x, p + 1, right);
}

/// <summary>
/// 直接插入排序法标准实现，用来解决接近有序序列的情况
/// </summary>
template <typename T>
void insert_sort(T x[], int left, int right)
{
	for (int i = left + 1; i < right; i++) {
		if (x[i] < x[i - 1]) {
			T tmp = std::move(x[i]);
			int j = i-1;
			while (j >= 0 && tmp < x[j]) {
				x[j + 1] = std::move(x[j]);
				j--;
			}
			//j最终指向要插入的前一个；j+1是空位
			x[j + 1] = std::move(tmp);
		}
	}
}


#include <algorithm>
#include <time.h>
#include <cstdlib>
using namespace std;

int main()
{
	constexpr int N = 100;
	int x[N];
	srand(time(NULL));
	for (int i = 0; i < N; i++) {
		x[i] = rand() % 1000;
	}
	insert_sort(x, 0, N);
	for (int i = 0; i < N; i++) {
		cout << x[i] << ' ';
	}
	cout << endl;
	cout << std::is_sorted(x, x + N) << endl;
	return 0;
}
