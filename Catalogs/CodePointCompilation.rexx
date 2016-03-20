/**
\\	Japanese.rexx
**/
Options Results
Parse Arg ArgVec
/**/
alpha='abcdefghijklmnopqrstuvwxyz'
datadir=Pragma(D)||'/Data'
/*
	Clear the FileSystem storage location
	and build the required "tree" arrangement

*/
/**/
'Echo' 'Compiling ...'
/*
	Some Initial Variables,  these are for a memory leak workaround and later printing of the MaximumReadingLength used
*/
MRL=1;MF=1;
/**/
If Open(DBFH,ucodedata,READ) Then Do
	Do While ~Eof(DBFH)
		L=ReadLn(DBFH);																	/* Read a line */
		If SubStr(L,1,2)='U+' Then Do 													/* Verify it is a valid entry beginning with a Codepoint Hex value */
			Parse Var L With 'U+' CodePoint '09'x  dbEntryType '09'x Vector             /* Split the valid line into sections of interest */
			Vector=Translate(Vector,'20'x,'09'x);										/* force all spaces to tabs */
			Glyph=EnCodepoint(C2D(X2C(CodePoint)));B=C2B(X2C(CodePoint));				/* Convert the Hexidecimal Codepoint into a valid UTF8 of the same CodePoint numbered Glyph */
			Select
				When dbEntryType='kJapaneseKun' Then Do i=1 To Words(Vector) BY 1		/* Iterate the Japanese Readings (Kun readings are originally Japanese and not chinese orignated ) */
					KanaPath=KanaConvert(Upper(Word(Vector,i)));						/* Convert the Reading from "romaji" ascii text to proper Japanese UTF8 as defined within this script */
					Reading='K '||CodePoint||' '||Glyph||' '||Translate(Word(Vector,i),alpha,Upper(alpha))||' '||KanaPath||' ' /* formatted arguments for the output processing function */
					If Length(Kana)>MRL Then MRL=Length(Kana);							/* update the MRL Reading Length to the current length if it is longer */
					WriteJapaneseData(Reading);										/* Call the Output Function */
				End;
				When dbEntryType='kJapaneseOn' Then Do i=1 TO Words(Vector) BY 1		/* This is a repeat of the "kJapaneseKun" for "kJapaneseOn" readings (sound readings from chinese origin) */
					KanaPath=KanaConvert(Upper(Word(Vector,i)));
					Reading='O '||CodePoint||' '||Glyph||' '||Translate(Word(Vector,i),alpha,Upper(alpha))||' '||KanaPath||' '
					If Length(Kana)>MRL Then MRL=Length(Kana);
					WriteJapaneseData(Reading);
				End;
				When dbEntryType='kMandarin' Then WriteChineseData('M '||CodePoint||' '||Glyph||' '||Vector);
				When dbEntryType='kCantonese' Then WriteChineseData('C '||CodePoint||' '||Glyph||' '||Vector);
				When dbEntryType='kHangul' Then WriteKoreanData('H '||CodePoint||' '||Glyph||' '||Vector);
				OtherWise NOP;
			End;
		End;
        If MF=15 Then Do; MF=1;
			Address COMMAND 'C:Avail >Nil: FLUSH';
		End;Else MF=1+MF;
	End;
	Close(DBFH);
End;
/**/
'Echo' 'Processing Completed,  DataBase Generated,  Now compiling Kanji Index...'
/**/
Exit(0);

WriteChineseData: PROCEDURE EXPOSE datadir
	Options Results
	Parse Arg Variant Codepoint Ideograph Vector /* Split the Argument string */
	FNAME='zh_Chinese.txt'

	CWD=Pragma(D,datadir);
	If Variant='C' Then FNAME='zh_Cantonese.txt'
	If Variant='M' Then FNAME='zh_Mandarin.txt'
	If Open(HANZIFH,FNAME,APPEND) Then Do
		WriteLn(HANZIFH,'['||Codepoint||'='||Ideograph||']'||Vector);
		Close(HANZIFH);
	End;Else If Open(HANZIFH,FNAME,WRITE) Then Do
		WriteLn(HANZIFH,'['||Codepoint||'='||Ideograph||']'||Vector);
		Close(HANZIFH);
	End;
	Pragma(D,CWD);

	return rc;

