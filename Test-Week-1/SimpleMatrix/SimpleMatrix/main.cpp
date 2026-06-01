#include <cstdlib>
#include <iostream>
#include <stdexcept>
//#include "wx/string.h"

#include "matrix.h"

using namespace std;

void test_add_1() {
    int a[][3] = { {2,3,4}, {5,6,7} };
    int b[] = { 10, 11, 12, 13, 14, 15 };
    int c[] = { 12, 14, 16, 18, 20, 22 }; // c=a+b
    int d[] = { 0, 0, 0, 0 };

    Matrix<int> test = Matrix<int>(2, 3, (int*)a);
    Matrix<int> test2 = Matrix<int>(2, 3, b);
    Matrix<int> test3 = Matrix<int>(2, 3, c);
    Matrix<int> test4 = Matrix<int>(2, 2, d);

    cout << "add_1 (+ operator): ";
    Matrix<int> res;
    res = test + test2; // THE TEST
    (res == test3) ? cout << "\033[32mpassed\033[0m" << endl : cout << "\033[31mFAILED!!\033[0m" << endl;

    cout << "add_1 (addInPlace): ";
    test.addInPlace(test2); // THE TEST
    (test == test3) ? cout << "\033[32mpassed\033[0m" << endl : cout << "\033[31mFAILED!!\033[0m" << endl;

    cout << "add_1 (generate error): ";
    try {
        res = test + test4;
    endl: cout << "\033[31mFAILED!!\033[0m" << endl;
    }
    catch (std::invalid_argument e) {
        cout << e.what() << endl << flush;
        cout << "\033[32mpassed\033[0m" << endl;
    }
}

void test_multiply_1() {
    int a[] = { 2, 3, 4, 5, 6, 7 };
    int b[] = { 9, 6, 8, 5, 7, 4 };
    int c[] = { 70, 43, 142, 88 };  // c=a*b 
    int d[] = { 4, 6, 8, 10, 12, 14 }; // d=a*2

    Matrix<int> test = Matrix<int>(2, 3, a);
    Matrix<int> test2 = Matrix<int>(3, 2, b);
    Matrix<int> test3 = Matrix<int>(2, 2, c);
    Matrix<int> test4 = Matrix<int>(2, 3, d);

    cout << "multiply_1 (* operator scalar): ";
    Matrix<int> res;
    //res = Matrix<int>::multiply(test, 2);
    res = test * 2; // THE TEST
    (res == test4) ? cout << "\033[32mpassed\033[0m" << endl : cout << "\033[31mFAILED!!\033[0m" << endl;

    cout << "multiply_1 (* operator matrix): ";
    res = test * test2; // THE TEST
    (res == test3) ? cout << "\033[32mpassed\033[0m" << endl : cout << "\033[31mFAILED!!\033[0m" << endl;

    cout << "multiply_1 (multiplyInPlace scalar): ";
    test.multiplyInPlace(2); //THE TEST
    (test == test4) ? cout << "\033[32mpassed\033[0m" << endl : cout << "\033[31mFAILED!!\033[0m" << endl;

    cout << "multiply_1 (generate error): ";
    Matrix<int> r;
    try {
        r = test * test4;
        cout << "\033[31mFAILED!!\033[0m" << endl;
    }
    catch (std::invalid_argument e) {
        cout << e.what() << endl << flush;
        cout << "\033[32mpassed\033[0m" << endl;
    }
}

void test_multiply_2() {
    int a[] = { 2, 3, 4, 5, 6, 7 };
    int b[] = { 9, 6, 8, 5, 7, 4 };
    int c[] = { 18, 18, 32, 25, 42, 28 }; //c = hadamard product of a, b

    Matrix<int> test = Matrix<int>(2, 3, a);
    Matrix<int> test2 = Matrix<int>(2, 3, b);
    Matrix<int> test3 = Matrix<int>(2, 3, c);

    cout << "multiply_2 (hadamard): ";
    Matrix<int> res;
    res = test.hadamard(test2); // THE TEST
    (res == test3) ? cout << "\033[32mpassed\033[0m" << endl : cout << "\033[31mFAILED!!\033[0m" << endl;

    cout << "multiply_2 (hadamardInPlace): ";
    test.hadamardInPlace(test2); // THE TEST
    (test == test3) ? cout << "\033[32mpassed\033[0m" << endl : cout << "\033[31mFAILED!!\033[0m" << endl;
}

void test_transpose_1() {
    int a[] = { 2, 3, 4, 5, 6, 7 };
    int b[] = { 2, 5, 3, 6, 4, 7 };  // b= transpose of a

    Matrix<int> test = Matrix<int>(2, 3, a);
    Matrix<int> test2 = Matrix<int>(3, 2, b);

    cout << "Transpose : ";
    Matrix<int> res;
    res = test.transpose(); // THE TEST
    (res == test2) ? cout << "\033[32mpassed\033[0m" << endl : cout << "\033[31mFAILED!!\033[0m" << endl;

    cout << "Transpose (InPlace): ";
    test.transposeInPlace(); // THE TEST
    (test == test2) ? cout << "\033[32mpassed\033[0m" << endl : cout << "\033[31mFAILED!!\033[0m" << endl;
}

void test_convert_1() {
    int a[] = { 2, 3, 4, 5, 6, 7 };
    Matrix<int> test = Matrix<int>(2, 3, a);

    cout << "Convert to float: " << endl;
    Matrix<float> fmat;
    test.convertTo(fmat);  // THE TEST : copy r (int) to fmat (float)
    //fmat = fmat * 1.1;
    fmat.multiplyInPlace(1.1); // show that fmat is realy holding floats
    fmat.debug();
}

int main() {

    // Create matrix based on array
    int a[][3] = { {2,3,4},
                  {5,6,7} };
    Matrix<int> test = Matrix<int>(2, 3, (int*)a);
    cout << "matrix test : " << endl;
    test.debug();
    cout << endl << flush;

    // Create matrix, and fill element by element
    Matrix<int> test2 = Matrix<int>(4, 2);
    // using indexes
    test2[0][0] = 9; test2[0][1] = 6;
    test2[1][0] = 8; test2[1][1] = 5;
    test2[2][0] = 7; test2[2][1] = 4;
    test2[3][0] = 6; test2[3][1] = 3;
    // or use set
    // test2.set(0,1,6);

    cout << "matrix test2 : " << endl;
    test2.debug();
    cout << endl << flush;

    test_add_1();
    test_multiply_1();
    test_multiply_2();
    test_transpose_1();
    test_convert_1();

    // now test some impossible situations

    // This creates a compiler error. Types of testf and test2 must match!
    // (testf uses floats, test2 int's)
    // float af[] = {2.1,3,4,5,6,7};
    // Matrix<float> testf = Matrix<float>(2,3,af); // OK!
    // Matrix<float> impossible = testf.multiply(test2); // Error!
}
