int W = 600,
    H = 600;
int dx = 0,
    dy = 0;
PFont font;
color Yellow = color(255, 255, 0),      
      Green = color(0, 255, 0),
      Red = color(255, 0, 0),
      BG_C = Yellow;
Car C = new Car(40, 150, 100, 40, 20, Green, Red); 
int NumBs = 4;
 
void setup() {
  size(W, H);
  smooth();
  fill(Green);
}

void draw() {
  background(BG_C);
  C.Draw();
  C.Move();
}

void mousePressed() {
  C.FallApart(1);
}

void keyPressed() {
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
       



