//---------------------------------------------------------------------------

#pragma hdrstop

#include "MyPoint.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

MyPoint::MyPoint(double x, double y, int color) {
	SetX(x);
	SetY(y);
	SetColor(color);
    ShowPoint();
}
//---------------------------------------------------------------------------

void MyPoint::SetX(double x) {
	X = x;
}
//---------------------------------------------------------------------------

void MyPoint::SetY(double y) {
	Y = y;
}
//---------------------------------------------------------------------------

void MyPoint::SetColor(int color) {
	Color = color;
}
//---------------------------------------------------------------------------

double MyPoint::GetX() const {
	return X;
}
//---------------------------------------------------------------------------

double MyPoint::GetY() const {
	return Y;
}
//---------------------------------------------------------------------------

int MyPoint::GetColor() const {
	return Color;
}
//---------------------------------------------------------------------------

void MyPoint::ShowPoint(){
	TForm1* form = dynamic_cast<TForm1*>(Application->MainForm);
	form->Canvas->Pen->Color=clWhite;
	form->Canvas->Pen->Width=2;
	form->Canvas->Brush->Color = GetColor()==1 ? clBlue :  clRed;
	form->Canvas->Ellipse(GetX()-6,GetY()-6,GetX()+6,GetY()+6);
}
//---------------------------------------------------------------------------

