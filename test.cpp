#include <iostream>
#include "structures.h"
#include "awgn_generator.h"
#include "mapping.h"
#include "demapper.h"
#include "structures.h"
#include "extern_func.h"
#include "awgn_generator.h"
#include <iostream>

int main(){

    auto BitTx = generateBitVector(120000, 0.5);

    Mapper mapping(BitTx);
    auto qpsk_samples = mapping.modulate("QPSK");
    auto qam16_samples = mapping.modulate("QAM16");
    auto qam64_samples = mapping.modulate("QAM64");

    std::vector<int> SNRs(1200); 
    double SNR = -20;
    for (unsigned int i = 0; i < SNRs.size(); ++i) {
        std::cout << SNR << ", ";
        SNRs[i] = SNR;
        SNR += 0.04;
    }
    std::cout << std::endl;

    std::vector<double> BERS1(SNRs.size());
    for (unsigned int i = 0; i < SNRs.size(); ++i) {
        AWGN_Adder noise(SNRs[i]);
        auto noised_sig = noise.addNoise(qpsk_samples);
        Demapper demap(noised_sig);
        auto BitRx = demap.demodulate("QPSK");
        auto ber = bitErrorProbability(BitTx, BitRx);
        std::cout << ber << ", ";
        BERS1[i] = ber;
    }

    std::cout << std::endl;

    std::vector<double> BERS2(SNRs.size());
    for (unsigned int i = 0; i < SNRs.size(); ++i) {
        AWGN_Adder noise(SNRs[i]);
        auto noised_sig = noise.addNoise(qam16_samples);
        Demapper demap(noised_sig);
        auto BitRx = demap.demodulate("QAM16");
        auto ber = bitErrorProbability(BitTx, BitRx);
        std::cout << ber << ", ";
        BERS2[i] = ber;
    }

    std::cout << std::endl;
    std::vector<double> BERS3(SNRs.size());
    for (unsigned int i = 0; i < SNRs.size(); ++i) {
        AWGN_Adder noise(SNRs[i]);
        auto noised_sig = noise.addNoise(qam64_samples);
        Demapper demap(noised_sig);
        auto BitRx = demap.demodulate("QAM64");
        auto ber = bitErrorProbability(BitTx, BitRx);
        std::cout << ber << ", ";
        BERS3[i] = ber;
    }



    return 0;
}