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
		When Romaji='A'		Then rc='3042';
		When Romaji='I'		Then rc='3044';
		When Romaji='U'		Then rc='3046';
		When Romaji='E'		Then rc='4048';
		When Romaji='O'		Then rc='304A';
		When Romaji='KA'	Then rc='ka';
		When Romaji='KI'	Then rc='ki';
		When Romaji='KU'	Then rc='ku';
		When Romaji='KE'	Then rc='ke';
		When Romaji='KO'	Then rc='ko';
		When Romaji='KYA'	Then rc='kya';
		When Romaji='KYU'	Then rc='kyu';
		When Romaji='KYO'	Then rc='kyo';
		When Romaji='GA'	Then rc='ga';
		When Romaji='GI'	Then rc='gi';
		When Romaji='GU'	Then rc='gu';
		When Romaji='GE'	Then rc='ge';
		When Romaji='GO'	Then rc='go';
		When Romaji='GYA'	Then rc='gya';
		When Romaji='GYU'	Then rc='gyu';
		When Romaji='GYO'	Then rc='gyo';
		When Romaji='SA'	Then rc='sa';
		When Romaji='SI'	Then rc='si';
		When Romaji='SHI'	Then rc='shi';
		When Romaji='SU'	Then rc='su';
		When Romaji='SE'	Then rc='se';
		When Romaji='SO'	Then rc='so';
		When Romaji='SHA'	Then rc='sha';
		When Romaji='SYA'	Then rc='sha';
		When Romaji='SHU'	Then rc='shu';
		When Romaji='SYU'	Then rc='shu';
		When Romaji='SHO'	Then rc='sho';
		When Romaji='SYO'	Then rc='sho';
		When Romaji='ZA'	Then rc='za';
		When Romaji='JI'	Then rc='ji';
		When Romaji='ZI'	Then rc='zi';
		When Romaji='ZU'	Then rc='zu';
		When Romaji='ZE'	Then rc='ze';
		When Romaji='ZO'	Then rc='zo';
		When Romaji='JA'	Then rc='ja';
		When Romaji='JYA'	Then rc='jya';
		When Romaji='JU'	Then rc='ju';
		When Romaji='JYU'	Then rc='jyu';
		When Romaji='JE'	Then rc='je';
		When Romaji='JO'	Then rc='jo';
		When Romaji='JYO'	Then rc='jyo';
		When Romaji='DYA'	Then rc='dya';
		When Romaji='DYU'	Then rc='dyu';
		When Romaji='DYO'	Then rc='dyo';
		When Romaji='TA'	Then rc='ta';
		When Romaji='CHI'	Then rc='chi';
		When Romaji='CI'	Then rc='chi';
		When Romaji='TI'	Then rc='chi';
		When Romaji='tsu'	Then rc='tsu';
		When Romaji='tu'	Then rc='tsu';
		When Romaji='te'	Then rc='te';
		When Romaji='to'	Then rc='to';
		When Romaji='CHA'	Then rc='cha';
		When Romaji='CYA'	Then rc='cha';
		When Romaji='CHU'	Then rc='chu';
		When Romaji='CYU'	Then rc='chu';
		When Romaji='CHO'	Then rc='cho';
		When Romaji='CYO'	Then rc='cho';
		When Romaji='DA'	Then rc='da';
		When Romaji='DI'	Then rc='di';
		When Romaji='DzU'	Then rc='dzu';
		When Romaji='DU'	Then rc='du';
		When Romaji='DE'	Then rc='de';
		When Romaji='DO'	Then rc='do';
		When Romaji='NA'	Then rc='na';
		When Romaji='NI'	Then rc='ni';
		When Romaji='NU'	Then rc='nu';
		When Romaji='NE'	Then rc='ne';
		When Romaji='NO'	Then rc='no';
		When Romaji='NYA'	Then rc='nya';
		When Romaji='NYU'	Then rc='nyu';
		When Romaji='NYO'	Then rc='nyo';
		When Romaji='HA'	Then rc='ha';
		When Romaji='HI'	Then rc='hi';
		When Romaji='FU'	Then rc='fu';
		When Romaji='HU'	Then rc='hu';
		When Romaji='HE'	Then rc='he';
		When Romaji='HO'	Then rc='ho';
		When Romaji='HYA'	Then rc='hya';
		When Romaji='HYU'	Then rc='hyu';
		When Romaji='HYO'	Then rc='hyo';
		When Romaji='BA'	Then rc='ba';
		When Romaji='BI'	Then rc='bi';
		When Romaji='BU'	Then rc='bu';
		When Romaji='BE'	Then rc='be';
		When Romaji='BO'	Then rc='bo';
		When Romaji='BYA'	Then rc='bya';
		When Romaji='BYU'	Then rc='byu';
		When Romaji='BYO'	Then rc='byo';
		When Romaji='PA'	Then rc='pa';
		When Romaji='PI'	Then rc='pi';
		When Romaji='PU'	Then rc='pu';
		When Romaji='PE'	Then rc='pe';
		When Romaji='PO'	Then rc='po';
		When Romaji='PYA'	Then rc='pya';
		When Romaji='PYU'	Then rc='pyu';
		When Romaji='PYO'	Then rc='pyo';
		When Romaji='MA'	Then rc='ma';
		When Romaji='MI'	Then rc='mi';
		When Romaji='MU'	Then rc='mu';
		When Romaji='ME'	Then rc='me';
		When Romaji='MO'	Then rc='mo';
		When Romaji='MYA'	Then rc='mya';
		When Romaji='MYU'	Then rc='myu';
		When Romaji='MYO'	Then rc='myo';
		When Romaji='YA'	Then rc='ya';
		When Romaji='YU'	Then rc='yu';
		When Romaji='YO'	Then rc='yo';
		When Romaji='RA'	Then rc='ra';
		When Romaji='RI'	Then rc='ri';
		When Romaji='RU'	Then rc='ru';
		When Romaji='RE'	Then rc='re';
		When Romaji='RO'	Then rc='ro';
		When Romaji='RYA'	Then rc='rya';
		When Romaji='RYU'	Then rc='ryu';
		When Romaji='RYO'	Then rc='ryo';
		When Romaji='WA'	Then rc='wa';
		When Romaji='WI'	Then rc='wi';
		When Romaji='WE'	Then rc='we';
		When Romaji='WO'	Then rc='wo';
		When Romaji='NN'	Then rc='nn';
		When Romaji='VU'	Then rc='vu';
		When Romaji='VA'	Then rc='va';
		When Romaji='VI'	Then rc='vi';
		When Romaji='VE'	Then rc='ve';
		When Romaji='VO'	Then rc='vo';
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
