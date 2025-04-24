export module temple;

#include <string>

import manuscript_container;
import ancient_types;
import printable_container;

using container_t = manuscript_container<manuscript>;
using cont_p = printable_container;

struct Temple
{
private:

	std::string name{};
	std::string monk_name{};
	int diligence;
	cont_p manuscripts;
public:

	Temple() = default;
	Temple(const std::string& name, const std::string& monk_name, int diligence)
		: name(name), monk_name(monk_name), diligence(diligence) {}




	void add_manuscript(const manuscript& m) { manuscripts.add(m); }

	bool remove_manuscript(size_t index) { return manuscripts.remove_by_index(index); }

	container_t& get_manuscript() { return manuscripts; }

	const std::string& get_name() const { return name; }
	const std::string& get_monk_name() const { return monk_name; }
	int get_diligence() const { return diligence; }

	void update_info(const std::string& new_name, const std::string& new_monk_name, int new_diligence) {
		name = new_name;
		monk_name = new_monk_name;
		diligence = new_diligence;
	}

	friend std::ostream& operator<<(std::ostream& out, const Temple& t) {
		out << t.name << '\n'
			<< t.monk_name << '\n'
			<< t.diligence << '\n'
			<< t.manuscripts;
		return out;
	}

	friend std::istream& operator>>(std::istream& in, Temple& t) {
		std::getline(in, t.name);
		std::getline(in, t.monk_name);
		in >> t.diligence;
		in.ignore();
		in >> t.manuscripts;
		return in;
	}

	bool will_copy() const {
		return std::rand() % 100 < diligence;
	}
};