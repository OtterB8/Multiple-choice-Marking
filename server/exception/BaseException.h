#pragma once

#include <iostream>
#include <exception>
#include <string>

using namespace std;

class BaseException : public exception {
	private:
		string cause;
	public:
		BaseException(string const &cause) {
			this->cause = cause;
		}
	public:
	   	const char * what () const throw () {
	      	return this->cause.c_str();
	   	}
};