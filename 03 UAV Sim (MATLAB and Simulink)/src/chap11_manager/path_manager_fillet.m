% path_manager_fillet
%   - follow lines between waypoints.  Smooth transition with fillets
%
% Modified:  
%   - 4/1/2010 - RWB
%   - 3/19/2019 - RWB
%
% input is:
%   num_waypoints - number of waypoint configurations
%   waypoints    - an array of dimension 5 by P.size_waypoint_array.
%                - the first num_waypoints rows define waypoint
%                  configurations
%                - format for each waypoint configuration:
%                  [wn, we, wd, dont_care, Va_d]
%                  where the (wn, we, wd) is the NED position of the
%                  waypoint, and Va_d is the desired airspeed along the
%                  path.
%
% output is:
%   flag - if flag==1, follow waypoint path
%          if flag==2, follow orbit
%   
%   Va^d - desired airspeed
%   r    - inertial position of start of waypoint path
%   q    - unit vector that defines inertial direction of waypoint path
%   c    - center of orbit
%   rho  - radius of orbit
%   lambda = direction of orbit (+1 for CW, -1 for CCW)
%
function out = path_manager_fillet(in,PLAN,start_of_simulation)
  persistent wp_idx;
  persistent isOrbit;
  
  NN = 0;
  num_waypoints = in(1+NN);
  waypoints = reshape(in(2+NN:5*PLAN.size_waypoint_array+1+NN),5,PLAN.size_waypoint_array);
  NN = NN + 1 + 5*PLAN.size_waypoint_array;
  pn        = in(1+NN);
  pe        = in(2+NN);
  h         = in(3+NN);
  % Va      = in(4+NN);
  % alpha   = in(5+NN);
  % beta    = in(6+NN);
  % phi     = in(7+NN);
  % theta   = in(8+NN);
  % chi     = in(9+NN);
  % p       = in(10+NN);
  % q       = in(11+NN);
  % r       = in(12+NN);
  % Vg      = in(13+NN);
  % wn      = in(14+NN);
  % we      = in(15+NN);
  % psi     = in(16+NN);
  state     =  in(1+NN:16+NN);
  NN = NN + 19;
  t         = in(1+NN);
  if start_of_simulation
      wp_idx = 2;
      isOrbit = 0;
  end
  Va_d = waypoints(5, wp_idx);
  p = [pn; pe; -h];
  wp0 = waypoints(1:3, wp_idx);
  wp1 = waypoints(1:3, wp_idx+1);
  r = waypoints(1:3, wp_idx-1);
  q =  wp0-r;
  q = q/norm(q);
  
  q_i = wp1-wp0;
  q_i = q_i/norm(q_i);
  
  varrho = acos(-q'*q_i);
  varrho_half = varrho*0.5;
  
  if ~isOrbit
      flag = 1;
      z = wp0 - (PLAN.R_min/tan(varrho_half))*q;
      if dot(p-z, q) >= 0
          isOrbit = 1;
      end
      c      = [0;0;0];    % not used for waypoint path
      lambda = 0;          % not used for waypoint path
  else
      flag = 2;
      temp = q-q_i;
      temp = temp/norm(temp);
      c = wp0 - (PLAN.R_min/sin(varrho_half))*temp;
      lambda = sign(q(1)*q_i(2) - q(2)*q_i(1));
      z = wp0 + (PLAN.R_min/tan(varrho_half))*q;
      if dot(p-z, q_i) >= 0
          wp_idx = wp_idx + 1;
          isOrbit = 0;
      end
  end

  if wp_idx < num_waypoints - 1
      flag_need_new_waypoints = 0;
  else
      flag_need_new_waypoints = 1;
  end
  
  out = [flag; Va_d; r; q; c; PLAN.R_min; lambda; state; flag_need_new_waypoints];
  

end