/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		prehtml.lex
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
* 			Lorenzo Bacci (bacci@ittig.cnr.it)
******************************************************************************/

/*

&#186;			prehtmlAppendString("�");		//GRADO
\xBA				prehtmlAppendString("�");		//GRADO

*/

/*
//OLD InComment:

\<!--						{
							BEGIN(InComment);
							}

<InComment>({TKN}+|{Q})		{
							prehtmlAppendString(strdup(prehtmltext));
							}

<InComment>-->				{
							prehtmlAppendString(strdup(prehtmltext));
							BEGIN(0);
							}											
*/

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
int flagpre = 1;
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

&quot;			prehtmlAppendString("\"");		// convertite virgolette
&nbsp;			prehtmlAppendString(" ");		// convertito spazio non divisibile

&[a-z]+;		prehtmlAppendString(strdup(prehtmltext));	// lasciate entit� simboliche

&#128;			prehtmlAppendChars(1, '\x80');		//mapping dei caratteri "windows"
&#129;			prehtmlAppendChars(1, '\x81');
&#130;			prehtmlAppendChars(1, '\x82');
&#131;			prehtmlAppendChars(1, '\x83');
&#132;			prehtmlAppendChars(1, '\x84');
&#133;			prehtmlAppendChars(1, '\x85');
&#134;			prehtmlAppendChars(1, '\x86');
&#135;			prehtmlAppendChars(1, '\x87');
&#136;			prehtmlAppendChars(1, '\x88');
&#137;			prehtmlAppendChars(1, '\x89');
&#138;			prehtmlAppendChars(1, '\x8A');
&#139;			prehtmlAppendChars(1, '\x8B');
&#140;			prehtmlAppendChars(1, '\x8C');
&#141;			prehtmlAppendChars(1, '\x8D');
&#142;			prehtmlAppendChars(1, '\x8E');
&#143;			prehtmlAppendChars(1, '\x8F');
&#144;			prehtmlAppendChars(1, '\x90');
&#145;			prehtmlAppendChars(1, '\x91');
&#146;			prehtmlAppendChars(1, '\x92');
&#147;			prehtmlAppendChars(1, '\x93');
&#148;			prehtmlAppendChars(1, '\x94');
&#149;			prehtmlAppendChars(1, '\x95');
&#150;			prehtmlAppendChars(1, '\x96');
&#151;			prehtmlAppendChars(1, '\x97');
&#152;			prehtmlAppendChars(1, '\x98');
&#153;			prehtmlAppendChars(1, '\x99');
&#154;			prehtmlAppendChars(1, '\x9A');
&#155;			prehtmlAppendChars(1, '\x9B');
&#156;			prehtmlAppendChars(1, '\x9C');
&#157;			prehtmlAppendChars(1, '\x9D');
&#158;			prehtmlAppendChars(1, '\x9E');
&#159;			prehtmlAppendChars(1, '\x9F');

&#171;			prehtmlAppendChars(1, '\xAB');		//#LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
&#187;			prehtmlAppendChars(1, '\xBB');		//#RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK

&#8216;			prehtmlAppendString("\'");
&#8217;			prehtmlAppendString("\'");

&#8221;			prehtmlAppendString("\"");
&#8222;			prehtmlAppendString("\"");

&#8026;			prehtmlAppendString(""); //Elimina punto centrale (bullet)
&#167;				prehtmlAppendString(""); //Elimina section symbol

&#[0-9]+;		prehtmlAppendString(strdup(prehtmltext));	// lasciate entit� numeriche  //(..se ne rimangono..)

&					prehtmlAppendString("&#38;");		// & che non delimita un'entit�

\<\?						{
							prehtmlAppendString(strdup(prehtmltext));
							BEGIN(InPI);}
							
<InPI>({TKN}+|{Q})			prehtmlAppendString(strdup(prehtmltext));

<InPI>\?>					{
							prehtmlAppendString(strdup(prehtmltext));
							BEGIN(0);}
							
\<!--						{
							BEGIN(InComment);}
							
<InComment>-->				{
							BEGIN(0);
							}

\<pre>						{	
							flagpre=1;}

{NL}							{
									prehtmlAppendChars(1,'\n');
								}
								
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
								if (Tipo[ii]==-1) Tipo[ii]=1; //Se non c'� il tipo di defaut � numerica
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

