#include "seq.h"
#include "blank.h"
#include "transition_map.h"

namespace tree_sitter {
    namespace rules {
        Seq::Seq(rule_ptr left, rule_ptr right) : left(left), right(right) {};

        rule_ptr seq(const std::initializer_list<rule_ptr> &rules) {
            return build_binary_rule_tree<Seq>(rules);
        }

        TransitionMap<Rule> Seq::transitions() const {
            return left->transitions().map<Rule>([&](rule_ptr left_rule) -> rule_ptr {
                if (typeid(*left_rule) == typeid(Blank))
                    return right;
                else
                    return seq({ left_rule, right });
            });
        }
        
        bool Seq::operator==(const Rule &rule) const {
            const Seq *other = dynamic_cast<const Seq *>(&rule);
            return other && (*other->left == *left) && (*other->right == *right);
        }
        
        std::string Seq::to_string() const {
            return std::string("(seq ") + left->to_string() + " " + right->to_string() + ")";
        }
    }
}
