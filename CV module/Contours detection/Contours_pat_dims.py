import math

def get_pts(pts):
    pts.sort()
    [p1,p2,p3,p4] = pts
    (x1, y1), (x2, y2), (x3, y3), (x4, y4) = p1, p2, p3, p4
    if(y1 > y4): p1, p4 = p4, p1
    if(y2 > y3): p2, p3 = p3, p2
    return pts

def get_dims(crnr_pts):
    [P1, P2, P3, P4] = crnr_pts
    w = math.sqrt(pow(P1[0] - P2[0],2) + pow(P1[1] - P2[1],2))
    h = math.sqrt(pow(P1[0] - P4[0],2) + pow(P1[1] - P4[1],2))
    c = (P1[0]+(w/2), P1[1]+(h/2))
    return [w, h, c]

def get_angle(p2,p1):
    (x2, y2), (x1, y1) = p2, p1
    slope = (y2 - y1)/(x2 - x1)
    angle =  math.degrees(math.atan(slope)) + 90
    return angle