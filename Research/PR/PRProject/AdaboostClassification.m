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

training_data = zeros(1000, 383);
test_data = zeros(1000, 383);

training_labels = cell(383,1);
training_labels(1:108) = {'bedroom'};
training_labels(109:228) = {'house'};
training_labels(229:383) = {'industry'};

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
    training_data = [bedroom_sample house_sample industry_sample];
    test_data = [bedroom_test house_test industry_test];
    
    ada = fitensemble(training_data', training_labels, 'AdaBoostM2', 200, 'tree');
    plot(resubLoss(ada,'mode','cumulative'));
    [test_labels score] = predict(ada, test_data');
    
    wrong_count_bedroom = 0;
    wrong_count_house = 0;
    wrong_count_industry = 0;
    for k=1:383
        if k>0 && k<=108
            if ~strcmp(test_labels{k},'bedroom')
                wrong_count_bedroom = wrong_count_bedroom + 1;
            end
        end
        if k>108 && k<=228
            if ~strcmp(test_labels{k},'house')
                wrong_count_house = wrong_count_house + 1;
            end
        end
        if k>228 && k<=383
            if ~strcmp(test_labels{k},'industry')
                wrong_count_industry = wrong_count_industry + 1;
            end
        end
    end
    wrong_bedroom_counts(i) = wrong_count_bedroom;
    wrong_house_counts(i) = wrong_count_house;
    wrong_industry_counts(i) = wrong_count_industry;
end

ave_wrong_bedroom_count = mean(wrong_bedroom_counts)
ave_wrong_house_count = mean(wrong_house_counts)
ave_wrong_industry_count = mean(wrong_industry_counts)