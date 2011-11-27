import os
import sys
import dicom
from PIL import Image

def dicomConvert(input, output):
	fileNames = os.listdir(input)
	for dcmFile in fileNames:
		bmpFile = dcmFile.split('.')[0] + ".bmp";
		dcmFile = input + dcmFile
		ds = dicom.read_file(dcmFile)
		size = ds.pixel_array.shape
		im = Image.frombuffer("L", size, ds.pixel_array, "raw", "L", 0, 1)
		im.save(output + bmpFile)


if __name__=="__main__":
	if len(sys.argv) < 3:
		print u'Please input the input directory and output directory'
	else:
		input = sys.argv[1]
		output = sys.argv[2]
		dicomConvert(input, output)
