export module printable_container;

import manuscript_container;
import ancient_types;
import <ostream>;
import <iterator>;

export class printable_container : public manuscript_container<manuscript> {
public:
    using manuscript_container<manuscript>::manuscript_container;
    using manuscript_container<manuscript>::add;
    using manuscript_container<manuscript>::remove_by_index;
    using manuscript_container<manuscript>::update;
    using manuscript_container<manuscript>::selection;
    using manuscript_container<manuscript>::size;

    friend std::ostream& operator<<(std::ostream& out, const printable_container& pc) {
        std::ostringstream temp;
        std::ostream_iterator<manuscript> it(temp, "\n");
        const_cast<printable_container&>(pc).print(it);
        out << temp.str();
        return out;
    }
};