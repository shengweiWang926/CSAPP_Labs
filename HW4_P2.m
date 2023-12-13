clear all

load('hw4_prob2.mat');

% what do you mean?

% sample sin and cos value


% y_pb_low_SNR_amp = zeros(1,length(y_pb_low_SNR));
% % channel length modulation
% for i = 1:length(y_pb_low_SNR)
%     real_val = real(y_pb_low_SNR(i));
%     imag_val = imag(y_pb_low_SNR(i));
%     temp_amp = sqrt((real_val^2)+(imag_val^2));
% 
%     if imag_val > 0
%         y_pb_low_SNR_amp(i) = temp_amp;
%     else
%         y_pb_low_SNR_amp(i) = -temp_amp;
%     end
% 
% end
% 
% plot(y_pb_low_SNR_amp);
% 
% 
% 
% y_pb_high_SNR_amp = zeros(1,length(y_pb_high_SNR));
% % channel length modulation
% for i = 1:length(y_pb_high_SNR)
%     real_val_high = real(y_pb_high_SNR(i));
%     imag_val_high = imag(y_pb_high_SNR(i));
%     temp_amp_high = sqrt((real_val_high^2)+(imag_val_high^2));
% 
%     if imag_val_high > 0
%         y_pb_high_SNR_amp(i) = temp_amp_high;
%     else
%         y_pb_high_SNR_amp(i) = -temp_amp_high;
%     end
% 
% end

% figure(2);
% plot(y_pb_high_SNR_amp);


% y_pb_high_SNR = 

% frequency down conversion

% real_high_snr = real(y_pb_high_SNR);
% imag_high_snr = imag(y_pb_high_SNR_amp);

% Generate a local oscillator signal at the downconversion frequency
Fs = 12e6;  % Sampling frequency
fc = 3.2e6;   % Downconversion frequency
t_high_SNR = 0:1/Fs:2;  % Time vector
t_high_SNR = t_high_SNR(1:length(y_pb_high_SNR));
Down_convert_sin = sin(2*pi*fc*t_high_SNR);
Down_convert_cos = cos(2*pi*fc*t_high_SNR);
Down_convert_complex = Down_convert_cos - 1i*Down_convert_sin;

Down_convert_result = y_pb_high_SNR .* Down_convert_complex;

% figure(1);
% subplot(2,1,1);
% plot(real(y_pb_high_SNR));
% subplot(2,1,2);
% plot(imag(y_pb_high_SNR));



% Down_convert_result_Lp = lowpass(Down_convert_result,fc,Fs);

% lpf = designfilt('lowpassfir', 'FilterOrder', 8, ...
%     'PassbandFrequency', fc, 'DesignMethod', 'butter', ...
%     'SampleRate', Fs);

order = 8;  % Filter order
cutoff_frequency = 4e5; % 3.2e6;  % Cutoff frequency in Hz

% Design a low-pass Butterworth filter using the butter function
[b, a] = butter(order, cutoff_frequency/(Fs/2), 'low');

Down_convert_result_Lp = filter(b,a, Down_convert_result);
% figure;
% scatter(real(Down_convert_result_Lp), imag(Down_convert_result_Lp), 'b', 'filled');
% grid on;

% figure(2);
% subplot(2,1,1);
% plot(real(Down_convert_result));
% subplot(2,1,2);
% plot(imag(Down_convert_result));
% 
% 
% figure(3);
% subplot(2,1,1);
% plot(real(Down_convert_result_Lp));
% subplot(2,1,2);
% plot(imag(Down_convert_result_Lp));



% matched filtering: rectangle pulse
T_sym = 1/300000;
Ts = 1/12000000;

% matched_filter = (1/sqrt(T_sym)) * ones([1,T_sym/Ts]);
matched_filter = ones([1,T_sym/Ts]);

matched_filter_result = conv(Down_convert_result_Lp,matched_filter);

