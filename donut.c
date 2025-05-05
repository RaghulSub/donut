#include <stdio.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846
#define DONUT_RADIUS 2 // R2
#define DONUT_THICKNESS 1 // R1

#define SCREEN_WIDTH 70
#define SCREEN_HEIGHT 50

#define theta_spacing 0.02
#define phi_spacing 0.02

const int K2 = 15;
const int K1 = (SCREEN_WIDTH * K2 * 3) / (8 * (DONUT_RADIUS + DONUT_THICKNESS));

const char *gradient = ".,-~:;=!*#$@";

void render_frame(double A,double B){

    int grad_len = (int)strlen(gradient);
    
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH];
    double depth[SCREEN_HEIGHT][SCREEN_WIDTH];
    for (int i = 0; i < SCREEN_HEIGHT; i++){
        for (int j = 0; j < SCREEN_WIDTH; j++){
            depth[i][j] = 0;
            screen[i][j] = ' ';
        }
    }

    double cosA = cos(A), sinA = sin(A);
    double cosB = cos(B), sinB = sin(B);

    for( double theta = 0 ; theta < 2*PI ; theta += theta_spacing ) {
        double cos_theta = cos(theta);
        double sin_theta = sin(theta);

        double circle_x = DONUT_RADIUS + (DONUT_THICKNESS*cos_theta);
        double circle_y = DONUT_THICKNESS * sin_theta;

        for( double phi = 0 ; phi < 2*PI ; phi += phi_spacing ) {
            double cos_phi = cos(phi);
            double sin_phi = sin(phi);

            double x = circle_x * (cosB*cos_phi + sinA*sinB*sin_phi) - circle_y*cosA*sinB;
            double y = circle_x * (sinB*cos_phi - sinA*cosB*sin_phi) + circle_y*cosA*cosB;
            double z = K2 + cosA*circle_x*sin_phi + circle_y*sinA;

            if (z <= 0) continue; 

            double current_depth = 1/z;
            double L = cos_phi*cos_theta*sinB - cosA*cos_theta*sin_phi - sinA*sin_theta + cosB*(cosA*sin_theta - cos_theta*sinA*sin_phi);

            int x_coordinate = (int) ((SCREEN_WIDTH / 2) + K1*current_depth*x);
            int y_coordinate = (int) ((SCREEN_HEIGHT / 2) - K1*current_depth*y*0.5);

            if (x_coordinate < 0 || x_coordinate >= SCREEN_WIDTH || y_coordinate < 0 || y_coordinate >= SCREEN_HEIGHT)
                continue;

            
            if(L > 0 && current_depth > depth[y_coordinate][x_coordinate]){
                depth[y_coordinate][x_coordinate] = current_depth;
                int idx = (int)(L * (grad_len - 1));
                idx = idx < 0 ? 0 : idx >= grad_len ? grad_len - 1 : idx;
                screen[y_coordinate][x_coordinate] = gradient[idx];
            }
        }
        
    }
    printf("\x1b[H");
    for(int i = 0;i < SCREEN_HEIGHT;i++){
        for(int j = 0;j < SCREEN_WIDTH;j++){
            putchar(screen[i][j]);
        }
        putchar('\n');
    }
}

int main () {
    for(float a = 0; ;a+=theta_spacing){
        render_frame(a,a);
    }
    return 0;
}