/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		struttura.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
#include "struttura.h"
#include "disposto.h"
#include "xml.h"

//RICORSIVA
// ------------------------------------------------- Scorre l'albero restituendo il nodo + a destra che è testo
/*
xmlNodePtr GetLastRightTextNode(xmlNodePtr pParentNode)
{
	if (pParentNode==NULL)return NULL;

	xmlNodePtr cur=xmlGetLastChild(pParentNode);
	if (xmlNodeIsText(cur)){
		return cur;
	}
	else {
		return GetLastRightTextNode(cur);
	}
}
*/

//Find last right partition (usually a "corpo" of a "comma"),
//returns his text (avoid "nota" and "ndr" problems)
xmlNodePtr GetLastRightTextNode(xmlNodePtr pParentNode)
{
	if (pParentNode==NULL) return NULL;
	xmlNodePtr cur = pParentNode, child = NULL;

	while(cur!=NULL) {
		child = cur->children; //child not null ?! yes, if there is an error tag at the end...
		if(child==NULL) {
			//printf("\n>>WARNING<< - GetLastRightTextNode() - child is null (tag:%s)\n", (char *)cur->name);
			return NULL;
		}
		if(child->children == NULL) //child is the last right partition
			return getPartitionTextNode(child);
		cur = xmlGetLastChild(cur);
	}
	return NULL;
}

//Scan node "next" list, returns a text node if present
//(improvements: manage multiple text node in "next" list, and eventual NodeList (...+text+entity+...)
xmlNodePtr getPartitionTextNode(xmlNodePtr node) {
	if(node==NULL) return NULL;

	while(node!=NULL) {
		//printf("\n-%s", node->name);
		if(xmlNodeIsText(node))
			return node;
		node=node->next;
	}
	return NULL;
}

//Scan and delete eventual empty "rubrica"
void checkEmptyRubrica(xmlNodePtr node) {
	int rubsize = 1024; //troppo? troppo poco?
	xmlNodePtr rubriche[rubsize];
	int i;

	//init rubriche
	for(i=0;i<rubsize;i++)
		rubriche[i]=NULL;

	//fill rubriche
	totalGetFirstNodebyTagTipo(node, BAD_CAST tagTipoToNome(rubrica), rubriche, rubsize);

	//check rubriche
	for(i=0;(i<rubsize && rubriche[i]!=NULL);i++)
		if(isEmptyRubrica(rubriche[i])) {
			//printf("\nElimino rubrica vuota...\n");
			utilNodeDelete(rubriche[i]);
			rubriche[i]=NULL;
		}
}

//Returns 1 if node is an empty "rubrica"
int isEmptyRubrica(xmlNodePtr node) {
	xmlNodePtr child;
	if(node==NULL || xmlStrcmp(node->name, BAD_CAST tagTipoToNome(rubrica)) ) return 0;
	if(node->children==NULL) return 1; //null children implies empty "rubrica" !?
	child=node->children;
	while(child!=NULL) {
		//non-text children
		if(!xmlNodeIsText(child)) return 0;
		//text children
		if(!isEmptyText((char *)xmlNodeGetContent(child)))
			return 0;
		child=child->next;
	}
	return 1;
}

//Returns 1 if str (trimmed) is an empty string ( something like: str.trim().length() > 0)
//>>>>MIGLIORARE QUESTA FUNZIONE<<<
int isEmptyText(char *str) {
	int i;
	int len = strlen(str); //str must be null-terminated !
	//printf("\n isEmptyText() - len:%d str:%s",len,str);
	for(i=0; i<len; i++)
		if(str[i] > 33 && str[i]<127)
			return 0; //str is not empty if there is a character in that ascii-range (?!)
	return 1;
}

// ------------------------------------------------- crea nodo ndr
xmlNodePtr CreateNdr(xmlChar *mid)
{
	xmlNodePtr mndr= xmlNewNode( NULL, BAD_CAST tagTipoToNome(ndr));
	xmlAttrPtr	newattr;

	newattr = xmlNewProp (mndr,BAD_CAST "num", mid);

	return mndr;
}

