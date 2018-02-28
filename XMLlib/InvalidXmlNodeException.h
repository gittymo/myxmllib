/* InvalidXmlNodeException.h
	Thrown when malformed XML node operation occurs.
	(C)2018 Morgan Evans */

#include "stdafx.h"

using namespace std;

class InvalidXmlNodeException {
	const char * message;
public:
	InvalidXmlNodeException() noexcept {
		this->message = NULL;
	}
	InvalidXmlNodeException(const char * message) noexcept {
		this->message = message;
	}
	InvalidXmlNodeException(const InvalidXmlNodeException& e) noexcept {
		this->message = e.message;
	}
	InvalidXmlNodeException& operator= (const InvalidXmlNodeException& e) noexcept {
		this->message = e.message;
	}
	virtual ~InvalidXmlNodeException() {

	}
	virtual const char* what() const throw() {
		return message;
	}
};