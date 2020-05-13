# include <iostream>
# include "Vectors.h"

int main()
{
	std::ifstream datafile("C:/Users/amruk/Documents/kmeans_cpp/data.txt");
	vectors some_vectors(datafile);
	datafile.close();
	some_vectors.shape();
	return 0;
}