//RICORSIVA
//(Scandisce a partire da pParentNode: ricorsiva in profondità (->children), while interno in larghezza (->next) )
// ------------------------------------------------- muove ndr in meta
void MoveNotesInMeta(int IDStartFrom,xmlNodePtr pParentNode,xmlNodePtr predazionale)
{
	if(pParentNode==NULL) return;

	xmlNodePtr cur=pParentNode->children;
	xmlNodePtr tmpnodo=NULL;
	xmlNodePtr mndr=NULL;
	int curID=0;
	while (cur != NULL) {
		if (IsNode(cur,nota))
		{
			xmlChar *mid=xmlGetProp(cur,BAD_CAST (ATTRIB_ID));
			curID=atoi((char *)mid);
			//printf("\nMovesNotes: %s\n", (char *)mid);

			if (curID>=1000)
			{
				curID = curID - 1000 + IDStartFrom;
				mid=BAD_CAST utilConcatena(2,tagTipoToNomeID(nota),utilItoa(curID));
				xmlSetProp(cur, BAD_CAST (ATTRIB_ID),mid); //il primo id,"n1" non viene assegnato qui
			}

			mndr=CreateNdr(BAD_CAST mid);

			//sostituita:
			//tmpnodo=xmlReplaceNode(cur,mndr);//tmpnodo contiene la vecchia nota
			tmpnodo = replaceNodeWithChildren(cur,mndr,cur->prev);

			//aggancia la NOTA al REDAZIONALE
			xmlAddChild(predazionale,tmpnodo);

			//Ma si dovrebbe tenere conto del fatto che la nota può essere in un annesso!?
			//L'id di ndr dovrebbe essere qualcosa tipo "ann1-n1" e non soltanto "n1"
			//Qui sia l'id di nota che quello di ndr sono del tipo "n1".
			//Quando viene aggiunto "ann1-" ? <-- vedi domAttributeIDUpdate() che viene richiamata
			//alla fine da parser.c
			cur=mndr;
		}
		/*
		else {
			//MoveNotesInMeta(cur);
		}
		*/
		MoveNotesInMeta(IDStartFrom,cur,predazionale);
		cur=cur->next;
	}
}

//RICORSIVA
// ----------------------------- se il fratello di un CORPO è una LETTERA/NUMERO->rinomina il CORPO in ALINEA
void Corpo2Alinea(xmlNodePtr pParentNode) {

	//Passo base
	if (pParentNode==NULL)return;

	xmlNodePtr cur=pParentNode->children; //FirstChild
	xmlNodePtr PrevNodoCorpo=NULL;

	while (cur != NULL) {

		if ( (IsNode(cur,lettera)) || (IsNode(cur,numero)) || (IsNode(cur,puntata)) )	{
			//Se il nodo corrente è una LETTERA/NUMERO
			if (PrevNodoCorpo!=NULL) //Se il nodo precedente è un CORPO
			{
				loggerDebug((char *)utilConcatena(2, "Convertito in alinea:", (char *)PrevNodoCorpo->name));
				xmlNodeSetName(PrevNodoCorpo,BAD_CAST tagTipoToNome(alinea));
			}
		}
		if (IsNode(cur,corpo))	{
			//Se il nodo corrente è un CORPO
			PrevNodoCorpo=cur;

		} else if(!IsNode(cur,tagerrore)) //ignora eventuali nodi errore
			PrevNodoCorpo=NULL;

		Corpo2Alinea(cur);
		cur = cur->next; //next fratello
	}
}

// ==========================================================================
// Analizza la struttura del blocco di testo: doc. principale o allegato
//	note: 1) se dtd base, ruolo non è allegato; 2) buffer non è vuoto
// ==========================================================================

