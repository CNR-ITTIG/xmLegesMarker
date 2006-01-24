#include "util.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

//char *numeriOrdinali[] = { "" /* 0 */,
//	"prim", "second", "terz", "quart", "quint",
//	"sest", "settim", "ottav", "non", "decim",
//        "undicesim", "dodicesim", "tredicesim", "quattordicesim", "quindicesim",
//	"sedicesim", "diciassettesim", "diciottesim", "diciannovesim", "ventesim"
//	};

//-----------------------FUNZIONI USATE PER IL CALCOLO DELLE PERCENTUALI------------------

static float mPercSingoloElemento=100;
static float mCurrPerc=0;	//Percentuale di esecuzione Corrente

// -------------------------------FLAG----------------------------------------------------

// Flag che determina l'opportunit� di effettuare la conversione in iso 
static int	 globSetAnnessi = 1;

// Se � a 1 indica che il file di origine � in formato UTF-8
static int	 flagUTF8 = 0;


//Imposta la percentuale corrente
void utilPercCurrSet(int pPerc)
{
	mCurrPerc=pPerc;
	loggerPerc(utilItoa((int)mCurrPerc));
}

void utilPercCalc(int pPerc)
{
	mCurrPerc+= (mPercSingoloElemento/100.0f)*pPerc;
	//if (mCurrPerc>100.0f )mCurrPerc=100.0f;
	loggerPerc(utilItoa((int)mCurrPerc));
}

void utilPercNumBlockSet(int pNumAnnessi)
{
	mPercSingoloElemento= 100.0f /(pNumAnnessi + 1);
}

//----------------------------------------------------------------------------------------


//Sgancia il nodo e lo elimina
void utilNodeDelete(xmlNodePtr pNodo)
{
	if (pNodo!=NULL){
		xmlUnlinkNode(pNodo);
		xmlFreeNode(pNodo);
	}
}

//Crea un buffer di dimensioni indicate e lo azzera
//NEW
char *utilCreaBuffer(int size){
	char *tmpbuff=(char *)malloc( sizeof(char)*(size + 1) );
	memset( tmpbuff, 0, sizeof(char)*(size + 1) );
	return tmpbuff;
}

//-----------------	CONTEGGIO NODI (-n) -------------------

static int _utilNodeCountVect[TAGTIPODIM];

void utilNodeCount(xmlNodePtr parentNode){
	int n;
	//Init
	for (n=0;n<TAGTIPODIM;n++)_utilNodeCountVect[n]=0;

	_utilNodeCount(parentNode);
	for (n=0;n<TAGTIPODIM;n++){
		loggerInfo(utilConcatena(4,"NODECOUNT ",tagTipoToNome(n),"=",utilItoa(_utilNodeCountVect[n])));
	}
}

//RICORSIVA
void _utilNodeCount(xmlNodePtr node)
{
	if(node==NULL)return;
	int k=(int)	utilString2TagTipo((char *)node->name);	//Conteggio del nodo
	_utilNodeCountVect[k]++;
	xmlNodePtr cur=node->children;

	while(cur!=NULL)
	{
		if (!xmlNodeIsText(cur))_utilNodeCount(cur);
		cur=cur->next;
	}
}

//---------------------------------------------------------


tagTipo utilString2TagTipo(char  *p)
{
	int n;
	for (n=0;n<TAGTIPODIM;n++){
		if (xmlStrEqual(p,BAD_CAST tagTipoToNome(n))){
			return n;
		}
	}
	//printf("%s\n",p);
	return sconosciuto;
}



//--------------------- ICONV ---------------------
static iconv_t  ConvDescr=NULL;
static iconv_t  ConvDescr2=NULL;

