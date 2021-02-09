% gps.m
%   Compute the output of gps sensor
%
%  Revised:
%   3/5/2010 - RWB 
%   5/14/2010 - RWB
%   2/18/2019 - RWB

function y = gps(uu, SENSOR)

    % relabel the inputs
    Va      = uu(1);
    alpha   = uu(2);
    beta    = uu(3);
    wn      = uu(4);
    we      = uu(5);
    wd      = uu(6);
    pn      = uu(7);
    pe      = uu(8);
    pd      = uu(9);
    u       = uu(10);
    v       = uu(11);
    w       = uu(12);
    phi     = uu(13);
    theta   = uu(14);
    psi     = uu(15);
    p       = uu(16);
    q       = uu(17);
    r       = uu(18);
    t       = uu(19);
    
    persistent gps_eta_n;
    persistent gps_eta_e;
    persistent gps_eta_h;
    persistent k;
    if t == 0
       gps_eta_n = 0;
       gps_eta_e = 0;
       gps_eta_h = 0;
       k = exp(-SENSOR.gps_beta*SENSOR.ts_gps);
    end
    
    gps_eta_n = k*gps_eta_n + normrnd(0, SENSOR.gps_n_sigma);
    gps_eta_e = k*gps_eta_e + normrnd(0, SENSOR.gps_e_sigma);
    gps_eta_h = k*gps_eta_h + normrnd(0, SENSOR.gps_h_sigma);
    
    y_gps_n      = pn + gps_eta_n;
    y_gps_e      = pe + gps_eta_e;
    y_gps_h      = -pd + gps_eta_h;
    
    Pdot = getVehicleToBodyRotationMatrix([phi;theta;psi])' *[u;v;w];
    
    y_gps_Vg     = norm(Pdot) + normrnd(0, SENSOR.gps_Vg_sigma);
    y_gps_course = atan2(Pdot(2), Pdot(1)) + normrnd(0, SENSOR.gps_course_sigma);

    % construct total output
    y = [...
        y_gps_n;...
        y_gps_e;...
        y_gps_h;...
        y_gps_Vg;...
        y_gps_course;...
        ];
    
end



