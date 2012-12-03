close all, clear all, clc;
fileDir = 'Data\GBMForeground\';
scrsz = get(0,'ScreenSize');
figure1 = figure('Position',[0 30 scrsz(3) scrsz(4)-95]);
cur_state = 0; %norm 0  enter 1 leave 2
wait_for = 1; %wait for enter 1 wait for leave 2
enterleave_count = 0;
norm_count = 0;
margin_count = 0;
for i=1:2240
    clf;
    fileName = strcat(fileDir,num2str(i));
    fileName = strcat(fileName,'.bmp');
    pic = imread(fileName);
    EachCol = sum(pic);
    %EachRow = sum(pic,2);
    PicSize = size(pic);
    AvgCol = mean(EachCol)*ones(1,PicSize(1,2));
    %AvgRow = mean(EachRow)*ones(1,PicSize(1,1));
    plot(EachCol);
    hold;
    plot(2*AvgCol,'r');
    axis([0 720 0 50000]);
    candidates = find(EachCol > 2*AvgCol);
    candidates_num = length(candidates);
    continue_num = 0;
    sum_weight = 0;
    each_col_sum = 0;
    last_index = candidates(1,1);
    for j=1:candidates_num
        if candidates(1,j)-last_index < 50 && j ~= candidates_num
            continue_num = continue_num + 1;
            sum_weight = sum_weight + candidates(1,j)*EachCol(1,candidates(1,j))/1000;
            each_col_sum = each_col_sum + EachCol(1,candidates(1,j))/1000;
        else
            if continue_num > 30
                bary_center_x = sum_weight/each_col_sum;
                bary_center_y = each_col_sum/continue_num * 1000;
                if bary_center_x <= 120 || bary_center_x >= 600
                    enterleave_count = enterleave_count + 1;
                    if enterleave_count > 10
                        if wait_for == 1
                            cur_state = 1;
                        end
                    end
                    if wait_for == 2
                        cur_state = 2;
                    end
                end
                if bary_center_x > 120 && bary_center_x < 600 && cur_state ~= 0
                    norm_count = norm_count + 1;
                end
                
                if bary_center_x > 120 && bary_center_x < 600 && i <= 5
                    cur_state = 0;
                    wait_for = 2;
                end
                
                
                if norm_count > 10
                    if cur_state == 1 && wait_for == 1
                        cur_state = 0;
                        wait_for = 2;
                        enterleave_count = 0;
                    end
                    if cur_state == 2 && wait_for == 2
                        cur_state = 0;
                        enterleave_count = 0;
                    end
                    norm_count = 0;
                end
                
                if cur_state ~= 0 && bary_center_y < 4*AvgCol(1,candidates(1,j))
                    margin_count = margin_count + 1;
                    if margin_count > 3
                        margin_count = 0;
                        if cur_state == 1 && wait_for == 1
                            cur_state = 0;
                        end
                        if cur_state == 2 && wait_for == 2
                            cur_state = 0;
                            wait_for  = 1;
                        end
                    end
                end
                
                color = 'r+';
                if cur_state == 1
                    color = 'b+';
                end
                if cur_state == 2
                    color = 'g+';
                end
                plot(bary_center_x, bary_center_y, color, 'linewidth', 2);
                break;
            end
            continue_num = 0;
            sum_weight = 0;
            each_col_sum = 0;
        end
        last_index = candidates(1,j);
    end
    saveas(gcf,num2str(i),'bmp');
end

