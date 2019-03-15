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

public class CarDemo4 extends PApplet {

int W = 600,
    H = 600;
int dx = 0,
    dy = 0;
PFont font;
int Yellow = color(255, 255, 0),      
      Green = color(0, 255, 0),
      Red = color(255, 0, 0),
      BG_C = Yellow;
Car C = new Car(40, 150, 100, 40, 20, Green, Red); 
int NumBs = 4;
 
public void setup() {
  size(W, H);
  smooth();
  fill(Green);
}

public void draw() {
  background(BG_C);
  C.Draw();
  C.Move();
}

public void mousePressed() {
  C.FallApart(1);
}

public void keyPressed() {
  switch(key) {
    case 'h':
      C.Incr_Velocity(2, 0);
    break;
    case 't':
      C.Incr_Velocity(0, -2);
    break;
    case 'f':
      C.Incr_Velocity(-2, 0);
    break;
    case 'b':
      C.Incr_Velocity(0, 2);
    break;
    case 'g':
      C.Set_Velocity(0, 0);
    break;
   }
}  
       



class Block {
  int Lx,
      Ty;
  int W,
      H;  
  int dx,
      dy;
  int FillCol;            
 
  Block(int Lx, int Ty, int W, int H) {
    
    this.Lx = Lx;
    this.Ty = Ty;
    this.dx = dx;
    this.dy = dy;
    this.W = W;
    this.H = H;
    this.FillCol = FillCol;
  }
  
  // PRE  TRUE
  // POST Block drawn on canvas
  public void Draw() {
    fill(FillCol);
    rect(Lx, Ty, W, H);    
  } 
  
  // PRE  TRUE
  // POST Moves this (dx, dy)
  public void Move() {
    Lx += dx;
    Ty += dy;
  }
}
class Car {
  Block Body;
  Block Roof;
  Circle Wheel1,
         Wheel2;
  
  Car(int Lx, int Ty, int W, int H, int WR, int BCol, int WCol) {
    Body = new Block(Lx, Ty, W, H);
    Roof = new Block(Lx-5, Ty-10, W+10, 10);
    Wheel1 = new Circle(Lx, Ty + H, 2 * WR);
    Wheel2 = new Circle(Lx + W, Ty + H, 2 * WR); 
    Body.FillCol = BCol;
    Wheel1.FillCol = WCol;   
    Wheel2.FillCol = WCol;   
  }
  
  public void Draw() {
    Body.Draw();
    Roof.Draw();
    Wheel1.Draw();
    Wheel2.Draw();
  }
  
  public void Set_Velocity(int dx, int dy) {
    Body.dx = dx;
    Body.dy = dy;
    Roof.dx = dx;
    Roof.dy = dy;
    Wheel1.dx = dx;
    Wheel1.dy = dy; 
    Wheel2.dx = dx;
    Wheel2.dy = dy; 
  }
  
  public void Incr_Velocity(int dx, int dy) {
    Body.dx += dx;
    Body.dy += dy;
    Roof.dx += dx;
    Roof.dy += dy;
    Wheel1.dx += dx;
    Wheel1.dy += dy; 
    Wheel2.dx += dx;
    Wheel2.dy += dy; 
  }
  
  public void Move() {
    Body.Move();
    Roof.Move();
    Wheel1.Move();
    Wheel2.Move();
  }

  public void FallApart(int v) {
    Body.Ty += Body.H/2;
    Body.dx = 0;
    Body.dy = 0;
    Roof.dy = - v;
    Wheel1.dx = - v;
    Wheel2.dx = v;    
  }       
}    
class Circle {
  int Cx,
      Cy;
  int dx,
      dy;
  int D;
  int FillCol;            
 
  Circle(int Cx, int Cy, int D) {
    
    this.Cx = Cx;
    this.Cy = Cy;
    this.D = D;
  }
  
  // PRE  TRUE
  // POST Circle drawn on canvas
  public void Draw() {
    fill(FillCol);
    ellipse(Cx, Cy, D, D);    
  } 
  
  // PRE  TRUE
  // POST Moves this (dx, dy)
  public void Move() {
    Cx += dx;
    Cy += dy;
  }
}

  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#ece9d8", "CarDemo4" });
  }
}
