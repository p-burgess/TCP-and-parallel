class Block {
  int Lx,
      Ty;
  int W,
      H;  
  int dx,
      dy;
  color FillCol;            
 
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
  void Draw() {
    fill(FillCol);
    rect(Lx, Ty, W, H);    
  } 
  
  // PRE  TRUE
  // POST Moves this (dx, dy)
  void Move() {
    Lx += dx;
    Ty += dy;
  }
}
