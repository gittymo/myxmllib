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
	}
	catch (InvalidXmlNodeException * e) {
		cout << e->what() << endl;
	}
	_getch();
    return 0;
}

