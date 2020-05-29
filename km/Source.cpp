# include <iostream>
# include "kmeans.h"

int main()
{
	std::ifstream datafile("C:/Users/amruk/Documents/kmeans_cpp/data.txt");
	vectors some_vectors(datafile);
	datafile.close();
	some_vectors.shape(); 
	std::cout << some_vectors;
	std::cout << std::endl;
	vectors st = standarise(some_vectors);
	std::cout << st;
	std::cout << std::endl;
	return 0;
}
