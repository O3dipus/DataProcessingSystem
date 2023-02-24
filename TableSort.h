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

	int partition(std::vector<std::pair<int64_t, int64_t>>& _table, int _left, int _right, bool _column){
		int _pivotValue = _column ? _table[_right].second : _table[_right].first;
		int _divideIndex = _left;
		for(int _index=_left; _index<_right; _index++){
			if ((_column ? _table[_index].second : _table[_index].first) < _pivotValue){
				swapRow(_table,_index,_divideIndex);
				_divideIndex++;
			}
		}
		swapRow(_table,_right,_divideIndex);
		return _divideIndex;
	}

	void inPlaceQuick(std::vector<std::pair<int64_t, int64_t>>& _table, int _left, int _right, bool _column) {
		if (_right > _left) {
			int _newPivotIndex = partition(_table, _left, _right, _column);
			inPlaceQuick(_table, _left, _newPivotIndex-1, _column);
			inPlaceQuick(_table, _newPivotIndex+1, _right, _column);
		}
	}
};