/**
\\	Japanese.rexx
**/
Options Results
Parse Arg ArgVec
/**/
alpha='abcdefghijklmnopqrstuvwxyz'
/**/
If Open(DBFH,'Unihan_Readings.txt',READ) Then Do While ~Eof(DBFH)
	L=ReadLn(DBFH);
	If SubStr(L,1,2)='U+' Then Do
		Parse Var L With 'U+' CodePoint '09'x  dbEntryType '09'x Vector
		Vector=Translate(Vector,'20'x,'09'x);
		B=C2B(X2C(CodePoint))
		Select
			When X2D(CodePoint) < X2D('80') Then Glyph=X2C(B)
/*			When X2D(CodePoint) < X2D('100') Then Glyph=B2C('110000'||SubStr(B,1,2))||B2C('10'||SubStr(B,3,6));*/
/*			When X2D(CodePoint) < X2D('800') Then Glyph=B2C('110'||SubStr(B,1,1)||'10'||SubStr(B,2,1)||'10'||SubStr(B,3,1));*/
			When X2D(CodePoint) < X2D('10000') Then	Glyph=B2C('1110'||SubStr(B,1,4)||'10'||SubStr(B,5,6)||'10'||SubStr(B,11,6));
			When X2D(CodePoint) < X2D('200000') Then Glyph=B2C('11110'||SubStr(B,1,3)||'100'||SubStr(B,4,5)||'10'||SubStr(B,9,6)||'10'||SubStr(B,14,6)||'10'||SubStr(B,20,6)||'10'||SubStr(B,27,6));
			Otherwise Glyph=' ';
		End;
		Select
			When dbEntryType='kJapaneseKun' Then Do i=1 To Words(Vector) BY 1
				Echo 'U='||CodePoint||'=['||C2X(Glyph)||']='||KanaConvert(Upper(Word(Vector,i))||' Hiragana');
			End;
			When dbEntryType='kJapaneseOn' Then Do i=1 TO Words(Vector) BY 1
				Echo 'U='||CodePoint||'=['||C2X(Glyph)||']='||KanaConvert(Upper(Word(Vector,i))||' Katakana');
			End;
			OtherWise NOP;
		End;
	End;
End;
Return;

KanaConvert: PROCEDURE
	Options Results
	Parse Arg Reading Type ArgV
	rc='';Syllable='';Do i=1 To Length(Reading) By 1;
        c=SubStr(Reading,i,1);Syllable=Syllable||c;
		Select
			When c='A' Then Do; rc=rc||KanaCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='I' Then Do; rc=rc||KanaCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='U' Then Do; rc=rc||KanaCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='E' Then Do; rc=rc||KanaCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='O' Then Do; rc=rc||KanaCandidate(Syllable||' '||Type); Syllable=''; End;
			Otherwise NOP;
		End;
	End;
	Return rc

