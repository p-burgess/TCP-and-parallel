class Car {
  Block Body;
  Block Roof;
  Circle Wheel1,
         Wheel2;
  
  Car(int Lx, int Ty, int W, int H, int WR, color BCol, color WCol) {
    Body = new Block(Lx, Ty, W, H);
    Roof = new Block(Lx-5, Ty-10, W+10, 10);
    Wheel1 = new Circle(Lx, Ty + H, 2 * WR);
    Wheel2 = new Circle(Lx + W, Ty + H, 2 * WR); 
    Body.FillCol = BCol;
    Wheel1.FillCol = WCol;   
    Wheel2.FillCol = WCol;   
  }
  
  void Draw() {
    Body.Draw();
    Roof.Draw();
    Wheel1.Draw();
    Wheel2.Draw();
  }
  
  void Set_Velocity(int dx, int dy) {
    Body.dx = dx;
    Body.dy = dy;
    Roof.dx = dx;
    Roof.dy = dy;
    Wheel1.dx = dx;
    Wheel1.dy = dy; 
    Wheel2.dx = dx;
    Wheel2.dy = dy; 
  }
  
  void Incr_Velocity(int dx, int dy) {
    Body.dx += dx;
    Body.dy += dy;
    Roof.dx += dx;
    Roof.dy += dy;
    Wheel1.dx += dx;
    Wheel1.dy += dy; 
    Wheel2.dx += dx;
    Wheel2.dy += dy; 
  }
  
  void Move() {
    Body.Move();
    Roof.Move();
    Wheel1.Move();
    Wheel2.Move();
  }

  void FallApart(int v) {
    Body.Ty += Body.H/2;
    Body.dx = 0;
    Body.dy = 0;
    Roof.dy = - v;
    Wheel1.dx = - v;
    Wheel2.dx = v;    
  }       
}    
