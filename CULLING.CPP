int check(int pts[])
{
 if( (pts[2] - pts[0])*(pts[4] - pts[0]) - (pts[3]-pts[1])*(pts[5] - pts[1]) > 0)
   return 1;
 else
   return 0;
}
void place()
{
 obj[0].get("CAR.inf",0,0,500);
 obj[0].draw_o();
/* obj[1].get("cube2.inf",150,0,500);
 obj[1].draw_o();
 obj[2].get("helix.inf",500,0,0);
 obj[2].draw_o(15);
 obj[3].get("pyramid5.inf",-500,0,0);
 obj[3].draw_o(15);*/

}

void clean_wrld()
{
  //waitretrace();
  setfillstyle(1 , 0);
  bar(0 , 0 , maxx , maxy);
}
void rot_wrld_y(int ang)
{
  clean_wrld();
  for(int i=0;i<n;i++)
    {
     obj[i].rot_y(ang);
     obj[i].draw_o();
     }
//  light.rot_y(ang);
}

void rot_wrld_x(int ang)
{
  clean_wrld();
  for(int i=0;i<n;i++)
    {
     obj[i].rot_x(ang);
     obj[i].draw_o();
     }
 }

void mov_wrld_x(int dis)
{
  clean_wrld();
  for(int i=0;i<n;i++)
    {
     obj[i].mov_x(dis);
     obj[i].draw_o();
     }
}

void mov_wrld_z(int dis)
{
  clean_wrld();
  for(int i=0;i<n;i++)
    {
     obj[i].mov_z(dis);
     obj[i].draw_o();
     }
}

void destroy_wrld()
{
 for(int i=0 ; i<n ; i++)
 {
  delete obj[i].pol;
  delete obj[i].p;
 }
}

void jump_seq()
{
 //jump=1;
 }
