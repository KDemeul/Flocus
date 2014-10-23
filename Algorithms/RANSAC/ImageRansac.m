function [HRNS,CRNS,DRNS,TRNS,Inliers] = ImageRansac(im,rect,Jinit,maxRANSAC)
%IMAGERANSAC Apply RANSAC alg to the sub rect of the image

%% global parameters

normalize = 1;
hist_equa = 0;
percent_th = 10;
use_4_points = 1;

%% Image amelioration

whole_im = im;

% Crop frame
x = rect(1);
y = rect(2);
w = rect(3);
h = rect(4);
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
for i=1:maxRANSAC
    CRNS = Inf;
    while CRNS == Inf
        eta = exp(Jinit*log(1-0.5^2));
        if(use_4_points == 1)
            [HRNS_tmp,CRNS,DRNS_tmp,TRNS_tmp,Inliers_tmp] = RANSAC4PT(bool_map,eta,1);
        else
            [HRNS_tmp,CRNS,DRNS_tmp,TRNS_tmp,Inliers_tmp] = RANSAC2PT(bool_map,eta,1);   
    end
    if(CRNS < best_CRNS)
        best_CRNS = CRNS;
        HRNS = HRNS_tmp;
        DRNS = DRNS_tmp;
        TRNS = TRNS_tmp;
        Inliers = Inliers_tmp;
    end
end

end

