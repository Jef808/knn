
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <sstream>
#include <type_traits>

#include <boost/tokenizer.hpp>



std::ostream& operator<<(std::ostream& out, const std::vector<uint8_t>& vec) {
    for (auto v : vec) {
        std::cout << int(v) << ' ';
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const std::vector<int>& vec) {
    for (auto v : vec) {
        std::cout << v << ' ';
    }
    return out;
}



template < typename... Args >
std::ostream& PRINT (Args&&... args) {
    return (std::cout << ... << args) << std::endl;
}

template < typename... Args >
std::ostream& OUT (std::ostream& out, Args&&... args) {
    return (out << ... << args);
}

auto now() {
    return std::chrono::steady_clock::now();
}

void time(auto start, auto end) {
    std::chrono::duration<double> elapsed = end - start;
    PRINT("elapsed time: ", elapsed.count(), "s\n");
}

auto tokenize(const std::string& str, std::vector<uint8_t>& ret) {
    auto it = str.begin(), end = str.end();
    for (int i=0; i<785; ++i) {
        auto it_n = std::find(it, end, ',');
        auto ss = static_cast<uint8_t>(std::stoi(std::string(it, it_n)));
        ret.push_back(ss);
        // if (it_n == str.end())
        //     break;
        it = ++it_n;
    }

    return ret;
}

auto input2(std::istream& ifs, int count = 0) {
    std::vector<uint8_t> ret;
    ret.reserve(count * 785);
    std::string buf;
    int c = 0;
    while (std::getline(ifs, buf)) {
        ++c;
        tokenize(buf, ret);
        if (c == count)
            break;
    }
    return ret;
}

struct load_csv_bare {
    auto operator()(const std::string& filename, int n_lines = 12000) {
        std::ifstream ifs (filename);
        if (!ifs) {
            PRINT("could not open document");
            return std::vector<uint8_t>();
        }
        std::string _buf;
        std::getline(ifs, _buf); _buf.clear();
        return input2(ifs, n_lines);
    }
};


class csv_tokenizer {
public:
    //csv_tokenizer() {}
    csv_tokenizer(const std::string& s) :
        m_str(s), m_it(m_str.begin()), m_end(s.end())
    { find_next_token(); }

    void find_next_token() {
        m_nex = std::find(m_it, m_end, ',');
    }

    //csv_tokenizer& operator=(const csv_tokenizer& other) = default;

    // void setbuf(const std::string& s) {
    //     m_str = s;
    //     m_it = m_str.begin();
    //     m_end = m_str.end();
    //     find_next_token();
    // }

    void setbuf(const std::string& s) {
        m_str = s;
        m_it = m_str.begin();
        m_end = m_str.end();
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
    std::string m_str;
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

    csv_tokenizer* rdbuf() {
        return m_buf;
    }

    std::string operator*() const {
        return m_buf->get_token();
    }

    csv_tokenizer_iterator& operator++() {
        //PRINT("bumping");
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
        // std::back_insert_iterator<std::vector<std::string>>(), m_vec(vec)
    {}

    back_converter_inserter& operator=(const std::string& s) {
        //PRINT("In operator=");
        m_vec.push_back(static_cast<uint8_t>(std::stoi(s)));
        return *this;
    }

    back_converter_inserter& operator++() { return *this; }
    back_converter_inserter& operator++(int) { return *this; }
    back_converter_inserter& operator*() { return *this; }

private:
    //std::vector<uint8_t> m_vec;
    std::vector<uint8_t>& m_vec;
};

auto input(std::istream& ifs, int count=0) {
    std::vector<uint8_t> ret{};
    ret.reserve(785 * count);
    std::string buf;
    int c = 0;
    auto tok = csv_tokenizer(buf);
    auto tok_it = csv_tokenizer_iterator(&tok);
    auto out = back_converter_inserter(ret);
    while (getline(ifs, buf)) {
        tok_it.rdbuf()->setbuf(buf);
        ++c;
        //auto tok_end = csv_tokenizer_iterator();
        while (tok_it) {
            auto _n = *tok_it;
            out = _n; ++tok_it;
        }
    }
    return ret;
}

struct load_csv {
    auto operator()(const std::string& filename, int n_lines = 12000) {
        std::ifstream ifs (filename);
        if (!ifs) {
            PRINT("could not open document");
            return std::vector<uint8_t>();
        }
        std::string _buf;
        std::getline(ifs, _buf); _buf.clear();
        return input(ifs, n_lines);
    }
};


int main()
{
    std::string fn =  "data/mnist/train.csv";
    int n_images = 12000;


    PRINT("document open");

    std::string buf;
    // first line

    PRINT("bare method");
    auto start0 = now();
    auto ret0 = load_csv_bare()(fn, n_images);
    auto end0 = now();
    time(start0, end0);
    ret0.clear();

    PRINT("fancy method");
    auto start = now();
    auto ret = load_csv()(fn, n_images);
    auto end = now();
    time(start, end);



}
