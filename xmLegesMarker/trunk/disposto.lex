/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		disposto.lex
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
%{
// Scanner Case-Insensitive
// compilare con flex -i -8 -Pdis disposto
#include <stdio.h>
#include <string.h>
#include "disposto.h"
#include <IttigLogger.h>
#define __DEBUG__  0

int diswrap()
{
	return 1;
}


%}

S	([ ])
NL	(\n)
FINEPAR	([.;:]{S}*)
DECRETA	(d{S}*e{S}*c{S}*r{S}*e{S}*t{S}*a)
EMANA		(e{S}*m{S}*a{S}*n{S}*a)
ADOTTA	(a{S}*d{S}*o{S}*t{S}*t{S}*a)
DELIBERA	(d{S}*e{S}*l{S}*i{S}*b{S}*e{S}*r{S}*a)
DISPONE	(d{S}*i{S}*s{S}*p{S}*o{S}*n{S}*e)
DETERMINA (d{S}*e{S}*t{S}*e{S}*r{S}*m{S}*i{S}*n{S}*a)

AGISCE	({DECRETA}|{EMANA}|{ADOTTA}|{DELIBERA}|{DISPONE}|{DETERMINA})

REGOLAMENTO	({S}+il{S}+seguente{S}+regolamento)

%x InDisposto

%%

^{S}*{AGISCE}{REGOLAMENTO}?{S}*:?{S}*{NL}		{	
							disAppendString(strdup(distext));
							disAppendString("</h:p>\n");
							disSaveIni();
							disAppendString("<h:p>");
							BEGIN(InDisposto);
							}
							
({FINEPAR}{NL}|({NL}{S}*){2,})				{
							disAppendString(strdup(distext));
							disAppendString("</h:p>\n<h:p>");
							}

([a-z0-9]+|.|\n)					disAppendString(strdup(distext));

<InDisposto>({FINEPAR}{NL}|({NL}{S}*){2,})		{
							disAppendString(strdup(distext));
							disAppendString("</h:p>\n<h:p>");
							}

<InDisposto>([a-z0-9]+|.|\n)				disAppendString(strdup(distext));

%%

