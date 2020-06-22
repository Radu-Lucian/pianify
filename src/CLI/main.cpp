#include <argparse.hpp>
#include <Pianify.h>
#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

utilities::FourierType parseFourierType(std::string_view fourierMethodName)
{
	static const std::unordered_map<std::string, utilities::FourierType> nameToType {
		{ "fft", utilities::FourierType::FastFourierTransformAquila },
		{ "stft", utilities::FourierType::ShortTimeFourierTransform },
		{ "astft", utilities::FourierType::ShortTimeFourierTransformAquila }
	};

	auto it = nameToType.find(fourierMethodName.data());
	if (it == std::end(nameToType))
	{
		throw std::runtime_error("Error: Unrecognized Fourier Method Type.");
	}
	return it->second;
}

std::string getInstrumentName(Instruments::InstrumentType instrumentType)
{
	auto& nameMap = Instruments::kStringToInstrument;
	auto it = std::find_if(std::cbegin(nameMap), std::cend(nameMap), [instrumentType](const auto& pair) { return pair.second == instrumentType; });
	return it != std::cend(nameMap) ? it->first : "none";
}

int main(int argc, char* argv[]) {
	argparse::ArgumentParser program("pianify");

	auto stringToPath = [](const std::string& fileName) { return fs::path(fileName); };

	program.add_argument("input_file")
		.help("the input 16-bit pcm wav file")
		.required()
		.action(stringToPath);

	program.add_argument("output_file")
		.help("the output MIDI file path")
		.required()
		.action(stringToPath);

	program.add_argument("-k", "--complexity_factor")
		.help("how many notes should be extracted per window")
		.default_value(5)
		.action([](const std::string& value) { 
			auto invalidFactor = std::runtime_error("Error: Invalid complexity factor. Must be an integer between 1 and 100.");
			try {
				int complexityFactor = std::stoi(value);
				if (complexityFactor < 1 || complexityFactor > 100)
					throw invalidFactor;
				return complexityFactor;
			}
			catch (std::invalid_argument)
			{ throw invalidFactor; }
			catch (std::out_of_range)
			{ throw invalidFactor; }
		});

	program.add_argument("-f", "--fourier_method")
		.help("fourier method used for extracting the notes | possible values: fft, stft, astft")
		.default_value(utilities::FourierType::ShortTimeFourierTransformAquila)
		.action(parseFourierType);

	program.add_argument("-r", "--recognize_instruments")
		.help("use if you want the instruments separated in the MIDI | this makes --fourier_method useless")
		.default_value(false)
		.implicit_value(true);

	try {
		program.parse_args(argc, argv);
	}
	catch (const std::runtime_error & err) {
		std::wcout << err.what() << std::endl;
		program.print_help();
		exit(0);
	}

	auto inputFilePath = program.get<fs::path>("input_file");
	if (!fs::exists(inputFilePath))
	{
		std::wcout << "Error: Input file '" << inputFilePath.wstring() << "' does not exist." << std::endl;
		std::exit(0);
	}
	auto outputFilePath = program.get<fs::path>("output_file");

	auto complexityFactor = program.get<int>("--complexity_factor");
	auto recognizeInstruments = program.get<bool>("--recognize_instruments");
	auto fourierMethod = program.get<utilities::FourierType>("--fourier_method");

	try {
		if (recognizeInstruments)
		{
			auto instruments = Pianify::recognizeInstruments(inputFilePath.string(), outputFilePath.string(), complexityFactor);
			std::wcout << "Conversion finished. Saved result as '" << outputFilePath.wstring() << "'." << std::endl;
			std::wcout << "Instruments found:" << std::endl;
			for (auto& [instrument, percent] : instruments)
			{
				std::cout << getInstrumentName(instrument) << " : " << std::setprecision(2) << std::fixed << percent * 100 << "%" << std::endl;
			}
		}
		else
		{
			Pianify::wavToMidi(inputFilePath.string(), outputFilePath.string(), complexityFactor, fourierMethod);
			std::wcout << "Conversion finished. Saved result as '" << outputFilePath.wstring() << "'.";
		}
	}
	catch (const std::runtime_error & err) {
		std::wcout << err.what() << std::endl;
	}

	return 0;
}