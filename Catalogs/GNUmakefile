#
#	'Perception.Library' Build Scripting
#
MAKE		=	make
REXX		=	RX
UNICODE 	=	$(REXX) Unicode.rexx
MKDIR		=	Makedir
RM 			=	Delete >Nil:

main:
	@ Echo "Generating [Language=Chinese] Datasets"
#	$(REXX) Chinese.rexx Chinese
	@ Echo "Generating [Language=Japanese] Datasets"
	$(REXX) Japanese.rexx Japanese >Kanji-Readings.txt
	@ Echo "Generating [Language=Korean] Datasets"
#	$(REXX) Korean.rexx Korean

clean:
	@ Echo >dummy.txt " "
	@ Echo >dummy.cd " "
	@ Echo >dummy.ct " "
	@ $(RM) #?.txt #?.cd #?.ct

#
#	Everything below this point is commentative and non-essential
#

##	$(UNICODE)  -	-
#	$(UNICODE) Test-Range1 0000 0080 ASCIIZ
#	$(UNICODE) Test-Range2 0000 0080 ASCIIZ
##	$(UNICODE) -	-
##
##	Template Entry
##
##	$(UNICODE) Name Start	Stop	Comment
##
## Names are from the Unicode Standard
## Start Codes are the First Codepoint for the Region
## Stop Codes are the last printable plus one for the Region
##
##	$(UNICODE) C0-Controls-and-Basic-Latin 0000 0080
##	$(UNICODE) C1-Controls-and-Latin-1-Suppliment 0080 0100
##	...
##	$(UNICODE) CJK-Unified-Ideographs-Extension-B 20000 2FA1E
##	$(UNICODE) Tags E0000 E0080
##	$(UNICODE) Variation-Selectors-Suppliment E0100 E0200
##	# The next Section would begin from CodePoint E0200
##	$(UNICODE) - E0200 -
##	$(UNICODE) -	-

money:
	$(UNICODE) Currency-Symbols 20A0 20Be
#
Math:
	$(UNICODE) Mathematical-Operators 2200 2300
	$(UNICODE) Miscellaneous-Mathematical-Symbols-B 2980 2A00
	$(UNICODE) Supplimentary-Mathematical-Operators 2A00 2B00
	$(UNICODE) Mathematical-Alphanumeric-Symbols 1D400 1D800
	$(UNICODE) Miscellaneous-Mathematical-Symbols-A 27C0 27F0

Music:
	$(UNICODE) Byzantine-Musical-Symbols 1D000 1D0F6
	$(UNICODE) Musical-Symbols 1D100 1D200
	$(UNICODE) Ancient-Greek-Musical-Symbols 1D200 1D246

Shapes:
	$(UNICODE) Geometric-Shapes 25A0 2600
	$(UNICODE) Geometric-Shapes-Extended 1F780 1F7D5

Graphics:
	$(UNICODE) Dingbats 2700 27C0
	$(UNICODE) Emoticons 1F600 1F650

Misc:
	$(UNICODE) IPA-Extensions 0250 02B0
	$(UNICODE) Spacing-Modifier-Letters 02B0 0300
	$(UNICODE) Combining-Diacritical-Marks 0300 0370
	$(UNICODE) Combining-Diacritical-Marks-Extended 1AB0 1ABF
	$(UNICODE) Phonetic-Extensions 1D00 1D80
	$(UNICODE) Phonetic-Extensions-Suppliment 1D80 1DA0
	$(UNICODE) Combining-Diacritical-Marks-Suppliment 1DA0 1E00
	$(UNICODE) Combining-Diacritical-Marks-For-Symbols 20D0 20F1
	$(UNICODE) LetterLike-Symbols 2100 2150
	$(UNICODE) Number-Forms 2150 218A
	$(UNICODE) Arrows 2190 2200
	$(UNICODE) Miscellaneous-Technical 2300 2400
	$(UNICODE) Control-Pictures 2400 2427
	$(UNICODE) Enclosed-AlphaNumerics 2460 2500
	$(UNICODE) Box-Drawing 2500 2580
	$(UNICODE) Box-Elements 2580 25A0
	$(UNICODE) Miscellaneous-Symbols 2600 2700
	$(UNICODE) Supplimental-Arrows-A 27F0 2800
	$(UNICODE) Braille-Patterns 2800 2900
	$(UNICODE) Supplimental-Arrows-B 2900 2980
	$(UNICODE) Miscellaneous-Symbols-and-Arrows 2B00 2BD2
	$(UNICODE) Supplimental-Punctuation 2E00 2E80
	$(UNICODE) Modified-Tone-Letters A700 A720
	$(UNICODE) Alphabetic-Presentation-Forms FB00 FB50
	$(UNICODE) Variation-Selectors FE00 FE10
	$(UNICODE) Specials FFF9 FFFE
	$(UNICODE) Shorthand-Format-Controls 1BCA0 1BCB0
	$(UNICODE) Mahjong-Tiles 1F000 1F02C
	$(UNICODE) Domino-Tiles 1F030 1F094
	$(UNICODE) Playing-Cards 1F0A0 1F0F6
	$(UNICODE) Enclosed-Alphanumeric-Suppliment 1F100 1F200
	$(UNICODE) Ornamental-Dingbats 1F650 1F680
	$(UNICODE) Transport-and-Map-Symbols 1F680 1F6F4
	$(UNICODE) Alchemical-Symbols 1F700 1F774
	$(UNICODE) Supplimental-Arrows-C 1F800 1F8AE
	$(UNICODE) Tags E0000 E0080
	$(UNICODE) Variation-Selectors-Suppliment E0100 E0200

