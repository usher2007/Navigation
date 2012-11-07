%-------------------------------------------------
% 标准粒子滤波程序 可演示
%-------------------------------------------------
% 彩色直方图 加权
% 均值或最大后验 更新模版
% 手动选取区域
% 直方图增强

close all
clear all

% 1. read the image or video
video_file = 'H:\GitHubCode\Navigation\Research\ParticleFiltering\FromShunli\Video\MVI_2132.avi';
% video_file = 'E:\Research topic\Download_video\car4.avi';
% video_file = '002.avi';


video = mmreader(video_file , 'tag', 'myreader1');
offset_frame = 10;%80
nb_frame  = get(video, 'numberOfFrames');

dim_x = get(video , 'Width');
dim_y = get(video , 'Height');

I = read(video ,offset_frame + 1);

fig = figure(1);
imshow(I);
hold on
rect = getrect(fig);
Px = rect(1);
Py = rect(2);
Tx = rect(3);
Ty = rect(4);


x = [Px Px+Tx Px+Tx Px Px];
y = [Py Py Py+Ty Py+Ty Py];
plot(x,y,'r-');
hold off
%-------------------------------------------------
% 2. initialize the  parameters
% 2.1 initialize the color feature parameters
Npdf = 120;     % Number of samples to draw inside ellipse to evaluate color histogram
n1 = 6;         % Number of bins in first color dimension (R or H)
n2 = 6;         % Number of bins in second color dimension (G or S)
n3 = 6;         % Number of bins in third color dimension (B or V)
nbin = [n1 n2 n3];

sigma_color = 0.20;      % Measurement Color noise

range = 1;

% 2.2 initialize the target localization
% % 状态矢量[Ix Iy Tx Ty]
% Px = 140;  %起始点x
% Py = 80;  %起始点y
% 
% % 2.3 initialize the template
% Tx = 40; %矩形模版长
% Ty = 25; %矩形模版宽

PT = [Px;Py;Tx;Ty];

% 2.4 initialize the number of the particles
Particle_num = 20;
State_num = 4;

State = zeros(State_num,Particle_num);
State = repmat(PT,1,Particle_num);

% 重采样方差
sigma_px = 5;
sigma_py = 2;
sigma_tx = 0;
sigma_ty = 0;
sigma = [sigma_px;sigma_py;sigma_tx;sigma_ty];

% 权值设置
sigma_weight = 0.05;

% 注意可能会溢出边界
State(1,:) = Px + fix(sigma_px * randn(1,Particle_num));
State(2,:) = Py + fix(sigma_py * randn(1,Particle_num));
State(3,:) = Tx + fix(sigma_tx * randn(1,Particle_num));
State(4,:) = Ty + fix(sigma_ty * randn(1,Particle_num));

%-------------------------------------------------
% 计算原始特征 （颜色特征）
% pu = particle_colorhist(double(I(Py:Py+Ty,Px:Px+Tx,:)),nbin);
pu = particle_hsvhist2((I(Py:Py+Ty,Px:Px+Tx,:)),nbin);
%----------------------------------------------------
% Main Loop
for ii=2:nb_frame-offset_frame
    I_ori = read(video, ii+offset_frame);
    I_now = read(video, ii+offset_frame);
    for jj=1:Particle_num
        % 计算每个粒子特征
        Px = floor(min(State(1,jj),dim_x));
        Py = floor(min(State(2,jj),dim_y));
        Tx = State(3,jj);
        Ty = State(4,jj);
        Top1 = min(Px+Tx,dim_x);
        Top2 = min(Py+Ty,dim_y);
        Bot1 = max(Px,1);
        Bot2 = max(Py,1);
        if (Top1<Bot1 || Top2<Bot2)
            Tempt = zeros(Ty,Tx,3);
        else
            Tempt = I_now(Bot2:Top2,Bot1:Top1,:);
        end
        
        % 提取特征
%         qu(jj,:) = particle_colorhist(double(Tempt),nbin);
        qu(jj,:) = particle_hsvhist2((Tempt),nbin);
        thou(jj) = sum(sqrt((qu(jj,:).*pu)));
        
        pai(jj) = 1/sqrt(2*pi)/sigma_weight*exp(-(1-thou(jj))/2/sigma_weight^2);

   end
    %t_tpou = max( thou );
    % update the feature
    pai_total = sum(pai)
    pai2 = pai/pai_total;
    ind_feat = find(pai == max(pai));
%     Feat0 = Feat(ind_feat(1),:);       %最大后验
%     iPx = State(1,ind_feat(1));
%     iPy = State(2,ind_feat(1));
%     iTx = State(3,ind_feat(1));
%     iTy = State(4,ind_feat(1)); 

%     pu = qu(ind_feat(1),:);
    iPx = floor(State(1,:)*pai2')+1;  %均值
    iPy = floor(State(2,:)*pai2')+1;
    iTx = floor(State(3,:)*pai2');
    iTy = floor(State(4,:)*pai2'); 
    
%     temp = Tempt(:);
%     f1 = hist(temp(:),Nh);
%     Feat0 = f1/sum(f1);
    
    % resamping
    cdf = cumsum(pai2);
    for kk=1:Particle_num
        ind_k = find(cdf>=rand);
        reind(kk) = ind_k(1);
    end
    
    State1 = State(:,reind);
    

    State1(1,:) = State1(1,:) + sigma_px * randn(1,Particle_num);
    State1(2,:) = State1(2,:) + sigma_py * randn(1,Particle_num);
    State1(3,:) = State1(3,:) + sigma_tx * randn(1,Particle_num);
    State1(4,:) = State1(4,:) + sigma_ty * randn(1,Particle_num);
    
    State = State1;
            
    % display    
    imshow(I_now);
    title(sprintf('Frame = %d',ii+offset_frame));
    hold on
    x = [iPx iPx+iTx iPx+iTx iPx iPx];
    y = [iPy iPy iPy+iTy iPy+iTy iPy];

    for k=1:Particle_num
        scatter(State(1,k)+Tx/2,State(2,k)+Ty/2,'.b');
    end
    
    plot(x,y,'r-');
    hold off
%     pause,
    filename = sprintf('%d.bmp', ii);
    saveas(fig, filename, 'bmp');
end










 