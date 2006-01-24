%{
// Scanner Case-Insensitive
// compilare con flex -i -8 -Pall nomefile
#include <stdio.h>
#include <string.h>
#include "tabelle.h"

int tabpos=0;
int IsInTable=0;
int lastRowStart=0;
int firstSave=1;
int currentRowCount=0;
int lastRowCount=0;

void TabIncPos(void){
	tabpos+=tableng;
}

void tsave(tagTipo t,int p) //p è la posizione all'interno della tabella
{
	if(firstSave)
	{
		xxxTagOpen(tagerrore,0,0);
		firstSave=0;
	}
	
	if (IsInTable==0){
	
		//Tutto il testo non riconosciuto viene Acodato "all'ultimo" tag Aperto
		xxxAppendTextToLastNode(p);
		xxxTagCloseFrom(tagerrore);	//Il tag ERRORE viene chiuso
					
		xxxTagOpen(h_table,p,0);
		xxxTagOpen(h_tr,p,0);
	
		loggerDebug("Individuata Tabella");	
		IsInTable=1;
	}
	
	xxxTagOpen(t,p,0);
}

%}

S		([ ])
NL		(\n)
ROWS		([-]{4,}|[=]{4,})
SEPCOL_SPACE	([|]|{S}{6,})
SEPCOL		([|])

%%
{NL}				{	//Nuova Riga
						if(IsInTable)
						if ((currentRowCount-lastRowCount)>1) 
						{
							xxxTagOpen(tagerrore,lastRowStart,0);
							IsInTable=0;
						}
						TabIncPos();
						if (IsInTable){
							tsave(h_tr,tabpos);
							tsave(h_td,tabpos);
						}
						lastRowStart=tabpos;
						currentRowCount++;
					}

{SEPCOL}			{
						lastRowCount=currentRowCount;
						//se non è in una tabella imposta un TD ad inizio Riga
						if (!IsInTable){
							tsave(h_td,lastRowStart);
						}
						tsave(h_td,tabpos);
						TabIncPos();
					}

^{S}*{ROWS}/{NL}	{
						lastRowCount=currentRowCount;					
						tsave(h_td,lastRowStart);
						TabIncPos();
					}

[a-z0-9]+				TabIncPos();
.					TabIncPos();

%%


int tabwrap()
{
	return 1;
}

int _tabLexStart(char *testo)
{
	BEGIN(0);
	yy_init = 1;
	tabpos=0;

	
	firstSave=1;
	IsInTable=0;
	lastRowStart=0;
	currentRowCount=0;
	lastRowCount=0;
					

	tab_scan_string(testo);

	tablex();
	return !firstSave; //ritorna 1 se è stata individuata una tabella
}
