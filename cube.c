#include <stdio.h>
#include <string.h>
#include <math.h>


#define PI 3.14
#define ORIGIN_X 0
#define ORIGIN_Y 0
#define ORIGIN_Z 0

#define x_spacing 0.07
#define y_spacing 0.07
#define z_spacing 0.07

#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 40

#define height 5
#define width 1
#define length 5


float max(float a,float b){
    if(a > b){
        return a;
    }
    return b;
}

const int K2 = 200;
const int K1 = (SCREEN_WIDTH * K2 * 3) / (8 * (height));

float lx = 0, ly = 1, lz = -1;


const char *gradient = ".,-~:;=!*#$@";

void render_frame(float A,float B){
    int grad_len = (int)strlen(gradient);
    
    float cosA = cos(A), cosB = cos(B);
    float sinA = sin(A), sinB = sin(B);


    float depth[SCREEN_HEIGHT][SCREEN_WIDTH];
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH];

    for(int i = 0;i < SCREEN_HEIGHT;i++){
        for(int j = 0;j < SCREEN_WIDTH;j++){
            depth[i][j] = 0;
            screen[i][j] = ' ';
        }
    }

    for(float x = ORIGIN_X-(length/2);x <= ORIGIN_X+(length/2);x += x_spacing){
        for(float y = ORIGIN_Y-(width/2);y <= ORIGIN_Y+(width/2);y += y_spacing){
            for(float z = ORIGIN_Z-(height/2);z <= ORIGIN_Z+(height/2);z += z_spacing){
                


                float xp = x*cosB - sinB*(y*cosA - z*sinA);
                float yp = x*sinB + (y*cosA -  z*sinA)*cosB;
                float zp = y*sinA + z*cosA;

                // float xp = cosB*x - (cosA*sinB*y) + sinA*sinB*z;
                // float yp = sinB*x + cosA*cosB*y - sinA*cosB*z;
                // float zp = sinA*y + cosA*z;

                float nx = xp, ny = yp, nz = -zp;
                float norm = sqrt(nx*nx + ny*ny + nz*nz);
                nx /= norm; ny /= norm; nz /= norm;


                if(zp == 0) continue;
                
                float current_depth = 1.0/(zp+K2);

                int x_coordinate = (int) ((SCREEN_WIDTH / 2) + K1*current_depth*xp);
                int y_coordinate = (int) ((SCREEN_HEIGHT / 2) - K1*current_depth*yp*0.5);

                if (x_coordinate < 0 || x_coordinate >= SCREEN_WIDTH || y_coordinate < 0 || y_coordinate >= SCREEN_HEIGHT)
                    continue;

                float L = nx*lx + ny*ly + nz*lz;
                L = max(L,-L);

                if(L > 0 && current_depth > depth[y_coordinate][x_coordinate]){
                    depth[y_coordinate][x_coordinate] = current_depth;
                    int idx = (int)(L * (grad_len - 1) + 0.5);
                    idx = idx < 0 ? 0 : idx >= grad_len ? grad_len - 1 : idx;
                    screen[y_coordinate][x_coordinate] = gradient[idx];

                }
            }
        }
    }

    printf("\x1b[H");
    for(int i = 0;i < SCREEN_HEIGHT;i++){
        for(int j = 0;j < SCREEN_WIDTH;j++){
            putchar(screen[i][j]);
            // putchar('a');
        }
        putchar('\n');
    }
    
}

int main(){
    float i = 0.0;
    for(float i = 0;;i+=0.0005)
        render_frame(i,i);
    return 0;
}