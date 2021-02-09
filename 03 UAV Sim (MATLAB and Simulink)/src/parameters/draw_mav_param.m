P.scale      = 100;
P.fuse_l3    = 3.0;  % center of gravity to the tail
P.fuse_l1     = P.fuse_l3/3.0; % center of gravity to tip of nose
P.fuse_l2    = P.fuse_l1/2.0;  % center of gravity to base of nose
P.fuse_h     = P.fuse_l1-P.fuse_l2;
P.fuse_w     = P.fuse_h;

P.wing_w     = P.fuse_l3;
P.wing_l     = P.wing_w/1.5;

P.tailwing_w = P.wing_w/2.0;
P.tailwing_l = P.tailwing_w/3.0;

P.tail_h     = P.tailwing_l*1.5;