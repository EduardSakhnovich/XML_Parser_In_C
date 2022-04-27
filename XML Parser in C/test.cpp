#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "lxml.h"

int main()
{
	XMLDocument doc;
	if (XMLDocument_load(&doc, "D://C Language//XML Parser in C//test.xml"))
	{
		printf("%s: %s\n", doc.root->tag, doc.root->inner_text);
		XMLDocument_free(&doc);
	}

	return 0;
}