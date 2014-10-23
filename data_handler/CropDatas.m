function struc_crop = CropDatas(struc,x,y,w,h)
%CropData return the new struct {header,all_pic} 
% in which each pic starts at (x,y) 
% with width w and height h

header_crop = struc.header;

header_crop.w = w;
header_crop.h = h;

for i=1:header_crop.nframes
    all_pic_crop(:,:,i) = struc.all_pic(x:x+w-1,y:y+h-1,i);
end

struc_crop = struct('header',header_crop,'all_pic',all_pic_crop);

end

