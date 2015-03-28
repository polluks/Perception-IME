/**
\\	Korean.rexx
**/
Options Results
Parse Arg CatFname Vector
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
