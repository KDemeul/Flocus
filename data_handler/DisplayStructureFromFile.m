function DisplayStructureFromFile(filename)
%DisplayStructure animates a matlab structure
% file created from CreateExamStruc.m

load(filename);

for i=1:length(str.all_pic(1,1,:))
    imagesc(squeeze(str.all_pic(:,:,i)));
    colormap(gray);
    M(i) = getframe;
end
movie(M,1,str.header.ss);

end

