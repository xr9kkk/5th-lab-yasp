export module Temple_manager;

#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <random>
#include "nlohmann.hpp"
#include <iostream>

import temple;

using json = nlohmann::json;

export class TempleManager {
private:
    std::vector<Temple> _temples;
    const std::string _storage_file = "temples.json";

public:
    TempleManager() {
        load_state();
    }

    ~TempleManager() {
        save_state();
    }

    void add_temple(const Temple& temple) {
        _temples.push_back(temple);
    }

    bool remove_temple(const std::string& name) {
        auto it = std::remove_if(_temples.begin(), _temples.end(),
            [&](const Temple& t) { return t.get_name() == name; });
        if (it != _temples.end()) {
            _temples.erase(it, _temples.end());
            return true;
        }
        return false;
    }

    Temple* get_temple(const std::string& name) {
        for (auto& t : _temples) {
            if (t.get_name() == name) return &t;
        }
        return nullptr;
    }

    void move_manuscript(const std::string& from_name, const std::string& to_name, size_t index) {
        Temple* from = get_temple(from_name);
        Temple* to = get_temple(to_name);
        if (!from || !to) return;

        auto all = from->get_manuscript().selection([](const manuscript&) { return true; });
        if (index >= all.size()) return;

        size_t i = 0;
        for (const auto& m : all) {
            if (i++ == index) {
                from->get_manuscript().remove_by_index(index);  // Удаляем в from
                to->add_manuscript(m);                          // Добавляем в to
                if (from->will_copy()) {
                    from->add_manuscript(m);                   // Копируем обратно при необходимости
                }
                break;
            }
        }
    }

    void save_state() const {
        json j;
        for (const auto& temple : _temples) {
            json t;
            t["name"] = temple.get_name();
            t["monk"] = temple.get_monk_name();
            t["diligence"] = temple.get_diligence();

            // Сохраняем рукописи вручную
            json manuscripts_json = json::array();
            for (const auto& m : temple.get_manuscript().selection([](const manuscript&) { return true; })) {
                manuscripts_json.push_back({
                    {"author", m.get_author()},
                    {"date", m.get_date()},
                    {"length", m.get_length()},
                    {"text", m.get_text()},
                    {"type", static_cast<int>(m.get_type())}
                    });
            }
            t["manuscripts"] = manuscripts_json;

            j.push_back(t);
        }

        try {
            std::ofstream out(_storage_file);
            if (!out) throw std::ios_base::failure("Ошибка открытия файла для записи");
            out << j.dump(4);
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка при сохранении состояния: " << e.what() << "\n";
        }
    }

    void load_state() {
        std::ifstream in(_storage_file);
        if (!in.is_open()) return;

        try {
            json j;
            in >> j;
            _temples.clear();

            for (const auto& t : j) {
                Temple temple(
                    t.at("name").get<std::string>(),
                    t.at("monk").get<std::string>(),
                    t.at("diligence").get<int>()
                );
                temple.load_manuscript(t.at("manuscripts"));
                _temples.push_back(temple);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка при загрузке состояния: " << e.what() << "\n";
        }
    }

    void generate_report(std::ostream& out) {
        size_t total_manuscripts = 0, tablet = 0, parchment = 0, scroll = 0, book = 0;
        std::unordered_set<std::string> unique_texts;

        out << "=== Отчёт для Великого Императора ===\n";
        for (const auto& temple : _temples) {
            size_t count = temple.get_manuscript().size();
            out << "Храм: " << temple.get_name() << ", Монах: " << temple.get_monk_name()
                << ", Рукописей: " << count << '\n';
            total_manuscripts += count;

            for (const auto& m : temple.get_manuscript().selection([](const manuscript&) { return true; })) {
                unique_texts.insert(m.get_text());
                switch (m.get_type()) {
                case manuscript_type::tablet:    ++tablet; break;
                case manuscript_type::parchment: ++parchment; break;
                case manuscript_type::scroll:    ++scroll; break;
                case manuscript_type::book:      ++book; break;
                }
            }
        }

        out << "\nСуммарное число рукописей: " << total_manuscripts << "\n"
            << "Суммарное количество храмов: " << _temples.size() << "\n"
            << "Уникальных рукописей: " << unique_texts.size() << "\n"
            << "Табличек: " << tablet << ", Пергаментов: " << parchment
            << ", Свитков: " << scroll << ", Книг: " << book << "\n";
    }
};
