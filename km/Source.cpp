# include <iostream>
# include "kmeans.h"

int main()
{
	std::ifstream datafile("C:/Users/amruk/Documents/kmeans_cpp/data.txt");
	vectors some_vectors(datafile);
	datafile.close();
	some_vectors.shape();
	std::cout << some_vectors;
	vectors c(3, 2);
	first_centroid(&c, some_vectors);
	std::cout << c;
	return 0;
}
