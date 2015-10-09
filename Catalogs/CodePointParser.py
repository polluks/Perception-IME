import os,time

#open file, create language files etc, iterate line by line
#identify language and encode code point to correct unicode

"should probably have a more reliable absolute path "
pathToTopLevelDir = os.path.dirname(os.getcwd())
CodePointsZipFile = pathToTopLevelDir+"/Resources/Unicode/Unihan.zip"

allLines = iter(open("Unihan_Readings.txt",'r').read().split('\n'))  #with is a context manager, automatically closes file handles etc no matter what happens


def parseJapaneseLine(tokens,lines,dialect):
    hex =tokens[0][2:]
    decimal =int(hex,16)

    codePoint = unichr(decimal)
    readings = tokens[2]
    out = codePoint +": "+readings
    print "hex: ",hex
    print "decimal: ",decimal
    print "codePoint: ",codePoint
    print out
    print
    return out

def writeOut(readingsetc,path):
    output = open(path,"w")
    for readings in readingsetc:
        output.write(readings+"\n\n")
    output.close()


def parseJapanese(lines):
    kJapaneseKun =[]
    kJapaneseOn = []
    line = ""

    while "EOF" not in line:
        line = lines.next()
        if not line.startswith("U"):
          continue
        tokens = line.split("\t")

        if "kJapaneseKun" == tokens[1]:
            kJapaneseKun.append(parseJapaneseLine(tokens,lines,"kun"))
        
        if "kJapaneseOn" == tokens[1]:
            kJapaneseOn.append(parseJapaneseLine(tokens,lines,"On"))

    kJapaneseOn.sort(key=len,reverse=True)
    writeOut(kJapaneseOn,"kJapaneseOn.output")

    kJapaneseKun.sort(key=len,reverse=True)
    writeOut(kJapaneseKun,"kJapaneseKun.output")

    print "\t"+str(len(kJapaneseOn)+len(kJapaneseKun))+" characters processed."

parseJapanese(allLines)


