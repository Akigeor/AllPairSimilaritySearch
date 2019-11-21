#include <bits/stdc++.h>
#include "apss.hpp"
int main() {
	std::vector< std::vector< std::pair<int, double> > > vec = {{{10, 0.1}, {20, 0.9}, {30, 0.5}},
																{{10, 0.99}, {40, 0.99}},
																{{30, 2.0}, {40, 0.6}}};
	double threshold = 0.5;
	std::vector<std::pair<int, int>> result = all_pairs2(vec, threshold);
	for (auto e : result) {
		printf("%d %d\n", e.first, e.second);
	}
}

