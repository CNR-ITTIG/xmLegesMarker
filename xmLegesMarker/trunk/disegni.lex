%{
// Scanner Case-Insensitive
// compilare con flex -i -8 -Pddl nomefile

// conta le testate 'Disegno di legge' presenti nel documento
// se il numero di testate è maggiore di 2, lo imposta a 2

#include <stdio.h>
#include <string.h>
#include <IttigLogger.h>
#include <IttigUtil.h>
#include "config.h"
#define __DEBUG__  0

int ddlwrap()
{
	return 1;
}

int  disegni=0;
char msg[30]="DDL: testate trovate=\0";

%}

S	([ ])
NL	(\n)

DISEGNO		((disegno|progetto|proposta){S}+di{S}+legge)

%%

^({S}*{DISEGNO}{S}*)$	{
			disegni++;
			}


[a-z0-9]+		
.|{NL}			

%%

//Restituisce il numero di testate 'disegno di legge' trovati

void  _disegniLexStart(char *testo)
{
	yy_init = 1;
	ddl_scan_string(testo);
	ddllex();
	
	loggerDebug(utilConcatena(2, msg, utilItoa(disegni)));
	if ( disegni > 2 )
		disegni=2;

	configSetDdlTestate(disegni);

	return;
}

