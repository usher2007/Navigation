close all;
%clear all;
fprefix = 'data\singer1\';
fannote = 'data\GroundTruth2\GT_singer1';
fres = 'res.txt';
fsigma = 'sigma.txt';
freplace = 'replace.txt';
fweight = 'weight.txt';
fext = 'jpg';

%fprefix = 'data\OneLeaveShopReenter2cor\';
%fannote = 'OneLeaveShopReenter2cor.txt';
% fext = 'jpg';
s_annotation = fannote;
start_frame = 1;
nframes = 885;

t_row = 35;
t_col = 15;

hog_bin_size = 5;
hog_n_orient = 9;
hog_index = 1;

particle_num = 800;
particles = zeros(particle_num, 4);
sigma_px = 2;
sigma_py = 2;
sigma_pw = 2;
sigma_ph = 2;

sigma_px_th = 5;
sigma_py_th = 3;
sigma_pw_th = 1;
sigma_ph_th = 2;

param.lambda=1;
param.numThreads=-1; % number of threads

param.mode=0;
param.approx=0;
param.pos = true;

template_num = 10;
template_weight = zeros(1,10);
sim_thresh_hog = 0.0005;
s_frames = cell(nframes, 1);
template_decay = 0.01;

minThresh = 0.015;
maxThresh = 0.6;
alpha_big = 1.02;
alpha_small = 0.98;

sigma_weight = 0.05;


weight_th = 1/800;

%gray feature
%todo: 15*12=180 vector, append to the end of hog vector
gray_feature_r_len = 15;
gray_feature_c_len = 12;
gray_feature_len = gray_feature_r_len * gray_feature_c_len;
for i=1:nframes
    image_no = start_frame + i - 1;
    fid = sprintf('%05d', image_no);
    %s_frames{i} = strcat(fprefix, 'OneLeaveShopReenter2cor', fid, '.', fext);
    s_frames{i} = strcat(fprefix, 'img', fid, '.', fext);
end

%for the 1st frame, get the true object
load(s_annotation);
fout = fopen(fres,'wt');
fout_sigma = fopen(fsigma,'wt');
fout_replace = fopen(freplace,'wt');
fout_weight = fopen(fweight,'wt');
first_loc = GT_corner(:,:,start_frame);
image_gray = imread(s_frames{start_frame});
if(size(image_gray,3) ~= 1)
    image_gray = rgb2gray(image_gray);
end
[image_h image_w] = size(image_gray);
col_begin = first_loc(1,1);
row_begin = first_loc(2,1);
col_width = first_loc(1,2) - first_loc(1,1);
row_height = first_loc(2,3) - first_loc(2,1);
col_end = first_loc(1,2);
row_end = first_loc(2,3);
object = image_gray(row_begin:row_end, col_begin:col_end);
object = imresize(object, [t_row t_col]);
hog_row = ceil(t_row)/hog_bin_size;
hog_col = ceil(t_col)/hog_bin_size;
hog_feature_len = hog_row*hog_col*hog_n_orient;
HT = zeros(1+template_num+(hog_feature_len+gray_feature_len)*2, hog_feature_len+gray_feature_len);
HT(1, 1:hog_feature_len) = my_hog2(object(:, :), hog_bin_size, hog_n_orient, hog_index);
HT(1, hog_feature_len+1:hog_feature_len+gray_feature_len) = reshape(imresize(object, [gray_feature_r_len gray_feature_c_len]), gray_feature_len,1);
% trival templates
HT(2+template_num:1+template_num+hog_feature_len+gray_feature_len, :) = eye(hog_feature_len+gray_feature_len);
HT(2+template_num+hog_feature_len+gray_feature_len:end,:) = -1*eye(hog_feature_len+gray_feature_len);

State = [col_begin, row_begin, col_width, row_height];
State = round(State);
particles(:,1) = State(1) + round(sigma_px*randn(1,particle_num));
particles(:,2) = State(2) + round(sigma_py*randn(1,particle_num));
particles(:,3) = State(3) + round(sigma_pw*randn(1,particle_num));
particles(:,4) = State(4) + round(sigma_ph*randn(1,particle_num));

