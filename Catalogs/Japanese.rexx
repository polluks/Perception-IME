/**
\\	Japanese.rexx
**/
Options Results
Parse Arg ArgVec
/**/
alpha='abcdefghijklmnopqrstuvwxyz'
/**/
If Open(OFH,'Japanese-Kanji.dataset',WRITE) Then NOP;Else Exit(-1);
/**/
If Open(DBFH,'Unihan_Readings.txt',READ) Then Do While ~Eof(DBFH)
	L=ReadLn(DBFH);
	If SubStr(L,1,2)='U+' Then Do
		Parse Var L With 'U+' CodePoint '09'x  dbEntryType '09'x Vector
		Vector=Translate(Vector,'20'x,'09'x);
		B=C2B(X2C(CodePoint))
		Glyph=B2C('1110'||SubStr(B,1,4))||B2C('10'||SubStr(B,5,6))||B2C('10'||SubStr(B,11,6));
		Select
			When dbEntryType='kJapaneseKun' Then Do
				O='U+'||CodePoint||'=['||Glyph||']='||Translate(Vector,alpha,Upper(alpha))
			End;
			When dbEntryType='kJapaneseOn' Then Do
				O='U+'||CodePoint||'=['||Glyph||']='||Translate(Vector,Upper(alpha),alpha)
			End;
			OtherWise O=NULL;
		End;
		If O='NULL' Then NOP;
		Else Do;
			WriteLn(OFH,O);
			Echo CodePoint;
		End;
	End;
End;
Return;

/*
\\	Primary Activity is to generate the template datasets in the first pass
//		This is a CodePoint listing with Emitted character contained in []s following
\\		These datasets will be useful for expansion into a "reading" dataset
**
'Unicode' "Hiragana" 3040 30A0
'Unicode' "Katakana" 30A0 3100
'Unicode' "Katakana-Phonetic-Extensions" 31F0 3200
'Unicode' "Vertical-Forms" FE10 FE1A
'Unicode' "Combining-Half-Marks" FE20 FE2E
'Unicode' "Halfwidth-and-Fullwidth-Forms" FF01 FFEF
'Unicode' "Kana-Suppliment" 1B000 1B100
**
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
'Unicode' "CJK-Unified-Ideographs-Extension-B" 20000 2FA1E
**/
