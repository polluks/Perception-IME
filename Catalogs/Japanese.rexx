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
				When dbEntryType='kJapaneseKun' Then Do i=1 To Words(Vector) BY 1
					KanaPath=KanaConvert(Upper(Word(Vector,i)));
                    CodePath=CodePointConvert(Upper(Word(Vector,i)));
					Reading='K '||CodePoint||' '||Glyph||' '||Translate(Word(Vector,i),alpha,Upper(alpha))||' '||KanaPath||' '||CodePath
					If Length(Kana)>MRL Then MRL=Length(Kana);
                    WriteOutputEntries(Reading);
					If MF=10 Then 'Echo' '@ '||CodePoint||'['||Glyph||']'
				End;
				When dbEntryType='kJapaneseOn' Then Do i=1 TO Words(Vector) BY 1
					KanaPath=KanaConvert(Upper(Word(Vector,i)));
					Reading='O '||CodePoint||' '||Glyph||' '||Translate(Word(Vector,i),alpha,Upper(alpha))||' '||KanaPath||' '
					If Length(Kana)>MRL Then MRL=Length(Kana);
                    WriteOutputEntries(Reading);
					If MF=10 Then 'Echo' '@ '||CodePoint||'['||Glyph||']'
				End;
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
	Parse Arg Variant Codepoint Ideograph Reading Kana
/**/
	CWD=Pragma(D,datadir||'/Kanji');
	If Open(KANJIFH,CodePoint,APPEND) Then Do
		WriteLn(KANJIFH,Kana||'='||Reading||'='||Codepath);
		Close(KANJIFH);
	End;Else If Open(KANJIFH,CodePoint,WRITE) Then Do
		WriteLn(KANJIFH,Ideograph||'0A'x||Kana||'='||Reading||'='||Codepath);
		Close(KANJIFH);
	End;
	Pragma(D,CWD);
	CWD=Pragma(D,datadir||'/Romaji');
	If Open(IDXFH,Reading,APPEND) Then Do
   	    WriteLn(IDXFH,Variant||' '||Codepoint||' '||Ideograph)
		Close(IDXFH)
	End;Else If Open(IDXFH,Reading,WRITE) Then Do
   	    WriteLn(IDXFH,Reading||' '||Kana||'0A'x||Variant||' '||Codepoint||' '||Ideograph);
		Close(IDXFH);
	End;
	Pragma(D,CWD);
/*
	Echo Codepoint Ideograph Variant Reading Kana Codepath
*/
	return rc;

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
	Return rc;

KanaCandidate: PROCEDURE	/* Encoded UTF8 Hiragana Sequences are output as rc */
	Options Results
	Parse Arg Romaji ArgV
	Select
		When Romaji='A'		Then rc=X2C('E381822F');
		When Romaji='I'		Then rc=X2C('E381842F');
		When Romaji='U'		Then rc=X2C('E381862F');
		When Romaji='E'		Then rc=X2C('E381882F');
		When Romaji='O'		Then rc=X2C('E3818A2F');
		When Romaji='KA'	Then rc=X2C('E3818B2F');
		When Romaji='GA'	Then rc=X2C('E3818C2F');
		When Romaji='KI'	Then rc=X2C('E3818D2F');
		When Romaji='GI'	Then rc=X2C('E3818E2F');
		When Romaji='KU'	Then rc=X2C('E3818F2F');
		When Romaji='GU'	Then rc=X2C('E381902F');
		When Romaji='KE'	Then rc=X2C('E381912F');
		When Romaji='GE'	Then rc=X2C('E381922F');
		When Romaji='KO'	Then rc=X2C('E381932F');
		When Romaji='GO'	Then rc=X2C('E381942F');
		When Romaji='SA'	Then rc=X2C('E381952F');
		When Romaji='ZA'	Then rc=X2C('E381962F');
		When Romaji='SHI'	Then rc=X2C('E381972F');
		When Romaji='SI'	Then rc=X2C('E381972F');
		When Romaji='JI'	Then rc=X2C('E381982F');
		When Romaji='SU'	Then rc=X2C('E381992F');
		When Romaji='ZU'	Then rc=X2C('E3819A2F');
		When Romaji='SE'	Then rc=X2C('E3819B2F');
		When Romaji='ZE'	Then rc=X2C('E3819C2F');
		When Romaji='SO'	Then rc=X2C('E3819D2F');
		When Romaji='ZO'	Then rc=X2C('E3819E2F');
		When Romaji='TA'	Then rc=X2C('E3819F2F');
		When Romaji='DA'	Then rc=X2C('E381A02F');
		When Romaji='CHI'	Then rc=X2C('E381A12F');
		When Romaji='DI'	Then rc=X2C('E381A22F');
		When Romaji='tsu'	Then rc=X2C('E381A32F');
		When Romaji='TSU'	Then rc=X2C('E381A42F');
		When Romaji='DZU'	Then rc=X2C('E381A52F');
		When Romaji='TE'	Then rc=X2C('E381A62F');
		When Romaji='DE'	Then rc=X2C('E381A72F');
		When Romaji='TO'	Then rc=X2C('E381A82F');
		When Romaji='DO'	Then rc=X2C('E381A92F');
		When Romaji='NA'	Then rc=X2C('E381AA2F');
		When Romaji='NI'	Then rc=X2C('E381AB2F');
		When Romaji='NU'	Then rc=X2C('E381AC2F');
		When Romaji='NE'	Then rc=X2C('E381AD2F');
		When Romaji='NO'	Then rc=X2C('E381AE2F');
		When Romaji='HA'	Then rc=X2C('E381AF2F');
		When Romaji='BA'	Then rc=X2C('E381B02F');
		When Romaji='PA'	Then rc=X2C('E381B12F');
		When Romaji='HI'	Then rc=X2C('E381B22F');
		When Romaji='BI'	Then rc=X2C('E381B32F');
		When Romaji='PI'	Then rc=X2C('E381B42F');
		When Romaji='HU'	Then rc=X2C('E381B52F');
		When Romaji='FU'	Then rc=X2C('E381B52F');
		When Romaji='BU'	Then rc=X2C('E381B62F');
		When Romaji='PU'	Then rc=X2C('E381B72F');
		When Romaji='HE'	Then rc=X2C('E381B82F');
		When Romaji='BE'	Then rc=X2C('E381B92F');
		When Romaji='PE'	Then rc=X2C('E381BA2F');
		When Romaji='HO'	Then rc=X2C('E381BB2F');
		When Romaji='BO'	Then rc=X2C('E381BC2F');
		When Romaji='PO'	Then rc=X2C('E381BD2F');
		When Romaji='MA'	Then rc=X2C('E381BE2F');
		When Romaji='MI'	Then rc=X2C('E381BF2F');
		When Romaji='MU'	Then rc=X2C('E382802F');
		When Romaji='ME'	Then rc=X2C('E382812F');
		When Romaji='MO'	Then rc=X2C('E382822F');
		When Romaji='YA'	Then rc=X2C('E382842F');
		When Romaji='YU'	Then rc=X2C('E382862F');
		When Romaji='YO'	Then rc=X2C('E382882F');
		When Romaji='RA'	Then rc=X2C('E382892F');
		When Romaji='RI'	Then rc=X2C('E3828A2F');
		When Romaji='RU'	Then rc=X2C('E3828B2F');
		When Romaji='RE'	Then rc=X2C('E3828C2F');
		When Romaji='RO'	Then rc=X2C('E3828D2F');
		When Romaji='WA'	Then rc=X2C('E3828F2F');
		When Romaji='WI'	Then rc=X2C('E382902F');
		When Romaji='WE'	Then rc=X2C('E382912F');
		When Romaji='WO'	Then rc=X2C('E382922F');
		When Romaji='N'		Then rc=X2C('E382932F');
		When Romaji='VU'	Then rc=X2C('E382942F');
