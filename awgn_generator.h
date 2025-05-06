#pragma once
#include <vector>
#include <random>
#include <cmath>
#include "structures.h"


class AWGN_Adder {
public:
    // Constructor with SNR in dB
    AWGN_Adder(double snr_db) : snr_db_(snr_db) {}
    
    // Add AWGN noise to QAM signal
    std::vector<IQ> addNoise(const std::vector<IQ>& signal) {
        std::vector<IQ> noisy_signal;
        noisy_signal.reserve(signal.size());
        
        // 1. Calculate signal power
        double sig_power = 0.0;
        for (const auto& point : signal) {
            sig_power += point.Re*point.Re + point.Im*point.Im;
        }
        sig_power /= signal.size();
        
        // 2. Calculate noise standard deviation
        double noise_power = sig_power / std::pow(10.0, snr_db_/10.0);
        double stddev = std::sqrt(noise_power/2.0); // For complex noise
        
        // 3. Generate and add noise
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, stddev);
        
        for (const auto& point : signal) {
            noisy_signal.push_back({
                point.Re + dist(gen),
                point.Im + dist(gen)
            });
        }
        
        return noisy_signal;
    }

private:
    double snr_db_;  // Signal-to-Noise Ratio in dB
};