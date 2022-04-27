#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#ifndef LITTLE_XML_H
#define LITTLE_XML_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifndef TRUE
   #define TRUE 1
#endif

#ifndef FALSE
   #define FALSE 0
#endif

//
// Definition
//

struct _XMLNode
{
	char* tag;
	char* inner_text;
	struct _XMLNode* parent; // pointer to parent node
};

typedef struct _XMLNode XMLNode;

XMLNode* XMLNode_new(XMLNode* parent);
void XMLNode_free(XMLNode* node);

struct _XMLDocument
{
	XMLNode* root;
};

typedef struct _XMLDocument XMLDocument;
int XMLDocument_load(XMLDocument* doc, const char* path);
void XMLDocument_free(XMLDocument* doc);


//
// Implementation
//
XMLNode* XMLNode_new(XMLNode* parent)
{
	XMLNode* node = (XMLNode*)malloc(sizeof(XMLNode));
	node->parent = parent;
	node->tag = NULL;
	node->inner_text = NULL;
	return node;
}

void XMLNode_free(XMLNode* node)
{
	if (node->tag)
	{
		free(node->tag);
	}
	if (node->inner_text)
	{
		free(node->inner_text);
	}

	free(node);
}


int XMLDocument_load(XMLDocument* doc, const char* path)
{
	FILE* file = fopen(path, "r");
	// if file doesn't load
	if (!file)
	{
		fprintf(stderr, "could not load file from %s\n", path);
		return FALSE;
	}
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* buff = (char*)malloc(sizeof(char) * size + 1);
	fread(buff, 1, size, file);
	fclose(file);
    buff[size] = '\0';


	doc->root = XMLNode_new(NULL);

	/*Lexical analysis*/
	char lex[256];
	int lexi = 0;
	int i = 0;

	XMLNode* curr_node = NULL;
	while (buff[i] != '\0')
	{
		if (buff[i] == '<')
		{
			lex[lexi] = '\0';

			//inner text
			if (lexi > 0)
			{
				if (!curr_node)
				{
					fprintf(stderr, "Text outside of document\n");
					return FALSE;
				}

				curr_node->inner_text = _strdup(lex);
				lexi = 0;
			}
			// end of node
			if (buff[i + 1] == '/')
			{
				i += 2;
				while (buff[i] != '>')
				{
					lex[lexi++] = buff[i++];
				}
				lex[lexi] = '\0';

				if (!curr_node)
				{
					fprintf(stderr, "Already at the root\n");
					return FALSE;
				}

				if (strcmp(curr_node->tag, lex))
				{
					fprintf(stderr, "Mismatched tags(%s != %s)", curr_node->tag, lex);
					return FALSE;
				}

				curr_node = curr_node->parent;
				i++;
				continue;
			}

			// set current node
			if (!curr_node)
			{
				curr_node = doc->root;
			}
			else
			{
				curr_node = XMLNode_new(curr_node);
			}

			// get tag name
			i++;
			while (buff[i] != '>')
			{
				lex[lexi++] = buff[i++];
			}
			lex[lexi] = '\0';
			curr_node->tag = _strdup(lex);

			//reset lexer
			lexi = 0;
			i++;

			continue;
		}
		else
		{
			lex[i++] = buff[i++];
		}
	}
	return TRUE;
}


void XMLDocument_free(XMLDocument* doc)
{
	XMLNode_free(doc->root);
}

#endif // !LITTLE_XML_H


