/******************************************************************************
* Project:	xmLeges
* Module:	HeaderParser
* File:		HeaderParser.cpp
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Andrea Passerini
* 			Lorenzo Bacci (lorenzobacci@gmail.com)
******************************************************************************/
#include "HeaderParser.h"
#include <fstream>
#include <assert.h>
#include "Default.h"

using namespace std;

Month MonthMapping;

HeaderParser::HeaderParser(std::string modeldir,
			   std::string header_intestazione_model_file,
			   std::string header_pubblicazione_model_file,
			   std::string header_formulainiziale_model_file,
			   std::string header_cnr_model_file,
			   std::string header_ddl_model_file,
			   std::string footer_formulafinale_model_file,
			   std::string footer_dataeluogo_model_file,
			   std::string footer_sottoscrizioni_model_file,
			   std::string footer_annessi_model_file,
			   std::string header_extractor_model_file,
			   std::string header_extractor_config_file,
			   std::string footer_extractor_model_file,
			   std::string footer_extractor_config_file,
			   std::string parser_config_file)
{
  ifstream in((modeldir + "/" + header_intestazione_model_file).c_str());
  if(in.good()){
    in >> header_intestazione_model;
    in.close();
  }
  else{
    istringstream in_s(header_intestazione_model_default);
    in_s >> header_intestazione_model;
  }

  ifstream in8((modeldir + "/" + header_pubblicazione_model_file).c_str());
  if(in8.good()){
    in8 >> header_pubblicazione_model;
    in8.close();
  }
  else{
    istringstream in_s(header_pubblicazione_model_default);
    in_s >> header_pubblicazione_model;
  }

  ifstream in9((modeldir + "/" + header_formulainiziale_model_file).c_str());
  if(in9.good()){
    in9 >> header_formulainiziale_model;
    in9.close();
  }
  else{
    istringstream in_s(header_formulainiziale_model_default);
    in_s >> header_formulainiziale_model;
  }

  ifstream in12((modeldir + "/" + header_cnr_model_file).c_str());
  if(in12.good()){
    in12 >> header_cnr_model;
    in12.close();
  }
  else{
    istringstream in_s(header_cnr_model_default);
    in_s >> header_cnr_model;
  }

  ifstream in13((modeldir + "/" + header_ddl_model_file).c_str());
  if(in13.good()){
    in13 >> header_ddl_model;
    in13.close();
  }
  else{
    istringstream in_s(header_ddl_model_default);
    in_s >> header_ddl_model;
  }

  ifstream in2((modeldir + "/" + footer_formulafinale_model_file).c_str());
  if(in2.good()){
    in2 >> footer_formulafinale_model;
    in2.close();
  }
  else{
    istringstream in_s(footer_formulafinale_model_default);
    in_s >> footer_formulafinale_model;
  }

  ifstream in6((modeldir + "/" + footer_dataeluogo_model_file).c_str());
  if(in6.good()){
    in6 >> footer_dataeluogo_model;
    in6.close();
  }
  else{
    istringstream in_s(footer_dataeluogo_model_default);
    in_s >> footer_dataeluogo_model;
  }

  ifstream in7((modeldir + "/" + footer_sottoscrizioni_model_file).c_str());
  if(in7.good()){
    in7 >> footer_sottoscrizioni_model;
    in7.close();
  }
  else{
    istringstream in_s(footer_sottoscrizioni_model_default);
    in_s >> footer_sottoscrizioni_model;
  }

  ifstream in10((modeldir + "/" + footer_annessi_model_file).c_str());
  if(in10.good()){
    in10 >> footer_annessi_model;
    in10.close();
  }
  else{
    istringstream in_s(footer_annessi_model_default);
    in_s >> footer_annessi_model;
  }

  ifstream in3((modeldir + "/" + header_extractor_model_file).c_str());
  ifstream in3_config((modeldir + "/" + header_extractor_config_file).c_str());
  if(in3.good() && in3_config.good()){
    header_extractor(in3_config, in3);
    in3.close();
    in3_config.close();
  }
  else{
    istringstream in_s(header_extractor_model_default);
    istringstream in_sc(header_extractor_config_default);
    header_extractor(in_sc, in_s);
  }
  
  ifstream in4((modeldir + "/" + footer_extractor_model_file).c_str());
  ifstream in4_config((modeldir + "/" + footer_extractor_config_file).c_str());
  if(in4.good() && in4_config.good()){
    footer_extractor(in4_config, in4);
    in4.close();
    in4_config.close();
  }
  else{
    istringstream in_s(footer_extractor_model_default);
    istringstream in_sc(footer_extractor_config_default);
    footer_extractor(in_sc, in_s);
  }

  ifstream in5_config((modeldir + "/" + parser_config_file).c_str());
  if(in5_config.good()){
    init(in5_config);
    in5_config.close();
  }
  else{
    istringstream in_sc(parser_config_default);
    init(in_sc);
  }
}

