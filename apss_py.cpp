/*cppimport
<%
cfg['compiler_args'] = ['-std=c++14']
setup_pybind11(cfg)
%>
*/
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <cstring>
#include <sstream>
#include <istream>
#include <ctime>
#include <cassert>
#include <chrono>

namespace py = pybind11;

typedef double Val;
Val sqr(Val x) { return x * x; }
typedef std::vector<std::vector<std::pair<int, Val>>> mat;
struct edge { int u, v; Val w; };

std::vector<std::pair<int, int>> all_pairs2(mat V, Val t) {
	std::vector<int> middle2old_id;
	mat vv;
	std::unordered_map<int, int> demension_cnt;
	std::set<int> exsiting_demensions;
	std::vector<Val> maxweight_x;

	for (int i = 0; i < (int)V.size(); ++i) {
		auto &feature = V[i];
		std::vector<std::pair<int, Val>> tmp_feature;
		for (auto x : feature) {
			if (x.second > 0.0) tmp_feature.push_back(x);
		}
		if (tmp_feature.size() > 0) {
			middle2old_id.push_back(i);
			Val _max = 0.0;
			for (auto &iVal : tmp_feature) {
				int i = iVal.first;
				Val val = iVal.second;
				demension_cnt[i] += 1;
				exsiting_demensions.insert(i);
				_max = std::max(_max, val);
			}
			maxweight_x.push_back(_max);
			vv.push_back(tmp_feature);
		}
	}

	std::vector<int> new2old_demensions;
	for (int i : exsiting_demensions) new2old_demensions.push_back(i);
	sort(new2old_demensions.begin(), new2old_demensions.end(), [&](int i, int j) { auto a = demension_cnt[i]; auto b = demension_cnt[j]; return a == b ? (i > j) : (a > b); });
	std::unordered_map<int, int> old2new_demensions;
	for (int i = 0; i < (int)new2old_demensions.size(); ++i) {
		old2new_demensions[new2old_demensions[i]] = i;
	}
	for (auto &feature : vv) {
		for (auto &iVal : feature) {
			auto &i = iVal.first;
			i = old2new_demensions[i];
		}
	}

	int m = (int) new2old_demensions.size();
	std::vector<Val> maxweight_V(m, 0.0);
	for (auto &feature : vv) {
		for (auto &iVal : feature) {
			int i = iVal.first;
			Val val = iVal.second;
			maxweight_V[i] = std::max(maxweight_V[i], val);
		}
	}

	int n = (int) vv.size();
	std::vector<int> new2middle_id;
	for (int i = 0; i < n; ++i) new2middle_id.push_back(i);
	sort(new2middle_id.begin(), new2middle_id.end(), [&](int i, int j) { auto a = maxweight_x[i]; auto b = maxweight_x[j]; return a == b ? i > j : a > b; });
	mat vvv;
	for (int i = 0; i < n; ++i) {
		vvv.push_back(vv[new2middle_id[i]]);
		sort(vvv.back().begin(), vvv.back().end());
	}
	vv.swap(vvv);
	vvv.clear();
	std::vector<Val> _maxweight_x(n, 0.0);
	for (int i = 0; i < n; ++i) {
		_maxweight_x[i] = maxweight_x[new2middle_id[i]];
	}
	maxweight_x.swap(_maxweight_x);
	_maxweight_x.clear();
	V.swap(vv);

	std::vector<edge> O;
	std::vector<std::vector<std::pair<int, Val>>> I;
	for (int i = 0; i < m; ++i) {
		std::vector<std::pair<int, Val>> emp;
		I.push_back(emp);
	}
	vv = V;
	auto &V_backup = vv;
	for (int xid = 0; xid < n; ++xid) {
		auto &x = V[xid];
		std::unordered_map<int, Val> A;
		Val remscore = 0.0;
		for (auto x_ix_val : x) {
			auto x_i = x_ix_val.first;
			auto x_val = x_ix_val.second;
			remscore += x_val * maxweight_V[x_i];
		}
		Val minsize = t / maxweight_x[xid];
		auto &xx = x;
		//sort(xx.begin(), xx.end());
		for (int ___ = (int)xx.size() - 1; ___ >= 0; --___) {
			auto x_i = xx[___].first;
			auto x_val = xx[___].second;
			std::vector<std::pair<int, Val>> tmp_I;
			for (const auto &yidy_val : I[x_i]) {
				const auto &yid = yidy_val.first;
				const auto &y_val = yidy_val.second;
				if (V_backup[yid].size() >= minsize) {
					if (A.count(yid) >= 1 || remscore >= t) {
						A[yid] += x_val * y_val;
					}
					tmp_I.push_back(std::make_pair(yid, y_val));
				}
			}
			remscore -= x_val * maxweight_V[x_i];
			I[x_i] = tmp_I;
		}
		for (auto yidAy : A) {
			auto yid = yidAy.first;
			auto Ay = yidAy.second;
			if (Ay + std::min((int)V[yid].size(), (int)V[xid].size()) * maxweight_x[xid] * maxweight_x[yid] >= t) {
				auto s = Ay;
				int ix = 0, iy = 0;
				while (ix < (int)V[xid].size() && iy < (int)V[yid].size() && s < t) { // early quit, dot value will be smaller
					std::pair<int, Val> px = V[xid][ix];
					std::pair<int, Val> py = V[yid][iy];
					if (px.first == py.first) {
						s += px.second * py.second;
						ix++;
						iy++;
					}
					else if (px.first < py.first) {
						ix++;
					}
					else {
						iy++;
					}
				}
				if (s >= t) {
					edge e;
					e.u = xid;
					e.v = yid;
					e.w = s;
					O.push_back(e);
				}
			}
		}
		Val b = 0.0, _max = 0.0;
		auto new_x = x; new_x.clear();
		for (int ___ = 0; ___ < (int)xx.size(); ++___) {
			auto x_i = xx[___].first;
			auto x_val = xx[___].second;
			b += std::min(maxweight_V[x_i], maxweight_x[xid]) * x_val;
			if (b >= t) {
				I[x_i].push_back(std::make_pair(xid, x_val));
			}
			else {
				new_x.push_back(std::make_pair(x_i, x_val));
				_max = std::max(_max, x_val);
			}
		}
		x = new_x;
		maxweight_x[xid] = _max;
	}
	std::vector<edge> _O;
	for (edge e : O) {
		e.u = middle2old_id[new2middle_id[e.u]];
		e.v = middle2old_id[new2middle_id[e.v]];
		_O.push_back(e);
	}
	std::vector<std::pair<int, int>> return_pairs;
	for (auto &e : _O) {
		return_pairs.push_back(std::make_pair(e.u, e.v));
	}
	return return_pairs;
}

PYBIND11_MODULE(apss_py, m) {
	m.def("all_pairs2", &all_pairs2);
}
