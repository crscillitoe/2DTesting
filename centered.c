#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>

//Define Window Size
#define W 600
#define H 400
#define P 10

#define min(a,b)             (((a) < (b)) ? (a) : (b)) // min: Choose smaller of two scalars.
#define max(a,b)             (((a) > (b)) ? (a) : (b)) // max: Choose greater of two scalars.
#define clamp(a, mi,ma)      min(max(a,mi),ma)         // clamp: Clamp value into set range.
#define vxs(x0,y0, x1,y1)    ((x0)*(y1) - (x1)*(y0))   // vxs: Vector cross product
//Draw a vertical line on screen, with a different color pixel in the top, bottom, and middle.
// Overlap:  Determine whether the two number ranges overlap.
#define Overlap(a0,a1,b0,b1) (min(a0,a1) <= max(b0,b1) && min(b0,b1) <= max(a0,a1))
// IntersectBox: Determine whether two 2D-boxes intersect.
#define IntersectBox(x0,y0, x1,y1, x2,y2, x3,y3) (Overlap(x0,x1,x2,x3) && Overlap(y0,y1,y2,y3))
// PointSide: Determine which side of a line the point is on. Return value: <0, =0 or >0.
#define PointSide(px,py, x0,y0, x1,y1) vxs((x1)-(x0), (y1)-(y0), (px)-(x0), (py)-(y0))
// Intersect: Calculate the point of intersection between two lines.
#define Intersect(x1,y1, x2,y2, x3,y3, x4,y4) ((struct xy) { \
    vxs(vxs(x1,y1, x2,y2), (x1)-(x2), vxs(x3,y3, x4,y4), (x3)-(x4)) / vxs((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)), \
    vxs(vxs(x1,y1, x2,y2), (y1)-(y2), vxs(x3,y3, x4,y4), (y3)-(y4)) / vxs((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)) })

static void vline(int x, int y1, int y2, int top, int middle, int bottom);
static void drawLine(int x1 , int x2 , int y1 , int y2 , int top, int middle, int bottom);
static void drawPlayer(int px , int py,  int playerSize , float angle , int top , int middle, int bottom);
static void drawBlack();

static SDL_Surface* surface = NULL;

int vx1 = 400;
int vy1 = 300;

int vx2 = 70;
int vy2 = 200;

float px = 50;
float py = 50;
float angle = 35;

int main() {
    surface = SDL_SetVideoMode(W , H , 32 , 0);
    SDL_EnableKeyRepeat(150 , 30);
    SDL_ShowCursor(SDL_DISABLE);
    SDL_WM_GrabInput(SDL_GRAB_ON);
    

    int wsad[4]={0,0,0,0};
    
    while(1) {
        int i;
        
        SDL_LockSurface(surface);
   //     for(i = 250 ; i < 750 ; i++) { 
   //         vline(i , 0 , H , 0xFFFFFF , 0xFFFFFF , 0xFFFFFF); 
   //     }
        drawBlack();
        drawLine(vx1 , vx2 , vy1 , vy2 ,0xFFFFFF,0xFFFFFF,0xFFFFFF);
        drawPlayer((int)px , (int)py , P , angle , 0x0000FF , 0x0000FF , 0x0000FF);
        SDL_UnlockSurface(surface);
        SDL_Flip(surface);



        SDL_Event ev;
        while(SDL_PollEvent(&ev))
            switch(ev.type)
            {
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    switch(ev.key.keysym.sym)
                    {
                        case 'w': wsad[0] = ev.type==SDL_KEYDOWN; break;
                        case 's': wsad[1] = ev.type==SDL_KEYDOWN; break;
                        case 'a': wsad[2] = ev.type==SDL_KEYDOWN; break;
                        case 'd': wsad[3] = ev.type==SDL_KEYDOWN; break;
                        case 'j': angle += 8.3f; break;
                        case 'l': angle -= 8.3f; break;
                        case 'q': goto done;
                        default: break;
                    }
                    break;
                case SDL_QUIT: goto done;
            }
        int x,y;

//        SDL_GetRelativeMouseState(&x,&y);
//        angle += x * 0.001f;
//        printf("X : %d\n" , x);
        SDL_Delay(10);

         float move_vec[2] = {0.f, 0.f};
        if(wsad[0]) { move_vec[0] += cosf(angle * 0.0174533)*1.2f; move_vec[1] -= sinf(angle * 0.0174533)*1.2f; }
        if(wsad[1]) { move_vec[0] -= cosf(angle * 0.0174533)*1.2f; move_vec[1] += sinf(angle * 0.0174533)*1.2f; }
        if(wsad[2]) { move_vec[0] -= sinf(angle * 0.0174533)*1.2f; move_vec[1] -= cosf(angle * 0.0174533)*1.2f; }
        if(wsad[3]) { move_vec[0] += sinf(angle * 0.0174533)*1.2f; move_vec[1] += cosf(angle * 0.0174533)*1.2f; }

        
        px = px + move_vec[0];
        py = py + move_vec[1];
    }
done:
    SDL_Quit();
    return 0;
}