WriteJapaneseData: PROCEDURE EXPOSE datadir
	Options Results
	Parse Arg Variant Codepoint Ideograph Romaji Kana			/* Split the Argument string */

	CWD=Pragma(D,datadir||'/Kana');								/* Go to the Readings Storage Directory */
	If Open(ROMAJIFH,Romaji,APPEND) Then Do						/* If the File Exists, add a new line */
		WriteLn(ROMAJIFH,Ideograph);
		Close(ROMAJIFH);
	End;Else If Open(ROMAJIFH,Romaji,WRITE) Then Do				/* If the File Didn't exist...create it, and add the first entry */
		WriteLn(ROMAJIFH,Romaji||'='||Kana||'0A'x||Ideograph);
		Close(ROMAJIFH);
	End;
	Pragma(D,CWD);

	CWD=Pragma(D,datadir||'/Kanji');							/* Go to the Kanji Storage Directory */
	If Open(KANJIFH,CodePoint,APPEND) Then Do					/* If the File Exists, add a new line */
		WriteLn(KANJIFH,Reading||'='||Kana);
		Close(KANJIFH);
	End;Else If Open(KANJIFH,CodePoint,WRITE) Then Do			/* If the File Didn't exist...create it, and add the first entry */
		WriteLn(KANJIFH,Ideograph||'0A'x||Reading||'='||Kana);
		Close(KANJIFH);
	End;
	Pragma(D,CWD);

	return rc;

WriteKoreanData: PROCEDURE EXPOSE datadir
	Options Results
	Parse Arg Variant Codepoint Ideograph Vector /* Split the Argument string */
	FNAME='ko_Hangul.txt'

	CWD=Pragma(D,datadir);
	If Open(HANGULFH,FNAME,APPEND) Then Do
		WriteLn(HANGULFH,'['||Codepoint||'='||Ideograph||']'||Vector);
		Close(HANGULFH);
	End;Else If Open(HANGULFH,FNAME,WRITE) Then Do
		WriteLn(HANGULFH,'['||Codepoint||'='||Ideograph||']'||Vector);
		Close(HANGULFH);
	End;
	Pragma(D,CWD);

	return rc;

EnCodepoint: PROCEDURE	/* Conversion of the Input Character text assuming UTF8 output encoding */
	Options Results
	Parse Arg c			/* the Input Character as a Decimal String of the CodePoint */
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

KanaConvert: PROCEDURE /* given a proper Japanese syllable in Romaji, returns the correct UTF8 sequence of that syllable */
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
	Parse Arg Romaji ArgV	/* Input romaji is "A" "KA" or "RYU" style text sequences */
