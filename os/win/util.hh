// WINDOWS UTIL

#ifndef WIN_UTIL_HH
#define WIN_UTIL_HH

#include "../../gl/image.hh"

// 10 mil pixel buffer for image conversion
ui bmp_data[10000000]={0};

// Convert bitmap file to image object
//! Only works for certain bitmap sizes, e.g. 60x90
image load_bmp(str dir){
  HBITMAP hbmp = (HBITMAP)LoadImage(NULL, dir.c_str(), IMAGE_BITMAP,
      0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
  BITMAP b;
  GetObject(hbmp, sizeof(BITMAP), &b);
  int w = b.bmWidth, h = b.bmHeight;
  image img(ipoint(w, h));
  int area = w * h;
  for(int k = 0; k < area * 3; k += 3){
    int i = (k / 3) / w;
    int j = (k / 3) % w;
    img.set_pixel(ipoint(j, h-i-1), color(((uchar*)b.bmBits)[k],
        ((uchar*)b.bmBits)[k+1], ((uchar*)b.bmBits)[k+2])); }
  return img; }

// Convert image object to bitmap file
//! Saves it upside down
void save_bmp(image f, str file){
  for(int i = 0; i < f.size.y; ++i)
    for(int j = 0; j < f.size.x; ++j)
      bmp_data[i * f.size.x + j] = ((ui)f.data[i][j].r << 16)
          | ((ui)f.data[i][j].g << 8) | f.data[i][j].b;
  BITMAPINFOHEADER ih;
  ih.biSize = sizeof(BITMAPINFOHEADER);
  ih.biBitCount = 32;
  ih.biPlanes = 1;
  ih.biCompression = BI_RGB;
  ih.biWidth = f.size.x;
  ih.biHeight = f.size.y;
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

// Convert image object to Windows bitmap
HBITMAP image_to_bmp(image* f, const ipoint& pos, const ipoint& size){
  for(int i = pos.y; i < pos.y + size.y; ++i)
    for(int j = pos.x; j < pos.x + size.x; ++j)
      bmp_data[(i - pos.y) * size.x + j - pos.x] = ((ui)f->data[i][j].r << 16)
          | ((ui)f->data[i][j].g << 8) | f->data[i][j].b;
  return CreateBitmap(size.x, size.y, 1, 32, bmp_data); }

#endif
