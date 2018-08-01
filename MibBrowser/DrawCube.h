#ifndef _Draw_CUBE_H
#define _Draw_CUBE_H

void DrawColorCube(CDC* pDC, CRect& rect, COLORREF colorFrom, COLORREF colorTo = RGB(255,255,255), BOOL IsHori = true);
void PaintGradiantRect(CDC *pDC, const RECT &rect,COLORREF clrFrom, COLORREF clrTo =RGB(255,255,255), 
                       BOOL hori = true, BOOL ascend = true);
#endif