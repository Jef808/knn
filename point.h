#ifndef POINT_H_
#define POINT_H_


#include <vector>
#include <iostream>

class Point_Set {
public:
    using pixel_type = uint8_t;
    using label_type = uint8_t;
    using container_type = std::vector<pixel_type>;

    Point_Set() = default;

    Point_Set(container_type&& labels, container_type&& pixels) :
        m_labels(std::move(labels)), m_pixels(std::move(pixels))
    { }

    container_type::const_iterator beg(size_t n) const {
        return m_pixels.begin() + n * 784;
    }

    container_type::const_iterator end(size_t n) const {
        return m_pixels.begin() + (n + 1) * 784;
    }

    size_t size() const {
        return m_labels.size();
    }

    label_type label(size_t n) const {
        return m_labels[n];
    }

private:
    container_type m_labels;
    container_type m_pixels;

    /**
     * From anywhere in the pixels vector, jump to the beginning of
     * the next image.
     */
    // container_type::iterator next_image(container_type::iterator it) {
    //     //NOTE: Don't add a new entry corresponding to the index at
    //     // each image, instead put that in the iterator.
    // }
};


#endif // POINT_H_
