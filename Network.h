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
		double learningRate;
		std::vector<double> firstLayerActivations;
		std::vector<double> secondLayerActivations;
		std::vector<double> outputLayerActivations;
		std::vector<std::vector<std::vector<double>>> network;
		//std::vector<double> output;


		double Sigmoid(double activation) const;
		double SigmoidDerivative(double output) const;

		std::vector<std::vector<double>>ActivateAndForwardPropagate(int x0, int x1);

		void GenerateWeights();
		void SetLearningRate(double lRate);
		//void SetOutput();
		void SetTheNumberOfNeuronsForEachLayer(int n0, int n1, int n2);
		void BackwardPropagateErrorAndUpdateWeights(int expected, int x0, int x1);

	public:
		Network(int n0, int n1, int n2, double learningRate);

		void NetworkTraining(int x0, int x1, int excepted);

};
//---------------------------------------------------------------------------
#endif
