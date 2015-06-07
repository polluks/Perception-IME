/**
\\	Chinese.rexx
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
			When dbEntryType='kCantonese' Then Do i=1 To Words(Vector) BY 1
				WriteReadingEntry('C '||Glyph||' '||CodePoint||' '||Word(Vector,i));
			End;
			When dbEntryType='kMandarin' Then Do i=1 To Words(Vector) BY 1
				WriteReadingEntry('M '||Glyph||' '||CodePoint||' '||Word(Vector,i));
			End;
			When dbEntryType='kTang' Then Do i=1 To Words(Vector) BY 1
				WriteReadingEntry('T '||Glyph||' '||CodePoint||' '||Word(Vector,i));
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
	fname=datadir||'/'||Variant||'/'||Selector;
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
'Unicode' "Tibetan" 0F00 0FDB
'Unicode' "Kangxi-Radicals" 2F00 2FD6
'Unicode' "Ideographic-Description" 2FF0 2FFC
'Unicode' "Bopomofo" 3105 312E
'Unicode' "Kanbun" 3190 31A0
'Unicode' "Bopomofo-Extended" 31A0 31BB
'Unicode' "Yijing-Hexagram-Symbols" 4DC0 4E00
'Unicode' "Yi-Syllables" A000 A48D
'Unicode' "Yi-Radicals" A490 A4C7
'Unicode' "Small-Form-Variants" FE50 FE6C
'Unicode' "Tai-Xuan-Jing-Symbols" 1D300 1D357
/**/
'Unicode' "CJK-Radicals-Suppliment" 2E80 2EF4
'Unicode' "CJK-Punctuation" 3000 3040
'Unicode' "CJK-Strokes" 31C0 31E4
'Unicode' "CJK-Enclosed-Letters-and-Months" 3200 32FF
'Unicode' "CJK-Compatability" 3300 3400
'Unicode' "CJK-Unified-Ideographs-Extension-A" 3400 4DB6
'Unicode' "CJK-Unified-Ideographs.0" 4E00 7700
'Unicode' "CJK-Unified-Ideographs.1" 7700 A000
'Unicode' "CJK-Compatability-Ideographs" F900 FADA
'Unicode' "CJK-Compatability-Forms" FE30 FE50
'Unicode' "Enclosed-Ideographic-Suppliment" 1F200 1F252
'Unicode' "Miscellaneous-Symbols-and-Pictographs" 1F300 1F600
/*
'Unicode' "CJK-Unified-Ideographs-Extension-B" 20000 2FA1E
*/
