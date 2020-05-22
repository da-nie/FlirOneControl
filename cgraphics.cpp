#include "cgraphics.h"

//----------------------------------------------------------------------------------------------------
//���������� ����������
//----------------------------------------------------------------------------------------------------

static unsigned char Font8x14[224][14]={
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 //{ 0x00, 0x10, 0x38, 0x38, 0x38, 0x10, 0x10, 0x10, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00},
 { 0x40, 0xA0, 0x4E, 0x1F, 0x19, 0x18, 0x18, 0x18, 0x19, 0x1F, 0x0E, 0x00, 0x00, 0x00},
 { 0x00, 0x66, 0x66, 0x22, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
 { 0x00, 0x44, 0x44, 0xFE, 0xFE, 0x44, 0x44, 0xFE, 0xFE, 0x44, 0x44, 0x00, 0x00, 0x00},
 { 0x18, 0x18, 0x7C, 0xC6, 0xC2, 0xC0, 0x7C, 0x06, 0x06, 0x86, 0xC6, 0x7C, 0x18, 0x18}, 
 { 0x00, 0x00, 0x00, 0xC2, 0xC6, 0x0C, 0x18, 0x30, 0x60, 0xC6, 0x86, 0x00, 0x00, 0x00}, 
 { 0x00, 0x38, 0x6C, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00}, 
 { 0x00, 0x30, 0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0x7C, 0x7C, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x30, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00}, 
 { 0x02, 0x06, 0x04, 0x0C, 0x08, 0x18, 0x10, 0x30, 0x20, 0x60, 0x40, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0xCE, 0xDE, 0xF6, 0xE6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x18, 0x38, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0xC6, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0x06, 0x06, 0x3C, 0x06, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x0C, 0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0xC0, 0xC0, 0xC0, 0xFC, 0x06, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x38, 0x60, 0xC0, 0xC0, 0xFC, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0xC6, 0x06, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x06, 0x06, 0x0C, 0x78, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x00, 0x00, 0x7E, 0x7E, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0x0C, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xDE, 0xDE, 0xDE, 0xDC, 0xC0, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xC0, 0xC0, 0xC2, 0x66, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xF8, 0x6C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68, 0x60, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xDE, 0xC6, 0xC6, 0x66, 0x3A, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 0x00, 0x00}, 
 { 0x00, 0xE6, 0x66, 0x66, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xF0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xE6, 0xF6, 0xFE, 0xDE, 0xCE, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xDE, 0x7C, 0x0C, 0x0E, 0x00}, 
 { 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0x60, 0x38, 0x0C, 0x06, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7E, 0x7E, 0x5A, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x10, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xD6, 0xD6, 0xFE, 0xEE, 0x6C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0x6C, 0x7C, 0x38, 0x38, 0x7C, 0x6C, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0xC6, 0x86, 0x0C, 0x18, 0x30, 0x60, 0xC2, 0xC6, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3C, 0x3C, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x40, 0x60, 0x20, 0x30, 0x10, 0x18, 0x08, 0x0C, 0x04, 0x06, 0x02, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3C, 0x3C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x3C, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00}, 
 { 0x00, 0x20, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00}, 
 { 0x00, 0xE0, 0x60, 0x60, 0x78, 0x6C, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC0, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x1C, 0x0C, 0x0C, 0x3C, 0x6C, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x38, 0x6C, 0x64, 0x60, 0xF0, 0x60, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xCC, 0x78, 0x00}, 
 { 0x00, 0xE0, 0x60, 0x60, 0x6C, 0x76, 0x66, 0x66, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x18, 0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x06, 0x06, 0x00, 0x0E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C, 0x00}, 
 { 0x00, 0xE0, 0x60, 0x60, 0x66, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0xE6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xEC, 0xFE, 0xD6, 0xD6, 0xD6, 0xD6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0xF0, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xDC, 0x76, 0x66, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0x60, 0x38, 0x0C, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x10, 0x30, 0x30, 0xFC, 0x30, 0x30, 0x30, 0x30, 0x36, 0x1C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xD6, 0xD6, 0xD6, 0xFE, 0x6C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0x6C, 0x38, 0x38, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0xC6, 0x7C, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xFE, 0xCC, 0x18, 0x30, 0x60, 0xC6, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0x0E, 0x18, 0x18, 0x18, 0x70, 0x18, 0x18, 0x18, 0x18, 0x0E, 0x00, 0x00, 0x00}, 
 { 0x00, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00}, 
 { 0x00, 0x70, 0x18, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x18, 0x18, 0x70, 0x00, 0x00, 0x00}, 
 { 0x00, 0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0xFE, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03}, 
 { 0x00, 0x00, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
 { 0x00, 0x18, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
 { 0x00, 0x00, 0xC0, 0xE1, 0xF1, 0xF9, 0xF9, 0xF9, 0xFD, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE}, 
 { 0x00, 0x00, 0x00, 0xC0, 0x20, 0xA0, 0xE0, 0xA0, 0xE0, 0x20, 0xC0, 0x00, 0x00, 0x00}, 
 { 0x07, 0x0F, 0x0F, 0x0F, 0x1F, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F, 0x7F}, 
 { 0xFB, 0xF9, 0xF1, 0xF0, 0xE0, 0xE0, 0xE0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x80, 0xC0}, 
 { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x7E, 0x3E, 0x3E, 0x3C, 0x1C, 0x1E}, 
 { 0xFF, 0xDF, 0xCF, 0x8F, 0x87, 0x87, 0x07, 0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x00}, 
 { 0x00, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF0, 0xF8}, 
 { 0x7F, 0x7F, 0x7C, 0x7C, 0x7C, 0x78, 0x78, 0x78, 0x70, 0x70, 0x70, 0x60, 0x60, 0x60}, 
 { 0xE0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF0, 0x78, 0x78, 0x78, 0x3C, 0x3C, 0x3C, 0x1E, 0x1E}, 
 { 0x1F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x03, 0x03, 0x03, 0x01, 0x01, 0x01, 0x00}, 
 { 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xF0, 0xF0}, 
 { 0x78, 0x78, 0x3C, 0x3C, 0x3C, 0x1E, 0x1E, 0x1E, 0x1F, 0x0F, 0x0F, 0x0F, 0x07, 0x07}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x7F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
 { 0x00, 0x00, 0x80, 0x80, 0x80, 0xC0, 0xC1, 0xC1, 0xE1, 0xE3, 0xE3, 0xF3, 0xF7, 0xFF}, 
 { 0x78, 0x78, 0x78, 0x7C, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
 { 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80}, 
 { 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x00}, 
 { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x18, 0x00, 0x00}, 
 { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xC0, 0x00, 0x00, 0x00}, 
 { 0x80, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x18, 0x3C, 0x24, 0x24, 0x24, 0x24, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x10, 0x10, 0x7C, 0x7C, 0x10, 0x10, 0x00, 0x7C, 0x7C, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x28, 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x3C, 0x66, 0x42, 0x81, 0x99, 0xBD, 0xA1, 0xA1, 0xBD, 0x99, 0x81, 0x42, 0x66, 0x3C}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00, 0x00, 0x00}, 
 { 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA}, 
 { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
 { 0x3C, 0x66, 0x42, 0x81, 0xB9, 0xBD, 0xA5, 0xB9, 0xBD, 0xA5, 0x81, 0x42, 0x66, 0x3C}, 
 { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF0, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x00, 0x28, 0x28, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00, 0x00, 0x00}, 
 { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xFF, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, 
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 
 { 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0}, 
 { 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F}, 
 { 0x00, 0x1E, 0x36, 0x66, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0x62, 0x62, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0x62, 0x62, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00}, 
 { 0x00, 0x1E, 0x36, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0xFF, 0xC3, 0x81, 0x00}, 
 { 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68, 0x60, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0xD6, 0xD6, 0x54, 0x54, 0x7C, 0x7C, 0x54, 0xD6, 0xD6, 0xD6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0x06, 0x06, 0x3C, 0x06, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xCE, 0xCE, 0xD6, 0xE6, 0xE6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x28, 0x10, 0xC6, 0xC6, 0xCE, 0xCE, 0xD6, 0xE6, 0xE6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xE6, 0x66, 0x6C, 0x6C, 0x78, 0x78, 0x6C, 0x6C, 0x66, 0xE6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x1E, 0x36, 0x66, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xC0, 0xC0, 0xC2, 0x66, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7E, 0x5A, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x3C, 0x18, 0x7E, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0x7E, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0x6C, 0x7C, 0x38, 0x38, 0x7C, 0x6C, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFE, 0x06, 0x06, 0x00}, 
 { 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00}, 
 { 0x00, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xFF, 0x00, 0x00, 0x00}, 
 { 0x00, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xFF, 0x03, 0x03, 0x00}, 
 { 0x00, 0xF8, 0xB0, 0x30, 0x30, 0x3C, 0x36, 0x36, 0x36, 0x36, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xC3, 0xC3, 0xC3, 0xC3, 0xF3, 0xDB, 0xDB, 0xDB, 0xDB, 0xF3, 0x00, 0x00, 0x00}, 
 { 0x00, 0xF0, 0x60, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00}, 
 { 0x00, 0x7C, 0xC6, 0x06, 0x26, 0x3E, 0x26, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0xCE, 0xDB, 0xDB, 0xDB, 0xFB, 0xDB, 0xDB, 0xDB, 0xDB, 0xCE, 0x00, 0x00, 0x00}, 
 { 0x00, 0x3F, 0x66, 0x66, 0x66, 0x3E, 0x3E, 0x66, 0x66, 0x66, 0xE7, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00}, 
 { 0x02, 0x06, 0x3C, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7E, 0x32, 0x32, 0x30, 0x30, 0x30, 0x78, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x1E, 0x36, 0x36, 0x66, 0x66, 0x66, 0xFF, 0xC3, 0xC3, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xD6, 0xD6, 0x54, 0x7C, 0x54, 0xD6, 0xD6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x06, 0x0C, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xCE, 0xD6, 0xE6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x28, 0x10, 0xC6, 0xC6, 0xCE, 0xD6, 0xE6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xE6, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0xE6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x1E, 0x36, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xD6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0xF0, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC0, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7E, 0x5A, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0xC6, 0x7C, 0x00}, 
 { 0x00, 0x00, 0x00, 0x3C, 0x18, 0x7E, 0xDB, 0xDB, 0xDB, 0xDB, 0x7E, 0x18, 0x3C, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0x6C, 0x38, 0x38, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFE, 0x06, 0x06, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x06, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xFE, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xFE, 0x03, 0x03, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xF8, 0xB0, 0x30, 0x3E, 0x33, 0x33, 0x7E, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xF6, 0xDE, 0xDE, 0xF6, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xF0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x06, 0x1E, 0x06, 0x66, 0x3C, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0xCE, 0xDB, 0xDB, 0xFB, 0xDB, 0xDB, 0xCE, 0x00, 0x00, 0x00}, 
 { 0x00, 0x00, 0x00, 0x00, 0x7E, 0xCC, 0xCC, 0xFC, 0x6C, 0xCC, 0xCE, 0x00, 0x00, 0x00}
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-����������� ������---------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CGraphics::CGraphics(void)
{ 
 VideoPtr=NULL;
 ScreenWidth=0;
 ScreenHeight=0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//-���������� ������----------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CGraphics::~CGraphics()
{
}

//-������� ������------------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�������� ������� ������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�������� ������� ������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------------------------------
//�������������
//----------------------------------------------------------------------------------------------------
void CGraphics::Init(unsigned long *video_ptr,unsigned long screen_width,unsigned long screen_height)
{
 VideoPtr=video_ptr;
 ScreenWidth=screen_width;
 ScreenHeight=screen_height;
 LineSize=screen_width;
}
//----------------------------------------------------------------------------------------------------
//����� ������� � �������
//----------------------------------------------------------------------------------------------------
void CGraphics::PutSymbol(unsigned long x,unsigned long y,char symbol,unsigned long color)
{ 
 if (VideoPtr==NULL) return;
 unsigned char s=static_cast<unsigned char>(symbol);
 if (s<=32) return;
 s-=32;
 if (s>223) return;
 //������ ������ 
 for(unsigned long ys=0;ys<FONT_HEIGHT;ys++)
 {
  unsigned char byte=Font8x14[s][ys];
  unsigned char mask=128;
  for(unsigned long xs=0;xs<FONT_WIDTH;xs++,mask>>=1)
  {   
   if (x+xs<0 || x+xs>=ScreenWidth) continue;
   if (y+ys<0 || y+ys>=ScreenHeight) continue;
   unsigned long *v_ptr=VideoPtr+x+xs+(y+ys)*LineSize;
   if (byte&mask) *v_ptr=color;
  }
 }

}
//----------------------------------------------------------------------------------------------------
//����� ������� � ������� � ��������� �����
//----------------------------------------------------------------------------------------------------
void CGraphics::PutSymbolInversion(unsigned long x,unsigned long y,char symbol)
{ 
 if (VideoPtr==NULL) return;
 unsigned char s=static_cast<unsigned char>(symbol);
 if (s<=32) return;
 s-=32;
 if (s>223) return;
 //������ ������ 
 for(unsigned long ys=0;ys<FONT_HEIGHT;ys++)
 {
  unsigned char byte=Font8x14[s][ys];
  unsigned char mask=128;
  for(unsigned long xs=0;xs<FONT_WIDTH;xs++,mask>>=1)
  {   
   if (x+xs<0 || x+xs>=ScreenWidth) continue;
   if (y+ys<0 || y+ys>=ScreenHeight) continue;
   unsigned long *v_ptr=VideoPtr+x+xs+(y+ys)*LineSize;
   if (byte&mask) (*v_ptr)^=0x00FFFFFF;
  }
 }
}

//----------------------------------------------------------------------------------------------------
//����� ������� � �������
//----------------------------------------------------------------------------------------------------
void CGraphics::PutString(unsigned long x,unsigned long y,const char *string,unsigned long color)
{
 unsigned char l=strlen(string);
 for(unsigned long n=0;n<l;n++,x+=FONT_WIDTH) PutSymbol(x,y,string[n],color);
}
//----------------------------------------------------------------------------------------------------
//����� ������� � ������� � ��������� �����
//----------------------------------------------------------------------------------------------------
void CGraphics::PutStringInversion(unsigned long x,unsigned long y,const char *string)
{
 unsigned char l=strlen(string);
 for(unsigned long n=0;n<l;n++,x+=FONT_WIDTH) PutSymbolInversion(x,y,string[n]);
}

//----------------------------------------------------------------------------------------------------
//��������� �������������
//----------------------------------------------------------------------------------------------------
void CGraphics::SolidFill(unsigned long x,unsigned long y,unsigned long width,unsigned long height,unsigned long color)
{ 
 if (VideoPtr==NULL) return;
 for(unsigned long h=0;h<height;h++)
 {
  for(unsigned long w=0;w<width;w++)
  {
   if (x+w<0 || x+w>=ScreenWidth) continue;
   if (y+h<0 || y+h>=ScreenHeight) continue;
   unsigned long *v_ptr=VideoPtr+x+w+(y+h)*LineSize;
   *v_ptr=color;
  }
 }
}
//----------------------------------------------------------------------------------------------------
//��������� ����� � ������� � ��������� �����
//----------------------------------------------------------------------------------------------------
void CGraphics::DrawPointInversion(unsigned long x,unsigned long y)
{
 if (VideoPtr==NULL) return;
 if (x<0 || x>=ScreenWidth) return;
 if (y<0 || y>=ScreenHeight) return;
 unsigned long *v_ptr=VideoPtr+x+y*LineSize;
 (*v_ptr)^=0x00FFFFFF;
}