/*
       ____________________                  _______________
       |||||||||||||||||| /                  ||||||||||||||  \
		      || /                   || ]           || \
		     || /                    || ]	      || \
		    || /                     || ]		|| \
		   || /                      || ]		  || \
		  || /                       || ]		   || \
		 || /                        || ]                   || \
	       ||  /            _____        || ]		      || |
		 || \           ~~~~~        || ]		      || |
		   || \                      || ]		     || /
     ----	     || \                    || ]		    || /
     \ || \	       || |                  || ]		   || /
      \ || \	     ||  /                   || ]		 || /
       \ || \_____ ||  /                     || ]	       || /
	\   ||  ||   /                       || ]	     || /
	  \________/                         |||||||||||||||  /

*/

#include <fstream.h>
#include <key.h>
#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <dos.h>
#include <process.h>
#include <mouse.h>

#define FILL_COLOR 9
#define LINE_COLOR 15

#define NORMAL_LIGHT 50
#define AMBIENT_LIGHT 250


double ori[2] = { 329,239 },view=512;
double sine[360] , cosi[360];

int n=1,maxx,maxy,jump=0;

	#define PALETTE_MASK			0x3c6
	#define PALETTE_REGISTER_RD		0x3c7
	#define PALETTE_REGISTER_WR		0x3c8
	#define PALETTE_DATA			0x3c9


void set_palette( int index, int r , int g , int b )
{
	///	      	sets the palette
	///		tell the vga card to update the palette
	outp (PALETTE_MASK, 0xff);
	//		tell the vga card the number
	outp (PALETTE_REGISTER_WR ,index);
	// 		update the rbg
	outp(PALETTE_DATA ,  r);
	outp(PALETTE_DATA ,  g);
	outp(PALETTE_DATA ,  b);
	//  		palette set
}

void setpalette(void)
{
 for(int i=0 ; i<256 ; i++)
  set_palette(i , i , i , i);
}

void initialize()

{
 setpalette();
 for(int i=0 ; i < 360 ; i++)
 {
   sine[i] = sin((i)*M_PI_2/90);
   cosi[i] = cos((i)*M_PI_2/90);
  }
 }

void waitretrace(void)
{

  _DX = 0x03DA;

  l1: asm{
	in  al,dx;
	and al,0x08;
	jnz l1;
      }

  l2: asm{
	in  al,dx;
	and al,0x08;
	jz  l2;
      }
}

int check(int pts[]);

struct point
{
 double x,y,z;


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
  void rot_x(long ax)
  {
//   draw_pt(0);
   if(ax>=360)
    ax%=360;
   if( ax < 0 )
     ax += 360;
   double t=y*cosi[ax] - z*sine[ax];
   z = z*cosi[ax] + y*sine[ax];
   y=t;
//   draw_pt(15);
   }

  void rot_y(long ay)
  {
//   draw_pt(0);
   if(ay>=360)
    ay%=360;
   if(ay < 0 )
     ay += 360;
   double t = x*cosi[ay]+z*sine[ay];
   z = z*cosi[ay]-x*sine[ay];
   x=t;
//   draw_pt(15);
   }

};

point light = {0 , 0 , 1};

point subtract_vector(point p1 , point p2)
{
 point p;
 p.x = p1.x - p2.x;
 p.y = p1.y - p2.y;
 p.z = p1.z - p2.z;

 return p;
}

double dot_product(point p1 , point p2)
{
 return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z );
}

point cross_product(point v1 , point v2)
{
  point v;
   v.x= (v1.y * v2.z) - (v2.y * v1.z);
   v.y= (v1.z * v2.x) - (v2.z * v1.x);
   v.z= (v1.x * v2.y) - (v2.x * v1.y);

   return v;
}

void normalize(point *n)
{
   double length=sqrt(n->x*n->x + n->y*n->y + n->z*n->z); //Pythagoras

   if(length==0) return;  //avoid division by zero

   n->x/=length;    //divide each dimension by the vector's length
   n->y/=length;    //and at the end you'll get a unit length
   n->z/=length;
}