char * utilConvertiText(char *txtin)
{
	char *inizio;

	if ( GetFlagUTF8() )
		inizio = txtin;

	else //converti solo se il testo originario non � UTF-8
	{
	
		size_t lin=strlen(txtin),lout=lin*2,v;
		
		char *bufout=( char *)malloc(lout);
		memset( bufout, 0, lout );
		inizio=bufout;
		const char * por=(const char *)txtin;

		if (!ConvDescr)	
			//if (strcmpi ((const char *)configEncoding(),"UTF-8") !=0 )
			ConvDescr=iconv_open("UTF-8",(char *)configEncoding());
			//ConvDescr=iconv_open("UTF-8","iso-8859-15");
		

		if (ConvDescr==(iconv_t)-1){
			loggerError("iconv Errore iconv_open()");
			exit(0);
		}
		
		v=iconv(ConvDescr,&por,&lin,&bufout,&lout);

		if (v== (size_t) -1){
			if (errno==EILSEQ)loggerError("iconv Errore EILSEQ.");
			if (errno==E2BIG)loggerError("iconv Buffer insufficiente.");
			exit(0);
		}
	}

	//iclose = iconv_close(ConvDescr);
	//if (iclose != 0)
	//{
	//	loggerError("iconv_close() error");
	//	exit(0);
	//}
	
	return inizio;
}


//------------ PER CONVERTIRE DA UTF-8 NEL FORMATO ORIGINARIO, CHE PER DEFAULT E' ISO-8859-15 ************

char * utilConvTextToIso(char *txtin)
{
	char *inizio;

	if ( GetFlagUTF8() )
		inizio = txtin;

	else //converti solo se il testo originario non � UTF-8
	{
		size_t lin=strlen(txtin),lout=lin*2,v;
		
		char *bufout=( char *)malloc(lout);
		memset( bufout, 0, lout );
		inizio=bufout;
		const char * por=(const char *)txtin;

		//UTF8Toisolat1(bufout,lout,txtin,lin);
		
		if (!ConvDescr2)	
			ConvDescr2=iconv_open((char *)configEncoding(),"UTF-8");
			//ConvDescr2=iconv_open("iso-8859-15","UTF-8");


		if (ConvDescr2==(iconv_t)-1){
			loggerError("iconv Errore iconv_open()");
			exit(0);
		}

		v=iconv(ConvDescr2,&por,&lin,&bufout,&lout);

		if (v== (size_t) -1){
			if (errno==EILSEQ)loggerError("iconv Errore EILSEQ.");
			if (errno==E2BIG)loggerError("iconv Buffer insufficiente.");
			exit(0);
		}
	}

	return inizio;
}


//-----------------------------------------------

//----- Setta il valore di globSetAnnessi
void SetFlagAnnessi(int flag)
{
	globSetAnnessi = flag;
}

//----- Recupera il valore di globSetAnnessi
int GetFlagAnnessi(void)
{
	return globSetAnnessi;
}

//---------------------------------------------------

//------Setta il valore di flagUTF8
void SetFlagUTF8(int flagutf)
{
	flagUTF8 = flagutf;
}

//----- Recupera il valore di flagUTF8
int GetFlagUTF8(void)
{
	return flagUTF8;
}

//----------------------------------------------------------

int GetAllNodebyTagTipo(xmlNodePtr *bufdest, xmlNodePtr pNodoParent , xmlChar *pnomeTag){
	
	int count=0;

	xmlNodePtr cur;
	cur = pNodoParent->xmlChildrenNode;	//FirstChild

	while (cur != NULL) {
		if (!xmlStrcmp(cur->name, pnomeTag)){
			bufdest[count]=cur;
			count++;
		}
		cur = cur->next; //next fratello
	}
	return count;
}

int IsNode(xmlNodePtr pNodo,tagTipo ptipo)
{
	if (pNodo!=NULL)
	if (!xmlStrcmp(pNodo->name, tagTipoToNome(ptipo)))
	{
		return 1;
	}
	return 0;
}

