import manuscript_container;
import ancient_types;

#include <iostream>
#include <string>
#include <type_traits>

using container_t = manuscript_container<manuscript>;

void print_menu() {
	std::cout << "\n--- Ancient Manuscript Library ---\n";
	std::cout << "1. Load manuscripts from file\n";
	std::cout << "2. Show all manuscripts\n";
	std::cout << "3. Add manuscript (left/right)\n";
	std::cout << "4. Remove manuscript (left/right)\n";
	std::cout << "5. Update manuscript (left/right)\n";
	std::cout << "6. Select by author\n";
	std::cout << "7. Select by text length range\n";
	std::cout << "8. Select by creation date range\n";
	std::cout << "9. Save manuscripts to file\n";
	std::cout << "0. Exit\n";
}


	//template<typename T>
	//T read_and_check(const std::string& prompt) {
	//	T value;
	//	while (true) {
	//		std::cout << prompt;
	//		std::cin >> value;
	//
	//		if (std::cin.fail()) {
	//			std::cin.clear(); 
	//			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	//			std::cout << "Invalid input. Try again.\n";
	//			continue;
	//		}
	//
	//		if constexpr (std::is_arithmetic<T>::value) {
	//			if (value < 0) {
	//				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	//				std::cout << "Negative values are not allowed. Try again.\n";
	//				continue;
	//			}
	//		}
	//
	//		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	//		return value;
	//	}
	//}
	//
	//template<>
	//std::string read_and_check<std::string>(const std::string& prompt) {
	//	std::string line;
	//	while (true) {
	//		if (!prompt.empty()) std::cout << prompt;
	//		if (!std::getline(std::cin, line) || line.empty()) {
	//			std::cout << "Invalid input. Try again.\n";
	//			std::cin.clear();
	//		}
	//		else {
	//			return line;
	//		}
	//	}
	//}

manuscript input_manuscript() {
	manuscript m;
	std::cout << "Enter manuscript details (text | author | type | year-month-day):\n";
	std::cin >> m;  // использование перегруженного оператора >>
	return m;
}


int main() {
	container_t library;
	int choice;

	while (true) {
		print_menu();
		choice = read_and_check<int>("Choice: ");

		switch (choice) {
		case 1: {
			std::string filename = read_and_check<std::string>("Enter file name: ");
			library.read_from_file(filename);
			std::cout << "Loaded.\n";
			break;
		}
		case 2:
			std::cout << library;
			break;

		case 3: {
			bool to_left = read_and_check<bool>("Insert to left (1) or right (0)? ");
			auto m = input_manuscript();
			library.add(m, to_left);
			break;
		}

		case 4: {
			if (library.size() == 0) {
				std::cout << "Library is empty.\n";
				break;
			}
			bool from_left = read_and_check<bool>("Remove from left (1) or right (0)? ");
			size_t index = from_left ? 0 : library.size() - 1;
			library.remove_by_index(index);
			std::cout << "Removed.\n";
			break;
		}

		case 5: {
			if (library.size() == 0) {
				std::cout << "Library is empty.\n";
				break;
			}
			bool from_left = read_and_check<bool>("Update left (1) or right (0)? ");
			size_t index = from_left ? 0 : library.size() - 1;
			auto m = input_manuscript();
			library.update(index, m);
			std::cout << "Updated.\n";
			break;
		}

		case 6: {
			std::string author = read_and_check<std::string>("Enter author name: ");
			auto results = library.select_by_author(author);
			for (const auto& m : results)
				std::cout << m << "\n";
			break;
		}

		case 7: {
			size_t min_len = read_and_check<size_t>("Enter min text length: ");
			size_t max_len = read_and_check<size_t>("Enter max text length: ");
			auto results = library.select_by_length(min_len, max_len);
			for (const auto& m : results)
				std::cout << m << "\n";
			break;
		}

		case 8: {
			int y1 = read_and_check<int>("Enter start year: ");
			int m1 = read_and_check<int>("Enter start month: ");
			int d1 = read_and_check<int>("Enter start day: ");
			int y2 = read_and_check<int>("Enter end year: ");
			int m2 = read_and_check<int>("Enter end month: ");
			int d2 = read_and_check<int>("Enter end day: ");
			auto results = library.select_by_date(
				std::chrono::year{ y1 } / m1 / d1,
				std::chrono::year{ y2 } / m2 / d2);
			for (const auto& m : results)
				std::cout << m << "\n";
			break;
		}

		case 9: {
			std::string filename = read_and_check<std::string>("Enter file name: ");
			library.write_to_file(filename);
			std::cout << "Saved.\n";
			break;
		}

		case 0:
			std::cout << "Goodbye!\n";
			return 0;

		default:
			std::cout << "Invalid choice.\n";
			break;
		}
	}
}
