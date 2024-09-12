//---------------------------------------------------------------------------

#ifndef NetworkH
#define NetworkH
//---------------------------------------------------------------------------
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
//---------------------------------------------------------------------------

class Network{
	private:
		int N0, N1, N2;
		std::vector<double> firstLayerActivations;
		std::vector<double> secondLayerActivations;
		std::vector<double> outputLayerActivations;
		std::vector<std::vector<std::vector<double>>> network;
		void SetTheNumberOfNeuronsForEachLayer(int n0, int n1, int n2);

		void BackwardPropagateErrorAndUpdateWeights(double expected, double learningRate, int x0, int x1);
		void GenerateWeights();
		std::vector<std::vector<double>>ActivateAndForwardPropagate(int x0, int x1);
		double Sigmoid(double activation) const;
		double SigmoidDerivative(double output) const;
        void NetworkTraining();

	public:
		Network(int n0, int n1, int n2);

};
//---------------------------------------------------------------------------
#endif
