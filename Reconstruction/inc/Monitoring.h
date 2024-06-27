#ifndef MONITORING_H
#define MONITORING_H

namespace Reconstruction{
    void Correction(const int& corrRC = 1, const int& corrGain = 1, const int& corrWF = 1, const int& corrDrift = 1);
    void Settings(const std::string &Selection, const std::string &outputDir, const int &uploader, const int &modules, const int& peaking_time, const int& diffusion, const int& drift_dist, const int& timbin);
    void Monitoring();
}

#endif