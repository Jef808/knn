#ifndef CONSOLE_VIEW_H_
#define CONSOLE_VIEW_H_

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace display {

enum class Grayscale {
    black = 232,
    white = 255
};

/**
 * Prefix a string with
 *    "\033[38;5;<N>m"
 * where N is the above Grayscale number.
 *
 */
class Console_grayscale_view {
    using image_t = std::vector<uint8_t>;

public:
    Console_grayscale_view(const std::vector<uint8_t>& image)
        : m_image(image)
    {
    }

    void display(std::ostream& out, const image_t& other = image_t())
    {
        for (int y = 0; y < 28; ++y) {
            for (int x = 0; x < 28; ++x) {
                uint8_t pixel = m_image[coords_to_index(x, y)];
                out << colored(pixel) << square();
            }
            if (!other.empty()) {
                reset_out(out);
                out << std::string(' ', 32);
                for (int x = 0; x < 28; ++x) {
                    uint8_t other_pixel = other[coords_to_index(x, y)];
                    out << colored(other_pixel) << square();
                }
            }
            reset_out(out);
            out << std::endl;
        }
        reset_out(out);
        out << std::endl;
    }

    void display_raw(std::ostream& out)
    {
        for (int y = 0; y < 28; ++y) {
            for (int x = 0; x < 28; ++x) {
                int pixel = m_image[coords_to_index(x, y)];
                out << pixel << ' ';
            }
        }
    }

private:
    const std::vector<uint8_t>& m_image;

    int coords_to_index(int x, int y)
    {
        return x + 28 * y;
    }

    std::string to_string(uint8_t pixel)
    {
        return std::to_string(static_cast<int>(pixel));
    }

    std::string colored(uint8_t pixel)
    {
        return "\e[38;5;" + std::to_string(uint8_to_grayscale(pixel)) + "m";
    }

    std::string square()
    {
        return "\u25A0";
    }

    void reset_out(std::ostream& out)
    {
        out << "\e[m";
    }

    /** Linearly rescale from [0, 255] to [232, 255]. */
    int uint8_to_grayscale(uint8_t in)
    {
        return static_cast<int>(232.0 + 23.0 * (static_cast<double>(static_cast<int>(in)) / 255.0));
    }
};

struct Single_viewer {

    using image_t = std::vector<uint8_t>;
    using image_it = image_t::const_iterator;

    Single_viewer(const image_t& image_l, const image_t& image_r = image_t())
    {
        display(image_l, image_r);
    }

    Single_viewer(image_it beg_l, image_it end_l, image_it beg_r = image_it(), image_it end_r = image_it())
    {
        display(image_t(beg_l, end_l), image_t(beg_r, end_r));
    }

    Single_viewer(const image_t& image_l, image_it beg_r = image_it(), image_it end_r = image_it())
    {
        display(image_l, image_t(beg_r, end_r));
    }

    void display(image_t imagea, image_t imageb)
    {
        Console_grayscale_view view(imagea);
        view.display(std::cout, imageb);
    }
};

struct View_iterator {
    using image_t = std::vector<uint8_t>;
    image_t m_image;

    View_iterator(const image_t& image)
        : m_image(image)
    {
    }

    void operator()(const image_t& image_r)
    {
        Console_grayscale_view view(m_image);
        view.display(std::cout, image_r);
    }
};

} // namespace display

#endif // CONSOLE_VIEW_H_
