export module ancient_types;

import <string>;
import <chrono>;
import <iostream>;
import <unordered_map>;

export enum class manuscript_type {
    tablet,
    parchment,
    scroll,
    book,
    unknown
};

export struct manuscript {
private:
    std::string text;
    std::string author;
    manuscript_type type;
    std::chrono::year_month_day creation_date;

public:
    manuscript() = default;
    manuscript(std::string t, std::string a, manuscript_type ty, std::chrono::year_month_day date)
        : text(std::move(t)), author(std::move(a)), type(ty), creation_date(date) {}


    const std::string& get_text() const { return text; }
    const std::string& get_author() const { return author; }
    manuscript_type get_type() const { return type; }
    const std::chrono::year_month_day& get_creation_date() const { return creation_date; }

    bool operator==(const manuscript& other) const {
        return text == other.text &&
            author == other.author &&
            type == other.type &&
            creation_date == other.creation_date;
    }
};

std::string to_string(manuscript_type type) {
    switch (type) {
    case manuscript_type::tablet: return "tablet";
    case manuscript_type::parchment: return "parchment";
    case manuscript_type::scroll: return "scroll";
    case manuscript_type::book: return "book";
    default: return "unknown";
    }
}

export std::ostream& operator<<(std::ostream& out, const manuscript& m) {
    out << m.get_text() << " | "
        << m.get_author() << " | "
        << to_string(m.get_type()) << " | "
        << int(m.get_creation_date().year()) << "-"
        << unsigned(m.get_creation_date().month()) << "-"
        << unsigned(m.get_creation_date().day());
    return out;
}

export manuscript_type manuscript_type_from_string(const std::string& str) {
    static const std::unordered_map<std::string, manuscript_type> type_map = {
        {"tablet", manuscript_type::tablet},
        {"parchment", manuscript_type::parchment},
        {"scroll", manuscript_type::scroll},
        {"book", manuscript_type::book}
    };

    std::unordered_map<std::string, manuscript_type>::const_iterator it = type_map.find(str);
    if (it != type_map.end()) return it->second;

    std::cout << "Неизвестный тип \n";
    return manuscript_type::unknown;
}

export std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    size_t end = s.find_last_not_of(" \t\n\r");
    return (start == std::string::npos || end == std::string::npos)
        ? ""
        : s.substr(start, end - start + 1);
}

bool is_valid_date(int year, int month, int day) {
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    const int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (day > days_in_month[month - 1]) return false;

    if (month == 2 && day == 29) {
        if (!((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
            return false;  // Не високосный год
        }
    }

    return true;
}

export template<typename T>
T read_and_check(const std::string& prompt) {
    T value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неправильный ввод, повторите еще раз.\n";
            continue;
        }

        if constexpr (std::is_arithmetic<T>::value) {
            if (value < 0) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Отрицательный значения невозможны, повторите еще раз\n";
                continue;
            }
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}

export std::string read_and_check_string(const std::string& prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        if (value.empty()) {
            std::cout << "Ввод не может быть пустой, повторите еще раз\n";
            continue;
        }
        return value;
    }
}

export std::istream& operator>>(std::istream& in, manuscript& m) {
    if (&in != &std::cin) {
        std::string line;
        if (!std::getline(in, line)) return in;

        std::istringstream iss(line);
        std::string text, author, type_str, date_str;

        if (!std::getline(iss, text, '|')) return in;
        if (!std::getline(iss, author, '|')) return in;
        if (!std::getline(iss, type_str, '|')) return in;
        if (!std::getline(iss, date_str)) return in;

        std::string new_text = trim(text);
        std::string new_author = trim(author);
        manuscript_type new_type = manuscript_type_from_string(trim(type_str));

        int year, month, day;
        char dash1, dash2;
        std::istringstream date_stream(date_str);

        if (!(date_stream >> year >> dash1 >> month >> dash2 >> day) || !is_valid_date(year, month, day)) {
            std::cout << "Неправильный формат даты в файле.\n";
            in.setstate(std::ios::failbit);
            return in;
        }

        m = manuscript(new_text, new_author, new_type, std::chrono::year{ year } / month / day);
        return in;
    }
    //Ввод с клавиатуры
    std::string text = trim(read_and_check_string("Введите текст: "));
    std::string author = trim(read_and_check_string("Введите автора: "));

    manuscript_type type;
    while (true) {
        std::string type_str = trim(read_and_check_string("Введите тип (tablet, book, scroll, parchment): "));
        type = manuscript_type_from_string(type_str);
        if (type != manuscript_type::unknown) break;
        std::cout << "Неправильный ввод, повторите еще раз.\n";
    }

    int year = read_and_check<int>("Введите год: ");
    int month = read_and_check<int>("Введите месяц: ");
    int day = read_and_check<int>("Введите день: ");

    while (!is_valid_date(year, month, day)) {
        std::cout << "Неправильный ввод, повторите еще раз.\n";
        year = read_and_check<int>("Введите год: ");
        month = read_and_check<int>("Введите месяц: ");
        day = read_and_check<int>("Введите день: ");
    }

    m = manuscript(text, author, type, std::chrono::year{ year } / month / day);
    return in;
}