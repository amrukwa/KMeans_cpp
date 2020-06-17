[![CodeFactor](https://www.codefactor.io/repository/github/amrukwa/kmeans_cpp/badge)](https://www.codefactor.io/repository/github/amrukwa/kmeans_cpp)
[![BCH compliance](https://bettercodehub.com/edge/badge/amrukwa/kmeans_cpp?branch=master)](https://bettercodehub.com/)

# kmeans_cpp
K-Means algorithm implementation in C++

## Tools
1) [`Vectors`](https://github.com/amrukwa/kmeans_cpp/blob/master/km/Vectors.h) - provides class and functions for operations on matrices
2) [`initialization`](https://github.com/amrukwa/kmeans_cpp/blob/master/km/initialization.h) - entails functions choosing some part of given vectors (random or k++ initialization)
3) [`kmeans`](https://github.com/amrukwa/kmeans_cpp/blob/Vectors-documentation/km/kmeans.h) - runs k-means algorithm
4) [`dunn`](https://github.com/amrukwa/kmeans_cpp/blob/Vectors-documentation/km/dunn.h) - runs k-means algorithm with sweep by Dunn index
5) [`silhouette`](https://github.com/amrukwa/kmeans_cpp/blob/Vectors-documentation/km/silhouette.h) - runs k-means algorithm with sweep by Silhouette Score
6) [`sorting`](https://github.com/amrukwa/kmeans_cpp/blob/Vectors-documentation/km/sorting.h) - sorts two vectors in descending order of one of them
7) [`pca`](https://github.com/amrukwa/kmeans_cpp/blob/Vectors-documentation/km/pca.h) - performs dimension reduction by Principal Component Analysis

## Exemplary usage
You can use this implementation as shown [`here`](https://github.com/amrukwa/kmeans_cpp/blob/Vectors-documentation/km/Example.cpp). 

## Parameters

### Distance
In this version the used distance metric can be chosen from Euclidean, correlation and cityblock distance.

### Initialization
The default initialization method is set to k++, however it is possible to choose MacQueen's random method as well.

### Inter and intra distances
Dunn Index relies on the choice of the appropriate inter and intra distance measurement types. For distance between clusters the available metrics are: centroid, closest, furthest, avg, while for distance within cluster: centroid, furthest, avg.

## The data file
I prepared a special constructor for matrices that loads the data from the specified stream. You can find it in *Vectors.h*. The usage is shown in the [`exemplary file`](https://github.com/amrukwa/kmeans_cpp/blob/Vectors-documentation/km/Example.cpp).
You can use as many samples and features as you wish - the program will take care of the dimensions.
You should not place the number of desired clusters in this file - choose it while creating kmeans instance or let it be done by Dunn or Silhouette. Default minimum and maximum numbers of clusters for these scores are 2 and 20 respectively.
In the following lines of the file insert points coordinates, don't use commas. The delimiter is set to space. You can use exponential notation. Every line, including the last line, should be ended with enter. This version doesn't check if your file is correctly prepared for overall shorter execution time. This can lead to unexpected errors in case of incorrect file.
