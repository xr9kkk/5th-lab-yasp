import manuscript_container;
import ancient_types;

#include <iostream>
#include <string>
#include <fstream>

using container_t = manuscript_container<manuscript>;


void print_to_file(container_t& container);
void read_from_file(container_t& container);

void print_menu() {
	std::cout << "\n--- ���������� ������� ��������� ---\n";
	std::cout << "1. ��������� �������� �� �����\n";
	std::cout << "2. �������� ��� ��������\n";
	std::cout << "3. �������� �������� (�����/������)\n";
	std::cout << "4. ������� �������� (�����/������)\n";
	std::cout << "5. �������� �������� (�����/������)\n";
	std::cout << "6. ����� �� ������\n";
	std::cout << "7. ����� �� ����� ������\n";
	std::cout << "8. ����� �� ���� ��������\n";
	std::cout << "9. ��������� �������� � ����\n";
	std::cout << "0. �����\n";
}

manuscript input_manuscript() {
	manuscript m;
	std::cout << "������� ������ �������� (����� | ����� | ��� | ���-�����-����):\n";
	std::cin >> m;
	return m;
}

int main() {
	setlocale(LC_ALL, "ru");
	container_t library;
	int choice;

	while (true) {
		print_menu();
		choice = read_and_check<int>("�����: ");

		switch (choice) {
		case 1: {
			std::string filename = read_and_check<std::string>("������� ��� �����: ");
			library.read_from_file(filename);
			/*std::cout << "�������� ���������.\n";*/
			break;
		}
		case 2:
			std::cout << library;
			break;

		case 3: {
			bool to_left = read_and_check<bool>("�������� ����� (1) ��� ������ (0)? ");
			manuscript m = input_manuscript();
			library.add(m, to_left);
			break;
		}

		case 4: {
			if (library.size() == 0) {
				std::cout << "���������� �����.\n";
				break;
			}
			
			bool from_left = read_and_check<bool>("������� ����� (1) ��� ������ (0)? ");
			size_t index = from_left ? 0 : library.size() - 1;
			library.remove_by_index(index);
			std::cout << "�������.\n";
			break;
		}

		case 5: {
			if (library.size() == 0) {
				std::cout << "���������� �����.\n";
				break;
			}
			bool from_left = read_and_check<bool>("�������� ����� (1) ��� ������ (0)? ");
			size_t index = from_left ? 0 : library.size() - 1;
			manuscript m = input_manuscript();
			library.update(index, m);
			std::cout << "���������.\n";
			break;
		}

		

		case 6: {
			if (library.size() == 0) {
				std::cout << "���������� �����.\n";
				break;
			}
			std::string author = read_and_check<std::string>("������� ��� ������: ");
			auto results = library.selection([&author](const manuscript& m) {
				return m.get_author() == author;
				});
			

			std::cout << results;
			break;
		}

		case 7: {
			if (library.size() == 0) {
				std::cout << "���������� �����.\n";
				break;
			}
			size_t min_len = read_and_check<size_t>("����������� ����� ������: ");
			size_t max_len = read_and_check<size_t>("������������ ����� ������: ");
			auto results = library.selection([min_len, max_len](const manuscript& m) {
				size_t len = m.get_text().size();
				return len >= min_len && len <= max_len;
				});
			
			std::cout << results;
			break;
		}

		case 8: {
			if (library.size() == 0) {
				std::cout << "���������� �����.\n";
				break;
			}
			int y1 = read_and_check<int>("��������� ���: ");
			int m1 = read_and_check<int>("��������� �����: ");
			int d1 = read_and_check<int>("��������� ����: ");
			int y2 = read_and_check<int>("�������� ���: ");
			int m2 = read_and_check<int>("�������� �����: ");
			int d2 = read_and_check<int>("�������� ����: ");
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
				std::cout << "���������� �����.\n";
				break;
			}
			print_to_file(library);
			std::cout << "���������.\n";
			break;
		}

		case 0:
			std::cout << "�� ��������!\n";
			return 0;

		default:
			std::cout << "�������� �����.\n";
			break;
		}
	}
}

void print_to_file(container_t& container)
{
	std::string filename = read_and_check<std::string>("������� ��� �����: ");
	std::ofstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "������ �������� ����� ��� ������!\n";
		return;
	}
	try
	{
		file << container;
	}
	catch (const std::exception& e)
	{
		std::cerr << "������ ��� ������ � ����: " << e.what() << '\n';
	}
	file.close();
}

void read_from_file(container_t& container)
{
	
	
}
