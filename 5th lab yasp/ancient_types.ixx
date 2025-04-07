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
//manuscript_type manuscript_type_from_string(const std::string& str) {
//	if (str == "tablet") return manuscript_type::tablet;
//	if (str == "parchment") return manuscript_type::parchment;
//	if (str == "scroll") return manuscript_type::scroll;
//	if (str == "book") return manuscript_type::book;
//	if (str != "tablet" && str != "parchment" && str != "scroll" && str != "book") {
//		std::cout << "unknown type \n";
//		return manuscript_type::unknown;
//	}
//
//}

export manuscript_type manuscript_type_from_string(const std::string& str) {
	static const std::unordered_map<std::string, manuscript_type> type_map = {
		{"tablet", manuscript_type::tablet},
		{"parchment", manuscript_type::parchment},
		{"scroll", manuscript_type::scroll},
		{"book", manuscript_type::book}
	};

	auto it = type_map.find(str);
	//std::cout << typeid(it).name() << std::endl;
	if (it != type_map.end()) return it->second;

	std::cout << "unknown type \n";
	return manuscript_type::unknown;
}

export std::string trim(const std::string& s) {
	auto start = s.find_first_not_of(" \t\n\r");
	auto end = s.find_last_not_of(" \t\n\r");
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
			std::cin.clear(); // сброс ошибки
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Invalid input. Try again.\n";
			continue;
		}

		if constexpr (std::is_arithmetic<T>::value) {
			if (value < 0) {
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Negative values are not allowed. Try again.\n";
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
			std::cout << "Input cannot be empty. Try again.\n";
			continue;
		}
		return value;
	}
}

// чтение из потока
//export std::istream& operator>>(std::istream& in, manuscript& m) {
//	std::string line;
//	if (!std::getline(in, line)) return in;
//
//	std::istringstream iss(line);
//	std::string text, author, type_str, date_str;
//
//	if (!std::getline(iss, text, '|')) return in;
//	if (!std::getline(iss, author, '|')) return in;
//	if (!std::getline(iss, type_str, '|')) return in;
//	if (!std::getline(iss, date_str)) return in;
//
//	m.text = trim(text);
//	m.author = trim(author);
//	m.type = manuscript_type_from_string(trim(type_str));
//
//	int year, month, day;
//	char dash1, dash2;
//	std::istringstream date_stream(date_str);
//
//	// Повторный запрос даты, если она некорректна
//	while (!(date_stream >> year >> dash1 >> month >> dash2 >> day) || !is_valid_date(year, month, day)) {
//		std::cout << "Invalid date. Please enter a valid date (YYYY-MM-DD): ";
//		std::getline(in, date_str);
//		date_stream.clear();
//		date_stream.str(date_str);
//	}
//
//	m.creation_date = std::chrono::year{ year } / month / day;
//
//	return in;
//}


export std::istream& operator>>(std::istream& in, manuscript& m) {
	if (&in != &std::cin) {
		// старый режим — из файла или строки
		std::string line;
		if (!std::getline(in, line)) return in;

		std::istringstream iss(line);
		std::string text, author, type_str, date_str;

		if (!std::getline(iss, text, '|')) return in;
		if (!std::getline(iss, author, '|')) return in;
		if (!std::getline(iss, type_str, '|')) return in;
		if (!std::getline(iss, date_str)) return in;

		m.text = trim(text);
		m.author = trim(author);
		m.type = manuscript_type_from_string(trim(type_str));

		int year, month, day;
		char dash1, dash2;
		std::istringstream date_stream(date_str);

		if (!(date_stream >> year >> dash1 >> month >> dash2 >> day) || !is_valid_date(year, month, day)) {
			std::cerr << "Invalid date format in input.\n";
			in.setstate(std::ios::failbit);
			return in;
		}

		m.creation_date = std::chrono::year{ year } / month / day;
		return in;
	}

	// интерактивный ввод
	m.text = trim(read_and_check_string("Enter text: "));
	m.author = trim(read_and_check_string("Enter author: "));

	while (true) {
		std::string type_str = trim(read_and_check_string("Enter type (tablet, parchment, scroll, book): "));
		try {
			m.type = manuscript_type_from_string(type_str);
			break;
		}
		catch (...) {
			std::cout << "Invalid type. Try again.\n";
		}
	}

	int year = read_and_check<int>("Enter year: ");
	int month = read_and_check<int>("Enter month: ");
	int day = read_and_check<int>("Enter day: ");

	while (!is_valid_date(year, month, day)) {
		std::cout << "Invalid date. Try again.\n";
		year = read_and_check<int>("Enter year: ");
		month = read_and_check<int>("Enter month: ");
		day = read_and_check<int>("Enter day: ");
	}

	m.creation_date = std::chrono::year{ year } / month / day;
	return in;
}
