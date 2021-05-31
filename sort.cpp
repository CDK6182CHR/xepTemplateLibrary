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

template <typename T>
void __max_heap_down(T* x, const int n, int start)
{
	int i = start;   //指向当前根节点
	int j = 2 * start + 1;   //指向当前左子结点
	while (j < n) {
		if (j + 1 < n && x[j] < x[j + 1]) {
			j++;
		}
		//j指向子女结点中较大的那个
		if (x[i] < x[j]) {
			std::swap(x[i], x[j]);
			i = j;
			j = 2 * i + 1;
		}
		else 
			break;
	}
}


template <typename T>
void heap_sort(T* x, int left, int right)
{
	T* hp = x + left;
	int n = right - left;
	//初始化堆，从最后一个元素的父节点开始
	for (int i = (n - 2) / 2; i >= 0; i--) {
		__max_heap_down(hp, n, i);
	}
	//从最后一个元素开始，倒排
	for (int i = n - 1; i >= 0; i--) {
		std::swap(hp[0], hp[i]);
		__max_heap_down(hp, i, 0);
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
