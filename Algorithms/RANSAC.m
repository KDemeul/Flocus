function [best_ma,best_mb,best_er] = RANSAC(data,n,k,t,d)
%RANSAC 
 % data - un ensemble d'observations
 % n - le nombre minimum de données nécessaires pour ajuster le modèle
 % k - le nombre maximal d'itérations de l'algorithme
 % t - une valeur seuil pour déterminer si une donnée correspond à un modèle
 % d - le nombre de données proches des valeurs nécessaires pour faire valoir 
 % que le modèle correspond bien aux donnée sorties

best_ma=1;
best_mb=0;
best_er=1;

% ALG
it = 0; 
meilleur_ensemble_points = [];
meilleur_points_alea = [];

%tant que itérateur < k
while it < k
    points_alea = []; 
    nb_point_picked = 0;
    thresh_ids = find(data(:) > 0);
    [x_id,y_id] = ind2sub(size(data),thresh_ids);
    while nb_point_picked < n
        i = randi(length(x_id(:)),1);
        points_alea = [points_alea;[x_id(i) y_id(i)]];
        nb_point_picked = nb_point_picked + 1;
    end
    
    %modèle_possible := paramètres du modèle correspondant aux points_aléatoires
    mean_x = mean(points_alea(:,1));
    mean_y = mean(points_alea(:,2));
    Sx2 = 1 / (n-1) * sum((points_alea(:,1)-mean_x).*(points_alea(:,1)-mean_x));
    Sxy = 1 / (n-1) * sum((points_alea(:,1)-mean_x).*(points_alea(:,2)-mean_y));
    
    ma = Sxy / Sx2;
    mb = mean_y - ma * mean_x;
    
    %ensemble_points := points_aléatoires
    ensemble_point = points_alea;
    
  
    %Pour chaque point des données pas dans points_aléatoires
    %    si le point s'ajuste au modèle_possible avec une erreur inférieure à t
    %        Ajouter un point à ensemble_points  
    thresh_ids = find(data(:));
    [x_id,y_id] = ind2sub(size(data),thresh_ids);
    for i=1:1:length(x_id(:))
        dist = abs(ma * x_id(i) - y_id(i) + mb) / sqrt(1 + ma*ma);
            if dist < t
                ensemble_point = [ensemble_point;[x_id(i) y_id(i)]];
            end
    end
    
    %     si le nombre d'éléments dans ensemble_points est > d
    %         (ce qui implique que nous avons peut-être trouvé un bon modèle,
    %         on teste maintenant dans quelle mesure il est correct)

    if length(ensemble_point(:,1)) > d
        % modèle_possible := paramètres du modèle réajusté à tous les points de ensemble_points
        n_select = length(ensemble_point(:,1));
        mean_x = 1 / n_select * sum(ensemble_point(:,1));
        mean_y = 1 / n_select * sum(ensemble_point(:,2));
        Sx2 = 1 / (n_select-1) * sum((ensemble_point(:,1)-mean_x).*(ensemble_point(:,1)-mean_x));
        Sy2 = 1 / (n_select-1) * sum((ensemble_point(:,2)-mean_y).*(ensemble_point(:,2)-mean_y));
        Sxy = 1 / (n_select-1) * sum((ensemble_point(:,1)-mean_x).*(ensemble_point(:,2)-mean_y));

        ma = Sxy / Sx2;
        mb = mean_y - ma * mean_x;
  

      % erreur := une mesure de la manière dont ces points correspondent au modèle_possible
        erreur = sqrt(abs(Sxy*Sxy / (Sx2 * Sy2)));
      
      
       % si erreur < meilleure_erreur
        %    (nous avons trouvé un modèle qui est mieux que tous les précédents,
         %   le garder jusqu'à ce qu'un meilleur soit trouvé)
          % meilleur_modèle := modèle_possible
           %meilleur_ensemble_points := ensemble_points
            %meilleure_erreur := erreur
       if erreur < best_er
            best_er = erreur;
            best_ma = ma;
            best_mb = mb;
            meilleur_ensemble_points = ensemble_point;
            meilleur_points_alea = points_alea;
       end
    end
    it = it + 1;
end

end