void MoveAllChildren(xmlNodePtr pFrom,xmlNodePtr pTo)
{
	
	xmlNodePtr cur= pFrom->children;	//FirstChild
	xmlNodePtr tmp;
	while (cur != NULL) {
	
		tmp=cur;
		//NON SPOSTARE: altrimenti si perde l'informazione sul fratello
		cur = cur->next; //NextChild
		
		xmlUnlinkNode(tmp);
		xmlAddChild(pTo,tmp);
	}
}

//NON RICORSIVA
//Restituisce il primo nodo testo figlio di pNodoParent
xmlNodePtr GetFirstTextNode(xmlNodePtr pNodoParent)
{
	if (pNodoParent!=NULL){
		xmlNodePtr cur= pNodoParent->xmlChildrenNode;	//FirstChild

		while (cur != NULL) {
			if (xmlNodeIsText(cur)){
				return cur;
			}
			cur = cur->next; //NextChild
		}
	}
	return NULL;
}

//RICORSIVA
//Converte tutti i nodi TAGERRORE in PI
//Se il nodo � TAGERRORE e non � un PI, lo sostituisce con una nuova PI
void utilErrore2ProcessingInstruction(xmlNodePtr pNodoParent )
{
	
	unsigned char * xmlCont=NULL; //� come xmlchar *
	xmlNodePtr cur,txtnode,pinode;
	cur = pNodoParent->xmlChildrenNode;	//FirstChild
	//printf("%s\n",pNodoParent->name);
	while (cur != NULL) {
		if ((IsNode(cur,tagerrore))&&(cur->type!=XML_PI_NODE)){
			txtnode=GetFirstTextNode(cur);
	
			if (txtnode){
				xmlCont= xmlNodeGetContent(txtnode);
				/*puts("---------------------------------------------------------------INIZIO PI NODE-----------------------------------------------\n");
				puts(xmlCont);
				puts("---------------------------------------------------------------FINE PI NODE-----------------------------------------------\n");			*/
				pinode=xmlNewPI(BAD_CAST tagTipoToNome(tagerrore),xmlCont);
			}else
			{
				//ERRORE VUOTO
				pinode=xmlNewPI(BAD_CAST tagTipoToNome(tagerrore),BAD_CAST "");
			}
			//sostituisco la PI al nodo corrente e poi libero il nodo vecchio
			xmlFreeNode(xmlReplaceNode(cur,pinode));
			cur=pinode;//riaggiorno il puntatore corrente alla nuova PI			}
		}
		utilErrore2ProcessingInstruction(cur);
		cur = cur->next; //NextChild
	}
}

//Di un nodo "pNodoParent" restituisce il primo nodo figlio che possiede il nome "pnomeTag"
//NON RICORSIVA
xmlNodePtr GetFirstNodebyTagTipo(xmlNodePtr pNodoParent , xmlChar *pnomeTag){
	
	xmlNodePtr cur;
	cur = pNodoParent->xmlChildrenNode;	//FirstChild

	while (cur != NULL) {
		if (!xmlStrcmp(cur->name, pnomeTag))	{
			return cur;
		}
		cur = cur->next; //NextChild
	}
	return NULL;
}

void InsertXmlFile(char *memdoc,xmlNodePtr pParentNode) {
	xmlDocPtr doc;
	xmlNodePtr cur;
	
	doc = xmlParseMemory(memdoc,strlen(memdoc));
//	doc = xmlParseFile(docname);
	if (doc == NULL ) {
		return;
	}
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		xmlFreeDoc(doc);
		return;
	}

	//Accoda tutti i figli della radice a pParentNode
	xmlNodePtr ntmp;
	ntmp = cur->xmlChildrenNode;	//FirstChild

	while (ntmp != NULL) {
			xmlAddChild(pParentNode, ntmp);
			ntmp = ntmp->next; //next fratello
	}
	return;
}

//Restituisce (creando un nuovo buffer con malloc) una porzione di testo di srctesto
//NEW
char	*utilGetPartialText(char *ptesto,int pindexstart, int plen)
{
	char *buff=utilCreaBuffer(plen);
	char *ptemp=ptesto + pindexstart; //Avanzamento del buffer di testo
	strncpy(buff,ptemp,plen);
	return buff;
}


