
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <fstream>
#include <string>
#include <assert.h>
#include "HeaderParser.h"

using namespace std;

/*
 *To compile this file using gcc you can type
 *gcc `xml2-config --cflags --libs` -o parser parser.c
 */

const char * version="1.0";
const char * encoding="UTF-8";
const char * dtd="nircompleto.dtd";
const char * tipodoc="Legge";

int main(int argc, char **argv)
{
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL;/* node pointer */
    xmlDtdPtr dtd = NULL;       /* DTD pointer */

    LIBXML_TEST_VERSION;


    if(argc < 2){
    	cerr << "Usage:\n\t" << argv[0] << " <srcfile> [<dstfile> (default STDOUT)]\n" << endl;
	exit(1);
    }
    /* 
     * load document to parse
     */
    ifstream in(argv[1]);
    assert(in.good());
    string buf,text;
    while(getline(in, buf))
      text += buf + "\n";  
   

    /* 
     * Creates a new document, a node and set it as a root node
     */
    doc = xmlNewDoc(BAD_CAST version);
    root_node = xmlNewNode(NULL, BAD_CAST "NIR");
    xmlDocSetRootElement(doc, root_node);

    /*
     * Creates a DTD declaration. Isn't mandatory. 
     */
    dtd = xmlCreateIntSubset(doc, BAD_CAST "NIR", NULL, BAD_CAST dtd);

    /*
     * Creates a skeleton of the document
     */

    xmlNodePtr tipo_node = xmlNewChild(root_node, NULL, BAD_CAST tipodoc, NULL);
    xmlNodePtr meta = xmlNewChild(tipo_node, NULL, BAD_CAST "meta", NULL);
    xmlNodePtr descrittori = xmlNewChild(meta, NULL, BAD_CAST "descrittori", NULL);
    xmlNodePtr intestazione = xmlNewChild(tipo_node, NULL, BAD_CAST "intestazione", NULL);
    xmlNodePtr formulainiziale = xmlNewChild(tipo_node, NULL, BAD_CAST "formulainiziale", BAD_CAST "");
    xmlNodePtr articolato_node = xmlNewChild(tipo_node, NULL, BAD_CAST "articolato", NULL);
    xmlNodePtr formulafinale = xmlNewChild(tipo_node, NULL, BAD_CAST "formulafinale", BAD_CAST "");
    xmlNodePtr conclusione = xmlNewChild(tipo_node, NULL, BAD_CAST "conclusione", BAD_CAST "");
    
    //parseArticolato(meta, articolato, text);  
    // restituisce: int -> prossima nota
    //              header -> testo intestazione
    //              nodo corpo ultimo comma
    unsigned int notes = 1; // prova

    HeaderParser headerparser("default_models");    
    headerparser.setRootNode(tipo_node);
    notes = headerparser.parseHeader(text, meta, descrittori, intestazione, formulainiziale, notes);
    xmlNodePtr lastcomma_node = xmlNewChild(articolato_node, NULL, BAD_CAST "corpo", BAD_CAST text.c_str()); // prova
    headerparser.parseFooter(lastcomma_node, meta, descrittori, formulafinale, conclusione, notes);

    /* 
     * Dumping document to stdio or file
     */
    xmlSaveFormatFileEnc(argc > 2 ? argv[2] : "-", doc, encoding, 1);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
    return(0);
}

