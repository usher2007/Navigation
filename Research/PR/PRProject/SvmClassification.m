close all, clear all, clc;
bedroom = load('bedroom.mat');
house = load('CALsuburb.mat');
industry = load('industrial.mat');
bedroom_sample = zeros(1000, 108);
bedroom_test = zeros(1000, 108);
house_sample = zeros(1000, 120);
house_test = zeros(1000, 120);
industry_sample = zeros(1000, 155);
industry_test = zeros(1000, 155);

bedroom_and_house = zeros(1000, 228);
bedroom_and_house_labels = cell(228,1);
house_and_industry = zeros(1000, 275);
house_and_industry_labels = cell(275,1);
industry_and_bedroom = zeros(1000, 263);
industry_and_bedroom_labels = cell(263,1);

bedroom_and_house_labels(1:108) = {'bedroom'};
bedroom_and_house_labels(109:228) = {'house'};
house_and_industry_labels(1:120) = {'house'};
house_and_industry_labels(121:275) = {'industry'};
industry_and_bedroom_labels(1:155) = {'industry'};
industry_and_bedroom_labels(156:263) = {'bedroom'};

wrong_bedroom_counts = zeros(1,5);
wrong_house_counts = zeros(1,5);
wrong_industry_counts = zeros(1,5);
for i=1:5
    rand_order = randperm(216);
    sample_pos = rand_order(1, 1:108);
    test_pos = rand_order(1,109:216);
    for j=1:108
        bedroom_sample(:,j) = bedroom.aver_fea(:,sample_pos(j));
        bedroom_test(:,j) = bedroom.aver_fea(:,test_pos(j));
        
    end
    rand_order = randperm(240);
    sample_pos = rand_order(1, 1:120);
    test_pos = rand_order(1,121:240);
    for j=1:120
        house_sample(:,j) = house.aver_fea(:,sample_pos(j));
        house_test(:,j) = house.aver_fea(:,test_pos(j));
    end
    rand_order = randperm(310);
    sample_pos = rand_order(1, 1:155);
    test_pos = rand_order(1,156:310);
    for j=1:155
        industry_sample(:,j) = industry.aver_fea(:,sample_pos(j));
        industry_test(:,j) = industry.aver_fea(:,test_pos(j));
    end
    bedroom_and_house(:, 1:108) = bedroom_sample(:,1:108);
    bedroom_and_house(:, 109:228) = house_sample(:,1:120);
    house_and_industry(:, 1:120) = house_sample(:,1:120);
    house_and_industry(:, 121:275) = industry_sample(:,1:155);
    industry_and_bedroom(:,1:155) = industry_sample(:,1:155);
    industry_and_bedroom(:,156:263) = bedroom_sample(:,1:108);
    
    svm_bedroom_and_house = svmtrain(bedroom_and_house',bedroom_and_house_labels);
    svm_house_and_industry = svmtrain(house_and_industry', house_and_industry_labels);
    svm_industry_and_bedroom = svmtrain(industry_and_bedroom', industry_and_bedroom_labels);
    
    %Euclidean distance for bedroom and house
    wrong_count_bedroom = 0;
    wrong_count_house = 0;
    wrong_count_industry = 0;
    res_bedroom_of_house = svmclassify(svm_bedroom_and_house, bedroom_test');
    res_bedroom_of_industry = svmclassify(svm_industry_and_bedroom, bedroom_test');
    res_house_of_bedroom = svmclassify(svm_bedroom_and_house, house_test');
    res_house_of_industry = svmclassify(svm_house_and_industry, house_test');
    res_industry_of_bedroom = svmclassify(svm_industry_and_bedroom, industry_test');
    res_industry_of_house = svmclassify(svm_house_and_industry, industry_test');
    
    for k=1:108
        if ~strcmp(res_bedroom_of_house{k},'bedroom') || ~strcmp(res_bedroom_of_industry{k},'bedroom')
            wrong_count_bedroom = wrong_count_bedroom + 1;
        end
    end
    for k=1:120
        if ~strcmp(res_house_of_bedroom{k},'house') || ~strcmp(res_house_of_industry{k},'house')
            wrong_count_house = wrong_count_house + 1;
        end
    end
    for k=1:155
        if ~strcmp(res_industry_of_house{k},'industry') || ~strcmp(res_industry_of_bedroom{k},'industry')
            wrong_count_industry = wrong_count_industry + 1;
        end
    end
    
    wrong_bedroom_counts(i) = wrong_count_bedroom;
    wrong_house_counts(i) = wrong_count_house;
    wrong_industry_counts(i) = wrong_count_industry;
    
    wrong_count_bedroom = 0;
    wrong_count_house = 0;
    wrong_count_industry = 0;
end

ave_wrong_bedroom_count = mean(wrong_bedroom_counts)
ave_wrong_house_count = mean(wrong_house_counts)
ave_wrong_industry_count = mean(wrong_industry_counts)
