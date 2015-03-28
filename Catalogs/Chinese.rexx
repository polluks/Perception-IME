/**
\\	Chinese.rexx
**/
Options Results
Parse Arg CatFname Vector
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
