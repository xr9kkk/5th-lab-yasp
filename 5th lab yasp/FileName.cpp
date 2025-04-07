import manuscript_container;
import ancient_types;

#include <iostream>
#include <string>

using container_t = manuscript_container<manuscript>;

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
			std::cout << "�������� ���������.\n";
			break;
		}
		case 2:
			std::cout << library;
			break;

		case 3: {
			bool to_left = read_and_check<bool>("�������� ����� (1) ��� ������ (0)? ");
			auto m = input_manuscript();
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
			auto m = input_manuscript();
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
			auto results = library.select_by_author(author);
			for (const auto& m : results)
				std::cout << m << "\n";
			break;
		}

		case 7: {
			if (library.size() == 0) {
				std::cout << "���������� �����.\n";
				break;
			}
			size_t min_len = read_and_check<size_t>("����������� ����� ������: ");
			size_t max_len = read_and_check<size_t>("������������ ����� ������: ");
			auto results = library.select_by_length(min_len, max_len);
			for (const auto& m : results)
				std::cout << m << "\n";
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
			auto results = library.select_by_date(
				std::chrono::year{ y1 } / m1 / d1,
				std::chrono::year{ y2 } / m2 / d2);
			for (const auto& m : results)
				std::cout << m << "\n";
			break;
		}

		case 9: {
			if (library.size() == 0) {
				std::cout << "���������� �����.\n";
				break;
			}
			std::string filename = read_and_check<std::string>("������� ��� �����: ");
			library.write_to_file(filename);
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
