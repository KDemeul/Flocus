close all

filename = 'Result_video/tmp.avi';

x = 120;
y = 1;
w = 80;
h = 640;
nb_loop = 1;

frame_treated = 10;
aviobj=VideoWriter(filename);
open(aviobj);
hFig=figure('Visible','Off');

for i=1:str.header.nframes
    cla
    im = squeeze(str.all_pic(:,:,i));
    
    % Drawing image
    imshow(im/max(im(:)));
    colormap(gray)
    hold on
    
    % Image processing - nb loop RANSAC
    if(mod(i,frame_treated)==0 || i==1)
        [HRNS,CRNS,DRNS,TRNS,Inliers] = ImageRansac(im,[x,y,w,h],10000,nb_loop);
    end
    
    % Draw axis estimate & inliers
    
%     y_axis = y:(y+h)/100:(y+h-1);
%     x_axis = x+HRNS(1,1)+HRNS(1,2)*(y_axis-HRNS(2,1))/HRNS(2,2);
%     plot(y_axis,x_axis,'red','LineWidth',2);  
    
    t = [ones(1,201);(-1000:10:1000)];
    axis = HRNS*t;
    plot(axis(2,:),x+axis(1,:),'red','LineWidth',2);
    
    if((mod(i,frame_treated)==0 || i==1) & size(Inliers)~=[0,0])
        scatter(Inliers(2,:),x+Inliers(1,:),...
          'MarkerEdgeColor',[0 .5 .5],...
          'MarkerFaceColor',[0 .7 .7],...
          'LineWidth',1.5);
    end
    
    % Write image to avi file
    img = hardcopy(hFig, '-dzbuffer', '-r0');
    writeVideo(aviobj, im2frame(img));
    clc
    sprintf('Done: %d%%',round(i/str.header.nframes*100))
end

close(aviobj);
aviobj.FrameRate = str.header.ss;

implay(filename);