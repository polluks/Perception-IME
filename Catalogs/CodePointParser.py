import sys,os,time

allLines = iter(open("Unihan_Readings.txt",'r').read().split('\n')) #iterators are great

def encode(hex):
	glyph = unichr(int(hex,16))
	return glyph

def parseDefault(tokens,lines,keyword):
	readings = tokens[2]
	return readings

def parseJapanese(tokens,lines,keyword):
	readings = tokens[2]
	return readings

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
		out = parsers[keyword](tokens,lines)
		print out

	print "\t"+str(len(kJapaneseOn)+len(kJapaneseKun))+" characters processed."

parseEverything(allLines)


