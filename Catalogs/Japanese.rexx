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
'C:Makedir '||datadir||' '||datadir||'/Kanji '||datadir||'/Romaji '||datadir||'/Reading '
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
				End;
				When dbEntryType='kJapaneseOn' Then Do i=1 TO Words(Vector) BY 1
					KanaPath=KanaConvert(Upper(Word(Vector,i)));
                    CodePath=CodePointConvert(Upper(Word(Vector,i)));
					Reading='O '||CodePoint||' '||Glyph||' '||Translate(Word(Vector,i),alpha,Upper(alpha))||' '||KanaPath||' '||CodePath
					If Length(Kana)>MRL Then MRL=Length(Kana);
                    WriteOutputEntries(Reading);
				End;
				OtherWise NOP;
			End;
		End;
        If MF=20 Then Do
			MF=1;Address COMMAND 'C:Avail >Nil: FLUSH'
		End;Else MF=1+MF;
	End;
	Close(DBFH);
End;
/**/
Exit(0);

WriteOutputEntries: PROCEDURE EXPOSE datadir
	Options Results
	Parse Arg Variant Codepoint Ideograph Reading Kana Codepath
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
	CWD=Pragma(D,datadir||'/Reading');
	fname=MakePath(Codepath)||Codepoint;
	If Open(IDXFH,fname,APPEND) Then Do
		WriteLn(IDXFH,Ideograph);
		Close(IDXFH);
	End;Else If Open(IDXFH,fname,WRITE) Then Do
		WriteLn(IDXFH,Ideograph);
		Close(IDXFH);
	End;
	Pragma(D,CWD);
	/**/
	Echo Codepoint Reading Codepath
	/**/
	return rc;

MakePath: PROCEDURE EXPOSE datadir  /* Relative Only...need to add disk support*/
	Options Results;
	Parse Arg ' ' path
	root=Pos(':',path)
	If root=0 Then rpath=path; Else Parse Var path With disk ':' rpath;
	wpath=Translate(rpath,' ','/');
	CWD=Pragma(D)
	mpath='';Do i=1 To Words(wpath)
		mpath=mpath||Word(wpath,i);
		If ~Exists(mpath) Then Address COMMAND 'C:MakeDir' mpath
		mpath=mpath||'/';
	End;
	Pragma('D',CWD)
	Return mpath;

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

CodePointConvert: PROCEDURE
	Options Results
	Parse Arg Reading Type ArgV
	rc='';Syllable='';
	Do i=1 To Length(Reading) By 1;
        c=SubStr(Reading,i,1);cx=SubStr(Reading,i+1,1);
		If Syllable=c Then rc=rc||CodePointCandidate('tsu'||' '||Type);Else Syllable=Syllable||c;
		Select
			When c='A' Then Do; rc=rc||CodePointCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='I' Then Do; rc=rc||CodePointCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='U' Then Do; rc=rc||CodePointCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='E' Then Do; rc=rc||CodePointCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='O' Then Do; rc=rc||CodePointCandidate(Syllable||' '||Type); Syllable=''; End;
			When c='N' Then Select;
				When cx='A' Then NOP;
				When cx='I' Then NOP;
				When cx='U' Then NOP;
				When cx='E' Then NOP;
				When cx='O' Then NOP;
				Otherwise rc=rc||CodePointCandidate(Syllable||' '||Type); Syllable='';
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

