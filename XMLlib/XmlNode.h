#pragma once
#include <map>
#include <list>
#include <string>
#include "stdafx.h"

using namespace std;

class XmlNode {
private:
	string * name;									//	Textual node identifier
	string * value;									//	Inner text value of node
	list<XmlNode *> * child_nodes;					//	List of child nodes
	map<string *, string *> * attributes;			//	Map of attribute=values
	void parseInnerText(string * inner_text);		//	Extracts nodes from inner text.
	map<int, XmlNode *> * innerTextNodes;			//  Map of parse code->node for innerText embedded nodes.
	int next_parse_code;							//	Number to use for next innerText embedded node.

	// String length method - why?!?!!
	int cstringLength(const char * c_string);
	string * getTidyString(string * source_string);

	// The three methods below are used to check suitability of characters in a node's name.
	bool isValidNodeNameAZ(const char * c_string, int offset);			// Alphabetic characters
	bool isValidNodeNameNumeral(const char * c_string, int offset);		// Numeric characters
	bool isValidNodeNameDash(const char * c_string, int offset);		// Minus sign and underscore

	int parseInnerTextNode(string * node_string);	/* Used to parse and extract a node from a string, returns
														parse code to be embedded in innerText. */
public:
	XmlNode(string * name);							//	Create an XmlNode with the given name.
	void setName(string * name);					//	Sets the name of an existing XmlNode.
	string * getName();								//	Returns the name of an existing node.
	void setInnerText(string * inner_text);			/*	Sets the innerText of a node, will also parse embedded
														nodes and add them as child nodes to this node.*/
	string * getInnerText();						/*	Returns the inner textual value (found between the opening
														and closing tags of a node) */
};