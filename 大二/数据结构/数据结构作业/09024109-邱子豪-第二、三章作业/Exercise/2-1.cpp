//Exercise P93:6
#include<cmath>

class Term {
    friend class Polynomial;
private:
    float coef;
    int exp;
};

class Polynomial {
public:
    float Evaluate(float x0);
private:
    Term* termArray;
    int capacity;
    int terms;
};

float Polynomial::Evaluate(float x0) 
{
    if (terms == 0) return 0.0;

    float result = 0.0;
    float currentPow = 1.0;

    for (int i = 0; i < terms; ++i) {
        int exp_diff = (i == 0) ? termArray[i].exp : (termArray[i-1].exp - termArray[i].exp);
        currentPow *= pow(x0, exp_diff); // 只乘指数差值
        result += termArray[i].coef * currentPow;
    }

    return result;
}