import <iostream>;
import <chrono>;
import ancient_types;
import manuscript_container;

int main() {
    manuscript_container<manuscript> library;
    library.read_from_file("file.txt");
    std::cout << library;
    
    library.write_to_file("out.txt");

    
    return 0;
}
