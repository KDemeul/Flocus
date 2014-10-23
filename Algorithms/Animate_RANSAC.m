close all

filename = 'Result_video/RANSAC_only_1call_evry1frame.mp4';

x = 120;
y = 1;
w = 80;
h = 640;

frame_treated = 1;
aviobj=VideoWriter(filename,'MPEG-4');
open(aviobj);
hFig=figure('Visible','Off');

for i=1:str.header.nframes
    cla
    im = squeeze(str.all_pic(:,:,i));
    % Drawing part
    imshow(im/max(im(:)));
    colormap(gray)
    hold on
    
    % Image processing - 1 loop
    if(mod(i,frame_treated)==0 || i==1)
        [HRNS,CRNS,DRNS,TRNS,Inliers] = ImageRansac(im,[x,y,w,h],10000,1);
    end
    y_axis = y:(y+h)/100:(y+h-1);
    x_axis = x+HRNS(1,1)+HRNS(1,2)*(y_axis-HRNS(2,1))/HRNS(2,2);
    
    plot(y_axis,x_axis,'red','LineWidth',2);  
    
    if(mod(i,frame_treated)==0 || i==1)
        scatter(Inliers(2,:),x+Inliers(1,:),...
          'MarkerEdgeColor',[0 .5 .5],...
          'MarkerFaceColor',[0 .7 .7],...
          'LineWidth',1.5);
    end
    
    img = hardcopy(hFig, '-dzbuffer', '-r0');
    writeVideo(aviobj, im2frame(img));
    i
end
close(aviobj);
aviobj.FrameRate = str.header.ss;

implay(filename);