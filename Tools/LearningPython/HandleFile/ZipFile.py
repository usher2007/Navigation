import sys,os,zipfile,string

def ZipFileExcept(except_files,zip_filename,dir):
    filenames = os.listdir(dir)
    zip_res = zipfile.ZipFile(zip_filename,'w',zipfile.ZIP_DEFLATED)
    for filename in filenames:
        if filename not in except_files:
            print "Add %s"%filename
            zip_res.write(dir + '\\' + filename)
    
    zip_res.close()

if __name__=="__main__":
    zip_filename = sys.argv[1]
    dir = sys.argv[2]
    except_files = sys.argv[3].split(' ')
    print except_files
    ZipFileExcept(except_files, zip_filename, dir)

