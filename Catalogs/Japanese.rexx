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
			When X2D(CodePoint) < X2D('100') Then Glyph=B2C('110000'||SubStr(B,1,2))||B2C('10'||SubStr(B,3,6));
			When X2D(CodePoint) < X2D('800') Then Glyph=B2C('110'||SubStr(B,1,1)||'10'||SubStr(B,2,1)||'10'||SubStr(B,3,1));
			When X2D(CodePoint) < X2D('10000') Then	Glyph=B2C('1110'||SubStr(B,1,4)||'10'||SubStr(B,5,6)||'10'||SubStr(B,11,6));
/*			When X2D(CodePoint) < X2D('200000') Then Glyph=B2C('11110'||SubStr(4,3)||'10'||SubStr(B,7,6)||'10'||SubStr(B,13,6)||'10'||SubStr(B,19,6));
			When X2D(CodePoint) < X2D('1000000') Then Glyph=C2B(
'111110'||SubStr(B,,2)||'10'||SubStr(B,,6)||'10'||SubStr(B,,6)||
'10'||SubStr(B,,6)||'10'||SubStr(B,,6)||'10'||SubStr(B,,6));
*/
			Otherwise Glyph='#'||C2B(X2C(CodePoint))||'#';
		End;
		Select
			When dbEntryType='kJapaneseKun' Then Do i=1 To Words(Vector) BY 1
				Echo 'U='||CodePoint||'['||Glyph||']='||Translate(Word(Vector,i),alpha,Upper(alpha))||'['||KanaConvert(Upper(Word(Vector,i))||' Hiragana')||']';
			End;
			When dbEntryType='kJapaneseOn' Then Do i=1 TO Words(Vector) BY 1
				Echo 'U='||CodePoint||'['||Glyph||']='||Translate(Word(Vector,i),alpha,Upper(alpha))||'['||KanaConvert(Upper(Word(Vector,i))||' Katakana')||']';
			End;
			OtherWise NOP;
		End;
	End;
End;
Return;

KanaConvert: PROCEDURE
	Options Results
	Parse Arg Reading Type ArgV
	rc='';Syllable='';
	Do i=1 To Length(Reading) By 1;
        c=SubStr(Reading,i,1);cx=SubStr(Reading,i+1,1);
		If Syllable=c Then rc=rc||KanaCandidate('tsu'||' '||Type);Else Syllable=Syllable||c;
		Select
			When c='A' Then Do; rc=rc||KanaCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='I' Then Do; rc=rc||KanaCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='U' Then Do; rc=rc||KanaCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='E' Then Do; rc=rc||KanaCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='O' Then Do; rc=rc||KanaCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='N' Then Select;
				When cx='A' Then NOP;
				When cx='I' Then NOP;
				When cx='U' Then NOP;
				When cx='E' Then NOP;
				When cx='O' Then NOP;
				Otherwise rc=rc||KanaCandidate(Syllable||' '||Type); Syllable='';
			End;
			Otherwise NOP;
		End;
	End;
	Return rc

KanaCandidate: PROCEDURE	/* Encoded UTF8 Hiragana Sequences are output as rc */
	Options Results
	Parse Arg Romaji Key
