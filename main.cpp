#include <random>
#include <iostream>
#include <chrono>

#include "TableSort.h"
#include "Utilities.h"

int main() {
	TableSort ts = TableSort();

	std::vector<std::pair<int64_t, int64_t>> A = generateRandomTable(56, 1024);
	std::vector<std::pair<int64_t, int64_t>> B = generateRandomTable(56, 1024);

	// auto start_1 = std::chrono::high_resolution_clock::now();
	// ts.tableQuickSort(A, 0, A.size() - 1, 0);
	// auto finish_1 = std::chrono::high_resolution_clock::now();
	// std::cout << "Original:" << std::chrono::duration_cast<std::chrono::nanoseconds>(finish_1-start_1).count() << '\n';	
	// // for (std::pair<int64_t, int64_t> p: A)
	// // 	std::cout << p.first << ' ' << p.second << '\n';
	// // std::cout << "*************************************" << std::endl;

	// auto start_2 = std::chrono::high_resolution_clock::now();
	// ts.inPlaceQuick(A_copy, 0, A_copy.size()-1, 0);
	// auto finish_2 = std::chrono::high_resolution_clock::now();
	// std::cout << "Mine:" << std::chrono::duration_cast<std::chrono::nanoseconds>(finish_2-start_2).count() << '\n';
	// // for (std::pair<int64_t, int64_t> p: A_copy)
	// // 	std::cout << p.first << ' ' << p.second << '\n';
	// // std::cout << "*************************************" << std::endl;

	auto start_2 = std::chrono::high_resolution_clock::now();
	std::vector<std::vector<int64_t>> C_copy = loopTableMerge(A, B, 0, 1);
	auto finish_2 = std::chrono::high_resolution_clock::now();
	std::cout << "LoopJoin:" << std::chrono::duration_cast<std::chrono::nanoseconds>(finish_2-start_2).count() << '\n';	
	for (std::vector<int64_t> p: C_copy){
		for (int64_t n: p){
			std::cout << n << ' ';
		}
		std::cout << " \n";
	}
	std::cout << "*************************************" << std::endl;


	auto start_1 = std::chrono::high_resolution_clock::now();
	ts.tableQuickSort(A, 0, A.size() - 1, 0);
	ts.tableQuickSort(B, 0, B.size() - 1, 1);
	std::vector<std::vector<int64_t>> C = orderedTableMerge(A, B, 0, 1);
	auto finish_1 = std::chrono::high_resolution_clock::now();
	std::cout << "SortJoin:" << std::chrono::duration_cast<std::chrono::nanoseconds>(finish_1-start_1).count() << '\n';	
	for (std::vector<int64_t> p: C){
		for (int64_t n: p){
			std::cout << n << ' ';
		}
		std::cout << " \n";
	}
	std::cout << "*************************************" << std::endl;


	


	// std::vector<std::pair<int64_t, int64_t>> B = generateRandomTable(8, 16);
	// std::cout << std::endl;
	// std::cout << "Sorted A:" << std::endl;
	// ts.tableQuickSort(A, 0, A.size() - 1, 0);
	// for (int i = 0; i < A.size(); i++) {
	// 	std::cout << A[i].first << " " << A[i].second << std::endl;
	// }
	// std::cout << std::endl;
	// std::cout << "Sorted B:" << std::endl;
	// ts.tableQuickSort(B, 0, B.size() - 1, 1);
	// for (int i = 0; i < B.size(); i++) {
	// 	std::cout << B[i].first << " " << B[i].second << std::endl;
	// }
	// std::vector<std::vector<int64_t>> C = orderedTableMerge(A, B, 0, 1);

	// for (int i = 0; i < C.size(); i++) {
	// 	std::cout << C[i][0] << " " << C[i][1] << " " << C[i][2] << " " << C[i][3] << std::endl;
	//}
}