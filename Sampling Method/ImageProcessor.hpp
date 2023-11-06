//
//  ImageProcessor.cpp
//  Sampling Method
//
//  Created by Глеб Капустин on 06.11.2023.
//
#define STB_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
class ImageProcessor {
public:
    ImageProcessor(const char* originalImagePath);
    ~ImageProcessor();

    void reduceNearestNeighbor(int reducedWidth, int reducedHeight, const char* outputPath);
    void reduceSampling(int reducedWidth, int reducedHeight, const char* outputPath);

private:
    int originalWidth;
    int originalHeight;
    int originalChannels;
    unsigned char* originalImage;

    void loadOriginalImage(const char* originalImagePath);
    void saveReducedImage(const char* outputPath, const unsigned char* reducedImage, int width, int height);
};

ImageProcessor::ImageProcessor(const char* originalImagePath) {
    loadOriginalImage(originalImagePath);
}

ImageProcessor::~ImageProcessor() {
    stbi_image_free(originalImage);
}

void ImageProcessor::loadOriginalImage(const char* originalImagePath) {
    originalImage = stbi_load(originalImagePath, &originalWidth, &originalHeight, &originalChannels, 0);
    if (!originalImage) {
        std::cerr << "Invalid path to source image." << std::endl;
        return;
    }
}

void ImageProcessor::saveReducedImage(const char* outputPath, const unsigned char* reducedImage, int width, int height) {
    stbi_write_jpg(outputPath, width, height, originalChannels, reducedImage, 100); // Save in JPEG format
}

void ImageProcessor::reduceNearestNeighbor(int reducedWidth, int reducedHeight, const char* outputPath) {
    unsigned char* reducedImage = new unsigned char[reducedWidth * reducedHeight * originalChannels];

    for (int y = 0; y < reducedHeight; ++y) {
        for (int x = 0; x < reducedWidth; ++x) {
            int orig_x = x * originalWidth / reducedWidth;
            int orig_y = y * originalHeight / reducedHeight;

            int origIndex = (orig_y * originalWidth + orig_x) * originalChannels;
            int reducedIndex = (y * reducedWidth + x) * originalChannels;

            for (int c = 0; c < originalChannels; ++c) {
                reducedImage[reducedIndex + c] = originalImage[origIndex + c];
            }
        }
    }

    saveReducedImage(outputPath, reducedImage, reducedWidth, reducedHeight);

    delete[] reducedImage;
}

void ImageProcessor::reduceSampling(int reducedWidth, int reducedHeight, const char* outputPath) {
    unsigned char* reducedImage = new unsigned char[reducedWidth * reducedHeight * originalChannels];

    for (int y = 0; y < reducedHeight; ++y) {
        for (int x = 0; x < reducedWidth; ++x) {
            float orig_x = static_cast<float>(x) * (static_cast<float>(originalWidth) / reducedWidth);
            float orig_y = static_cast<float>(y) * (static_cast<float>(originalHeight) / reducedHeight);

            int orig_x_floor = static_cast<int>(orig_x);
            int orig_y_floor = static_cast<int>(orig_y);
            int orig_x_ceil = orig_x_floor + 1;
            int orig_y_ceil = orig_y_floor + 1;

            float x_weight = orig_x - orig_x_floor;
            float y_weight = orig_y - orig_y_floor;

            int reducedIndex = (y * reducedWidth + x) * originalChannels;

            for (int c = 0; c < originalChannels; ++c) {
                float top_left = originalImage[(orig_y_floor * originalWidth + orig_x_floor) * originalChannels + c];
                float top_right = originalImage[(orig_y_floor * originalWidth + orig_x_ceil) * originalChannels + c];
                float bottom_left = originalImage[(orig_y_ceil * originalWidth + orig_x_floor) * originalChannels + c];
                float bottom_right = originalImage[(orig_y_ceil * originalWidth + orig_x_ceil) * originalChannels + c];

                float interpolated_value = top_left * (1 - x_weight) * (1 - y_weight) +
                                          top_right * x_weight * (1 - y_weight) +
                                          bottom_left * (1 - x_weight) * y_weight +
                                          bottom_right * x_weight * y_weight;

                reducedImage[reducedIndex + c] = static_cast<unsigned char>(interpolated_value);
            }
        }
    }

    saveReducedImage(outputPath, reducedImage, reducedWidth, reducedHeight);

    delete[] reducedImage;
}
