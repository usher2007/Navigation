function median_xs = interpolation(src_x, des_x, pic_num)
% this function generate the intermediate grid map 
% src_x start grid
% des_x end grid
% pic_num number of frames you want, including the start and end gird
pgsize = size(src_x);
median_xs = zeros(pic_num,pgsize(1),pgsize(2));
median_xs(1,:,:) = src_x;
median_xs(end,:,:) = des_x;
for i = 2:pic_num-1
    median_xs(i,:,:) = src_x + (des_x-src_x)*(i-1)/(pic_num-1);
end
end