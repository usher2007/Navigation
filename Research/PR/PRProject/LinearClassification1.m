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

wrong_counts = zeros(1,5);
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
    m_bedroom = mean(bedroom_sample, 2);
    m_house = mean(house_sample, 2);
    m_industry = mean(industry_sample, 2);
    
    %Euclidean distance for bedroom and house
    wrong_count = 0;
    wrong_bedroom_count = 0;
    wrong_house_count = 0;
    wrong_industry_count = 0;
    for k=1:108
        bedroom_case = bedroom_test(:,k);
       
        weight_bedroom_bedroom = bedroom_case' * m_bedroom - 0.5 * (m_bedroom' * m_bedroom);
        weight_bedroom_house = bedroom_case' * m_house - 0.5 * (m_house' * m_house);
        weight_bedroom_industry = bedroom_case' * m_industry - 0.5 * (m_industry' * m_industry);
        if weight_bedroom_bedroom < weight_bedroom_house || weight_bedroom_bedroom < weight_bedroom_industry
            wrong_count = wrong_count + 1;
            wrong_bedroom_count = wrong_bedroom_count + 1;
        end
    end
    for k=1:120
        house_case = house_test(:,k);
        
        weight_house_bedroom = house_case' * m_bedroom - 0.5 * (m_bedroom' * m_bedroom);
        weight_house_house = house_case' * m_house - 0.5 * (m_house' * m_house);
        weight_house_industry = house_case' * m_industry - 0.5 * (m_industry' * m_industry);
        if weight_house_house < weight_house_bedroom || weight_house_house < weight_house_industry
            wrong_count = wrong_count + 1;
            wrong_house_count = wrong_house_count + 1;
        end
    end
    for k=1:155
        industry_case = industry_test(:,k);
        weight_industry_bedroom = industry_case' * m_bedroom - 0.5 * (m_bedroom' * m_bedroom);
        weight_industry_house = industry_case' * m_house - 0.5 * (m_house' * m_house);
        weight_industry_industry = industry_case' * m_industry - 0.5 * (m_industry' * m_industry);
        if weight_industry_industry < weight_industry_bedroom || weight_industry_industry < weight_industry_house
            wrong_count = wrong_count + 1;
            wrong_industry_count = wrong_industry_count + 1;
        end
    end
    wrong_counts(i) = wrong_count;
    wrong_bedroom_counts(i) = wrong_bedroom_count;
    wrong_house_counts(i) = wrong_house_count;
    wrong_industry_counts(i) = wrong_industry_count;
end

ave_wrong_count = mean(wrong_counts)
ave_wrong_bedroom_count = mean(wrong_bedroom_counts)
ave_wrong_house_count = mean(wrong_house_counts)
ave_wrong_industry_count = mean(wrong_industry_counts)

