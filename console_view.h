#ifndef CONSOLE_VIEW_H_
#define CONSOLE_VIEW_H_

#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

enum class Grayscale {
    black = 232, white = 255
};

/** Linearly rescale from [0, 255] to [232, 255]. */
int uint8_to_grayscale(uint8_t in) {
    return static_cast<int>(232.0 + 23.0 * (static_cast<double>(static_cast<int>(in)) / 255.0));
}

/**
 * Prefix a string with
 *    "\033[38;5;<N>m"
 * where N is the above Grayscale number.
 *
 */
class Console_grayscale_view
{
public:

    Console_grayscale_view(const std::vector<uint8_t>& image)
        : m_image(image) { }


    void finalize(std::ostream& out, const std::string& s) {
        out << s << std::string("\[033[39m") << std::endl;
    }

    void display(std::ostream& out) {
        std::ostringstream ss;

        for (int y = 0; y < 28; ++y) {
            for (int x = 0; x < 28; ++x) {
                uint8_t pixel = m_image[coords_to_index(x, y)];
                out << colored(pixel) << square();
            }
            out << "\n";
        }
        //finalize(std::cout, ss.str());
    }

    int coords_to_index(int x, int y) {
        return x + 28 * y;
    }

    std::string to_string(uint8_t pixel) {
         return std::to_string(static_cast<int>(pixel));
    }

    std::string colored(uint8_t pixel) {
        return "\e[38;5;" + std::to_string(uint8_to_grayscale(pixel)) + "m";
    }

    std::string square() {
        return "\u25A0";
    }

private:
    const std::vector<uint8_t> m_image;
};




#endif // CONSOLE_VIEW_H_
