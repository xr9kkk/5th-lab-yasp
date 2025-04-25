export module manuscript_serialization;

#include "nlohmann.hpp"

import <string>;
import <sstream>;
import <iomanip>;
import <chrono>;
import ancient_types;

using json = nlohmann::json;

inline std::string to_date_string(const std::chrono::year_month_day& ymd) {
    std::ostringstream oss;
    oss << int(ymd.year()) << "-"
        << std::setw(2) << std::setfill('0') << unsigned(ymd.month()) << "-"
        << std::setw(2) << std::setfill('0') << unsigned(ymd.day());
    return oss.str();
}

inline std::chrono::year_month_day from_date_string(const std::string& date_str) {
    std::istringstream iss(date_str);
    int y, m, d;
    char dash1, dash2;
    if (!(iss >> y >> dash1 >> m >> dash2 >> d) || dash1 != '-' || dash2 != '-') {
        throw std::runtime_error("Неверный формат даты: " + date_str);
    }
    return std::chrono::year{ y } / m / d;
}

export inline void to_json(json& j, const manuscript& m) {
    j = json{
        {"text", m.get_text()},
        {"author", m.get_author()},
        {"type", static_cast<int>(m.get_type())},
        {"creation_date", to_date_string(m.get_creation_date())}
    };
}

export inline void from_json(const json& j, manuscript& m) {
    m = manuscript(
        j.at("text").get<std::string>(),
        j.at("author").get<std::string>(),
        static_cast<manuscript_type>(j.at("type").get<int>()),
        from_date_string(j.at("creation_date").get<std::string>())
    );
}



