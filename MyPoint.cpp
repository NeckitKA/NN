//---------------------------------------------------------------------------

#pragma hdrstop

#include "MyPoint.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

MyPoint::MyPoint(int x, int y, bool color) {
	SetX(x);
	SetY(y);
	SetColor(color);
    ShowPoint();
}
//---------------------------------------------------------------------------

void MyPoint::SetX(int x) {
	X = x;
}
//---------------------------------------------------------------------------

void MyPoint::SetY(int y) {
	Y = y;
}
//---------------------------------------------------------------------------

void MyPoint::SetColor(bool color) {
	Color = color;
}
//---------------------------------------------------------------------------

int MyPoint::GetX() const {
	return X;
}
//---------------------------------------------------------------------------

int MyPoint::GetY() const {
	return Y;
}
//---------------------------------------------------------------------------

bool MyPoint::GetColor() const {
	return Color;
}
//---------------------------------------------------------------------------

void MyPoint::ShowPoint(){
	TForm1* form = dynamic_cast<TForm1*>(Application->MainForm);
	form->Canvas->Pen->Color=clWhite;
	form->Canvas->Pen->Width=2;
	form->Canvas->Brush->Color = GetColor() ? clBlue : clRed;
	form->Canvas->Ellipse(GetX()-6,GetY()-6,GetX()+6,GetY()+6);
}
//---------------------------------------------------------------------------