% matched_filter_result_cut = matched_filter_result(20:(29873-20));
% down_sampled_v1 = matched_filter_result_cut(21:40:length(matched_filter_result_cut));
down_sampled_v2 = matched_filter_result(40:40:length(matched_filter_result));
% down_sampled_v3 = matched_filter_result(1:40:length(matched_filter_result));

% figure(1);
% subplot(2,1,1);
% plot(abs(real(matched_filter_result)));
% subplot(2,1,2);
% plot(abs(imag(matched_filter_result)));
% 
% 
% figure(2);
% subplot(2,1,1);
% plot(abs(real(down_sampled_v2)));
% subplot(2,1,2);
% plot(abs(imag(down_sampled_v2)));


arsenal = down_sampled_v2;% (120:646);
t = 1:length(down_sampled_v2);
figure(1);
scatter(t,abs(down_sampled_v2));

% synchronization and channel estimation

% QAM demapping
% value = [3*sqrt(1/10) sqrt(1/10) -sqrt(1/10) -3*sqrt(1/10)];
% 
% A = []

figure(2);
scatter(real(arsenal), imag(arsenal), 'b', 'filled');
grid on; 

% Demodulate the received signal
SymMap = [2 3 1 0 6 7 5 4 14 15 13 12 10 11 9 8];

% demodulated_preamble = qamdemod(preamble, 16, SymMap, 'UnitAveragePower', true);
demodulated_preamble = demod(preamble);

index = 1;
start = 1;
while index < length(down_sampled_v2)
    temp_abs = abs(down_sampled_v2(index));
    if (temp_abs > 10)
        start = index;
        break;
    end
    index = index + 1;
end

scrambled_message_symbols = down_sampled_v2(119:(119+512+16-1));

channel_h = scrambled_message_symbols(1:16)./preamble;

mean_channel_h = mean(channel_h);

rotate_back_value = scrambled_message_symbols/mean_channel_h;
figure(5);
scatter(real(rotate_back_value),imag(rotate_back_value))


demodulated_message = demod(rotate_back_value);
% demodulated_message = qamdemod(rotate_back_value,16,SymMap,'UnitAveragePower',true);

message_symbols = [];
for idx = 1:4:2048
    decValue = bin2dec(num2str(message_bits(idx:(idx+3))));
    message_symbols = [message_symbols,decValue];
end

original_message = [demodulated_preamble,message_symbols];

match = 0;
wrong_idx = [];

for i = 1:length(original_message)
    if original_message(i) == demodulated_message(i)
        match = match + 1;
    else 
        wrong_idx = [wrong_idx,i];
    end
end

% after = xcorr(down_sampled_v2);
% t_after = 1:length(after);
% figure(3);
% scatter(t_after,min(abs(after),10000));


%%

clear all

load('hw4_prob2.mat');

% frequency down conversion

% Generate a local oscillator signal at the downconversion frequency
Fs = 12e6;  % Sampling frequency
fc = 3.2e6;   % Downconversion frequency
t_low_SNR = 0:1/Fs:2;  % Time vector
t_low_SNR = t_low_SNR(1:length(y_pb_low_SNR));
Down_convert_sin = sin(2*pi*fc*t_low_SNR);
Down_convert_cos = cos(2*pi*fc*t_low_SNR);
Down_convert_complex = Down_convert_cos - 1i*Down_convert_sin;

% Down_convert_complex = exp(-1i*2*pi*fc*t_low_SNR);

Down_convert_result = y_pb_low_SNR .* Down_convert_complex;

% figure(1);
% subplot(2,1,1);
% plot(real(y_pb_high_SNR));
% subplot(2,1,2);
% plot(imag(y_pb_high_SNR));

order = 8;  % Filter order
cutoff_frequency = 3.6e5; % 3.2e6;  % Cutoff frequency in Hz

% Design a low-pass Butterworth filter using the butter function
[b, a] = butter(order, cutoff_frequency/(Fs/2), 'low');

