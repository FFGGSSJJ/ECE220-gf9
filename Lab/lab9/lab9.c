#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "mp5.h"


static void
build_tree (int32_t level, int32_t x, int32_t y, double angle, double width)
{
    // you need to write this code
    // don't forget to set the color before drawing anything!
    double length, width_end, anglespan;
    int32_t x_end, y_end;
    int32_t num_children = (2 + 4*drand48());
    
    length = 60*(drand48() + 1)*pow(0.666666666, level);
    width_end = width*((drand48() + 2)/3);
    anglespan = M_PI_2*(1 + drand48());
    x_end = x + length*cos(angle);
    y_end = y - length*sin(angle);

    draw_line(x-width*sin(angle), y-width*cos(angle), x_end-width_end*sin(angle), y_end-width_end*cos(angle));
    draw_line(x+width*sin(angle), y+width*cos(angle), x_end+width_end*sin(angle), y_end+width_end*cos(angle));

    if (level == 5 && num_children == 0)    return;
    build_tree(level + 1, x_end, y_end, angle, width_end/2);
    if (level == 5 && num_children != 0){
        num_children--;
        draw_circle(x_end, y_end, 0, 5);
    }
    return;
}

int
main ()
{
    // this random seed produces the tree in the lab specification
    srand48 (0x5BDE78CB);

    // for random trees, uncomment the line below
    // srand48 (time (NULL));

    printf ("#include <stdint.h>\n\n#include \"mp5.h\"\n\nvoid\n");
    printf ("draw_tree (void) {\n");
    build_tree (0, WIDTH / 2, HEIGHT - 1, M_PI_2, 20);
    printf ("}\n");
    
    return 0;
}

