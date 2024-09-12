//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TForm1::ShowPoints() {
	for (size_t i = 0; i < coordinatesOfTestPoints.size(); ++i) {
		if (i <= coordinatesOfTestPoints.size() / 2) {
			testPoints.push_back(new MyPoint(coordinatesOfTestPoints[i][0],
										 coordinatesOfTestPoints[i][1], 1));
		}

		else {
			testPoints.push_back(new MyPoint(coordinatesOfTestPoints[i][0],
										 coordinatesOfTestPoints[i][1], 0));
		}
	}
}
//---------------------------------------------------------------------------


void TForm1::Clear() {
	for (size_t i = 0; i < testPoints.size(); ++i) {
		delete testPoints[i];
	}
	testPoints.clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
    Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	if (testPoints.empty()) {
			ShowPoints();
		}
}
//---------------------------------------------------------------------------

