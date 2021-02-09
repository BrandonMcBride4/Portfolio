% forces_moments.m
%   Computes the forces and moments acting on the airframe. 
%
%   Output is
%       F     - forces
%       M     - moments
%       Va    - airspeed
%       alpha - angle of attack
%       beta  - sideslip angle
%       wind  - wind vector in the inertial frame
%

function out = forces_moments(x, delta, wind, P)

    % relabel the inputs
    pn      = x(1);
    pe      = x(2);
    pd      = x(3);
    u       = x(4);
    v       = x(5);
    w       = x(6);
    phi     = x(7);
    theta   = x(8);
    psi     = x(9);
    p       = x(10);
    q       = x(11);
    r       = x(12);
    delta_e = delta(1);
    delta_a = delta(2);
    delta_r = delta(3);
    delta_t = delta(4);
    w_ns    = wind(1); % steady wind - North
    w_es    = wind(2); % steady wind - East
    w_ds    = wind(3); % steady wind - Down
    u_wg    = wind(4); % gust along body x-axis
    v_wg    = wind(5); % gust along body y-axis    
    w_wg    = wind(6); % gust along body z-axis
    
    % get rotation matrix
    R = getVehicleToBodyRotationMatrix([phi; theta; psi]);
    
    % compute wind data in NED
    V_wb = R*[w_ns; w_es; w_ds]+[u_wg; v_wg; w_wg]; % Wind vector in body frame
    V_ab = [u;v;w]-V_wb; % Airspeed vector in body frame
    V_wv = R'*V_wb; % Wind vector in the vehicle frame
    
    w_n = V_wv(1);
    w_e = V_wv(2);
    w_d = V_wv(3);
    
    % compute air data
    Va = norm(V_ab);
    alpha = atan2(V_ab(3),V_ab(1));
    beta = asin(V_ab(2)/Va);
    
    c_alpha = cos(alpha);
    s_alpha = sin(alpha);
    c_theta = cos(theta);
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % compute forces
    k = 0.5*P.rho*Va^2*P.S_wing;
    cq = (P.c/(2*Va))*q;
    k_b = (P.b/(2*Va));
    % Force = F_gravity + F_aero + F_prop
    % compute gravity forces
    F_gravity = P.mass * P.gravity *...
                [-sin(theta); c_theta*sin(phi); c_theta*cos(phi)];
    
    % compute aero forces
    % compute longitudinal aerodynamics
    sigma_alpha = (1 + exp(-P.M*(alpha-P.alpha0)) + exp(P.M*(alpha+P.alpha0)))/...
                  ((1+exp(-P.M*(alpha-P.alpha0))) * (1+exp(P.M*(alpha+P.alpha0))));
              
    C_L_alpha = (1-sigma_alpha) * (P.C_L_0 + P.C_L_alpha*alpha) + ...
                (sigma_alpha * (2*sign(alpha) * s_alpha^2 * c_alpha));
            
    f_lift = k*(C_L_alpha + P.C_L_q*cq + P.C_L_delta_e*delta_e);
    
    C_D_alpha = P.C_D_p + (P.C_L_0 + P.C_L_alpha*alpha)^2 / (pi*P.e*P.AR);
    f_drag = k*(C_D_alpha + P.C_D_q*cq + P.C_D_delta_e*delta_e);
    
    m  = P.c*k*(P.C_m_0 + P.C_m_alpha*alpha + P.C_m_q*cq + P.C_m_delta_e*delta_e);
    
    F_long = [c_alpha, -s_alpha; s_alpha, c_alpha] * [-f_drag; -f_lift]; % = fx;fz
    
    % compute lateral aerodynamics
    f_y = k*(P.C_Y_0 + P.C_Y_beta*beta + P.C_Y_p*k_b*p + P.C_Y_r*k_b*r + P.C_Y_delta_a*delta_a + P.C_Y_delta_r*delta_r);
    ell = k*P.b*(P.C_ell_0 + P.C_ell_beta*beta + P.C_ell_p*k_b*p + P.C_ell_r*k_b*r + P.C_ell_delta_a*delta_a + P.C_ell_delta_r*delta_r);
    n = k*P.b*(P.C_n_0 + P.C_n_beta*beta + P.C_n_p*k_b*p + P.C_n_r*k_b*r + P.C_n_delta_a*delta_a + P.C_n_delta_r*delta_r);
    
    % compute prop forces
    V_in = P.V_max * delta_t;
    
    a = P.C_Q0 * P.rho * P.D_prop^5 / (2*pi)^2;
    b = (P.C_Q1*P.rho*P.D_prop^4/(2*pi))*Va + P.KQ*P.KQ/P.R_motor;
    c = P.C_Q2*P.rho*P.D_prop^3*Va^2 - (P.KQ/P.R_motor)*V_in + P.KQ*P.i0;
    
    omega_op = (-b + sqrt(b^2 - 4*a*c)) / (2*a); % Consider only positive root
    J_op = 2*pi*Va/(omega_op*P.D_prop); % Advanced ratio
    
    C_T = P.C_T2*J_op^2 + P.C_T1*J_op + P.C_T0;
    C_Q = P.C_Q2*J_op^2 + P.C_Q1*J_op + P.C_Q0;
    
    n_prop = omega_op / (2*pi);
    
    T_prop = P.rho * n_prop^2 * P.D_prop^4 * C_T;
    
    Q_prop = -P.rho * n_prop^2 * P.D_prop^5 * C_Q;
    
    % combine calculated forces and torques
    F = F_gravity + [F_long(1);f_y;F_long(2)] + [T_prop;0;0];
    
    M = [...
            ell + Q_prop;...
            m;...
            n;...
        ];
   
    out = [[F(1); F(2); F(3); M(1); M(2); M(3)]; [Va; alpha; beta; w_n; w_e; w_d];];
end