static void drawPlayer(int px , int py,  int playerSize , float angle , int top , int middle, int bottom) {
//   
//    int i;
//    for(i = 0 ; i < playerSize ; i++) {
//        vline(px + i , py , py + playerSize , top , middle , bottom);
//    }
//
//    //This is one of the points we are drawing our angle line from.
//    int centerX = px + (playerSize/2);  
//    int centerY = py + (playerSize/2);
//
//    const int viewLength = 12; 
//
//    //Don't touch these numbers
//    float specialAngle = angle * 0.00268512307 * 6.5;
//
//    int x2 = (int)(cos(specialAngle) * viewLength) + centerX;
//    int y2 = -(int)(sin(specialAngle) * viewLength) + centerY;
//
//    drawLine(centerX , x2 , centerY , y2 , 0xFFFFFF , 0xFFFFFF , 0xFFFFFF);

        int i;
        for(i = 0 ; i < playerSize ; i++) {
            drawLine(W/2 - playerSize/2 + i , W/2 - playerSize/2 + i , H/2 - playerSize/2 , H/2 + playerSize/2 , top , middle , bottom);
        }

        drawLine(W/2 - playerSize/4 , W/2 - playerSize/4 , H/2 - playerSize/4 , H/2 - playerSize * 2, 0xFFFFFF , 0xFFFFFF , 0xFFFFFF);

}

static void drawLine(int x1 , int x2 , int y1 , int y2 , int top, int middle, int bottom) {
    int i;
    float delta;
    float size;

    if(x2 - x1 != 0) {
        delta = ((float)(y2 - y1) / (float)(x2 - x1));
    } else if(x2 == x1) {
        if(y1 > y2) {
            vline(x1 , y2 , y1 , top , middle,  bottom);   
        } else {
            vline(x1 , y1 , y2 , top , middle,  bottom);   
        }
        return;
    } else {
        delta = max(y2-y1 , y1-y2);
    }

    

    float countingY = y1; 

    if(max(x1 , x2) == x2) {
        for(i = x1 ; i <= x2 ; i++) {
            if(max(y1,y2) == y1) {
                vline(i , (int)countingY , (int)countingY - delta , top , middle , bottom);
                countingY += delta;
            } else {
                vline(i , (int)countingY , (int)countingY + delta , top , middle , bottom);
                countingY += delta;
            }
        }
    } else {
        countingY = y2;
        for(i = x2 ; i <= x1 ; i++) {
            if(max(y1,y2) == y1) {
                vline(i , (int)countingY - delta, (int)countingY , top , middle , bottom);
                countingY += delta;
            } else {
                vline(i , (int)countingY + delta, (int)countingY  , top , middle , bottom);
                countingY += delta;
            }
        }
    }
}
static void drawBlack() {
    int x;
    for(x = 0 ; x < W ; x++) {
        vline(x , 0 , H , 0x000000 , 0x000000 , 0x000000);
    }
}

static void vline(int x, int y1, int y2, int top, int middle, int bottom) {
    int *pix = (int*) surface->pixels;
    y1 = clamp(y1, 0, H-1);
    y2 = clamp(y2, 0, H-1);
    if(y2 == y1)
        pix[y1*W+x] = middle;
    else if(y2 > y1)
    {
        pix[y1*W+x] = top;
        for(int y=y1+1; y<y2; ++y) pix[y*W+x] = middle;
        pix[y2*W+x] = bottom;
    }
}

















