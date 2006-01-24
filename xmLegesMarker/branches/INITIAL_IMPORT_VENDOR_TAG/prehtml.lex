%{
#include <stdio.h>
#include <string.h>
#include <IttigUtil.h>
#include <IttigLogger.h>

#include "config.h"
#include "prehtml.h"
#include "pre.h"

int Lista[20];
int Tipo[20];
int ii,jj;
int flagpre = 0;
static int numStart;
char ExtractChar;

//char *numLI;
char *cifre = NULL;
char *Uita = NULL;

int prehtmlwrap() 
{
	return 1;
}

void azzera(void)
{

	for (ii=0; ii<20; ii++)
	{
		Lista[ii] = 0;
		Tipo[ii] = -1;
	}
	ii = 0;
	jj = 0;
}


//\<pre>							flagpre=1;

//{NL}							{
//								if (flagpre==0) prehtmlAppendChars(1,' ');
//								if (flagpre==1)	prehtmlAppendChars(1,'\n');}
								
//\<\/pre>						flagpre=0;


%}


S	([ ])
NL	(\n)
Q	(.|\n)
TKN	([a-z0-9])
LISTAORD (OL) 
LISTANONORD (UL)
ELEMLISTA (li)
TAGNL	(br)
TAG2NL	(div|p)
TAGFORMAT (I)
TAGA (a )

TAGHEAD	(head)
TAGFORM (form)

TAGDEL	(head|form|script|style)

%x InTagLista ListaTipo ListaStart InPI InTag InTagListaNonOrd
%x InTagBA InTagDiv InTagDel InTagElementList InComment
%option stack

%%

&quot;			prehtmlAppendChars(1, '"');		// convertite virgolette
&nbsp;			prehtmlAppendChars(1, ' ');		// convertito spazio non divisibile

&[a-z]+;		prehtmlAppendString(strdup(prehtmltext));	// lasciate entità simboliche
&#[0-9]+;		prehtmlAppendString(strdup(prehtmltext));	// lasciate entità numeriche

&			norAppendString("&#38;");		// & che non delimita un'entità


\<\?						{
							prehtmlAppendString(strdup(prehtmltext));
							BEGIN(InPI);}
							
<InPI>({TKN}+|{Q})					prehtmlAppendString(strdup(prehtmltext));

<InPI>\?>					{
							prehtmlAppendString(strdup(prehtmltext));
							BEGIN(0);}
							
\<!--						{
							prehtmlAppendString(strdup(prehtmltext));
							BEGIN(InComment);}
							
<InComment>({TKN}+|{Q})				prehtmlAppendString(strdup(prehtmltext));

<InComment>-->				{
							prehtmlAppendString(strdup(prehtmltext));
							BEGIN(0);}

\<pre>						{	
							flagpre=1;}

{NL}							{
								if (flagpre==0) prehtmlAppendChars(1,' ');
								if (flagpre==1)	prehtmlAppendChars(1,'\n');}
								
\<\/(pre)>						{
								flagpre=0;}
								

\<{TAGNL}>						{
								prehtmlAppendChars(1, '\n'); }
								
\<{TAG2NL}						{		
								prehtmlAppendChars(2, '\n');
								BEGIN(InTagDiv);}
<InTagDiv>[^>]
<InTagDiv>>						BEGIN(0);
				
\<								{
								BEGIN(InTag);}
<InTag>[^>]{0,100}					
<InTag>>						{
								BEGIN(0);}

\<\<							prehtmlAppendString(strdup(prehtmltext));
\>\>							prehtmlAppendString(strdup(prehtmltext));
							
\<{TAGDEL}						{
								jj++;							
								BEGIN(InTagDel);}
<InTagDel>({TKN}+|{Q})				
<InTagDel>\<\/{TAGDEL}>			{
								jj--;
								if (jj==0)
									BEGIN(0);}
				
\<{LISTAORD}		            {
								ii++;BEGIN(InTagLista);}	
<InTagLista>>					{
								if (Tipo[ii]==-1) Tipo[ii]=1; //Se non c'è il tipo di defaut è numerica
								BEGIN(0);}					

<InTagLista>(type=["]?[0-9]["]?)	{
								Tipo[ii]=1; //lista numerica
								}
<InTagLista>(type=["]?[a-zA-Z]["]?)	{
								Tipo[ii]=0; //lista alfabetica
								}
<InTagLista>(start=["]?[0-9]+["]?)	{
								cifre=utilIsolaNum(yytext); 
								numStart = atoi(cifre);
								Lista[ii]=(numStart-1);
								}
								
<InTagLista>({TKN}+|{Q})

\<{LISTANONORD}[^>]*>			ii++; Tipo[ii]=2; //lista puntata




 						
\<{ELEMLISTA}					Lista[ii]++;BEGIN(InTagElementList);

<InTagElementList>(value=["]?[0-9]+["]?)	{
											cifre=utilIsolaNum(yytext); 
											numStart = atoi(cifre);
											Lista[ii]=(numStart-1);
											}

<InTagElementList>>		{
						
						prehtmlAppendChars(2,'\n');
						if (Tipo[ii]==1)
							{
							
							Uita=utilItoa(Lista[ii]);					
							prehtmlAppendString(Uita);//visualizza il numero
							prehtmlAppendChars(1,')');
							prehtmlAppendChars(1,' ');
							}
						if (Tipo[ii]==0)
							{
							ExtractChar = prehtmlExtractChar(Lista[ii]);
							if (Lista[ii]<27)
								prehtmlAppendChars(1,ExtractChar);
							else if (Lista[ii]<677)
								prehtmlAppendChars(2,ExtractChar);
							else
								prehtmlAppendChars(3,ExtractChar);
							prehtmlAppendChars(1,')');
							prehtmlAppendChars(1,' ');
							}
						if (Tipo[ii]==2)
							{
							prehtmlAppendChars(1,'-');
							prehtmlAppendChars(1,' ');
							}

						BEGIN(0);
						}

<InTagElementList>({TKN}+|{Q})




							

\<\/{ELEMLISTA}>				
\<\/{LISTAORD}>					{
								prehtmlAppendChars(2,'\n');
								Lista[ii]=0;
								ii--; }
\<\/{LISTANONORD}>				{
								prehtmlAppendChars(2,'\n');
								Lista[ii]=0;
								ii--; }
									
.								prehtmlAppendString(strdup(prehtmltext));
												

%%