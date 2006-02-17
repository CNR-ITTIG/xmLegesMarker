#include "struttura.h"
#include "disposto.h"

//RICORSIVA
// ------------------------------------------------- Scorre l'albero restituendo il nodo + a destra che è testo
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

// ------------------------------------------------- crea nodo ndr
xmlNodePtr CreateNdr(xmlChar *mid)
{
	xmlNodePtr mndr= xmlNewNode( NULL, BAD_CAST tagTipoToNome(ndr));
	xmlAttrPtr	newattr;

	newattr = xmlNewProp (mndr,BAD_CAST "num", mid);

	return mndr;
}

//RICORSIVA
// ------------------------------------------------- muove ndr in meta
void MoveNotesInMeta(int IDStartFrom,xmlNodePtr pParentNode,xmlNodePtr predazionale)
{

	if (pParentNode==NULL)return;

	xmlNodePtr cur=pParentNode->children;
	xmlNodePtr tmpnodo=NULL;
	xmlNodePtr mndr=NULL;
	int curID=0;
	while (cur != NULL) {
		if (IsNode(cur,nota))
		{
			xmlChar *mid=xmlGetProp(cur,BAD_CAST (ATTRIB_ID));
			curID=atoi((char *)mid);

			if (curID>=1000)
			{
				curID= curID - 1000 + IDStartFrom;
				mid=BAD_CAST utilConcatena(2,tagTipoToNomeID(nota),utilItoa(curID));
				xmlSetProp	(cur, BAD_CAST (ATTRIB_ID),mid);//il primo id,"n1" non viene assegnato qui
			}
			
			mndr=CreateNdr(BAD_CAST mid);

			tmpnodo=xmlReplaceNode(cur,mndr);//tmpnodo contiene la vecchia nota
			
			
			//aggancia la NOTA al REDAZIONALE
			xmlAddChild(predazionale,tmpnodo);
			cur=mndr;
		}
		else {
			//MoveNotesInMeta(cur);
		}
		MoveNotesInMeta(IDStartFrom,cur,predazionale);
		cur=cur->next;
	}
	
}

//RICORSIVA
// ----------------------------- se il fratello di un CORPO è una LETTERA/NUMERO->rinomina il CORPO in ALINEA
void Corpo2Alinea(xmlNodePtr pParentNode){

	//Passo base
	if (pParentNode==NULL)return;

	xmlNodePtr cur=pParentNode->children; //FirstChild
	xmlNodePtr PrevNodoCorpo;
	
	while (cur != NULL) {

		if ( (IsNode(cur,lettera)) || (IsNode(cur,numero)) )	{
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

		}else PrevNodoCorpo=NULL;
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
	int   mnotes=0;
	xmlNodePtr nmeta=NULL, ndescrittori=NULL, nreda=NULL, nintestazione=NULL, nformulainiziale=NULL, narticolato=NULL, narticolo=NULL,
		   ncontenitore=NULL, nformulafinale=NULL, nconclusione=NULL;
	xmlNodePtr mNodoTipoDocumento=NULL, mNodoArticolato=NULL, mFirstError=NULL, mFirstErrorText= NULL;
	xmlNodePtr nnodo= NULL;
	//xmlNodePtr nApprovazione=NULL, nRedazione=NULL, nUrn=NULL; //Aggiunti per Disegni di Legge
	
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
		else				// allegati
			xmlNodeSetName(mNodoTipoDocumento, BAD_CAST "DocumentoNIR");
	}

	//Aggiunta -- Si deve differenziare doc.generici, disegni di legge, prov.cnr, ecc...
	int tdoc = 0; //variabile che tiene conto del tipo di documento
	if(configGetDocTestoTipo() == disegnolegge)
		tdoc=1;

	// inserisco nodi di testa
	nmeta = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "meta", NULL);
	ndescrittori = xmlNewChild(nmeta, NULL, BAD_CAST "descrittori", NULL);
	nintestazione = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "intestazione", NULL);
	if(tdoc!=1)
		nformulainiziale = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "formulainiziale", NULL);
