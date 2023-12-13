clear all;

load('hw4_prob1.mat');

% Define network architecture
input_size = 16;
hidden_size1 = 128;
hidden_size2 = 128;
output_size = 16;

% Initialize weights and biases
% W1 = -1 + 2*randn(hidden_size1, input_size);
% b1 = -1 + 2*randn(hidden_size1, 1);
% W2 = -1 + 2*randn(hidden_size2, hidden_size1);
% b2 = -1 + 2*randn(hidden_size2, 1);
% W3 = -1 + 2*randn(output_size, hidden_size2);
% b3 = -1 + 2*randn(output_size, 1);

W1 = randn(hidden_size1, input_size);
b1 = randn(hidden_size1, 1);
W2 = randn(hidden_size2, hidden_size1);
b2 = randn(hidden_size2, 1);
W3 = randn(output_size, hidden_size2);
b3 = randn(output_size, 1);

% Hyperparameters
learning_rate = 0.005;
num_epochs = 250000;

% Actual Training Data: training_x and training_y

% Training data (x) and target data (y)
x = rand(input_size, 1);  % Replace with your actual input data
y = rand(output_size, 1);  % Replace with your actual target data

% Training loop
for epoch = 1:num_epochs
    % for sample_num = 1:50
        sample_num = randi([1,50000]); % mod(epoch,50000);
        % Forward pass, x size 16*50000
        z1 = W1 * training_x(:,sample_num) + b1; % W1  128*16, z1 128*50000
        a1 = max(0, z1);  % ReLU activation for the first layer
    
        z2 = W2 * a1 + b2; % W2  128*128
        a2 = max(0, z2);  % ReLU activation for the second layer
    
        z3 = W3 * a2 + b3; % W3  16*128
        y_pred = exp(z3)/sum(exp(z3));
        
        % Compute cross-entropy loss
        loss = -sum(training_y(:,sample_num) .* log(y_pred));
        % fprintf('Sample_num:%d | loss:%d\n',sample_num,loss);

        % Backpropagation
        delta3 = y_pred - training_y(:,sample_num);
        delta2 = (W3' * delta3) .* (z2 > 0);
        delta1 = (W2' * delta2) .* (z1 > 0);
    
        % if mod(epoch,10000) == 0 
             % disp(delta3 * a2');
        % end
        
        % Gradient descent update
        W3 = W3 - learning_rate * (delta3 * a2');
        b3 = b3 - learning_rate * delta3;
        W2 = W2 - learning_rate * (delta2 * a1');
        b2 = b2 - learning_rate * delta2;
        W1 = W1 - learning_rate * (delta1 * training_x(:,sample_num)');
        b1 = b1 - learning_rate * delta1;
        % end
        if mod(epoch,10000) == 0
            fprintf('Epoch:%d | loss:%d\n',epoch,loss);
            disp(sample_num);
        end
end



%%
correct_num = 0;

for i = 1:1000
    % Make predictions
    z1_test = W1 * test_x(:,i) + b1; % W1  128*16, z1 128*50000
    a1_test = max(0, z1_test);  % ReLU activation for the first layer

    z2_test = W2 * a1_test + b2; % W2  128*128
    a2_test = max(0, z2_test);  % ReLU activation for the second layer

    z3_test = W3 * a2_test + b3; % W3  16*128
    y_pred_test = exp(z3_test)/sum(exp(z3_test));

    [val1,pred_index] = max(y_pred_test);
    [val2,true_index] = max(test_y(:,i));

    if (pred_index == true_index) 
        correct_num = correct_num+1;
    end
end

correct_rate = correct_num/1000;


