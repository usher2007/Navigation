close all, clear all, clc;
fileDir = 'Data\GBMForeground\';
scrsz = get(0,'ScreenSize');
figure1 = figure('Position',[0 30 scrsz(3) scrsz(4)-95]);
for i=0:19
    clf;
    fileName = strcat(fileDir,num2str(i));
    fileName = strcat(fileName,'.bmp');
    pic = imread(fileName);
    EachCol = sum(pic);
    EachRow = sum(pic,2);
    PicSize = size(pic);
    AvgCol = mean(EachCol)*ones(1,PicSize(1,2));
    AvgRow = mean(EachRow)*ones(1,PicSize(1,1));
    plot(EachCol);
    hold;
    plot(2*AvgCol,'r');
    axis([0 720 0 50000]);
    saveas(gcf,num2str(i),'bmp');
end

