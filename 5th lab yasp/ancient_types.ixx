export module ancient_types;

import <string>;
import <chrono>;

export enum class manuscript_type {
    tablet,
    parchment,
    scroll,
    book
};

export struct manuscript {
    std::string text;
    std::string author;
    manuscript_type type;
    std::chrono::year_month_day creation_date;

    bool operator==(const manuscript& other) const {
        return text == other.text &&
            author == other.author &&
            type == other.type &&
            creation_date == other.creation_date;
    }
};