struct _poly
{
 int ind[4];
 point normal;
 int c , c1;


 void calc_normal(point *p)
 {
  point p1 , p2;
  p1 = subtract_vector(p[ind[1]] , p[ind[2]]);
  p2 = subtract_vector(p[ind[0]] , p[ind[1]]);

  normal = cross_product(p2 , p1);
  normalize(&normal);

 }

 void calc_light(void)
 {
  double angle = dot_product(normal , light);

  //if(angle >= 0)
   c1 = NORMAL_LIGHT * angle + AMBIENT_LIGHT;
  //else
   //c1 = AMBIENT_LIGHT;

 }

};


struct obj
{
 int n,l;
 point *p,cen;
 _poly *pol;

 void get(char *ob,int x,int y,int z)
 {

   ifstream in(ob);
   if(!in)
    exit(0);
   cen.x=x;
   cen.y=y;
   cen.z=z;
   in>>n;
   p = new point[n];
   for(int i=0;i<n;i++)
      in>>p[i].x>>p[i].y>>p[i].z;
   in>>l;
   pol = new _poly [l];

   for(i=0 ; i<l ; i++)
    {
     for(int j=0 ; j<4 ; j++)
      in>>pol[i].ind[j];
     in>>pol[i].c;
     }
   in.close();
  }

  void draw_o()
  {

   int a=0;
   int pts[10];

   for(int i=0 ; i<l ; i++)
   {
     pol[i].calc_normal(p);
     if(pol[i].normal.z > 0 )
      { for(int j=0 ; j<4 ; j++)
	{
	 pts[j*2]   = ori[0]+(cen.x + p[pol[i].ind[j]].x)*view/(cen.z+p[pol[i].ind[j]].z);
	 pts[j*2+1] = ori[1]+(cen.y + p[pol[i].ind[j]].y)*view/(cen.z+p[pol[i].ind[j]].z);
	}

	pts[8] = pts[0];
	pts[9] = pts[1];

       if(check(pts))
       {
	pol[i].calc_light();
	setfillstyle(1 , pol[i].c1);
	setcolor(pol[i].c1);
	fillpoly(5 , pts);
	setcolor(pol[i].c1);
	drawpoly(5 , pts);
	}
       }

   }

   delay(10);
   }

  void rot_o_x(int ang)
  {
     for(int i=0;i<n;i++)
       p[i].rot_x(3*ang);

   }
  void rot_o_y(int ang)
  {
   for(int i=0 ; i<n ; i++)
     p[i].rot_y(3*ang);
   }
  void rot_y(int ang)
   {
    cen.rot_y(3*ang);
    rot_o_y(ang);
    }
   void rot_x(int ang)
   {
    cen.rot_x(3*ang);
    rot_o_x(ang);
    }
  void mov_x(int amt)
   {
    cen.x += amt;
    }
  void mov_z(int amt)
   {
    cen.z += amt;
    }
}obj[4];


#include "culling.cpp"

int huge vgamode(void)
{
 return 4;
}

void main()
{
 int dri=0,md;
 installuserdriver("svga256" , vgamode);
 initgraph(&dri,&md,"c:\\tc\\bgi");
 maxx=getmaxx();
 maxy=getmaxy();
 place();
 initialize();
 install_keyboard();
 while(!key_ESC)
 {
  if(key_A)
    rot_wrld_y(3);
  if(key_D)
    rot_wrld_y(-3);
/*  if(key_W)
    rot_wrld_x(-1);
  if(key_S)
    rot_wrld_x(1);*/
  if(key_SPACEBAR)
    jump_seq();
  if(key_J)
    mov_wrld_x(15);
  if(key_L)
    mov_wrld_x(-15);
  if(key_I)
    mov_wrld_z(-10);
  if(key_K)
    mov_wrld_z(10);
  }
 closegraph();
 uninstall_keyboard();
 destroy_wrld();
}
