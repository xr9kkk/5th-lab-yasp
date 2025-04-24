import manuscript_container;
import ancient_types;

#include <iostream>
#include <string>
#include <fstream>

using container_t = manuscript_container<manuscript>;


void print_to_file(container_t& container);
void read_from_file(container_t& container);

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
			/*std::cout << "Загрузка завершена.\n";*/
			break;
		}
		case 2:
			std::cout << library;
			break;

		case 3: {
			bool to_left = read_and_check<bool>("Добавить влево (1) или вправо (0)? ");
			manuscript m = input_manuscript();
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
			manuscript m = input_manuscript();
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
			auto results = library.selection([&author](const manuscript& m) {
				return m.get_author() == author;
				});
			

			std::cout << results;
			break;
		}

		case 7: {
			if (library.size() == 0) {
				std::cout << "Библиотека пуста.\n";
				break;
			}
			size_t min_len = read_and_check<size_t>("Минимальная длина текста: ");
			size_t max_len = read_and_check<size_t>("Максимальная длина текста: ");
			auto results = library.selection([min_len, max_len](const manuscript& m) {
				size_t len = m.get_text().size();
				return len >= min_len && len <= max_len;
				});
			
			std::cout << results;
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
			auto results = library.selection(
				[y1, m1, d1, y2, m2, d2](const manuscript& m) {
					auto date = m.get_creation_date();
					return date >= (std::chrono::year{ y1 } / m1 / d1) &&
						date <= (std::chrono::year{ y2 } / m2 / d2);
				});

			std::cout << results;
			break;
		}

		case 9: {
			if (library.size() == 0) {
				std::cout << "Библиотека пуста.\n";
				break;
			}
			print_to_file(library);
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

void print_to_file(container_t& container)
{
	std::string filename = read_and_check<std::string>("Введите имя файла: ");
	std::ofstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Ошибка открытия файла для записи!\n";
		return;
	}
	try
	{
		file << container;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Ошибка при записи в файл: " << e.what() << '\n';
	}
	file.close();
}

void read_from_file(container_t& container)
{
	
	
}
