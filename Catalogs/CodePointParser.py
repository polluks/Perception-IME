##
##  Thank you to JamesWKerr for the initial code for this script,
##    I did try to explain what was essential and what was irrelevant...*sigh*
##
import sys,os,time,unicodedata
try:
	import amiga
except:
	import posix
import romkan

#
#	I need to store Kanji recursively where each "syllable" of Japanese is a branch-key at each level
#
#
#	the Japanese word "oyogu" requires "oyo" to hold a specific Kanji...and is two syllables
#

def parseDefault(tokens,lines,glyph):			#
	vector = tokens[2]
	return

def parseJapanese(tokens,lines,glyph):			# Build the Japanese ReadingsTree<->Kanji Mappings
	vector = tokens[2]
	readings = vector.split(" ")
	for reading in readings[:]:
		try:
			kana=kana=romkan.to_hiragana(reading)
		except:
			kana=reading
		try:
			print glyph.encode("ascii","backslashreplace")+" "+kana.encode("ascii","backslashreplace")
		except:
			print glyph.encode("ascii","backslashreplace")+" BROKEN="+reading
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
	Chinese={}
	Japanese={}
	Korean={}
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
		glyph = unichr(int(tokens[0][2:],16))
		parsers[keyword](tokens,lines,glyph)

parseEverything(iter(open("Unihan_Readings.txt",'r').read().split('\n')))
