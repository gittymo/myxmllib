/*XmlNode.cpp
Base XML Node access and manipulation
(C)2018 Morgan Evans */

#include "stdafx.h"
#include "XmlNode.h" 
#include "InvalidXmlNodeException.h"

XmlNode::XmlNode(string * name) {
	this->setName(name);
	this->value = NULL;
	this->child_nodes = new list<XmlNode *>();
	this->attributes = new map<string *, string *>();
	this->next_parse_code = 1;
	this->innerTextNodes = new map<int, XmlNode *>();
}

void XmlNode::setName(string * new_name) {
	if (this->name != NULL) delete this->name;
	if (new_name == NULL || new_name->empty()) {
		throw new InvalidXmlNodeException("Node cannot have NULL or empty name");
	}
	else this->name = new string(*new_name);
}

string * XmlNode::getName() {
	return new string(*this->name);
}

void XmlNode::parseInnerText(string * innerText) {
	// Then we parse the string to add the nodes to the DOM.
	int ob, cb;
	do {
		cb = string::npos;
		ob = innerText->find('<');
		if (ob != string::npos) {
			cb = innerText->find('>', ob);
			if (cb != string::npos) {
				int place_holder = this->parseInnerTextNode(&innerText->substr(ob, cb));
			}
			string * old_innerText = innerText;
			innerText =
				new string(innerText->substr(0, ob - 1) + innerText->substr(cb + 1));
			delete old_innerText;
		}
	} while (ob != string::npos && cb != string::npos);
	this->value = new string(*innerText);
}

string * XmlNode::getTidyString(string * source_string) {
	const int STR_LEN = source_string->length();
	const char * untidy_string = source_string->data();
	char quoted = 0;
	bool escaped = false;
	char * tidy_string = new char[STR_LEN];
	int wp = 0, spaces = 0;
	for (int i = 0; i < STR_LEN; i++) {
		if (untidy_string[i] != ' ' && untidy_string[i] != '\t') {
			if (spaces > 0) {
				// Deal with spaces first
				if (quoted > 0) for (int j = 0; j < spaces; j++) tidy_string[wp++] = ' ';
				else {
					char left_of_spaces = untidy_string[i - (spaces + 1)];
					if ((left_of_spaces >= 'A' && left_of_spaces <= 'Z') ||
						(left_of_spaces >= 'a' && left_of_spaces <= 'z'))
						tidy_string[wp++] = ' ';
				}
				spaces = 0;
			}

			// Now escape characters
			if (untidy_string[i] == '\\') escaped = !escaped;
			else escaped = false;
			
			// Now text within quotes
			if (untidy_string[i] == '\'' || untidy_string[i] == '\"') {
				if (!escaped) {
					if (quoted == 0) quoted = untidy_string[i];
					else quoted = 0;
				}
			}
			
			// Write character to tidy string.
			tidy_string[wp++] = untidy_string[i];
		} else spaces++;
	}

	tidy_string[wp] = 0;
	return new string(tidy_string, 0, wp);
}

int XmlNode::parseInnerTextNode(string * node_string) {
	return 0;
}

inline int XmlNode::cstringLength(const char * c_string) {
	int len = 0;
	if (c_string != NULL) {
		for (; c_string[len] != 0; len++);
	}
	return len;
}

bool XmlNode::isValidNodeNameAZ(const char* c_string, int offset) {
	// Get c_string length
	const int STR_LEN = XmlNode::cstringLength(c_string);
	// If the character to check is outside the c_string's range, exit with false.
	if (offset < 0 || offset > STR_LEN - 1) return false;
	// If the ASCII value of the character does not fall within the upper case range A-Z, return false.
	if ((c_string[offset] < 'a') && (c_string[offset] < 'A' || c_string[offset] > 'Z')) return false;
	// If the ASCII value of the character does not fall within the lower case range a-z, return false.
	if ((c_string[offset] > 'Z') && (c_string[offset] < 'a' || c_string[offset] > 'z')) return false;
	// Otherwise, we're all good!
	return true;
}

bool XmlNode::isValidNodeNameNumeral(const char * c_string, int offset) {
	// Get c_string length
	const int STR_LEN = XmlNode::cstringLength(c_string);
	// If the character to check is outside the c_string's range, exit with false.
	if (offset < 0 || offset > STR_LEN - 1) return false;
	// If the character isn't a numeral, return false.
	if (c_string[offset] < '0' || c_string[offset] > '9') return false;
	// The character to the left of a numeral needs to be either a valid dash or be alphabetic.
	// If the digit is the first character of the c_string, it's not valid.
	if (offset - 1 < 0) return false;
	if (!XmlNode::isValidNodeNameDash(c_string, offset - 1) ||
		!XmlNode::isValidNodeNameAZ(c_string, offset - 1)) return false;
	// Otherwise, we're good!
	return true;

}
bool XmlNode::isValidNodeNameDash(const char * c_string, int offset) {
	// Get c_string length
	const int STR_LEN = XmlNode::cstringLength(c_string);
	// If the character to check is outside the c_string's range, exit with false.
	if (offset < 0 || offset > STR_LEN - 1) return false;
	// If the character is neither a minus sign (-) or underscore (_) return false. 
	if (c_string[offset] != '_' || c_string[offset] != '-') return false;
	// If the character is an underscore (_) return true.
	if (c_string[offset] == '_') return true;

	// We're only left with the checks for minus sign (-)
	// Return false if the character to the left isn't a alphanumeric.
	if (offset - 1 < 0 || !XmlNode::isValidNodeNameNumeral(c_string, offset - 1) ||
		!XmlNode::isValidNodeNameAZ(c_string, offset - 1)) return false;
	// Same for the character to the right
	if (offset + 1 == STR_LEN || !XmlNode::isValidNodeNameNumeral(c_string, offset + 1) ||
		!XmlNode::isValidNodeNameAZ(c_string, offset + 1)) return false;
	// If we've passed those two tests, everything's good!
	return true;
}