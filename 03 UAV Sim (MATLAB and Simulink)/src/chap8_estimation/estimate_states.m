% estimate_states
%   - estimate the MAV states using gyros, accels, pressure sensors, and
%   GPS.
%
% Outputs are:
%   pnhat    - estimated North position, 
%   pehat    - estimated East position, 
%   hhat     - estimated altitude, 
%   Vahat    - estimated airspeed, 
%   alphahat - estimated angle of attack
%   betahat  - estimated sideslip angle
%   phihat   - estimated roll angle, 
%   thetahat - estimated pitch angel, 
%   chihat   - estimated course, 
%   phat     - estimated roll rate, 
%   qhat     - estimated pitch rate, 
%   rhat     - estimated yaw rate,
%   Vghat    - estimated ground speed, 
%   wnhat    - estimate of North wind, 
%   wehat    - estimate of East wind
%   psihat   - estimate of heading angle
% 
% 
% Modified:  3/15/2010 - RB
%            5/18/2010 - RB
%

function xhat = estimate_states(uu, MAV, WIND, EKF)

   % rename inputs
   y_gyro_x      = uu(1);
   y_gyro_y      = uu(2);
   y_gyro_z      = uu(3);
   y_accel_x     = uu(4);
   y_accel_y     = uu(5);
   y_accel_z     = uu(6);
   y_static_pres = uu(7);
   y_diff_pres   = uu(8);
   y_gps_n       = uu(9);
   y_gps_e       = uu(10);
   y_gps_h       = uu(11);
   y_gps_Vg      = uu(12);
   y_gps_course  = uu(13);
   t             = uu(14);
   
   persistent p0;
   persistent q0;
   persistent r0;
   persistent y_static_pres0;
   persistent y_diff_pres0;
   persistent xhat_att;
   persistent xhat_gps;
   persistent y_gps_n_old;  % last measurement of gps_n - used to detect new GPS signal
   persistent y_gps_e_old;  % last measurement of gps_e - used to detect new GPS signal
   persistent y_gps_Vg_old; % last measurement of gps_Vg - used to detect new GPS signal
   persistent y_gps_course_old;  % last measurement of gps_course - used to detect new GPS signal
   
   if t == 0
       p0 = MAV.p0;
       q0 = MAV.q0;
       r0 = MAV.r0;
       y_static_pres0 = MAV.rho*MAV.gravity*-MAV.pd0;
       y_diff_pres0 = MAV.rho*WIND.Va0^2/2;
       xhat_att = [MAV.phi0; MAV.theta0];
%        xhat_att = [0, 0];
       xhat_gps = [MAV.pn0; MAV.pe0; WIND.Va0; MAV.psi0; MAV.psi0];
       y_gps_n_old  = -9999;
       y_gps_e_old  = -9999;
       y_gps_Vg_old = -9999;
       y_gps_course_old  = -9999;

   end
    lpf_y_static_pres = lpf(0.9, y_static_pres0, y_static_pres);
    lpf_y_diff_pres = lpf(0.5, y_diff_pres0, y_diff_pres);
    
    hhat = lpf_y_static_pres/(MAV.rho*MAV.gravity);
    Vahat = sqrt((2/MAV.rho)*lpf_y_diff_pres);
    
    phat = lpf(0.25, p0, y_gyro_x);
    qhat = lpf(0.25, q0, y_gyro_y);
    rhat = lpf(0.25, r0, y_gyro_z);

    
    %-------------------------------------------------------------------
    % implement continous-discrete EKF to estimate roll and pitch angles
    phihat = xhat_att(1);
    thetahat = xhat_att(2);
    s_phi = sin(phihat);
    c_phi = cos(phihat);
    t_theta = tan(thetahat);
    % prediction step
    for k=1:EKF.N_att
        G_att = [...
                    1, s_phi*t_theta, c_phi*t_theta;...
                    0, c_phi, -s_phi;...
                ];
        f_att = G_att * [phat; qhat; rhat];
        xhat_att = xhat_att + EKF.Ts_att*f_att;
        xhat_att = wrapToPi(xhat_att);
        phihat = xhat_att(1);
        thetahat = xhat_att(2);
        s_phi = sin(phihat);
        c_phi = cos(phihat);
        s_theta = sin(thetahat);
        c_theta = cos(thetahat);
        t_theta = tan(thetahat);
        A_att = [...
                    qhat*c_phi*t_theta-rhat*s_phi*t_theta, (qhat*s_phi+rhat*c_phi)/(c_theta^2);...
                    -qhat*s_phi-rhat*c_phi, 0;...
                ];
        % Continuous
