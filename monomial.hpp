#ifndef MONOMIAL_HPP
#define MONOMIAL_HPP

#include<iostream>
#include<vector>
#include<string>
#include<array>

std::vector<std::string> mono_alpha = {"x", "y", "z", "w", "q", "r", "s", "t", "u", "v"};

std::string var_name (int n) {
    if (n < mono_alpha.size()) return mono_alpha[n];
    return "(x_" + std::to_string(n - mono_alpha.size()) + ")";
}

template <int n>
class Monomial {
    private:
        std::array<unsigned, n> exps;

    public:
        Monomial(std::array<unsigned, n> e) : exps(e) {}

        Monomial() {
            int exps[n];
        }
        
        void print_plain() const {
            for (unsigned i : exps) {
                std::cout << i << " ";
            }
            std::cout << "\n";
        }

        std::string to_string() const {
            std::string rString;
            for (int i = 0; i < n; i++) {
                if (exps[i] != 0) {
                    if (exps[i] != 1) rString += (var_name(i) + "^" + std::to_string(exps[i]));
                    else rString += var_name(i);
                    if (i != n - 1) rString += " ";
                }
            }
            return rString;
        }
        
        void print() const {
            std::cout << to_string() << "\n";
        }

        Monomial operator*=(const Monomial &o) {
            for (int i = 0; i < n; i++) exps[i] += o.exps[i];
            return *this;
        }

        Monomial operator*(const Monomial &o) const {
            Monomial temp = *this;
            temp *= o;
            return temp;
        }

        bool operator==(const Monomial &o) {
            return exps == o.exps;
        }

        // Comparison is wrt the lexicographical monomial ordering
        bool operator>(const Monomial &o) const {
            for (int i = 0; i < n; i++) {
                if (exps[i] > o.exps[i]) return true;
                else if (exps[i] < o.exps[i]) return false;
            }
            return false;
        }

        bool operator<(const Monomial &o) const {
            for (int i = 0; i < n; i++) {
                if (exps[i] < o.exps[i]) return true;
                else if (exps[i] > o.exps[i]) return false;
            }
            return false;
        }

        bool operator<=(const Monomial &o) const {
            return !(this > o);
        }

        bool operator>=(const Monomial &o) const {
            return !(this < o);
        }

        static Monomial lcm(const Monomial &l, const Monomial &r) {
            Monomial<n> temp;
            for (int i = 0; i < n; i++) {
                temp.exps[i] = std::max(l.exps[i], r.exps[i]);
            }
            return temp;
        }

        static Monomial gcd(const Monomial &l, const Monomial &r) {
            Monomial<n> temp;
            for (int i = 0; i < n; i++) {
                temp.exps[i] = std::min(l.exps[i], r.exps[i]);
            }
            return temp;
        }

        bool dvd(const Monomial &o) const {
            for (int i = 0; i < n; i++) {
                if (exps[i] > o.exps[i]) return false;
            }
            return true;
        }
};

template<int n>
std::ostream& operator<<(std::ostream& os, const Monomial<n> &obj) {
    os << obj.to_string();
    return os;
}

#endif