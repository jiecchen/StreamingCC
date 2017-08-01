#include <streamingcc>
#include <iostream>
using namespace streamingcc;

int main() {
    // create an object which will maintain
    // 10 samples (with replacement) dynamically
    ReservoirSampler<int> rsmp(10);
    // sample from a data stream with length 1,000,000
    for (int i = 0; i < 1000000; i++)
        rsmp.ProcessItem(i);

    // print the samples
    for (auto sample: rsmp.GetSamples())
        std::cout << sample << " ";
    std::cout << std::endl;
    return 0;
}
