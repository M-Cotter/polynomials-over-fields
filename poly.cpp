/*
poly.cpp

Implementation of polynomial algorithms over fields.
*/

#include<iostream>
#include<vector>
#include<bit>

// This template makes sure a field has the requisite operations and constructors for the algorithms in Poly to work
// To use the print function there must be << as well

template <typename F>
concept Field = requires(F a, F b) {
    {a == b};
    {F{0}};
    {F{1}};
    {a + b};
    {a - b};
    {a / b};
};

template <Field F>
class Poly {
    private:
        std::vector<F> coeffs;
        
    public:
    // Constructor from a vector of field elements (smallest index to largest)
        Poly(const std::vector<F> c) : coeffs(c) {
            while (coeffs.size() > 1 && coeffs.back() == 0) {
                    coeffs.pop_back();
            }

            if (coeffs.empty()) {
                coeffs.push_back(0);
            }
        }

        // Returns nth coefficient
        F nth_coeff(unsigned int n) {
            if (n > coeffs.size()) {
                std::cout << "bad";
            }
            return coeffs[n];
        }

        // Returns leading (largest) coefficient
        F lead_coeff() {
            return coeffs.back();
        }

        // Removes leading zeros in place, and will convert the empty coefficient vector to {0}
        void normalize() {
            while (coeffs.size() > 1 && coeffs.back() == 0) {
                    coeffs.pop_back();
            }

            if (coeffs.empty()) {
                coeffs.push_back(0);
            }
        }

        // Plain printing (just coefficients)
        void print_plain() {
            for (F i : coeffs) {
                std::cout << i << " ";
            }
            std::cout << "\n";
        }

        // Prints fancily with coefficients and summation
        void print() { 
            std::cout << coeffs[0];
            for (unsigned int i = 1; i < coeffs.size() - 1; i++) {
                if (coeffs[i] == 1) {
                    std::cout << " + x^" << i;
                } else if (coeffs[i] != 0) {
                    std::cout << " + " << coeffs[i] << "x^" << i;
                }
            }
            std::cout << "\n";
        }

        // Multiplies a polynomial in place by a field element, in the obvious sense
        void small_mul(F x) {
            // This only works if 0 is unique
            if (x == 0) {
                coeffs = {0};
            }

            for (unsigned int i = 0; i < coeffs.size(); i++) {
                coeffs[i] *= x;
            }
        }

        // Normalizes a polynomial in place, so its leading coefficient is 1
        void monic() {
            if (coeffs.back() == 0) {
                throw std::runtime_error("Cannot make 0 a monic polynomial");
            }

            small_mul(1 / coeffs.back());
        }

        // Basic arithmetic operations
        Poly operator+(const Poly &o) {
            int mSize = std::max(o.coeffs.size(), coeffs.size());
            std::vector<F> sumCoeffs(mSize, 0);

            for (int i = 0; i < mSize; i++) {
                sumCoeffs[i] = o.coeffs[i] + coeffs[i];
            }
            
            return Poly(sumCoeffs);
        }

        Poly operator-(const Poly &o) {
            int mSize = std::max(o.coeffs.size(), coeffs.size());
            std::vector<F> sumCoeffs(mSize, 0);

            for (int i = 0; i < mSize; i++) {
                sumCoeffs[i] = coeffs[i] - o.coeffs[i];
            }

            return Poly(sumCoeffs);
        }

        Poly &operator+=(const Poly &b) {
            if (b.coeffs.size() > coeffs.size()) {
                coeffs.insert(coeffs.end(), b.coeffs.size() - coeffs.size(), 0);
            }

            for (unsigned int i = 0; i < coeffs.size(); i++) {
                coeffs[i] += b.coeffs[i];
            }

            normalize();
            return *this;
        }

        Poly &operator-=(const Poly &b) {
            if (b.coeffs.size() > coeffs.size()) {
                coeffs.insert(coeffs.end(), b.coeffs.size() - coeffs.size(), 0);
            }

            for (unsigned int i = 0; i < coeffs.size(); i++) {
                coeffs[i] -= b.coeffs[i];
            }

            normalize();
            return *this;
        }

