import sys

logfile = file('Log.txt','r')
resfile = file('res.txt','w')
index = 0
for line in logfile.readlines():
    index = index + 1
    words = line.split(' ')
    for word in words:
        word = word.strip('.-=,')
    resfile.write('line %d: %d words\n'%(index, len(words)))

logfile.close()
resfile.close()


