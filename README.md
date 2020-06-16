[![CodeFactor](https://www.codefactor.io/repository/github/amrukwa/kmeans_cpp/badge)](https://www.codefactor.io/repository/github/amrukwa/kmeans_cpp)
[![BCH compliance](https://bettercodehub.com/edge/badge/amrukwa/kmeans_cpp?branch=master)](https://bettercodehub.com/)

# kmeans_cpp
K-Means algorithm implementation in C++

# Tools
1) [`Vectors`](https://github.com/amrukwa/kmeans_cpp/blob/master/km/Vectors.h) - provides class and functions for operations on matrices
2) [`initialization`](https://github.com/amrukwa/kmeans_cpp/blob/master/km/initialization.h) - entails functions choosing some part of given vectors (random or k++ initialization)
3) [`kmeans`](https://github.com/amrukwa/kmeans_cpp/blob/Vectors-documentation/km/kmeans.h) - runs k-means algorithm
4) [`dunn`](https://github.com/amrukwa/kmeans_cpp/blob/Vectors-documentation/km/dunn.h) - runs k-means algorithm with sweep by Dunn index
5) [`silhouette`](https://github.com/amrukwa/kmeans_cpp/blob/Vectors-documentation/km/silhouette.h) - runs k-means algorithm with sweep by Silhouette Score
6) [`sorting`](https://github.com/amrukwa/kmeans_cpp/blob/Vectors-documentation/km/sorting.h) - sorts two vectors in descending order of one of them
7) [`pca`](https://github.com/amrukwa/kmeans_cpp/blob/Vectors-documentation/km/pca.h) - performs dimension reduction by Principal Component Analysis