Down_convert_result_Lp = filter(b,a, Down_convert_result);
% figure;
% scatter(real(Down_convert_result_Lp), imag(Down_convert_result_Lp), 'b', 'filled');
% grid on;

% figure(2);
% subplot(2,1,1);
% plot(real(Down_convert_result));
% subplot(2,1,2);
% plot(imag(Down_convert_result));
% 
% 
% figure(3);
% subplot(2,1,1);
% plot(abs(real(Down_convert_result_Lp)));
% subplot(2,1,2);
% plot(abs(imag(Down_convert_result_Lp)));



% matched filtering: rectangle pulse
T_sym = 1/300000;
Ts = 1/12000000;

% matched_filter = (1/sqrt(T_sym)) * ones([1,T_sym/Ts]);
matched_filter = ones([1,T_sym/Ts]);

matched_filter_result = conv(Down_convert_result_Lp,matched_filter)./40;

down_sampled_v2 = matched_filter_result(24:40:length(matched_filter_result));

% figure(1);
% subplot(2,1,1);
% plot(abs(real(matched_filter_result)));
% subplot(2,1,2);
% plot(abs(imag(matched_filter_result)));
% 

figure(1);
subplot(2,1,1);
plot(abs(real(down_sampled_v2)));
subplot(2,1,2);
plot(abs(imag(down_sampled_v2)));

% synchronization and channel estimation

% figure(2);
% scatter(real(down_sampled_v2), imag(down_sampled_v2), 'b', 'filled');
% grid on; 

% Demodulate the received signal


% index = 1;
% start = 1;
% while index < length(down_sampled_v2)
%     temp_abs = abs(down_sampled_v2(index));
%     if (temp_abs > 10)
%         start = index;
%         break;
%     end
%     index = index + 1;
% end

start = 55;

scrambled_message_symbols = down_sampled_v2(start:(start+512+16-1));

% obtain flat fading channel h
channel_h = scrambled_message_symbols(1:16)./preamble;
mean_channel_h = mean(channel_h);
% mean_channel_h = 40*(-0.2724 - 0.6880i);

% 0.74*exp(j*2*pi*1.69)

% convert back to original symbol level
rotate_back_value = scrambled_message_symbols/mean_channel_h; % ./exp(2*pi*0*1i)/1.05;
figure(3);
scatter(real(rotate_back_value),imag(rotate_back_value),'b', 'filled')

% demodulated_message = qamdemod(rotate_back_value,16,SymMap,'UnitAveragePower',true);

% SymMap = [2 3 1 0 6 7 5 4 14 15 13 12 10 11 9 8];
% demodulated_preamble = qamdemod(preamble, 16, SymMap, 'UnitAveragePower', true);
demodulated_preamble = demod(preamble);

demodulated_message = demod(rotate_back_value);

message_symbols = [];
for idx = 1:4:2048
    decValue = bin2dec(num2str(message_bits(idx:(idx+3))));
    message_symbols = [message_symbols,decValue];
end

original_message = [demodulated_preamble,message_symbols];

match = 0;
wrong_idx = [];

for i = 1:length(original_message)
    if original_message(i) == demodulated_message(i)
        match = match + 1;
    else 
        wrong_idx = [wrong_idx,i];
    end
end



%%
function output = demod(input_symbol_vec)
    symbol_map = [-3-3i -3-1i -3+3i -3+1i -1-3i -1-1i -1+3i -1+1i 3-3i 3-1i 3+3i 3+1i 1-3i 1-1i 1+3i 1+1i];
    symbol_map = symbol_map * (1/sqrt(10));

    output = zeros(1,length(input_symbol_vec));
    
    for idx = 1:length(input_symbol_vec)
        % output(idx) = min(abs(input_symbol_vec(idx)-symbol_map));
        
        [~, index] = min(abs(input_symbol_vec(idx) - symbol_map));
        output(idx) = index-1;
    end
   
end
