/*
    The Following are all single syllable sequences
*/
	Select
		When Romaji='A'		Then rc=X2C('E381822E');
		When Romaji='I'		Then rc=X2C('E381842E');
		When Romaji='U'		Then rc=X2C('E381862E');
		When Romaji='E'		Then rc=X2C('E381882E');
		When Romaji='O'		Then rc=X2C('E3818A2E');
		When Romaji='KA'	Then rc=X2C('E3818B2E');
		When Romaji='GA'	Then rc=X2C('E3818C2E');
		When Romaji='KI'	Then rc=X2C('E3818D2E');
		When Romaji='GI'	Then rc=X2C('E3818E2E');
		When Romaji='KU'	Then rc=X2C('E3818F2E');
		When Romaji='GU'	Then rc=X2C('E381902E');
		When Romaji='KE'	Then rc=X2C('E381912E');
		When Romaji='GE'	Then rc=X2C('E381922E');
		When Romaji='KO'	Then rc=X2C('E381932E');
		When Romaji='GO'	Then rc=X2C('E381942E');
		When Romaji='SA'	Then rc=X2C('E381952E');
		When Romaji='ZA'	Then rc=X2C('E381962E');
		When Romaji='SHI'	Then rc=X2C('E381972E');
		When Romaji='SI'	Then rc=X2C('E381972E');
		When Romaji='JI'	Then rc=X2C('E381982E');
		When Romaji='SU'	Then rc=X2C('E381992E');
		When Romaji='ZU'	Then rc=X2C('E3819A2E');
		When Romaji='SE'	Then rc=X2C('E3819B2E');
		When Romaji='ZE'	Then rc=X2C('E3819C2E');
		When Romaji='SO'	Then rc=X2C('E3819D2E');
		When Romaji='ZO'	Then rc=X2C('E3819E2E');
		When Romaji='TA'	Then rc=X2C('E3819F2E');
		When Romaji='DA'	Then rc=X2C('E381A02E');
		When Romaji='CHI'	Then rc=X2C('E381A12E');
		When Romaji='DI'	Then rc=X2C('E381A22E');
		When Romaji='tsu'	Then rc=X2C('E381A32E');
		When Romaji='TSU'	Then rc=X2C('E381A42E');
		When Romaji='DZU'	Then rc=X2C('E381A52E');
		When Romaji='TE'	Then rc=X2C('E381A62E');
		When Romaji='DE'	Then rc=X2C('E381A72E');
		When Romaji='TO'	Then rc=X2C('E381A82E');
		When Romaji='DO'	Then rc=X2C('E381A92E');
		When Romaji='NA'	Then rc=X2C('E381AA2E');
		When Romaji='NI'	Then rc=X2C('E381AB2E');
		When Romaji='NU'	Then rc=X2C('E381AC2E');
		When Romaji='NE'	Then rc=X2C('E381AD2E');
		When Romaji='NO'	Then rc=X2C('E381AE2E');
		When Romaji='HA'	Then rc=X2C('E381AF2E');
		When Romaji='BA'	Then rc=X2C('E381B02E');
		When Romaji='PA'	Then rc=X2C('E381B12E');
		When Romaji='HI'	Then rc=X2C('E381B22E');
		When Romaji='BI'	Then rc=X2C('E381B32E');
		When Romaji='PI'	Then rc=X2C('E381B42E');
		When Romaji='HU'	Then rc=X2C('E381B52E');
		When Romaji='FU'	Then rc=X2C('E381B52E');
		When Romaji='BU'	Then rc=X2C('E381B62E');
		When Romaji='PU'	Then rc=X2C('E381B72E');
		When Romaji='HE'	Then rc=X2C('E381B82E');
		When Romaji='BE'	Then rc=X2C('E381B92E');
		When Romaji='PE'	Then rc=X2C('E381BA2E');
		When Romaji='HO'	Then rc=X2C('E381BB2E');
		When Romaji='BO'	Then rc=X2C('E381BC2E');
		When Romaji='PO'	Then rc=X2C('E381BD2E');
		When Romaji='MA'	Then rc=X2C('E381BE2E');
		When Romaji='MI'	Then rc=X2C('E381BF2E');
		When Romaji='MU'	Then rc=X2C('E382802E');
		When Romaji='ME'	Then rc=X2C('E382812E');
		When Romaji='MO'	Then rc=X2C('E382822E');
		When Romaji='YA'	Then rc=X2C('E382842E');
		When Romaji='YU'	Then rc=X2C('E382862E');
		When Romaji='YO'	Then rc=X2C('E382882E');
		When Romaji='RA'	Then rc=X2C('E382892E');
		When Romaji='RI'	Then rc=X2C('E3828A2E');
		When Romaji='RU'	Then rc=X2C('E3828B2E');
		When Romaji='RE'	Then rc=X2C('E3828C2E');
		When Romaji='RO'	Then rc=X2C('E3828D2E');
		When Romaji='WA'	Then rc=X2C('E3828F2E');
		When Romaji='WI'	Then rc=X2C('E382902E');
		When Romaji='WE'	Then rc=X2C('E382912E');
		When Romaji='WO'	Then rc=X2C('E382922E');
		When Romaji='N'		Then rc=X2C('E382932E');
		When Romaji='VU'	Then rc=X2C('E382942E');
