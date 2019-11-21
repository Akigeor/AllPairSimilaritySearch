# All Pair Similarity Search for dot product

This is a C++ implementation for paper [Scaling Up All Pairs Similarity Search](https://research.google.com/pubs/archive/32781.pdf) in dot product task: we want to find all the vector pairs with dot product greater than or equal to some threshold. The vectors are given in sparse matrix and the values in all vectors should be **non-negative**.

Example for C++:

```c++
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
```

Output:

```
1 2
0 2
```

Example for Python3 (using pybind11 and cppimport):

```python
import cppimport
apss = cppimport.imp("apss_py")

vec = [[(10, 0.1), (20, 0.9), (30, 0.5)],
	   [(10, 0.99), (40,0.99)],
	   [(30, 2.0), (40, 0.6)]]
threshold = 0.5
for u, v in apss.all_pairs2(vec, threshold):
	print(u, v)
```

Output:

````
1 2
0 2
````

