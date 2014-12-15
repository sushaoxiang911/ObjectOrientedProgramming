#ifndef STRING_H
#define STRING_H

#include <iostream>
/* 
String class - a subset of the C++ Standard Library <string> class
String objects contain a C-string in a dynamically allocated piece of memory and 
support input/output, comparisons, copy/move construction and assignment, 
concatenation, access to individual characters and substrings, and insertion and removal
of parts of the string. 

Individual characters in the string are indexed the same as an array, 0 through length - 1.
The "size" of the string is the length of the internal C-string, as defined by std::strlen
and does not count the null byte marking the end of the C-string. The "allocation" does 
count the null byte. Thus allocation must be >= size + 1.

Many operations result in a string that occupies the minimum amount of memory
(allocation = size + 1), but for efficiency, the operations that involve adding characters
to the string such as += use a doubling rule for allocation to avoid frequent reallocation
of memory and data copying.

The doubling rule: If n characters are to be added to a string, and the current allocation
is  not large enough to hold the result (allocation < size + n + 1), a new piece of memory
is allocated whose size is 2 * (size + n + 1).

The doubling rule is a way to prevent excessive reallocation and copying work as 
the internal contents of a String are expanded - thus it only applies in cases where 
more characters are being added to the String. In particular:

* The concatenation operators += follow the doubling rule.
* Any operator that should be implemented in terms of +=, such as operator+ and operator>>, 
and the function getline, will then also follow the doubling rule as a result.
* The insert_before function also follows the doubling rule.
* All other functions and operators either leave the allocation unchanged from the source
(e.g. swap, copy/move assignment) or result in the minimum allocation (size +1).

For those operations that involve indexing into the string such as operator[],
a String_exception is thrown with an error message if the index is not within a valid range.

For testing and demonstration purposes, this class contains static members that record the 
current number of Strings in existence and their total memory allocation. 
If the messages_wanted variable is true, the constructors, destructor, and assignment operators
output a message to demonstrate when these functions are called. The message is output
before the function does the actual work.  To help identify the String involved,
the message includes the relevant string data as follows:
* Constructors - the string data used to initialize this String.
* Destructor - the current string data being held in this String.
* Assignment operators - the string data from the right-hand-side (either a C-string or another String).
Note that only these functions output the messages. Other mumber functions may result 
in these messages being output, but only because they call a constructor, destructor, 
or assignment operator as part of their work.
*/

// Simple exception class for reporting String errors
struct String_exception {
	String_exception(const char* msg_) : msg(msg_)
		{}
	const char* msg;
};


class String {
public:
	// Default initialization is to contain an empty string with no allocation.
    // If a non-empty C-string is supplied, this String gets minimum allocation.
	String(const char* cstr_ = "");
	// The copy constructor initializes this String with the original's data,
	// and gets minimum allocation.
	String(const String& original);
    // Move constructor - take original's data, replace it with values like 
    // those of a default constructed object
    String(String&& original) noexcept;
	// deallocate C-string memory
	~String() noexcept;
	
	// Assignment operators
	// Left-hand side gets a copy of rhs data and gets minimum allocation.
	// This operator use the copy-swap idiom for assignment.
	String& operator= (const String& rhs);
 	// This operator creates a temporary String object from the rhs C-string, and swaps the contents
	String& operator= (const char* rhs);
    // Move assignment - simply swaps contents with rhs without any copying
    String& operator= (String&& rhs) noexcept;
	
	// Accesssors
	// Return a pointer to the internal C-string
	const char* c_str() const	
		{return str_ptr;}
	// Return size (length) of internal C-string in this String
	int size() const
		{return length;}
	// Return current allocation for this String
	int get_allocation() const
		{return allocation;}
		
	// Return a reference to character i in the string.
	// Throw exception if 0 <= i < size is false.
	char& operator[] (int i);
	const char& operator[] (int i) const;	// const version for const Strings

	
	/* Return a String starting with i and extending for len characters
	The substring must be contained within the string.
	Values of i and len for valid input are as follows:
	i >= 0 && len >= 0 && i <= size && (i + len) <= size.
	If both i = size and len = 0, the input is valid and the result is an empty string.
	Throw exception if the input is invalid. */
	String substring(int i, int len) const;
	
