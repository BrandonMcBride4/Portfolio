% dubinsParameters
%   - Find Dubin's parameters between two configurations
%
% Modified:  
%   - 4/13/2010 - RWB
%
% input is:
%   start_node  - [wn_s, we_s, wd_s, chi_s, Va_d]
%   end_node    - [wn_e, wn_e, wd_e, chi_e, Va_d]
%   R           - minimum turn radius
%
% output is:
%   dubinspath  - a matlab structure with the following fields
%       dubinspath.ps   - the start position in re^3
%       dubinspath.chis - the start course angle
%       dubinspath.pe   - the end position in re^3
%       dubinspath.chie - the end course angle
%       dubinspath.R    - turn radius
%       dubinspath.L    - length of the Dubins path
%       dubinspath.cs   - center of the start circle
%       dubinspath.lams - direction of the start circle
%       dubinspath.ce   - center of the end circle
%       dubinspath.lame - direction of the end circle
%       dubinspath.w1   - vector in re^3 defining half plane H1
%       dubinspath.q1   - unit vector in re^3 along straight line path
%       dubinspath.w2   - vector in re^3 defining position of half plane H2
%       dubinspath.w3   - vector in re^3 defining position of half plane H3
%       dubinspath.q3   - unit vector defining direction of half plane H3
% 

function dubinspath = dubinsParameters(start_node, end_node, R)

  ell = norm(start_node(1:2)-end_node(1:2));
  if ell<2*R
      disp('The distance between nodes must be larger than 2R.');
      dubinspath = [];
  else
    
    p_s   = start_node(1:3)';
    chi_s = start_node(4);
    p_e   = end_node(1:3)';
    chi_e = end_node(4);
    
    c_chi_s = cos(chi_s);
    c_chi_e = cos(chi_e);
    s_chi_s = sin(chi_s);
    s_chi_e = sin(chi_e);
    
    PI2 = 2*pi;
    HALF_PI = pi/2;
    e1 = [1; 0; 0];

    c_rs = p_s + R*R_z(HALF_PI)*[c_chi_s; s_chi_s; 0];
    c_ls = p_s + R*R_z(-HALF_PI)*[c_chi_s; s_chi_s; 0];
    c_re = p_e + R*R_z(HALF_PI)*[c_chi_e; s_chi_e; 0];
    c_le = p_e + R*R_z(-HALF_PI)*[c_chi_e; s_chi_e; 0];
   
    % Length of case 1
    vartheta = calc_vartheta(c_rs, c_re);
    L1 = norm(c_rs-c_re) +...
        R*mod(PI2 + mod(vartheta-HALF_PI, PI2) - mod(chi_s-HALF_PI, PI2), PI2) +...
        R*mod(PI2 + mod(chi_e-HALF_PI, PI2) - mod(vartheta-HALF_PI, PI2), PI2);
     
    % Length of case 2
    vartheta = calc_vartheta(c_rs, c_le);
    ell = norm(c_le-c_rs);
    vartheta2 = vartheta - HALF_PI + asin(2*R/ell);
    if isreal(vartheta2)
        L2 = sqrt(ell^2 - 4*R^2) +...
            R*mod(PI2 + mod(vartheta2, PI2) - mod(chi_s-HALF_PI, PI2), PI2) +...
            R*mod(PI2 + mod(vartheta2+pi, PI2) - mod(chi_e+HALF_PI, PI2), PI2);
    else
        L2 = inf;
    end
     
    % Length of case 3
    vartheta = calc_vartheta(c_ls, c_re);
    ell = norm(c_re-c_ls);
    vartheta2 = acos(2*R/ell);
    if isreal(vartheta2) 
        L3 = sqrt(ell^2 - 4*R^2) +...
            R*mod(PI2 + mod(chi_s+HALF_PI, PI2) - mod(vartheta+vartheta2, PI2), PI2) +...
            R*mod(PI2 + mod(chi_e-HALF_PI, PI2) - mod(vartheta+vartheta2-pi, PI2), PI2);
    else
        L3 = inf;
    end
      
    % Length of case 4
    vartheta = calc_vartheta(c_ls, c_le);
    L4 = norm(c_ls-c_le) +...
        R*mod(PI2 + mod(chi_s+HALF_PI, PI2) - mod(vartheta+HALF_PI, PI2), PI2) +...
        R*mod(PI2 + mod(vartheta+HALF_PI, PI2) - mod(chi_e+HALF_PI, PI2), PI2); 
    
    % L is the minimum distance
    [L,idx] = min([L1,L2,L3,L4]);
    switch(idx)
        case 1
            c_s = c_rs;
            lambda_s = 1;
            c_e = c_re;
            lambda_e = 1;
            q1 = c_e-c_s;
            q1 = q1/norm(q1);
            z1 = c_s + R*R_z(-HALF_PI)*q1;
            z2 = c_e + R*R_z(-HALF_PI)*q1;
        case 2
            c_s = c_rs;
            lambda_s = 1;
            c_e = c_le;
            lambda_e = -1;
            ell = norm(c_e-c_s);
            vartheta = calc_vartheta(c_s, c_e);
            vartheta2 = vartheta - HALF_PI + asin(2*R/ell);
            q1 = R_z(vartheta2 + HALF_PI)*e1;
            z1 = c_s + R*R_z(vartheta2)*e1;
            z2 = c_e + R*R_z(vartheta2+pi)*e1;
        case 3
            c_s = c_ls;
            lambda_s = -1;
            c_e = c_re;
            lambda_e = 1;
            ell = norm(c_e-c_s);
            vartheta = calc_vartheta(c_s, c_e);
            vartheta2 = acos(2*R/ell);
            q1 = R_z(vartheta+vartheta2-HALF_PI)*e1;
            z1 = c_s + R*R_z(vartheta+vartheta2)*e1;
            z2 = c_e + R*R_z(vartheta+vartheta2-pi)*e1;
        case 4
            c_s = c_ls;
            lambda_s = -1;
            c_e = c_le;
            lambda_e = -1;
            q1 = c_e-c_s;
            q1 = q1/norm(q1);
            z1 = c_s + R*R_z(HALF_PI)*q1;
            z2 = c_e + R*R_z(HALF_PI)*q1;
    end
    
    z3 = p_e;
    q3 = R_z(chi_e)*e1;
    
    % assign path variables
    dubinspath.ps   = p_s';
    dubinspath.chis = chi_s;
    dubinspath.pe   = p_e';
    dubinspath.chie = chi_e;
    dubinspath.R    = R;
    dubinspath.L    = L;
    dubinspath.cs   = c_s;
    dubinspath.lams = lambda_s;
    dubinspath.ce   = c_e;
    dubinspath.lame = lambda_e;
    dubinspath.w1   = z1;
    dubinspath.q1   = q1;
    dubinspath.w2   = z2;
    dubinspath.w3   = z3;
    dubinspath.q3   = q3;
  end
end

function y = R_z(theta)
    c = cos(theta);
    s = sin(theta);
    y = [c, -s, 0; s, c, 0; 0, 0, 1];
end

function theta = calc_vartheta(c_s, c_e)
    p = c_e-c_s;
    theta = atan2(p(2), p(1));
    theta = mod(theta, 2*pi);
end