//	nnodo = xmlNewChild(nformulainiziale, NULL, BAD_CAST tagTipoToNome(h_p), NULL);  	// base: vuole almeno un h:p
	
	mNodoArticolato = ArticolatoAnalizza(buffer); 	//Analisi di un eventuale ARTICOLATO

	if (mNodoArticolato) /* ------------------------------------------------------------------------------------------------ trovato l'articolato*/
	{
		xmlAddChild(mNodoTipoDocumento,mNodoArticolato);	//<-- Aggancia il nodo dell'ARTICOLATO
		// inserisco nodi di coda
		//(se il doc. non è un disegno di lege...)
		if(tdoc!=1) {
			nformulafinale = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "formulafinale", NULL);
			nconclusione = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "conclusione", NULL);
		}

		//Viene individuato il primo ERRORE presente nel nodo ARTICOLATO e viene passato all'HeaderParser
		mFirstError=GetFirstNodebyTagTipo(mNodoArticolato, BAD_CAST tagTipoToNome(tagerrore));
		
		mFirstErrorText=GetFirstTextNode(mFirstError);
		
		mnotes=testa(mFirstErrorText,mNodoTipoDocumento,nmeta,ndescrittori,nintestazione,nformulainiziale);

		//Sgancia il nodo TAGERRORE e lo libera
		utilNodeDelete(mFirstError);
		mFirstErrorText=NULL;
		mFirstError=NULL;

		//Viene individuato il nodo TESTO (ultimo nodo) + a destra nell'albero
		mFirstErrorText=GetLastRightTextNode(mNodoArticolato);
		mnotes=coda(mnotes,mNodoTipoDocumento,mFirstErrorText,nmeta,ndescrittori,nformulafinale,nconclusione);

		nreda=GetFirstNodebyTagTipo(nmeta, BAD_CAST "redazionale");
		if (nreda)
			MoveNotesInMeta(mnotes, mNodoArticolato, nreda);
		else
		{
			//DA FARE
			//se non esistono le meta che inventa???
			//Le note rimangono dove sono e taggate come <nota id="art1-com1-1000">
		}
	}
	else 			/* ---------------------------------------------------------------------------------------- NON trovato l'articolato */
	{
		if (ruolo == principale)
			buffer = utilConvertiText(buffer);		// converto in UTF-8
		int nnl = contaChar(buffer, '\n');
		nnl = strlen(buffer) + 50 + 12 * nnl;
		disposto = (char *) malloc(sizeof(char) * nnl);
		*disposto=0;
		prima = (char *) malloc(sizeof(char) * nnl);
		*prima=0;
		
		dispostoAnalizza(ruolo, buffer, &disposto, &prima, nnl);	// cerco disposto e prima del disposto
		
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
			mnotes=testa(nnodo,mNodoTipoDocumento,nmeta,ndescrittori,nintestazione,nformulainiziale);
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
//			mnotes=testa(nnodo,mNodoTipoDocumento,nmeta,ndescrittori,nintestazione,nformulainiziale);
			// coda?
		}
		else	// prima in contenitore
		{
			ncontenitore = xmlNewChild(mNodoTipoDocumento, NULL, BAD_CAST "contenitore", NULL);
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
		nnodo = GetFirstNodebyTagTipo(ndescrittori, BAD_CAST "vigenza");			// descrittori
		xmlSetProp (nnodo, BAD_CAST "inizio", BAD_CAST "");
		
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
			
			if (contNodo)
			{
				nnodo = xmlNewChild(nintestazione, NULL, BAD_CAST tagTipoToNome(tagerrore), NULL);
				xmlAddChild(nnodo, xmlNewText(BAD_CAST "===== DTD BASE: FORMULA INIZIALE NON AMMESSA =====\n\n"));
				xmlAddChild(nnodo, xmlNewText(contNodo));

			}
			utilNodeDelete(nformulainiziale);
			
			//Trattare il testo come una lista testo/entità:
			//contNodo = xmlNodeGetContent(nformulafinale);	// formula finale
			contNodo = xmlNodeListGetString(NULL, nformulafinale, 0);
			
			if (contNodo)
			{
				nnodo = xmlNewChild(narticolato, NULL, BAD_CAST tagTipoToNome(tagerrore), NULL);
				xmlAddChild(nnodo, xmlNewText(BAD_CAST "===== DTD BASE: CORPO ALLEGATO NON AMMESSO =====\n\n"));
				xmlAddChild(nnodo, xmlNewText(contNodo));

			}
			utilNodeDelete(nformulafinale);
			
			//Trattare il testo come una lista testo/entità:
			//contNodo = xmlNodeGetContent(nconclusione);	// conclusione
			contNodo = xmlNodeListGetString(NULL, nconclusione, 0);
			
			if (contNodo)
			{
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
