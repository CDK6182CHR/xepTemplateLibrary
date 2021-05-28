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
	quick_sort(x, 0, N);
	for (int i = 0; i < N; i++) {
		cout << x[i] << ' ';
	}
	cout << endl;
	return 0;
}
