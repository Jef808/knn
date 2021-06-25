#include "console_view.h"

#include <vector>
#include <iostream>
#include <cstdint>
#include <random>

#include "mnist_new.h"


auto beg_end(int n) {
    auto beg = n * 785;
    auto end = (n + 1) * 785;
    return std::pair{ beg, end };
}

struct Viewer {
    Viewer(const std::vector<uint8_t>& images)
        : m_images(images) { }

    void operator()(int n, bool display_raw = false) {
        auto [nbeg, nend] = beg_end(n);
        auto label = int(*(m_images.begin() + nbeg));
        auto beg = m_images.begin() + nbeg + 1;
        auto end = m_images.begin() + nend;

        auto img = std::vector<uint8_t>(beg, end);

        std::cout << "        " << label << "\n" << std::endl;

        if (display_raw)
            std::cout << img << std::endl;

        Console_grayscale_view view(img);

        view.display(std::cout);
    }

    const std::vector<uint8_t>& m_images;
};


int main()
{
    std::vector<uint8_t> images = load_csv()("data/mnist/train.csv");

    // for (int i=0;i<784; ++i) {
    //     int val = rand() % 256;
    //     image.push_back(static_cast<uint8_t>(val));
    // }

    Viewer view(images);

    for (int i=0; i<10; ++i) {
        view(i);
    }
}
