/*
Fields.cpp

This document contains several fields for use in the polynomial algorithms. So far,
- Rational numbers
- Complex rational numbers
*/

#include<iostream>

int gcd(int a, int b) {
    int t;

    while (b != 0) {
        t = b;
        b = a % b;
        a = t;
    }

    return a;
}

class Rational {
    private:
        long long num;
        long long den;
    
    public:
        Rational(long long n, long long d) {
            if (d == 0) {
                throw std::runtime_error("division by 0");
            }

            long long g = gcd(n, d); 
            num = n / g;
            den = d / g;
        }

        Rational(long long n) {
            num = n;
            den = 1;
        }

        Rational() {
            num = 0;
            den = 1;
        }

        long long numerator() {
            return num;
        }

        long long denominator() {
            return den;
        }

        bool operator==(const Rational &o) const {
            return o.num == num && o.den == den;
        }

        bool operator!=(const Rational &o) {
            return !(*this == o);
        }

        Rational operator+(const Rational &o) const {
            return Rational(num * o.den + o.num * den, den * o.den);
        }

        Rational operator-(const Rational &o) const {
            return Rational(num * o.den - o.num * den, den * o.den);
        }

        Rational operator*(const Rational &o) const {
            return Rational(num * o.num, den * o.den);
        }

        Rational operator/(const Rational &o) const {
            if (o.num == 0) {
                throw std::runtime_error("division by 0");
            }

            return Rational(num * o.den, den * o.num);
        }

        friend std::ostream &operator<<(std::ostream &out, const Rational &n) {
            out << "(" << n.num << " / " << n.den << ")";
            return out;
        }
};

class ComplexRational {
    private:
        Rational real;
        Rational imag;

    public:
        ComplexRational(Rational r, Rational i) {
            real = r;
            imag = i;
        }
        
        ComplexRational(long long n) {
            real = Rational(n);
            imag = Rational(0);
        }

        Rational imaginary_part() {
            return imag;
        }

        Rational real_part() {
            return real;
        }

        bool operator==(const ComplexRational &o) const {
            return o.real == real && o.imag == imag;
        }

        
        bool operator!=(const ComplexRational &o) {
            return !(*this == o);
        }

        ComplexRational operator+(const ComplexRational &o) const {
            return ComplexRational(real + o.real, imag + o.imag);
        }

        ComplexRational operator-(const ComplexRational &o) const {
            return ComplexRational(real - o.real, imag - o.imag);
        }
        
        ComplexRational operator*(const ComplexRational &o) const {
            return ComplexRational(real * o.real - imag * o.imag, real * o.imag + o.real * imag);
        }

        ComplexRational operator/(const ComplexRational &o) const {
            if ((o.imag == Rational(0)) & (o.real == Rational(0))) {
                throw std::runtime_error("division by 0");
            }

            Rational denom = o.real * o.real + o.imag * o.imag;
            return ComplexRational((real * o.real + imag * o.imag) / denom, (imag * o.real - real * o.imag) / denom);
        }

        friend std::ostream &operator<<(std::ostream &out, const ComplexRational z) {
            out << "(" << z.real << " + " << z.imag << "i)";
            return out;
        }

};
