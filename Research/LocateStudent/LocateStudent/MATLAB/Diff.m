close all, clear all, clc;

fileDir = 'Data\Origin\TT';
saveDir = 'Data\Result\12.20\TT';
fileName = strcat(fileDir,num2str(1));
fileName = strcat(fileName,'.bmp');
last_pic = imread(fileName);
last_pic = rgb2gray(last_pic);

for i=5:2468
    fileName = strcat(fileDir,num2str(i));
    fileName = strcat(fileName,'.bmp');
    pic = imread(fileName);
    pic = rgb2gray(pic);
    diff_pic = (pic - last_pic);
    %imshow(diff_pic);
    lastFile = strcat(fileDir, num2str(i-4));
    lastFile = strcat(lastFile, '.bmp');
    last_pic = imread(lastFile);
    last_pic = rgb2gray(last_pic);
    saveFile = strcat(saveDir,num2str(i));
    saveFile = strcat(saveFile,'.bmp');
    %imwrite(diff_pic, saveFile, 'bmp');
    
    level = graythresh(diff_pic);
    diff_pic = im2bw(diff_pic, 0.12);
    
    diff_pic_roi = diff_pic(:,360:600);
    %imshow(diff_pic_roi);
    %imwrite(diff_pic_roi, saveFile, 'bmp');
    EachCol = sum(diff_pic_roi, 2);
    plot(EachCol);
    axis([0 576 0 80]);
    pause(0.1);
end