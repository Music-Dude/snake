void draw();
void menu();

int speeds[] = {80, 60, 40, 20};
char *modes[] = {"Easy", "Medium", "Difficult", "Expert"};


struct point
{
    int y;
    int x;
};

struct snake
{
    struct point segs[100];
    bool alive;
    int len;
    int dir;
};
