//---------------------------------------------------------------------------

#ifndef NetworkH
#define NetworkH
//---------------------------------------------------------------------------
#include <vector>
#include <random>
#include <cmath>
//---------------------------------------------------------------------------

class Network{
	private:
		int objectNum = 0;
		int N0, N1, N2, N3;
		double learningRate;

		std::vector<double> firstLayerActivations;
		std::vector<double> secondLayerActivations;
		std::vector<double> outputLayerActivations;

		std::vector<double> outputLayerErrors;
		std::vector<double> secondLayerErrors;
		std::vector<double> firstLayerErrors;

		std::vector<std::vector<std::vector<double>>> network;

		double Sigmoid(double activation) const;
		double SigmoidDerivative(double output) const;

		void GenerateWeights();
		void Activate(int layer);
		void SetLearningRate(double lRate);
		void Visualize(double result, int g);
		void BackwardPropagateError(int expected);
		void UpdateWeights(std::vector<double> inputData);
		void ForwardPropagate(std::vector<double> inputData);
		void SetTheNumberOfNeuronsForEachLayer(int n0, int n1, int n2, int N3);

	public:
		Network(int n0, int n1, int n2, int n3, double learningRate);

		double NetworkTraining(std::vector<double>, int excepted);
		double Predict(const std::vector<double> inputData);

};
//---------------------------------------------------------------------------
#endif