other:
	$(UNICODE) Greek-and-Coptic 0370 0400
	$(UNICODE) Cyrillic 0400 0500
	$(UNICODE) Cyrillic-Suppliment 0500 0530
	$(UNICODE) Armenian 0530 0590
	$(UNICODE) Hebrew 0591 05F5
	$(UNICODE) Arabic 0600 0700
	$(UNICODE) Syriac 0700 0750
	$(UNICODE) Arabic-Suppliment 0750 0780
	$(UNICODE) Thaana 0780 07B2
	$(UNICODE) NKo 07C0 07FB
	$(UNICODE) Samaritan 0800 0840
	$(UNICODE) Mandiac 0840 085F
	$(UNICODE) Arabic-Extended-A 08A0 0900
	$(UNICODE) Devanagari 0900 0980
	$(UNICODE) Bengali 0980 09FC
	$(UNICODE) Gurmukhi 0A00 0A76
	$(UNICODE) Gujurati 0A80 0AF2
	$(UNICODE) Oriya 0B00 0B78
	$(UNICODE) Tamil 0B80 0BFB
	$(UNICODE) Telugu 0C00 0C80
	$(UNICODE) Kannada 0C80 0CF3
	$(UNICODE) Malayalam 0D00 0D80
	$(UNICODE) Sinhala 0D80 0DF5
	$(UNICODE) Thai 0E00 0E5C
	$(UNICODE) Lao 0E80 0EE0
	$(UNICODE) Myanmar 1000 10A0
	$(UNICODE) Georgian 10A0 1100
	$(UNICODE) Ethiopic 1200 137D
	$(UNICODE) Ethiopic-Suppliment 1380 139A
	$(UNICODE) Cherokee 13A0 13F5
	$(UNICODE) Unified-Canadian-Aboriginal-Syllabics 1400 1680
	$(UNICODE) Tagalog 1700 1715
	$(UNICODE) Hanunoo 1720 1737
	$(UNICODE) Buhid 1740 1754
	$(UNICODE) Tagbanwa 1760 1774
	$(UNICODE) Khmer 1780 17FA
	$(UNICODE) Mongolian 1800 18AB
	$(UNICODE) Unified-Canadian-Aboriginal-Syllabics-Extended 18B0 1900
	$(UNICODE) Limbu 1900 1950
	$(UNICODE) Tai-Le 1950 1975
	$(UNICODE) New-Tai-Lue 1980 19E0
	$(UNICODE) Khmer-Symbols 19E0 1A00
	$(UNICODE) Buginese 1A00 1A20
	$(UNICODE) Tai-Tham 1A20 1AAE
	$(UNICODE) Balinese 1B00 1B7D
	$(UNICODE) Sundanese 1B80 1BC0
	$(UNICODE) Batak 1BC0 1C00
	$(UNICODE) Lepcha 1C00 1C50
	$(UNICODE) Ol-Chiki 1C50 1C80
	$(UNICODE) Sundanese-Suppliment 1CC0 1CC8
	$(UNICODE) Vedic-Extensions 1CD0 1CFA
	$(UNICODE) Greek-Extended 1F00 2000
	$(UNICODE) Glagolithic 2C00 2C5F
	$(UNICODE) Coptic 2C80 2D00
	$(UNICODE) Georgian-Suppliment 2D00 2D28
	$(UNICODE) Tifinagh 2D30 2D80
	$(UNICODE) Ethiopic-Extended 2D80 2DDF
	$(UNICODE) Cyrillic-Extended-A 2DE0 2E00
	$(UNICODE) Lisu A4D0 A500
	$(UNICODE) Val A500 A62C
	$(UNICODE) Cyrillic-Extended-B A640 A6A0
	$(UNICODE) Bamum A6A0 A6F8
	$(UNICODE) Syloti-Nagri A800 A82C
	$(UNICODE) Common-Indic-Number-Forms A830 A83A
	$(UNICODE) Phags-Pa A840 A878
	$(UNICODE) Saurashtra A880 A8D8
	$(UNICODE) Devenagari-Extended A8E0 A8Fc
	$(UNICODE) Kayah-Li A900 A930
	$(UNICODE) Rejang A930 A960
	$(UNICODE) Javanese A980 A9E0
	$(UNICODE) Myanmar-Extended-B A9E0 A9FF
	$(UNICODE) Cham AA00 AA60
	$(UNICODE) Myanmar-Extended-A AA60 AA80
	$(UNICODE) Tai-Viet AA80 AAE0
	$(UNICODE) Meetei-Mayek-Extensions AAE0 AAF7
	$(UNICODE) Ethiopic-Extended-A AB00 AB2F
	$(UNICODE) Meetei-Mayek ABC0 ABFA
	$(UNICODE) Arabic-Presentation-Forms-A FB50 FDFE
	$(UNICODE) Arabic-Presentation-Forms-B FE70 FF00
	$(UNICODE) Linear-B-Syllabary 10000 1005E
	$(UNICODE) Linear-B-Ideograms 10080 100FB
	$(UNICODE) Aegean-Numbers 10100 10140
	$(UNICODE) Ancient-Greek-Numbers 10140 10190
	$(UNICODE) Ancient-Symbols 10190 101A1
	$(UNICODE) Phaistos-Disc 101D0 101FE
	$(UNICODE) Lycian 10280 1029D
	$(UNICODE) Carian 102A0 102D1
	$(UNICODE) Coptic-Epact-Numbers 102E0 102FC
	$(UNICODE) Old-Italic 10300 10324
	$(UNICODE) Gothic 10330 1034B
	$(UNICODE) Old-Permic 10350 1037B
	$(UNICODE) Ugaritic 10380 103A0
	$(UNICODE) Old-Persian 103A0 103D6
	$(UNICODE) Deseret 10400 10450
	$(UNICODE) Shavian 10450 10480
	$(UNICODE) Osmanya 10480 104AA
	$(UNICODE) Elbasan 104B0 10528
	$(UNICODE) Caucasian-Albanian 10530 10570
	$(UNICODE) Linear-A 10600 10800
	$(UNICODE) Cypriot-Syllabary 10800 10840
	$(UNICODE) Imperial-Aramaic 10840 10860
	$(UNICODE) Palmyrene 10860 10880
	$(UNICODE) Nabataean 10880 10900
	$(UNICODE) Phonecian 10900 10920
	$(UNICODE) Lydian 10920 10980
	$(UNICODE) Meroitic-Heiroglyphs 10980 109A0
	$(UNICODE) Meroitic-Cursive 109A0 109BF
	$(UNICODE) Kharoshthi 10A00 10A59
	$(UNICODE) Old-South-Arabian 10A60 10A80
	$(UNICODE) Old-North-Arabian 10A80 10AA0
	$(UNICODE) Manichaean 10AC0 10AF7
	$(UNICODE) Avestan 10B00 10B40
	$(UNICODE) Inscriptional-Parthian 10B40 10B60
	$(UNICODE) Inscriptional-Parlavi 10B60 10B80
	$(UNICODE) Psalter-Parlavi 10B80 10C00
	$(UNICODE) Old-Turkic 10C00 10C49
	$(UNICODE) Rumi-Numeral-Symbols 10E60 10E7F
	$(UNICODE) Brahmi 11000 11080
	$(UNICODE) Kaithi 11080 110C2
	$(UNICODE) Sera-Sompeng 110D0 110FA
	$(UNICODE) Chakma 11100 11144
	$(UNICODE) Mahajani 11150 11177
	$(UNICODE) Sharada 11180 111DB
	$(UNICODE) Sinhala-Archaic-Numbers 111E0 111F5
	$(UNICODE) Khojki 11200 1123E
	$(UNICODE) Khudawadi 112B0 112FA
	$(UNICODE) Grantha 11300 11375
	$(UNICODE) Tirhuta 11480 114DA
	$(UNICODE) Siddham 11580 115CA
	$(UNICODE) Modi 11600 1165A
	$(UNICODE) Takri 11680 116CA
	$(UNICODE) Warang-Citi 118A0 11900
	$(UNICODE) Pau-Cin-Hau 11900 11AF9
	$(UNICODE) Cuniform 12000 12399
	$(UNICODE) Cuniform-Numbers-and-Punctuation 12400 12475
	$(UNICODE) Egyptian-Heiroglyphs 13000 1342F
	$(UNICODE) Bamum-Suppliment 16800 16A39
	$(UNICODE) Mro 16A40 16AD0
	$(UNICODE) Basse-Vah 16AD0 16B00
	$(UNICODE) Pahawh-Hmong 16B00 16B90
	$(UNICODE) Miao 16F00 16FA0
	$(UNICODE) Duployan 1BC00 1BCA0
	$(UNICODE) Counting-Rod-Symbols 1D360 1D372
	$(UNICODE) Mende-Kikakui 1D800 1D8D7
	$(UNICODE) Arabic-Mathematical-Alphabetic-Symbols 1EE00 1EEF2
#
