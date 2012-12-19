close all, clear all, clc;

fileDir = 'Data\Origin\TT';
saveDir = 'Data\Result\12.19\';
fileName = strcat(fileDir,num2str(1));
fileName = strcat(fileName,'.bmp');
last_pic = imread(fileName);

for i=5:2468
    fileName = strcat(fileDir,num2str(i));
    fileName = strcat(fileName,'.bmp');
    pic = imread(fileName);
    diff_pic = (pic - last_pic);
    imshow(diff_pic);
    lastFile = strcat(fileDir, num2str(i-4));
    lastFile = strcat(lastFile, '.bmp');
    last_pic = imread(lastFile);
    saveFile = strcat(saveDir,num2str(i));
    saveFile = strcat(saveFile,'.bmp');
    imwrite(diff_pic, saveFile, 'bmp');
    
    pause(0.1);
end