/*
	If Key='Hiragana' Then Transform=0;
	If Key='Katakana' Then Transform=1;
*/
	Select
		When Romaji='A'		Then rc=X2C('E38182');
		When Romaji='I'		Then rc=X2C('E38184');
		When Romaji='U'		Then rc=X2C('E38186');
		When Romaji='E'		Then rc=X2C('E38188');
		When Romaji='O'		Then rc=X2C('E3818A');
		When Romaji='KA'	Then rc=X2C('E3818B');
		When Romaji='GA'	Then rc=X2C('E3818C');
		When Romaji='KI'	Then rc=X2C('E3818D');
		When Romaji='GI'	Then rc=X2C('E3818E');
		When Romaji='KU'	Then rc=X2C('E3818F');
		When Romaji='GU'	Then rc=X2C('E38190');
		When Romaji='KE'	Then rc=X2C('E38191');
		When Romaji='GE'	Then rc=X2C('E38192');
		When Romaji='KO'	Then rc=X2C('E38193');
		When Romaji='GO'	Then rc=X2C('E38194');
		When Romaji='SA'	Then rc=X2C('E38195');
		When Romaji='ZA'	Then rc=X2C('E38196');
		When Romaji='SHI'	Then rc=X2C('E38197');
		When Romaji='SI'	Then rc=X2C('E38197');
		When Romaji='JI'	Then rc=X2C('E38198');
		When Romaji='SU'	Then rc=X2C('E38199');
		When Romaji='ZU'	Then rc=X2C('E3819A');
		When Romaji='SE'	Then rc=X2C('E3819B');
		When Romaji='ZE'	Then rc=X2C('E3819C');
		When Romaji='SO'	Then rc=X2C('E3819D');
		When Romaji='ZO'	Then rc=X2C('E3819E');
		When Romaji='TA'	Then rc=X2C('E3819F');
		When Romaji='DA'	Then rc=X2C('E381A0');
		When Romaji='CHI'	Then rc=X2C('E381A1');
		When Romaji='DI'	Then rc=X2C('E381A2');
		When Romaji='tsu'	Then rc=X2C('E381A3');
		When Romaji='TSU'	Then rc=X2C('E381A4');
		When Romaji='DZU'	Then rc=X2C('E381A5');
		When Romaji='TE'	Then rc=X2C('E381A6');
		When Romaji='DE'	Then rc=X2C('E381A7');
		When Romaji='TO'	Then rc=X2C('E381A8');
		When Romaji='DO'	Then rc=X2C('E381A9');
		When Romaji='NA'	Then rc=X2C('E381AA');
		When Romaji='NI'	Then rc=X2C('E381AB');
		When Romaji='NU'	Then rc=X2C('E381AC');
		When Romaji='NE'	Then rc=X2C('E381AD');
		When Romaji='NO'	Then rc=X2C('E381AE');
		When Romaji='HA'	Then rc=X2C('E381AF');
		When Romaji='BA'	Then rc=X2C('E381B0');
		When Romaji='PA'	Then rc=X2C('E381B1');
		When Romaji='HI'	Then rc=X2C('E381B2');
		When Romaji='BI'	Then rc=X2C('E381B3');
		When Romaji='PI'	Then rc=X2C('E381B4');
		When Romaji='HU'	Then rc=X2C('E381B5');
		When Romaji='FU'	Then rc=X2C('E381B5');
		When Romaji='BU'	Then rc=X2C('E381B6');
		When Romaji='PU'	Then rc=X2C('E381B7');
		When Romaji='HE'	Then rc=X2C('E381B8');
		When Romaji='BE'	Then rc=X2C('E381B9');
		When Romaji='PE'	Then rc=X2C('E381BA');
		When Romaji='HO'	Then rc=X2C('E381BB');
		When Romaji='BO'	Then rc=X2C('E381BC');
		When Romaji='PO'	Then rc=X2C('E381BD');
		When Romaji='MA'	Then rc=X2C('E381BE');
		When Romaji='MI'	Then rc=X2C('E381BF');
		When Romaji='MU'	Then rc=X2C('E38280');
		When Romaji='ME'	Then rc=X2C('E38281');
		When Romaji='MO'	Then rc=X2C('E38282');
		When Romaji='YA'	Then rc=X2C('E38284');
		When Romaji='YU'	Then rc=X2C('E38286');
		When Romaji='YO'	Then rc=X2C('E38288');
		When Romaji='RA'	Then rc=X2C('E38289');
		When Romaji='RI'	Then rc=X2C('E3828A');
		When Romaji='RU'	Then rc=X2C('E3828B');
		When Romaji='RE'	Then rc=X2C('E3828C');
		When Romaji='RO'	Then rc=X2C('E3828D');
		When Romaji='WA'	Then rc=X2C('E3828F');
		When Romaji='WI'	Then rc=X2C('E38290');
		When Romaji='WE'	Then rc=X2C('E38291');
		When Romaji='WO'	Then rc=X2C('E38292');
		When Romaji='N'		Then rc=X2C('E38293');
		When Romaji='VU'	Then rc=X2C('E38294');
		When Romaji='KYA'	Then rc=X2C('E3818DE38283');
		When Romaji='GYA'	Then rc=X2C('E3818EE38283');
		When Romaji='KYU'	Then rc=X2C('E3818DE38285');
		When Romaji='GYU'	Then rc=X2C('E3818EE38285');
		When Romaji='KYO'	Then rc=X2C('E3818DE38287');
		When Romaji='GYO'	Then rc=X2C('E3818EE38287');
		When Romaji='SHYA'	Then rc=X2C('E38197E38283');
		When Romaji='SHA'	Then rc=X2C('E38197E38283');
		When Romaji='JA'	Then rc=X2C('E38198E38283');
		When Romaji='SHYU'	Then rc=X2C('E38197E38285');
		When Romaji='SHU'	Then rc=X2C('E38197E38285');
		When Romaji='JU'	Then rc=X2C('E38198E38285');
		When Romaji='JE'	Then rc=X2C('E38198E38187');
		When Romaji='SHYO'	Then rc=X2C('E38197E38287');
		When Romaji='SHO'	Then rc=X2C('E38197E38287');
		When Romaji='JO'	Then rc=X2C('E38198E38287');
		When Romaji='CHYA'	Then rc=X2C('E381A1E38283');
		When Romaji='CHA'	Then rc=X2C('E381A1E38283');
		When Romaji='CHYU'	Then rc=X2C('E381A1E38285');
		When Romaji='CHU'	Then rc=X2C('E381A1E38285');
		When Romaji='CHYO'	Then rc=X2C('E381A1E38287');
		When Romaji='CHO'	Then rc=X2C('E381A1E38287');
		When Romaji='NYA'	Then rc=X2C('E381ABE38283');
		When Romaji='NYU'	Then rc=X2C('E381ABE38285');
		When Romaji='NYO'	Then rc=X2C('E381ABE38287');
		When Romaji='HYA'	Then rc=X2C('E381B2E38283');
		When Romaji='BYA'	Then rc=X2C('E381B3E38283');
		When Romaji='PYA'	Then rc=X2C('E381B4E38283');
		When Romaji='FA'	Then rc=X2C('E381B2E38181');
		When Romaji='FI'	Then rc=X2C('E381B2E38183');
		When Romaji='FE'	Then rc=X2C('E381B2E38187');
		When Romaji='FO'	Then rc=X2C('E381B2E38189');
		When Romaji='BYU'	Then rc=X2C('E381B3E38285');
		When Romaji='PYU'	Then rc=X2C('E381B4E38285');
		When Romaji='HYO'	Then rc=X2C('E381B2E38287');
		When Romaji='BYO'	Then rc=X2C('E381B3E38287');
		When Romaji='PYO'	Then rc=X2C('E381B4E38287');
		When Romaji='MYA'	Then rc=X2C('E382BFE38283');
		When Romaji='MYU'	Then rc=X2C('E382BFE38285');
		When Romaji='MYO'	Then rc=X2C('E382BFE38287');
		When Romaji='RYA'	Then rc=X2C('E3828AE38283');
		When Romaji='RYU'	Then rc=X2C('E3828AE38285');
		When Romaji='RYO'	Then rc=X2C('E3828AE38287');
		Otherwise rc=' @@ ';
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
