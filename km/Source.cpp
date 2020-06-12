# include <iostream>
# include "dunn.h"
# include "pca.h"
int main()
{
	std::ifstream datafile("C:/Users/amruk/Desktop/file_name.txt");
	vectors some_vectors(datafile);
	datafile.close();
	vectors st = standarise(some_vectors);
	kmeans estim = kmeans(3);
	//dunn_index(&estim, some_vectors);*/
	vectors labels = estim.fit_predict(st);
	std::cout << labels; //change to saving to some result file
	std::cout << estim.n_iter;
	/*PCA transformer("NO");
	vectors x = transformer.fit_transform(st);
	std::cout << x;*/
	return 0;
}
