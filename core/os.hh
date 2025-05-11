// OS UTIL

#ifndef OS_HH
#define OS_HH

#include "const.hh"

ui bmp_data[10000000]; // 10 mil pixels

LPCWSTR str_to_lpcw(str s){
  int size = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
  wchar_t* r = new wchar_t[size];
  MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, r, size);
  return r; }

image load_bmp(str dir){
  HBITMAP bmp = (HBITMAP)LoadImage(NULL, str_to_lpcw(dir), IMAGE_BITMAP,
      0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
  BITMAP b;
  GetObject(bmp, (int)sizeof(b), &b);
  point size(b.bmWidth, b.bmHeight);
  image img(point(size.x, size.y));
  int area = size.x * size.y;
  for(int k = 0; k < area * 3; k += 3){
    int i = k / (size.x * 3);
    int j = (k / 3) % size.x;
    img.data[i][j] = color(((uchar*)b.bmBits)[k], ((uchar*)b.bmBits)[k+1],
        ((uchar*)b.bmBits)[k+2]); }
  return img; }

HBITMAP image_to_bmp(image& f){
  for(int i = 0; i < f.size.y; ++i)
    for(int j = 0; j < f.size.x; ++j)
      bmp_data[i * f.size.y + j] |= ((ui)f.data[i][j].r << 16)
          | ((ui)f.data[i][j].g << 8) | f.data[i][j].b;
  return CreateBitmap(f.size.x, f.size.y, 1, 32, bmp_data); }

#endif
