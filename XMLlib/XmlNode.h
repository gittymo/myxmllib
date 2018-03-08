#pragma once
#include <map>
#include <list>
#include <string>
#include "stdafx.h"
#include <iostream>

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

	// The three methods below are used to check suitability of characters in a node's name.
	static bool isValidNodeNameAZ(const char * c_string, int offset);		// Alphabetic characters
	static bool isValidNodeNameNumeral(const char * c_string, int offset);	// Numeric characters
	static bool isValidNodeNameDash(const char * c_string, int offset);		// Minus sign and underscore

	/* Used to parse and extract a node from another node's innerText value and add it as a child.
		Returns a token to be embedded in place of the node string in the innerText, so getInnerText() can
		return the original string value given. */
	int parseInnerTextNode(string * node_string);
public:
	//	Create an XmlNode with the given name.
	XmlNode(string * name);									
	/*	Create an XmlNode with the given name and innerText.  Any nodes in the innerText will be parsed and added to
		as child nodes to this node. */
	XmlNode(string * name, string * innerText);
	//	Sets the name of an existing XmlNode.
	void setName(string * name);
	//	Returns the name of an existing node.
	string * getName();						
	//	Add attribute to node
	void addAttribute(string * name, string * value);
	//	Get map of attributes
	map<string*, string *> * getAttributes();
	//	Add child node to node
	void addChildNode(XmlNode * child_node);
	//	Get list of child nodes
	list<XmlNode *> * getChildNodes();

	//void setInnerText(string * inner_text);				/*	Sets the innerText of a node, will also parse embedded
	//														nodes and add them as child nodes to this node.*/
	//string * getInnerText();								/*	Returns the inner textual value (found between the opening
	//														and closing tags of a node) */
	
	
	// Public static methods
	// Returns a pointer to a new version of the given string which has had excess whitespace removed.
	static string * getTidyString(string * source_string);
	// Returns the length of a c string array.
	static int cstringLength(const char * c_string);
};
