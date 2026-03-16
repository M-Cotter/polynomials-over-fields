#include<iostream>
#include<vector>
#include<string>
#include<array>
#include<algorithm>
#include "monomial.hpp"

#ifndef MVPOLY_HPP
#define MVPOLY_HPP

template<typename F, unsigned int n>
class MvPoly {
    private:
        std::vector<std::pair<F, Monomial<n>>> monoms;

    public:
        MvPoly(std::vector<std::pair<F, Monomial<n>>> m) : monoms(m) {}

        MvPoly() {monoms = {};}

        void print() {
            for (int i = 0; i < monoms.size(); i++) {
                std::cout << monoms[i].first << " ( " << monoms[i].second << " )";
                if (i != monoms.size() - 1) std::cout << " + ";
            }
            std::cout << "\n";
        }

        std::pair<F, Monomial<n>> nth_monom(int i) const {return monoms[i];}

        unsigned monoms_size() const {return monoms.size();}
        
        MvPoly operator+=(const MvPoly &o) {
            monoms.insert(monoms.end(), o.monoms.begin(), o.monoms.end());
            return *this;
        }

        MvPoly operator+(const MvPoly &o) const {
            MvPoly temp = *this;
            temp += o;
            return temp;
        }

        void mono_mul_ip(const Monomial<n> &o) {
            for (auto &m : monoms) m.second *= o;
        }

        MvPoly mono_mul(const Monomial<n> &o) const {
            MvPoly temp = *this;
            temp.mono_mul_ip(o);
            return temp;
        }

        MvPoly small_mul_ip(const F &o) {
            for (auto &m : monoms) m.first *= o;
        }

        MvPoly small_mul(const F &o) const {
            MvPoly temp = *this;
            temp.small_mul_ip(o);
            return temp;
        }
        
        MvPoly operator*(const MvPoly &o) const {
            MvPoly sum;
            for (int i = 0; i < monoms.size(); i++) {
                for (int j = 0; j < o.monoms.size(); j++) {
                    sum.monoms.push_back({monoms[i].first * o.monoms[j].first, monoms[i].second * o.monoms[j].second});
                }
            }
            return sum;
        }
        
        // Sort by monomial order and collect like terms
        void simp() {
            std::sort(monoms.begin(), monoms.end(), [](const std::pair<F, Monomial<n>> &a, const std::pair<F, Monomial<n>> &b) {return a.second < b.second;});

            for (int i = 1; i < monoms.size();) {
                if (monoms[i].second == monoms[i-1].second) {
                    monoms[i-1].first += monoms[i].first;
                    monoms.erase(monoms.begin() + i);
                } else {
                    i++;
                }
                if (monoms[i].first == 0) monoms.erase(monoms.begin() + i);
            }
        }
        
};

template<typename F, unsigned int n>
std::ostream& operator<<(std::ostream& os, const MvPoly<F, n> &obj) {
    for (int i = 0; i < obj.monoms_size(); i++) {
        os << obj.nth_monom(i).first << " ( " << obj.nth_monom(i).second << " )";
            if (i != obj.monoms_size() - 1) os << " + ";
        }
    os << "\n";
    return os;
}

#endif