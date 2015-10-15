##
##  Thank you to JamesWKerr for the initial code for this script,
##    I did try to explain what was essential and what was irrelevant...*sigh*
##
import sys,os,time
try:
	import posix
except:
	import amiga

def encode(hex):								# Generate the actual UTF8 Sequence for a CodePoint
	glyph = unichr(int(hex,16))
	return glyph

#
#	Romaji and CodePoint mappings stored as proper Unicode UTF8 sequences in a proper order
#
romadict = {
	"KA":unichr(int("304B",16)),	"GA":unichr(int("304C",16)),
	"KI":unichr(int("304D",16)),	"GI":unichr(int("304E",16)),
	"KU":unichr(int("304F",16)),	"GU":unichr(int("3050",16)),
	"KE":unichr(int("3051",16)),	"GE":unichr(int("3052",16)),
	"KO":unichr(int("3053",16)),	"GO":unichr(int("3054",16)),
	"SA":unichr(int("3055",16)),	"ZA":unichr(int("3056",16)),
	"SHI":unichr(int("3057",16)),	"JI":unichr(int("3058",16)),
	"SI":unichr(int("3057",16)),	"ZI":unichr(int("3058",16)),
	"SU":unichr(int("3059",16)),	"ZU":unichr(int("305A",16)),
	"SE":unichr(int("305B",16)),	"ZE":unichr(int("305C",16)),
	"SO":unichr(int("305D",16)),	"ZO":unichr(int("305E",16)),
	"TA":unichr(int("305F",16)),	"DA":unichr(int("3060",16)),
	"CHI":unichr(int("3061",16)),	"DI":unichr(int("3062",16)),
	"CI":unichr(int("3061",16)),	"TI":unichr(int("3061",16)),
	"TSU":unichr(int("3064",16)),	"DZU":unichr(int("3065",16)),
	"TU":unichr(int("3064",16)),	"DU":unichr(int("3065",16)),
	"TE":unichr(int("3066",16)),	"DE":unichr(int("3067",16)),
	"TO":unichr(int("3068",16)),	"DO":unichr(int("3069",16)),
	"HA":unichr(int("306F",16)),	"BA":unichr(int("3070",16)),	"PA":unichr(int("3071",16)),
	"HI":unichr(int("3072",16)),	"BI":unichr(int("3073",16)),	"PI":unichr(int("3074",16)),
	"FU":unichr(int("3075",16)),
	"HU":unichr(int("3075",16)),	"BU":unichr(int("3076",16)),	"PU":unichr(int("3077",16)),
	"HE":unichr(int("3078",16)),	"BE":unichr(int("3079",16)),	"PE":unichr(int("307A",16)),
	"HO":unichr(int("307B",16)),	"BO":unichr(int("307C",16)),	"PO":unichr(int("307D",16)),
	"MA":unichr(int("307E",16)),
	"MI":unichr(int("307F",16)),
	"MU":unichr(int("3080",16)),
	"ME":unichr(int("3081",16)),
	"MO":unichr(int("3082",16)),
	"YA":unichr(int("3084",16)),
	"YU":unichr(int("3086",16)),
	"YO":unichr(int("3088",16)),
	"RA":unichr(int("3089",16)),
	"RI":unichr(int("308A",16)),
	"RU":unichr(int("308B",16)),
	"RE":unichr(int("308C",16)),
	"RO":unichr(int("308D",16)),
	"WA":unichr(int("308F",16)),
	"WO":unichr(int("3092",16)),
	"KYA":unichr(int("304D",16))+unichr(int("3083",16)),	"GYA":unichr(int("304E",16))+unichr(int("3083",16)),
	"KYU":unichr(int("304D",16))+unichr(int("3085",16)),	"GYU":unichr(int("304E",16))+unichr(int("3085",16)),
	"KYO":unichr(int("304D",16))+unichr(int("3087",16)),	"GYO":unichr(int("304E",16))+unichr(int("3087",16)),
	"SHA":unichr(int("3057",16))+unichr(int("3083",16)),	"JA":unichr(int("3058",16))+unichr(int("3083",16)),	"DYA":unichr(int("3058",16))+unichr(int("3083",16)),
	"SYA":unichr(int("3057",16))+unichr(int("3083",16)),	"JYA":unichr(int("3058",16))+unichr(int("3083",16)),
	"SHU":unichr(int("3057",16))+unichr(int("3085",16)),	"JU":unichr(int("3058",16))+unichr(int("3085",16)),	"DYU":unichr(int("3058",16))+unichr(int("3085",16)),
	"SYU":unichr(int("3057",16))+unichr(int("3085",16)),	"JYU":unichr(int("3058",16))+unichr(int("3085",16)),
	"JE":unichr(int("3058",16))+unichr(int("3047",16)),
	"SHO":unichr(int("3057",16))+unichr(int("3087",16)),	"JO":unichr(int("3058",16))+unichr(int("3087",16)),	"DYO":unichr(int("3058",16))+unichr(int("3087",16)),
	"SYO":unichr(int("3057",16))+unichr(int("3087",16)),	"JYO":unichr(int("3058",16))+unichr(int("3087",16)),
	"CHA":unichr(int("3061",16))+unichr(int("3083",16)),	"CYA":unichr(int("3061",16))+unichr(int("3083",16)),
	"CHU":unichr(int("3061",16))+unichr(int("3085",16)),	"CYU":unichr(int("3061",16))+unichr(int("3085",16)),
	"CHO":unichr(int("3061",16))+unichr(int("3087",16)),	"CYO":unichr(int("3061",16))+unichr(int("3087",16)),
	"NYA":unichr(int("306B",16))+unichr(int("3083",16)),
	"NYU":unichr(int("306B",16))+unichr(int("3085",16)),
	"NYO":unichr(int("306B",16))+unichr(int("3087",16)),
	"HYA":unichr(int("3072",16))+unichr(int("3083",16)),	"BYA":unichr(int("3073",16))+unichr(int("3083",16)),	"PYA":unichr(int("3074",16))+unichr(int("3083",16)),
	"HYU":unichr(int("3072",16))+unichr(int("3085",16)),	"BYU":unichr(int("3073",16))+unichr(int("3085",16)),	"PYU":unichr(int("3074",16))+unichr(int("3085",16)),
	"HYO":unichr(int("3072",16))+unichr(int("3087",16)),	"BYO":unichr(int("3073",16))+unichr(int("3087",16)),	"PYO":unichr(int("3074",16))+unichr(int("3087",16)),
	"MYA":unichr(int("307F",16))+unichr(int("3083",16)),
	"MYU":unichr(int("307F",16))+unichr(int("3085",16)),
	"MYO":unichr(int("307F",16))+unichr(int("3087",16)),
	"RYA":unichr(int("308A",16))+unichr(int("3083",16)),
	"RYU":unichr(int("308A",16))+unichr(int("3085",16)),
	"RYO":unichr(int("308A",16))+unichr(int("3087",16)),
}

def romaji(string):                             # Convert Romaji to Hiragana
	chord=""
	for s in string[:]:
		pass
	return kana

def parseDefault(tokens,lines,glyph):			#
	vector = tokens[2]
	return

def parseJapanese(tokens,lines,glyph):			# Build the Japanese ReadingsTree<->Kanji Mappings
	vector = tokens[2]
	index = 0
	readings = vector.split(" ")
	try:
		while readings[index]:
			kana=romaji(readings[index])
			print kana
			index+=1
	except:
		pass
	return

parsers = {
	"Chinese" : parseDefault,
	"Cantonese" : parseDefault,
	"JapaneseKun" : parseJapanese,
	"JapaneseOn" : parseJapanese,
	"Korean" : parseDefault,
	"Tang" : parseDefault
}

def parseEverything(lines):
	line = ""
	while "EOF" not in line:
		line = lines.next()
		if not line.startswith("U"):
			continue
		tokens = line.split("\t")
		keyword = tokens[1]
		keyword = keyword[1:]
		if keyword not in parsers:
			continue
		glyph = encode(tokens[0][2:])
		parsers[keyword](tokens,lines,glyph)

parseEverything(iter(open("Unihan_Readings.txt",'r').read().split('\n')))
