import manuscript_container;
import ancient_types;

#include <iostream>
#include <string>

using container_t = manuscript_container<manuscript>;

void print_menu() {
	std::cout << "\n--- Библиотека древних рукописей ---\n";
	std::cout << "1. Загрузить рукописи из файла\n";
	std::cout << "2. Показать все рукописи\n";
	std::cout << "3. Добавить рукопись (влево/вправо)\n";
	std::cout << "4. Удалить рукопись (влево/вправо)\n";
	std::cout << "5. Обновить рукопись (влево/вправо)\n";
	std::cout << "6. Поиск по автору\n";
	std::cout << "7. Поиск по длине текста\n";
	std::cout << "8. Поиск по дате создания\n";
	std::cout << "9. Сохранить рукописи в файл\n";
	std::cout << "0. Выход\n";
}

manuscript input_manuscript() {
	manuscript m;
	std::cout << "Введите данные рукописи (текст | автор | тип | год-месяц-день):\n";
	std::cin >> m;
	return m;
}

int main() {
	setlocale(LC_ALL, "ru");
	container_t library;
	int choice;

	while (true) {
		print_menu();
		choice = read_and_check<int>("Выбор: ");

		switch (choice) {
		case 1: {
			std::string filename = read_and_check<std::string>("Введите имя файла: ");
			library.read_from_file(filename);
			std::cout << "Загрузка завершена.\n";
			break;
		}
		case 2:
			std::cout << library;
			break;

		case 3: {
			bool to_left = read_and_check<bool>("Добавить влево (1) или вправо (0)? ");
			auto m = input_manuscript();
			library.add(m, to_left);
			break;
		}

		case 4: {
			if (library.size() == 0) {
				std::cout << "Библиотека пуста.\n";
				break;
			}
			bool from_left = read_and_check<bool>("Удалить слева (1) или справа (0)? ");
			size_t index = from_left ? 0 : library.size() - 1;
			library.remove_by_index(index);
			std::cout << "Удалено.\n";
			break;
		}

		case 5: {
			if (library.size() == 0) {
				std::cout << "Библиотека пуста.\n";
				break;
			}
			bool from_left = read_and_check<bool>("Обновить слева (1) или справа (0)? ");
			size_t index = from_left ? 0 : library.size() - 1;
			auto m = input_manuscript();
			library.update(index, m);
			std::cout << "Обновлено.\n";
			break;
		}

		case 6: {
			if (library.size() == 0) {
				std::cout << "Библиотека пуста.\n";
				break;
			}
			std::string author = read_and_check<std::string>("Введите имя автора: ");
			auto results = library.select_by_author(author);
			for (const auto& m : results)
				std::cout << m << "\n";
			break;
		}

		case 7: {
			if (library.size() == 0) {
				std::cout << "Библиотека пуста.\n";
				break;
			}
			size_t min_len = read_and_check<size_t>("Минимальная длина текста: ");
			size_t max_len = read_and_check<size_t>("Максимальная длина текста: ");
			auto results = library.select_by_length(min_len, max_len);
			for (const auto& m : results)
				std::cout << m << "\n";
			break;
		}

		case 8: {
			if (library.size() == 0) {
				std::cout << "Библиотека пуста.\n";
				break;
			}
			int y1 = read_and_check<int>("Начальный год: ");
			int m1 = read_and_check<int>("Начальный месяц: ");
			int d1 = read_and_check<int>("Начальный день: ");
			int y2 = read_and_check<int>("Конечный год: ");
			int m2 = read_and_check<int>("Конечный месяц: ");
			int d2 = read_and_check<int>("Конечный день: ");
			auto results = library.select_by_date(
				std::chrono::year{ y1 } / m1 / d1,
				std::chrono::year{ y2 } / m2 / d2);
			for (const auto& m : results)
				std::cout << m << "\n";
			break;
		}

		case 9: {
			if (library.size() == 0) {
				std::cout << "Библиотека пуста.\n";
				break;
			}
			std::string filename = read_and_check<std::string>("Введите имя файла: ");
			library.write_to_file(filename);
			std::cout << "Сохранено.\n";
			break;
		}

		case 0:
			std::cout << "До свидания!\n";
			return 0;

		default:
			std::cout << "Неверный выбор.\n";
			break;
		}
	}
}
