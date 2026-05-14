// SELECT OPERATIONS

#ifndef PANEL_SELECT
#define PANEL_SELECT

void InputPanel::set_mark(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  if(p.mark.y == -1){
    p.mark.y = c.pos.y, p.mark.x = c.pos.x;
    return; }
  ipoint p0 = (p.mark.y < c.pos.y
      || (p.mark.y == c.pos.y && p.mark.x < c.pos.x)) ? p.mark : c.pos;
  ipoint pf = (p.mark == p0) ? c.pos : p.mark;
  p.mark = ipoint(-1, -1);
  p.draw_selection(p0, ipoint(pf.x - 1, pf.y), true); }

void InputPanel::select_line(){

}

void InputPanel::select_block(){

}

void InputPanel::select_all(){
  Panel& p = *focus;
  Cursor& c = p.cursor;
  p.mark = ipoint((int)p.text.back().size(), (int)p.text.size() - 1);
  c.pos = ipoint(0, 0);
  p.top_line = 0;
  p.draw(true); }

#endif
