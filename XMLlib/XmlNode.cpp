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

XmlNode::XmlNode(string * name, string * innerText) {
	this->setName(name);
	this->parseInnerText(innerText);
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
				int place_holder =
					this->parseInnerTextNode(XmlNode::getTidyString(&innerText->substr(ob, cb)));
			}
			if (cb < innerText->length() - 1) {
				string * old_innerText = innerText;
				innerText = new string(innerText->substr(0, ob - 1) + innerText->substr(cb + 1));
				cout << ob << ", " << cb << " = " << *innerText << endl;
				delete old_innerText;
			}
			else ob = string::npos;
		}
	} while (ob != string::npos && cb != string::npos);
	this->value = new string(*innerText);
}

int XmlNode::parseInnerTextNode(string * node_string) {
	// Get the c string data for the string.
	const char * c_string = node_string->data();
	/* Get name of node by taking the substring of the characters between < and the
		first whitespace character. */
	int s;
	for (s = 0; c_string[s] != ' '; s++);
	XmlNode * new_node = new XmlNode(new string(c_string + 1, s));
	// Everything else will be attributes up to the closing >
	// Get the length of the node string
	const int STR_LEN = node_string->length();
	for (s; s < STR_LEN; s++) {
		// Find everything up to = (if it exists!)
		int e,v;
		for (e = s; c_string[e] != '=' && e < STR_LEN; e++);
		if (c_string[e] == '=') {
			// We've got an attribute name, let's get it's value.
			v = e + 1; // Ignore opening quote.
			while (!((c_string[v] == '\"' || c_string[v] == '\'') && c_string[v - 1] != '\\') && v < STR_LEN) {
				v++;
			}
			// Okay, we've got offsets to find the name and value, so let's add them to
			//	the node.
			new_node->addAttribute(new string(c_string + s, e - s - 1),
				new string(c_string + e + 1, v - e - 1));
			e = v + 1;
		}
		s = e;
	} 

	return 0;
}

void XmlNode::addAttribute(string * name, string * value) {
	this->attributes->insert(std::pair<string*, string*>(name, value));
}

map<string*, string *> * XmlNode::getAttributes() {
	return this->attributes;
}

void XmlNode::addChildNode(XmlNode * child_node) {
	if (child_node != NULL) {
		list<XmlNode *>::iterator it = child_nodes.begin();
		this->child_nodes->insert(it, child_node);
	}
}

list <XmlNode *> * XmlNode::getChildNodes() {
	return this->child_nodes;
}
// ----------------------------------------------- PUBLIC STATIC METHOD DEFINITIONS ---------------------------------------------------- //

inline int XmlNode::cstringLength(const char * c_string) {
	int len = 0;
	if (c_string != NULL) {
		for (; c_string[len] != 0; len++);
	}
	return len;
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
		}
		else spaces++;
	}

	tidy_string[wp] = 0;
	return new string(tidy_string, wp);
}

// ----------------------------------------------- PRIVATE STATIC METHOD DEFINITIONS --------------------------------------------------- //

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
