function CreateExamStruct(filename,outputname)
%CreateExamStruct(filename) 
% read a exam file from sonixTouch and convert 
% it into a structure {header,pictures}

[all_pic,header] = RPread(filename);
str = struct('header',header,'all_pic',all_pic);
save(outputname,'str');

end

