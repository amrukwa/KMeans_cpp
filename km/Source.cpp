# include <iostream>
# include "Vectors.h"

int main()
{
	std::ifstream datafile("C:/Users/amruk/Documents/kmeans_cpp/data.txt");
	vectors some_vectors(datafile);
	datafile.close();
	some_vectors.shape();
	std::cout << some_vectors;

	double mean = 0;
	for (int i = 0; i < some_vectors.n_features; i++)
	{
		some_vectors.mean_of_column(i, &mean);
		std::cout << mean << std::endl;
	}
	return 0;
}
