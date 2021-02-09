
function drawVehicle(uu,P,V,F,patchcolors)

    % process inputs to function
    pn       = uu(1);       % inertial North position     
    pe       = uu(2);       % inertial East position
    pd       = uu(3);       % inertial Down position    
    u        = uu(4);       
    v        = uu(5);       
    w        = uu(6);       
    phi      = uu(7);                
    theta    = uu(8);    
    psi      = uu(9);
    p        = uu(10);       % roll rate
    q        = uu(11);       % pitch rate     
    r        = uu(12);       % yaw rate    
    t        = uu(13);       % time

    % define persistent variables 
    persistent vehicle_handle;
    persistent Vertices;
    persistent Faces;
    persistent facecolors;
    persistent fig1;
    
   % first time function is called, initialize plot and persistent vars
    ax_pos = [pe, pe, pn, pn, -pd, -pd];
    if t==0,
        figure(1), clf
        [Vertices,Faces,facecolors] = defineVehicleBody(P);
        vehicle_handle = drawVehicleBody(Vertices,Faces,facecolors,...
                                               pn,pe,pd,phi,theta,psi,...
                                               [],'normal');
        title('Vehicle')
        xlabel('East')
        ylabel('North')
        zlabel('-Down')
        view(32,47)  % set the vieew angle for figure
        AX = P.scale*[-10, 10, -10, 10, -10, 10] + ax_pos;
        axis(AX);
        grid on
        hold on
        fig1 = figure(1);
        
    % at every other time step, redraw base and rod
    else
        AX = P.scale*[-10, 10, -10, 10, -10, 10] + ax_pos;
        fig1.CurrentAxes.XLim = AX(1:2);
        fig1.CurrentAxes.YLim = AX(3:4);
        fig1.CurrentAxes.ZLim = AX(5:6);
        drawVehicleBody(Vertices,Faces,facecolors,...
                           pn,pe,pd,phi,theta,psi,...
                           vehicle_handle);
    end
end

  
%=======================================================================
% drawVehicle
% return handle if 3rd argument is empty, otherwise use 3rd arg as handle
%=======================================================================
%
function handle = drawVehicleBody(V,F,patchcolors,...
                                     pn,pe,pd,phi,theta,psi,...
                                     handle,mode)
  V = rotate(V, phi, theta, psi);  % rotate vehicle
  V = translate(V, pn, pe, pd);  % translate vehicle
  % transform vertices from NED to XYZ (for matlab rendering)
  R = [...
      0, 1, 0;...
      1, 0, 0;...
      0, 0, -1;...
      ];
  V = R*V;
  
  if isempty(handle),
  handle = patch('Vertices', V', 'Faces', F,...
                 'FaceVertexCData',patchcolors,...
                 'FaceColor','flat',...
                 'EraseMode', mode);
  else
    set(handle,'Vertices',V','Faces',F);
    drawnow
  end
end

%%%%%%%%%%%%%%%%%%%%%%%
function pts=rotate(pts,phi,theta,psi)
  c_phi = cos(phi);
  s_phi = sin(phi);

  c_theta = cos(theta);
  s_theta = sin(theta);
  
  c_psi = cos(psi);
  s_psi = sin(psi);

  % define rotation matrix (right handed)
  R_roll = [...
          1, 0, 0;...
          0, c_phi, s_phi;...
          0, -s_phi, c_phi];
  R_pitch = [...
          c_theta, 0, -s_theta;...
          0, 1, 0;...
          s_theta, 0, c_phi];
  R_yaw = [...
          c_psi, s_psi, 0;...
          -s_psi, c_psi, 0;...
          0, 0, 1];
  R = R_roll*R_pitch*R_yaw;  
    % note that R above either leaves the vector alone or rotates
    % a vector in a left handed rotation.  We want to rotate all
    % points in a right handed rotation, so we must transpose
  R = R';

  % rotate vertices
  pts = R*pts;
  
end
% end rotateVert

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% translate vertices by pn, pe, pd
function pts = translate(pts,pn,pe,pd)

  pts = pts + repmat([pn;pe;pd],1,size(pts,2));
  
end

% end translate


%=======================================================================
% defineVehicleBody
%=======================================================================
function [V,F,facecolors] = defineVehicleBody(P)

% Define the vertices (physical location of vertices
V = P.scale*[... 
    P.fuse_l1, 0, 0;...                                   % pt 1
    P.fuse_l2, (P.fuse_w*0.5), -(P.fuse_h*0.5);...        % pt 2
    P.fuse_l2, -(P.fuse_w*0.5), -(P.fuse_h*0.5);...       % pt 3
    P.fuse_l2, -(P.fuse_w*0.5), (P.fuse_h*0.5);...        % pt 4
    P.fuse_l2, (P.fuse_w*0.5), (P.fuse_h*0.5);...         % pt 5
    -P.fuse_l3, 0, 0;...                                  % pt 6
    0, (P.wing_w*0.5), 0;...                              % pt 7
    -(P.wing_l*0.5), (P.wing_w*0.5), 0;...                % pt 8
    -(P.wing_l*0.5), -(P.wing_w*0.5), 0;...               % pt 9
    0, -(P.wing_w*0.5), 0;...                             % pt 10
    -(P.fuse_l3-P.tailwing_l), (P.tailwing_w*0.5), 0;...  % pt 11
    -P.fuse_l3, (P.tailwing_w*0.5), 0;...                 % pt 12
    -P.fuse_l3, -(P.tailwing_w*0.5), 0;...                % pt 13
    -(P.fuse_l3-P.tailwing_l), -(P.tailwing_w*0.5), 0;... % pt 14
    -(P.fuse_l3-P.tailwing_l), 0, 0;...                   % pt 15
    -P.fuse_l3, 0, -P.tail_h;...                          % pt 16
    ]';

% define faces as a list of vertices numbered above
  F = [...
        1, 2, 3;...   % top head
        1, 2, 5;...   % right head
        1, 4, 5;...   % bottom head
        1, 3, 4;...   % left head
        
        6, 2, 3;...   % top body
        6, 2, 5;...   % right body
        6, 4, 5;...   % bottom body
        6, 3, 4;...   % left body
                
        7, 8, 9;...   % wing
        7, 9, 10;...  % wing
        11, 12, 13;... % tailwing
        11, 13, 14;... % tailwing
        6, 15, 16;... % tail 
        ];

% define colors for each face    
  myred = [1, 0, 0];
  mygreen = [0, 1, 0];
  myblue = [0, 0, 1];
  myyellow = [1, 1, 0];
  mycyan = [0, 1, 1];

  facecolors = [...
    myyellow;...  % top head
    myyellow;...  % right head
    myyellow;...  % bottom head
    myyellow;...  % left head

    myblue;...  % top body
    myblue;...  % right body
    myblue;...  % bottom body
    myblue;...  % left body

    mygreen;... % wing
    mygreen;... % wing
    mygreen;... % tailwing
    mygreen;... % tailwing
    myblue;...  % tail
    ];
end
  