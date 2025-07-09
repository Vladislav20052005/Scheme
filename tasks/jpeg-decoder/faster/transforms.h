#pragma once

#include <vector>
#include <image.h>
#include <fft.h>
#include <../fields.h>
#include <glog/logging.h>

void Convert(std::vector<std::vector<double>>& vec);

void DCTTransform(std::vector<std::vector<double>>& vec);

void ConstructImage(std::vector<std::vector<double>>& input, Image& image, Stats& stats);

RGB ToRGB(double y, double cb, double cr);