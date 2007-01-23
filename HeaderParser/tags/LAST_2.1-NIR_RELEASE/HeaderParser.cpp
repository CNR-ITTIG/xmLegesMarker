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

//printf("\nHeaderParser\nbuffer: %s\n\n", header.c_str());
//printf("\nHeaderParser\ntdoc: %d\n", tdoc);

//Disegno di legge
if(tdoc == 1) {
	if(sequence.size() > 0){
		found = true;
		//Aggiungi tag specifici
		xmlNodePtr nApprovazione = xmlNewChild(descrittori, NULL, BAD_CAST "approvazione", NULL);
		xmlNodePtr nRedazione = xmlNewChild(descrittori, NULL, BAD_CAST "redazione", NULL);
		xmlNodePtr nUrn = xmlNewChild(descrittori, NULL, BAD_CAST "urn", NULL);
		xmlNewProp(nApprovazione, BAD_CAST "internal_id", BAD_CAST "");
		xmlNewProp(nApprovazione, BAD_CAST "leg", BAD_CAST "");
		xmlNewProp(nApprovazione, BAD_CAST "norm", BAD_CAST "");	
		xmlNewProp(nApprovazione, BAD_CAST "tipodoc", BAD_CAST "");
		xmlNewProp(nRedazione, BAD_CAST "nome", BAD_CAST "");
		xmlNewProp(nRedazione, BAD_CAST "norm", BAD_CAST "");
		//xmlNewProp(redazione, BAD_CAST "url", BAD_CAST ""); //non obbligatorio nella dtd
		
		states = new int[sequence.size()];
		header_intestazione_model.viterbiPath(sequence, states, sequence.size());
		if ((first = getFirstMatchingState(states, sequence.size(), header_intestazione_tags)) < sequence.size()){
	    	found = true;
			//for(int kk=0; kk < sequence.size(); kk++)
				//printf("\n %d: sequence[]=%d   states[]=%d", kk, sequence[kk], states[kk]);
			
	        last = saveTags(strbuffer, states, sequence.size(), offsets, offset, last, intestazione, header_intestazione_tags, tdoc);      
	        removeProcessedElements(sequence, last);
	        offset += last + 1;
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
	xmlNodePtr errorNode = intestazione; //per inserire il testo non rilevato nella giusta posizione
	if(sequence.size() > 0) {
		found = true;
		//Aggiungi tag 'proprietario' vuoti
		xmlNodePtr proprietario = xmlNewChild(meta, NULL, BAD_CAST "proprietario", BAD_CAST "");
		xmlNewProp(proprietario, BAD_CAST "xlink:type", BAD_CAST "simple");
		xmlNewProp(proprietario, BAD_CAST "xmlns:cnr", BAD_CAST "http://www.cnr.it/provvedimenti/1.0");
		
		xmlNodePtr cnrmeta = xmlNewChild(proprietario, NULL, BAD_CAST "cnr:meta", BAD_CAST "");
		xmlNodePtr proptmp = xmlNewChild(cnrmeta, NULL, BAD_CAST "cnr:strutturaEmanante", BAD_CAST "");
		xmlAddChild(proptmp, xmlNewText(BAD_CAST ""));
		proptmp = xmlNewChild(cnrmeta, NULL, BAD_CAST "cnr:autoritaEmanante", BAD_CAST "");
		xmlAddChild(proptmp, xmlNewText(BAD_CAST ""));
		proptmp = xmlNewChild(cnrmeta, NULL, BAD_CAST "cnr:tipoDestinatario", BAD_CAST "");
		xmlAddChild(proptmp, xmlNewText(BAD_CAST ""));
		proptmp = xmlNewChild(cnrmeta, NULL, BAD_CAST "cnr:disciplina", BAD_CAST "");
		xmlAddChild(proptmp, xmlNewText(BAD_CAST ""));
		//disciplina e areaScientifica sono in OR nella DTD.
		//proptmp = xmlNewChild(cnrmeta, NULL, BAD_CAST "cnr:areaScientifica", BAD_CAST "");
		//xmlAddChild(proptmp, xmlNewText(BAD_CAST ""));
		
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
		xmlAddChild(nUrn, xmlNewText(BAD_CAST "urn:nir:consiglio.nazionale.ricerche:provvedimento:"));

		states = new int[sequence.size()];
		header_intestazione_model.viterbiPath(sequence, states, sequence.size());
		if ((first = getFirstMatchingState(states, sequence.size(), header_intestazione_tags)) < sequence.size()){
	        //Aggiungere emanante (valore costante per questo tipo di doc):
	        xmlNodePtr emanante = xmlNewChild(intestazione, NULL, BAD_CAST "emanante", BAD_CAST "");
			xmlAddChild(emanante, xmlNewText(BAD_CAST "Consiglio Nazionale delle Ricerche"));

		  //Se provv.CNR tralasciare l'inizio di formula iniziale...
		  //(vedi parser_config per sapere lo stato in cui inizia la formula iniziale)
		  int klast = 0;
	      for(int kk=0; kk < sequence.size(); kk++) {
			//printf("\n %d: sequence[]=%d   states[]=%d", kk, sequence[kk], states[kk]);
			if(tdoc == 2 && states[kk] == 53) { //<-- 'IL' presidente/dirigente/ecc...
				klast = kk;
				errorNode = formulainiziale;
			}
	      }
     		//salva i tag riferiti a 'intestazione'		
	        last = saveTags(strbuffer, states, klast, offsets, offset, last, intestazione, header_intestazione_tags, tdoc);              
	        //salva i tag di 'formulainiziale'
	        last = saveTags(strbuffer, states, sequence.size(), offsets, 0, klast, formulainiziale, header_intestazione_tags, tdoc);              
	        removeProcessedElements(sequence, last);
	        offset = last+1;
		}
		delete[] states;
	}
	if(sequence.size() > 0 || found == false) { //Se non è stato rilevata l'intestazione viene messo tutto in error:
		//printf("\n HeaderParser -- Testo non rilevato (error tag) -- offset:%d size:%d\n",offset,offsets.size());
		// se rimane qualcosa mettilo in un tag errore
    	if (offset < offsets.size())
      		saveTag(hp_sconosciuto, offsets[offset], strbuffer.length(), strbuffer, errorNode, tdoc); 
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
		if(pub_found)
	 	 last = saveTags(strbuffer, pub_states, pub_sequence.size(), offsets, offset, last, meta ,header_pubblicazione_tags, tdoc, &notes, NULL, intestazione);     
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
  if(urn != "")
    xmlNewChild(descrittori, NULL, BAD_CAST "urn", BAD_CAST urn.c_str());
  
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
  xmlNodePtr urn = xmlNewChild(descrittori, NULL, BAD_CAST "urn", BAD_CAST "");
  xmlNodePtr vigenza = xmlNewChild(descrittori, NULL, BAD_CAST "vigenza", NULL);
  xmlNewProp(vigenza, BAD_CAST "id", BAD_CAST "v1");
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
		tmpnode = xmlNewChild(intestazione, NULL, BAD_CAST "emanante", BAD_CAST "");
		xmlAddChild(tmpnode, xmlNewText(BAD_CAST ""));
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
  if((statesnumber - last) > first){
    xmlNodePtr titlenode = saveTag(hp_titolodoc,offsets[offset+last+1], offsets[offset+statesnumber], strbuffer, intestazione, 0);     
    saveTags(strbuffer, states, statesnumber, offsets, offset, state, meta, tags, 0, id, NULL, titlenode);    
    
    return statesnumber-1;
  }
  else{
    xmlNodePtr titlenode = saveTag(hp_titolodoc,offsets[offset], offsets[offset+first], strbuffer, intestazione, 0); 
    return saveTags(strbuffer, states, statesnumber, offsets, offset, first, meta, tags, 0, id, titlenode, NULL);    
  }
}

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
  xmlChar* content = xmlNodeGetContent(lastcomma);
  //printf("\nFooter tdoc:%d\n",tdoc);
  if(content == NULL){
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
  
  //Last Comma Init: (inutile non cambia niente rimane il <mod> )
  //xmlNodeSetContent(lastcomma, BAD_CAST strbuffer.c_str());
  //defaultFooter(strbuffer, lastcomma);

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
    
   //for(int kk=0; kk < sequence.size(); kk++)
	//	printf("\n %d: sequence[]=%d   states[]=%d", kk, sequence[kk], states[kk]);
    
    if (hasCorrectStates(states, sequence.size())){
      if (!found){
      	//printf("\nformulafinale\n");
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
  
  // parse annessi
  if(sequence.size() > 0){
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

  if(!found)
  	//printf("\nNon è stato rilevato niente");
  
  // if nothing found print default footer
  if(!found)
    defaultFooter(strbuffer, lastcomma);	
  else{
    // put remains (annessi) in error tag
    if (offset < offsets.size())
      saveTag(hp_sconosciuto, offsets[offset], strbuffer.length(), strbuffer, root_node, tdoc); 
  }
  
  // save URN
  if(urn != "")
    addChildIfMissing("urn", NULL, descrittori, urn.c_str());

  // add compulsory meta

  //Aggiunta -- addMissingMeta() solo nel caso generico...
  if(tdoc==0)
	    addMissingMeta(descrittori);

  addMissingFooter(meta, descrittori, formulafinale, conclusione, tdoc);
  
  //Last Comma Init:
  //xmlNodeSetContent(lastcomma, BAD_CAST strbuffer.c_str());
    
  //printf("\nFineFooter\n");
    
  return notes;
}

//Cerca il carattere '.' e mette in lastcomma tutto ciò che è alla 
//sinistra del carattere (il resto va in error, nel footer).
void  HeaderParser::defaultFooter(std::string footer, xmlNodePtr lastcomma) const
{
	//printf("\nDefault footer\n");
	
  unsigned int dot = footer.find('.');
  if(dot != string::npos){
    xmlNodeSetContent(lastcomma, BAD_CAST footer.substr(0, dot+1).c_str());
    footer = footer.substr(dot+1);
  }
  if (footer.find_first_not_of(" \n\t\r") != string::npos)
    saveTag(hp_sconosciuto, 0, footer.length(), footer, root_node, 0, NULL, NULL, false, NULL);
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
  xmlNodePtr urn = addChildIfMissing("urn", &added, descrittori); 
  xmlNodePtr vigenza = addChildIfMissing("vigenza", &added, descrittori, NULL); 
  if(added)
    xmlNewProp(vigenza, BAD_CAST "id", BAD_CAST "v1");
}

//TRUE se la sequenza non è completamente composta di stati -1 e 0
bool HeaderParser::hasCorrectStates(int * states, int statesnumber)
{
  for(int i = 0; i < statesnumber; i++)
    if(states[i] > 0)
      return true;
  return false;
}

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

  int last = 0;
  int * pub_states = new int[first];
  vector<int> pub_sequence;
  copyElements(sequence, pub_sequence, 0, first-1);
  header_pubblicazione_model.viterbiPath(pub_sequence, pub_states, pub_sequence.size());
  savePubblicazione(strbuffer, pub_states, pub_sequence.size(), offsets, offset, findChild("descrittori", meta), header_pubblicazione_tags);
  if(hasCorrectStates(pub_states, pub_sequence.size()))
    last = saveTags(strbuffer, pub_states, pub_sequence.size(), offsets, offset, 0, meta, header_pubblicazione_tags, 0, notes, curr_node);     
  if(last < first-1)
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
  if(state==0) {
  	//Aggiunto:
  	//in questo caso viene rilevata una sequenza nel footer in uno dei modelli ma tale
  	//sequenza inizia con l'inizio del buffer. In questo caso o si duplica il contenuto
  	//del buffer (va sia nell'ultimo comma con tag <mod> (?) che nel footer) oppure
  	//si lascia l'ultimo comma vuoto (l'errore è commesso dal modello del footer...):
  	xmlNodeSetContent(lastcomma, BAD_CAST "");
  	//Invece nel caso non venga rilevato niente, cioè nessuna sequenza con nessun modello
  	//del footer, e dunque non viene mai richiamata la saveLastComma(), allora si
  	//richiama defaultFooter() alla fine di parseFooter().
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
    xmlNodeSetContent(lastcomma, BAD_CAST strbuffer.substr(offsets[offset], offsets[offset+state]-offsets[offset]).c_str());
    //printf("\nSave Here\n");
  }
  else{
    xmlNodeSetContent(lastcomma, BAD_CAST strbuffer.substr(offsets[offset], offsets[offset+first]-offsets[offset]).c_str());
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
    return NULL;
  }
  //AGGIUNTO: non scrive niente (salta la parola). 
  if(nothingTag(tagvalue)) 
  	return NULL;
  //
  xmlNodePtr currnode = (withtags) ? openTag(tagvalue, startnode, buffer.substr(start,end-start), id) : startnode;
  
  //Aggiunta:
  if(tagvalue == hp_relazione) {
	addFormatTagsDiv(buffer.substr(start,end-start), currnode);
	xmlAddNextSibling(startnode,currnode);
	return currnode;
  }
  
  if(formatTag(tagvalue)){
    if(tagvalue == hp_preambolo)
      if(tdoc==2) //Se provvedimento CNR considera solo i ritorni a capo e non i ';'
        addFormatTags(buffer.substr(start,end-start), currnode);
      else
        addSemicolumnFormatTags(buffer.substr(start,end-start), currnode);
    else
      addFormatTags(buffer.substr(start,end-start), currnode);
  }
  else if(tagvalue == hp_formulafinale)
    xmlNewChild(startnode, NULL, BAD_CAST "h:p", BAD_CAST buffer.substr(start,buffer.find_last_not_of(" \r\n\t", end-1)-start+1).c_str());
  else if( trimmedTag(tagvalue) && withtags ||
  			(tdoc > 0 && trimmedTagDDL(tagvalue)) ){
    unsigned int trimmed = 0;
    xmlAddChild(currnode, xmlNewText(BAD_CAST trimEnd(buffer.substr(start,end-start), &trimmed).c_str()));
    if (trimmed < end-start) {
    	if(tdoc==1) {
    		//Con i DDL metti in error tutto cio' che non viene riconosciuto
    		//(e che ha lunghezza 'trimmata' maggiore di 0...)
    		int isblank = buffer.substr(start+trimmed,end-start-trimmed).find_last_not_of(" \n\t\r");
    		if( isblank > 0 ) {
	    	  	xmlNodePtr errnode = xmlNewPI(BAD_CAST "error", BAD_CAST buffer.substr(start+trimmed,end-start-trimmed).c_str());
	    	  	xmlAddChild(startnode, errnode);
    		}
    	} else
    	    //Ma se quello che rimane è tutta roba " \n\t\r" non la riscrivere neanche...
    	    if( buffer.substr(start+trimmed,end-start-trimmed).find_last_not_of(" \n\t\r") != string::npos)
	   			xmlAddChild(startnode, xmlNewText(BAD_CAST buffer.substr(start+trimmed,end-start-trimmed).c_str()));
    }
  }
  else
    xmlAddChild(currnode, xmlNewText(BAD_CAST buffer.substr(start,end-start).c_str()));
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

void HeaderParser::addSemicolumnFormatTags(string text, xmlNodePtr startnode) const
{
  istringstream in(text);
  string line, buf;
  unsigned int end = 0;

  while(Lexer::getLine(in, line)){
    unsigned int semicolumn = line.find(";");
    if(semicolumn != string::npos){
      xmlNewChild(startnode, NULL, BAD_CAST "h:p", BAD_CAST (buf + line.substr(0,semicolumn+1)).c_str());
      if(semicolumn < line.length()-1)
	buf = line.substr(semicolumn+1);
      else
	buf = "";
      continue;
    }
    if (line.find_first_not_of(" \n\t\r") != string::npos)
      buf += line + "\n";
  }
  if ((end = buf.find_last_not_of(" \n\t\r")) != string::npos)
    xmlNewChild(startnode, NULL, BAD_CAST "h:p", BAD_CAST buf.substr(0,end+1).c_str());
}

//Modificata: non considerare righe composte solo da "-" e togli eventuali "-" ad inizio riga
void HeaderParser::addFormatTags(string buf, xmlNodePtr startnode) const
{
  istringstream in(buf);
  string line, out;

  while(Lexer::getLine(in, line))
    if (line.find_first_not_of(" -\n\t\r") != string::npos) {
      int ststr = line.find_first_not_of(" -\n\t\r");
      xmlNewChild(startnode, NULL, BAD_CAST "h:p", BAD_CAST line.substr(ststr,line.size()-ststr+1).c_str());
    }
}

//Aggiunta:
void HeaderParser::addFormatTagsDiv(string buf, xmlNodePtr startnode) const
{
  istringstream in(buf);
  string line, out;

  while(Lexer::getLine(in, line))
    if (line.find_first_not_of(" -\n\t\r") != string::npos)
      xmlNewChild(startnode, NULL, BAD_CAST "h:div", BAD_CAST line.c_str());
}

string normalizeDate(const string& buffer)
{
	//Aggiunta (data del tipo xx/yy/zz )
	//if(buffer.find_first_not_of("0123456789/.- \n\r\t") == string::npos) {
	if(buffer.find_first_of("gfmalsond") == string::npos) {
		//int tmpfind = buffer.find_first_not_of("0123456789/.- \n\r\t");
		int tmpfind = buffer.find_first_of("gfmalsond");
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
  case hp_titolodoc:
  case hp_emanante:
  case hp_relazione: return true;
  default: return false;
  }
}

bool HeaderParser::noteTag(int tagvalue) const
{
  switch(HP_tagTipo(tagvalue)){
  case hp_pubblicazione: 
  case hp_datapubbl:
  case hp_numpubbl:
  case hp_sopubbl:
  case hp_nota:
  case hp_registrazione:
  case hp_lavoripreparatori: return true;
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

string  extractURN(string& strbuffer)
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



