export module manuscript_container;

import <deque>;
import <fstream>;
import <ranges>;
import <algorithm>;
import <iostream>;
import <chrono>;
import ancient_types;

export template<typename T>
class manuscript_container {
private:
    std::deque<T> data;

public:
    void add(const T& element, bool to_left = false) {
        if (to_left) data.push_front(element);
        else data.push_back(element);
    }

    bool remove_by_index(size_t index) {
        if (index >= data.size()) return false;
        data.erase(data.begin() + index);
        return true;
    }


    bool update(size_t index, const T& new_value) {
        if (index >= data.size()) return false;
        data[index] = new_value;
        return true;
    }

    void read_from_file(const std::string& file_name) {
        try {
            std::ifstream file(file_name);
            if (!file.is_open()) {
                throw std::runtime_error("Файл не удалось открыть.");
            }

            T temp;
            while (file >> temp) {
                data.push_back(temp);
            }
            std::cout << "Загрузка завершена.\n";
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка при чтении файла: " << e.what() << "\n";
        }
    }


    void write_to_file(const std::string& file_name) const {
        std::ofstream file(file_name);
        for (const auto& el : data) {
            file << el << "\n";
        }
    }

    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, const manuscript_container<U>& container);

    std::vector<T> select_by_author(const std::string& author) const {
        return data | std::views::filter([&](const T& m) { //result = adapter1(adapter2(...(data))) | используется как pipeline  
            return m.author == author;
            }) | std::ranges::to<std::vector>();
    }

    std::vector<T> select_by_length(size_t min_chars, size_t max_chars) const {
        return data | std::views::filter([&](const T& m) {
            size_t len = m.text.size();
            return len >= min_chars && len <= max_chars;
            }) | std::ranges::to<std::vector>();
    }

    std::vector<T> select_by_date(std::chrono::year_month_day from, std::chrono::year_month_day to) const {
        return data | std::views::filter([&](const T& m) {
            return m.creation_date >= from && m.creation_date <= to;
            }) | std::ranges::to<std::vector>();
    }

    size_t size() const { return data.size(); }

    const std::deque<T>& get_all() const { return data; }
};

export template<typename T>
std::ostream& operator<<(std::ostream& out, const manuscript_container<T>& container) {
    if (container.size() == 0)
        std::cout << "Библиотека пуста\n";

    for (const auto& el : container.data) {
        out << el << '\n';
    }
    return out;
}
