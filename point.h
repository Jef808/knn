#ifndef POINT_H_
#define POINT_H_


#include <vector>
#include <iostream>

class Point_Set {
public:
    using pixel_type = uint8_t;
    using label_type = uint8_t;
    using image_type = std::vector<pixel_type>;

    const size_t image_size;
    const int n_labels;

    Point_Set(const size_t _image_size, const int _n_labels) :
        n_labels(_n_labels), image_size(_image_size)
    { }

    Point_Set(image_type&& labels, image_type&& pixels, const size_t _image_size, const int _n_labels) :
        m_labels(std::move(labels)), m_pixels(std::move(pixels)), n_labels(_n_labels), image_size(_image_size)
    { }

    image_type::const_iterator beg(size_t n) const {
        return m_pixels.begin() + n * image_size;
    }

    image_type::const_iterator end(size_t n) const {
        return m_pixels.begin() + (n + 1) * image_size;
    }

    size_t size() const {
        return m_labels.size();
    }

    label_type label(size_t n) const {
        return m_labels[n];
    }

    image_type operator[](size_t n) const {
        return image_type(beg(n), end(n));
    }

private:
    image_type m_labels;
    image_type m_pixels;

    /**
     * From anywhere in the pixels vector, jump to the beginning of
     * the next image.
     */
    // image_type::iterator next_image(image_type::iterator it) {
    //     //NOTE: Don't add a new entry corresponding to the index at
    //     // each image, instead put that in the iterator.
    // }
};


#endif // POINT_H_
