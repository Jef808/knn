#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <sstream>
#include <utility>
#include <tuple>
#include <type_traits>


template < typename... Args >
inline std::ostream& PRINT (Args&&... args) {
    return (std::cout << ... << args) << std::endl;
}

inline auto tokenize(const std::string& str, std::vector<uint8_t>& ret, std::vector<uint8_t>& labels) {
    auto it = str.begin(), end = str.end();
    auto it_n = std::find(it, end, ',');
    auto ss = static_cast<uint8_t>(std::stoi(std::string(it, it_n)));
    uint8_t label = ss;
    labels.push_back(label);
    for (int i=0; i<784; ++i) {
        it_n = std::find(it, end, ',');
        ss = static_cast<uint8_t>(std::stoi(std::string(it, it_n)));
        ret.push_back(ss);
        // if (it_n == str.end())
        //     break;
        it = ++it_n;
    }
}

inline auto input2(std::istream& ifs, int count = 0) {
    std::vector<uint8_t> ret;
    std::vector<uint8_t> labels;
    ret.reserve(count * 785);
    labels.reserve(count);
    std::string buf;
    int c = 0;
    while (std::getline(ifs, buf)) {
        ++c;
        tokenize(buf, ret, labels);
        if (c == count)
            break;
    }
    return std::pair{ labels, ret };
}

struct load_csv_bare {
    auto operator()(const std::string& filename, int n_lines = 12000) {
        std::ifstream ifs (filename);
        if (!ifs) {
            std::cerr << "could not open document";
            return std::pair{ std::vector<uint8_t>(), std::vector<uint8_t>() };
        }
        std::string _buf;
        std::getline(ifs, _buf); _buf.clear();
        return input2(ifs, n_lines);
    }
};


class csv_tokenizer {
public:
    csv_tokenizer(std::string* s) :
        m_str(s), m_it(m_str->begin()), m_end(s->end())
    { find_next_token(); }

    void find_next_token() {
        m_nex = std::find(m_it, m_end, ',');
    }

    void reset() {
        m_it = m_str->begin();
        m_end = m_str->end();
        find_next_token();
    }

    std::string get_token() const {
        return std::string(m_it, m_nex);
    }

    bool bump() {
        m_it = m_nex + 1;
        find_next_token();
        return m_nex != m_end;
    }

private:
    std::string* m_str;
    std::string::const_iterator m_it;
    std::string::const_iterator m_nex;
    std::string::const_iterator m_end;
};


class csv_tokenizer_iterator :
    public std::iterator<std::input_iterator_tag, std::string> {
public:

    csv_tokenizer_iterator() :
        m_buf(nullptr), m_ok(false) { }

    csv_tokenizer_iterator(csv_tokenizer* _buf) :
        m_buf(_buf), m_ok(_buf) { }

    csv_tokenizer_iterator& operator=(const csv_tokenizer_iterator&) = default;
    ~csv_tokenizer_iterator() = default;

    void resetbuf() {
        m_buf->reset();
        m_ok = true;
    }

    std::string operator*() const {
        return m_buf->get_token();
    }

    csv_tokenizer_iterator& operator++() {
        m_ok = m_buf->bump();
        return *this;
    }

    csv_tokenizer_iterator operator++(int) {
        csv_tokenizer_iterator it = *this;
        m_ok = m_buf->bump();
        return it;
    }

    operator bool() const {
        return m_ok;
    }

    bool friend operator==(const csv_tokenizer_iterator& a, const csv_tokenizer_iterator& b) {
        return a.m_ok == b.m_ok;
    }

    bool friend operator!=(const csv_tokenizer_iterator& a, const csv_tokenizer_iterator& b) {
        return a.m_ok;
    }

private:
    mutable csv_tokenizer* m_buf;
    bool m_ok;
};

class back_converter_inserter
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = std::string;

    back_converter_inserter(std::vector<uint8_t>& vec) :
        m_vec(vec)
    {}

    back_converter_inserter& operator=(const std::string& s) {
        m_vec.push_back(static_cast<uint8_t>(std::stoi(s)));
        return *this;
    }

    back_converter_inserter& operator++() { return *this; }
    back_converter_inserter& operator++(int) { return *this; }
    back_converter_inserter& operator*() { return *this; }

