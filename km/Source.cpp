# include <iostream>
# include "kmeans.h"

int main()
{
	std::ifstream datafile("C:/Users/amruk/Documents/kmeans_cpp/file_name.txt");
	vectors some_vectors(datafile);
	datafile.close();
	vectors st = standarise(some_vectors);
	kmeans estim = kmeans(3);
	vectors labels = estim.fit_predict(st);
	//std::cout << labels; //change to saving to some result file
	std::cout << estim.n_iter;
	return 0;
}