CodePointCandidate: PROCEDURE	/* Raw Hiragana Codepoint Sequences are output as rc */
	Options Results
	Parse Arg Romaji ArgV
	Select
		When Romaji='A'		Then rc='3042/';
		When Romaji='I'		Then rc='3044/';
		When Romaji='U'		Then rc='3046/';
		When Romaji='E'		Then rc='3048/';
		When Romaji='O'		Then rc='304A/';
		When Romaji='KA'	Then rc='304B/';
		When Romaji='GA'	Then rc='304C/';
		When Romaji='KI'	Then rc='304D/';
		When Romaji='GI'	Then rc='304E/';
		When Romaji='KU'	Then rc='304F/';
		When Romaji='GU'	Then rc='3050/';
		When Romaji='KE'	Then rc='3051/';
		When Romaji='GE'	Then rc='3052/';
		When Romaji='KO'	Then rc='3053/';
		When Romaji='GO'	Then rc='3054/';
		When Romaji='SA'	Then rc='3055/';
		When Romaji='ZA'	Then rc='3056/';
		When Romaji='SHI'	Then rc='3057/';
		When Romaji='SI'	Then rc='3057/';
		When Romaji='JI'	Then rc='3058/';
		When Romaji='SU'	Then rc='3059/';
		When Romaji='ZU'	Then rc='305A/';
		When Romaji='SE'	Then rc='305B/';
		When Romaji='ZE'	Then rc='305C/';
		When Romaji='SO'	Then rc='305D/';
		When Romaji='ZO'	Then rc='305E/';
		When Romaji='TA'	Then rc='305F/';
		When Romaji='DA'	Then rc='3060/';
		When Romaji='CHI'	Then rc='3061/';
		When Romaji='DI'	Then rc='3062/';
		When Romaji='tsu'	Then rc='3063/';
		When Romaji='TSU'	Then rc='3064/';
		When Romaji='DZU'	Then rc='3065/';
		When Romaji='TE'	Then rc='3066/';
		When Romaji='DE'	Then rc='3067/';
		When Romaji='TO'	Then rc='3068/';
		When Romaji='DO'	Then rc='3069/';
		When Romaji='NA'	Then rc='306A/';
		When Romaji='NI'	Then rc='306B/';
		When Romaji='NU'	Then rc='306C/';
		When Romaji='NE'	Then rc='306D/';
		When Romaji='NO'	Then rc='306E/';
		When Romaji='HA'	Then rc='306F/';
		When Romaji='BA'	Then rc='3070/';
		When Romaji='PA'	Then rc='3071/';
		When Romaji='HI'	Then rc='3072/';
		When Romaji='BI'	Then rc='3073/';
		When Romaji='PI'	Then rc='3074/';
		When Romaji='HU'	Then rc='3075/';
		When Romaji='FU'	Then rc='3075/';
		When Romaji='BU'	Then rc='3076/';
		When Romaji='PU'	Then rc='3077/';
		When Romaji='HE'	Then rc='3078/';
		When Romaji='BE'	Then rc='3079/';
		When Romaji='PE'	Then rc='307A/';
		When Romaji='HO'	Then rc='307B/';
		When Romaji='BO'	Then rc='307C/';
		When Romaji='PO'	Then rc='307D/';
		When Romaji='MA'	Then rc='307E/';
		When Romaji='MI'	Then rc='307F/';
		When Romaji='MU'	Then rc='3080/';
		When Romaji='ME'	Then rc='3081/';
		When Romaji='MO'	Then rc='3082/';
		When Romaji='YA'	Then rc='3084/';
		When Romaji='YU'	Then rc='3086/';
		When Romaji='YO'	Then rc='3088/';
		When Romaji='RA'	Then rc='3089/';
		When Romaji='RI'	Then rc='308A/';
		When Romaji='RU'	Then rc='308B/';
		When Romaji='RE'	Then rc='308C/';
		When Romaji='RO'	Then rc='308D/';
		When Romaji='WA'	Then rc='308F/';
		When Romaji='WI'	Then rc='3090/';
		When Romaji='WE'	Then rc='3091/';
		When Romaji='WO'	Then rc='3092/';
		When Romaji='N'		Then rc='3093/';
		When Romaji='VU'	Then rc='3094/';
/**/
		When Romaji='KYA'	Then rc='304D,3083/';
		When Romaji='GYA'	Then rc='304E,3083/';
		When Romaji='KYU'	Then rc='304D,3085/';
		When Romaji='GYU'	Then rc='304E,3085/';
		When Romaji='KYO'	Then rc='304D,3087/';
		When Romaji='GYO'	Then rc='304E,3087/';
		When Romaji='SHYA'	Then rc='3057,3083/';
		When Romaji='SHA'	Then rc='3057,3083/';
		When Romaji='JA'	Then rc='3058,3083/';
		When Romaji='SHYU'	Then rc='3057,3085/';
		When Romaji='SHU'	Then rc='3057,3085/';
		When Romaji='JU'	Then rc='3058,3085/';
		When Romaji='JE'	Then rc='3058,3047/';
		When Romaji='SHYO'	Then rc='3057,3087/';
		When Romaji='SHO'	Then rc='3057,3087/';
		When Romaji='JO'	Then rc='3058,3087/';
		When Romaji='CHYA'	Then rc='3061,3083/';
		When Romaji='CHA'	Then rc='3061,3083/';
		When Romaji='CHYU'	Then rc='3061,3085/';
		When Romaji='CHU'	Then rc='3061,3085/';
		When Romaji='CHYO'	Then rc='3061,3087/';
		When Romaji='CHO'	Then rc='3061,3087/';
		When Romaji='NYA'	Then rc='306B,3083/';
		When Romaji='NYU'	Then rc='306B,3085/';
		When Romaji='NYO'	Then rc='306B,3087/';
		When Romaji='HYA'	Then rc='3072,3083/';
		When Romaji='BYA'	Then rc='3073,3083/';
		When Romaji='PYA'	Then rc='3074,3083/';
		When Romaji='FA'	Then rc='3075,3041/';
		When Romaji='FI'	Then rc='3075,3043/';
		When Romaji='FE'	Then rc='3075,3047/';
		When Romaji='FO'	Then rc='3075,3049/';
		When Romaji='BYU'	Then rc='3073,3085/';
		When Romaji='PYU'	Then rc='3074,3085/';
		When Romaji='HYO'	Then rc='3072,3087/';
		When Romaji='BYO'	Then rc='3073,3087/';
		When Romaji='PYO'	Then rc='3074,3087/';
		When Romaji='MYA'	Then rc='307F,3083/';
		When Romaji='MYU'	Then rc='307F,3085/';
		When Romaji='MYO'	Then rc='307F,3087/';
		When Romaji='RYA'	Then rc='308A,3083/';
		When Romaji='RYU'	Then rc='308A,3085/';
		When Romaji='RYO'	Then rc='308A,3087/';
		Otherwise rc='^';
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
