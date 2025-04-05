import <iostream>;
import <chrono>;
import ancient_types;
import manuscript_container;

int main() {
    manuscript_container<manuscript> library;

    manuscript m1{
        "Behold the tablet of ancient wisdom.",
        "Aurelian",
        manuscript_type::tablet,
        std::chrono::year{1420} / 3 / 12
    };

    manuscript m2{
        "Scroll of the forgotten rituals.",
        "Seraphinus",
        manuscript_type::scroll,
        std::chrono::year{1555} / 9 / 30
    };

    library.add(m1, true);   // monk adds to the left
    library.add(m2, false);  // monk adds to the right

    library.print();

    auto filtered = library.select_by_author("Seraphinus");
    for (const auto& m : filtered) {
        std::cout << "Found by author: " << m.text << "\n";
    }

    return 0;
}
