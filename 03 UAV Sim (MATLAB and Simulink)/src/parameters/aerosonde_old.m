%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Params for Aerosonde UAV
%physical parameters of airframe
P.mass = 13.5;
P.Jx   = 0.8244;
P.Jy   = 1.135;
P.Jz   = 1.759;
P.Jxz  = .1204;

% gamma parameters
P.gamma = P.Jx*P.Jz-P.Jxz^2;
P.gamma1 = (P.Jxz*(P.Jx-P.Jy+P.Jz))/P.gamma;
P.gamma2 = (P.Jz*(P.Jz-P.Jy)+P.Jxz^2)/P.gamma;
P.gamma3 = P.Jz/P.gamma;
P.gamma4 = P.Jxz/P.gamma;
P.gamma5 = (P.Jz-P.Jx)/P.Jy;
P.gamma6 = P.Jxz/P.Jy;
P.gamma7 = ((P.Jx-P.Jy)*P.Jx+P.Jxz^2)/P.gamma;
P.gamma8 = P.Jx/P.gamma;

% aerodynamic coefficients
P.S_wing        = 0.55;
P.b             = 2.8956;
P.c             = 0.18994;

P.S_prop        = 0.2027;
P.rho           = 1.2682;
P.k_motor       = 80;
P.k_T_P         = 0;
P.k_Omega       = 0;
P.e             = 0.9;
P.AR            = P.b^2 / P.S_wing;

% longitudinal coeffecients
P.C_L_0         = 0.28;
P.C_L_alpha     = 3.45;
P.C_L_q         = 0.0;
P.C_L_delta_e   = -0.36;
P.C_D_0         = 0.03;
P.C_D_alpha     = 0.30;
P.C_D_p         = 0.0437;
P.C_D_q         = 0.0;
P.C_D_delta_e   = 0.0;
P.C_m_0         = -0.02338;
P.C_m_alpha     = -0.38;
P.C_m_q         = -3.6;
P.C_m_delta_e   = -0.5;
P.C_prop        = 1.0;
P.M             = 50;
P.epsilon       = 0.1592;
P.alpha0        = 0.4712;

% lateral coefficients
P.C_Y_0         = 0.0;
P.C_Y_beta      = -0.98;
P.C_Y_p         = 0.0;
P.C_Y_r         = 0.0;
P.C_Y_delta_a   = 0.0;
P.C_Y_delta_r   = -0.17;
P.C_ell_0       = 0.0;
P.C_ell_beta    = -0.12;
P.C_ell_p       = -0.26;
P.C_ell_r       = 0.14;
P.C_ell_delta_a = 0.08;
P.C_ell_delta_r = 0.105;
P.C_n_0         = 0.0;
P.C_n_beta      = 0.25;
P.C_n_p         = 0.022;
P.C_n_r         = -0.35;
P.C_n_delta_a   = 0.06;
P.C_n_delta_r   = -0.032;


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Parameters for propulsion thrust and torque models

% Prop parameters
P.D_prop = 20*(0.0254);     % prop diameter in m

% Motor parameters
P.K_V = 145;                    % from datasheet RPM/V
P.K_Q = (1/P.K_V)*60/(2*pi);   % KQ in N-m/A, V-s/rad
P.R_motor = 0.042;              % ohms
P.i0 = 1.5;                     % no-load (zero-torque) current (A)

% Inputs
P.ncells = 12;
P.V_max = 3.7*P.ncells;       % max voltage for specified number of battery cells

% Coeffiecients from prop_data fit
P.C_Q2 = -0.01664;
P.C_Q1 = 0.004970;
P.C_Q0 = 0.005230;

P.C_T2 = -0.1079;
P.C_T1 = -0.06044;
P.C_T0 = 0.09357;
