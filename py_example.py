import cppimport
apss = cppimport.imp("apss_py")

vec = [[(10, 0.1), (20, 0.9), (30, 0.5)],
	   [(10, 0.99), (40,0.99)],
	   [(30, 2.0), (40, 0.6)]]
threshold = 0.5
for u, v in apss.all_pairs2(vec, threshold):
	print(u, v)
