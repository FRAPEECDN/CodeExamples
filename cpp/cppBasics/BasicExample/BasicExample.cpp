// BasicExample.cpp : This is a basic code for testing the most basic usage in c/c++
// Currently set to use c++ 14 standard
#include "pch.h"
#include <iostream>
#include <string>
#include <limits> 
#include <math.h>
#include <vector>

struct ExampleStruct {
    int a{};
    std::string b{};
    ExampleStruct() {}
};

void testIfStatement(int a, int b) {
    if (a > b) {
        std::cout << "a is bigger than b";
    } else {
        std::cout << "b is bigger than a";
    }
    std::cout << std::endl;
}

void testLoopWhile(int a, int b) {
    int count = 0;
    while (a < b) {
        std::cout << "looping " << count << std::endl;
        std::cout << "a is " << a << std::endl;
        count++;
        a++;
    }
    std::cout << "while loop ended" << std::endl;
}

void testLoopRepeat(int a, int b) {
    int count = 0;
    do {
        std::cout << "looping " << count << std::endl;
        std::cout << "a is " << a << std::endl;
        count++;
        a++;
    } while (a < b);
    std::cout << "repeat loop ended" << std::endl;
}

void testForloop(int a, int b) {
    for (int idx = a; idx < b; idx++) {
        std::cout << "looping " << idx << std::endl;
    }
    std::cout << "for loop ended" << std::endl;
}

