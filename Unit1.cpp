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
	for (int i = 0; i < testSet.size(); ++i) {
		delete testSet[i];
	}
	testSet.clear();
	for (int i = 0; i < resultSet.size(); ++i) {
		delete resultSet[i];
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


/*

    */
/*
if (testSet.empty()) {
		GenerateDataSet(1000,1000,cX,cY, 30,95,100,false,true);
	}
	Timer1->Enabled=true;
*/

void TForm1::SaveResultsToFile(double o, int i) {
	std::ofstream file("logs/results.txt",ios::app);
	file << i << ":" << std::fixed << std::setprecision(3) << o << "\n";
	file.close();
}

void TForm1::GenerateDataSet(int nClassObj1, int nClassObj2, int centerX,
  int centerY, int circleRadius, int ringInnerRadius, int ringOuterRadius, bool shuffle, bool learning) {

	int x, y, distanceSquared;
	int countClassObj1 = 0, countClassObj2 = 0;
	//std::random_device rd;
	//std::mt19937 gen(rd());
	std::mt19937 gen(0);
	std::uniform_int_distribution<> distCircleX(centerX-circleRadius, centerX+circleRadius);
	std::uniform_int_distribution<> distCircleY(centerY-circleRadius, centerY+circleRadius);
	std::uniform_int_distribution<> distRingX(centerX-ringOuterRadius, centerX+ringOuterRadius);
	std::uniform_int_distribution<> distRingY(centerY-ringOuterRadius, centerY+ringOuterRadius);

	if (learning) {
		if (testSet.empty()) {


			while (countClassObj1 != nClassObj1) {
				x = distCircleX(gen);
				y = distCircleY(gen);
				if (pow((x - centerX), 2) + pow((y - centerY), 2) <= pow(circleRadius, 2)) {
					countClassObj1++;
					testSet.push_back(new MyPoint(x, y, 1));
				}
			}
            while (countClassObj2 != nClassObj2) {
				x = distRingX(gen);
				y = distRingY(gen);
				distanceSquared = pow((x - centerX), 2) + pow((y - centerY), 2);
				if (pow(ringInnerRadius, 2) <= distanceSquared && distanceSquared <= pow(ringOuterRadius, 2)) {
					countClassObj2++;
					testSet.push_back(new MyPoint(x, y, 0));
				}
			}


			if (shuffle) {
				std::shuffle(testSet.begin(), testSet.end(), gen);
			}
		}
	}
	else {
		if (resultSet.empty()) {
            while (countClassObj1 != nClassObj1) {
				x = distCircleX(gen);
				y = distCircleY(gen);
				if (pow((x - centerX), 2) + pow((y - centerY), 2) <= pow(circleRadius, 2)) {
					countClassObj1++;
					resultSet.push_back(new MyPoint(x, y, 1));
				}
			}

			while (countClassObj2 != nClassObj2) {
				x = distRingX(gen);
				y = distRingY(gen);
				distanceSquared = pow((x - centerX), 2) + pow((y - centerY), 2);
				if (pow(ringInnerRadius, 2) <= distanceSquared && distanceSquared <= pow(ringOuterRadius, 2)) {
					countClassObj2++;
					resultSet.push_back(new MyPoint(x, y, 0));
				}
			}
            if (shuffle) {
				std::shuffle(resultSet.begin(), resultSet.end(), gen);
			}
		}
	}
}


void __fastcall TForm1::Timer1Timer(TObject *Sender)
{

	switch (i) {
		case 1:
			std::random_shuffle(testSet.begin(), testSet.end());
			static int epochTestSet = 0;
			if (epochTestSet < 80) {
				Canvas->Brush->Color = clGreen;
				Canvas->FillRect(ClientRect);
				int accuracy=0;
				for (int j = 0; j < testSet.size(); ++j) {
					std::vector<double> inputData;
					inputData.push_back(testSet[j]->GetX() - cX);
					inputData.push_back(testSet[j]->GetY() - cY);
					inputData.push_back((testSet[j]->GetX() - cX) * (testSet[j]->GetX() - cX));
					inputData.push_back((testSet[j]->GetY() - cY) * (testSet[j]->GetY() - cY));
					inputData.push_back((testSet[j]->GetX() - cX) * (testSet[j]->GetY() - cY));
					inputData.push_back(std::sin(testSet[j]->GetX() - cX));
					inputData.push_back(std::sin(testSet[j]->GetY() - cY));
					double output = network->NetworkTraining(inputData, testSet[j]->GetColor());
					inputData.clear();
					SaveResultsToFile(output, j);
					accuracy += ((testSet[j]->GetColor() == 1 && output > 0.5) ||
								 (testSet[j]->GetColor() == 0 && output < 0.5));
				}

				Series1->AddXY(epochTestSet, accuracy, "", clBlue);
				epochTestSet++;
			}
			else {
				Timer1->Enabled = false;
			}
			break;
		case 2:

            epochTestSet = 0;
			if (epochTestSet < 1) {
				Canvas->Brush->Color = clGreen;
				Canvas->FillRect(ClientRect);
				int accuracy=0;
				for (int j = 0; j < resultSet.size(); ++j) {
					std::vector<double> inputData;
					inputData.push_back(resultSet[j]->GetX() - cX);
					inputData.push_back(resultSet[j]->GetY() - cY);
					inputData.push_back((resultSet[j]->GetX() - cX) * (resultSet[j]->GetX() - cX));
					inputData.push_back((resultSet[j]->GetY() - cY) * (resultSet[j]->GetY() - cY));
					inputData.push_back((testSet[j]->GetX() - cX) * (testSet[j]->GetY() - cY));
					inputData.push_back(std::sin(testSet[j]->GetX() - cX));
					inputData.push_back(std::sin(testSet[j]->GetY() - cY));
					double output = network->Predict(inputData);
					inputData.clear();
					SaveResultsToFile(output, j);
					accuracy += ((resultSet[j]->GetColor() == 1 && output > 0.5) ||
								 (resultSet[j]->GetColor() == 0 && output < 0.5));
				}
				Label1->Caption=IntToStr(accuracy);
				epochTestSet++;
				//Series2->AddXY(1, accuracy, "", clRed);stSet++;
			}

			else {
				Timer1->Enabled = false;
			}
            Timer1->Enabled = false;
			break;

	default:

		break;
	}




}

//---------------------------------------------------------------------------

/*
else if (flag==1) {
		Canvas->Brush->Color = clGreen;
		Canvas->FillRect(ClientRect);
		int accuracy=0;
		for (int j = 0; j < resultSet.size(); ++j) {
			std::vector<double> inputData;
			inputData.push_back(resultSet[j]->GetX() - cX);
			inputData.push_back(resultSet[j]->GetY() - cY);
			inputData.push_back((resultSet[j]->GetX() - cX) * (resultSet[j]->GetX() - cX));
			inputData.push_back((resultSet[j]->GetY() - cY) * (resultSet[j]->GetY() - cY));
			double output = network->Predict(inputData);
			inputData.clear();
			SaveResultsToFile(output, j);
			accuracy += ((resultSet[j]->GetColor() == 1 && output > 0.5) ||
						 (resultSet[j]->GetColor() == 0 && output < 0.5));
		}
		Series2->AddXY(epochTestSet, accuracy, "", clRed);

	}
*/

/*
int cX = 200, cY = 200;
	for (int i = 0; i < 1; ++i) {
		Canvas->Brush->Color=clGreen;
		Canvas->FillRect(ClientRect);
		for (int j = 0; j < testSet.size(); ++j) {
			std::vector<double> inputData;
			inputData.push_back(testSet[j]->GetX()-cX);
			inputData.push_back(testSet[j]->GetY()-cY);
			inputData.push_back((testSet[j]->GetX()-cX)*(testSet[j]->GetX()-cX));
			inputData.push_back((testSet[j]->GetY()-cY)*(testSet[j]->GetY()-cY));
			double o = network->NetworkTraining(inputData,testSet[j]->GetColor());
			inputData.clear();
			SaveResultsToFile(o,j);
		}
	}
	if (testSet.empty()) {
		GenerateDataSet(1000,1000,cX,cY, 30,95,100,false,true);
	}
*/

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	if (testSet.empty()) {
		GenerateDataSet(3000,3000,cX,cY, 30,95,100,true,true);
	}
	i=1;
	Timer1->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	if (resultSet.empty()) {
		GenerateDataSet(500,500,cX,cY, 30,95,100,false,false);
	}
    i=2;
	Timer1->Enabled=true;
}
//---------------------------------------------------------------------------


