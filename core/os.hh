// OS UTIL

#ifndef OS_HH
#define OS_HH

#include "const.hh"

ui bmp_data[10000000]; // 10 mil pixels

LPCWSTR str_to_lpcw(str s){
  int size = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
  wchar_t* r = new wchar_t[size];
  MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, r, size);
  return r;
}

image load_bmp(str dir){
  HBITMAP bmp = (HBITMAP)LoadImage(NULL, str_to_lpcw(dir), IMAGE_BITMAP,
      0, 0, LR_LOADFROMFILE);
  SIZE size;
  GetBitmapDimensionEx(bmp, &size);
  image img(point(size.cx, size.cy));
  int area = size.cx * size.cy;
  GetBitmapBits(bmp, area * 4, &bmp_data);
  for(int k = 0; k < area; ++k){
    int i = k / size.cy;
    int j = k % size.cy;
    ui pix = bmp_data[k];
    img.data[i].pb(color(pix >> 16, (pix << 16) >> 24, (pix << 24) >> 24));
  }
  return img;
}

HBITMAP image_to_bmp(image& f){
  for(int i = 0; i < f.size.x; ++i)
    for(int j = 0; j < f.size.y; ++j)
      bmp_data[i * f.size.x + j] |=
          ((ui)f.data[i][j].r << 16) | ((ui)f.data[i][j].g << 8)
          | f.data[i][j].b;
  return CreateBitmap(f.size.x, f.size.y, 1, 32, bmp_data);
}

#endif
