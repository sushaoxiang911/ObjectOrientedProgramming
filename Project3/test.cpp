#include <string>
#include <algorithm>
#include <iostream>
#include <cctype>
#include <vector>
#include <set>

using namespace std;

struct comp {
    bool operator()(const int* val1, const int* val2) 
        {return *val1 < *val2;}
};


string read_title()
{
    string buff;
    getline(cin, buff);

cout << "before:  " << buff << endl;
cout << "before length: " << buff.size() << endl;
    auto isspace_binary = [](char _char) {return isspace(_char);};
    replace_if(buff.begin(), buff.end(), isspace_binary, ' ');
cout << "change whitespace: "  << "|" << buff << "|" << endl;
cout << "change length: " << buff.size() << endl;

    auto continue_space = [](char lhs, char rhs) {
        return (lhs == rhs) && (lhs == ' ');
    };
    string::iterator new_end = unique(buff.begin(), buff.end(), continue_space);
    
    buff.erase(new_end, buff.end());

    if (buff[0] == ' ')
        buff.erase(0, 1);

    int last = buff.size() - 1;
    if (buff[last] == ' ')
        buff.erase(last, 1);

    return buff;

}

int main()
{
//    string result = read_title();
//    cout << "string is " << "|" << result << "|" << endl;
/*    vector<int> my_vector;
    my_vector.insert(my_vector.end(), 2);
    for (int i = 0; i < my_vector.size(); ++i)
        cout << my_vector[i] << endl;
    
    auto it = lower_bound(my_vector.begin(), my_vector.end(), 3);
    if (it == my_vector.end() || *it != 3) {
        cout << "Not found" << endl;
        if (it != my_vector.end())
            cout << *it << endl;
        else
            cout << "end of vector" << endl;
    }
*/
    int* a = new int;
    int* b = new int;
    int* c = new int;
    *a = 1;
    *b = 2;
    *c = 3;
    set<int*, comp> my_set;
    my_set.insert(c);
    my_set.insert(b);
    my_set.insert(a);

    for(auto i = my_set.begin(); i != my_set.end(); i++)
        cout << *(*i) << endl;





    return 0;
}
