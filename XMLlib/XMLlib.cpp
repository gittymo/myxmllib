// XMLlib.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "XmlNode.h"
#include <iostream>
#include "InvalidXmlNodeException.h"
#include <conio.h>

int main()
{
	try {
		XmlNode *n = new XmlNode(new string("Books"));
		string test("< Dummy   att1=\"test   three spaces should be left.\"  />");
		string * tidy_string = XmlNode::getTidyString(&test);
		cout << test << endl;
		cout << *tidy_string << endl;
	}
	catch (InvalidXmlNodeException * e) {
		cout << e->what() << endl;
	}
	_getch();
    return 0;
}

