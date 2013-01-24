close all, clear all, clc;
fileDir = 'Data\GBMForeground\';
scrsz = get(0,'ScreenSize');
figure1 = figure('Position',[0 30 scrsz(3) scrsz(4)-95]);

for i=1:2469
    clf;
    fileName = strcat(fileDir,num2str(i));
    fileName = strcat(fileName,'.bmp');
    pic = imread(fileName);
    pic = edge(pic,'log');
    EachCol = sum(pic);
    EachRow = sum(pic,2);
    PicSize = size(pic);
    AvgCol = mean(EachCol)*ones(1,PicSize(1,2));
    AvgRow = mean(EachRow)*ones(1,PicSize(1,1));
    
    plot(EachRow);
    hold;
    imshow(pic);
    pause(0.1);
    continue;
    
    person_start = 0;
    continue_num = 0;
    sum_weight = 0;
    each_col_sum = 0;
    candidate_cols = find(EachCol > 2*AvgCol);
    candidate_size = length(candidate_cols);
    last_index = candidate_cols(1,1);
    for j=1:candidate_size
        if candidate_cols(1,j) - last_index < 50 && j~= candidate_size
            continue_num = continue_num + 1;
            sum_weight = sum_weight + candidate_cols(1,j)*EachCol(1,candidate_cols(1,j))/1000;
            each_col_sum = each_col_sum + EachCol(1,candidate_cols(1,j))/1000;
        else
            if continue_num > 30
                bary_center_x = sum_weight/each_col_sum;
                bary_center_y = each_col_sum/continue_num * 1000;
                plot(bary_center_x, bary_center_y, 'r+', 'linewidth', 2);
            end
            continue_num = 0;
            sum_weight = 0;
            each_col_sum = 0;
        end
        last_index = candidate_cols(1,j);
    end
    plot(2*AvgCol,'r');
    axis([0 720 0 20000]);
    pause(0.1);
end