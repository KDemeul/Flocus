function DisplayStructure(struc,nb_loop)
%DisplayStructure animates a matlab structure
% file created from CreateExamStruc.m

if nargin < 2
    nb_loop = 1;
end
for i=1:length(struc.all_pic(1,1,:))
    imagesc(squeeze(struc.all_pic(:,:,i)));
    colormap(gray);
    axis image
    M(i) = getframe;
end
movie(M,nb_loop,struc.header.ss);

end

