close all, clear all, clc;
bedroom = load('bedroom.mat');
house = load('CALsuburb.mat');
industry = load('industrial.mat');
bedroom_sample = zeros(1000, 108);
bedroom_test = zeros(1000, 108);
house_sample = zeros(1000, 108);
house_test = zeros(1000, 108);
industry_sample = zeros(1000, 108);
industry_test = zeros(1000, 108);

bedroom_and_house = zeros(1000, 216);
bedroom_and_house_labels = cell(216,1);
house_and_industry = zeros(1000, 216);
house_and_industry_labels = cell(216,1);
industry_and_bedroom = zeros(1000, 216);
industry_and_bedroom_labels = cell(216,1);

bedroom_and_house_labels(1:108) = {'bedroom'};
bedroom_and_house_labels(109:216) = {'house'};
house_and_industry_labels(1:108) = {'house'};
house_and_industry_labels(109:216) = {'industry'};
industry_and_bedroom_labels(1:108) = {'industry'};
industry_and_bedroom_labels(109:216) = {'bedroom'};

wrong_counts = zeros(1,5);
for i=1:5
    rand_order = randperm(216);
    sample_pos = rand_order(1, 1:108);
    test_pos = rand_order(1,109:216);
    for j=1:108
        bedroom_sample(:,j) = bedroom.aver_fea(:,sample_pos(j));
        bedroom_test(:,j) = bedroom.aver_fea(:,test_pos(j));
        house_sample(:,j) = house.aver_fea(:,sample_pos(j));
        house_test(:,j) = house.aver_fea(:,test_pos(j));
        industry_sample(:,j) = industry.aver_fea(:,sample_pos(j));
        industry_test(:,j) = industry.aver_fea(:,test_pos(j));
    end
    bedroom_and_house(:, 1:108) = bedroom_sample(:,1:108);
    bedroom_and_house(:, 109:216) = house_sample(:,1:108);
    house_and_industry(:, 1:108) = house_sample(:,1:108);
    house_and_industry(:, 109:216) = industry_sample(:,1:108);
    industry_and_bedroom(:,1:108) = industry_sample(:,1:108);
    industry_and_bedroom(:,109:216) = bedroom_sample(:,1:108);
    
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
        if ~strcmp(res_house_of_bedroom{k},'house') || ~strcmp(res_house_of_industry{k},'house')
            wrong_count_house = wrong_count_house + 1;
        end
        if ~strcmp(res_industry_of_house{k},'industry') || ~strcmp(res_industry_of_bedroom{k},'industry')
            wrong_count_industry = wrong_count_industry + 1;
        end
    end
    
    wrong_count_bedroom
    wrong_count_house
    wrong_count_industry
    
    wrong_count_bedroom = 0;
    wrong_count_house = 0;
    wrong_count_industry = 0;
    %wrong_counts(i) = wrong_count;
end

ave_wrong_count = mean(wrong_counts);
