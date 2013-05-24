close all, clear all, clc;
data_src = 'data\singer1\';
my_res = 'res\res\singer1.txt';
vtd_res = 'res\other\VTD\singer1.txt';
l1_res = 'res\other\L1\singer1_corner';
mil_res = 'res\other\MIL\MIL_singer1_corners.txt';
oab_res = 'res\other\OAB\OAB_singer1_corners.txt';

my = load(my_res);
vtd = load(vtd_res);
l1 = load(l1_res);
mil = load(mil_res);
oab = load(oab_res);

my_color = 'r-';
vtd_color = 'g-';
l1_color = 'b-';
mil_color = 'c-';
oab_color = 'y-';
src_names = {};
fext = 'jpg';
for i=1:321
    image_no = i;
    fid = sprintf('%05d', image_no);
    src_names{i} = strcat(data_src, 'img', fid, '.', fext);
end

len = size(src_names);
fig = figure(1);
for i=1:len(1,2)
    image_gray = imread(src_names{i});
    if(size(image_gray,3) ~= 1)
        image_gray = rgb2gray(image_gray);
    end
    imshow(image_gray);
    hold on;
    %x = [State(1) State(1)+State(3) State(1)+State(3) State(1) State(1)];
    %y = [State(2) State(2) State(2)+State(4) State(2)+State(4) State(2)];
    x = [my(i,1) my(i,1)+my(i,3) my(i,1)+my(i,3) my(i,1) my(i,1)];
    y = [my(i,2) my(i,2) my(i,2)+my(i,4) my(i,2)+my(i,4) my(i,2)];
    plot(x,y,my_color,'LineWidth',2);
    
    [h w] = size(image_gray);
    tmp = vtd;
    vtd(i,2) = h - tmp(i,4);
    vtd(i,4) = h - tmp(i,2);
    vtd(i,3) = vtd(i,3) - vtd(i,1);
    vtd(i,4) = vtd(i,4) - vtd(i,2);
    x = [vtd(i,1) vtd(i,1)+vtd(i,3) vtd(i,1)+vtd(i,3) vtd(i,1) vtd(i,1)];
    y = [vtd(i,2) vtd(i,2) vtd(i,2)+vtd(i,4) vtd(i,2)+vtd(i,4) vtd(i,2)];
    plot(x,y,vtd_color,'LineWidth',2);
    
    l1r = [l1.vos(1,1,i) l1.vos(2,1,i) l1.vos(1,3,i)-l1.vos(1,1,i) l1.vos(2,2,i)-l1.vos(2,1,i)];
    x = [l1r(1) l1r(1)+l1r(3) l1r(1)+l1r(3) l1r(1) l1r(1)];
    y = [l1r(2) l1r(2) l1r(2)+l1r(4) l1r(2)+l1r(4) l1r(2)];
    plot(x,y,l1_color,'LineWidth',2);
    
    x = [mil(i,1) mil(i,1)+mil(i,3) mil(i,1)+mil(i,3) mil(i,1) mil(i,1)];
    y = [mil(i,2) mil(i,2) mil(i,2)+mil(i,4) mil(i,2)+mil(i,4) mil(i,2)];
    plot(x,y,mil_color,'LineWidth',2);
    
    x = [oab(i,1) oab(i,1)+oab(i,3) oab(i,1)+oab(i,3) oab(i,1) oab(i,1)];
    y = [oab(i,2) oab(i,2) oab(i,2)+oab(i,4) oab(i,2)+oab(i,4) oab(i,2)];
    plot(x,y,oab_color,'LineWidth',2);
    
    text(15,30,['#' int2str(i)],'color','g','fontsize',25);
    hold off;
    filename = sprintf('drawRes%d.jpg',i);
    saveas(fig,filename,'jpg');
end