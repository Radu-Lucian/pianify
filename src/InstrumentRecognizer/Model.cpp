#include "Model.h"
#include <directories.h>

#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <random>

using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;

Model::Model()
{
	loadModel(directories::getModelsDir() + std::string("/") + std::string("net"));
}

size_t Model::predictOutput(const vec_t& data)
{
	auto label = networkMLP.predict_label(data);
	return static_cast<size_t>(label);
}

void Model::trainModel(const std::string& inPath, const std::string& outName)
{
	RMSprop opt;
	network<sequential> networkMLP;
	networkMLP << fully_connected_layer(40, 256) << relu()
		<< fully_connected_layer(256, 2) << softmax();

	std::vector<std::pair<int, std::vector<double>>> inputMatrix;
	std::ifstream inputFile;
	
	inputFile.open(inPath);
	std::string line;
	while (std::getline(inputFile, line))
	{
		std::stringstream lineStream(line);
		std::string cell;
		int count = 0;
		bool first = true;
		int label = 0;
		std::vector<double> aux(40);

		while (std::getline(lineStream, cell, ','))
		{
			if (first)
			{
				first = false;
				label = std::atoi(cell.c_str());
				if (label == 56)
					label = 1;
				else
					label = 0;
			}
			else
			{
				aux[count++] = std::atof(cell.c_str());
			}
		}
		inputMatrix.push_back(std::make_pair(label, aux));
	}
	inputFile.close();

	std::shuffle(inputMatrix.begin(), inputMatrix.end(), std::mt19937{ std::random_device{}() });

	std::vector<vec_t> x_data, x_train;
	std::vector<label_t> y_data, y_train;

	for (size_t i = 0; i < inputMatrix.size(); i++)
	{
		y_data.push_back(inputMatrix[i].first);
		std::vector<float_t, aligned_allocator<float_t, 64>> aux;
		for (size_t j = 0; j < inputMatrix[i].second.size(); j++)
		{
			aux.push_back(inputMatrix[i].second[j]);
		}
		x_data.push_back(aux);
	}
	for (size_t i = 0; i < inputMatrix.size() * 0.1; i++)
	{
		x_train.push_back(x_data[i]);
		y_train.push_back(y_data[i]);
	}

	x_data.erase(x_data.begin(), x_data.begin() + x_data.size() * 0.1);
	y_data.erase(y_data.begin(), y_data.begin() + y_data.size() * 0.1);


	int epochs = 2000;
	int epoch = 0;
	int batch = 32;
	timer t;
	networkMLP.train<mse>(opt, x_data, y_data, batch, epochs,
		[&]() {
		t.elapsed();
		t.restart();
	},
		[&]() {
		result res = networkMLP.test(x_train, y_train);
		std::cout << "Accuracy: " << res.accuracy() << std::endl;
		std::cout << res.num_success << "/" << res.num_total << std::endl;
		std::cout << "Epoch: " << std::to_string(epoch++) << std::endl;
	});

	auto test_result = networkMLP.test(x_data, y_data);
	auto predictedLabel = networkMLP.predict_label(x_data[120]);
	auto trueLabel = y_data[120];
	std::cout << "Predicted Label by MLP: " << predictedLabel << ", actual label: " << trueLabel << std::endl;
	std::cout << test_result.accuracy() << std::endl;

	networkMLP.save(outName);
}

void Model::loadModel(const std::string& networkName)
{
	networkMLP << fully_connected_layer(40, 256) << relu()
		<< fully_connected_layer(256, 2) << softmax();

	networkMLP.load(networkName);
}