/******************************************************************************/
/************************************************************ CERCA SPAZIO ****/
/******************************************************************************/
char * cercaSpazio(char *s) {
	for (; *s; s++)
		if (isspace(*s))
			return s;
	return NULL;
}


/******************************************************************************/
/************************************************************ CONTA CHAR ******/
/******************************************************************************/
int contaChar(char *s, char c) {
	int n=0;
	for (; *s; s++)
		if (*s == c) n++;
	return n;
}


/******************************************************************************/
/********************************************************* LATIN TO ARABIC ****/
/******************************************************************************/
int latinToArabic(char *s) {
	int ret = 0;
	if (strstr(s,"un")) ret = 1 ;
	else if (strstr(s, "bis")||strstr(s,"duo")) ret = 2;
	else if (strstr(s, "quater")) 	ret = 4;
	else if (strstr(s, "ter")) 		ret = 3;
	else if (strstr(s, "quinquies"))	ret = 5;
	else if (strstr(s, "sexies")) 	ret = 6;
	else if (strstr(s, "septies")) 	ret = 7;
	else if (strstr(s, "octies")) 	ret = 8;
	else if (strstr(s, "novies")) 	ret = 9;
	
	if (strstr(s,"decies")) ret=ret+10;
	else if (strstr(s,"venies")) ret=ret+20;

	return ret;
}



/******************************************************************************/
/********************************************************* ARABIC TO LATIN ****/
/******************************************************************************/
char * arabicToLatin(int n) {
	char * r;
	switch (n % 10) {
		case 1: r = "un";	break;
		case 2: r = "bis";	break;
		case 3: r = "ter";	break;
		case 4: r = "quater";	break;
		case 5: r = "quinquies";break;
		case 6: r = "sexies";	break;
		case 7: r = "septies";	break;
		case 8: r = "octies";	break;
		case 9: r = "novies";	break;
		default:r = "";	break;
	}
	return r;
}

/******************************************************************************/
/************************************************************ CONV LETTERA ****/
/******************************************************************************/
int convLetteraToInt(char *s) {
	int r;
	if (isalpha(*s))  //// Con 3 cifre!
		r = tolower(*s) - 'a' + 1;
	if (isalpha(*(s+1)))
		r += 26;
	return r;
}

/******************************************************************* 
Nome:			sostStr
Scopo:		SOSTITUISCE IN <STRINGA> OGNI OCCORRENZA DELLA STRINGA <OLD>
				CON LA STRINGA <NEW>                                              
Parametri:	STRINGA = STRINGA SULLA QUALE AVVIENE IL CAMBIAMENTO             
				OLD     = SOTTOSTRINGA DA SOSTITUIRE                             
				NEW     = SOTTOSTRINGA SOSTITUITA A <OLD>          P.L.S.   
 ********************************************************************/ 

void sostStr(stringa, old, new)
	char  *stringa, *old, *new;
{
int   lold, lnew, ldif, k1, i;
char  *pun;

lold=strlen(old);

if (strlen(stringa)>0 && lold>0) 
	{

	lnew=strlen(new);
	ldif=lnew-lold;

	pun=strstr(stringa,old);   /* cerco old in stringa */
	while (pun != NULL)        /* finche' trovo old */
		{
		if (ldif>0)             /* si inseriscono car. */
			{
			k1=pun-stringa;      /* ind. ultimo car. da spostare */

			for(i=strlen(stringa)+ldif; i>k1; i--)    /* creo */ 
				stringa[i]=stringa[i-ldif];            /* posto */
			}

		else if (ldif<0)        /* accorcio */ 
			strcpy(pun+lold+ldif,pun+lold);

		strncpy(pun,new,lnew);     /* sostituisco */

		pun=strstr(pun+lnew,old);  /* cerco prossimo old */

		} /* fine while */

	} /* fine if */

} /* fine sostStr */
