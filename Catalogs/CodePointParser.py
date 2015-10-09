import os,time

#open file, create language files etc, iterate line by line
#identify language and encode code point to correct unicode

"should probably have a more reliable absolute path "
pathToTopLevelDir = os.path.dirname(os.getcwd())
CodePointsZipFile = pathToTopLevelDir+"/Resources/Unicode/Unihan.zip"

allLines = iter(open("Unihan_Readings.txt",'r').read().split('\n')) #iterators are great 
verbose = True

def encode(hex,keyword):
    decimal =int(hex,16)
    codePoint = unichr(decimal)
    return codePoint

"""base parsing line, to be decorated """
def parseLine(tokens,lines,keyword):
    hex = tokens[0][2:]
    codePoint = encode(hex,keyword)
    readings = tokens[2]
    readings = readings.decode("utf-8")

    out = codePoint +": "+readings
    """
    print "hex: ",hex
    print "decimal: ",decimal
    print "codePoint: ",codePoint
    print out
    print
    """
    return out

def japaneseDecoration(parseLine):
    def innerWrapper(tokens,lines,keyword):
        print "Decorated."
        return parseLine(tokens,lines,keyword)
    return innerWrapper

parseJapaneseLine = japaneseDecoration(parseLine) #move into dict

parsers = {"JapaneseKun" : parseJapaneseLine,"Definition" : parseLine,"Korean" : parseLine,"Tang" : parseLine}

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
        keyword = tokens[1]
        keyword = keyword[1:]
       
        if verbose:
            #print "processing: ",keyword
            #print tokens
            pass

        if keyword not in parsers:
            continue
        out = parsers[keyword](tokens,lines,keyword)
        if verbose:
            print out
            #time.sleep(4)
            #kJapaneseKun.append(parseJapaneseLine(tokens,lines,"kun"))
        
        #if "kMandarin" ==tokens[1]:
        #make dispatcher instead of ifs.keyword is key, lines,tokens,keyword minus k values.
 

    kJapaneseOn.sort(key=len,reverse=True)
    writeOut(kJapaneseOn,"kJapaneseOn.output")

    kJapaneseKun.sort(key=len,reverse=True)
    writeOut(kJapaneseKun,"kJapaneseKun.output")

    print "\t"+str(len(kJapaneseOn)+len(kJapaneseKun))+" characters processed."

parseJapanese(allLines)


