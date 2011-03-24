import sys

#sys.path.append(r'/home/popsul/projects/zen-coding/python')

try:
    from zencoding import zen_core
    from zencoding.zen_settings import zen_settings
except:
    print "error importing zen-codign library...\n"

class ZObj:
    def __init__(self):
        self._o = -1
    def set(self, i):
        self._o = i
    def get(self):
        return self._o

zobj = ZObj()

def expand(line, currpos, mime):
    try:
        zobj.set(-1)
        abbr = zen_core.find_abbr_in_line(line, currpos)
        abbrlist = list(abbr)
        if(abbrlist[0] == ''):
            raise
        etype = ''
        if(mime == 'text/css'):
            etype = 'css'
        else:
            etype = 'html'
        expanded = zen_core.expand_abbreviation(abbrlist[0], etype)
        if(expanded == ''):
            raise
        line = line.replace(abbrlist[0], expanded)
        currentPosition = line.find(expanded) + expanded.__len__()
        dpos = line.find('|')
        if(dpos != -1):
            currentPosition = dpos
            line = line.replace('|', '')
        zobj.set(currentPosition)
        return line
    except Exception as inst:
        print inst
        return ''

def getpos():
    return zobj.get()
