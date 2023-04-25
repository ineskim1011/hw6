#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
				size_t length = k.size();
				// array to hold (sub)string of 6 "letters"
				unsigned long long values[5];
				// stores first "letter" starting from the back;
				// decrements when iterate to next letter 
				size_t arrayIndex = 4;
				// runs for each element in array 
				for (size_t i = 4; i >= 0; i--) {
					// substring to hold content 
					std::string sub;
					// if not enough letters to fill current index of array,
					// set element to 0 and move on to next index,
					// if index is 0, break
					if (length <= 0) {
						values[arrayIndex] = 0;
						if (arrayIndex == 0) {
							break;
						}
						arrayIndex--;
						continue;
					}
					// creates substring from orig. string to add
					// to current index 
					else if (length <= 6) {
						sub = k.substr(0, length);
					}
					else {
						sub = k.substr(length - 6, 6);
					}
					// decrease used letters from string running length 
					length -= sub.size();
					// holds key to be entered as array value 
					unsigned long long key = 0;
					// calculates power of 36 according to formula 
					int power = 5 - (6 - sub.size());
					// calculates given formula and stores key in 
					// index of array 
					for (size_t i = 0; i < sub.size(); i++) {
						key += pow(36, power) * letterDigitToNumber(sub[i]);
						power--;
					}
					values[arrayIndex] = key;
					arrayIndex--;
				}

				HASH_INDEX_T result = 0;
				for (size_t i = 0; i < 5; i++) {
					result += rValues[i] * values[i];
				}
				return result;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // converts letters to numbers according
				// to ASCII chart 
				if (letter >= 65 && letter <= 90) {
					letter += 32;
				}

				if (letter >= 97 && letter <= 122) {
					return letter - 97;
				}

				if (letter >= 48 && letter <= 57) {
					return letter - 22;
				}
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
