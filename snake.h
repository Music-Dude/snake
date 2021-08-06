void draw();

struct point {
    int y;
    int x;
};

struct snake {
    struct point segs[100];
    bool alive;
    int len;
    int dir;
};

