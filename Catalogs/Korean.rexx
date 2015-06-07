/**
\\	Korean.rexx
**/
Options Results
Parse Arg ArgVec
/**/
alpha='abcdefghijklmnopqrstuvwxyz'
xmldata='Unihan_Readings.txt'
/**/
datadir='Chinese'
/**/
If Open(DBFH,xmldata,READ) Then Do While ~Eof(DBFH)
	L=ReadLn(DBFH);
	If SubStr(L,1,2)='U+' Then Do
		Parse Var L With 'U+' CodePoint '09'x  dbEntryType '09'x Vector
		Vector=Translate(Vector,'20'x,'09'x);
		B=C2B(X2C(CodePoint))
		Select
			When X2D(CodePoint) < X2D('80') Then Glyph=X2C(B)
			When X2D(CodePoint) < X2D('100') Then Glyph=B2C('110000'||SubStr(B,1,2))||B2C('10'||SubStr(B,3,6));
			When X2D(CodePoint) < X2D('800') Then Glyph=B2C('110'||SubStr(B,1,1)||'10'||SubStr(B,2,1)||'10'||SubStr(B,3,1));
			When X2D(CodePoint) < X2D('10000') Then	Glyph=B2C('1110'||SubStr(B,1,4)||'10'||SubStr(B,5,6)||'10'||SubStr(B,11,6));
/*
	The Following two conditions of Unicode need better encoding support from the input Hex length.

    Run the script as it exists to see the issue at the end of the current ouput.

			When X2D(CodePoint) < X2D('200000') Then Glyph=B2C(
'11110'||SubStr(4,3)||
'10'||SubStr(B,7,6)||
'10'||SubStr(B,13,6)||
'10'||SubStr(B,19,6));

			When X2D(CodePoint) < X2D('1000000') Then Glyph=C2B(
'111110'||SubStr(B,,2)||
'10'||SubStr(B,,6)||
'10'||SubStr(B,,6)||
'10'||SubStr(B,,6)||
'10'||SubStr(B,,6));
*/
			Otherwise Glyph='#'||C2B(X2C(CodePoint))||'#';
		End;
		Select
			When dbEntryType='kKorean' Then Do i=1 To Words(Vector) BY 1
				WriteReadingEntry('- '||Glyph||' '||CodePoint||' '||Word(Vector,i));
			End;
			OtherWise NOP;
		End;
		Close(DBFH);
	End;
End;
/*
	Dataset is filtered to readings...now to rebuild into a central index
*/
Return;

WriteReadingEntry: PROCEDURE EXPOSE datadir
	Options Results
	Parse Arg Variant Ideograph CodePoint Selector ArgVec
	fname=datadir||'/'||Selector;
	Echo fname
	If Open(IDXFH,fname,APPEND) Then Do
        WriteLn(IDXFH,Ideograph||' '||CodePoint)
		Close(IDXFH)
	End;Else If Open(IDXFH,fname,WRITE) Then Do
        WriteLn(IDXFH,Ideograph||' '||CodePoint)
		Close(IDXFH)
	End;
	return rc;

/*
\\	Primary Activity is to generate the template datasets in the first pass
//		This is a CodePoint listing with Emitted character contained in []s following
\\		These datasets will be useful for expansion into a "reading" dataset
*/
'Unicode' "Hangul-Jamo" 1100 1200
'Unicode' "Hangul-Compatability-Jamo" 3131 318F
'Unicode' "Hangul-Jamo-Extended-A" A960 A97D
'Unicode' "Hangul-Syllables" AC00 D7AF
'Unicode' "Hangul-Jamo-Extended-B" D7B0 D7FC
