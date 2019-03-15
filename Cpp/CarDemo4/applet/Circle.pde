class Circle {
  int Cx,
      Cy;
  int dx,
      dy;
  int D;
  color FillCol;            
 
  Circle(int Cx, int Cy, int D) {
    
    this.Cx = Cx;
    this.Cy = Cy;
    this.D = D;
  }
  
  // PRE  TRUE
  // POST Circle drawn on canvas
  void Draw() {
    fill(FillCol);
    ellipse(Cx, Cy, D, D);    
  } 
  
  // PRE  TRUE
  // POST Moves this (dx, dy)
  void Move() {
    Cx += dx;
    Cy += dy;
  }
}
