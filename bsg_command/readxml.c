#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>

int file_exists(const char *filename)
{
	FILE *file;
	if ((file = fopen(filename, "r")) != NULL)
	{
		fclose(file);
		return 1;
	}
	return 0;
}


void read_xml(char *msisdnout, char *codeout, char *commandout) 
{
	LIBXML_TEST_VERSION;
	char *filename="config.xml";
       	xmlChar *msisdn;
       	xmlChar *code;
       	xmlChar *command; 

   	xmlDoc *document;
   	xmlNode *root, *first_child, *node;


	if (file_exists(filename) == NULL)
	{
		printf("Error opening file!!!\n");
		exit(1);
	}

  	document = xmlReadFile(filename, NULL, 0);
  	root = xmlDocGetRootElement(document);
	first_child=root->children;

  	for (node = first_child; node != NULL; node = node->next) 
	{
		if (node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *) "msisdn"))
		{
			msisdn = xmlNodeGetContent(node);
		} 
		else if (node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *) "code"))
		{
			code = xmlNodeGetContent(node);
		}
	        else if(node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *) "command"))
		{
			command = xmlNodeGetContent(node);
		}
  	}
	//printf("MSISDN: %s\n", msisdn);
	//printf("CODE: %s\n", code);
	//printf("COMMAND: %s\n", command);
	strncpy(msisdnout, msisdn, 11+1);
	strncpy(codeout, code, 4+1);
	strncpy(commandout, command, 1000+1);
	//msisdnout=(char *) malloc(strlen(msisdn)+1);
	//strncpy(msisdnout, msisdn, strlen(msisdn));	
	xmlFree(msisdn);
	xmlFree(code);
	xmlFree(command);
	//printf ("msisdn: %s\n", msisdn);
}

