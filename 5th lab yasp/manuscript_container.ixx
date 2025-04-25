export module manuscript_container;

import <deque>;
import <fstream>;
import <ranges>;
import <algorithm>;
import <iostream>;
import <chrono>;
import <functional>;
#include <string>

export template<typename T>
class manuscript_container {
private:
    std::deque<T> container;

public:
    void add(const T& element, bool to_left = false) {
        if (to_left) container.push_front(element);
        else container.push_back(element);
    }

    bool remove_by_index(size_t index) {
        if (index >= container.size()) return false;
        container.erase(container.begin() + index);
        return true;
    }



    bool update(size_t index, const T& new_value) {
        if (index >= container.size()) return false;
        container[index] = new_value;
        return true;
    }


    void read_from_file(const std::string& file_name) {
        try {
            std::ifstream file(file_name);
            if (!file.is_open()) {
                throw std::runtime_error("Файл не удалось открыть.");
            }

            file >> *this;

            std::cout << "Загрузка завершена.\n";
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка при чтении файла: " << e.what() << "\n";
        }
    }


    void print(std::ostream_iterator<T> it)
    {
        std::copy(container.begin(), container.end(), it);
    }

    void read(std::istream_iterator<T> it)
    {
        container.clear();
        std::istream_iterator<T> eos;
        while (it != eos)
        {
            container.push_back(*it);
            ++it;
        }
    }

    manuscript_container<T> selection(std::function<bool(const T&)> predicate)
    {
        manuscript_container<T> result;
        std::copy_if(container.begin(), container.end(), std::back_inserter(result.container), predicate);
        return result;
    }

    friend std::ostream& operator<<(std::ostream& out, manuscript_container& cont)
    {
        std::ostream_iterator<T> it(out, "\n");
        cont.print(it);
        return out;
    }

    friend std::istream& operator>>(std::istream& in, manuscript_container& cont)
    {
        std::istream_iterator<T> it(in);
        cont.read(it);
        return in;
    }

    manuscript_container& operator=(const manuscript_container& other) {
        if (this != &other) {
            container.clear();
            std::copy(other.container.begin(), other.container.end(), std::back_inserter(container));
        }
        return *this;
    }

    size_t size() const { return container.size(); }

};

export template<typename T>
std::ostream& operator<<(std::ostream& out, const manuscript_container<T>& container) {

    for (const T& el : container.container) {
        out << el << '\n';
    }
    return out;
}