%         EKF.P_att = EKF.P_att +...
%                     EKF.Ts_att*(A_att*EKF.P_att+EKF.P_att*A_att' + G_att*EKF.Q_gyro*G_att' + EKF.Q_att);
        % Discrete
        Ad_att = eye(2) + EKF.Ts_att*A_att + A_att^2*EKF.Ts_att^2;
        Gd_att = EKF.Ts_att*G_att;
        EKF.P_att = Ad_att*EKF.P_att*Ad_att' + Gd_att*EKF.Q_gyro*Gd_att' + EKF.Q_att;
    end
    
    % Attitude Measurement step
    y_att = [y_accel_x; y_accel_y; y_accel_z];
    h_att = [...
                qhat*Vahat*s_theta + MAV.gravity*s_theta;...
                rhat*Vahat*c_theta - phat*Vahat*s_theta - MAV.gravity*c_theta*s_phi;...
                -qhat*Vahat*c_theta - MAV.gravity*c_theta*c_phi;...
            ];
    C_att = [...
                0, qhat*Vahat*c_theta + MAV.gravity*c_theta;...
                -MAV.gravity*c_phi*c_theta, -rhat*Vahat*s_theta - phat*Vahat*c_theta+MAV.gravity*s_phi*s_theta;...
                MAV.gravity*s_phi*c_theta, (qhat*Vahat+MAV.gravity*c_phi)*s_theta;...
            ];
    
    % The inverse makes it computationally expensive so loop for each sensor
    for k=1:length(y_att)
        Ci = C_att(k, :);
        Li = EKF.P_att*Ci'/(EKF.R_accel(k, k)+Ci*EKF.P_att*Ci');
        EKF.P_att = (eye(2) - Li*Ci)*EKF.P_att;
        if norm(y_att(k)-h_att(k)) < EKF.threshold_att
            xhat_att = xhat_att + Li*(y_att(k)-h_att(k));
            xhat_att = wrapToPi(xhat_att);
        end
    end
    
    phihat = xhat_att(1);
    thetahat = xhat_att(2);
    
    
    %-------------GPS Smoothing-------------
    pnhat = xhat_gps(1);
    pehat = xhat_gps(2);
    Vghat = xhat_gps(3);
    chihat = xhat_gps(4);
    psihat = xhat_gps(5);

    
    % prediction step
    c_chi = cos(chihat);
    s_chi = sin(chihat);
    c_phi = cos(phihat);
    s_phi = sin(phihat);
    t_phi = tan(phihat);
    c_theta = cos(thetahat);
    c_psi = cos(psihat);
    s_psi = sin(psihat);
    
    wnhat  = Vghat*c_chi - Vahat*c_psi;
    wehat  = Vghat*s_chi - Vahat*s_psi;
    
    psidot = qhat*s_phi/c_theta + rhat*c_phi/c_theta;
    Vgdot = ((Vahat*c_psi+wnhat)*(-Vahat*psidot*s_psi)+(Vahat*s_psi+wehat)*(Vahat*psidot*c_psi))/Vghat;
    chidot = MAV.gravity*tan(phihat)*cos(chihat-psihat)/Vghat;
    for k=1:EKF.N_gps
        f_gps = [...
                    Vghat*c_chi;...
                    Vghat*s_chi;...
                    Vgdot;...
                    chidot;...
                    psidot;...
                ];
        xhat_gps = xhat_gps + EKF.Ts_gps*f_gps;
        pnhat  = xhat_gps(1);
        pehat  = xhat_gps(2);
        Vghat  = xhat_gps(3);
        chihat = xhat_gps(4);
        psihat = xhat_gps(5);
        c_chi = cos(chihat);
        s_chi = sin(chihat);
        c_psi = cos(psihat);
        s_psi = sin(psihat);
        c_chi_psi = cos(chihat-psihat);
        s_chi_psi = sin(chihat-psihat);
        Vgdot = ((Vahat*c_psi+wnhat)*(-Vahat*psidot*s_psi)+(Vahat*s_psi+wehat)*(Vahat*psidot*c_psi))/Vghat;
        chidot = MAV.gravity*tan(phihat)*c_chi_psi/Vghat;
        Vgdot_psi = (-psidot*Vahat*(wnhat*c_psi+wehat*s_psi))/Vghat;
        chidot_Vg = -MAV.gravity*t_phi*c_chi_psi/Vghat^2;
        chidot_chi = -MAV.gravity*t_phi*s_chi_psi/Vghat;
        chidot_psi = MAV.gravity*t_phi*s_chi_psi/Vghat;
       
        A_gps = [...
                    0, 0, c_chi, -Vghat*s_chi, 0;...
                    0, 0, s_chi,  Vghat*c_chi, 0;...
                    0, 0, -Vgdot/Vghat, 0, Vgdot_psi;...
                    0, 0, chidot_Vg, chidot_chi, chidot_psi;...
                    0, 0, 0, 0, 0;...
                ];
        % Continuous
