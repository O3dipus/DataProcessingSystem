#include<vector>

class TableSort {
public:
	void tableQuickSort(std::vector<std::pair<int64_t, int64_t>>& _table, int _l, int _r, int column) {
		if (_l >= _r || column > 1) return;
		int _flag = column == 0 ? _table[_l].first : _table[_l].second;
		int _pl = _l + 1;
		int _pr = _r;
		while (_pl <= _pr) {
			int _l_val = column == 0 ? _table[_pl].first : _table[_pl].second;
			while (_l_val > _flag && _pl <= _pr) {
				swapRow(_table, _pl, _pr);
				_pr--;
				_l_val = column == 0 ? _table[_pl].first : _table[_pl].second;
			}
			_pl++;
		}
		swapRow(_table, _l, _pr);
		tableQuickSort(_table, _l, _pr - 1, column);
		tableQuickSort(_table, _pr + 1, _r, column);
	}

	void swapRow(std::vector<std::pair<int64_t, int64_t>>& _table, int _index1, int _index2) {
		std::pair<int64_t, int64_t> temp = _table[_index1];
		_table[_index1] = std::pair<int64_t, int64_t>(_table[_index2]);
		_table[_index2] = std::pair<int64_t, int64_t>(temp);
	}
};
