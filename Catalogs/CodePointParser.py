##
##  Thank you to JohnWKerr for the initial code for this script,
##    I did try to explain what was essential and what was irrelevant...*sigh*
##


def encode(hex):					# Generate the actual UTF8 Sequence for the CodePoint value
	glyph = unichr(int(hex,16))
	return glyph

def parseDefault(tokens,lines,glyph):
	vector = tokens[2]
	return vector

#
#	Japanese Syllables all count as 1,2 or 3 English letter combinations.
#		The Input Strings are currently English Letter romaji (this needs Hiragana conversion)
#		NONE of the Input Strings contain UTF8 outside the ASCII range for processing Japanese
#
#	Each "Syllable" in the reading (multiple syllables is common) requires to be
#		a branch of each level into the tree,  leaf nodes are Kanji strings (use the glyphs!)
#
#
#	Example:	"DA/SU/" for the tree and adds a Kanji Entry to that locations string
#				"KO/U/"  is the most popular branch and will have many many Kanji listed
#				"SHI"    is a single syllable and has the Kanji for "city" along with at at least 20 more meanings
#				"HI/TO/" is include the Kanji for "person" amongst others
#				"KA/N/"  is another popular reading with many Kanji
#
#				tree[KA][N]+=<Kanji>  based on each reading.
#
#   One Kanji to One Reading IS NOT NORMAL... Many Kanji HAVE Many Readings BY DEFAULT
#


def parseJapanese(tokens,lines,glyph):
	vector = tokens[2]
	return vector

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
		out = parsers[keyword](tokens,lines,glyph)


parseEverything(iter(open("Unihan_Readings.txt",'r').read().split('\n')))
