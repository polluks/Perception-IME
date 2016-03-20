/**
\\	Japanese.rexx
**/
Options Results
Parse Arg ArgVec
/**/
datadir=Pragma(D)||'/Data'
KanjiDir=datadir||'/Kanji'
/**/
'Echo' 'Compiling ...'

rmin='37AD';rmax='3FFFF'

Do c=X2D(rmin) while c<X2D(rmax)
	FNAME=KanjiDir||'/'||D2X(c);
	If Open(DFH,FNAME) Then Do
		Echo FNAME;
		Close(DFH);
	End;
End;
/**/
'Echo' 'Processing Completed,  Kanji Index Compiled'
/**/
Exit(0);

ProcessReading: PROCEDURE	/* Given a reading,  convert the input file into a tree entry */
	Options Results
	Parse Arg Keyword'='Ideographic
	rc=0;
	Return rc;

EnCodepoint: PROCEDURE		/* Conversion of the Input Character text assuming UTF8 output encoding */
	Options Results
	Parse Arg c				/* the Input Character as a Decimal String of the CodePoint */
/*
	The following "Select" works like "switch()" in C language

	ARexx has extensive text processing functionality but is also showing bitrot on newer systems.
		that is why the use of X2D() B2C() C2B() functions where B is Binary, D is Decimal and X is Hex
		SubStr() is to retrieve part of a larger string returning the given parts of the string
*/
	Select
		When c<X2D('80') Then cx=D2C(c);
		When c<X2D('100') Then cx=B2C('110000'||SubStr(C2B(D2C(c)),1,2))||B2C('10'||SubStr(C2B(D2C(c)),3,6));
		When c<X2D('800') Then cx=B2C('110'||SubStr(C2B(SubStr(D2C(c),1,1)),6,3)||SubStr(C2B(SubStr(D2C(c),2,1)),1,2))||B2C('10'||SubStr(C2B(SubStr(D2C(c),2,1)),3,6))
		When c<X2D('10000') Then cx=B2C('1110'||SubStr(C2B(SubStr(D2C(c),1,1)),1,4))||B2C('10'||SubStr(C2B(SubStr(D2C(c),1,1)),5,4)||SubStr(C2B(SubStr(D2C(c),2,1)),1,2))||B2C('10'||SubStr(C2B(SubStr(D2C(c),2,1)),3,6))
		When c<X2D('200000') Then If Length(D2C(c))=4 Then Do
				cx=B2C('11110'||SubStr(C2B(SubStr(D2C(c),1,1)),6,3)))||B2C('10'||SubStr(C2B(SubStr(D2C(c),2,1)),1,6))||B2C('10'||SubStr(C2B(SubStr(D2C(c),2,1)),7,2)||SubStr(C2B(SubStr(D2C(c),3,1)),1,4))||B2C('10'||SubStr(C2B(SubStr(D2C(c),3,1)),5,4)||SubStr(C2B(SubStr(D2C(c),4,1)),1,2))||B2C('10'||SubStr(C2B(SubStr(D2C(c),4,1)),3,6))
			End;Else If Length(D2C(c))=3 Then Do
				cx=B2C('11110'||SubStr(C2B(SubStr(D2C(c),1,1)),4,3))||B2C('10'||SubStr(C2B(SubStr(D2C(c),1,1)),7,2)||SubStr(C2B(SubStr(D2C(c),2,1)),1,4))||B2C('10'||SubStr(C2B(SubStr(D2C(c),2,1)),5,4)||SubStr(C2B(SubStr(D2C(c),3,1)),1,2))||B2C('10'||SubStr(C2B(SubStr(D2C(c),3,1)),3,6))
			End;
		Otherwise Return('##');
	End;
/*	the cx returned here is a properly encoded UTF8 Character Sequence
*/
	Return cx;

/**/
