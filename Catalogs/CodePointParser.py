import zipfile,os

#open file, create language files etc, iterate line by line
#identify language and encode code point to correct unicode

"should probably have a more reliable absolute path "
pathToTopLevelDir = os.path.dirname(os.getcwd())
CodePointsZipFile = pathToTopLevelDir+"/Resources/Unicode/Unihan.zip"


with zipfile.ZipFile(CodePointsZipFile,'r') as allCodePointsZip:
  allCodePointsZip.extract("Unihan_Readings.txt")



with open("Unihan_Readings.txt",'r') as readings: #with is a context manager, automatically closes file handles etc no matter what happens
  
  def getLanguages(readings):
      for line in readings.readlines():
          print line,"="
          if "For details on" in line:
              print "returning"
              return languages
          print "found ",line
          languages.append(line[1:].strip())
          print languages


  for line in readings.readlines(): #this reads lines into memory one at a  time, so doesnt matter how big the file is.otherwise theres a better way to do this
      #if "This file contains" in line:
          #print "this"
          #languages = getLanguages(readings)
          #dictionary of language to function
          if line.startswith('#'):
              continue
          tokens = line.split("\t")
          print tokens
          cp = tokens[0]
          language = tokens[1]
          meaning = tokens[2]
          print cp, language, meaning
