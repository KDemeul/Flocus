close all

%% global parameters

normalize = 1;
hist_equa = 0;
percent_th = 10;

%% Image amelioration


% Select n-th frame
n=100;
im = squeeze(str.all_pic(:,:,n));
whole_im = im;


% Crop frame
x = 120;
y = 1;
w = 80;
h = 640;
im = im(x:x+w-1,y:y+h-1);
N = length(im(:));

% Normalize frame
if normalize==1
mu = sum(im(:))/N;
sigma2 = sum((im(:)-mu).*(im(:)-mu))/(N-1);
im = (im - mu)/sqrt(sigma2);
end

% Map image to [0,1]
im_max = max(im(:));
im_min = min(im(:));
im = (im-im_min)/(im_max - im_min);

% Histogram equalization
if hist_equa==1
    % Histogram normalized image
    im = histeq(im);
end

% Keep fraction of brightest pixels
histo = imhist(im);
count_brightest = 0;
index = 256;
while count_brightest < percent_th*N/100
    count_brightest = count_brightest + histo(index);
    index = index - 1;
end
threshold = index/255;
bool_map = (im >= threshold);


best_CRNS = Inf;
for i=1:1
    CRNS = Inf;
    while CRNS == Inf
    [HRNS_tmp,CRNS,DRNS_tmp,TRNS_tmp,Inliers_tmp] = RANSAC4PT(bool_map,0.5,1,10);
    end
    if(CRNS < best_CRNS)
        CRNS = best_CRNS;
        HRNS = HRNS_tmp;
        DRNS = DRNS_tmp;
        TRNS = TRNS_tmp;
        Inliers = Inliers_tmp;
    end
end


%% Display part

% fg1 = figure(1);
% set(fg1, 'Position', [0 0 length(whole_im(1,:)) length(whole_im(:,1))]);
% imagesc(whole_im)
% colormap(gray)
% line('XData', [y y+h-1],            'YData', [x x],         'LineStyle', '-.','LineWidth', 1, 'Color','red');
% line('XData', [y y+h-1],            'YData', [x+w-1 x+w-1], 'LineStyle', '-.','LineWidth', 1, 'Color','red');
% line('XData', [max(y,2) max(y,2)],  'YData', [x x+w-1],     'LineStyle', '-.','LineWidth', 1, 'Color','red');
% line('XData', [y+h-1 y+h-1],        'YData', [x x+w-1],     'LineStyle', '-.','LineWidth', 1, 'Color','red');


y_axis = y:(y+h)/100:(y+h-1);
x_axis = x+HRNS(1,1)+HRNS(1,2)*(y_axis-HRNS(2,1))/HRNS(2,2);

% fg2 = figure(2)
% hold on
% subplot(3,1,1)
% imshow(im)
% subplot(3,1,2)
% imshow(bool_map)
% subplot(3,1,3)
% imshow(im)
% hold on
% scatter(Inliers(2,:),Inliers(1,:))
% plot(y_axis,x_axis);
% set(fg2, 'Position', [length(whole_im(1,:)) 0 length(whole_im(1,:)) length(whole_im(:,1))]);

% fg3 = figure(3);
figure
imshow(whole_im/max(whole_im(:)))
colormap(gray)
hold on
plot(y_axis,x_axis,'red','LineWidth',2);
% set(fg3, 'Position', [2*length(whole_im(1,:)) 0 length(whole_im(1,:)) length(whole_im(:,1))]);

