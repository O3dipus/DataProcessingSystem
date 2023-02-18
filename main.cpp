#include <random>
#include <iostream>

#include "TableSort.h"
#include "Utilities.h"

int main() {
	TableSort ts = TableSort();

	std::vector<std::pair<int64_t, int64_t>> A = generateRandomTable(10, 16);
	std::vector<std::pair<int64_t, int64_t>> B = generateRandomTable(8, 16);
	std::cout << std::endl;
	std::cout << "Sorted A:" << std::endl;
	ts.tableQuickSort(A, 0, A.size() - 1, 0);
	for (int i = 0; i < A.size(); i++) {
		std::cout << A[i].first << " " << A[i].second << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Sorted B:" << std::endl;
	ts.tableQuickSort(B, 0, B.size() - 1, 1);
	for (int i = 0; i < B.size(); i++) {
		std::cout << B[i].first << " " << B[i].second << std::endl;
	}
	std::vector<std::vector<int64_t>> C = orderedTableMerge(A, B, 0, 1);

	for (int i = 0; i < C.size(); i++) {
		std::cout << C[i][0] << " " << C[i][1] << " " << C[i][2] << " " << C[i][3] << std::endl;
	}
}