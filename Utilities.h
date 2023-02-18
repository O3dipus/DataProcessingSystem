#include<vector>

std::vector<std::vector<int64_t>> orderedTableMerge(std::vector<std::pair<int64_t, int64_t>>& _table1,
	std::vector<std::pair<int64_t, int64_t>>& _table2,
	int _col1, int _col2) {
	int _p1 = 0;
	int _p2 = 0;
	std::vector<std::vector<int64_t>> _result = {};
	while (_p1 < _table1.size() && _p2 < _table2.size()) {
		int _val1 = _col1 == 0 ? _table1[_p1].first : _table1[_p1].second;
		int _val2 = _col2 == 0 ? _table2[_p2].first : _table2[_p2].second;
		if (_val1 < _val2) _p1++;
		else if (_val1 > _val2) _p2++;
		else {
			int _pt1 = _p1;
			int _pt2 = _p2;
			while (_pt1 < _table1.size()) {
				int _ptval1 = _col1 == 0 ? _table1[_pt1].first : _table1[_pt1].second;
				if (_ptval1 != _val1) break;
				_pt2 = _p2;
				while (_pt2 < _table2.size()) {
					int _ptval2 = _col2 == 0 ? _table2[_pt2].first : _table2[_pt2].second;
					if (_ptval2 != _val2) break;
					_result.push_back(std::vector<int64_t>({ _table1[_pt1].first, _table1[_pt1].second,
															 _table2[_pt2].first, _table2[_pt2].second }));
					_pt2++;
				}
				_pt1++;
			}
			_p1 = _pt1;
			_p2 = _pt2;
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
