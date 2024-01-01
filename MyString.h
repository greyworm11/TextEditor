#ifndef STRING_CLASS_MYSTRING_H_
#define STRING_CLASS_MYSTRING_H_

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

class MyString
{
	int leng;
	int capa;
	char* string;
public:
	static const int NOT_AN_IDENTIFIER = -1;

	MyString();
	MyString(const char* char_element);
	MyString(const std::string string_element);
	MyString(const char* char_element, const int count);
	MyString(const int count, const char char_element);
	MyString(const MyString& mystring_element);
	MyString(int number);
	~MyString();

	MyString operator+ (const MyString& mystring_elem) const;
	MyString operator+ (const char* char_elem) const;//
	MyString operator+ (const std::string string_elem) const;//
	MyString& operator += (const std::string string_element);
	MyString& operator += (const char* char_element);
	MyString& operator += (const MyString& mystring_element);
	MyString& operator = (const char* char_element);
	MyString& operator = (const char char_element);
	MyString& operator = (const std::string string_element);
	MyString& operator = (const MyString& mystring_elem);
	char& operator[](int index);

	char* c_str() const;
	char* data() const;
	int size() const;
	int length() const;
	bool empty() const;
	int capacity() const;
	void shrink_to_fit();
	void clear();

	void insert(int, int, char);
	void insert(int, const char*, int count = -1);
	void insert(int, std::string, int count = -1);

	void erase(int index, int count);

	void append(int, char);
	void append(const char*, int index = 0, int count = -1);
	void append(std::string, int index = 0, int count = -1);

	void replace(int, int, const char*);
	void replace(int, int, std::string);

	char* substr(int, int count = -1);

	bool equals(const char* str);

	long long int find(const char* char_elem, int index = 0) const;
	long long int find(std::string string_elem, int index = 0) const;

	bool operator<(const MyString& mystring_elem)const;
	bool operator>(const MyString& mystring_elem)const;
	bool operator<=(const MyString& mystring_elem)const;
	bool operator>=(const MyString& mystring_elem)const;
	bool operator!=(const MyString& mystring_elem)const;
	bool operator==(const MyString& mystring_elem) const;
	friend std::ostream& operator<< (std::ostream& out, const MyString& mystring_elem);
	friend std::istream& operator>> (std::istream& out, MyString& mystring_elem);

private:
	void strcpy_(char* mass_1, const char* mass_2, int length = -1) const
	{
		length = (length == -1) ? strlen(mass_2) : length;
		if (length)
		{
			for (int i = 0; i < length; i++)
				mass_1[i] = mass_2[i];
			mass_1[length] = '\0';
		}
	}

	bool find_str(const char* mass1, char* mass2) const
	{
		for (size_t i = 0; i < strlen(mass1); i++)
			if (mass1[i] != mass2[i])
				return false;
		return true;
	}
};

#endif