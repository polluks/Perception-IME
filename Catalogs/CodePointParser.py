import zipfile,os

#open file, create language files etc, iterate line by line
#identify language and encode code point to correct unicode

"should probably have a more reliable absolute path "
pathToTopLevelDir = os.path.dirname(os.getcwd())
CodePointsZipFile = pathToTopLevelDir+"/Resources/Unicode/Unihan.zip"


with zipfile.ZipFile(CodePointsZipFile,'r') as allCodePointsZip:
  allCodePointsZip.extract("Unihan_Readings.txt")