particle_features = zeros(particle_num, hog_feature_len+gray_feature_len);
particle_alpha = zeros(1+template_num+(hog_feature_len+gray_feature_len)*2, particle_num);
particle_sim = zeros(1, particle_num);
particle_reind = zeros(1,particle_num);
fig = figure(1);
imshow(image_gray);
hold on
x = [State(1) State(1)+State(3) State(1)+State(3) State(1) State(1)];
y = [State(2) State(2) State(2)+State(4) State(2)+State(4) State(2)];
    
plot(x,y,'r-');
hold off
oldState = State;
for i=2:nframes
    image_gray = imread(s_frames{i});
    if(size(image_gray,3) ~= 1)
        image_gray = rgb2gray(image_gray);
    end
   
    for j=1:particle_num
        pcol_begin = particles(j,1);
        pcol_end = pcol_begin + particles(j,3);
        prow_begin = particles(j,2);
        prow_end = prow_begin + particles(j,4);
        if  pcol_begin <= 0
            pcol_begin = 1;
        end
        if prow_begin <= 0
            prow_begin = 1;
        end
        if pcol_end > image_w
            pcol_end = image_w;
        end
        if prow_end > image_h
            prow_end = image_h;
        end
        object_candidate = image_gray(prow_begin:prow_end, pcol_begin:pcol_end);
        object_candidate = imresize(object_candidate, [t_row t_col]);
        particle_features(j,1:hog_feature_len) = my_hog2(object_candidate(:,:),hog_bin_size, hog_n_orient, hog_index);
        particle_features(j,hog_feature_len+1:hog_feature_len+gray_feature_len) = reshape(imresize(object_candidate, [gray_feature_r_len gray_feature_c_len]), gray_feature_len, 1);
    end
    
    particle_alpha = my_lasso(particle_features', HT', param);
    particle_sim(1,:) = reshape((sum(particle_alpha(1:1+template_num,:))+0.0001+29*particle_alpha(1,:))./(sum(particle_alpha(2+template_num:end,:))+0.0001+29*particle_alpha(1,:)), 1, particle_num);
    particle_sim(1,:) = particle_sim(1,:)/sum(particle_sim(1,:));
    
    for k=1:particle_num
       particle_sim(1,k) = 1/sqrt(2*pi)/sigma_weight*exp(-(1-particle_sim(1,k))/2/sigma_weight^2);
    end
    particle_sim(1,:) = particle_sim(1,:)/sum(particle_sim(1,:));
    %best_index = find(particle_sim_hog == max(particle_sim_hog));
    
    
    %particle_sim = hog_weight*particle_sim_hog+gray_weight*particle_sim_gray;
    particle_sim = particle_sim.*(particle_sim>weight_th);
    particle_sim = particle_sim/sum(particle_sim);
    particle_sim_matrix = repmat(particle_sim',1,4);
    particles_temp = particles.*particle_sim_matrix;
    State = round(sum(particles_temp(:, :), 1));
    State
    fprintf(fout, '%d %d %d %d\n', State(1), State(2), State(3), State(4));
    % display
    imshow(image_gray);
    title(sprintf('Frame = %d',i+start_frame-1));
    hold on
    if State(1) <= 0
        State(1) = 1;
    end
    if State(1) > image_w-1
        State(1) = image_w-1;
    end
    if State(2) <= 0
        State(2) = 1;
    end
    if State(2) > image_h-1
        State(2) = image_h-1;
    end
    if State(3) <= 0
        State(3) = 1;
    end
    if State(1)+State(3) > image_w
        State(3) = image_w - State(1);
    end
    if State(4) <= 0
        State(4) = 1;
    end
    if State(2)+State(4) > image_h
        State(4) = image_h - State(2);
    end
    
    % Update the template
    sorted_sim = sort(particle_sim);
    new_weight = sum(sorted_sim(particle_num-9:particle_num));
    template_weight = template_weight * (1-template_decay);
    min_weight = min(template_weight);
    max_weight = max(template_weight);
    mean_weight = mean(template_weight);
    fprintf('max:%f    min:%f    ave:%f    new:%f\n', max_weight, min_weight, mean_weight, new_weight);
    fprintf(fout_weight, '%f %f %f %f\n', max_weight, min_weight, mean_weight, new_weight);
    if new_weight > mean_weight*1.2 
        fprintf('Replace!!!!\n');
        fprintf(fout_replace,'1\n');
        min_indexes = find(template_weight == min_weight);
        min_index = min_indexes(1);
        template = image_gray(State(2):State(2)+State(4), State(1):State(1)+State(3));
        template = imresize(template,[t_row t_col]);
        HT(min_index+1,1:hog_feature_len) = my_hog2(template(:,:),hog_bin_size, hog_n_orient, hog_index);
        HT(min_index+1,hog_feature_len+1:hog_feature_len+gray_feature_len) = reshape(imresize(template, [gray_feature_r_len gray_feature_c_len]), gray_feature_len, 1);
        template_weight(min_index) = new_weight;
    else
        fprintf(fout_replace,'0\n');
    end
%     if new_weight < minThresh
%         State = oldState;
%         if sigma_px <= sigma_px_th
%             sigma_px = sigma_px * alpha_big;
%         end
%         if sigma_py <= sigma_py_th
%             sigma_py = sigma_py * alpha_big;
%         end
%         if sigma_pw <= sigma_pw_th
%             sigma_pw = sigma_pw * alpha_big;
%         end
%         if sigma_ph <= sigma_ph_th
%             sigma_ph = sigma_ph * alpha_big;
%         end
%     end
%     
%     if new_weight > maxThresh
%         if sigma_px > 1
%             sigma_px = sigma_px * alpha_small;
%         end
%         if sigma_py > 1
%             sigma_py = sigma_py * alpha_small;
%         end
%         if sigma_pw > 1
%             sigma_pw = sigma_pw * alpha_small;
%         end
%         if sigma_ph > 1
%             sigma_ph = sigma_ph * alpha_small;
%         end
%     end
    fprintf('sigma: x-%f, y-%f, w-%f, h-%f\n', sigma_px, sigma_py, sigma_pw, sigma_ph);
    fprintf(fout_sigma, '%f %f %f %f\n', sigma_px, sigma_py, sigma_pw, sigma_ph);
    x = [State(1) State(1)+State(3) State(1)+State(3) State(1) State(1)];
    y = [State(2) State(2) State(2)+State(4) State(2)+State(4) State(2)];
    
    plot(x,y,'r-','LineWidth',2);
    hold off
    filename = sprintf('%d.jpg',i+start_frame-1);
    saveas(fig,filename,'jpg');
    

    %max(particle_sim)
    % Generate the new particles
    cdf_sim = cumsum(particle_sim);
    for j=1:particle_num
        index = find(cdf_sim>=rand);
        particle_reind(1,j) = index(1);
    end
    particles = particles(particle_reind, :);
    particles(:,1) =  particles(:,1) + round(sigma_px*randn(particle_num,1));
    particles(:,2) =  particles(:,2) + round(sigma_py*randn(particle_num,1));
    particles(:,3) =  particles(:,3) + round(sigma_pw*randn(particle_num,1));
    particles(:,4) =  particles(:,4) + round(sigma_ph*randn(particle_num,1));
    
    oldState = State;
%     index = mod(i-1, template_num);
%     if index == 0
%         index = template_num;
%     end
%     %HT(:,index+1,:) = mean(particle_features(:, best_index, :), 2);
%     template = image_gray(State(2):State(2)+State(4), State(1):State(1)+State(3));
%     template = imresize(template,[t_row t_col]);
%     template_patches = im_get_patches(template, n_row, n_col);
%     for j=1:n_row*n_col
%         HT(j,index+1,:) = my_hog2(template_patches(j,:,:),hog_bin_size, hog_n_orient, hog_index);
%     end
end