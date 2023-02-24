#include <random>
#include <iostream>
#include <chrono>

#include "TableSort.h"
#include "Utilities.h"


int main() {
	std::vector<std::pair<int64_t, int64_t>> A = generateRandomTable(256, 1024);
	std::vector<std::pair<int64_t, int64_t>> B = generateRandomTable(8192, 1024);
	auto start_1 = std::chrono::high_resolution_clock::now();
	std::vector<std::vector<int64_t>> C = loopTableMerge(A,B,0,1);
	auto finish_1 = std::chrono::high_resolution_clock::now();
	std::cout << "LoopJoin:" << std::chrono::duration_cast<std::chrono::nanoseconds>(finish_1-start_1).count() << '\n';
	// for (std::vector<int64_t> p: C){
	// 	for (int64_t n: p){
	// 		std::cout << n << ' ';
	// 	}
	// 	std::cout << " \n";
	// }
	std::cout << "*************************************" << std::endl;




	std::vector<std::pair<int64_t, int64_t>> input1 = A;
	std::vector<int64_t> phase1_a, phase1_d;
	std::vector<int64_t> firstResultColumn, secondResultColumn;
    for (int i = 0; i < B.size(); i++) {
        phase1_a.push_back(B[i].first);
        phase1_d.push_back(B[i].second);
    }

	auto start_2 = std::chrono::high_resolution_clock::now();
	// start
    std::vector<std::pair<int64_t,int64_t>> hash_table(16417, std::pair<int64_t, int64_t>());
    for (int i = 0; i < input1.size(); i++) {
        int64_t hash_val = hash_function(input1[i].first);
        while (hash_table[hash_val] != std::pair<int64_t, int64_t>()){
          hash_val = hash_function(hash_val);
        }
        hash_table[hash_val] = input1[i];
    }
    for (int j = 0; j < phase1_d.size(); j++) { 
        int64_t test_val = phase1_d[j];
        int64_t probe_val = hash_function(test_val);
        while (hash_table[probe_val] != std::pair<int64_t, int64_t>()){
          if (hash_table[probe_val].first == test_val){
            firstResultColumn.push_back(phase1_a[j]);
            secondResultColumn.push_back(hash_table[probe_val].second);
		  }
		  probe_val = hash_function(probe_val);
        }
    }
	// end
	auto finish_2 = std::chrono::high_resolution_clock::now();
	std::cout << "HashJoin:" << std::chrono::duration_cast<std::chrono::nanoseconds>(finish_2-start_2).count() << '\n';
	// for (int i = 0; i < firstResultColumn.size(); i++){
	// 	std::cout << secondResultColumn[i] << " " << firstResultColumn[i] << '\n';
	// }
}