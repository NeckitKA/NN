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

void TForm1::Clear() {
	for (int i = 0; i < trainingSet.size(); ++i) {
		delete trainingSet[i];
	}
	trainingSet.clear();
	for (int i = 0; i < testSet.size(); ++i) {
		delete testSet[i];
	}
	testSet.clear();
	delete network;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	Clear();
}
//---------------------------------------------------------------------------

void TForm1::GenerateDataSet(int nClassObj1, int nClassObj2, int centerX,
  int centerY, int circleRadius, int ringInnerRadius, int ringOuterRadius,
  bool shuffle, bool learning) {

	int x, y, distanceSquared;
	int countClassObj1 = 0, countClassObj2 = 0;
	std::mt19937 gen(0);
	std::uniform_int_distribution<> distCircleX(centerX-circleRadius,
												centerX+circleRadius);
	std::uniform_int_distribution<> distCircleY(centerY-circleRadius,
												centerY+circleRadius);
	std::uniform_int_distribution<> distRingX(centerX-ringOuterRadius,
											  centerX+ringOuterRadius);
	std::uniform_int_distribution<> distRingY(centerY-ringOuterRadius,
											  centerY+ringOuterRadius);
	if (learning) {
		if (trainingSet.empty()) {
			while (countClassObj1 != nClassObj1) {
				x = distCircleX(gen);
				y = distCircleY(gen);
				if (pow((x-centerX),2)+pow((y-centerY),2)<=pow(circleRadius,2)){
					countClassObj1++;
					trainingSet.push_back(new MyPoint(x, y, 1));
				}
			}
			while (countClassObj2 != nClassObj2) {
				x = distRingX(gen);
				y = distRingY(gen);
				distanceSquared = pow((x - centerX), 2) + pow((y - centerY), 2);
				if (pow(ringInnerRadius, 2 )<= distanceSquared &&
					distanceSquared <= pow(ringOuterRadius, 2)) {
					countClassObj2++;
					trainingSet.push_back(new MyPoint(x, y, 0));
				}
			}

			if (shuffle) {
				std::shuffle(trainingSet.begin(), trainingSet.end(), gen);
			}
		}
	}
	else {
		if (testSet.empty()) {
            while (countClassObj1 != nClassObj1) {
				x = distCircleX(gen);
				y = distCircleY(gen);
				if (pow((x-centerX),2)+pow((y-centerY),2)<=pow(circleRadius,2)){
					countClassObj1++;
					testSet.push_back(new MyPoint(x, y, 1));
				}
			}

			while (countClassObj2 != nClassObj2) {
				x = distRingX(gen);
				y = distRingY(gen);
				distanceSquared = pow((x - centerX), 2) + pow((y - centerY), 2);
				if (pow(ringInnerRadius, 2) <= distanceSquared &&
					distanceSquared <= pow(ringOuterRadius, 2)) {
					countClassObj2++;
					testSet.push_back(new MyPoint(x, y, 0));
				}
			}
			if (shuffle) {
				std::shuffle(testSet.begin(), testSet.end(), gen);
			}
		}
	}
}

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	if (trainingSet.empty()) {
		GenerateDataSet(3000,3000,cX,cY, 30,95,100,true,true);
		Timer1->Enabled=false;
	}
	if (testSet.empty()) {
		GenerateDataSet(3000,3000,cX,cY, 30,95,100,false,false);
	}

	if (epoch < 80 && learning) {
		std::random_shuffle(trainingSet.begin(), trainingSet.end());
		double accuracy=0;
		double loss=0;
		for (int j = 0; j < trainingSet.size(); ++j) {
			std::vector<double> inputData;
			inputData.push_back(trainingSet[j]->GetX() - cX);
			inputData.push_back(trainingSet[j]->GetY() - cY);
			inputData.push_back((trainingSet[j]->GetX() - cX)*
				(trainingSet[j]->GetX() - cX));
			inputData.push_back((trainingSet[j]->GetY() - cY)*
				(trainingSet[j]->GetY() - cY));
			inputData.push_back((trainingSet[j]->GetX() - cX)*
				(trainingSet[j]->GetY() - cY));
			inputData.push_back(std::sin(trainingSet[j]->GetX() - cX));
			inputData.push_back(std::sin(trainingSet[j]->GetY() - cY));
			double output = network->NetworkTraining(inputData,
				trainingSet[j]->GetColor());
			inputData.clear();
			accuracy+=((trainingSet[j]->GetColor()==1 && output>0.5) ||
					   (trainingSet[j]->GetColor()==0 && output < 0.5));
			loss+=abs(trainingSet[j]->GetColor()-output);
		}
		accuracy = accuracy / trainingSet.size();
		loss = loss/trainingSet.size();
		Series1->AddXY(epoch, accuracy, "",clBlue);
		Series3->AddXY(epoch, loss, "", clBlue);

		accuracy=0;
		loss=0;
        Canvas->Brush->Color = clGreen;
		Canvas->FillRect(ClientRect);
		for (int j = 0; j < testSet.size(); ++j) {
			std::vector<double> inputData;
			inputData.push_back(testSet[j]->GetX() - cX);
			inputData.push_back(testSet[j]->GetY() - cY);
			inputData.push_back((testSet[j]->GetX() - cX)*
				(testSet[j]->GetX() - cX));
			inputData.push_back((testSet[j]->GetY() - cY)*
				(testSet[j]->GetY() - cY));
			inputData.push_back((testSet[j]->GetX() - cX)*
				(testSet[j]->GetY() - cY));
			inputData.push_back(std::sin(testSet[j]->GetX() - cX));
			inputData.push_back(std::sin(testSet[j]->GetY() - cY));
			double output = network->Predict(inputData);
			inputData.clear();

			accuracy += ((testSet[j]->GetColor()==1 && output > 0.5) ||
						 (testSet[j]->GetColor()==0 && output < 0.5));
			loss+=abs(testSet[j]->GetColor()-output);
		}
		accuracy = accuracy / testSet.size();
		loss = loss/testSet.size();
		Series2->AddXY(epoch, accuracy, "",clRed);
		Series4->AddXY(epoch, loss, "", clRed);
		epoch++;
	}
	else {
		Timer1->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	if (trainingSet.empty()) {
		GenerateDataSet(3000,3000,cX,cY, 30,95,100,true,true);
	}
	learning=true;
	Timer1->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    Timer1->Enabled=true;
}
//---------------------------------------------------------------------------
