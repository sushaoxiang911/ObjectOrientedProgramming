#include <cstdlib>
#include <iostream>

using namespace std;

int gen()
{
    return rand();
}


int main()
{
    int a = rand();
    int b = rand();
    int c = rand();
    int d = gen();
    cout << a << " " << b << " " << c << " " << d << endl;
}


