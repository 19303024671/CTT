#include <iostream>
#include <chrono>
#include <thread>

const int kProgressBarWidth = 40;  // ���������

class ProgressBar {
public:
    ProgressBar(int totalSteps) :
        totalSteps_(totalSteps), currentStep_(0) {}

    // ���½�����
    void update() {
        currentStep_++;
        float progress = (float)currentStep_ / (float)totalSteps_;
        int progressBarLength = (int)(progress * kProgressBarWidth);
        std::cout << "\r[";
        for (int i = 0; i < kProgressBarWidth; ++i) {
            if (i < progressBarLength) {
                std::cout << ">";
            }
            else {
                std::cout << " ";
            }
        }
        std::cout << "] " << int(progress * 100.0) << " %";
        std::cout.flush();
    }

private:
    int totalSteps_;
    int currentStep_;
};