#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef ANIMATE
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define X_SIZE 800
#define Y_SIZE 800
#endif

#define prec float
#define PI 3.14159265359

typedef struct Star{
  //pos_x, pos_y, mass, velocity_x, velocity_y, force_x, force_y
  prec x, y, m, vx, vy, fx, fy;
} body;

static prec newPosition(prec *vel, prec force, prec pos, prec mass, prec dt){
  prec acc = force / mass;
  *vel = *vel + (acc * dt);
  return pos + ((*vel) * dt) + ((acc * pow(dt, 2) / 2));
}

static void update(body* a, prec dt){
  a->x = newPosition(&(a->vx), a->fx, a->x, a->m, dt);
  a->y = newPosition(&(a->vy), a->fy, a->y, a->m, dt);
}

//add the force to two stars
static void addForce(body* a, body* b){
  prec g = 9.82;
  prec dx = b->x - a->x;
  prec dy = b->y - a->y;
  prec dist = sqrt(pow(dy, 2) + pow(dx, 2));
  prec f = ((a->m * b->m) / dist)*g;
  a->fx = ((dx)/dist)*f;
  a->fy = ((dy)/dist)*f;
}

static prec newRand(){
  prec r = (prec)((double)rand()/(double)RAND_MAX);
  return r;
}

static void resetForce(body* star){
  star -> fx = 0;
  star -> fy = 0;
}

//Create and place the stars in a circle at start
static void init(int N, body* stars[]){
  int R = 50;
  for (int i=0; i<N; i++){
    body* star = malloc(sizeof(body));

    prec a = newRand();
    prec b = newRand();
    if (b<a){
      prec temp = b;
      b = a;
      a = temp;
    }
    star->x = 400 + b*R*cos(2*PI*a/b);
    star->y = 400 + b*R*sin(2*PI*a/b);
    star->m = 100;
    stars[i] = star;
  }
}

//go through every star and update their forces
static void updateForces(int N, body* stars[]){
  for (int i=0; i<N; i++){
    resetForce(stars[i]);
    for (int j=0; j<N; j++){
      if (i != j){
        addForce(stars[i], stars[j]);
      }
    }
  }
}

// Manually copy coordinates from stars into points (to be drawn).
// Look at the manual file for XPoint to see which
// format XPoint accepts its coordinates in.
#ifdef ANIMATE
static void copyToXBuffer(body* stars[], XPoint* points, int N){
  for (int i = 0; i < N; i++){
    points[i].x = (short)stars[i]->x;
    points[i].y = (short)stars[i]->y;
  }
}
#endif

int main(int argc, char* argv[]){
  int N = 200;
  int iter = 10000;

  if(argc == 3){
    N = atoi(argv[1]);
    iter = atoi(argv[2]);
  }

  body* stars[N];
  init(N, stars);

#ifdef ANIMATE
  XPoint* points = malloc(sizeof(XPoint)*N);
  Display* disp;
  Window window, rootwin;
  int screen;

  disp = XOpenDisplay(NULL);
  screen = DefaultScreen(disp);
  rootwin = RootWindow(disp,screen);
  window = XCreateSimpleWindow(disp,rootwin,
                               0,0,X_SIZE,Y_SIZE,1,0,0);
  GC gc = XCreateGC(disp, window, 0, 0);
  XSetForeground(disp, gc, WhitePixel(disp, screen));
  XSetBackground(disp, gc, BlackPixel(disp, screen));
  XMapWindow(disp,window);

  XClearWindow(disp,window);

  copyToXBuffer(stars, points, N);
  XDrawPoints(disp, window, gc, points, N, 0);

  XFlush(disp);

#endif

  clock_t start = clock();
  for(int i = 0; i < iter; i++)
    {
      updateForces(N, stars);
      for (int j=0; j<N; j++){
        update(stars[j], 0.001);
      }
#ifdef ANIMATE
      copyToXBuffer(stars, points, N);
      XDrawPoints(disp, window, gc, points, N, CoordModeOrigin);
      XClearWindow(disp,window);
#endif
    }
  for (int i = 0; i < N; i++) {
    free(stars[i]);
  }
  clock_t stop = clock();
  float diff = (float)(stop - start)/CLOCKS_PER_SEC;
  printf("Total: %lf seconds\n",diff);
  printf("Bodies: %d\n",N);
  printf("Iterations: %d\n", iter);

#ifdef ANIMATE
  XCloseDisplay(disp);
#endif

  return 0;
}
