//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeeFunci.hpp>
#include <vector>
#include <MyPoint.h>
#include <Network.h>
#include <algorithm>
#include <random>
#include <math.h>
#include <iomanip>
#include <fstream>
#include <string>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components

	TTimer *Timer1;
	TButton *Button1;
	TChart *Chart1;
	TFastLineSeries *Series1;
	TCustomTeeFunction *TeeFunction1;
	TFastLineSeries *Series2;
	TCustomTeeFunction *TeeFunction2;
	TChart *Chart2;
	TFastLineSeries *Series3;
	TCustomTeeFunction *TeeFunction3;
	TFastLineSeries *Series4;
	TCustomTeeFunction *TeeFunction4;

	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);

private:	// User declarations
	Network* network  = new Network(2,30,30,1,0.01);
	int cX = 200, cY = 200;
	int epoch = 0;
    bool learning = false;
	void Clear();
	void GenerateDataSet(int nClassObj1, int nClassObj2, int centerX,
						 int centerY, int circleRadius,  int ringInnerRadius,
						 int ringOuterRadius, bool shuffle, bool learning);

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	std::vector<MyPoint*> trainingSet;
	std::vector<MyPoint*> testSet;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
