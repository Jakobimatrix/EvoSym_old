clear;

T_stern = @(dt,T)(1/(T/dt + 1));
T = @(dt,T_s)(dt*(1/T_s -1));

time_factor = 12/(60*60*24*365);
Periode = 60*60*24*365*time_factor;

year = 60*60*24*365;
deltaT = 60*60*24;


T(deltaT,0.05)


%pt_T ~memory: 1.0 strong delay | 1.99 no delay
%pt_K ~dependancy: 0.9 depends on last layer | 1.0 depends on air layer

%water temperature ocean
% assumed to have a week memory (water) and a strong dependency to the last constant layer (energy in ocean) https://en.wikipedia.org/wiki/Thermocline#/media/File:ThermoclineSeasonDepth.png
% depth_ground = 3; %meter
% resolution = 0.25; %meter
% pt_T = 0.1; 
% pt_K = 1;  
% last_layer_temp = 10;%°C //tropic=24; //polar = -2;
% temp_max = -10; %°C //tropic=30; //polar = -10;
% temp_min = -50;%°C  //tropic=30; //polar = -50;
% topLayer = 0.33;
% botLayer = 0.67;

%watertemp lake
% depth_ground = 6; %meter
% resolution = 1; %meter
% pt_T = T(deltaT,0.05);%0.1; 
% pt_K = 1;  
% last_layer_temp = 4;%°C //tropic=24; //polar = -2;
% temp_max = 25; %°C //tropic=30; //polar = -10;
% temp_min = -10;%°C  //tropic=30; //polar = -50;
% topLayer = 0.55;
% botLayer = 0.45;


%soil temperature dessert: https://www.journals.uchicago.edu/doi/abs/10.1086/333411?journalCode=botanicalgazette
% at 4cm between 17 - 70°C
%at 100cm const 24 °C
depth_ground = 1; %meter
resolution = 0.25; %meter
pt_T = T(deltaT,0.5); 
pt_K = 1;  
topLayer = 0.33;
botLayer = 0.67;
last_layer_temp = 24;%°C
temp_max = 70; %°C 
temp_min = 30;%°C  

% soil temperature meaddow: https://de.wikipedia.org/w/index.php?title=Datei:Bodentemperatur.png&filetimestamp=20110410101339&
% best parameter fit:
% depth_ground = 2; %meter
% resolution = 0.25; %meter
% pt_T = 0.3; 
% pt_K = 1;  
% last_layer_temp = 10;%°C
% temp_max = 20; %°C
% temp_min = -5;%°C
% topLayer = 0.51;
% botLayer = 0.49;


% soil temperature tropical forest: https://de.wikipedia.org/w/index.php?title=Datei:Bodentemperatur.png&filetimestamp=20110410101339&
% best parameter fit:
% depth_ground = 2; %meter
% resolution = 0.25; %meter
% pt_T = 0.1; 
% pt_K = 1;  
% last_layer_temp = 24;%°C
% temp_max = 30; %°C
% temp_min = 25;%°C
% topLayer = 0.65;
% botLayer = 0.35;

% soil temperature moor:
% assuming strong correlation to the air temperature (fast freezing) and a weak memory (water) :
% depth_ground = 2; %meter
% resolution = 0.25; %meter
% pt_T = 0.2; 
% pt_K = 1;  
% topLayer = 0.44;
% botLayer = 0.56;
% last_layer_temp = 10;%°C //tropic=24; //polar = -2;
% temp_max = 20; %°C 
% temp_min = -10;%°C 

% soil temperature highlands: https://de.wikipedia.org/w/index.php?title=Datei:Bodentemperatur.png&filetimestamp=20110410101339&
% best parameter fit:
% depth_ground = 2; %meter
% resolution = 0.25; %meter
% pt_T = 201600; %T(deltaT,0.3)
% pt_K = 1;  
% last_layer_temp = 10;%°C
% temp_max = 20; %°C
% temp_min = -5;%°C
% topLayer = 0.55;
% botLayer = 0.45;

% soil temperature mountain:
% assuming strong correlation to the air temperature (fast freezing) and a weak connection to the last layer :
% depth_ground = 2.5; %meter
% resolution = 0.25; %meter
% pt_T = 0.15; 
% pt_K = 1;  
% topLayer = 0.50;
% botLayer = 0.50;
% last_layer_temp = 5;%°C //tropic=24; //polar = -2;
% temp_max = 20; %°C 
% temp_min = -30;%°C 

temperature = @(x) ((0.5*sin(2*pi/Periode*x))+0.5)*(temp_max-temp_min)+temp_min;%assuming the temp over one year is a sin curve


simulation_time = (year*100)*time_factor; %seconds
simulation_deltaTime = deltaT*time_factor; %seconds

t_step = 0:simulation_deltaTime:simulation_time;
t_steps = length(t_step);


amount_layers = depth_ground/resolution +1; %+ 1 for the air
temp_calc = zeros(t_steps, amount_layers); %thats where all the temperatures for every layer will be stored
temp_calc(1,:) = last_layer_temp; %every layer has at the beginning the temp of the last const layer



for t = 2 : t_steps %begin at t = 2 since we dont know the temperature at time -1
  time = t_step(t);  
  temp_calc(t,1) = temperature(time); %air temperature at time t;
  
  for depth = 2: amount_layers
      
    % assume pt1 behaviour 
    % T * y'(depth,t-1) +y(depth,t) = K * 1/2*[u(depth+resolution,t) + u(depth-resolution,t)]
    % time-discrete PT1: 
    % temp(depth,t) = T*[K* 1/2(temp(depth+resolution,t) + temp(depth-resolution,t)) - temp(depth,t-1)] + temp(depth,t-1)

    if depth == amount_layers %The last layer does not have a layer beneth it OR we assume that the last layer is constant!
        temp_calc(t, depth) = T_stern(deltaT, pt_T)*(pt_K * (botLayer*last_layer_temp + topLayer*temp_calc(t, depth-1)) - temp_calc(t-1, depth) ) + temp_calc(t-1, depth); 
    else
        temp_calc(t, depth) = T_stern(deltaT, pt_T)*(pt_K * (botLayer*temp_calc(t-1, depth+1) + topLayer*temp_calc(t, depth-1)) - temp_calc(t-1, depth) ) + temp_calc(t-1, depth); 
    end 
  end 
end

figure;
hold on;

for i = 1 : 1 : amount_layers
    %if i == 1 || i == 2 || i == 12 || i == 25 ||i == 50 ||i == 100 ||i == 150 || i == 200
        plot(t_step(end-year/deltaT+1:end),temp_calc(end-year/deltaT+1:end,i), 'DisplayName',[num2str((i-1)*resolution*100) 'cm']);  %print only th last year
        legend(gca,'show');
    %end
end

hold off;