int main()
{
    bool b = false;
    
    unsigned char uc = 10;
    char c = 10;
    unsigned short us = (unsigned short)std::numeric_limits<unsigned char>::max + 1;
    short s = (short)std::numeric_limits<char>::max + 1;
    unsigned int ui = (unsigned int)std::numeric_limits<unsigned short>::max + 1;
    int i = (int)std::numeric_limits<short>::max + 1;
    unsigned long ul = (unsigned long)std::numeric_limits<unsigned int>::max + 1;
    long l = (long)std::numeric_limits<int>::max + 1;

    float f = 343.12f;
    double d = 35438580.21473;

    char character = 'a';
    std::string bString = "basic string value";

    // Basic data tests (supported in c/c++)
    std::cout << "Basic data types" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Integers" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Limit functions example (int type)" << std::endl;
    std::cout << "Minimum value for int: " << std::numeric_limits<int>::min() << std::endl;
    std::cout << "Maximum value for int: " << std::numeric_limits<int>::max() << std::endl;
    std::cout << "Is int signed: " << std::numeric_limits<int>::is_signed << std::endl;
    std::cout << "unsigned byte (1): " << static_cast<unsigned>(uc) << std::endl;
    std::cout << "signed byte (1): " << static_cast<signed>(c) << std::endl;
    std::cout << "unsigned short (2): " << us << std::endl;
    std::cout << "signed short (2): " << s << std::endl;
    std::cout << "unsigned int types (4): " << ui << std::endl;
    std::cout << "signed int types (4): " << i << std::endl;
    std::cout << "unsigned long (8): " << ul << std::endl;
    std::cout << "signed long (8): " << l << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Floating points" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "float: " << f << std::endl;
    std::cout << "double: " << d << std::endl;
    std::cout << "Minimum value for double: " << std::numeric_limits<double>::min() << std::endl;
    std::cout << "Maximum value for bouble: " << std::numeric_limits<double>::max() << std::endl;
    std::cout << "Is double signed: " << std::numeric_limits<double>::is_signed << std::endl;
    std::cout << "Is double infinite support: " << std::numeric_limits<double>::has_infinity << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "String and characters" << std::endl;
    std::cout << "Basic character: " << character << std::endl;
    std::cout << "Basic string: " << bString << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Boolean" << std::endl;
    std::cout << "boolean example: " << b << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~" << std::endl;

    // If testing 
    std::cout << "If statement test" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~" << std::endl;

    int noA = 78;
    int noB = 77;
    testIfStatement(noA, noB);
    noB = 79;
    testIfStatement(noA, noB);
    noA = 2;
    noB = 10;

    // Loop testing
    std::cout << "while loop test" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~" << std::endl;
    testLoopWhile(noA, noB);
    std::cout << "do while test" << std::endl;
    std::cout << "~~~~~~~~~~~~~" << std::endl;
    testLoopRepeat(noA, noB);
    std::cout << "for test" << std::endl;
    std::cout << "~~~~~~~~" << std::endl;
    testForloop(noA, noB);

    // Input test (looking directly at stream) - Requires integer input
    std::string theInput;
    int inputAsInt;

    std::cout << "Please provide a number" << std::endl;
    std::getline(std::cin, theInput);

    while (std::cin.fail() || std::cin.eof() || theInput.find_first_not_of("0123456789") != std::string::npos) {

        std::cout << "Error" << std::endl;

        if (theInput.find_first_not_of("0123456789") == std::string::npos) {
            std::cin.clear();
            std::cin.ignore(256, '\n');
        }

        std::getline(std::cin, theInput);
    }

    std::string::size_type st;
    inputAsInt = std::stoi(theInput, &st);
    std::cout << inputAsInt << std::endl;

    // Input test requires small alphabetical characters
    std::cout << "Please provide a string (in lower case)" << std::endl;
    std::getline(std::cin, theInput);
    while (std::cin.fail() || std::cin.eof() || theInput.find_first_not_of("abcdefghijiklmnopqrstuvwxyz") != std::string::npos) {

        std::cout << "Error" << std::endl;

        if (theInput.find_first_not_of("abcdefghijiklmnopqrstuvwxyz") == std::string::npos) {
            std::cin.clear();
            std::cin.ignore(256, '\n');
        }

        std::getline(std::cin, theInput);
    }
    std::cout << inputAsInt << std::endl;

    // Basic collection workings
    std::cout << "collection test" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~" << std::endl;
    std::vector<int> intCollection;
    std::cout << "Collection contains: " << intCollection.size() << " elements" << std::endl;
    intCollection.push_back(9);
    intCollection.push_back(8);
    intCollection.push_back(7);
    intCollection.push_back(6);
    std::cout << "Collection contains: " << intCollection.size() << " elements" << std::endl;
    int extract = intCollection.back();
    intCollection.pop_back();
    std::cout << "Collection contains: " << intCollection.size() << " elements" << std::endl;
    std::cout << "removed number " << extract << std::endl;   
    std::cout << "number at position 1 is " << intCollection[1] << std::endl;

    std::vector<int>::iterator itrIntCollection;
    for (itrIntCollection = intCollection.begin(); itrIntCollection < intCollection.end(); itrIntCollection++) {
        std::cout << "Collection contains : " << *itrIntCollection << std::endl;
    }
    std::cout << "for each example" << std::endl;
    for (int no : intCollection)
    {
        std::cout << "Collection contains : " << no << std::endl;

    }


    // Smart pointers
    std::cout << "smart pointer test" << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~" << std::endl;

    std::unique_ptr<std::string> ps = std::make_unique<std::string>();
    *ps = "Hello";
    std::string helloStr = "Hello";
    if (helloStr.compare(*ps) == 0) {
        std::cout << "String comparison is done" << std::endl;
    }
    std::unique_ptr<ExampleStruct> p = std::make_unique<ExampleStruct>();
    std::cout << "Is p a null :" << (p == nullptr) << std::endl;
    p->a = 30;
    p->b = "Hello there";
    std::cout << "p's a value : " << p->a << std::endl;
    std::cout << "p's b value : " << p->b << std::endl;

    std::unique_ptr<ExampleStruct> q = std::move(p);
    std::cout << "Is p a null :" << (p == nullptr) << std::endl;    
    std::cout << "Is q a null :" << (q == nullptr) << std::endl;

    std::cout << "Done\n";
}
