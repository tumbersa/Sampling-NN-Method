//
//  main.cpp
//  Sampling Method
//
//  Created by Глеб Капустин on 06.11.2023.
//

#include "ImageProcessor.hpp"

int main() {
    const char* originalImagePath = "/Users/gleb_kapustin/Desktop/VSU/Sampling Method/Sampling Method/input.jpg";
    const char* reducedNNImagePath = "/Users/gleb_kapustin/Desktop/VSU/Sampling Method/Sampling Method/output_nearest_neighbor.jpg";
    const char* reducedSamplingImagePath = "/Users/gleb_kapustin/Desktop/VSU/Sampling Method/Sampling Method/output_sampling.jpg";
    
    ImageProcessor imageProcessor(originalImagePath);
    
    int reducedWidth = 10; // Your desired reduced width
    int reducedHeight = 10; // Your desired reduced height
    
    imageProcessor.reduceNearestNeighbor(reducedWidth, reducedHeight, reducedNNImagePath);
    imageProcessor.reduceSampling(reducedWidth, reducedHeight, reducedSamplingImagePath);
    
    return 0;
}
