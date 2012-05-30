close all, clear all, clc;
bedroom = load('bedroom.mat');
house = load('CALsuburb.mat');
industry = load('industrial.mat');
dim = 30;
data = [bedroom.aver_fea house.aver_fea industry.aver_fea];
data_reduce_dim = ReduceDim(data, dim);

bedroom_data = data_reduce_dim(:,1:216);
house_data = data_reduce_dim(:,217:216+241);
industry_data = data_reduce_dim(:,216+241:216+241+311);

coef11 = 2 * (120+155)/(120+155+108);
coef12 = 1 * (108)/(120+155+108);
coef21 = 1 * (155)/(120+155);
coef22 = 1 * (120)/(120+155);

bedroom_sample = zeros(dim, 108);
bedroom_test = zeros(dim, 108);
house_sample = zeros(dim, 120);
house_test = zeros(dim, 120);
industry_sample = zeros(dim, 155);
industry_test = zeros(dim, 155);

not_bedroom_sample = zeros(dim, 275);

wrong_bedroom_counts = zeros(1,5);
wrong_house_counts = zeros(1,5);
wrong_industry_counts = zeros(1,5);

for i=1:5
    rand_order = randperm(216);
    sample_pos = rand_order(1, 1:108);
    test_pos = rand_order(1,109:216);
    for j=1:108
        bedroom_sample(:,j) = bedroom_data(:,sample_pos(j));
        bedroom_test(:,j) = bedroom_data(:,test_pos(j));
    end
    rand_order = randperm(240);
    sample_pos = rand_order(1, 1:120);
    test_pos = rand_order(1,121:240);
    for j=1:120
        house_sample(:,j) = house_data(:,sample_pos(j));
        house_test(:,j) = house_data(:,test_pos(j));
    end
    rand_order = randperm(310);
    sample_pos = rand_order(1, 1:155);
    test_pos = rand_order(1,156:310);
    for j=1:155
        industry_sample(:,j) = industry_data(:,sample_pos(j));
        industry_test(:,j) = industry_data(:,test_pos(j));
    end
    
    not_bedroom_sample(:, 1:120) = house_sample(:,1:120);
    not_bedroom_sample(:, 121:275) = industry_sample(:,1:155);
    
    
    
    M11 = mean(bedroom_sample, 2);
    M12 = mean(not_bedroom_sample, 2);
    Cov11 = cov(bedroom_sample');
    Cov12 = cov(not_bedroom_sample');
    
    M21 = mean(house_sample, 2);
    M22 = mean(industry_sample, 2);
    Cov21 = cov(house_sample');
    Cov22 = cov(industry_sample');
    
    %Gauss
    wrong_count_bedroom = 0;
    wrong_count_house = 0;
    wrong_count_industry = 0;
    
    for k=1:108
        bedroom_case = bedroom_test(:,k);
        
        L1_bedroom_bedroom = coef11 * 1/(det(Cov12)^0.5)*exp(-0.5*((bedroom_case - M12)'*inv(Cov12)*(bedroom_case-M12)));
        L1_bedroom_not_bedroom = coef12 * 1/(det(Cov11)^0.5)*exp(-0.5*((bedroom_case - M11)'*inv(Cov11)*(bedroom_case-M11)));
        
       
        if L1_bedroom_bedroom > L1_bedroom_not_bedroom
            wrong_count_bedroom = wrong_count_bedroom + 1;
        end   
    end
    
    for k=1:120
        house_case = house_test(:,k);
        
        L1_house_bedroom = coef11 * 1/(det(Cov12)^0.5)*exp(-0.5*((house_case - M12)'*inv(Cov12)*(house_case-M12)));
        L1_house_not_bedroom = coef12 * 1/(det(Cov11)^0.5)*exp(-0.5*((house_case - M11)'*inv(Cov11)*(house_case-M11)));
        
        L2_house_house = coef21 * 1/(det(Cov22)^0.5)*exp(-0.5*((house_case - M22)'*inv(Cov22)*(house_case-M22)));
        L2_house_industry = coef22 * 1/(det(Cov21)^0.5)*exp(-0.5*((house_case - M21)'*inv(Cov21)*(house_case-M21)));
        if L1_house_bedroom < L1_house_not_bedroom || L2_house_house > L2_house_industry
            wrong_count_house = wrong_count_house + 1;
        end
    end
    
    for k=1:155
        industry_case = industry_test(:,k);
        
        L1_industry_bedroom = coef11 * 1/(det(Cov12)^0.5)*exp(-0.5*((industry_case - M12)'*inv(Cov12)*(industry_case-M12)));
        L1_industry_not_bedroom = coef12 * 1/(det(Cov11)^0.5)*exp(-0.5*((industry_case - M11)'*inv(Cov11)*(industry_case-M11)));
        
        L2_industry_house = coef21 * 1/(det(Cov22)^0.5)*exp(-0.5*((industry_case - M22)'*inv(Cov22)*(industry_case-M22)));
        L2_industry_industry = coef22 * 1/(det(Cov21)^0.5)*exp(-0.5*((industry_case - M21)'*inv(Cov21)*(industry_case-M21)));
        if L1_industry_bedroom < L1_industry_not_bedroom || L2_industry_house < L2_industry_industry
            wrong_count_industry = wrong_count_industry + 1;
        end
    end
    
    wrong_bedroom_counts(i) = wrong_count_bedroom;
    wrong_house_counts(i) = wrong_count_house;
    wrong_industry_counts(i) = wrong_count_industry;
end

ave_wrong_bedroom_count = mean(wrong_bedroom_counts)
ave_wrong_house_count = mean(wrong_house_counts)
ave_wrong_industry_count = mean(wrong_industry_counts)