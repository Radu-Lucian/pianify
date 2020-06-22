#pragma once

#include "tiny_dnn/tiny_dnn.h"


class Model
{
public:
	Model();

	~Model() = default;

public:
	size_t predictOutput(const tiny_dnn::vec_t& data);

private:
	void trainModel(const std::string& inPath, const std::string& outName);
	void loadModel(const std::string& networkName);

private:
	tiny_dnn::network<tiny_dnn::sequential> networkMLP;
};