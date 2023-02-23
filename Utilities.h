#include<vector>

std::vector<std::vector<int64_t>> orderedTableMerge(std::vector<std::pair<int64_t, int64_t>>& _table1,
	std::vector<std::pair<int64_t, int64_t>>& _table2,
	bool _table1Column, bool _table2Column) {
	
	std::vector<std::vector<int64_t>> _result = {};
	int _table1Pos = 0;
	int _table2Pos = 0;
	while (_table1Pos < _table1.size() && _table2Pos < _table2.size()) {
		int _table1Val = _table1Column ? _table1[_table1Pos].second : _table1[_table1Pos].first;
		int _table2Val = _table2Column ? _table2[_table2Pos].second : _table2[_table2Pos].first;
		//std::cout << "1: " << _table1Pos << ", " << _table1Val << "\n";
		//std::cout << "!: " << _table1Val << ", " << _table2Val << "\n";
		if (_table1Val < _table2Val){
			_table1Pos++;
		} else if (_table1Val > _table2Val) {
			_table2Pos++;
		} else {
			//std::cout << "Hey" << "\n";
			int _table1Check = _table1Pos;
			int _table2Check = _table2Pos;
			while ( _table1Check < _table1.size() &&
					(_table1Column ? _table1[_table1Check].second : _table1[_table1Check].first) == _table1Val) {
				_table2Check = _table2Pos;
				while ( _table2Check < _table2.size() &&
						(_table2Column ? _table2[_table2Check].second : _table2[_table2Check].first) == _table2Val) {
					_result.push_back(std::vector<int64_t>({ _table1[_table1Check].first, _table1[_table1Check].second,
															 _table2[_table2Check].first, _table2[_table2Check].second }));
					_table2Check++;
				}
				_table1Check++;
			}
			_table1Pos = _table1Check;
			_table2Pos = _table2Check;
		}
	}
	return _result;
}

std::vector<std::vector<int64_t>> loopTableMerge(std::vector<std::pair<int64_t, int64_t>>& _table1,
	std::vector<std::pair<int64_t, int64_t>>& _table2,
	bool _col1, bool _col2) {
	std::vector<std::vector<int64_t>> _result = {};
	for (int i=0; i<_table1.size(); i++){
		for (int j=0; j<_table2.size(); j++){
			int _val1 = _col1 ? _table1[i].second : _table1[i].first;
			int _val2 = _col2 ? _table2[j].second : _table2[j].first;
			if (_val1 == _val2){
				_result.push_back(std::vector<int64_t>({ _table1[i].first, _table1[i].second,
															 _table2[j].first, _table2[j].second }));
			}
		}
	}
	return _result;
}


std::vector<std::pair<int64_t, int64_t>> generateRandomTable(int _size, int _range) {
	std::vector<std::pair<int64_t, int64_t>> A = std::vector<std::pair<int64_t, int64_t>>();
	std::default_random_engine generator;
	std::uniform_int_distribution<int64_t> distribution(1, _range);
	std::cout << "*************************************" << std::endl;
	for (int i = 0; i < _size; i++) {
		int r1 = distribution(generator);
		int r2 = distribution(generator);
		std::cout << r1 << " " << r2 << std::endl;
		A.push_back(std::pair<int64_t, int64_t>(r1, r2));
	}
	std::cout << "*************************************" << std::endl;
	return A;
}