KanaCandidate: PROCEDURE
	Options Results
	Parse Arg Romaji Key
	Select
		When Key='Hiragana' Then Transform=0;
		When Key='Katakana' Then Transform=1;
		Otherwise NOP;
	End;
	Select
		When Romaji='A'		Then rc=X2C('');
		When Romaji='I'		Then rc=X2C('');
		When Romaji='U'		Then rc=X2C('');
		When Romaji='E'		Then rc=X2C('');
		When Romaji='O'		Then rc=X2C('');
		When Romaji='KA'	Then rc=X2C('');
		When Romaji='KI'	Then rc=X2C('');
		When Romaji='KU'	Then rc=X2C('');
		When Romaji='KE'	Then rc=X2C('');
		When Romaji='KO'	Then rc=X2C('');
		When Romaji='KYA'	Then rc=X2C('');
		When Romaji='KYU'	Then rc=X2C('');
		When Romaji='KYO'	Then rc=X2C('');
		When Romaji='GA'	Then rc=X2C('');
		When Romaji='GI'	Then rc=X2C('');
		When Romaji='GU'	Then rc=X2C('');
		When Romaji='GE'	Then rc=X2C('');
		When Romaji='GO'	Then rc=X2C('');
		When Romaji='GYA'	Then rc=X2C('');
		When Romaji='GYU'	Then rc=X2C('');
		When Romaji='GYO'	Then rc=X2C('');
		When Romaji='SA'	Then rc=X2C('');
		When Romaji='SI'	Then rc=X2C('');
		When Romaji='SHI'	Then rc=X2C('');
		When Romaji='SU'	Then rc=X2C('');
		When Romaji='SE'	Then rc=X2C('');
		When Romaji='SO'	Then rc=X2C('');
		When Romaji='SHA'	Then rc=X2C('');
		When Romaji='SYA'	Then rc=X2C('');
		When Romaji='SHU'	Then rc=X2C('');
		When Romaji='SYU'	Then rc=X2C('');
		When Romaji='SHO'	Then rc=X2C('');
		When Romaji='SYO'	Then rc=X2C('');
		When Romaji='ZA'	Then rc=X2C('');
		When Romaji='JI'	Then rc=X2C('');
		When Romaji='ZI'	Then rc=X2C('');
		When Romaji='ZU'	Then rc=X2C('');
		When Romaji='ZE'	Then rc=X2C('');
		When Romaji='ZO'	Then rc=X2C('');
		When Romaji='JA'	Then rc=X2C('');
		When Romaji='JYA'	Then rc=X2C('');
		When Romaji='JU'	Then rc=X2C('');
		When Romaji='JYU'	Then rc=X2C('');
		When Romaji='JE'	Then rc=X2C('');
		When Romaji='JO'	Then rc=X2C('');
		When Romaji='JYO'	Then rc=X2C('');
		When Romaji='DYA'	Then rc=X2C('');
		When Romaji='DYU'	Then rc=X2C('');
		When Romaji='DYO'	Then rc=X2C('');
		When Romaji='TA'	Then rc=X2C('');
		When Romaji='CHI'	Then rc=X2C('');
		When Romaji='CI'	Then rc=X2C('');
		When Romaji='TI'	Then rc=X2C('');
		When Romaji='tsu'	Then rc=X2C('');
		When Romaji='tu'	Then rc=X2C('');
		When Romaji='te'	Then rc=X2C('');
		When Romaji='to'	Then rc=X2C('');
		When Romaji='CHA'	Then rc=X2C('');
		When Romaji='CYA'	Then rc=X2C('');
		When Romaji='CHU'	Then rc=X2C('');
		When Romaji='CYU'	Then rc=X2C('');
		When Romaji='CHO'	Then rc=X2C('');
		When Romaji='CYO'	Then rc=X2C('');
		When Romaji='DA'	Then rc=X2C('');
		When Romaji='DI'	Then rc=X2C('');
		When Romaji='DzU'	Then rc=X2C('');
		When Romaji='DU'	Then rc=X2C('');
		When Romaji='DE'	Then rc=X2C('');
		When Romaji='DO'	Then rc=X2C('');
		When Romaji='NA'	Then rc=X2C('');
		When Romaji='NI'	Then rc=X2C('');
		When Romaji='NU'	Then rc=X2C('');
		When Romaji='NE'	Then rc=X2C('');
		When Romaji='NO'	Then rc=X2C('');
		When Romaji='NYA'	Then rc=X2C('');
		When Romaji='NYU'	Then rc=X2C('');
		When Romaji='NYO'	Then rc=X2C('');
		When Romaji='HA'	Then rc=X2C('');
		When Romaji='HI'	Then rc=X2C('');
		When Romaji='FU'	Then rc=X2C('');
		When Romaji='HU'	Then rc=X2C('');
		When Romaji='HE'	Then rc=X2C('');
		When Romaji='HO'	Then rc=X2C('');
		When Romaji='HYA'	Then rc=X2C('');
		When Romaji='HYU'	Then rc=X2C('');
		When Romaji='HYO'	Then rc=X2C('');
		When Romaji='BA'	Then rc=X2C('');
		When Romaji='BI'	Then rc=X2C('');
		When Romaji='BU'	Then rc=X2C('');
		When Romaji='BE'	Then rc=X2C('');
		When Romaji='BO'	Then rc=X2C('');
		When Romaji='BYA'	Then rc=X2C('');
		When Romaji='BYU'	Then rc=X2C('');
		When Romaji='BYO'	Then rc=X2C('');
		When Romaji='PA'	Then rc=X2C('');
		When Romaji='PI'	Then rc=X2C('');
		When Romaji='PU'	Then rc=X2C('');
		When Romaji='PE'	Then rc=X2C('');
		When Romaji='PO'	Then rc=X2C('');
		When Romaji='PYA'	Then rc=X2C('');
		When Romaji='PYU'	Then rc=X2C('');
		When Romaji='PYO'	Then rc=X2C('');
		When Romaji='MA'	Then rc=X2C('');
		When Romaji='MI'	Then rc=X2C('');
		When Romaji='MU'	Then rc=X2C('');
		When Romaji='ME'	Then rc=X2C('');
		When Romaji='MO'	Then rc=X2C('');
		When Romaji='MYA'	Then rc=X2C('');
		When Romaji='MYU'	Then rc=X2C('');
		When Romaji='MYO'	Then rc=X2C('');
		When Romaji='YA'	Then rc=X2C('');
		When Romaji='YU'	Then rc=X2C('');
		When Romaji='YO'	Then rc=X2C('');
		When Romaji='RA'	Then rc=X2C('');
		When Romaji='RI'	Then rc=X2C('');
		When Romaji='RU'	Then rc=X2C('');
		When Romaji='RE'	Then rc=X2C('');
		When Romaji='RO'	Then rc=X2C('');
		When Romaji='RYA'	Then rc=X2C('');
		When Romaji='RYU'	Then rc=X2C('');
		When Romaji='RYO'	Then rc=X2C('');
		When Romaji='WA'	Then rc=X2C('');
		When Romaji='WI'	Then rc=X2C('');
		When Romaji='WE'	Then rc=X2C('');
		When Romaji='WO'	Then rc=X2C('');
		When Romaji='NN'	Then rc=X2C('');
		When Romaji='VU'	Then rc=X2C('');
		When Romaji='VA'	Then rc=X2C('');
		When Romaji='VI'	Then rc=X2C('');
		When Romaji='VE'	Then rc=X2C('');
		When Romaji='VO'	Then rc=X2C('');
		Otherwise rc='';
	End;
	If Transform Then Do
        NOP
	End;
	Return rc;
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