/*
	The Following are all single-syllables with two output sequences concatenated.
*/
		When Romaji='KYA'	Then rc=X2C('E3818D2EE382832E');
		When Romaji='GYA'	Then rc=X2C('E3818E2EE382832E');
		When Romaji='KYU'	Then rc=X2C('E3818D2EE382852E');
		When Romaji='GYU'	Then rc=X2C('E3818E2EE382852E');
		When Romaji='KYO'	Then rc=X2C('E3818D2EE382872E');
		When Romaji='GYO'	Then rc=X2C('E3818E2EE382872E');
		When Romaji='SHYA'	Then rc=X2C('E381972EE382832E');
		When Romaji='SHA'	Then rc=X2C('E381972EE382832E');
		When Romaji='JA'	Then rc=X2C('E381982EE382832E');
		When Romaji='SHYU'	Then rc=X2C('E381972EE382852E');
		When Romaji='SHU'	Then rc=X2C('E381972EE382852E');
		When Romaji='JU'	Then rc=X2C('E381982EE382852E');
		When Romaji='JE'	Then rc=X2C('E381982EE381872E');
		When Romaji='SHYO'	Then rc=X2C('E381972EE382872E');
		When Romaji='SHO'	Then rc=X2C('E381972EE382872E');
		When Romaji='JO'	Then rc=X2C('E381982EE382872E');
		When Romaji='CHYA'	Then rc=X2C('E381A12EE382832E');
		When Romaji='CHA'	Then rc=X2C('E381A12EE382832E');
		When Romaji='CHYU'	Then rc=X2C('E381A12EE382852E');
		When Romaji='CHU'	Then rc=X2C('E381A12EE382852E');
		When Romaji='CHYO'	Then rc=X2C('E381A12EE382872E');
		When Romaji='CHO'	Then rc=X2C('E381A12EE382872E');
		When Romaji='NYA'	Then rc=X2C('E381AB2EE382832E');
		When Romaji='NYU'	Then rc=X2C('E381AB2EE382852E');
		When Romaji='NYO'	Then rc=X2C('E381AB2EE382872E');
		When Romaji='HYA'	Then rc=X2C('E381B22EE382832E');
		When Romaji='BYA'	Then rc=X2C('E381B32EE382832E');
		When Romaji='PYA'	Then rc=X2C('E381B42EE382832E');
		When Romaji='FA'	Then rc=X2C('E381B22EE381812E');
		When Romaji='FI'	Then rc=X2C('E381B22EE381832E');
		When Romaji='FE'	Then rc=X2C('E381B22EE381872E');
		When Romaji='FO'	Then rc=X2C('E381B22EE381892E');
		When Romaji='HYU'	Then rc=X2C('E381B22EE382852E');
		When Romaji='BYU'	Then rc=X2C('E381B32EE382852E');
		When Romaji='PYU'	Then rc=X2C('E381B42EE382852E');
		When Romaji='HYO'	Then rc=X2C('E381B22EE382872E');
		When Romaji='BYO'	Then rc=X2C('E381B32EE382872E');
		When Romaji='PYO'	Then rc=X2C('E381B42EE382872E');
		When Romaji='MYA'	Then rc=X2C('E382BF2EE382832E');
		When Romaji='MYU'	Then rc=X2C('E382BF2EE382852E');
		When Romaji='MYO'	Then rc=X2C('E382BF2EE382872E');
		When Romaji='RYA'	Then rc=X2C('E3828A2EE382832E');
		When Romaji='RYU'	Then rc=X2C('E3828A2EE382852E');
		When Romaji='RYO'	Then rc=X2C('E3828A2EE382872E');
		Otherwise rc=' ';
	End;
	Return rc;

/**/
