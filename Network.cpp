//---------------------------------------------------------------------------
#pragma hdrstop
#include "Network.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

Network::Network(int n0, int n1, int n2, int n3, double learningRate) {
	SetNumOfNeuronsForLayers(n0, n1, n2, n3);
	GenerateWeights();
	SetLearningRate(learningRate);
}
//---------------------------------------------------------------------------

void Network::SetNumOfNeuronsForLayers(int n0, int n1, int n2, int n3){
	N0 = n0;
	N1 = n1;
	N2 = n2;
	N3 = n3;
}
//---------------------------------------------------------------------------

void Network::SetLearningRate(double lRate) {
	learningRate = lRate;
}
//---------------------------------------------------------------------------

void Network::GenerateWeights() {
	std::mt19937 gen(0);
	std::uniform_real_distribution<> dis(-1.0, 1.0);
	std::vector<std::vector<double>> firstHiddenLayer;
	std::vector<std::vector<double>> secondHiddenLayer;
	std::vector<std::vector<double>> outputLayer;

	for (int i = 0; i < N1; ++i) {
		std::vector<double> neuron;
		for (int j = 0; j < N0+1; ++j) {
			neuron.push_back(dis(gen));
		}
		firstHiddenLayer.push_back(neuron);
	}

	for (int i = 0; i < N2; ++i) {
		std::vector<double> neuron;
		for (int j = 0; j < N1 + 1; ++j) {
			neuron.push_back(dis(gen));
		}
		secondHiddenLayer.push_back(neuron);
	}

	for (int i = 0; i < N3; ++i) {
		std::vector<double> neuron;
		for (int j = 0; j < N2 + 1; ++j) {
			neuron.push_back(dis(gen));
        }
		outputLayer.push_back(neuron);
    }

	network.push_back(firstHiddenLayer);
	network.push_back(secondHiddenLayer);
	network.push_back(outputLayer);
}
//---------------------------------------------------------------------------

double Network::Sigmoid(double activation) const {
	return 1/(1+ std::exp(-activation));
}
//---------------------------------------------------------------------------
double Network::SigmoidDerivative(double output) const {
	return output * (1 - output);
}
//---------------------------------------------------------------------------

void Network::ForwardPropagate(std::vector<double> inputData) {
	firstLayerActivations.assign(N1, 0.0);
	secondLayerActivations.assign(N2, 0.0);
	outputLayerActivations.assign(N3, 0.0);

	for (int i = 0; i < N1; ++i) {
		for (int j = 0; j < N0; ++j) {
			firstLayerActivations[i]+=network[0][i][j]*inputData[j];
		}
		firstLayerActivations[i]+=network[0][i][N0];
	}
	Activate(1);

	for (int i = 0; i < N2; ++i) {
		for (int j = 0; j < N1; ++j) {
			secondLayerActivations[i]+=network[1][i][j]*
			  firstLayerActivations[j];
		}
		secondLayerActivations[i]+=network[1][i][N1];
	}
	Activate(2);

	for (int i = 0; i < N3; ++i) {
		for (int j = 0; j < N2; ++j) {
			outputLayerActivations[i]+=network[2][i][j]*
			  secondLayerActivations[j];
		}
		outputLayerActivations[i]+=network[2][i][N2];
	}
	Activate(3);
}
//---------------------------------------------------------------------------

void Network::Activate(int layer) {

	switch (layer) {

	case 1:
		for (int i = 0; i < N1; ++i) {
			firstLayerActivations[i]=Sigmoid(firstLayerActivations[i]);
		}
		break;

	case 2:
		for (int i = 0; i < N2; ++i) {
			secondLayerActivations[i]=Sigmoid(secondLayerActivations[i]);
		}
		break;

	case 3:
		for (int i = 0; i < N3; ++i) {
			outputLayerActivations[i]=Sigmoid(outputLayerActivations[i]);

		}
		break;

	default:
		break;
	}
}
//---------------------------------------------------------------------------

void Network::BackwardPropagateError(int expected) {
	outputLayerErrors.clear();
	secondLayerErrors.assign(N2, 0.0);
	firstLayerErrors.assign(N1, 0.0);

	for (int i = 0; i < N3; ++i) {
		outputLayerErrors.push_back((outputLayerActivations[i]-expected)*
		  SigmoidDerivative(outputLayerActivations[i]));
	}

	for (int i = 0; i < N2; ++i) {
		for (int j = 0; j < N3; ++j) {
			secondLayerErrors[i]+=network[2][j][i]*outputLayerErrors[j];
		}
		secondLayerErrors[i]*=SigmoidDerivative(secondLayerActivations[i]);
	}

	for (int i = 0; i < N1; ++i) {
		for (int j = 0; j < N2; ++j) {
			firstLayerErrors[i]+=network[1][j][i]*secondLayerErrors[j];
		}
        firstLayerErrors[i]*=SigmoidDerivative(firstLayerActivations[i]);
	}
}
//---------------------------------------------------------------------------

void Network::UpdateWeights(std::vector<double> inputData) {
	for (int i = 0; i < N1; ++i) {
		for (int j = 0; j < N0; ++j) {
			network[0][i][j] -= learningRate*firstLayerErrors[i]*inputData[j];
		}
		network[0][i][N0] -= learningRate*firstLayerErrors[i];
	}

	for (int i = 0; i < N2; ++i) {
		for (int j = 0; j < N1; ++j) {
		  network[1][i][j]-=learningRate*secondLayerErrors[i]*network[0][j][i];
		}
		network[1][i][N1]-=learningRate*secondLayerErrors[i];
	}

	for (int i = 0; i < N3; ++i) {
		for (int j = 0; j < N2; ++j) {
		  network[2][i][j]-=learningRate*outputLayerErrors[i]*network[1][j][i];
		}
		network[2][i][N2]-=learningRate*outputLayerErrors[i];
	}
}
//---------------------------------------------------------------------------

double Network::NetworkTraining(std::vector<double> inputData, int excepted) {
	ForwardPropagate(inputData);
	BackwardPropagateError(excepted);
	UpdateWeights(inputData);
	return outputLayerActivations[0];
}
//---------------------------------------------------------------------------

double Network::Predict(const std::vector<double> inputData) {
	ForwardPropagate(inputData);
	Visualize();
	return outputLayerActivations[0];
}
//---------------------------------------------------------------------------

void Network::Visualize() {
	TForm1* form = dynamic_cast<TForm1*>(Application->MainForm);
	TCanvas* canvas = form->Canvas;
	form->Canvas->Pen->Width=0;
	double confidence =  8 * pow(2*abs(outputLayerActivations[0]-0.5),2);
	canvas->Brush->Color= outputLayerActivations[0]>0.5 ? clBlue : clRed;
	int x = form->testSet[objectNum]->GetX();
	int y = form->testSet[objectNum]->GetY();
	canvas->Ellipse(x-confidence,y-confidence,x+confidence,y+confidence);
	objectNum++;
	if (objectNum==form->testSet.size()) {
		objectNum=0;
	}
}
//---------------------------------------------------------------------------
