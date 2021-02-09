function [euler] = Quaternion2Euler(quat)
% euler(1) = phi
% euler(2) = theta
% euler(3) = psi

quat_sq = quat.^2;

euler = [...
            atan2((2*(quat(1)*quat(2) + quat(3)*quat(4))), (quat_sq(1)+quat_sq(4)-quat_sq(2)-quat_sq(3)));...
            asin(2*(quat(1)*quat(3) - quat(2)*quat(4)));...
            atan2((2*(quat(1)*quat(4) + quat(2)*quat(3))), (quat_sq(1)+quat_sq(2)-quat_sq(3)-quat_sq(4)));...
        ];
end