private:
    std::vector<uint8_t>& m_vec;
};

/** {labels, pixels} where pixels is a one-dimensional concatenation of all the rows. */
inline auto input(std::istream& ifs, int count=0) {
    std::vector<uint8_t> ret{};
    std::vector<uint8_t> labels{};
    ret.reserve(784 * count);
    labels.reserve(count);

    std::string buf;
    auto tok = csv_tokenizer(&buf);
    auto tok_it = csv_tokenizer_iterator(&tok);
    auto out_pixels = back_converter_inserter(ret);
    auto out_labels = back_converter_inserter(labels);

    while (getline(ifs, buf)) {
        tok_it.resetbuf();
        auto _n = *tok_it;
        out_labels = _n; ++tok_it;
        //auto tok_end = csv_tokenizer_iterator();
        while (tok_it) {
            _n = *tok_it;
            out_pixels = _n; ++tok_it;
        }
        // TODO: Fix this! The last entry doesn't get picked up
        // (when there is no comma left)
        _n = *tok_it;
        out_pixels = _n;
    }

    return std::pair{ labels, ret };
}

/** {labels, pixels} where pixels is a one-dimensional concatenation of all the rows. */
inline auto input_no_labels(std::istream& ifs, int n_lines) {
    std::vector<uint8_t> ret{};
    ret.reserve(784 * n_lines);
    
    std::string buf;
    auto tok = csv_tokenizer(&buf);
    auto tok_it = csv_tokenizer_iterator(&tok);
    auto out_pixels = back_converter_inserter(ret);
    
    while (getline(ifs, buf)) {
        tok_it.resetbuf();
        std::string _n;
        //auto tok_end = csv_tokenizer_iterator();
        while (tok_it) {
            _n = *tok_it;
            out_pixels = _n; ++tok_it;
        }
        // TODO: Fix this! The last entry doesn't get picked up
        // (when there is no comma left)
        _n = *tok_it;
        out_pixels = _n;
    }

    return ret;
}

struct load_csv_helper {

    std::pair<std::vector<uint8_t>,
              std::vector<uint8_t>>
    operator()(const std::string& filename, int n_lines) {
        std::ifstream ifs (filename);
        if (!ifs) {
            PRINT("could not open document");
            return std::pair { std::vector<uint8_t>(), std::vector<uint8_t>() };
        }
        std::string _buf;
        std::getline(ifs, _buf); _buf.clear();
        return input(ifs, n_lines);
    }
};

struct load_csv_helper_no_labels {
    std::vector<uint8_t>
    operator()(const std::string& filename, int n_lines) {
        std::ifstream ifs (filename);
        if (!ifs) {
            PRINT("could not open document");
            return std::vector<uint8_t>();
        }
        std::string _buf;
        std::getline(ifs, _buf); _buf.clear();
        return input_no_labels(ifs, n_lines);
    }
};


auto inline load_csv(const std::string& filename, int n_lines = 0) {
    return load_csv_helper()(filename, n_lines);
}

auto inline load_csv_no_labels(const std::string& filename, int n_lines = 0) {
    return load_csv_helper_no_labels()(filename, n_lines);
}

struct load_csv_train_test {
    using vec = std::vector<uint8_t>;

    std::tuple<vec, vec, vec, vec>
    operator()(const std::string& filename, int n_training, int n_test) {
        std::ifstream ifs (filename);
        if (!ifs) {
            PRINT("could not open document");
            return std::make_tuple( vec{}, vec{}, vec{}, vec{} );
        }
        std::string _buf;
        std::getline(ifs, _buf); _buf.clear();

        auto [labels_train, images_train] = input(ifs, n_training);
        auto [labels_test, images_test] = input(ifs, n_test);

        return std::make_tuple(labels_train, images_train, labels_test, images_test);
    }
};

