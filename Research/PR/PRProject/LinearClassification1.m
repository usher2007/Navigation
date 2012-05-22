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
    m_bedroom = mean(bedroom_sample, 2);
    m_house = mean(house_sample, 2);
    m_industry = mean(industry_sample, 2);
    
    %Euclidean distance for bedroom and house
    wrong_count = 0;
    for k=1:108
        bedroom_case = bedroom_test(:,k);
        house_case = house_test(:,k);
        industry_case = industry_test(:,k);
        
        weight_bedroom_bedroom = bedroom_case' * m_bedroom - 0.5 * (m_bedroom' * m_bedroom);
        weight_bedroom_house = bedroom_case' * m_house - 0.5 * (m_house' * m_house);
        weight_bedroom_industry = bedroom_case' * m_industry - 0.5 * (m_industry' * m_industry);
        if weight_bedroom_bedroom < weight_bedroom_house || weight_bedroom_bedroom < weight_bedroom_industry
            wrong_count = wrong_count + 1;
        end
        
        weight_house_bedroom = house_case' * m_bedroom - 0.5 * (m_bedroom' * m_bedroom);
        weight_house_house = house_case' * m_house - 0.5 * (m_house' * m_house);
        weight_house_industry = house_case' * m_industry - 0.5 * (m_industry' * m_industry);
        if weight_house_house < weight_house_bedroom || weight_house_house < weight_house_industry
            wrong_count = wrong_count + 1;
        end
        
        weight_industry_bedroom = industry_case' * m_bedroom - 0.5 * (m_bedroom' * m_bedroom);
        weight_industry_house = industry_case' * m_house - 0.5 * (m_house' * m_house);
        weight_industry_industry = industry_case' * m_industry - 0.5 * (m_industry' * m_industry);
        if weight_industry_industry < weight_industry_bedroom || weight_industry_industry < weight_industry_house
            wrong_count = wrong_count + 1;
        end
    end
    wrong_counts(i) = wrong_count;
end

ave_wrong_count = mean(wrong_counts);


