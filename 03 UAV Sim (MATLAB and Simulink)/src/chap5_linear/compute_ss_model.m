% x_trim is the trimmed state,
% u_trim is the trimmed input
  
  
[SS.A,SS.B,SS.C,SS.D]=linmod('mavsim_trim',TRIM.x_trim, TRIM.u_trim);

SS.A_lat = SS.A([5, 10, 12, 7, 9], [5, 10, 12, 7, 9]);
SS.B_lat = SS.B([5, 10, 12, 7, 9], [2, 3]);

SS.A_lon = SS.A([4, 6, 11, 8, 3], [4, 6, 11, 8, 3]);
SS.B_lon = SS.B([4, 6, 11, 8, 3], [1, 4]);