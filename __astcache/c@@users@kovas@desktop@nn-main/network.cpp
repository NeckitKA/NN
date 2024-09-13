﻿//---------------------------------------------------------------------------
#pragma hdrstop
#include "Network.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
Network::Network(int n0, int n1, int n2, double learningRate) {
	SetTheNumberOfNeuronsForEachLayer(n0, n1, n2);
	GenerateWeights();
	SetLearningRate(learningRate);

}
//---------------------------------------------------------------------------
void Network::SetTheNumberOfNeuronsForEachLayer(int n0, int n1, int n2){
	N0 = n0;
	N1 = n1;
	N2 = n2;
	std::vector<double> firstLayerActivations(N0, 0.0);
	std::vector<double> secondLayerActivations(N1, 0.0);
	std::vector<double> outputLayerActivations(N2, 0.0);
}
//---------------------------------------------------------------------------
//std::mt19937 g(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
void Network::GenerateWeights() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-1.0, 1.0);
	std::vector<std::vector<double>> firstHiddenLayer;
	for (int i = 0; i < N0; ++i) {
		std::vector<double> neuron;
		for (int j = 0; j < 3; ++j) {
			neuron.push_back(dis(gen));
		}
		firstHiddenLayer.push_back(neuron);
	}
	std::vector<std::vector<double>> secondHiddenLayer;
	for (int i = 0; i < N1; ++i) {
		std::vector<double> neuron;
		for (int j = 0; j < N0+1; ++j) {
			neuron.push_back(dis(gen));
		}
		secondHiddenLayer.push_back(neuron);
	}
	std::vector<std::vector<double>> outputLayer;
	for (int i = 0; i < N2; ++i) {
		std::vector<double> neuron;
		for (int j = 0; j < N1+1; ++j) {
			neuron.push_back(dis(gen));
		}
		outputLayer.push_back(neuron);
	}
	network.push_back(firstHiddenLayer);
	network.push_back(secondHiddenLayer);
	network.push_back(outputLayer);
}
//---------------------------------------------------------------------------
std::vector<std::vector<double>> Network::ActivateAndForwardPropagate(int x0, int x1) {
	std::vector<double> firstLayerActivations(N0, 0.0);
	std::vector<double> secondLayerActivations(N1, 0.0);
	std::vector<double> outputLayerActivations(N2, 0.0);
	for (int i = 0; i < N0; ++i) {
		firstLayerActivations[i]+=network[0][i][0]*x0+network[0][i][1]*x1+
										network[0][i][2];
	}
	for (int i = 0; i < N0; ++i) {
		firstLayerActivations[i]=Sigmoid(firstLayerActivations[i]);
	}
	for (int i = 0; i < N1; ++i) {
		for (int j = 0; j < N1; ++j) {
			secondLayerActivations[i]+=network[1][i][j]*firstLayerActivations[j];
		}
		secondLayerActivations[i]+=network[1][i][N0];
	}
	for (int i = 0; i < N1; ++i) {
		secondLayerActivations[i]=Sigmoid(secondLayerActivations[i]);
	}
	for (int i = 0; i < N2; ++i) {
		for (int j = 0; j < N1; ++j) {
			outputLayerActivations[i]+=network[2][i][j]*secondLayerActivations[j];
		}
		outputLayerActivations[i]+=network[2][i][N1];
	}
	for (int i = 0; i < N2; ++i) {
		outputLayerActivations[i]=Sigmoid(outputLayerActivations[i]);
	}
	std::vector<std::vector<double>> neuronsActivations;
	neuronsActivations.push_back(firstLayerActivations);
	neuronsActivations.push_back(secondLayerActivations);
	neuronsActivations.push_back(outputLayerActivations);
	return neuronsActivations;
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
void Network::SetLearningRate(double lRate) {
	learningRate = lRate;
}
//---------------------------------------------------------------------------
void Network::BackwardPropagateErrorAndUpdateWeights(int expected, int x0, int x1) {
	std::vector<std::vector<double>> neuronsActivations = ActivateAndForwardPropagate(x0,x1);
	std::vector<double> outputLayerErrors;
	for (int i = 0; i < N2; ++i) {
		outputLayerErrors.push_back((neuronsActivations[2][i]-expected)*SigmoidDerivative(neuronsActivations[2][i]));
	}
	std::vector<double> secondLayerErrors(N1, 0.0);
	for (int i = 0; i < N1; ++i) {
		for (int j = 0; j < N2; ++j) {
			secondLayerErrors[i]+=network[2][j][i]*outputLayerErrors[j];
		}
	}
	for (int i = 0; i < N2; ++i) {
		secondLayerErrors[i]*=SigmoidDerivative(neuronsActivations[1][i]);
	}
	std::vector<double> firstLayerErrors(N0, 0.0);
	for (int i = 0; i < N0; ++i) {
		for (int j = 0; j < N1; ++j) {
			firstLayerErrors[i]+=network[1][j][i]*secondLayerErrors[j];
		}
	}
	for (int i = 0; i < N1; ++i) {
		firstLayerErrors[i]*=SigmoidDerivative(neuronsActivations[0][i]);
	}
	for (int i = 0; i < N0; ++i) {
		for (int j = 0; j < 3; ++j) {
			switch (j) {
				case 0:
					network[0][i][j] -= learningRate*firstLayerErrors[i]*x0;
					break;
				case 1:
					network[0][i][j] -= learningRate*firstLayerErrors[i]*x1;
					break;
			default:
				network[0][i][j] -= learningRate*firstLayerErrors[i];
			}
		}
	}
	for (int i = 0; i < N1; ++i) {
		for (int j = 0; j < N0+1; ++j) {
			if (j<N0) {
				network[1][i][j]-=learningRate*secondLayerErrors[i]*network[0][j][i];
			}
			else {
				network[1][i][j]-=learningRate*secondLayerErrors[i];
			}
		}
	}
	for (int i = 0; i < N2; ++i) {
		for (int j = 0; j < N1+1; ++j) {
			if (j<N1) {
				network[2][i][j]-=learningRate*outputLayerErrors[i]*network[1][j][i];
			}
			else {
				network[2][i][j]-=learningRate*outputLayerErrors[i];
			}
		}
	}
}
//---------------------------------------------------------------------------
void Network::NetworkTraining(int x0, int x1, int excepted) {
	ActivateAndForwardPropagate(x0,x1);
	BackwardPropagateErrorAndUpdateWeights(excepted,x0,x1);
}
//---------------------------------------------------------------------------
