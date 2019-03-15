import processing.core.*; 
import processing.xml.*; 

import java.applet.*; 
import java.awt.*; 
import java.awt.image.*; 
import java.awt.event.*; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class q1 extends PApplet {

int W = 400,
    H = W;

Star S = new Star(120, 100, 20);
 int Yellow = color(255, 255, 0),
        Green = color(0, 255, 0);

public void setup() { 
  size(W, H); 
  background(Yellow); 
  S.Set_FillCol(Green);
  S.Draw();
  S.Set_dxdy(1, 1);  
}

public void AdjustVelocity(Star S) {
  int[] xys = S.Get_Extremes();
  if(xys[0] < 0 || W < xys[2]) {
    S.xReverse();
  }
  if(xys[1] < 0 || H < xys[3]) {
    S.yReverse();
  }
}

public void draw() {
  AdjustVelocity(S);
  S.Move();
  background(Yellow);
  S.Draw();
}

public void keyPressed() {
  switch(key) {
    case 'l':
      S.Incr_dxdy(-1, 0);
    break;
    case 'u':
      S.Incr_dxdy(0, -1);
    break;
    case 'r':
      S.Incr_dxdy(1, 0);
    break;
    case 'd':
      S.Incr_dxdy(0, 1);
    break;
    case 's':
      S.Incr_dxdy(0, 0);
    break;
  }
}  

class Star {
  int Cx;
  int Cy;
  int Side;
  int dx;
  int dy;
  int FillCol;
  
  Star (int Cx, int Cy, int Side){
    this.Cx = Cx;
    this.Cy = Cy;
    this.Side = Side;
  
     FillCol = Green;
  }
  
  public void Move() {
    Cx += dx;
    Cy += dy;
  }

  public void Set_dxdy(int dx, int dy) {
    this.dx = dx;
    this.dy = dy;
  }
    
  public void Incr_dxdy(int dx, int dy) {
    this.dx += dx;
    this.dy += dy;
  }
    
  public void Set_FillCol(int C) {
    FillCol = C;
  }
  
  public void Draw() {
    fill(FillCol);
    noStroke();
    beginShape();
      vertex(Cx - Side - Side/2, Cy);    // vertex 1
      vertex(Cx - Side/2, Cy + Side/2);  // vertex 2
      vertex(Cx , Cy + Side + Side/2);   // vertex 3
      vertex(Cx + Side/2, Cy + Side/2);  // vertex 4
      vertex(Cx + Side + Side/2, Cy);    // vertex 5
      vertex(Cx + Side/2, Cy - Side/2);  // vertex 6
      vertex(Cx , Cy - Side - Side/2);   // vertex 7
      vertex(Cx - Side/2, Cy - Side/2);  // vertex 8
   endShape(CLOSE);
   
  }
  
  public void xReverse() {
    this.dx = dx;
    dx = - dx;
  }
    
  public void yReverse() {
    this.dy = dy;
    dy = - dy;
  }
    
  public int[] Get_Extremes() {
    int[] xys = new int[4];
    xys[0] = Cx - Side/2 - Side;
    xys[1] = Cy - Side/2 - Side;
    xys[2] = Cx + Side/2 + Side;
    xys[3] = Cy + Side/2 + Side;
    return(xys);
  }   
}
class Star {
  int Cx;
  int Cy;
  int Side;
  int dx;
  int dy;
  int FillCol;
  
  Star (int Cx, int Cy, int Side){
    this.Cx = Cx;
    this.Cy = Cy;
    this.Side = Side;
  
     FillCol = Green;
  }
  
  public void Move() {
    Cx += dx;
    Cy += dy;
  }

  public void Set_dxdy(int dx, int dy) {
    this.dx = dx;
    this.dy = dy;
  }
    
  public void Incr_dxdy(int dx, int dy) {
    this.dx += dx;
    this.dy += dy;
  }
    
  public void Set_FillCol(int C) {
    FillCol = C;
  }
  
  public void Draw() {
    fill(FillCol);
    noStroke();
    beginShape();
      vertex(Cx - Side - Side/2, Cy);    // vertex 1
      vertex(Cx - Side/2, Cy + Side/2);  // vertex 2
      vertex(Cx , Cy + Side + Side/2);   // vertex 3
      vertex(Cx + Side/2, Cy + Side/2);  // vertex 4
      vertex(Cx + Side + Side/2, Cy);    // vertex 5
      vertex(Cx + Side/2, Cy - Side/2);  // vertex 6
      vertex(Cx , Cy - Side - Side/2);   // vertex 7
      vertex(Cx - Side/2, Cy - Side/2);  // vertex 8
   endShape(CLOSE);
   
  }
  
  public void xReverse() {
    this.dx = dx;
    dx = - dx;
  }
    
  public void yReverse() {
    this.dy = dy;
    dy = - dy;
  }
    
  public int[] Get_Extremes() {
    int[] xys = new int[4];
    xys[0] = Cx - Side/2 - Side;
    xys[1] = Cy - Side/2 - Side;
    xys[2] = Cx + Side/2 + Side;
    xys[3] = Cy + Side/2 + Side;
    return(xys);
  }   
}
class Triangle {
  int x1,
      y1,
      x2,
      y2,
      x3, 
      y3;
  int dx,
      dy;
  int FillCol;            
 
  Triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    
    this.x1 = x1;
    this.y1 = y1;
    this.x2 = x2;
    this.y2 = y2;
    this.x3 = x3;
    this.y3 = y3;
  }
  
  // PRE  TRUE
  // POST Circle drawn on canvas
  public void Draw() {
    fill(FillCol);
    triangle(x1, y1, x2, y2, x3, y3);    
  } 
  
  // PRE  TRUE
  // POST Moves this (dx, dy)
  public void Move() {
    x1 += dx;
    y1 += dy;
    x2 += dx;
    y2 += dy;
    x3 += dx;
    y3 += dy;
  }
}

  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#ece9d8", "q1" });
  }
}