void HeaderParser::init(istream& in)
{
  int state,tag,open;
  string buf;
  if(!Lexer::getLine(in,buf) || buf != "HEADER"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  if(!Lexer::getLine(in,buf) || buf != "INTESTAZIONE"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(Lexer::getLine(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    header_intestazione_tags[state] = make_pair(tag,open);
  }
  if(!Lexer::getLine(in,buf) || buf != "PUBBLICAZIONE"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(Lexer::getLine(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    header_pubblicazione_tags[state] = make_pair(tag,open);
  }
  if(!Lexer::getLine(in,buf) || buf != "FORMULAINIZIALE"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(Lexer::getLine(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    header_formulainiziale_tags[state] = make_pair(tag,open);
  }
  if(!Lexer::getLine(in,buf) || buf != "CNR"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(Lexer::getLine(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    header_cnr_tags[state] = make_pair(tag,open);
  }
  if(!Lexer::getLine(in,buf) || buf != "DDL"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(Lexer::getLine(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    header_ddl_tags[state] = make_pair(tag,open);
  }
  if(!Lexer::getLine(in,buf) || buf != "FOOTER"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  if(!Lexer::getLine(in,buf) || buf != "FORMULAFINALE"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(Lexer::getLine(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    footer_formulafinale_tags[state] = make_pair(tag,open);
  }
  if(!Lexer::getLine(in,buf) || buf != "DATAELUOGO"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(Lexer::getLine(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    footer_dataeluogo_tags[state] = make_pair(tag,open);
  }
  if(!Lexer::getLine(in,buf) || buf != "SOTTOSCRIZIONI"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(Lexer::getLine(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    footer_sottoscrizioni_tags[state] = make_pair(tag,open);
  }
  if(!Lexer::getLine(in,buf) || buf != "ANNESSI"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(Lexer::getLine(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    footer_annessi_tags[state] = make_pair(tag,open);
  }

  root_node = NULL;
}

//Aggiunto l'argomento tdoc che indica il tipo di documento:
//0: generico
//1: disegno di legge
//2: provvedimento CNR
int HeaderParser::parseHeader(std::string& header, 
			      xmlNodePtr meta,
			      xmlNodePtr descrittori,
			      xmlNodePtr intestazione,
			      xmlNodePtr formulainiziale,
			      int tdoc,
			      int notes)
{
  bool found = false, pub_found = false;
  unsigned int last = 0, first = 0, offset = 0;
  int * states = NULL, * pub_states = NULL;

	//printf("\nHeaderParser\ntdoc: %d\n", tdoc);
	//printf("\nHeaderParser\nbuffer: %s\n\n\n", header.c_str());
	
	//Replace "1°" (primo) with "1" (> and < ?)
	delPrimo(header);
	//printf("\nHeaderParser\nbuffer: %s\n\n\n", header.c_str());
  
  // recover URN if present
  string urn = extractURN(header);

  string strbuffer = header;
  SqueezeWords(strbuffer);
  const char * buffer = strbuffer.c_str();
  
  // extract terms
  vector<int> sequence, offsets;
  if(!header_extractor.buildExample(sequence, offsets, buffer, strlen(buffer), 0)){
    defaultHeader(descrittori, intestazione);
    return notes;
  }

//Disegno di legge
if(tdoc == 1) {
	if(sequence.size() > 0) {
		found = true;
		//Aggiungi tag specifici  <-- si dovrebbe controllare che non siano già presenti?!...
		xmlNodePtr nApprovazione = xmlNewChild(descrittori, NULL, BAD_CAST "approvazione", NULL);
		xmlNodePtr nRedazione = xmlNewChild(descrittori, NULL, BAD_CAST "redazione", NULL);
		xmlNodePtr nUrn = xmlNewChild(descrittori, NULL, BAD_CAST "urn", NULL);
		//xmlNewProp(nUrn, BAD_CAST "value", BAD_CAST "");
		xmlAddChild(nUrn, xmlNewText(BAD_CAST "")); //La dtd dei ddl non ha ancora value come attributo di urn
		xmlNewProp(nApprovazione, BAD_CAST "internal_id", BAD_CAST "");
		xmlNewProp(nApprovazione, BAD_CAST "leg", BAD_CAST "");
		xmlNewProp(nApprovazione, BAD_CAST "norm", BAD_CAST "");	
		xmlNewProp(nApprovazione, BAD_CAST "tipodoc", BAD_CAST "");
		xmlNewProp(nRedazione, BAD_CAST "nome", BAD_CAST "");
		xmlNewProp(nRedazione, BAD_CAST "norm", BAD_CAST "");
		//xmlNewProp(redazione, BAD_CAST "url", BAD_CAST ""); //non obbligatorio nella dtd
		
		states = new int[sequence.size()];
		header_ddl_model.viterbiPath(sequence, states, sequence.size());
		if ((first = getFirstMatchingState(states, sequence.size(), header_ddl_tags)) < sequence.size()){
	    	found = true;
			//for(int kk=0; kk < sequence.size(); kk++)
				//printf("\n %d: sequence[]=%d   states[]=%d", kk, sequence[kk], states[kk]);
			
	        last = saveTags(strbuffer, states, sequence.size(), offsets, offset, last, intestazione, header_ddl_tags, tdoc);      
	        removeProcessedElements(sequence, last);
	        offset += last + 1;
	        addDivDecorations(intestazione->parent);
		}
		delete[] states;
	} 
	if(sequence.size() > 0 || found == false) { 
		//Se non è stato rilevata l'intestazione viene messo tutto in error:
		//printf("\noffset:%d size:%d\n",offset,offsets.size());
		// se rimane qualcosa mettilo in un tag errore
    	if (offset < offsets.size())
      		saveTag(hp_sconosciuto, offsets[offset], strbuffer.length(), strbuffer, intestazione, tdoc); 
      	//Devono essere aggiunti dei tag di default vuoti per i disegni di legge
      	addMissingHeader(meta, descrittori, intestazione, formulainiziale, tdoc);
	}
	return notes; 	
}

//Provvedimenti CNR
if(tdoc == 2) {
	int cnrfirst = 0;
	int * cnr_states = 0;
	bool cnrfound = false;
	xmlNodePtr errorNode = intestazione; //per inserire il testo non rilevato nella giusta posizione

	//Aggiungi tag 'proprietario' vuoti
	xmlNodePtr proprietario = xmlNewChild(meta, NULL, BAD_CAST "proprietario", BAD_CAST "");
	xmlNewProp(proprietario, BAD_CAST "xlink:type", BAD_CAST "simple");
	//xmlNewProp(proprietario, BAD_CAST "xmlns:cnr", BAD_CAST "http://www.cnr.it/provvedimenti/1.0");
	//Nuova versione cnr-DTD:
	xmlNewProp(proprietario, BAD_CAST "xmlns:cnr", BAD_CAST "http://www.cnr.it/provvedimenti/2.1");
	
	xmlNodePtr cnrmeta = xmlNewChild(proprietario, NULL, BAD_CAST "cnr:meta", BAD_CAST "");
	xmlNodePtr thisCnrMeta = xmlNewChild(cnrmeta, NULL, BAD_CAST "cnr:strutturaEmanante", BAD_CAST "");
	xmlNewProp(thisCnrMeta, BAD_CAST "value", BAD_CAST "");
	thisCnrMeta = xmlNewChild(cnrmeta, NULL, BAD_CAST "cnr:autoritaEmanante", BAD_CAST "");
	xmlNewProp(thisCnrMeta, BAD_CAST "value", BAD_CAST "");
	thisCnrMeta = xmlNewChild(cnrmeta, NULL, BAD_CAST "cnr:tipoDestinatario", BAD_CAST "");
	xmlNewProp(thisCnrMeta, BAD_CAST "value", BAD_CAST "");

	//Nuovi:
	thisCnrMeta = xmlNewChild(cnrmeta, NULL, BAD_CAST "cnr:strutturaDestinataria", BAD_CAST "");
	xmlNewProp(thisCnrMeta, BAD_CAST "value", BAD_CAST "");
	thisCnrMeta = xmlNewChild(cnrmeta, NULL, BAD_CAST "cnr:tipoProvvedimento", BAD_CAST "");
	xmlNewProp(thisCnrMeta, BAD_CAST "value", BAD_CAST "");

	thisCnrMeta = xmlNewChild(cnrmeta, NULL, BAD_CAST "cnr:disciplina", BAD_CAST "");
	xmlNewProp(thisCnrMeta, BAD_CAST "value", BAD_CAST "");
	//disciplina e areaScientifica sono in OR nella DTD.
	
	//Aggiungi tag specifici
	xmlNodePtr nPubblicazione = xmlNewChild(descrittori, NULL, BAD_CAST "pubblicazione", NULL);
	xmlNewProp(nPubblicazione, BAD_CAST "norm", BAD_CAST "");
	xmlNewProp(nPubblicazione, BAD_CAST "num", BAD_CAST "");
	xmlNewProp(nPubblicazione, BAD_CAST "tipo", BAD_CAST "BUCNR");
			
	xmlNodePtr nRedazione = xmlNewChild(descrittori, NULL, BAD_CAST "redazione", NULL);
	xmlNewProp(nRedazione, BAD_CAST "id", BAD_CAST "red");
	xmlNewProp(nRedazione, BAD_CAST "nome", BAD_CAST "Urp-Cnr");
	xmlNewProp(nRedazione, BAD_CAST "norm", BAD_CAST "");
	
	xmlNodePtr nUrn = xmlNewChild(descrittori, NULL, BAD_CAST "urn", NULL);
	//dtd2.1 Il valore non è più figlio, ma va nell'attributo value:
	//xmlAddChild(nUrn, xmlNewText(BAD_CAST "urn:nir:consiglio.nazionale.ricerche:provvedimento:"));
	xmlNewProp(nUrn, BAD_CAST "value", BAD_CAST "urn:nir:consiglio.nazionale.ricerche:provvedimento:");

	xmlNodePtr emanode = xmlNewChild(intestazione, NULL, BAD_CAST "emanante", BAD_CAST "");
	xmlAddChild(emanode, xmlNewText(BAD_CAST "Consiglio Nazionale delle Ricerche"));

	states = new int[sequence.size()];
	
	header_formulainiziale_model.viterbiPath(sequence, states, sequence.size());
	if ((first = getFirstMatchingState(states, sequence.size(), header_formulainiziale_tags)) < sequence.size()){
		found = true;
		// recover eventual cnr-header before formulainiziale
		if(first > 0){
			cnr_states = new int[first];
			vector<int> cnr_sequence;
			copyElements(sequence, cnr_sequence, 0, first-1);
			header_cnr_model.viterbiPath(cnr_sequence, cnr_states, cnr_sequence.size());
				//for(int kk=0; kk < cnr_sequence.size(); kk++) //
					//printf("\n %d: cnr_sequence[]=%d   cnr_states[]=%d", kk, cnr_sequence[kk], cnr_states[kk]);
			
			if ((cnrfirst = getFirstMatchingState(cnr_states, cnr_sequence.size(), header_cnr_tags)) < cnr_sequence.size()){
				cnrfound = true;
		        last = saveTags(strbuffer, cnr_states, cnr_sequence.size(), offsets, offset, last, intestazione, header_cnr_tags, tdoc);
				//printf("\n last:%d first:%d cnrfirst:%d\n",last,first,cnrfirst);
				if(last < first-1) {
					//si deve verificare se titoloDoc esiste già:
					xmlNodePtr titolonode = findChild("titoloDoc", intestazione);
					if(titolonode == NULL)
						saveTag(hp_titolodoc, offsets[last+1], offsets[first], strbuffer, intestazione, tdoc);
					else { //era presente 'Oggetto:'. Aggiornare il valore di titoloDoc:
						string strtitolo=strbuffer.substr(offsets[last+1],offsets[first]-offsets[last+1]);
						xmlAddChild(titolonode, xmlStringGetNodeList(NULL, BAD_CAST strtitolo.c_str()));
					}
				}
				last = first;
				delete[] cnr_states;
			}
		}
		last = saveTags(strbuffer, states, sequence.size(), offsets, offset, last, formulainiziale, header_formulainiziale_tags, tdoc);            
		removeProcessedElements(sequence, last);
		offset+=last+1;
	}
    delete[] states;

	//Se non è stato rilevata l'intestazione viene messo tutto in error:
	if(sequence.size() > 0 || found == false || cnrfound == false) { 
		//printf("\n HeaderParser -- Testo non rilevato (error tag) -- offset:%d size:%d\n",offset,offsets.size());
		// se rimane qualcosa mettilo in un tag errore
    	if (offset < offsets.size())
      		saveTag(hp_sconosciuto, offsets[offset], strbuffer.length(), strbuffer, errorNode, tdoc); 
		addMissingHeader(meta, descrittori, intestazione, formulainiziale, tdoc);
	}
	return notes;
}

  // parse intestazione
  if(sequence.size() > 0){
    states = new int[sequence.size()];
  	header_intestazione_model.viterbiPath(sequence, states, sequence.size());
    if ((first = getFirstMatchingState(states, sequence.size(), header_intestazione_tags)) < sequence.size()){
      found = true;
      
      // recover eventual pubblicazione before intestazione
      if(first > 0){
		pub_states = new int[first];
		vector<int> pub_sequence;
		copyElements(sequence, pub_sequence, 0, first-1);
		header_pubblicazione_model.viterbiPath(pub_sequence, pub_states, pub_sequence.size());
		pub_found = savePubblicazione(strbuffer, pub_states, pub_sequence.size(), offsets, offset, descrittori, header_pubblicazione_tags);
		//pub_found = savePubblicazione(strbuffer, pub_states, pub_sequence.size(), offsets, offset, intestazione, header_pubblicazione_tags);
		if(pub_found)
	 	 //last = saveTags(strbuffer, pub_states, pub_sequence.size(), offsets, offset, last, meta, header_pubblicazione_tags, tdoc, &notes, NULL, intestazione);
	 	 last = saveTags(strbuffer, pub_states, pub_sequence.size(), offsets, offset, last, intestazione, header_pubblicazione_tags, tdoc, &notes, NULL, intestazione);
		if(last < first-1)
		  saveTag(hp_sconosciuto, offsets[last], offsets[first], strbuffer, root_node, tdoc, NULL, intestazione); 	
		last = first;
		delete[] pub_states;
      }
      last = saveTags(strbuffer, states, sequence.size(), offsets, offset, last, intestazione, header_intestazione_tags, tdoc);            
      removeProcessedElements(sequence, last);
      offset+=last+1;
    }
    delete[] states;
  }

  // parse formulainiziale
  if(sequence.size() > 0){
    last = 0;
    states = new int[sequence.size()];
    header_formulainiziale_model.viterbiPath(sequence, states, sequence.size());
    if ((first = getFirstMatchingState(states, sequence.size(), header_intestazione_tags)) < sequence.size()){
      // recover eventual pubblicazione before formulainiziale
      if(first > 0){
		pub_states = new int[first];
		vector<int> pub_sequence;
		copyElements(sequence, pub_sequence, 0, first-1);
		header_pubblicazione_model.viterbiPath(pub_sequence, pub_states, pub_sequence.size());
		if(!pub_found)
		  pub_found = savePubblicazione(strbuffer, pub_states, pub_sequence.size(), offsets, offset, descrittori, header_pubblicazione_tags);
		  //pub_found = savePubblicazione(strbuffer, pub_states, pub_sequence.size(), offsets, offset, intestazione, header_pubblicazione_tags);
		last = saveTitle(strbuffer, pub_states, pub_sequence.size(), offsets, offset, last, descrittori, intestazione, meta, found, header_pubblicazione_tags, &notes);
		last++;
		delete[] pub_states;
      } else{
		if(!found)
		  defaultHeader(descrittori, intestazione);
		else
		  xmlNewChild(intestazione, NULL, BAD_CAST "titoloDoc", BAD_CAST "");
      }
      last = saveTags(strbuffer, states, sequence.size(), offsets, offset, last, formulainiziale, header_formulainiziale_tags, tdoc);      
      removeProcessedElements(sequence, last);
      offset+=last+1;
      found = true;
    } else{ // search pubblicazione, otherwise put all in titolodoc
      header_pubblicazione_model.viterbiPath(sequence, states, sequence.size());
      if(!pub_found)
		pub_found = savePubblicazione(strbuffer, states, sequence.size(), offsets, offset, descrittori, header_pubblicazione_tags);
	  last = saveTitle(strbuffer, states, sequence.size(), offsets, offset, last, descrittori, intestazione, meta, found, header_pubblicazione_tags, &notes);
	  removeProcessedElements(sequence, last);
	  offset += last + 1;
	  found = true;
	}
	delete[] states;
  }

  // if nothing found print default header
  if(!found){
    defaultHeader(descrittori, intestazione);
    return notes;
  }

  // save URN
  if(urn != "") {
    xmlNodePtr nodeUrn = xmlNewChild(descrittori, NULL, BAD_CAST "urn", NULL);
    xmlNewProp(nodeUrn, BAD_CAST "value", BAD_CAST urn.c_str());
  }
  
  //AGGIUNTA: addMissingMeta() qui? Esistono documenti senza footer...
  addMissingMeta(descrittori);
  
  //Aggiunta: redazionale obbligatorio? (Problemi con le note se non è presente...)
  //addChildIfMissing("redazionale", NULL, meta); 	//Ma se non ci sono note nel documento,
	  //sono problemi lo stesso! Aggiungerlo se e quando serve oppure toglierlo evenetualmente dopo?
  
  // put remains in error tag
  //if (offset < offsets.size())
  //saveTag(sconosciuto, offsets[offset], strbuffer.length(), strbuffer, out); 
  
  // erase processed part
  if (offset < offsets.size())
    header = header.substr(offsets[offset]);
  else
    header = "";

  return notes;
}

void  HeaderParser::defaultHeader(xmlNodePtr descrittori, xmlNodePtr intestazione) const				  
{
  xmlNodePtr pubblicazione = xmlNewChild(descrittori, NULL, BAD_CAST "pubblicazione", NULL);
  xmlNewProp(pubblicazione, BAD_CAST "tipo", BAD_CAST "GU");
  xmlNewProp(pubblicazione, BAD_CAST "num", BAD_CAST "");
  xmlNewProp(pubblicazione, BAD_CAST "norm", BAD_CAST "");
  xmlNodePtr entratavig = xmlNewChild(descrittori, NULL, BAD_CAST "entratainvigore", NULL);
  xmlNewProp(entratavig, BAD_CAST "norm", BAD_CAST "");
  xmlNodePtr redazione = xmlNewChild(descrittori, NULL, BAD_CAST "redazione", NULL);
  xmlNewProp(redazione, BAD_CAST "nome", BAD_CAST "");
  xmlNewProp(redazione, BAD_CAST "id", BAD_CAST "red1");
  xmlNewProp(redazione, BAD_CAST "norm", BAD_CAST "");
  xmlNodePtr urn = xmlNewChild(descrittori, NULL, BAD_CAST "urn", NULL);
  xmlNewProp(urn, BAD_CAST "value", BAD_CAST "");
  xmlNodePtr tipodoc = xmlNewChild(intestazione, NULL, BAD_CAST "tipoDoc", BAD_CAST "");
  xmlNodePtr datadoc = xmlNewChild(intestazione, NULL, BAD_CAST "dataDoc", BAD_CAST "");
  xmlNewProp(datadoc, BAD_CAST "norm", BAD_CAST "");  
  xmlNodePtr titolodoc = xmlNewChild(intestazione, NULL, BAD_CAST "titoloDoc", BAD_CAST "");
}

//Affinchè il documento sia valido aggiunge i tag obbligatori secondo il tipo di doc.
void HeaderParser::addMissingHeader(xmlNodePtr meta,xmlNodePtr descrittori,xmlNodePtr intestazione,
			      xmlNodePtr formulainiziale, int tdoc) const 
{
	xmlNodePtr tmpnode = NULL;
	
	if(tdoc==1) { //disegno di legge
		//elementi sotto intestazione:
		tmpnode = xmlNewChild(intestazione, NULL, BAD_CAST "emanante", BAD_CAST "");
		xmlAddChild(tmpnode, xmlNewText(BAD_CAST ""));
		tmpnode = xmlNewChild(intestazione, NULL, BAD_CAST "legislatura", BAD_CAST "");
		xmlAddChild(tmpnode, xmlNewText(BAD_CAST ""));
		tmpnode = xmlNewChild(intestazione, NULL, BAD_CAST "numDoc", BAD_CAST "");
		xmlAddChild(tmpnode, xmlNewText(BAD_CAST ""));
		tmpnode = xmlNewChild(intestazione, NULL, BAD_CAST "tipoDoc", BAD_CAST "");
		xmlAddChild(tmpnode, xmlNewText(BAD_CAST ""));
		tmpnode = xmlNewChild(intestazione, NULL, BAD_CAST "dataDoc", BAD_CAST "");
		xmlNewProp(tmpnode, BAD_CAST "norm", BAD_CAST "");
		xmlAddChild(tmpnode, xmlNewText(BAD_CAST ""));
		tmpnode = xmlNewChild(intestazione, NULL, BAD_CAST "titoloDoc", BAD_CAST "");
		xmlAddChild(tmpnode, xmlNewText(BAD_CAST ""));
		
		//relazione?
		//tmpnode = xmlNewChild(intestazione, NULL, BAD_CAST "relazione", BAD_CAST "");
		//xmlAddNextSibling(intestazione,);
	}

	if(tdoc==2) { //Provvedimenti CNR
		//tmpnode = xmlNewChild(intestazione, NULL, BAD_CAST "emanante", BAD_CAST "");
		//xmlAddChild(tmpnode, xmlNewText(BAD_CAST "Consiglio Nazionale delle Ricerche"));
		tmpnode = xmlNewChild(intestazione, NULL, BAD_CAST "numDoc", BAD_CAST "");
		xmlAddChild(tmpnode, xmlNewText(BAD_CAST ""));
		tmpnode = xmlNewChild(intestazione, NULL, BAD_CAST "dataDoc", BAD_CAST "");
		xmlNewProp(tmpnode, BAD_CAST "norm", BAD_CAST "");
		xmlAddChild(tmpnode, xmlNewText(BAD_CAST ""));		
		tmpnode = xmlNewChild(intestazione, NULL, BAD_CAST "titoloDoc", BAD_CAST "");
		xmlAddChild(tmpnode, xmlNewText(BAD_CAST ""));	
	}
}

//Affinchè il documento sia valido aggiunge i tag obbligatori secondo il tipo di doc.
void HeaderParser::addMissingFooter(xmlNodePtr meta,xmlNodePtr descrittori,xmlNodePtr formulafinale,
			      xmlNodePtr conclusione,int tdoc) const 
{
	if(tdoc==2) { //Provvedimenti CNR
		//All'interno di sottoscrizioni deve esserci un 'visto'
		xmlNodePtr tmpnode = findChild("sottoscrizioni", conclusione);
		if(tmpnode == NULL) return;
		xmlNodePtr vistonode = findChild("visto", tmpnode);
		if(vistonode == NULL) {
			vistonode = xmlNewNode(NULL, BAD_CAST "visto");
			xmlAddChild(vistonode, xmlNewText(BAD_CAST ""));
			xmlAddChild(tmpnode,vistonode);
		}
	}
}

unsigned int HeaderParser::saveTitle(const string& strbuffer, 
				     int * states, 
				     unsigned int statesnumber,
				     const vector<int>& offsets, 
				     unsigned int offset, 
				     unsigned int state,
				     xmlNodePtr descrittori,
				     xmlNodePtr intestazione,
				     xmlNodePtr meta,
				     bool found,
				     const hash_map<int,pair<int,int> >& tags,
				     int * id) const
{
  unsigned int first = 0, last = 0;
  // if no pubblicazione found
  if ((first = getFirstMatchingState(states, statesnumber, tags)) == statesnumber){
    if (found) // if tipo previously found, consider text as title
      saveTag(hp_titolodoc, offsets[offset], offsets[offset+first], strbuffer, intestazione, 0); 	
    else{
      defaultHeader(descrittori, intestazione);
      saveTag(hp_sconosciuto, offsets[offset], offsets[offset+first], strbuffer, root_node, 0, intestazione, NULL); 	
    }
    return first-1;
  }
  // else choose as title the longer part either before or after match
  last = getLastMatchingState(states, statesnumber, tags);
  if( (statesnumber - last) > first ){ //significa che c'è testo prima e dopo la sequenza
  										//di pubblicazione e quello dopo è più lungo.
  	//Soluzione temporanea per DL con testo redazionale/pubblicazione più lungo del titolo:
	  string longpub = strbuffer.substr(offsets[offset+last+1],offsets[offset+statesnumber]-offsets[offset+last+1]);
	  int islongpub = longpub.find("e convertito in legge");
	  if(islongpub == string::npos) {
		xmlNodePtr titlenode = saveTag(hp_titolodoc,offsets[offset+last+1], offsets[offset+statesnumber], strbuffer, intestazione, 0);     
		//saveTags(strbuffer, states, statesnumber, offsets, offset, state, meta, tags, 0, id, NULL, titlenode);
		saveTags(strbuffer, states, statesnumber, offsets, offset, state, intestazione, tags, 0, id, NULL, titlenode);
		return statesnumber-1;
	  }
  }
  //else {
    xmlNodePtr titlenode = saveTag(hp_titolodoc,offsets[offset], offsets[offset+first], strbuffer, intestazione, 0); 
    //return saveTags(strbuffer, states, statesnumber, offsets, offset, first, meta, tags, 0, id, titlenode, NULL);
    return saveTags(strbuffer, states, statesnumber, offsets, offset, first, intestazione, tags, 0, id, titlenode, NULL);
  //}
}

//Salva i valori degli attributi di "pubblicazione"
bool HeaderParser::savePubblicazione(const string& strbuffer, 
				     int * states, 
				     unsigned int statesnumber,
				     const vector<int>& offsets, 
				     unsigned int offset, 
				     xmlNodePtr descrittori,
				     const hash_map<int,pair<int,int> >& tags) const
{
  if(findChild("pubblicazione", descrittori) != NULL)
    return true;
  if(!hasCorrectStates(states, statesnumber))
    return false;

  int found = 0, maxfound = 3;
  unsigned int trimmed;
  string data, num;
  for (unsigned int i = 0; i < statesnumber && found < maxfound; i++){
    hash_map<int,pair<int,int> >::const_iterator k = tags.find(states[i]);
    assert(k != tags.end());
    if((k->second).first == hp_datapubbl){
      data = normalizeDate(strbuffer.substr(offsets[offset+i],offsets[offset+i+3]-offsets[offset+i]));
      i+=2;
      found++;
    }
    else if((k->second).first == hp_numpubbl){
      num = trimEnd(strbuffer.substr(offsets[offset+i],offsets[offset+i+1]-offsets[offset+i]), &trimmed);
      found++;      
    }
    else if((k->second).first == hp_sopubbl){
      num += "/" + trimEnd(strbuffer.substr(offsets[offset+i],offsets[offset+i+1]-offsets[offset+i]), &trimmed);
      found++;
    }
  }
  xmlNodePtr pubblicazione = xmlNewChild(descrittori, NULL, BAD_CAST "pubblicazione", NULL);
  xmlNewProp(pubblicazione, BAD_CAST "tipo", BAD_CAST "GU");
  xmlNewProp(pubblicazione, BAD_CAST "num", BAD_CAST num.c_str());
  xmlNewProp(pubblicazione, BAD_CAST "norm", BAD_CAST data.c_str());
  return true;
  //cout << "<pubblicazione tipo=\"GU\" num=\"" << num << "\" norm=\"" << data << "\"/>" << endl;
}

void copyElements(const vector<int>& src, 
		  vector<int>& dst, 
		  unsigned int start, 
		  unsigned int end)
{
  assert(end >= start && end < src.size());
  for(int i = start; i <= end; i++)
    dst.push_back(src[i]);
}

int HeaderParser::parseFooter(xmlNodePtr lastcomma, 			      
			      xmlNodePtr meta,
			      xmlNodePtr descrittori,
			      xmlNodePtr formulafinale,
			      xmlNodePtr conclusione,
			      int tdoc,
			      int notes) 
{
	//Considerando il testo all'interno dei vari tag come una lista di nodi testo/entità
	//non si può tirare fuori il contenuto del tag -errore- con xmlNodeGetContent(),
	//in particolare quando si utilizzano testi in html (ricchi di entità).
	//xmlChar* content = xmlNodeGetContent(lastcomma);
	xmlChar* content = xmlNodeListGetString(NULL, lastcomma, 0);
  //printf("\nFooter tdoc:%d\n",tdoc);
  if(content == NULL || (char *)content == ""){
  	//printf("\nFooter NULL\n");
    defaultFooter("", lastcomma); 
    //Aggiunta -- addMissingMeta() solo nel caso generico...
    if(tdoc==0)
	  addMissingMeta(descrittori);
    return notes;
  }
    
  string strbuffer = (char *) content;
  xmlFree(content);
  
  //printf("\nParseFooter\nbuffer: %s\n\n", strbuffer.c_str());
  
  //Qui si deve mettere nel lastcomma il testo fino a .\n opp. \n\n opp. .DECORAZIONE\n
  //strbuffer = strbuffer.substr(saveCommaDefault(strbuffer,lastcomma));
  strbuffer = saveCommaDefault(strbuffer,lastcomma);
  
  if(strbuffer.find_first_not_of(" \n\t\r") == string::npos) return notes; //buffer vuoto, esci
  
  //printf("\nParseFooter\nbuffer: %s\n\n", strbuffer.c_str());
  //printf("\nLASTCOMMA0:%s", (char *)xmlNodeGetContent(lastcomma));
  
  // recover URN if present
  string urn = extractURN(strbuffer);

  bool found = false;
  unsigned int last = 0, offset = 0;
  int * states = NULL;

  // extract terms
  vector<int> sequence, offsets, header_sequence, header_offsets;
  if(!footer_extractor.buildExample(sequence, offsets, strbuffer.c_str(), strbuffer.length(), 0)){
    defaultFooter(strbuffer, lastcomma);	
    //Aggiunta -- addMissingMeta() solo nel caso generico...
    if(tdoc==0)
	    addMissingMeta(descrittori);//addMissingMeta(descrittori);
    return notes;
  }
  header_extractor.buildExample(header_sequence, header_offsets, strbuffer.c_str(), strbuffer.length(), 0);

  // parse formulafinale
  if(sequence.size() > 0){
    states = new int[sequence.size()];
    footer_formulafinale_model.viterbiPath(sequence, states, sequence.size());
    if (hasCorrectStates(states, sequence.size())){
      if (!found){
      	//printf("\nformulafinale\n");

			//for(int kk=0; kk < sequence.size(); kk++)
				//printf("\n %d: sequence[]=%d   states[]=%d", kk, sequence[kk], states[kk]);
      	
		last = saveLastComma(strbuffer, states, sequence.size(), offsets, offset, lastcomma, footer_formulafinale_tags, 
			     header_sequence, header_offsets, &notes);
		found = true;
      }
      last = saveTags(strbuffer, states, sequence.size(), offsets, offset, last, formulafinale, footer_formulafinale_tags, tdoc);
      removeProcessedElements(sequence, last);
      removeProcessedElements(header_sequence, last);
      offset += last + 1;
    }
    delete[] states;
  }

  // parse dataeluogo
  if(sequence.size() > 0){
    states = new int[sequence.size()];
    footer_dataeluogo_model.viterbiPath(sequence, states, sequence.size());
    if (hasCorrectStates(states, sequence.size())){
      	//printf("\ndataeluogo\n");
    	last = 0;
    	if (!found)
			last = saveLastComma(strbuffer, states, sequence.size(), offsets, offset, lastcomma, footer_dataeluogo_tags,
				header_sequence, header_offsets, &notes);
    	else { // search for pubblicazione
			last = getFirstMatchingState(states, sequence.size(), footer_dataeluogo_tags); 
			findPubblicazione(strbuffer, header_sequence, last, header_offsets, offset, meta, conclusione, &notes);
      	}
      found = true;
      last = saveTags(strbuffer, states, sequence.size(), offsets, offset, last, conclusione, footer_dataeluogo_tags, tdoc);
      removeProcessedElements(sequence, last);
      removeProcessedElements(header_sequence, last);
      offset += last + 1;
    }
  delete[] states;
  }	

  // parse sottoscrizioni
  if(sequence.size() > 0){
    states = new int[sequence.size()];
    footer_sottoscrizioni_model.viterbiPath(sequence, states, sequence.size());
    if (hasCorrectStates(states, sequence.size())){
		//printf("\nsottoscrizioni\n");
      last = 0;
      if (!found)
		last = saveLastComma(strbuffer, states, sequence.size(), offsets, offset, lastcomma, footer_sottoscrizioni_tags,
			     header_sequence, header_offsets, &notes);
      else{ // search for pubblicazione
		last = getFirstMatchingState(states, sequence.size(), footer_sottoscrizioni_tags); 
		findPubblicazione(strbuffer, header_sequence, last, header_offsets, offset, meta, conclusione, &notes);
      }
      found = true;
      xmlNodePtr sottoscrizioni = xmlNewChild(conclusione, NULL, BAD_CAST "sottoscrizioni", NULL);
      last = saveTags(strbuffer, states, sequence.size(), offsets, offset, last, sottoscrizioni, footer_sottoscrizioni_tags, tdoc);
      if(tdoc==0)
      	addChildIfMissing("visto", NULL, sottoscrizioni);
      removeProcessedElements(sequence, last);
      removeProcessedElements(header_sequence, last);
      offset += last + 1;
    }
    delete[] states;
  }
  
  // parse annessi   // <--- MA SERVE IL MODELLO ANNESSI ?? (DL30settembre2003 crasha se tolgo il commento!)
  /*
  if(sequence.size() > 0) {
    states = new int[sequence.size()];
    footer_annessi_model.viterbiPath(sequence, states, sequence.size());
    if (hasCorrectStates(states, sequence.size())){
      //printf("\nannessi\n");
      last = 0;
      if (!found)
		last = saveLastComma(strbuffer, states, sequence.size(), offsets, offset, lastcomma, footer_annessi_tags,
			     header_sequence, header_offsets, &notes);
      else{ // search for pubblicazione
		last = getFirstMatchingState(states, sequence.size(), footer_annessi_tags); 
		findPubblicazione(strbuffer, header_sequence, last, header_offsets, offset, meta, root_node, &notes);
      }
      found = true;
      last = saveTags(strbuffer, states, sequence.size(), offsets, offset, last, meta, footer_annessi_tags, tdoc, &notes, conclusione);
      removeProcessedElements(sequence, last);
      removeProcessedElements(header_sequence, last);
      offset += last + 1;
    }
    delete[] states;
  }
  */

  // if nothing found print default footer
  if(!found)
    defaultFooter(strbuffer, lastcomma);	
  else {
    // put remains (annessi) in error tag
    if (offset < offsets.size())
      saveTag(hp_sconosciuto, offsets[offset], strbuffer.length(), strbuffer, root_node, tdoc); 
  }
  
  // save URN
  if(urn != "") {
    xmlNodePtr nodUrn = addChildIfMissing("urn", NULL, descrittori, NULL);
	xmlNewProp(nodUrn, BAD_CAST "value", BAD_CAST urn.c_str()); 
  }

  // add compulsory meta

  //Aggiunta -- addMissingMeta() solo nel caso generico...
  if(tdoc==0)
	    addMissingMeta(descrittori);

  addMissingFooter(meta, descrittori, formulafinale, conclusione, tdoc);
  
  return notes;
}

//Cerca il carattere '.' e mette in lastcomma tutto ciò che è alla 
//sinistra del carattere (il resto va in error, nel footer). <<-- deve cercare '.' e A CAPO!
/*
void  HeaderParser::defaultFooter(std::string footer, xmlNodePtr lastcomma) const
{
  footer = footer.substr(saveCommaDefault(footer,lastcomma));
    
  if (footer.find_first_not_of(" \n\t\r") != string::npos)
    saveTag(hp_sconosciuto, 0, footer.length(), footer, root_node, 0, NULL, NULL, false, NULL);
}
*/
//defaultFooter() deve soltanto mettere tutto ciò che resta in un nodo error
void HeaderParser::defaultFooter(std::string footer, xmlNodePtr lastcomma) const
{
	saveTag(hp_sconosciuto, 0, footer.length(), footer, root_node, 0, NULL, NULL, false, NULL);
}

//true if node has children that are not text or entities
//(at the moment check only for "virgolette") <-- improve in the future !?
bool HeaderParser::structureNode(xmlNodePtr node) const
{
	xmlNodePtr tmp = node;
	if(tmp==NULL) return false;
	tmp = tmp->next;
	while(tmp!=NULL) {
		if(xmlStrcmp(tmp->name, (const xmlChar *)"text") != 0)
			printf("\nstructureNode - No text child:%s\n",tmp->name);
		if(!xmlStrcmp(tmp->name, (const xmlChar *)"virgolette"))
			return true;
		tmp=tmp->next;
	}
	return false;
}

//In caso di ultimo comma strutturato, cioè composto non di solo testo ed entità,
//la ricerca del footer inizia più tardi. Es.: se è presente uno o più elementi "virgolette",
//si mettono in lastcomma tali elementi e si inizia a cercare il "punto e a capo" 
//(saveCommaDefault()) subito dopo l'ultimo nodo "virgolette".
xmlNodePtr HeaderParser::getFooterFromStructureNode(xmlNodePtr node) const
{
	xmlNodePtr tmp = node, last = node;
	if(tmp==NULL) return NULL;
	tmp = tmp->next;
	while(tmp!=NULL) {
		if(!xmlStrcmp(tmp->name, (const xmlChar *)"virgolette"))
			last = tmp->next;
		tmp=tmp->next;
	}
	return last;
}

//Mette nell'ultimo comma il testo fino a un ". \n"
//Restituisce l'indice del carattere successivo il "\n" 
//Se non trova il "punto e a capo" ritorna la lunghezza della stringa (cioè considera
//tutto come corpo dell'ultimo comma...)
std::string HeaderParser::saveCommaDefault(std::string footer, xmlNodePtr lastcomma) const
{
//  unsigned int dot=0, ret1=0, ret2=0, ret=0;
  string::size_type dot=0, ret1=0, ret2=0, ret=0;
  bool found = false, structure = false;
  xmlNodePtr last = NULL;
  
  if(structureNode(lastcomma)) {
  	printf("\nLASTCOMMA has a structure!\n");
  	last = getFooterFromStructureNode(lastcomma);
  	structure = true;
  	if(last!=NULL) {
  		footer = (char *)xmlNodeListGetString(NULL, last, 0);
  	} else
  		footer = ""; //Se last è NULL non c'è niente dopo VIRGOLETTE ?
  }
  
  //I doc. html possono non avere un carattere di ritorno a capo alla fine del footer
  //quindi si aggiunge il carattere alla fine del footer (non disturba gli altri casi?!)
  footer.append(1, '\n');
  
  //printf("\n saveCommaDefault - analizzo FOOTER:\n%s",footer.c_str());

  //Assume che ci sia almeno un carattere di ritorno a capo (\n opp. \r)
  //all'interno della stringa del footer
  dot = footer.find('.');
  while(dot != string::npos) {
	ret1 = footer.find('\n',dot);
	ret2 = footer.find('\r',dot);
	ret = 0; //primo carattere \n opp. \r dopo il '.'

	if(ret1 != string::npos)
		ret = ret1;
	if(ret2 != string::npos)
			if( (ret1 == string::npos) || ret2 < ret1)
	  			ret = ret2;
	if(ret1 == string::npos && ret2 == string::npos)
		return 0;
		  	
	string middlestr = footer.substr(dot+1,ret-dot-1);
	
	if(middlestr.find_first_not_of(" \n\t\r") != string::npos)
		dot = footer.find('.',dot+1); //non è un punto e a capo, guarda il punto successivo
	else {
		found = true;
		break;
	}
  }
  
  printf("\nFINE. RET:%d\n",ret);
  
  if(structure) {
  	if(!found)
  		return footer.substr(footer.length());
  	else {
  		xmlUnlinkNode(last);
  		xmlFreeNode(last);
		addSiblingString(lastcomma->parent, footer.substr(0, ret+1)); //lastcomma non è corpo ma è gia il child !
		return footer.substr(ret+1);
  	}
  }
  
  if(!found)
	return footer.substr(footer.length());   //praticamente non restituisce niente...
	//return "";
  	
  xmlNodeSetContent(lastcomma, BAD_CAST "");
  addSiblingString(lastcomma->parent, footer.substr(0, ret+1)); //lastcomma non è corpo ma è gia il child !
  return footer.substr(ret+1);
}

xmlNodePtr HeaderParser::addChildIfMissing(const char * nodename, 
					   bool * added,
					   xmlNodePtr startnode, 
					   const char * content) const
{
  if(added != NULL)
    *added = false;
  if(startnode == NULL)
    startnode = root_node;
  
  xmlNodePtr child = findChild(nodename, startnode);
  if(child == NULL){
    child = xmlNewChild(startnode, NULL, BAD_CAST nodename, BAD_CAST content);
    if(added != NULL)
      *added = true;
  }
  return child;
}

xmlNodePtr HeaderParser::findChild(const char * nodename, xmlNodePtr startnode) const
{
  if(startnode == NULL)
    startnode = root_node;
  
  xmlNodePtr child = startnode->xmlChildrenNode;
  while (child != NULL) {
    if ((!xmlStrcmp(child->name, (const xmlChar *)nodename)))
      return child;
    child = child->next;
  }
  return NULL;
}

void HeaderParser::addMissingMeta(xmlNodePtr descrittori) const
{
  bool added = false;
  xmlNodePtr pubblicazione = addChildIfMissing("pubblicazione", &added, descrittori, NULL);
  if(added){
    xmlNewProp(pubblicazione, BAD_CAST "tipo", BAD_CAST "GU");
    xmlNewProp(pubblicazione, BAD_CAST "num", BAD_CAST "");
    xmlNewProp(pubblicazione, BAD_CAST "norm", BAD_CAST "");
  }
  xmlNodePtr entratavig = addChildIfMissing("entratainvigore", &added, descrittori, NULL);   
  if(added)
	xmlNewProp(entratavig, BAD_CAST "norm", BAD_CAST "");
  //xmlNodePtr vigenza = addChildIfMissing("vigenza", &added, descrittori, NULL); 
  xmlNodePtr redazione = addChildIfMissing("redazione", &added, descrittori, NULL);   
  if(added) {
    //xmlNewProp(vigenza, BAD_CAST "id", BAD_CAST "v1");
    xmlNewProp(redazione, BAD_CAST "nome", BAD_CAST "");
    xmlNewProp(redazione, BAD_CAST "id", BAD_CAST "red1");
    xmlNewProp(redazione, BAD_CAST "norm", BAD_CAST "");
  }
  xmlNodePtr urn = addChildIfMissing("urn", &added, descrittori);
  if(added)
	xmlNewProp(urn,BAD_CAST "value", BAD_CAST "");
}

//TRUE se la sequenza non è completamente composta di stati -1 e 0
bool HeaderParser::hasCorrectStates(int * states, int statesnumber)
{
  for(int i = 0; i < statesnumber; i++)
    if(states[i] > 0)
      return true;
  return false;
}

// primo stato diverso da errorTag (in particolare 0) ? 
unsigned int HeaderParser::getFirstMatchingState(int * states, 
						 unsigned int statesnumber,
						 const hash_map<int,pair<int,int> >& tags) const
{
  unsigned int state = 0;
  while(state < statesnumber){
    hash_map<int,pair<int,int> >::const_iterator k = tags.find(states[state]);
    assert(k != tags.end());
    if(!errorTag((k->second).first))
      break;
    state++;
  }
  return state;
}

// ultimo stato diverso da errorTag (in particolare -1) ? 
int HeaderParser::getLastMatchingState(int * states, 
				       unsigned int statesnumber,
				       const hash_map<int,pair<int,int> >& tags) const
{
  int state = statesnumber-1;
  while(state >= 0){
    hash_map<int,pair<int,int> >::const_iterator k = tags.find(states[state]);
    assert(k != tags.end());
    if(!errorTag((k->second).first))
      break;
    state--;
  }
  return state;
}

void HeaderParser::findPubblicazione(const string& strbuffer, 
				     const vector<int>& sequence, 
				     int first,
				     const vector<int>& offsets, 
				     unsigned int offset, 
				     xmlNodePtr meta,
				     xmlNodePtr curr_node,
				     int * notes) const
{
  if (first == 0)
    return;
	//printf("\nFindPubblicazione()");
  int last = 0;
  int * pub_states = new int[first];
  vector<int> pub_sequence;
  copyElements(sequence, pub_sequence, 0, first-1);
  header_pubblicazione_model.viterbiPath(pub_sequence, pub_states, pub_sequence.size());
  savePubblicazione(strbuffer, pub_states, pub_sequence.size(), offsets, offset, findChild("descrittori", meta), header_pubblicazione_tags);
  if(hasCorrectStates(pub_states, pub_sequence.size()))
    last = saveTags(strbuffer, pub_states, pub_sequence.size(), offsets, offset, 0, meta, header_pubblicazione_tags, 0, notes, curr_node);     
  //printf("\nFindPubblicazione() - first:%d last:%d\n",first,last);
  //if(last < first-1) // <-- se c'è solo una parola tra formulafinale e dataeluogo non viene messa in error!
  if(last <= first-1)
    saveTag(hp_sconosciuto, offsets[offset+last], offsets[offset+first], strbuffer, curr_node, 0);
  delete[] pub_states;
}   

unsigned int HeaderParser::saveLastComma(const string& strbuffer, 
					 int * states, 
					 unsigned int statesnumber, 
					 const vector<int>& offsets, 
					 unsigned int offset, 
					 xmlNodePtr lastcomma, 
					 const hash_map<int,pair<int,int> >& tags, 
					 const vector<int>& header_sequence, 
					 const vector<int>& header_offsets, 
					 int * notes) const 
{
  int state = getFirstMatchingState(states, statesnumber, tags);
  //printf("\nSaveLastComma - state:%d\n",state);
  if(state==0) {
  	//In questo caso viene rilevata una sequenza nel footer in uno dei modelli ma tale
  	//sequenza inizia con l'inizio del buffer: o si duplica il contenuto
  	//del buffer (va sia nell'ultimo comma con tag <mod> (?) che nel footer) oppure
  	//si lascia l'ultimo comma vuoto (l'errore è commesso dal modello del footer...):
  	//Quindi o si corregge il modello (solitamente sottoscrizioni è quello che "becca tutto",
  	//oppure si potrebbe aggiungere subito in lastcomma tutto ciò che precede un punto e un
  	//ritorno a capo.  	
  	//Invece nel caso non venga rilevato niente, cioè nessuna sequenza con nessun modello
  	//del footer, e dunque non viene mai richiamata la saveLastComma(), allora si
  	//richiama defaultFooter() alla fine di parseFooter().
  	//printf("\nsavelascomma 0\n");

  	//AGGIORNAMENTO:
  	//Dal momento che il testo fino al primo "punto e a capo" viene messo nel corpo dell'ultimo
  	//comma e non viene passato ai modelli del footer, se si arriva qui è ok e non si deve fare niente.
  	//In questo caso defaultFooter() deve soltanto mettere tutto ciò che resta in un nodo error.
    return state; 
  }
    				
  //printf("\nSave last state:%d\n",state);
  // find eventual pubblicazione
  xmlNodePtr meta = findChild("meta");
  int * pub_states = new int[state];
  vector<int> pub_sequence;
  copyElements(header_sequence, pub_sequence, 0, state-1);
  header_pubblicazione_model.viterbiPath(pub_sequence, pub_states, pub_sequence.size());
  int first = getFirstMatchingState(pub_states, pub_sequence.size(), header_pubblicazione_tags);
  if (first == pub_sequence.size()) {
  	//pubblicazione non trovata
    //xmlNodeSetContent(lastcomma, BAD_CAST strbuffer.substr(offsets[offset], offsets[offset+state]-offsets[offset]).c_str());
    string nlstr = strbuffer.substr(offsets[offset], offsets[offset+state]-offsets[offset]);
    //xmlAddChild(lastcomma, xmlStringGetNodeList(NULL, BAD_CAST nlstr.c_str()));
    addSiblingString(lastcomma, nlstr);
  	//printf("\nsavelascomma 1:%s\n", nlstr.c_str());
  } else {
  	//pubblicazione trovata!?
    //xmlNodeSetContent(lastcomma, BAD_CAST strbuffer.substr(offsets[offset], offsets[offset+first]-offsets[offset]).c_str());
    string nlstr = strbuffer.substr(offsets[offset], offsets[offset+first]-offsets[offset]);
	//xmlAddChild(lastcomma, xmlStringGetNodeList(NULL, BAD_CAST nlstr.c_str()));
	addSiblingString(lastcomma, nlstr);
  	//printf("\nsavelascomma 2\n");

    savePubblicazione(strbuffer, pub_states, pub_sequence.size(), header_offsets, offset, findChild("descrittori", meta), header_pubblicazione_tags);
    int last = saveTags(strbuffer, pub_states, pub_sequence.size(), header_offsets, offset, first, meta, header_pubblicazione_tags, 0, notes, lastcomma, NULL);     
    if(last < state-1)
      saveTag(hp_sconosciuto, offsets[offset+last], offsets[offset+state], strbuffer, lastcomma, 0);
  }
  delete[] pub_states;
  return state;
}

void HeaderParser::removeProcessedElements(vector<int>& sequence, int last)
{
  for(unsigned int i = last+1; i < sequence.size(); i++)
    sequence[i-last-1] = sequence[i];
  sequence.resize(sequence.size()-last-1);
}

unsigned int HeaderParser::saveTags(const string& strbuffer, 
				    int * states, 
				    unsigned int statesnumber,
				    const vector<int>& offsets, 
				    unsigned int offset, 
				    unsigned int initstate,
				    xmlNodePtr startnode,
				    const hash_map<int,pair<int,int> >& tags,
				    int tdoc,
				    int * id,
				    xmlNodePtr prev_node,
				    xmlNodePtr subs_node) const
{
  hash_map<int,pair<int,int> >::const_iterator statetag,currstatetag;

  // get last matched state
  int last;
  for(last = statesnumber-1; last >= 0; last--){
    statetag = tags.find(states[last]);
    assert(statetag != tags.end());
    if(!errorTag((statetag->second).first))
      break;
  }
 
  // save tags for matching states
  int currtag = -1, start = -1;
  for(int state=initstate; state <= last; state++){
    statetag = tags.find(states[state]);
    assert(statetag != tags.end());
    if((state == initstate || states[state] != states[state-1]) && 
	((statetag->second).second > -2) &&
	((statetag->second).second > 0 || (statetag->second).first != currtag)){
      if(currtag != -1)
		saveTag(currtag, start, offsets[state+offset], strbuffer, startnode, tdoc, prev_node, subs_node, ((currstatetag->second).second > -1), id);
      currstatetag = statetag;
      currtag = (statetag->second).first;
      start = offsets[state+offset];
    }
    if (state == last){
      if(currtag != -1){
	//if(last < statesnumber-1)
	  saveTag(currtag, start, offsets[state+offset+1], strbuffer, startnode, tdoc, prev_node, subs_node, ((currstatetag->second).second != -1), id);
	  //else
	  //saveTag(currtag, start, strbuffer.length(), strbuffer, out, ((currstatetag->second).second > -1), tdoc);
      }
    }
  }
  return last;
}

xmlNodePtr HeaderParser::saveTag(int tagvalue,
				 int start,
				 int end,
				 const string& buffer,
				 xmlNodePtr startnode,
				 int tdoc,
				 xmlNodePtr prev_node,
				 xmlNodePtr subs_node,
				 bool withtags,
				 int * id) const
{
  if(errorTag(tagvalue)){
    xmlNodePtr errornode = xmlNewPI(BAD_CAST "error", BAD_CAST buffer.substr(start,end-start).c_str());
	/*
	//Il nodo PI vuole xmlChar come 'content' e non vuole figli, le entità non si "risolvono" nemmeno così:
	xmlChar *errorText = xmlNodeListGetString(NULL, 
					xmlStringGetNodeList(NULL, BAD_CAST buffer.substr(start,end-start).c_str()),0);
	xmlNodePtr errornode = xmlNewPI(BAD_CAST "error", errorText);
	xmlFree(errorText);
	*/
    if(prev_node != NULL)
		xmlAddNextSibling(prev_node, errornode);
	else if (subs_node != NULL)
		xmlAddPrevSibling(subs_node, errornode);
	else
		xmlAddChild(startnode, errornode);
    return errornode;
  }
  if(ignoreTag(tagvalue)){ //scrive il testo fuori da un tag specifico (hp_varie)
    xmlAddChild(startnode, xmlNewText(BAD_CAST buffer.substr(start,end-start).c_str()));
    //xmlNewChild(startnode, NULL, currnode->name, BAD_CAST buffer.substr(start,end-start).c_str());
    return NULL;
  }
  //AGGIUNTO: non scrive niente (salta la parola). 
  if(nothingTag(tagvalue)) 
  	return NULL;
  //
    if(tdoc == 1 && tagvalue == hp_div) {
  	addFormatTagsDiv(buffer.substr(start,end-start), startnode);
  	return NULL;	
  }
  
  //Pubblicazione
  if(pubTag(tagvalue)) {
  	//printf("\npubTag:%s\n",buffer.substr(start,end-start).c_str());
  	xmlAddChild(startnode, xmlNewText(BAD_CAST buffer.substr(start,end-start).c_str()));
    return NULL;
  }
  
  xmlNodePtr currnode = (withtags) ? openTag(tagvalue, startnode, buffer.substr(start,end-start), id) : startnode;
  
  //Aggiunta:
  if(tagvalue == hp_relazione) {
	addFormatTagsDivPeriod(buffer.substr(start,end-start), currnode);
	xmlAddNextSibling(startnode,currnode);
	return currnode;
  }
    
  if(formatTag(tagvalue)){
    if(tagvalue == hp_preambolo)
        addSemicolumnFormatTags(buffer.substr(start,end-start), currnode);
    else
      addFormatTags(buffer.substr(start,end-start), currnode);
  }
  else if(tagvalue == hp_formulafinale)
    xmlNewChild(startnode, NULL, BAD_CAST "h:p", BAD_CAST buffer.substr(start,buffer.find_last_not_of(" \r\n\t", end-1)-start+1).c_str());
  else if( trimmedTag(tagvalue) && withtags ||
  			(tdoc == 1 && trimmedTagDDL(tagvalue)) ){
    unsigned int trimmed = 0;
    //xmlNewChild(startnode, NULL, currnode->name, BAD_CAST trimEnd(buffer.substr(start,end-start), &trimmed).c_str());
    //Attacca il testo come una lista testo/entità!
    xmlAddChild(currnode, xmlStringGetNodeList(NULL, BAD_CAST trimEnd(buffer.substr(start,end-start), &trimmed).c_str()));
    if (trimmed < end-start) {
    	if(tdoc==1) {
    		//Con i DDL metti in error tutto ciò che non viene riconosciuto
    		//(e che ha lunghezza 'trimmata' maggiore di 0...)
    		int isblank = buffer.substr(start+trimmed,end-start-trimmed).find_last_not_of(" \n\t\r");
    		if( isblank > 0 ) {
	    	  	xmlNodePtr errnode = xmlNewPI(BAD_CAST "error", BAD_CAST buffer.substr(start+trimmed,end-start-trimmed).c_str());
	    	  	xmlAddChild(startnode, errnode);
    		}
    	} else
    	    //Ma se quello che rimane è tutta roba " \n\t\r" non la riscrivere neanche...
    	    if( buffer.substr(start+trimmed,end-start-trimmed).find_last_not_of(" \n\t\r") != string::npos)
	   			//xmlAddChild(startnode, xmlNewText(BAD_CAST buffer.substr(start+trimmed,end-start-trimmed).c_str()));
	   			//Attacca il testo come una lista testo/entità!
	   			xmlAddChild(startnode, xmlStringGetNodeList(NULL, BAD_CAST buffer.substr(start+trimmed,end-start-trimmed).c_str()));
    }
  }
  else
    //Attacca il testo come una lista testo/entità!
    xmlAddChild(currnode, xmlStringGetNodeList(NULL, BAD_CAST buffer.substr(start,end-start).c_str()));
  return currnode;
}

xmlNodePtr HeaderParser::openTag(int tagvalue,
				 xmlNodePtr startnode, 
				 const std::string& tagcontent,
				 int * id) const
{
  xmlNodePtr contextnode = openContextTags(tagvalue, startnode);
  xmlNodePtr tagnode = xmlNewChild(contextnode, NULL, BAD_CAST tagName(tagvalue), NULL);
  addTagAttributes(tagvalue, tagcontent, tagnode, id);
  return tagnode;
}

xmlNodePtr HeaderParser::openContextTags(int tagvalue, xmlNodePtr startnode) const 
{
  xmlNodePtr contextnode = startnode;
  switch(HP_tagTipo(tagvalue)){
  case hp_pubblicazione: 
  case hp_datapubbl:
  case hp_numpubbl:
  case hp_sopubbl:
  case hp_nota:
  case hp_registrazione:
    contextnode = addChildIfMissing("redazionale", NULL, startnode);    
  }
  return contextnode;
}

void HeaderParser::addTagAttributes(int tagvalue, 
				    const std::string& tagcontent,
				    xmlNodePtr tagnode, 
				    int * id) const
{
  switch(HP_tagTipo(tagvalue)){
  case hp_datadoc:
    xmlNewProp(tagnode, BAD_CAST "norm", BAD_CAST normalizeDate(tagcontent).c_str());
    break;
  case hp_dataeluogo:
    xmlNewProp(tagnode, BAD_CAST "norm", BAD_CAST normalizeDate(tagcontent).c_str());
    xmlNewProp(tagnode, BAD_CAST "codice", BAD_CAST "00100");
    break;
  case hp_pubblicazione:
  case hp_datapubbl:
  case hp_numpubbl:
  case hp_sopubbl:
  case hp_nota:
  case hp_registrazione:{
    ostringstream attr;
    attr << tagIdName(HP_tagTipo(tagvalue)) << (*id)++;
    //attr << (*id)++;
    xmlNewProp(tagnode, BAD_CAST "id", BAD_CAST attr.str().c_str());
    break;
    }
  }
}

//Modifica: 
//Si deve chiudere il paragrafo solo quando si rileva un ';' (oppure un '.' ?!) 
//alla fine della riga, non se viene trovato in mezzo al testo.
void HeaderParser::addSemicolumnFormatTags(string text, xmlNodePtr startnode) const
{
  istringstream in(text);
  string line, buf;
  unsigned int end = 0;

  while(Lexer::getLine(in, line)){
    //unsigned int semicolumn = line.find(";");
    unsigned int lastchr = line.find_last_not_of(" \t\n\r");
    if(lastchr != string::npos && 
    		(line.substr(lastchr,1)==";" || line.substr(lastchr,1)==".") ) {
    //if(semicolumn != string::npos){
      //xmlNewChild(startnode, NULL, BAD_CAST "h:p", BAD_CAST (buf + line.substr(0,semicolumn+1)).c_str());
      xmlNewChild(startnode, NULL, BAD_CAST "h:p", BAD_CAST (buf + line.substr(0,lastchr+1)).c_str());
      /*
      if(semicolumn < line.length()-1)
		buf = line.substr(semicolumn+1);
      else
		buf = "";
		*/
	  buf="";
      continue;
    }
    if (line.find_first_not_of(" \n\t\r") != string::npos)
      buf += line + "\n";
  }
  if ((end = buf.find_last_not_of(" \n\t\r")) != string::npos)
    xmlNewChild(startnode, NULL, BAD_CAST "h:p", BAD_CAST buf.substr(0,end+1).c_str());
}

//Modificata: *considera* anche righe composte solo da "-" e *non* togliere eventuali "-" ad inizio riga
void HeaderParser::addFormatTags(string buf, xmlNodePtr startnode) const
{
  istringstream in(buf);
  string line, out;

  while(Lexer::getLine(in, line))
    if (line.find_first_not_of(" \n\t\r") != string::npos) {
      int ststr = line.find_first_not_of(" \n\t\r");
      xmlNewChild(startnode, NULL, BAD_CAST "h:p", BAD_CAST line.substr(ststr,line.size()-ststr+1).c_str());
    }
}

//Metti h:div al posto di h:p (uno per riga)
void HeaderParser::addFormatTagsDiv(string buf, xmlNodePtr startnode) const
{
  istringstream in(buf);
  string line, out;

  while(Lexer::getLine(in, line))
    if (line.find_first_not_of(" \n\t\r") != string::npos) {
      int ststr = line.find_first_not_of(" \n\t\r");
      //printf("\nDIV ststr:%d contents:%s", ststr, line.substr(ststr,line.size()-ststr+1).c_str());

      //xmlNewChild(startnode, NULL, BAD_CAST "h:div", BAD_CAST line.substr(ststr,line.size()-ststr+1).c_str());
      xmlNodePtr div = xmlNewChild(startnode, NULL, BAD_CAST "h:div", NULL);
      xmlAddChild(div, xmlStringGetNodeList(NULL, BAD_CAST line.substr(ststr,line.size()-ststr+1).c_str()));
    }
}

//Aggiunta: formatta il testo in h:div (uno per periodo)
void HeaderParser::addFormatTagsDivPeriod(string text, xmlNodePtr startnode) const
{
  istringstream in(text);
  string line, buf;
  unsigned int end = 0;

  while(Lexer::getLine(in, line)){
    unsigned int lastchr = line.find_last_not_of(" \t\n\r");
    if(lastchr != string::npos && line.substr(lastchr,1)=="." ) {
      xmlNewChild(startnode, NULL, BAD_CAST "h:div", BAD_CAST (buf + line.substr(0,lastchr+1)).c_str());
	  buf="";
      continue;
    }
    if (line.find_first_not_of(" \n\t\r") != string::npos)
      buf += line + "\n";
  }
  if ((end = buf.find_last_not_of(" \n\t\r")) != string::npos)
    xmlNewChild(startnode, NULL, BAD_CAST "h:div", BAD_CAST buf.substr(0,end+1).c_str());
}

//Aggiungi attributi di stile ai div nei DDL
//(metti l'allineamento centrale e se il contenuto inizia con '(' e termina con ')' metti in grassetto.
void addDivDecorations(xmlNodePtr parent) {
	xmlNodePtr ptr1 = NULL, ptr2 = NULL;
	string pname = "", name = "", content = "", c1 = "", c2 = "";
	if(parent == NULL) return;
	ptr1 = parent->children;
	while(ptr1!=NULL) {
		pname = (char *)ptr1->name;
		ptr2 = ptr1->children;
		while(ptr2!=NULL) {
			name = (char *)ptr2->name;
			xmlChar *xchar = xmlNodeGetContent(ptr2);
			content = (char *)xchar;
			if(xchar!=NULL && content.length()>0) {
				if(pname.compare("intestazione")==0) {
					c1=content.substr(content.find_first_not_of(" \n\t\r"),1);
					c2=content.substr(content.find_last_not_of(" \n\t\r"),1);
					if(name.compare("h:div")==0) {
						xmlAttrPtr attr;
						if(c1.compare("(")==0  && c2.compare(")")==0)
							attr = xmlNewProp(ptr2,BAD_CAST "style",BAD_CAST "text-align:center");
						else
							attr = xmlNewProp(ptr2,BAD_CAST "style",BAD_CAST "text-align:center; font:bold");
					}
				} else if(pname.compare("relazione")==0) {
					int first = content.find_first_not_of(" \n\r\t");
					int last = content.find_last_not_of(" \n\t\r");
					if(content.substr(first,last-first+1).compare("DISEGNO DI LEGGE") == 0) {
						xmlAttrPtr attr;
						attr = xmlNewProp(ptr2,BAD_CAST "style",BAD_CAST "text-align:center; font:bold");
					}
				}
			}
			xmlFree(xchar);
			ptr2=ptr2->next;
		}
		ptr1=ptr1->next;
	}
}

//Aggiungi alla fine della lista 'children' 
//(attacca una lista testo/entità piuttosto che un nodo di testo)
void addSiblingString(xmlNodePtr node, string str) {
	xmlNodePtr child = node->children;
	xmlNodePtr tmp = NULL;
	xmlNodePtr ncontent = xmlStringGetNodeList(NULL, BAD_CAST str.c_str());

	if(child==NULL) {
		xmlAddChild(node, ncontent);
		return;
	}

	tmp = child->next;
	while(tmp!=NULL) {
		child = tmp;
		tmp = tmp->next;
	}
	child->next = ncontent;
}

string normalizeDate(const string& buffer)
{
	//Aggiunta (data del tipo xx/yy/zz )
	if(buffer.find_first_of("gfmalsond") == string::npos && 
			buffer.find_first_of("GFMALSOND") == string::npos) {
		//int tmpfind = buffer.find_first_of("gfmalsond");
		unsigned int beg = buffer.find_first_of("0123456789");
		unsigned int end = buffer.find_last_of("0123456789");
		if (beg == string::npos || end == string::npos) return "";
		unsigned int sep_int = buffer.find_first_of("/.- ", beg);
  		if (sep_int == string::npos) return "";		
		string sep = buffer.substr(sep_int,1); //carattere separatore ( / . " " -)
		unsigned int sep_mese = buffer.find_first_of(sep,beg);
		unsigned int sep_anno = buffer.find_first_of(sep,sep_mese+1);
  		if (sep_mese == string::npos || sep_anno == string::npos) return "";
  		string day = buffer.substr(beg,sep_mese-beg);
  		string mese = buffer.substr(sep_mese+1,sep_anno-sep_mese-1);
		string anno = buffer.substr(sep_anno+1,end-sep_anno);
		//printf("\nstring:%s beg:%d end:%d sep_mese:%d sep_anno:%d day:%s mese:%s anno:%s\n",
			//		buffer.c_str(),beg,end,sep_mese,sep_anno,day.c_str(),mese.c_str(),anno.c_str());
		if(day.length() == 1)
			day = "0"+day;
		if(mese.length() == 1)
			mese = "0"+mese;
		if(anno.length() == 2)
			if(!strcmp(anno.substr(0,1).c_str(),"0"))
				anno = "20"+anno;
			else
				anno = "19"+anno;
		return anno+mese+day;
	}
  //Originale (data del tipo XX mese YYYY)
  unsigned int beg = buffer.find_first_of("0123456789");
  if (beg == string::npos) return "";
  unsigned int end = buffer.find_first_not_of("0123456789", beg);
  if (end == string::npos) return "";
  string day = buffer.substr(beg, end - beg);
  if(day.length() == 1)
    day = "0" + day;
  beg = buffer.find_first_not_of(" \r\t\n", end);
  if (beg == string::npos) return "";
  end = buffer.find_first_of(" \r\t\n", beg);
  if (end == string::npos) return "";
  string month = buffer.substr(beg, end - beg);
  if(month.find_first_not_of("0123456789") != string::npos)
    month = MonthMapping[lowercase(month)];
  else if(month.length() == 1)
    month = "0" + month;
  if (month == "") return "";
  beg = buffer.find_first_of("0123456789", end);
  if (beg == string::npos) return "";
  end = buffer.find_first_not_of("0123456789", beg);
  if (end == string::npos) end = buffer.length();
  string year = buffer.substr(beg, end - beg);
  if(year.length() != 4)
    return "";
  return year + month + day;
}

string lowercase(const string& word)
{

  string lower;
  for(unsigned int i=0; i<word.length(); i++)
    lower += tolower(word[i]);
  return lower;
}

string trimEnd(const string& buf, unsigned int * trimmed)
{
  *trimmed = buf.find_last_not_of("`';:,.?!\{}[]\\|/<>-_°() \r\t\n") + 1;
  return buf.substr(0,*trimmed);
}

void SqueezeWords(string& buf)
{
  static int numwords = 15;
  static string words[] = {"d i s p o n e", "d e c r e t a", "a d o t t a", "e m a n a", "p r o m u l g a",
			   "D i s p o n e", "D e c r e t a", "A d o t t a", "E m a n a", "P r o m u l g a",
			   "D I S P O N E", "D E C R E T A", "A D O T T A", "E M A N A", "P R O M U L G A"};
  static string squeezed[] = {"dispone", "decreta", "adotta", "emana", "promulga",
			      "Dispone", "Decreta", "Adotta", "Emana", "Promulga",
			      "DISPONE", "DECRETA", "ADOTTA", "EMANA", "PROMULGA"};
  for (int i = 0; i < numwords; i++){
    int beg = 0;
    while((beg = buf.find(words[i],beg)) != string::npos)
      buf.replace(beg,words[i].length(),squeezed[i]);
  }
}

//Replace "1°" (primo) with "1"
void delPrimo(string& buf)
{
	int beg = 0;
	while((beg = buf.find("1°",beg)) != string::npos)
		buf.replace(beg,2,"1");
	beg = 0;
	while((beg = buf.find("1&#xB0;",beg)) != string::npos)
		buf.replace(beg,7,"1");

	beg = 0;
	while((beg = buf.find("<",beg)) != string::npos)
		buf.replace(beg,1,"&lt;");
	beg = 0;
	while((beg = buf.find(">",beg)) != string::npos)
		buf.replace(beg,1,"&gt;");
}

bool HeaderParser::errorTag(int tagvalue) const
{
  switch(HP_tagTipo(tagvalue)){
  case hp_annessi:
  case hp_sconosciuto: return true;
  default: return false;
  }
}

bool HeaderParser::trimmedTag(int tagvalue) const
{
  switch(HP_tagTipo(tagvalue)){
  case hp_numdoc:
  case hp_datadoc: return true;
  default: return false;
  }
}
//Aggiunta: con i DDL viene messo in error ciò che non viene riconosciuto
//(funzione di prova...)
bool HeaderParser::trimmedTagDDL(int tagvalue) const
{
  switch(HP_tagTipo(tagvalue)){
  case hp_numdoc:
  case hp_datadoc:
  case hp_legislatura:
  case hp_tipodoc:
  //case hp_titolodoc:
  case hp_emanante:
  case hp_relazione: return true;
  default: return false;
  }
}

bool HeaderParser::pubTag(int tagvalue) const
{
  switch(HP_tagTipo(tagvalue)){
  case hp_pubblicazione: 
  case hp_datapubbl:
  case hp_numpubbl:
  case hp_sopubbl: return true;
  default: return false;
  }
}

bool HeaderParser::ignoreTag(int tagvalue) const
{
  switch(HP_tagTipo(tagvalue)){
  case hp_varie: return true;
  default: return false;
  }
}

//AGGIUNTO: non scrive niente (salta la parola). 
//Inutile? Niente deve essere tolto dal documento, si deve marcare ciò che viene riconosciuto.
bool HeaderParser::nothingTag(int tagvalue) const
{
  switch(HP_tagTipo(tagvalue)){
  case hp_nothing: return true;
  default: return false;
  }
}

bool HeaderParser::formatTag(int tagvalue) const
{
  switch(HP_tagTipo(tagvalue)){
  case hp_nota:
  case hp_datapubbl:
  case hp_numpubbl:
  case hp_sopubbl:
  case hp_registrazione:
  case hp_lavoripreparatori:
  case hp_annessi:
  case hp_pubblicazione: 
  case hp_formulainiziale:
  case hp_preambolo: return true;
  default: return false;
  }
}

const char * HeaderParser::tagIdName(int tagvalue)
{
	switch(HP_tagTipo(tagvalue)){
		case hp_pubblicazione:	return "n";
		case hp_datapubbl:		return "n";
		case hp_numpubbl:			return "n";
		case hp_sopubbl:			return "n";
		case hp_nota:				return "n";
		case hp_registrazione:	return "n";
		default:	return "???";
	}
}

const char * HeaderParser::tagName(int tagvalue)
{
  switch(HP_tagTipo(tagvalue)){
  case hp_libro:	return "libro";
  case hp_parte:	return "parte";
  case hp_titolo:	return "titolo";
  case hp_capo:	return "capo";
  case hp_sezione:	return "sezione";
  case hp_articolo: return "articolo";
  case hp_comma:	return "comma";
  case hp_lettera:	return "el";
  case hp_numero:	return "en";
  case hp_num:	return "num";
  case hp_corpo:	return "corpo";
  case hp_alinea:	return "alinea";
  case hp_rubrica:	return "rubrica";
  case hp_tipodoc: return "tipoDoc";
  case hp_datadoc: return "dataDoc";
  case hp_numdoc:  return "numDoc";
  case hp_titolodoc: return "titoloDoc";
  case hp_formulainiziale: return "formulainiziale";
  case hp_formulafinale: return "formulafinale";
  case hp_dataeluogo: return "dataeluogo";
  case hp_sottoscrizioni: return "sottoscrizioni";
  case hp_pubblicazione: return "nota";
  case hp_emanante: return "emanante";
  case hp_conclusione: return "conclusione";
  case hp_intestazione: return "intestazione";
  case hp_annessi: return "nota";
  case hp_nota: return "nota";
  case hp_registrazione: return "nota";
  case hp_lavoripreparatori: return "lavoripreparatori";
  case hp_varie: return "varie";
  case hp_sottoscrivente: return "sottoscrivente";
  case hp_visto: return "visto";
  case hp_sconosciuto: return "";
  case hp_preambolo: return "preambolo";
  case hp_datapubbl: return "nota";
  case hp_numpubbl: return "nota";
  case hp_sopubbl: return "nota";
  case hp_legislatura: return "legislatura";
  case hp_relazione: return "relazione";
  case hp_div: return "h:div";
  case hp_nothing: return "nothing";
  default: return "";
  }
}

//Cerca di individuare il tipo di documento in base all'intestazione
//int HeaderParser::parseHeaderGetTipo(std::string& strbuffer, int notes)
void HeaderParser::parseHeaderGetTipo(std::string& strbuffer)
{
	SqueezeWords(strbuffer);
	const char * buffer = strbuffer.c_str();
	//printf("\nHeaderParserUNKNOWN\nbuffer: %s\n\n\n", buffer);
	
	vector<int> sequence, offsets;
	if(!header_extractor.buildExample(sequence, offsets, buffer, strlen(buffer), 0))
		//return notes;
		return;
	
	string tipodoc = "";
	tipodoc = find_type(strbuffer, header_ddl_model, header_ddl_tags, sequence, offsets);
	if(tipodoc == "") {
		tipodoc = find_type(strbuffer, header_cnr_model, header_cnr_tags, sequence, offsets);
		if(tipodoc == "") 
			tipodoc = find_type(strbuffer, header_intestazione_model, header_intestazione_tags, sequence, offsets);
	}

	//manda il tipo di doc. allo stdout  <-- No, deve essere scritto su un file...
	FILE * fo = NULL;
	
	if (!(fo = fopen("temp/unknown_type.tmp", "w"))) //Nome file da concordare con xmLegesEditor
	{
		fprintf(stderr, "Errore apertura file \"temp/unknown_type.tmp\" \n");
		//return notes;
		return;
	}
	fprintf(fo, "%s", tipodoc.c_str());
	fclose(fo); 
	//return notes;
}

//Analizza la sequenza con un modello particolare e restituisci il contenuto del tag 'tipoDoc'
string HeaderParser::find_type(string strbuffer,
			HMM &model, hash_map<int,pair<int,int> > &tags,
					vector<int> &sequence, vector<int> &offsets) 
{	
	int * states = NULL, first = 0, currtag = -1, start = 0, end = 0;
	unsigned int trimmed = 0;
	hash_map<int,pair<int,int> >::const_iterator statetag;
	states = new int[sequence.size()];
	string tipodoc = "";

	model.viterbiPath(sequence, states, sequence.size());
	if ((first = getFirstMatchingState(states, sequence.size(), tags)) < sequence.size()){
		for(int k=0; k < sequence.size(); k++) {
			if(states[k] < 1) continue;
			statetag = tags.find(states[k]);
			currtag = (statetag->second).first;
			if(currtag == hp_tipodoc) {
				if(start == 0) {
					start = offsets[k];
					end = offsets[k+1];
					continue;
				}
				end = offsets[k+1];
			}
		}
	}
	delete[] states;
	return trimEnd(strbuffer.substr(start,end-start), &trimmed);
}

string extractURN(string& strbuffer)
{
  int beg = strbuffer.find("(urn:");
  if (beg != string::npos){
    int end = strbuffer.find_first_of(")", beg);
    if (end != string::npos){
      string urn = strbuffer.substr(beg, end - beg + 1);
      strbuffer.erase(beg, end - beg + 1);
      return urn;
    }
  }
  return "";
}



