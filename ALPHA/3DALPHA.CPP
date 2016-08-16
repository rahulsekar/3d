/*
      My first 3d program....it was very good except that i found a
      better algorithm to rotate a point...so i started working on the
      new algorithm.
*/

#include <fstream.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>
#include <dos.h>
#include <process.h>

double ori[2] = { 329,239 },view=512;
double sine[360] , cosi[360];

void initialize()
{
 for(int i=0 ; i < 360 ; i++)
 {
   sine[i] = sin((i)*M_PI_2/90);
   cosi[i] = cos((i)*M_PI_2/90);
  }
 }

struct point
{
 double x,y,z;
 double d,rx,ry;
 int ax,ay;
 point()
 {
  x=y=z=d=ax=ay=rx=ry=0;
  }
 long angle(double a,double r)
 {
  if(!r)
   return 0;
  long an;

  z*=100;
  r*=100;
  z = long(z);
  r = long(r);
  if(a >= 0)
   an = ( asin(z/r) )*180/M_PI ;
  else
   an = ( asin((-z)/r) )*180/M_PI + 180 ;
  z/=100;
  r/=100;
  return an;
  }

  void init()
  {
   d =  ( powl(x*x + y*y + z*z ,0.5) );
   rx = ( powl( d*d - x*x ,0.5) );
   ry = ( powl( d*d - y*y ,0.5) );

   ax = angle(y,rx);
   ay = angle(x,ry);
   }
  void draw_pt(int col)
  {
    setcolor(col);
    if(col==15)
    {
     if(z<=0)
      setcolor(WHITE);
     else
      setcolor(BROWN);
     }
    circle(ori[0]+x,ori[1]+y,2);
   }
  void rot_x(long ang)
  {
//   draw_pt(0);
   ax = (ax+ang)%360;
   if( ax < 0 )
     ax += 360;

   y = rx * cosi[ax] ;
   z = rx * sine[ax] ;
   ry = powl( d*d - y*y ,0.5);
   ay = angle(x,ry);
//   draw_pt(15);
   }

  void rot_y(long ang)
  {
//   draw_pt(0);
   ay = (ay+ang)%360;
   if(ay < 0 )
     ay += 360;

   x = ry * cosi[ay];
   z = ry * sine[ay];

   rx = powl( d*d - x*x ,0.5);
   ax = angle(y,rx);
/*   z = z*cosi[ay]-x*sine[ay];
   x = x*cosi[ay]+z*sine[ay];*/
//   draw_pt(15);
   }

};

struct obj
{
 int n,l,*ln[2];
 char ob[15];
 point *p,cen;

 void get()
 {
   cout<<" File : ";
   cin>>ob;
   ifstream in(ob);
   if(!in)
    exit(0);
   in>>cen.x>>cen.y>>cen.z;
   cen.init();
   in>>n;
   p = new point[n];
   for(int i=0;i<n;i++)
     {
      in>>p[i].x>>p[i].y>>p[i].z;
      p[i].init();
      }
   in>>l;
   ln[0] = new int[l];
   ln[1] = new int[l];
   for(i=0 ; i<l ; i++)
     in>>ln[0][i]>>ln[1][i];
   in.close();
  }

  void draw_o(int col)
  {
   if(cen.z <= 0 && col)
    return;
   setcolor(col);
   gotoxy(1,1);
   cout<<"                  ";
   gotoxy(1,1);
   cout<<cen.x<<" "<<cen.y<<" "<<cen.z;

   for(int i=0;i<l;i++)
     {
      int x1=ori[0]+(cen.x + p[ln[0][i]].x)*view/(cen.z+p[ln[0][i]].z);
      int y1=ori[1]+(cen.y + p[ln[0][i]].y)*view/(cen.z+p[ln[0][i]].z);
      int x2=ori[0]+(cen.x + p[ln[1][i]].x)*view/(cen.z+p[ln[1][i]].z);
      int y2=ori[1]+(cen.y + p[ln[1][i]].y)*view/(cen.z+p[ln[1][i]].z);
      line(x1,y1,x2,y2);
      }

   }

  void rot_o_x(int ang)
  {
   draw_o(0);
     for(int i=0;i<n;i++)
       p[i].rot_x(3*ang);

   draw_o(15);
   }
  void rot_o_y(int ang)
  {
   draw_o(0);
   for(int i=0 ; i<n ; i++)
     p[i].rot_y(3*ang);
   draw_o(15);
   }
  void mov_x(int amt)
  {
    draw_o(0);
    cen.x += amt;
    draw_o(15);
   }
  void mov_z(int amt)
  {
    draw_o(0);
    cen.z += amt;
    draw_o(15);
   }

};
void main()
{
 int dri=0,md;
 initgraph(&dri,&md,"c:\\tc\\bgi");
 char ch = 'y';
 obj o;
 initialize();
 o.get();
 o.draw_o(15);
 while(ch != 'q')
 {
  ch = 'y';
  if(kbhit())
   ch = getch();
  if(ch == 'a')
    o.rot_o_y(-1);
  if(ch == 'd')
     o.rot_o_y(1);
  if(ch == 'w')
     o.rot_o_x(-1);
  if(ch == 's')
     o.rot_o_x(1);

  if(ch == 'j')
    o.mov_x(15);
  if(ch == 'l')
    o.mov_x(-15);
  if(ch == 'i')
    o.mov_z(-10);
  if(ch == 'k')
    o.mov_z(10);

  }
 closegraph();
 delete o.p, o ;
}