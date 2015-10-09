import os

#open file, create language files etc, iterate line by line
#identify language and encode code point to correct unicode

"should probably have a more reliable absolute path "
pathToTopLevelDir = os.path.dirname(os.getcwd())
CodePointsZipFile = pathToTopLevelDir+"/Resources/Unicode/Unihan.zip"






allLines = iter(open("Unihan_Readings.txt",'r').read().split('\n'))  #with is a context manager, automatically closes file handles etc no matter what happens


def parseJapaneseLine(tokens,lines,dialect):
    codePoint = tokens[0]
    #glyph = blah
    outfile = "kJapanese"+dialect+".output"
    readings = tokens[2]
    kJapaneseOutput = open(outfile,"a")
    out = codePoint +" "+readings
    kJapaneseOutput.write(out+"\n")
    kJapaneseOutput.close()
    return out

def parseJapanese(lines):
    while lines:
        line = lines.next()
        if not line.startswith("U"):
          continue
        tokens = line.split("\t")
        if "kJapaneseKun" == tokens[1]:
            parseJapaneseLine(tokens,lines,"kun")
        
        if "kJapaneseOn" == tokens[1]:
            parseJapaneseLine(tokens,lines,"On")

        #if "kJap" in toke

parseJapanese(allLines)


def encodeDefinition():pass


#languages = {"kcantonese":encodeCantonese,"kDefinition":encodeDefinition,"kJapaneseKun":encodeJapaneseKun}
