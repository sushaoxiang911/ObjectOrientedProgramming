#include <iostream>
#include "String.h"

using namespace std;
String read_title()
{
    String result;
    getline(cin, result);
cout << "original: |" << result << "|" << endl;
cout << "original size: " << result.size() << endl;
    int i = 0;
    while (i < result.size()) {
        if (isspace(result[i]))
            ++i;
        else
            break;
    }
cout << "prefix whitespaces remove number: " << i << endl;
    if (i > 0)
        result.remove(0, i);
cout << result.size() << endl;
    i = result.size() - 1;
    while (i >= 0) {
        if (isspace(result[i]))
            --i;
        else
            break;
    }
    if (i < result.size() - 1)
        result.remove(i + 1, result.size() - i - 1);

    i = 0;
    int j = 0;
    while (i < result.size()) {
        while (i < result.size()) {
            if (isspace(result[i]))
                break;
            else
                ++i;
        }
        j = i;
        while (j < result.size()) {
            if (!isspace(result[j]))
                break;
            else
                ++j;
        }
        if (j - i - 1 > 0)
            result.remove(i + 1, j - i - 1);
        ++i;
    }
    return result;
}



int main()
{
    String result = read_title();
    cout << "result size: " << result.size() << endl;
    cout << "result: |" << result << '|'  << endl;
}
