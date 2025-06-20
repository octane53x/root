// OS UTIL

#ifndef OS_HH
#define OS_HH

#include "obj.hh"

ui bmp_data[10000000]={0}; // 10 mil pixels

LPCWSTR str_to_lpcw(str s){
  int size = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
  wchar_t* r = new wchar_t[size];
  MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, r, size);
  return r; }

image load_bmp(str dir){
#ifdef _WIN32
  HBITMAP hbmp = (HBITMAP)LoadImage(NULL, str_to_lpcw(dir), IMAGE_BITMAP,
      0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
#else
  HBITMAP hbmp = (HBITMAP)LoadImage(NULL, dir.c_str(), IMAGE_BITMAP,
      0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
#endif
  BITMAP b;
  GetObject(hbmp, sizeof(BITMAP), &b);
  int w = b.bmWidth, h = b.bmHeight;
  image img(w, h);
  int area = w * h;
  for(int k = 0; k < area * 3; k += 3){
    int i = (k / 3) / w;
    int j = (k / 3) % w;
    img.data[h-i-1][j] = color(((uchar*)b.bmBits)[k], ((uchar*)b.bmBits)[k+1],
        ((uchar*)b.bmBits)[k+2]); }

  return img; }

void save_bmp(image f, str file){
  for(int i = 0; i < f.height; ++i)
    for(int j = 0; j < f.width; ++j)
      bmp_data[i * f.width + j] = ((ui)f.data[i][j].r << 16)
          | ((ui)f.data[i][j].g << 8) | f.data[i][j].b;
  BITMAPINFOHEADER ih;
  ih.biSize = sizeof(BITMAPINFOHEADER);
  ih.biBitCount = 32;
  ih.biPlanes = 1;
  ih.biCompression = BI_RGB;
  ih.biWidth = f.width;
  ih.biHeight = f.height;
  ih.biSizeImage = ((((ih.biWidth * ih.biBitCount) + 31) & ~31) >> 3)
      * ih.biHeight;
  BITMAPFILEHEADER fh;
  fh.bfType = 'B'+('M'<<8);
  fh.bfOffBits = sizeof(BITMAPFILEHEADER) + ih.biSize;
  fh.bfSize = fh.bfOffBits + ih.biSizeImage;
  fh.bfReserved1 = fh.bfReserved2 = 0;
  FILE* fp;
#ifdef _WIN32
  fopen_s(&fp, file.c_str(), "wb");
#else
  fp = fopen(file.c_str(), "wb");
#endif
  fwrite(&fh, 1, sizeof(BITMAPFILEHEADER), fp);
  fwrite(&ih, 1, sizeof(BITMAPINFOHEADER), fp);
  fwrite(bmp_data, 1, ih.biSizeImage, fp);
  fclose(fp); }

HBITMAP image_to_bmp(HDC hdc, image* f){
  for(int i = 0; i < f->height; ++i)
    for(int j = 0; j < f->width; ++j)
      bmp_data[i * f->width + j] = ((ui)f->data[i][j].r << 16)
          | ((ui)f->data[i][j].g << 8) | f->data[i][j].b;
  return CreateBitmap(f->width, f->height, 1, 32, bmp_data); }

#endif