        Poly operator*(const Poly &o) {
            int mSize = o.coeffs.size() + coeffs.size();
            std::vector<F> prodCoeffs(mSize, 0);

            for (unsigned int i = 0; i < coeffs.size(); i++) {
                for (unsigned int j = 0; j < o.coeffs.size(); j++) {
                    prodCoeffs[i + j] += coeffs[i] * o.coeffs[j];
                }
            }

            return Poly(prodCoeffs);
        }

        Poly operator*=(const Poly &b) {
            *this = *this * b;
            return *this;
        }

        Poly operator/(const Poly &o) {
            if (o.coeffs == std::vector<F>{0}) {
                throw std::runtime_error("division by 0");
            }

            if (coeffs.size() < o.coeffs.size()) {
                return Poly({0});
            }

            std::vector<F> remainder = coeffs;
            std::vector<F> quotient(coeffs.size() - o.coeffs.size() + 1, 0);
            F int_quot;

            for (int i = coeffs.size() - o.coeffs.size(); i >= 0; i--) {
                int_quot = remainder[i + o.coeffs.size() - 1] / o.coeffs.back();
                quotient[i] = int_quot;

                for (int j = o.coeffs.size() - 1; j >= 0; j--) {
                    remainder[i + j] = remainder[i + j] - o.coeffs[j] * int_quot;
                }
            }
            
            return Poly(quotient);
        }

        Poly &operator/=(const Poly &b) {
            *this = *this / b;
            return *this;
        }

        Poly operator%(const Poly &o) {
            if (o.coeffs == std::vector<F>{0}) {
                throw std::runtime_error("division by 0");
            }

            if (coeffs.size() < o.coeffs.size()) {
                return *this;
            }

            std::vector<F> remainder = coeffs;
            std::vector<F> quotient(coeffs.size() - o.coeffs.size() + 1, 0);
            F int_quot;

            for (int i = coeffs.size() - o.coeffs.size(); i >= 0; i--) {
                int_quot = remainder[i + o.coeffs.size() - 1] / o.coeffs.back();
                quotient[i] = int_quot;

                for (int j = o.coeffs.size() - 1; j >= 0; j--) {
                    remainder[i + j] = remainder[i + j] - o.coeffs[j] * int_quot;
                }
            }

            return Poly(remainder);
        }
        
        // Method of repeated squares for exponentiation
        Poly operator^(unsigned int n) const {
            Poly power({1});
            Poly square = *this;

            while (n > 0) {
                if (n & 1) {
                    power *= square;
                }

                square *= square;
                n >>= 1;
            }

            return power;
        }
};

// Euclidean algorithm
template <typename F>
Poly<F> gcd(Poly<F> f, Poly<F> g) {
    Poly<F> t({0});
    
    while (g.lead_coeff() != 0) {
        t = g;
        g = f % g;
        f = t;
    }

    f.monic();
    return f;
}

// Extended Euclidean Algorithm: returns as {bezout coeff of f, bezout coeff of g, gcd}
template <typename F>
std::vector<Poly<F>> xgcd(Poly<F> f, Poly<F> g) {
    Poly<F> old_r = f, r = g, temp = g;
    Poly<F> old_s = Poly(std::vector<F>{1}), s = Poly(std::vector<F>{0});
    Poly<F> old_t = Poly(std::vector<F>{0}), t = Poly(std::vector<F>{1});
    Poly<F> quotient({0});

    while(r.lead_coeff() != 0) {
        quotient = old_r / r;
        temp = old_r; old_r = r; r = temp % r;
        temp = old_s; old_s = s; s = temp - quotient * s;
        temp = old_t; old_t = t; t = temp - quotient * t;
    }

    old_s.small_mul(1 / old_r.lead_coeff());
    old_t.small_mul(1 / old_r.lead_coeff());
    old_r.monic();
    return {old_s, old_t, old_r};
}
