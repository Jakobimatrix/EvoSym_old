%Temperaturen über das Jahr moddeliert als Sinus mit Offset für jede
%Season, welcher sich jedes Jahr Ändert. Die Änderungsrate hat pt1
%verhalten

%Frühling Sommer: 
%http://www.wolframalpha.com/input/?i=(sin(x)+%2B+abs(sin(x)))%2F2+++,+++(sin(x+%2B+pi%2F2)+%2B+abs(sin(x+%2B+pi%2F2+)))%2F2++,+((sin(x)+%2B+abs(sin(x)))%2F2)%5E2++%2B++((sin(x+%2B+pi%2F2)+%2B+abs(sin(x+%2B+pi%2F2+)))%2F2)%5E2
%Frühling Herbst:
%http://www.wolframalpha.com/input/?i=(sin(x)+%2B+abs(sin(x)))%2F2+++,+++(sin(x+%2B+pi)+%2B+abs(sin(x+%2B+pi+)))%2F2++,+((sin(x)+%2B+abs(sin(x)))%2F2)%5E2++%2B++((sin(x+%2B+pi)+%2B+abs(sin(x+%2B+pi+)))%2F2)%5E2
clear;
clc;


year = 60*60*24*365;
time_factor = 12/year;
deltaT = 60*60*24;

nextSeasonTemperature_offset = [5,-3,4,2];
thisSeasonTemperature_offset = [0,0,0,0];
temp_max = 30;
temp_min = -10;

zn = @(x)( (x + abs(x))/2);%equals x if positive, zero if negative
T_stern = @(dt,T)(1/(T/dt + 1));
T = @(dt,T_s)(dt*(1/T_s -1));

pt_T = T_stern(deltaT,T(deltaT,0.001));
T(deltaT,0.001)

simulation_time = (year*25); %seconds

t_step = 0:deltaT:simulation_time;
t_step_show = 0:deltaT*time_factor:simulation_time*time_factor;

temp_calc = zeros(2,length(t_step)); 

rad = 0;
for t = 1 : length(t_step) %begin at t = 2 since we dont know the temperature at time -1
    time = t_step(t); 
    rad = rad + 2*pi / year * deltaT;
    
    
    if rad >= 2*pi
        rad = rad - 2*pi;
        nextSeasonTemperature_offset = [ randn*5 , randn*5, randn*5 , randn*5 ];
    end
   
    for i = 1 : 4
        thisSeasonTemperature_offset(i) = pt_T*(nextSeasonTemperature_offset(i)-thisSeasonTemperature_offset(i))+thisSeasonTemperature_offset(i);
    end
    
    
    seasonOffset = @(rad) zn(sin(rad))^2*thisSeasonTemperature_offset(1) + zn(cos(rad))^2*thisSeasonTemperature_offset(2) + zn(-sin(rad))^2*thisSeasonTemperature_offset(3) +  zn(-cos(rad))^2*thisSeasonTemperature_offset(4)  ;
    temperature = @(x,rad) ((0.5*sin(2*pi/year*x))+0.5)*(temp_max-temp_min)+temp_min+seasonOffset(rad);%assuming the temp over one year is a sin curve

  
    temp_calc(1,t) = temperature(time,rad); 
    temp_calc(2,t) = seasonOffset(rad);
end

figure;
hold on;
plot(t_step_show,temp_calc(1,:));
plot(t_step_show,temp_calc(2,:));
hold off;
