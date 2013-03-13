close all, clear all;
fprefix = 'data\faceocc2\';
fannote = 'faceocc2.txt';
fext = 'png';
s_annotation = strcat(fprefix, fannote);
start_frame = 4;
nframes = 100;

n_row = 3;
n_col = 3;

t_row = 60;
t_col = 75;

patch_row = ceil(t_row / n_row);
patch_col = ceil(t_col / n_col);

hog_bin_size = 5;
hog_n_orient = 9;
hog_index = 1;

particle_num = 800;
particles = zeros(particle_num, 4);
sigma_px = 5;
sigma_py = 3;
sigma_pw = 2;
sigma_ph = 2;

param.lambda=1;
param.numThreads=-1; % number of threads

param.mode=0;
param.approx=0;
param.pos = true;

template_num = 10;
sim_thresh = 0;
s_frames = cell(nframes, 1);
for i=1:nframes
    image_no = start_frame + i - 1;
    fid = sprintf('%05d', image_no);
    s_frames{i} = strcat(fprefix,'img', fid, '.', fext);
end

%for the 1st frame, get the true object
truth = load(s_annotation);
first_loc = truth(start_frame,:);
image_gray = imread(s_frames{1});
if(size(image_gray,3) ~= 1)
    image_gray = rgb2gray(image_gray);
end
[image_h image_w] = size(image_gray);
col_begin = first_loc(1);
row_begin = first_loc(2);
col_width = first_loc(3);
row_height = first_loc(4);
col_end = col_begin + first_loc(3);
row_end = row_begin + first_loc(4);
object = image_gray(row_begin:row_end, col_begin:col_end);
object = imresize(object, [t_row t_col]);
object_patches = im_get_patches(object, n_row, n_col);
hog_row = ceil(t_row/n_row)/hog_bin_size;
hog_col = ceil(t_col/n_col)/hog_bin_size;
hog_feature_len = hog_row*hog_col*hog_n_orient;
HT = zeros(n_row*n_col, 1+template_num+hog_feature_len*2, hog_feature_len);
for i=1:n_row*n_col
    HT(i, 1, :) = my_hog(object_patches(i, :, :), hog_bin_size, hog_n_orient, hog_index);
end
% trival templates
for i=1:n_row*n_col
    HT(i, 2+template_num:1+template_num+hog_feature_len, :) = eye(hog_feature_len);
    HT(i, 2+template_num+hog_feature_len:end,:) = -1*eye(hog_feature_len);
end

State = [col_begin, row_begin, col_width, row_height];
particles(:,1) = State(1) + round(sigma_px*randn(1,particle_num));
particles(:,2) = State(2) + round(sigma_py*randn(1,particle_num));
particles(:,3) = State(3) + round(sigma_pw*randn(1,particle_num));
particles(:,4) = State(4) + round(sigma_ph*randn(1,particle_num));

particle_features = zeros(n_row*n_col, particle_num, hog_feature_len);
particle_alpha = zeros(n_row*n_col, 1+template_num+hog_feature_len*2, particle_num);
patch_particle_sim = zeros(n_row*n_col, particle_num);
particle_sim = zeros(1,particle_num);
particle_reind = zeros(1,particle_num);
sigma_weight = 0.05;
fig = figure(1);
imshow(image_gray);
hold on
x = [State(1) State(1)+State(3) State(1)+State(3) State(1) State(1)];
y = [State(2) State(2) State(2)+State(4) State(2)+State(4) State(2)];
    
plot(x,y,'r-');
hold off
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
            pcol = 1;
        end
        if prow_begin <= 0
            prow = 1;
        end
        object_candidate = image_gray(prow_begin:prow_end, pcol_begin:pcol_end);
        object_candidate = imresize(object_candidate, [t_row t_col]);
        candidate_patches =  im_get_patches(object_candidate, n_row, n_col);
        for k=1:n_row*n_col
            particle_features(k,j,:) = my_hog(candidate_patches(k,:,:),hog_bin_size, hog_n_orient, hog_index);
        end
    end
    
    for j=1:n_row*n_col
        patch_particle_feas = reshape(particle_features(j,:,:), particle_num, hog_feature_len);
        patch_HT = reshape(HT(j,:,:), 1+template_num+hog_feature_len*2, hog_feature_len);
        particle_alpha(j,:,:) = my_lasso(patch_particle_feas', patch_HT', param);
        patch_particle_sim(j,:) = reshape((sum(particle_alpha(j,1:1+template_num,:))+0.01)./(sum(particle_alpha(j,2+template_num:end,:))+0.01), 1, particle_num);
        patch_particle_sim(j,:) = patch_particle_sim(j,:)/sum(patch_particle_sim(j,:));
        for k=1:particle_num
           patch_particle_sim(j,k) = 1/sqrt(2*pi)/sigma_weight*exp(-(1-patch_particle_sim(j,k))/2/sigma_weight^2);
        end
        patch_particle_sim(j,:) = patch_particle_sim(j,:)/sum(patch_particle_sim(j,:));
    end
    big_sim_index = patch_particle_sim > sim_thresh;
    particle_sim = sum(big_sim_index.*patch_particle_sim);
    particle_sim = particle_sim / sum(particle_sim);
    best_index = find(particle_sim == max(particle_sim));
    particle_sim_matrix = repmat(particle_sim',1,4); 
    particles_temp = particles.*particle_sim_matrix;
    State = round(sum(particles_temp(:, :), 1));
    State
    
    % display
    imshow(image_gray);
    title(sprintf('Frame = %d',i+start_frame-1));
    hold on
    x = [State(1) State(1)+State(3) State(1)+State(3) State(1) State(1)];
    y = [State(2) State(2) State(2)+State(4) State(2)+State(4) State(2)];
    
    plot(x,y,'r-');
    hold off
    filename = sprintf('%d.bmp',i+start_frame-1);
    saveas(fig,filename,'bmp');
    
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
    
    % Update the template
    index = mod(i-1, template_num);
    if index == 0
        index = 10;
    end
    %HT(:,index+1,:) = mean(particle_features(:, best_index, :), 2);
    template = image_gray(State(2):State(2)+State(4), State(1):State(1)+State(3));
    template = imresize(template,[t_row t_col]);
    template_patches = im_get_patches(template, n_row, n_col);
    for j=1:n_row*n_col
        HT(j,index+1,:) = my_hog(template_patches(j,:,:),hog_bin_size, hog_n_orient, hog_index);
    end
end