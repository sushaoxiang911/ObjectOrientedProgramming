#include "String.h"
#include <cstring>
#include <new>
#include <cctype>

using namespace std;

void remove_c_str(char* cstr_, int start, int len)
{
    int cstr_length = strlen(cstr_);
    int i = start + len;
    while (i < cstr_length) {
        cstr_[start] = cstr_[i];
        ++start;
        ++i;
    }
    cstr_[start] = '\0';
}

void insert_c_str(char* des, const char* src, int point)
{
    int des_size = strlen(des);
    int src_size = strlen(src);
    for (int i = des_size; i >= point; i--)
        des[i + src_size] = des[i];
    strncpy(des + point, src, src_size);
    des[des_size + src_size] = '\0';
}


int String::number = 0;
int String::total_allocation = 0;
bool String::messages_wanted = false;
char String::a_null_byte = '\0';

String::String(const char* cstr_)
        :length(strlen(cstr_))
{
    if (length == 0) {
        str_ptr = &a_null_byte;
        allocation = 0;
    } else {
        allocation = length + 1;
        str_ptr = new char[allocation];
        strcpy(str_ptr, cstr_);
    }
    ++number;
    total_allocation += allocation;

    if (messages_wanted)
        cout << "Ctor: \"" << cstr_ << "\"" << endl;
}

String::String(const String& original)
        :length(original.length)
{
    if (messages_wanted)
        cout << "Copy ctor: \"" << original.str_ptr << "\"" << endl;    

    if (length) {
        allocation = length + 1;
        str_ptr = new char[allocation];
        strcpy(str_ptr, original.str_ptr);
    } else {
        allocation = 0;
        str_ptr = &a_null_byte;
    }
    
    ++number;
    total_allocation += allocation;

}

String::String(const char* cstr_, int cstr_length)
{
    length = cstr_length;
    if (length == 0) {
        str_ptr = &a_null_byte;
        allocation = 0;
    } else {
        allocation = length + 1;
        str_ptr = new char[allocation];
        strncpy(str_ptr, cstr_, cstr_length);
        str_ptr[length] = '\0';
    }
    ++number;
    total_allocation += allocation;


}
String::String(String&& original) noexcept
        :str_ptr(original.str_ptr), length(original.length), allocation(original.allocation)
{
    if (messages_wanted)
        cout << "Move ctor: \"" << str_ptr << "\"" << endl;
    original.length = 0;
    original.allocation = 0;
    original.str_ptr = &a_null_byte;

    ++number;   
}

String::~String() noexcept
{
    if (messages_wanted)
        cout << "Dtor: \"" << str_ptr << "\"" << endl;

    if (allocation != 0)
        delete[] str_ptr;
    
    --number;
    total_allocation -= allocation;
}

String& String::operator= (const String& rhs)
{
    if (messages_wanted)
        cout << "Copy assign from String:  \"" << rhs.str_ptr << "\"" << endl;
    String temp(rhs);
    swap(temp);
    return *this;
}

String& String::operator= (const char* rhs)
{
    if (messages_wanted)
        cout << "Assign from C-string:  \"" << rhs << "\"" << endl;
    
    String temp(rhs);
    swap(temp);
    return *this; 
}

String& String::operator= (String&& rhs) noexcept 
{
    if (messages_wanted) 
        cout << "Move assign from String:  \"" << rhs.str_ptr << "\"" << endl;
    swap(rhs);
    return *this;
}

char& String::operator[] (int i)
{
    if (i < 0 || i >= length) {
        throw String_exception("Subscript out of range");
    }
    else
        return str_ptr[i];
}

const char& String::operator[] (int i) const
{
    if (i < 0 || i >= length) {
        throw String_exception("Subscript out of range");
    }
    else 
        return str_ptr[i];
}

String String::substring(int i, int len) const
{
    if (i >= 0 && len >= 0 && i <= length && (i + len) <= length) {
        String result(str_ptr + i, len);
        return result;
    } else {
        throw String_exception("Substring bounds invalid");
    }
}

void String::clear()
{
    String temp("");
    swap(temp);
}

void String::remove(int i, int len)
{
    if (i >= 0 && len >= 0 && i <= length && (i + len) <= length) {
        length -= len;
        remove_c_str(str_ptr, i, len);
    } else
        throw String_exception("Remove bounds invalid");
}

void String::reallocate(int mem_size)
{
    if (allocation == 0) {
        str_ptr = new char[mem_size];
        str_ptr[0] = '\0';
    } else {
        char *temp_c_str = new char[mem_size];
        strcpy(temp_c_str, str_ptr);
        delete[] str_ptr;
        str_ptr = temp_c_str;
    }       
    total_allocation -= allocation;
    allocation = mem_size;
    total_allocation += allocation;

}

void String::insert_before(int i, const String& src)
{
    if (i >= 0 && i <= length) {
        if (allocation < length + src.length + 1)
            reallocate(2 * (length + src.length + 1));
        insert_c_str(str_ptr, src.str_ptr, i);

        length += src.length;
    } else 
        throw String_exception("Insertion point out of range");
}

String& String::operator += (char rhs)
{
    if (allocation < length + 2)
        reallocate(2 * (length + 2));
    char temp[2] = { 0 };
    temp[0] = rhs;
    insert_c_str(str_ptr, temp, length);
    length++;
    return *this;
}

String& String::operator += (const char* rhs)
{
    int src_size = strlen(rhs);
    if (allocation < length + src_size + 1) 
        reallocate(2 * (length + src_size + 1));
    insert_c_str(str_ptr, rhs, length);
    length += src_size;
    return *this;
}

String& String::operator += (const String& rhs)
{
    
    insert_before(length, rhs);
    return *this;
}

void String::swap(String& other) noexcept
{
    int temp_length = length;
    length = other.length;
    other.length = temp_length;
    
    int temp_allocation = allocation;
    allocation = other.allocation;
    other.allocation = temp_allocation;

    char* temp_ptr = str_ptr;
    str_ptr = other.str_ptr;
    other.str_ptr = temp_ptr;
}

bool operator== (const String& lhs, const String& rhs)
{
    return (strcmp(lhs.c_str(), rhs.c_str()) == 0) ? 1 : 0;
}

bool operator!= (const String& lhs, const String& rhs)
{
    return (strcmp(lhs.c_str(), rhs.c_str()) != 0) ? 1 : 0;
}

bool operator< (const String& lhs, const String& rhs)
{
    return (strcmp(lhs.c_str(), rhs.c_str()) < 0) ? 1 : 0;
}

bool operator> (const String& lhs, const String& rhs)
{
    return (strcmp(lhs.c_str(), rhs.c_str()) > 0) ? 1 : 0;
}

String operator+ (const String& lhs, const String& rhs)
{
    String result(lhs);
    result += rhs;
    return result;
}


ostream& operator<< (ostream& os, const String& str)
{
    os << str.c_str();
    return os;
}

istream& operator>> (istream& is, String& str)
{
    str.clear();
    while (isspace(is.peek()))
        is.get();
    
    while (is.peek() != EOF && !isspace(is.peek()))
        str += is.get();
    return is;
}

istream& getline(istream& is, String& str)
{
    str.clear();
    while (is.peek() != EOF && is.peek() != '\n')
        str += is.get();
    return is;
}
