#include <stdio.h>
#include <math.h>
#include "VecFunctions.h"

#define w 120
#define h 30

int main()
{
    char gradient[] = " .':,\"!/r(l1Z4H9W8$@";
    char screen[w * h + 1];
    screen[w * h] = '\0';
    double ratio = 1.8;
    for (int t = 0; t < 10000; t++)
    {
        vec3 spherePos = vec3(0, 3, 0);
        vec3 light = norm(vec3(-0.5, 0.5, -1.0));
        for (size_t i = 0; i < w; i++)
        {
            for (size_t j = 0; j < h; j++)
            {
                vec2 uv = vec2(i, j) / vec2(w, h) - 0.5;
                uv.x *= ratio;
                vec3 ro = vec3(-5, 0, 0);
                vec3 rd = norm(vec3(1, uv + 0.1));
                ro = rotateZ(ro, t * 0.01);
                rd = rotateZ(rd, t * 0.01);
                ro = rotateY(ro, t * 0.01);
                rd = rotateY(rd, t * 0.01);
                double diff = 0;
                vec2 minIt = vec2(99999);
                vec3 n = 0;
                vec2 it = sphIntersect(ro, rd, spherePos, 1);
                if (it.x > 0 && it.x < minIt.x)
                {
                    minIt = it;
                    n = norm(ro - spherePos + rd * it.x);
                }
                vec3 boxN = vec3(0);
                vec2 it2 = boxIntersection(ro, rd, vec3(1), boxN);
                if (it2.x > 0 && it2.x < minIt.x)
                {
                    minIt = it2;
                    n = boxN;
                }
                diff = dot(n, light);
                int col = 0;
                if (minIt.x < 99999)
                {
                    diff = diff * 0.5 + 0.5;
                    col = (int)(diff * 20.0);
                }
                if (col < 0) col = 0;
                if (col > 19) col = 19;
                screen[i + j * w] = gradient[col];
            }
        }
        printf("%s", screen);
    }
    getchar();
    return 0;
}