/**/
		When Romaji='KYA'	Then rc=X2C('E3818DE382832F');
		When Romaji='GYA'	Then rc=X2C('E3818EE382832F');
		When Romaji='KYU'	Then rc=X2C('E3818DE382852F');
		When Romaji='GYU'	Then rc=X2C('E3818EE382852F');
		When Romaji='KYO'	Then rc=X2C('E3818DE382872F');
		When Romaji='GYO'	Then rc=X2C('E3818EE382872F');
		When Romaji='SHYA'	Then rc=X2C('E38197E382832F');
		When Romaji='SHA'	Then rc=X2C('E38197E382832F');
		When Romaji='JA'	Then rc=X2C('E38198E382832F');
		When Romaji='SHYU'	Then rc=X2C('E38197E382852F');
		When Romaji='SHU'	Then rc=X2C('E38197E382852F');
		When Romaji='JU'	Then rc=X2C('E38198E382852F');
		When Romaji='JE'	Then rc=X2C('E38198E381872F');
		When Romaji='SHYO'	Then rc=X2C('E38197E382872F');
		When Romaji='SHO'	Then rc=X2C('E38197E382872F');
		When Romaji='JO'	Then rc=X2C('E38198E382872F');
		When Romaji='CHYA'	Then rc=X2C('E381A1E382832F');
		When Romaji='CHA'	Then rc=X2C('E381A1E382832F');
		When Romaji='CHYU'	Then rc=X2C('E381A1E382852F');
		When Romaji='CHU'	Then rc=X2C('E381A1E382852F');
		When Romaji='CHYO'	Then rc=X2C('E381A1E382872F');
		When Romaji='CHO'	Then rc=X2C('E381A1E382872F');
		When Romaji='NYA'	Then rc=X2C('E381ABE382832F');
		When Romaji='NYU'	Then rc=X2C('E381ABE382852F');
		When Romaji='NYO'	Then rc=X2C('E381ABE382872F');
		When Romaji='HYA'	Then rc=X2C('E381B2E382832F');
		When Romaji='BYA'	Then rc=X2C('E381B3E382832F');
		When Romaji='PYA'	Then rc=X2C('E381B4E382832F');
		When Romaji='FA'	Then rc=X2C('E381B2E381812F');
		When Romaji='FI'	Then rc=X2C('E381B2E381832F');
		When Romaji='FE'	Then rc=X2C('E381B2E381872F');
		When Romaji='FO'	Then rc=X2C('E381B2E381892F');
		When Romaji='HYU'	Then rc=X2C('E381B2E382852F');
		When Romaji='BYU'	Then rc=X2C('E381B3E382852F');
		When Romaji='PYU'	Then rc=X2C('E381B4E382852F');
		When Romaji='HYO'	Then rc=X2C('E381B2E382872F');
		When Romaji='BYO'	Then rc=X2C('E381B3E382872F');
		When Romaji='PYO'	Then rc=X2C('E381B4E382872F');
		When Romaji='MYA'	Then rc=X2C('E382BFE382832F');
		When Romaji='MYU'	Then rc=X2C('E382BFE382852F');
		When Romaji='MYO'	Then rc=X2C('E382BFE382872F');
		When Romaji='RYA'	Then rc=X2C('E3828AE382832F');
		When Romaji='RYU'	Then rc=X2C('E3828AE382852F');
		When Romaji='RYO'	Then rc=X2C('E3828AE382872F');
		Otherwise rc=' ';
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