xmlNodePtr StrutturaAnalizza (char *buffer, ruoloDoc ruolo)
{
	char* disposto=NULL; char* prima=NULL; char* ntext=NULL;
	int mnotes=0;
	xmlNodePtr nmeta=NULL, ndescrittori=NULL, nreda=NULL, nintestazione=NULL, nformulainiziale=NULL, narticolato=NULL, narticolo=NULL,
		   ncontenitore=NULL, nformulafinale=NULL, nconclusione=NULL;
	xmlNodePtr mNodoTipoDocumento=NULL, mNodoArticolato=NULL, mFirstError=NULL, mFirstErrorText= NULL;
	xmlNodePtr nnodo= NULL;

	loggerInfo("INIZIO Struttura");

	//Creazione del Nodo TIPODOCUMENTO
	mNodoTipoDocumento=xmlNewNode(NULL, BAD_CAST tagTipoToNome(_TipoDocumento));

	if (mNodoTipoDocumento)		// imposto nodo tipo documento e nome (se documentoNIR)
	{
		if (ruolo == principale)	// doc. principale
		{
			xmlNodeSetName(mNodoTipoDocumento, BAD_CAST configGetDocTestoString());

			if ((configGetDocTestoTipo() == documentoNIR || configGetDocTestoTipo() == provCNR)
						&& configGetDTDTipo() != base)
				xmlNewProp(mNodoTipoDocumento, BAD_CAST "nome", BAD_CAST configGetDocNome());
		}
		else {			// allegati
			xmlNodeSetName(mNodoTipoDocumento, BAD_CAST "DocumentoNIR");
			xmlNewProp(mNodoTipoDocumento, BAD_CAST "nome", BAD_CAST configGetDocNome()); //DTD 2.1
		}
	}

	//Aggiunta -- Si deve differenziare doc.generici, disegni di legge, prov.cnr, ecc...
	int tdoc = 0; //variabile che tiene conto del tipo di documento
	if(configGetDocTestoTipo() == disegnolegge && ruolo == principale) //<- doc.non valido se DDL+DecretoLegge
		tdoc=1;
	if(configGetDocTestoTipo() == provCNR && ruolo == principale) //<- doc.non valido se DDL+DecretoLegge
		tdoc=2;
	if(!strcmp(configGetDocNome(),"Regolamento Regionale"))
		tdoc=3;
	if(!strcmp(configGetDocNome(),"Delibera Consiliare"))
		tdoc=4;
	if(!strcmp(configGetDocNome(),"Regolamento Comunale"))
		tdoc=5;

	// inserisco nodi di testa
	if(configTipoInput() == xml && ruolo == principale) {
		xmlAdjustHeader(mNodoTipoDocumento);
		nmeta = xmlGetMeta();
		ndescrittori = xmlGetDescrittori();
		nintestazione = xmlGetIntestazione();
	} else {
		nmeta = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "meta", NULL);
		ndescrittori = xmlNewChild(nmeta, NULL, BAD_CAST "descrittori", NULL);
		nintestazione = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "intestazione", NULL);
	}

	if(tdoc!=1) {
		if(configTipoInput() == xml && ruolo == principale) {
			nformulainiziale = xmlGetFormulaIniziale();
		} else {
			nformulainiziale = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "formulainiziale", NULL);
		}
	}

	mNodoArticolato = ArticolatoAnalizza(buffer); 	//Analisi di un eventuale ARTICOLATO

	if (mNodoArticolato) /* ------------------------------------------------------------------------------------------------ trovato l'articolato*/
	{
		xmlAddChild(mNodoTipoDocumento,mNodoArticolato);	//<-- Aggancia il nodo dell'ARTICOLATO
		// inserisco nodi di coda
		//(se il doc. non è un disegno di legge...)
		if(tdoc!=1) {
			nformulafinale = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "formulafinale", NULL);
			nconclusione = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "conclusione", NULL);
		}

		//Viene individuato il primo ERRORE presente nel nodo ARTICOLATO e viene passato all'HeaderParser
		mFirstError=GetFirstNodebyTagTipo(mNodoArticolato, BAD_CAST tagTipoToNome(tagerrore));

		mFirstErrorText=GetFirstTextNode(mFirstError);

		mnotes=testa(mFirstErrorText,mNodoTipoDocumento,nmeta,ndescrittori,nintestazione,nformulainiziale,tdoc);

		//Sgancia il nodo TAGERRORE e lo libera
		utilNodeDelete(mFirstError);
		mFirstErrorText=NULL;
		mFirstError=NULL;

		//Rimuovi eventuali rubriche vuote
		checkEmptyRubrica(mNodoArticolato);

		//NOTE
		nreda=GetFirstNodebyTagTipo(nmeta, BAD_CAST "redazionale");

		//Possibile problema: documento con allegato, note alla fine del doc. e non c'è <redazionale> (nell'allegato).
		//nreda in quel caso è null, non viene fatto MovesNotesInMeta() e quindi rimangono dei <nota>,
		//xmLeges crasha (riferimento a nodo inesistente) -- Adesso è OK (ricerca di note in tutto il sotto-albero)
		if (nreda) {
			MoveNotesInMeta(mnotes, mNodoArticolato, nreda);
		} else {
			//printf("\nMissing node: \"redazionale\": checking if there are notes...");
			//Aggiungi il nodo redazionale solo se sono presenti delle note
			nnodo=totalGetFirstNodebyTagTipo(mNodoArticolato, BAD_CAST "nota", NULL, 0); // <--controlla il sotto-albero
			if(nnodo!=NULL) {
				//printf(" found! Adding \"redazionale\"...\n");
				nreda = xmlNewChild(nmeta, NULL, BAD_CAST "redazionale", NULL);
				MoveNotesInMeta(mnotes, mNodoArticolato, nreda);
			} else {
				//printf(" not found!\n");
			}
		}

		//Viene individuato il nodo TESTO (ultimo nodo) + a destra nell'albero
		//PROBLEMA: Se sono presenti delle note alla fine del testo, mFirstErrorText non è ciò che ci si aspetta.
		//--> correggere la funzione GetLastRightTextNode()
		mFirstErrorText=GetLastRightTextNode(mNodoArticolato);
		mnotes=coda(mnotes,mNodoTipoDocumento,mFirstErrorText,nmeta,ndescrittori,nformulafinale,nconclusione,tdoc);

	}
	else 			/* ---------------------------------------------------------------------------------------- NON trovato l'articolato */
	{

		//printf("\n------ BUFFER1:%s\n",(char *)buffer);
		if (ruolo == principale)
			buffer = utilConvertiText(buffer);		// converto in UTF-8
		unsigned int nnl = contaChar(buffer, '\n');
		nnl = strlen(buffer) + 50 + 12 * nnl;
		disposto = (char *) malloc(sizeof(char) * nnl);
		*disposto=0;
		prima = (char *) malloc(sizeof(char) * nnl);
		*prima=0;

		//Sostituisci eventuali caratteri > e < tramite sstring()
		dispostoAnalizza(ruolo, sstring(buffer), &disposto, &prima, nnl);	// cerco disposto e prima del disposto

		nnl = strlen(prima);
		if (disposto && strlen(disposto) > nnl)
			nnl = strlen(disposto);
		ntext = (char*) malloc(sizeof(char) * (nnl + 100));

		if (ruolo == principale && configGetDocStruttura() == docarticolato)	// minimo x articolato
		{
			narticolato = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "articolato", NULL);
			InsertXmlFile(configArticolatoVuoto(), narticolato);
			narticolo = GetFirstNodebyTagTipo(narticolato, BAD_CAST tagTipoToNome(articolo)); // punto articolo
		}
		//Aggiunto questo if per non inserire tag vuoti e se poi si richiama comunque testa()
		if(!disposto)
			InsertXmlFile(configDescrittoriVuoto(), ndescrittori);	// togliere se si chiama la funzione testa
		//------------------------------------------------------------------------------------- PRIMA
		if (disposto)
		{
			sostStr(prima, "<h:p>","");	// tolgo tag paragrafi
			sostStr(prima, "</h:p>","");
			nnodo = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST tagTipoToNome(tagerrore), NULL);	// prima in error
			xmlAddChild(nnodo,xmlNewText(BAD_CAST prima));
			mnotes=testa(nnodo,mNodoTipoDocumento,nmeta,ndescrittori,nintestazione,nformulainiziale,tdoc);
			utilNodeDelete(nnodo);
			// coda?
		}
		else if (ruolo == principale && configGetDocStruttura() == docarticolato)		// prima in error
		{
			nnodo = xmlNewChild(narticolo, NULL, BAD_CAST tagTipoToNome(tagerrore), NULL);
			xmlAddChild(nnodo,xmlNewText(BAD_CAST "===== IL TESTO DEL PROVVEDIMENTO NON E' ARTICOLATO =====\n\n"));
			sostStr(prima, "<h:p>","");	// tolgo tag paragrafi
			sostStr(prima, "</h:p>","");
			xmlAddChild(nnodo,xmlNewText(BAD_CAST prima));
			//mnotes=testa(nnodo,mNodoTipoDocumento,nmeta,ndescrittori,nintestazione,nformulainiziale);
			// coda?
		}
		else	// prima in contenitore
		{
			ncontenitore = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "contenitore", NULL);
			xmlNewProp(ncontenitore, BAD_CAST "nome", BAD_CAST ""); //DTD 2.1
			xmlNewProp(ncontenitore, BAD_CAST "id", BAD_CAST ""); //DTD 2.1
			strcpy(ntext,"<atto xmlns:h=\"html\">");
			strcat(ntext,prima);
			strcat(ntext,"</atto>");
			InsertXmlFile(ntext, ncontenitore);
		}
		//------------------------------------------------------------------------------------- DISPOSTO
		if (disposto)
		{
			if (ruolo == principale && configGetDocStruttura() == docarticolato)
			{
				// $$$$$ dopo spostare dopo il primo articolo $$$$$
				nnodo = xmlNewChild(nformulainiziale, NULL, BAD_CAST tagTipoToNome(tagerrore), NULL);	// disposto in error
				xmlAddChild(nnodo,xmlNewText(BAD_CAST "===== IL DISPOSTO DEL PROVVEDIMENTO NON E' ARTICOLATO =====\n\n"));
				sostStr(disposto, "<h:p>","");	// tolgo tag paragrafi
				sostStr(disposto, "</h:p>","");
				xmlAddChild(nnodo,xmlNewText(BAD_CAST disposto));
				// testa e coda?
			}
			else
			{
				ncontenitore = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "contenitore", NULL);
				xmlNewProp(ncontenitore, BAD_CAST "nome", BAD_CAST ""); //DTD 2.1
				xmlNewProp(ncontenitore, BAD_CAST "id", BAD_CAST ""); //DTD 2.1
				strcpy(ntext,"<atto xmlns:h=\"html\">");
				strcat(ntext,disposto);
				strcat(ntext,"</atto>");
				InsertXmlFile(ntext, ncontenitore);
			}
		}

		if (configGetDTDTipo() != base)
		{
			nformulafinale = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "formulafinale", NULL);
			nconclusione = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "conclusione", NULL);
		}
	}
	free(ntext);
	if (configGetDTDTipo() == base)				// elimino o metto nodi o attributi previsti
	{
		nnodo = GetFirstNodebyTagTipo(ndescrittori, BAD_CAST "vigenza");  // descrittori  //VIGENZA NON ESISTE PIU' !!
		if(nnodo!=NULL)
			xmlSetProp (nnodo, BAD_CAST "inizio", BAD_CAST "");

		//dtd base: formulainiziale deve avere almeno un blocco tra i figli
		if(nformulainiziale!=NULL && nformulainiziale->children==NULL)
			xmlNewChild(nformulainiziale, NULL, BAD_CAST tagTipoToNome(h_p), NULL);

		if (configGetDocStruttura() == docarticolato)
		{
			nnodo = xmlNewChild(nformulafinale, NULL, BAD_CAST tagTipoToNome(h_p), NULL);	// formula finale
			nnodo = GetFirstNodebyTagTipo(nconclusione, BAD_CAST "dataeluogo");		// dataeluogo
			xmlUnsetProp(nnodo, BAD_CAST "codice");
		}
		else
		{
			if (!narticolato)
				narticolato=mNodoArticolato;

			//Trattare il testo come una lista testo/entità:
			//xmlChar* contNodo = xmlNodeGetContent(nformulainiziale);// formula iniziale
			xmlChar* contNodo = xmlNodeListGetString(NULL, nformulainiziale, 0);
			//Con la precedente riga si perdono le entità per avere un unico nodo di testo
			//(può dare problemi in fase di visualizzazione...)


			if (contNodo) {
				nnodo = xmlNewChild(nintestazione, NULL, BAD_CAST tagTipoToNome(tagerrore), NULL);
				xmlAddChild(nnodo, xmlNewText(BAD_CAST "===== DTD BASE: FORMULA INIZIALE NON AMMESSA =====\n\n"));
				xmlAddChild(nnodo, xmlNewText(contNodo));
			}
			utilNodeDelete(nformulainiziale);

			//Trattare il testo come una lista testo/entità:
			//contNodo = xmlNodeGetContent(nformulafinale);	// formula finale
			contNodo = xmlNodeListGetString(NULL, nformulafinale, 0);
			//Con la precedente riga si perdono le entità per avere un unico nodo di testo
			//(può dare problemi in fase di visualizzazione...)

			if (contNodo) {
				nnodo = xmlNewChild(narticolato, NULL, BAD_CAST tagTipoToNome(tagerrore), NULL);
				xmlAddChild(nnodo, xmlNewText(BAD_CAST "===== DTD BASE: CORPO ALLEGATO NON AMMESSO =====\n\n"));
				xmlAddChild(nnodo, xmlNewText(contNodo));
			}
			utilNodeDelete(nformulafinale);

			//Trattare il testo come una lista testo/entità:
			//contNodo = xmlNodeGetContent(nconclusione);	// conclusione
			contNodo = xmlNodeListGetString(NULL, nconclusione, 0);
			//Con la precedente riga si perdono le entità per avere un unico nodo di testo
			//(può dare problemi in fase di visualizzazione...)

			if (contNodo) {
				nnodo = xmlNewChild(narticolato, NULL, BAD_CAST tagTipoToNome(tagerrore), NULL);
				xmlAddChild(nnodo, xmlNewText(BAD_CAST "===== DTD BASE: CONCLUSIONE NON AMMESSA =====\n\n"));
				xmlAddChild(nnodo,xmlNewText(BAD_CAST contNodo));
			}
			utilNodeDelete(nconclusione);
		}
	}

//		xmlNodePtr nodoContenitore = TabelleAnalizza(buffer);
//		if (nodoContenitore)
//		{
//			InsertXmlFile(configGetTestaDefault(2), mNodoTipoDocumento);
//			xmlAddChild(mNodoTipoDocumento,nodoContenitore);
//		}
//		else	boh!

	// tagControlloArticoloInizioEl(); //OLD
	loggerInfo("FINE Struttura");

	loggerInfo("INIZIO VIRGOLETTE");
	virgoletteInMod(mNodoArticolato);
	loggerInfo("FINE VIRGOLETTE");


	loggerInfo("INIZIO TAG ALINEE");
	Corpo2Alinea(mNodoTipoDocumento);
	loggerInfo("FINE TAG ALINEE");

	utilPercCalc(2);

	return mNodoTipoDocumento;
}
