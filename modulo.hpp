#ifndef MODULO_HPP
#define MODULO_HPP

#include<iostream>

template <int n>
class Modulo {
    private:
        int val;

    public:
        Modulo(int v) {val = (v % n + n) % n;}

        int get_val() const {return val;}
        
        void print() {std::cout << val << "\n";}

        Modulo operator+=(const Modulo &o) {val += o.val; val = (val % n + n) % n; return *this;}

        Modulo operator-=(const Modulo &o) {val -= o.val; val = (val % n + n) % n; return *this;}

        Modulo operator*=(const Modulo &o) {val *= o.val; val = (val % n + n) % n; return *this;}

        Modulo operator+(const Modulo &o) const {Modulo temp = *this; temp += o; return temp;}

        Modulo operator-(const Modulo &o) const {Modulo temp = *this; temp -= o; return temp;}

        Modulo operator*(const Modulo &o) const {Modulo temp = *this; temp *= o; return temp;}

        bool operator==(const Modulo &o) const {return o.val == val;}

        Modulo inv() {
            if (val == 0) throw std::runtime_error("Div by 0");
            int old_r = val, r = n, q, temp;
            int old_s = 1, s = 0;
            int old_t = 0, t = 1;
            while (r != 0) {
                q = old_r / r;
                temp = old_r; old_r = r; r = temp - q * r;
                temp = old_s; old_s = s; s = temp - q * s;
                temp = old_t; old_t = t; t = temp - q * t;
            }
            return ((old_s) % n + n) % n;
        }

        Modulo operator/=(const Modulo &o) {
            if (o.val == 0) throw std::runtime_error("Div by 0");
            *this *= inv(o);
            return *this;
        }

        Modulo operator/(const Modulo &o) const {
            if (o.val == 0) throw std::runtime_error("Div by 0");
            Modulo temp = *this;
            temp *= inv(o);
            return temp;
        }

        Modulo operator^(unsigned int e) const {
            Modulo power(1);
            Modulo square = *this;
            while (e > 0) {
                if (e & 1) {
                    power *= square;
                }
                square *= square;
                e >>= 1;
            }
            return power;
        }
};

template<int n>
std::ostream& operator<<(std::ostream& os, const Modulo<n>& obj) {
    os << std::to_string(obj.get_val());
    return os;
}

#endif