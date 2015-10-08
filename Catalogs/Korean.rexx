/**
\\	Japanese.rexx
**/
Options Results
Parse Arg ArgVec
/**/
alpha='abcdefghijklmnopqrstuvwxyz'
ucodedata='Unihan_Readings.txt'
datadir='Japanese'
KReadLog='T:'||datadir||'-ReadingList'
/**/
Address COMMAND
'C:Makedir dummy';'C:Delete dummy '||datadir||' ALL QUIET FORCE';
'C:Makedir '||datadir||' '||datadir||'/Kanji '||datadir||'/Romaji '||datadir||'/Vocabulary '
Address
/**/
'Echo' 'Processing ...'
/**/
MRL=1;MF=1;
/**/
If Open(DBFH,ucodedata,READ) Then Do
	Do While ~Eof(DBFH)
		L=ReadLn(DBFH);
		If SubStr(L,1,2)='U+' Then Do
			Parse Var L With 'U+' CodePoint '09'x  dbEntryType '09'x Vector
			Vector=Translate(Vector,'20'x,'09'x);
			Glyph=EnCodepoint(C2D(X2C(CodePoint)));B=C2B(X2C(CodePoint));
			Select
				When dbEntryType='kHangul' Then WriteOutputEntries('M='||Vector);
				OtherWise NOP;
			End;
		End;
        If MF=11 Then Do;
			MF=1;Address COMMAND;
			'C:Avail >Nil: FLUSH';
			'C:Avail >Nil: FLUSH';
			'C:Avail >Nil: FLUSH';
			Address
		End;Else MF=1+MF;
	End;
	Close(DBFH);
End;
/**/
'Echo' 'Processing Completed,  DataBase Generated,  Now compiling Kanji Index...'
/**/
Exit(0);

WriteOutputEntries: PROCEDURE EXPOSE datadir
	Options Results
	Parse Arg Variant Codepoint Ideograph Reading Unknown
/**/
	CWD=Pragma(D,datadir||'/Hangul');
	If Open(HANGULFH,CodePoint,APPEND) Then Do
		WriteLn(HANGULFH,'='||Reading||' '||Unknown);
		Close(HANGULFH);
	End;Else If Open(HANGULFH,CodePoint,WRITE) Then Do
		WriteLn(HANGULFH,Ideograph||'0A'x||'='||Reading||' '||Unknown);
		Close(HANGULFH);
	End;
	Pragma(D,CWD);
/**/
	return rc;

EnCodepoint: PROCEDURE
	Options Results
	Parse Arg c
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
	Return cx;

/**/