/*
---------------------------------
cp 1252 Encoding (from cp1252.txt)
---------------------------------
=20	U+0020	SPACE
=21	U+0021	EXCLAMATION MARK
=22	U+0022	QUOTATION MARK
=23	U+0023	NUMBER SIGN
=24	U+0024	DOLLAR SIGN
=25	U+0025	PERCENT SIGN
=26	U+0026	AMPERSAND
=27	U+0027	APOSTROPHE
=28	U+0028	LEFT PARENTHESIS
=29	U+0029	RIGHT PARENTHESIS
=2A	U+002A	ASTERISK
=2B	U+002B	PLUS SIGN
=2C	U+002C	COMMA
=2D	U+002D	HYPHEN-MINUS
=2E	U+002E	FULL STOP
=2F	U+002F	SOLIDUS
=30	U+0030	DIGIT ZERO
=31	U+0031	DIGIT ONE
=32	U+0032	DIGIT TWO
=33	U+0033	DIGIT THREE
=34	U+0034	DIGIT FOUR
=35	U+0035	DIGIT FIVE
=36	U+0036	DIGIT SIX
=37	U+0037	DIGIT SEVEN
=38	U+0038	DIGIT EIGHT
=39	U+0039	DIGIT NINE
=3A	U+003A	COLON
=3B	U+003B	SEMICOLON
=3C	U+003C	LESS-THAN SIGN
=3D	U+003D	EQUALS SIGN
=3E	U+003E	GREATER-THAN SIGN
=3F	U+003F	QUESTION MARK
=40	U+0040	COMMERCIAL AT
=41	U+0041	LATIN CAPITAL LETTER A
=42	U+0042	LATIN CAPITAL LETTER B
=43	U+0043	LATIN CAPITAL LETTER C
=44	U+0044	LATIN CAPITAL LETTER D
=45	U+0045	LATIN CAPITAL LETTER E
=46	U+0046	LATIN CAPITAL LETTER F
=47	U+0047	LATIN CAPITAL LETTER G
=48	U+0048	LATIN CAPITAL LETTER H
=49	U+0049	LATIN CAPITAL LETTER I
=4A	U+004A	LATIN CAPITAL LETTER J
=4B	U+004B	LATIN CAPITAL LETTER K
=4C	U+004C	LATIN CAPITAL LETTER L
=4D	U+004D	LATIN CAPITAL LETTER M
=4E	U+004E	LATIN CAPITAL LETTER N
=4F	U+004F	LATIN CAPITAL LETTER O
=50	U+0050	LATIN CAPITAL LETTER P
=51	U+0051	LATIN CAPITAL LETTER Q
=52	U+0052	LATIN CAPITAL LETTER R
=53	U+0053	LATIN CAPITAL LETTER S
=54	U+0054	LATIN CAPITAL LETTER T
=55	U+0055	LATIN CAPITAL LETTER U
=56	U+0056	LATIN CAPITAL LETTER V
=57	U+0057	LATIN CAPITAL LETTER W
=58	U+0058	LATIN CAPITAL LETTER X
=59	U+0059	LATIN CAPITAL LETTER Y
=5A	U+005A	LATIN CAPITAL LETTER Z
=5B	U+005B	LEFT SQUARE BRACKET
=5C	U+005C	REVERSE SOLIDUS
=5D	U+005D	RIGHT SQUARE BRACKET
=5E	U+005E	CIRCUMFLEX ACCENT
=5F	U+005F	LOW LINE
=60	U+0060	GRAVE ACCENT
=61	U+0061	LATIN SMALL LETTER A
=62	U+0062	LATIN SMALL LETTER B
=63	U+0063	LATIN SMALL LETTER C
=64	U+0064	LATIN SMALL LETTER D
=65	U+0065	LATIN SMALL LETTER E
=66	U+0066	LATIN SMALL LETTER F
=67	U+0067	LATIN SMALL LETTER G
=68	U+0068	LATIN SMALL LETTER H
=69	U+0069	LATIN SMALL LETTER I
=6A	U+006A	LATIN SMALL LETTER J
=6B	U+006B	LATIN SMALL LETTER K
=6C	U+006C	LATIN SMALL LETTER L
=6D	U+006D	LATIN SMALL LETTER M
=6E	U+006E	LATIN SMALL LETTER N
=6F	U+006F	LATIN SMALL LETTER O
=70	U+0070	LATIN SMALL LETTER P
=71	U+0071	LATIN SMALL LETTER Q
=72	U+0072	LATIN SMALL LETTER R
=73	U+0073	LATIN SMALL LETTER S
=74	U+0074	LATIN SMALL LETTER T
=75	U+0075	LATIN SMALL LETTER U
=76	U+0076	LATIN SMALL LETTER V
=77	U+0077	LATIN SMALL LETTER W
=78	U+0078	LATIN SMALL LETTER X
=79	U+0079	LATIN SMALL LETTER Y
=7A	U+007A	LATIN SMALL LETTER Z
=7B	U+007B	LEFT CURLY BRACKET
=7C	U+007C	VERTICAL LINE
=7D	U+007D	RIGHT CURLY BRACKET
=7E	U+007E	TILDE
=80	U+20AC	EURO SIGN
=82	U+201A	SINGLE LOW-9 QUOTATION MARK
=83	U+0192	LATIN SMALL LETTER F WITH HOOK
=84	U+201E	DOUBLE LOW-9 QUOTATION MARK
=85	U+2026	HORIZONTAL ELLIPSIS
=86	U+2020	DAGGER
=87	U+2021	DOUBLE DAGGER
=88	U+02C6	MODIFIER LETTER CIRCUMFLEX ACCENT
=89	U+2030	PER MILLE SIGN
=8A	U+0160	LATIN CAPITAL LETTER S WITH CARON
=8B	U+2039	SINGLE LEFT-POINTING ANGLE QUOTATION MARK
=8C	U+0152	LATIN CAPITAL LIGATURE OE
=8E	U+017D	LATIN CAPITAL LETTER Z WITH CARON
=91	U+2018	LEFT SINGLE QUOTATION MARK
=92	U+2019	RIGHT SINGLE QUOTATION MARK
=93	U+201C	LEFT DOUBLE QUOTATION MARK
=94	U+201D	RIGHT DOUBLE QUOTATION MARK
=95	U+2022	BULLET
=96	U+2013	EN DASH
=97	U+2014	EM DASH
=98	U+02DC	SMALL TILDE
=99	U+2122	TRADE MARK SIGN
=9A	U+0161	LATIN SMALL LETTER S WITH CARON
=9B	U+203A	SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
=9C	U+0153	LATIN SMALL LIGATURE OE
=9E	U+017E	LATIN SMALL LETTER Z WITH CARON
=9F	U+0178	LATIN CAPITAL LETTER Y WITH DIAERESIS
=A0	U+00A0	NO-BREAK SPACE
=A1	U+00A1	INVERTED EXCLAMATION MARK
=A2	U+00A2	CENT SIGN
=A3	U+00A3	POUND SIGN
=A4	U+00A4	CURRENCY SIGN
=A5	U+00A5	YEN SIGN
=A6	U+00A6	BROKEN BAR
=A7	U+00A7	SECTION SIGN
=A8	U+00A8	DIAERESIS
=A9	U+00A9	COPYRIGHT SIGN
=AA	U+00AA	FEMININE ORDINAL INDICATOR
=AB	U+00AB	LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
=AC	U+00AC	NOT SIGN
=AD	U+00AD	SOFT HYPHEN
=AE	U+00AE	REGISTERED SIGN
=AF	U+00AF	MACRON
=B0	U+00B0	DEGREE SIGN
=B1	U+00B1	PLUS-MINUS SIGN
=B2	U+00B2	SUPERSCRIPT TWO
=B3	U+00B3	SUPERSCRIPT THREE
=B4	U+00B4	ACUTE ACCENT
=B5	U+00B5	MICRO SIGN
=B6	U+00B6	PILCROW SIGN
=B7	U+00B7	MIDDLE DOT
=B8	U+00B8	CEDILLA
=B9	U+00B9	SUPERSCRIPT ONE
=BA	U+00BA	MASCULINE ORDINAL INDICATOR
=BB	U+00BB	RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
=BC	U+00BC	VULGAR FRACTION ONE QUARTER
=BD	U+00BD	VULGAR FRACTION ONE HALF
=BE	U+00BE	VULGAR FRACTION THREE QUARTERS
=BF	U+00BF	INVERTED QUESTION MARK
=C0	U+00C0	LATIN CAPITAL LETTER A WITH GRAVE
=C1	U+00C1	LATIN CAPITAL LETTER A WITH ACUTE
=C2	U+00C2	LATIN CAPITAL LETTER A WITH CIRCUMFLEX
=C3	U+00C3	LATIN CAPITAL LETTER A WITH TILDE
=C4	U+00C4	LATIN CAPITAL LETTER A WITH DIAERESIS
=C5	U+00C5	LATIN CAPITAL LETTER A WITH RING ABOVE
=C6	U+00C6	LATIN CAPITAL LETTER AE
=C7	U+00C7	LATIN CAPITAL LETTER C WITH CEDILLA
=C8	U+00C8	LATIN CAPITAL LETTER E WITH GRAVE
=C9	U+00C9	LATIN CAPITAL LETTER E WITH ACUTE
=CA	U+00CA	LATIN CAPITAL LETTER E WITH CIRCUMFLEX
=CB	U+00CB	LATIN CAPITAL LETTER E WITH DIAERESIS
=CC	U+00CC	LATIN CAPITAL LETTER I WITH GRAVE
=CD	U+00CD	LATIN CAPITAL LETTER I WITH ACUTE
=CE	U+00CE	LATIN CAPITAL LETTER I WITH CIRCUMFLEX
=CF	U+00CF	LATIN CAPITAL LETTER I WITH DIAERESIS
=D0	U+00D0	LATIN CAPITAL LETTER ETH
=D1	U+00D1	LATIN CAPITAL LETTER N WITH TILDE
=D2	U+00D2	LATIN CAPITAL LETTER O WITH GRAVE
=D3	U+00D3	LATIN CAPITAL LETTER O WITH ACUTE
=D4	U+00D4	LATIN CAPITAL LETTER O WITH CIRCUMFLEX
=D5	U+00D5	LATIN CAPITAL LETTER O WITH TILDE
=D6	U+00D6	LATIN CAPITAL LETTER O WITH DIAERESIS
=D7	U+00D7	MULTIPLICATION SIGN
=D8	U+00D8	LATIN CAPITAL LETTER O WITH STROKE
=D9	U+00D9	LATIN CAPITAL LETTER U WITH GRAVE
=DA	U+00DA	LATIN CAPITAL LETTER U WITH ACUTE
=DB	U+00DB	LATIN CAPITAL LETTER U WITH CIRCUMFLEX
=DC	U+00DC	LATIN CAPITAL LETTER U WITH DIAERESIS
=DD	U+00DD	LATIN CAPITAL LETTER Y WITH ACUTE
=DE	U+00DE	LATIN CAPITAL LETTER THORN
=DF	U+00DF	LATIN SMALL LETTER SHARP S
=E0	U+00E0	LATIN SMALL LETTER A WITH GRAVE
=E1	U+00E1	LATIN SMALL LETTER A WITH ACUTE
=E2	U+00E2	LATIN SMALL LETTER A WITH CIRCUMFLEX
=E3	U+00E3	LATIN SMALL LETTER A WITH TILDE
=E4	U+00E4	LATIN SMALL LETTER A WITH DIAERESIS
=E5	U+00E5	LATIN SMALL LETTER A WITH RING ABOVE
=E6	U+00E6	LATIN SMALL LETTER AE
=E7	U+00E7	LATIN SMALL LETTER C WITH CEDILLA
=E8	U+00E8	LATIN SMALL LETTER E WITH GRAVE
=E9	U+00E9	LATIN SMALL LETTER E WITH ACUTE
=EA	U+00EA	LATIN SMALL LETTER E WITH CIRCUMFLEX
=EB	U+00EB	LATIN SMALL LETTER E WITH DIAERESIS
=EC	U+00EC	LATIN SMALL LETTER I WITH GRAVE
=ED	U+00ED	LATIN SMALL LETTER I WITH ACUTE
=EE	U+00EE	LATIN SMALL LETTER I WITH CIRCUMFLEX
=EF	U+00EF	LATIN SMALL LETTER I WITH DIAERESIS
=F0	U+00F0	LATIN SMALL LETTER ETH
=F1	U+00F1	LATIN SMALL LETTER N WITH TILDE
=F2	U+00F2	LATIN SMALL LETTER O WITH GRAVE
=F3	U+00F3	LATIN SMALL LETTER O WITH ACUTE
=F4	U+00F4	LATIN SMALL LETTER O WITH CIRCUMFLEX
=F5	U+00F5	LATIN SMALL LETTER O WITH TILDE
=F6	U+00F6	LATIN SMALL LETTER O WITH DIAERESIS
=F7	U+00F7	DIVISION SIGN
=F8	U+00F8	LATIN SMALL LETTER O WITH STROKE
=F9	U+00F9	LATIN SMALL LETTER U WITH GRAVE
=FA	U+00FA	LATIN SMALL LETTER U WITH ACUTE
=FB	U+00FB	LATIN SMALL LETTER U WITH CIRCUMFLEX
=FC	U+00FC	LATIN SMALL LETTER U WITH DIAERESIS
=FD	U+00FD	LATIN SMALL LETTER Y WITH ACUTE
=FE	U+00FE	LATIN SMALL LETTER THORN
=FF	U+00FF	LATIN SMALL LETTER Y WITH DIAERESIS
*/

