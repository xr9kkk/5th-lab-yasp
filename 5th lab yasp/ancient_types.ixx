export module ancient_types;

import <string>;
import <chrono>;
import <iostream>;

export enum class manuscript_type {
	tablet,
	parchment,
	scroll,
	book,
	unknown
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


std::string to_string(manuscript_type type)
{
	switch (type)
	{
	case manuscript_type::tablet:return "tablet";
	case manuscript_type::parchment:return "parchment";
	case manuscript_type::scroll:return "scroll";
	case manuscript_type::book:return "book";
	default: return "unknown";
	}
}

export std::ostream& operator << (std::ostream& out, const manuscript& m)
{
	out << m.text << " | "
		<< m.author << " | "
		<< to_string(m.type) << " | "
		<< int(m.creation_date.year()) << "-"
		<< unsigned(m.creation_date.month()) << "-"
		<< unsigned(m.creation_date.day());
	return out;
}

// преобразование строки в тип
manuscript_type manuscript_type_from_string(const std::string& str) {
	if (str == "tablet") return manuscript_type::tablet;
	if (str == "parchment") return manuscript_type::parchment;
	if (str == "scroll") return manuscript_type::scroll;
	if (str == "book") return manuscript_type::book;
	if (str != "book" || "scroll" || "parchment" || "tablet")
	{
		std::cout << "unknown type \n";
		return manuscript_type::unknown;
	}
}
// чтение из потока
export std::istream& operator>>(std::istream& in, manuscript& m) {
	std::string line;
	if (!std::getline(in, line)) return in;

	std::istringstream iss(line);
	std::string text, author, type_str, date_str;

	if (!std::getline(iss, text, '|')) return in;
	if (!std::getline(iss, author, '|')) return in;
	if (!std::getline(iss, type_str, '|')) return in;
	if (!std::getline(iss, date_str)) return in;

	m.text = std::string(text.begin(), text.end() - (text.back() == ' ' ? 1 : 0)); // trim
	m.author = std::string(author.begin() + (author.front() == ' ' ? 1 : 0), author.end());
	m.type = manuscript_type_from_string(type_str.substr(type_str.find_first_not_of(' ')));

	int year, month, day;
	char dash1, dash2;
	std::istringstream date_stream(date_str);
	if (date_stream >> year >> dash1 >> month >> dash2 >> day) {
		m.creation_date = std::chrono::year{ year } / month / day;
	}

	return in;
}