	// Modifiers
	// Set to an empty string with minimum allocation by create/swap with an empty string.
	void clear();

	/* Remove the len characters starting at i; allocation is unchanged.
	The removed characters must be contained within the String.
	Valid values for i and len are the same as for substring. */
	void remove(int i, int len);
	
	/* Insert the supplied source String before character i of this String,
	pushing the rest of the contents back, reallocating as needed.
	If i == size, the inserted string is added to the end of this String.
	This function does not create any temporary String objects.
	It either directly inserts the new data into this String's space if it is big enough,
	or allocates new space and copies in the old data with the new data inserted.
	This String retains the final allocation.
	Throw exception if 0 <= i <= size is false. */
	void insert_before(int i, const String& src);

	/* These concatenation operators add the rhs string data to the lhs object.
	They do not create any temporary String objects. They either directly copy the rhs data
	into the lhs space if it is big enough to hold the rhs, or allocate new space
	and copy the old lhs data into it followed by the rhs data. The lhs object retains the
	final memory allocation. If the rhs is a null byte or an empty C-string or String,
    no change is made to lhs String. */
	String& operator += (char rhs);
	String& operator += (const char* rhs);
	String& operator += (const String& rhs);

	/* Swap the contents of this String with another one.
	The member variable values are interchanged, along with the
	pointers to the allocated C-strings, but the two C-strings
	are neither copied nor modified. No memory allocation/deallocation is done. */
	void swap(String& other) noexcept;
	
	/* Monitoring functions - not part of a normal implementation */
	/*	used here for demonstration and testing purposes. */
	
	// Return the total number of Strings in existence	
	static int get_number()
		{return number;}
	// Return total bytes allocated for all Strings in existence
	static int get_total_allocation()
		{return total_allocation;}
	// Call with true to cause ctor, assignment, and dtor messages to be output.
	// These messages are output from each function before it does anything else.
	static void set_messages_wanted(bool messages_wanted_)
		{messages_wanted = messages_wanted_;}
	
private:
    char *str_ptr;
    int length;
    int allocation;

    void reallocate(int size);
	
	static char a_null_byte;	// to hold a null byte for for empty string representation

	/* Variables for monitoring functions - not part of a normal implementation. */
	/* But used here for demonstration and testing purposes. */
	static int number;				// counts number of String objects in existence
	static int total_allocation;	// counts total amount of memory allocated
	static bool messages_wanted;	// whether to output constructor/destructor/operator= messages, initially false
    
    String(const char* cstr_, int cstr_length);
};

// non-member overloaded operators

// compare lhs and rhs strings; constructor will convert a C-string literal to a String.
// comparison is based on std::strcmp result compared to 0
bool operator== (const String& lhs, const String& rhs);
bool operator!= (const String& lhs, const String& rhs);
bool operator< (const String& lhs, const String& rhs);
bool operator> (const String& lhs, const String& rhs);

/* Concatenate a String with another String.
 If one of the arguments is a C-string, the String constructor will automatically create
 a temporary String for it to match this function (inefficient, but instructive).
 This automatic behavior would be disabled if the String constructor was declared "explicit".
 This function constructs a copy of the lhs, then concatenates the rhs to it with operator +=, and returns it. */
String operator+ (const String& lhs, const String& rhs);

// Input and output operators and functions
// The output operator writes the contents of the String to the stream
std::ostream& operator<< (std::ostream& os, const String& str);

/* The input operator clears the supplied String, then starts reading the stream.
It skips initial whitespace, then copies characters into
the supplied str until whitespace is encountered again. The terminating
whitespace remains in the input stream, analogous to how string input normally works.
str is expanded as needed, and retains the final allocation.
If the input stream fails, str contains whatever characters were read. */
std::istream& operator>> (std::istream& is, String& str);

/* getline for String clears str to an empty String, then reads characters into str until it finds a '\n',
which is left in the stream (this differs from the fgets and std::getline functions).
str's allocation is expanded as needed, and it retains the final allocation.
If the input stream fails, str contains whatever characters were read. */
std::istream& getline(std::istream& is, String& str);


#endif
