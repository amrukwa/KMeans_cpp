# include <iostream>
# include "dunn.h"
# include "pca.h"
# include "silhouette.h"

int main()
{
	std::ifstream datafile("directory/file_name.txt");
	vectors data(datafile);
	datafile.close();
	vectors st = standarise(data);
	kmeans estim = kmeans(2);
	SilhouetteSearch sil(estim);
	sil.fit(st);
	vectors labels_sil = sil.estimator.labels;
	DunnSearch dunn(estim);
	dunn.fit(st);
	vectors labels_dunn = dunn.estimator.labels;
	PCA transformer;
	vectors x = transformer.fit_transform(st);
	std::ofstream result("directory/result.txt");
	result << x;
	result.close();
	return 0;
}