%         EKF.P_gps = EKF.P_gps +...
%                     EKF.Ts_gps*(A_gps*EKF.P_gps+EKF.P_gps*A_gps' + EKF.Q_gps);
        % Discrete
        Ad_gps = eye(5) + EKF.Ts_gps*A_gps + EKF.Ts_gps^2*A_gps^2;
        EKF.P_gps = Ad_gps*EKF.P_gps*Ad_gps' + EKF.Q_gps;
    end
    
    % Measurement step
    if  (y_gps_n~=y_gps_n_old)...
        ||(y_gps_e~=y_gps_e_old)...
        ||(y_gps_Vg~=y_gps_Vg_old)...
        ||(y_gps_course~=y_gps_course_old)
    
        % wrap course measurement
        while (y_gps_course - xhat_gps(4))>pi, y_gps_course = y_gps_course - 2*pi; end
        while (y_gps_course - xhat_gps(4))<-pi, y_gps_course = y_gps_course + 2*pi; end
        
        y_gps = [y_gps_n; y_gps_e; y_gps_Vg; y_gps_course];
        h_gps = [pnhat; pehat; Vghat; chihat];
        C_gps = eye(4, 5);
        for k=1:4
            Ci = C_gps(k, :);
            Li = EKF.P_gps*Ci'/(EKF.R_gps(k, k)+Ci*EKF.P_gps*Ci');
            EKF.P_gps = (eye(5) - Li*Ci)*EKF.P_gps;
            xhat_gps = xhat_gps + Li*(y_gps(k)-h_gps(k));
            xhat_gps(4) = y_gps_course;
        end


        % update stored GPS signals
        y_gps_n_old      = y_gps_n;
        y_gps_e_old      = y_gps_e;
        y_gps_Vg_old     = y_gps_Vg;
        y_gps_course_old = y_gps_course;
    end

%-------------------TEST------------------------
%     if  (y_gps_n~=y_gps_n_old)...
%         ||(y_gps_e~=y_gps_e_old)...
%         ||(y_gps_Vg~=y_gps_Vg_old)...
%         ||(y_gps_course~=y_gps_course_old)
%         % gps North position
%         h_p = pnhat;
%         C_p = [1, 0, 0, 0, 0];
%         L_p = EKF.P_gps*C_p'/(EKF.R_gps(1, 1)+C_p*EKF.P_gps*C_p');
%         EKF.P_gps = (eye(5) - L_p*C_p)*EKF.P_gps;
%         xhat_gps = xhat_gps + L_p*(y_gps_n-h_p);
%         % gps East position
%         h_p = pehat;
%         C_p = [0, 1, 0, 0, 0];
%         L_p = EKF.P_gps*C_p'/(EKF.R_gps(2, 2)+C_p*EKF.P_gps*C_p');
%         EKF.P_gps = (eye(5) - L_p*C_p)*EKF.P_gps;
%         xhat_gps = xhat_gps + L_p*(y_gps_e-h_p);
%         % gps ground speed
%         h_p = Vghat;
%         C_p = [0, 0, 1, 0, 0];
%         L_p = EKF.P_gps*C_p'/(EKF.R_gps(3, 3)+C_p*EKF.P_gps*C_p');
%         EKF.P_gps = (eye(5) - L_p*C_p)*EKF.P_gps;
%         xhat_gps = xhat_gps + L_p*(y_gps_Vg-h_p);
%         % gps course
%         % wrap course measurement
%         y_gps_course = wrapToPi(y_gps_course);
%         h_p = chihat;
%         C_p = [0, 0, 0, 1, 0];
%         L_p = EKF.P_gps*C_p'/(EKF.R_gps(4, 4)+C_p*EKF.P_gps*C_p');
%         EKF.P_gps = (eye(5) - L_p*C_p)*EKF.P_gps;
%         xhat_gps = xhat_gps + L_p*(y_gps_course-h_p);
%         xhat_gps(4) = y_gps_course;
% 
%         % update stored GPS signals
%         y_gps_n_old      = y_gps_n;
%         y_gps_e_old      = y_gps_e;
%         y_gps_Vg_old     = y_gps_Vg;
%         y_gps_course_old = y_gps_course;
%     end
%-------------------TEST------------------------
    pnhat  = xhat_gps(1);
    pehat  = xhat_gps(2);
    Vghat  = xhat_gps(3);
    chihat = xhat_gps(4);
    psihat = xhat_gps(5);
    
    p0 = phat;
    q0 = qhat;
    r0 = rhat;
    y_static_pres0 = lpf_y_static_pres;
    y_diff_pres0 = lpf_y_diff_pres;
    
    % not estimating these states
    alphahat = 0;
    betahat  = 0;
    bxhat    = 0;
    byhat    = 0;
    bzhat    = 0;
    wnhat    = Vghat*cos(chihat) - Vahat*cos(psihat);
    wehat    = Vghat*sin(chihat) - Vahat*sin(psihat);
    
      xhat = [...
        pnhat;...
        pehat;...
        hhat;...
        Vahat;...
        alphahat;...
        betahat;...
        phihat;...
        thetahat;...
        chihat;...
        phat;...
        qhat;...
        rhat;...
        Vghat;...
        wnhat;...
        wehat;...
        psihat;...
        bxhat;...
        byhat;...
        bzhat;...
        ];
end

function y = lpf(alpha, y1, u)
    y = alpha*y1 + (1-alpha)*u;
end
