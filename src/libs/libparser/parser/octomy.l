%option noyywrap

%{

// WARNING:                                                             :WARNING
// WARNING: DO NOT EDIT THIS FILE! IT IS AUTOGENERATED BY FLEX          :WARNING
// WARNING:                                                             :WARNING


#include <cstdlib>
#include "octomy_parser.hpp"
#include <QDebug>

#define YY_DECL int OctomyLexer::nextToken()

%}

hex [0-9a-fA-F]
dec [1-9][0-9]*
oct [0-7]
digit [0-9]
bidig [01]

fract {digit}*\.{digit}+|{digit}+\.
exp [eE][+-]?{digit}+

hexfract {hex}*\.{hex}+|{hex}+\.
binexp [pP][+-]?{digit}+

icst ({dec}|0{oct}*|0[xX]{hex}+|0[bB]{bidig}+)

%%

%{
/* KEYWORDS */
%}

"translation_unit" { return octomy_table::TOK_TRANSLATION_UNIT; }

"spec" { return octomy_table::TOK_SPEC; }

"plan" { return octomy_table::TOK_PLAN; }


"sensor" { return octomy_table::TOK_SENSOR; }
"camera" { return octomy_table::TOK_CAMERA; }
"serial" { return octomy_table::TOK_SERIAL; }
"bluetooth" { return octomy_table::TOK_BLUETOOTH; }
"nfc" { return octomy_table::TOK_NFC; }
"controller" { return octomy_table::TOK_CONTROLLER; }

"hub" { return octomy_table::TOK_HUB; }
"remote" { return octomy_table::TOK_REMOTE; }
"agent" { return octomy_table::TOK_AGENT; }
"puppet" { return octomy_table::TOK_PUPPET; }

"member" { return octomy_table::TOK_MEMBER; }

"pubkey" { return octomy_table::TOK_PUBKEY; }
"key" { return octomy_table::TOK_KEY; }

"plugin" { return octomy_table::TOK_PLUGIN; }
"import" { return octomy_table::TOK_IMPORT; }

"descriptor" { return octomy_table::TOK_DESCRIPTOR; }

"("     { return octomy_table::TOK_LPAREN; }
")"     { return octomy_table::TOK_RPAREN; }
"["     { return octomy_table::TOK_LBRACKET; }
"]"     { return octomy_table::TOK_RBRACKET; }
"{"     { return octomy_table::TOK_LBRACE; }
"}"     { return octomy_table::TOK_RBRACE; }
":"     { return octomy_table::TOK_COLON; }
";"     { return octomy_table::TOK_SEMICOLON; }
"="     { return octomy_table::TOK_ASSIGN; }

%{
/* BOOLE CONSTANT */
%}

true|false {
	yylval = Value((bool)(QString(yytext)!="false"));
	return octomy_table::TOK_BOOLCONSTANT;
}

%{
/* INTEGER CONSTANTS */
%}

{icst} {
	yylval = Value((qint32)strtol(yytext, 0, 0));
	return octomy_table::TOK_INTCONSTANT;
}

{icst}[uU] {
	yylval = Value((quint32)strtoul (yytext, 0, 0));
	return octomy_table::TOK_INTCONSTANT;
}

{icst}[uU][lL] {
	yylval = Value((quint64)strtoul(yytext, 0, 0));
	return octomy_table::TOK_INTCONSTANT;
}

{icst}[lL][uU] {
	yylval = Value((quint64)strtoul(yytext, 0, 0));
	return octomy_table::TOK_INTCONSTANT;
}


{icst}[lL] {
	yylval = Value((qint64)strtol (yytext, 0, 0));
	return octomy_table::TOK_INTCONSTANT;
}

{icst}[uU](ll|LL) {
	yylval = Value((quint64)strtoull (yytext, 0, 0));
	return octomy_table::TOK_INTCONSTANT;
}

{icst}(ll|LL) {
	yylval = Value((qint64)strtoll (yytext, 0, 0));
	return octomy_table::TOK_INTCONSTANT;
}

{icst}(ll|LL)[uU] {
	yylval = Value((quint64)strtoull (yytext, 0, 0));
	return octomy_table::TOK_INTCONSTANT;
}

%{
/* FLOAT CONSTANTS */
%}

{fract}{exp}?[flFL]? {
	yylval = Value((float)strtof (yytext, 0));
	return octomy_table::TOK_FLOATCONSTANT;
}

{digit}+{exp}[flFL]? {
	yylval = Value((float)strtof (yytext, 0));
	return octomy_table::TOK_FLOATCONSTANT;
}

0[xX]{hexfract}{binexp}[flFL]? {
	yylval = Value((float)strtof (yytext, 0));
	return octomy_table::TOK_FLOATCONSTANT;
}

0[xX]{hex}+{binexp}[flFL]? {
	yylval = Value((float)strtof (yytext, 0));
	return octomy_table::TOK_FLOATCONSTANT;
}


%{
/* STRING CONSTANTS */
%}

\"[^\"\n]*\"  {
	yylval = Value(QString(yytext));
	return octomy_table::TOK_STRINGCONSTANT;
}

\"[^\"\n]*$ {
	qWarning()<<"Unterminated string";
	yylval= Value(QString(yytext));
	return octomy_table::TOK_STRINGCONSTANT;
}

%{
/* IDENTIFIERS */
%}

[a-z][_a-z0-9]* {
	QString s=QString(yytext);
	yylval=Value( s );
	if (isTypename (s)){
		return octomy_table::TOK_TYPE_NAME;
	}
	return octomy_table::TOK_IDENTIFIER;
}
%{
/* WHITESPACE */
%}

[\n]					{ 	context.incLine(); }

[ \t\r]+				{ /* skip */ context.incToken(); }

%{
/* ERROR HANDELING FALLBACK */
%}
. { qWarning()<< "Unexpected input character '"<<QString(yytext)<<"', ignoring\n"; }

%%