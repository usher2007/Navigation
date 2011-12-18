import os,sys,string
filenames = os.listdir(sys.argv[1])

for filename in filenames:
    postfix = string.split(filename, '.')[1]
    if postfix == 'py':
        continue
    filenum = string.split(filename, 'm')[0]
    newnum = string.atoi(filenum) + 200
    newname = str(newnum) + 'm.jpg'
    try:
        os.rename(filename, newname)
    except ValueError:
        print "Error when " + filename
    except OSError:
        print "Error when " + filename
    except NameError:
        print "Error when " + filename


