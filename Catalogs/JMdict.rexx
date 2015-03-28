/*
\\	JMdict.rexx
*/
Options Results
Parse Arg CatalogF XMLdatasetF DictionaryF Vector

CDFP=Pragma(D)||'/'||CatalogF
XMFP=Pragma(D)||'/'||XMLdatasetF
DCTFP=Pragma(D)||'/'||DictionaryF

If ~Open(CDFH,CDFP,WRITE) Then Do
    Echo 'Failed to Open Description Catalog for Writing'
	Exit
	End

If ~Open(XMLFH,XMFP,READ)  Then Do
    Echo 'Failed to Open XML dataset for Reading'
	Exit
	End

If ~Open(DCTFH,DCTFP,WRITE) Then Do
    Echo 'Failed to Open Dictionary for Writing'
	End

WriteLn(CDFH,';;')
WriteLn(CDFH,';;	This Catalog Description is Automatically regenerated at build time!!!')
WriteLn(CDFH,';;')

Echo "Searching XML for Entities"
Do Until Eof(XMLFH)
	S=ReadLn(XMLFH);
	SI=Pos('<',S);DI=Pos('>',S);TI=Pos('</',S);
	Tag='NULL';Item='NULL';
	If Sign(SI) Then Tag=Compress(SubStr(S,SI,DI),'<>');
	SELECT
		When Tag='keb'		Then Do;S=DelStr(S,1,5);KEB=Left(S,POS('<',S)-1);End;
		When Tag='reb'		Then Do;S=DelStr(S,1,5);REB=Left(S,POS('<',S)-1);End;
		When Tag='/entry'	Then Do;O=SubStr(KEB,1,1)
			If BitAnd(X2C('7F'),O)=O Then Do;L=1;O=SubStr(KEB,1,L);End;
			If BitAnd(X2C('DF'),O)=O Then Do;L=2;O=SubStr(KEB,1,L);End;
            If BitAnd(X2C('EF'),O)=O Then Do;L=3;O=SubStr(KEB,1,L);End;
			If Length(O)=Length(KEB) Then SELECT
				When L=3 Then Do;
					KX=B2C(SubStr(C2B(SubStr(KEB,1,1)),5,4)||SubStr(C2B(SubStr(KEB,2,1)),3,6)||SubStr(C2B(SubStr(KEB,3,1)),3,6))
					WriteLn(CDFH,'MSG_ID'||C2D(KX)||' ( '||C2D(KX)||' / 16 / 256 )'||'0A'x||C2X(REB)||'0A'x||'0A'x);KEB='';REB='';
                End;
				When L=2 Then Do;
					KX=B2C(SubStr(C2B(SubStr(KEB,1,1)),5,4)||SubStr(C2B(SubStr(KEB,2,1)),3,6))
					WriteLn(CDFH,'MSG_ID'||C2D(KX)||' ( '||C2D(KX)||' / 16 / 256 )'||'0A'x||C2X(REB)||'0A'x||';;'||'0A'x);KEB='';REB='';
                End;
				When L=1 Then Do;
					KX=KEB;WriteLn(CDFH,'MSG_ID'||C2D(KX)||' ( '||C2D(KX)||' / 16 / 256 )'||'0A'x||C2X(REB)||'0A'x||';;'||'0A'x);KEB='';REB='';
                End;
			End;Else Do;
			Nop;
			/*
				Dictionary Building using the alternate output file target
			*/
			End;
		End;
		Otherwise			Nop;
	End
End
