#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include <sstream>
#include <ctime>
#include <numeric>
#include <cstdio>

#include "Utils.h"

// Tested on a NVIDIA GeForce GTX 1080

/* 214 words
This program will calcualte the mean, min, max and standard deviation of the provided "temp_lincolnshire.txt" file.
The program follows a differnt structure to the one provided in tutorials 1 to 4. One of these differences was that
a vector containg vectors was used, where each of the sub vectors is the size of the kernel's max work group size 
and contains as many entries as the size of the work group. So for example if the max work group size is 256, each
vector will contain 256 entries. This was done to prevent any issues with the kernels being used on different systems.

Atmoic_add wasn't used and instead addReduce was used for calculating the sum of the mean. All the entries in the vector
were added to eachother right to left and the sum of the sub vector being the first value of the vector. An adapted 
version of add reduce was also used for working out the min and max but without the addition part.

A few optimisation strategies were used such as adapting the provided code to use division or binary shifting instead
of slow modulous to increase performance, and using private variables within the kernels to access the GPU's onboard
registers which are ultra fast, decreasing the kernels overall completion time.
*/

// Function declarations
void getEntries(std::vector<int>& tempVec);


void getEntries(std::vector<double>& tempVec) {

	std::ifstream readFile;
	readFile.open("temp_lincolnshire_datasets/temp_lincolnshire.txt"); // _short

	std::string line;

	while (std::getline(readFile, line)) {
		std::stringstream ss(line);
		std::string location, year, month, day, time, celcius;
		double tempurature;

		if (ss >> location >> year >> month >> day >> time >> celcius) {
			tempurature = stoi(celcius); // Converts string celcius to int
			tempVec.push_back(tempurature);
		}
	}

}

int main() {
	// Used to time the program
	// Ref: https://www.geeksforgeeks.org/chrono-in-c/
	auto start = chrono::high_resolution_clock::now();

	std::cout << "Reading in dataset...\n";

	std::vector<double> tempuratureVector; // Will contain tempurature data
	getEntries(tempuratureVector); // Loads dataset into memory

	std::cout << "Dataset loaded to memory...\n\n";

	// So the tempurature vector's size so the program doesn't have to keep recaluculating this
	int tempuratureVecSize = tempuratureVector.size();

	int platform_id = 0; int device_id = 0;
	cl_platform_id clPlatformId = 0;
	cl_device_id clDeviceId = 0;

	// gets platform context information (device id, platform id)
	cl::Context context = GetContext(platform_id, device_id);

	std::cout << "Runinng on " << GetPlatformName(platform_id) << ", " << GetDeviceName(platform_id, device_id) << std::endl;


	try {
		// Creates a queue that'll be used to push commands to the device
		cl::CommandQueue queue(context, CL_QUEUE_PROFILING_ENABLE);
		cl::Program::Sources sources;
		cl::Device device = GetDevice(platform_id, device_id);
		AddSources(sources, "kernels/my_kernels.cl");

		cl::Program program(context, sources);

		// Builds kernel files
		program.build(); // "-cl-denorms-are-zero"

		// Setsup kernels kernels
		cl::Kernel kernel_add = cl::Kernel(program, "addReduce");
		cl::Kernel kernel_minimum = cl::Kernel(program, "minimum");
		cl::Kernel kernel_maximum = cl::Kernel(program, "maximum");
		cl::Kernel kernel_stdDev = cl::Kernel(program, "stdDev");

		// All kernels similar enough that this data only needs to be displayed once
		int kernelWGSize = kernel_add.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device);
		auto maxWorkGroupSize = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
		auto localMemorySize = device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();

		// Gets the number of work groups thatll need to be made
		auto numOfWorkGroups = tempuratureVector.size() / maxWorkGroupSize;

		// Calculates needed workgroup size
		int workGroupSizeOf = maxWorkGroupSize * sizeof(double);

		std::cout << "Max work group size: " << maxWorkGroupSize << std::endl;
		std::cout << "Kernel work group size: " << kernelWGSize << std::endl;
		std::cout << "Size of each work group: " << maxWorkGroupSize << std::endl;
		std::cout << "Number work groups: " << numOfWorkGroups << std::endl;
		std::cout << "Device's local memory size: " << localMemorySize << " KB" << std::endl; // Maximum amount of memory a workgroup can use
		std::cout << "Size of dataset: " << tempuratureVector.size() << std::endl << std::endl;

		// Creates list containing list of data the length of the max work group size
		std::vector<std::vector<double>> tempVectorsVec;
		std::vector<double> temp;

		// Create a vector of vectors, where the subvectors are the size of the kernel's maximum work group size
		int ind = 0;
		for (ind; ind + maxWorkGroupSize < tempuratureVecSize; ind += maxWorkGroupSize) {
			temp = { tempuratureVector.begin() + ind, tempuratureVector.begin() + ind + maxWorkGroupSize };
			tempVectorsVec.push_back(temp);
		}

		// if the size of the list isnt divisable by the work group, the create a new list with the remaining data
		if (ind < tempuratureVecSize) {
			temp = { tempuratureVector.begin() + ind, tempuratureVector.end() };
			tempVectorsVec.push_back(temp);
		}

		// Initialises variables that store sum of all values in the dataset and mean, min and max
		double sum = 0;
		double mean;
		double minimum = DBL_MAX;
		double maximum = DBL_MIN;

		// vectors containing the data to push to the kernel
		std::vector<double> meanVec(workGroupSizeOf);
		std::vector<double> minVec(workGroupSizeOf);
		std::vector<double> maxVec(workGroupSizeOf);

		// vectors containing the results of each kernel's operations
		std::vector<double> meanRes(maxWorkGroupSize);
		std::vector<double> minRes(maxWorkGroupSize);
		std::vector<double> maxRes(maxWorkGroupSize);

		// Mean buffers
		cl::Buffer bufferMeanIn(context, CL_MEM_READ_WRITE, workGroupSizeOf);
		cl::Buffer bufferMeanOut(context, CL_MEM_READ_WRITE, workGroupSizeOf);

		// Minimum buffers
		cl::Buffer bufferMinIn(context, CL_MEM_READ_WRITE, workGroupSizeOf);
		cl::Buffer bufferMinOut(context, CL_MEM_READ_WRITE, workGroupSizeOf);

		// Maximum buffers
		cl::Buffer bufferMaxIn(context, CL_MEM_READ_WRITE, workGroupSizeOf);
		cl::Buffer bufferMaxOut(context, CL_MEM_READ_WRITE, workGroupSizeOf);

		// Stores the amount of time each kernel took to calculate the mean, min, max of the dataset respectively
		cl_ulong elapsedRunMean = 0; cl_ulong elapsedRunMin = 0; cl_ulong elapsedRunMax = 0;
		cl_ulong elapsedRWMean = 0; cl_ulong elapsedRWMin = 0; cl_ulong elapsedRWMax = 0;

		// Stores memory transfer time
		cl::Event runEventMean; cl::Event runEventMin; cl::Event runEventMax;
		cl::Event rwEventMean; cl::Event rwEventMin; cl::Event rwEventMax;

		std::cout << "\nRunning kernels...\n\n";

		// Needed to loop rather than passing the whole thing, may be a GPU specific issue as passing the whole tempuratureVector vector worked on a different PC
		for (int i = 0; i < tempVectorsVec.size(); i++) {

			// Needs to be be reinitialsed or there will be left over data from the last loop which will produce and incorrect mean
			// .clear() does not work in this case
			std::vector<double> meanVec(workGroupSizeOf);
			meanVec = tempVectorsVec[i];

			// Min and max vectors don't have to be clear and the minimum and maximum will always be the same.
			minVec = tempVectorsVec[i];
			maxVec = tempVectorsVec[i];

			// Writes to buffers
			queue.enqueueWriteBuffer(bufferMeanIn, CL_TRUE, 0, workGroupSizeOf, &meanVec[0], NULL, &rwEventMean);
			queue.enqueueWriteBuffer(bufferMinIn, CL_TRUE, 0, workGroupSizeOf, &minVec[0], NULL, &rwEventMin);
			queue.enqueueWriteBuffer(bufferMaxIn, CL_TRUE, 0, workGroupSizeOf, &maxVec[0], NULL, &rwEventMax);

			// Passes parameters to the kernels
			kernel_add.setArg(0, bufferMeanIn);
			kernel_add.setArg(1, workGroupSizeOf, nullptr);
			kernel_add.setArg(2, bufferMeanOut);

			kernel_minimum.setArg(0, bufferMinIn);
			kernel_minimum.setArg(1, workGroupSizeOf, nullptr);
			kernel_minimum.setArg(2, bufferMinOut);

			kernel_maximum.setArg(0, bufferMaxIn);
			kernel_maximum.setArg(1, workGroupSizeOf, nullptr);
			kernel_maximum.setArg(2, bufferMaxOut);

			// kernel, offset, Global size, local size
			// Work group size must be multiple of Global Size
			queue.enqueueNDRangeKernel(kernel_add, cl::NullRange, cl::NDRange(maxWorkGroupSize), cl::NDRange(maxWorkGroupSize), NULL, &runEventMean);
			queue.enqueueNDRangeKernel(kernel_minimum, cl::NullRange, cl::NDRange(maxWorkGroupSize), cl::NDRange(maxWorkGroupSize), NULL, &runEventMin);
			queue.enqueueNDRangeKernel(kernel_maximum, cl::NullRange, cl::NDRange(maxWorkGroupSize), cl::NDRange(maxWorkGroupSize), NULL, &runEventMax);

			// Waits for the kernels to finish before moving on
			runEventMean.wait();
			runEventMin.wait();
			runEventMax.wait();

			// Calculates how long each kernel took to run and adds that to a vaiable which tracks each kernels overall runtime
			elapsedRunMean += runEventMean.getProfilingInfo<CL_PROFILING_COMMAND_END>() - runEventMean.getProfilingInfo<CL_PROFILING_COMMAND_START>();
			elapsedRunMin += runEventMin.getProfilingInfo<CL_PROFILING_COMMAND_END>() - runEventMin.getProfilingInfo<CL_PROFILING_COMMAND_START>();
			elapsedRunMax += runEventMax.getProfilingInfo<CL_PROFILING_COMMAND_END>() - runEventMax.getProfilingInfo<CL_PROFILING_COMMAND_START>();

			// Reads the results of the kernels from the buffer
			queue.enqueueReadBuffer(bufferMeanOut, CL_TRUE, 0, workGroupSizeOf, &meanRes[0], NULL, &rwEventMean);
			queue.enqueueReadBuffer(bufferMinOut, CL_TRUE, 0, workGroupSizeOf, &minRes[0], NULL, &rwEventMin);
			queue.enqueueReadBuffer(bufferMaxOut, CL_TRUE, 0, workGroupSizeOf, &maxRes[0], NULL, &rwEventMax);

			// Waits for the kernels to finish before moving on - blocking
			rwEventMean.wait();
			rwEventMin.wait();
			rwEventMax.wait();

			// Calculates how long each kernel took to write to and read the buffer overall
			elapsedRWMean += rwEventMean.getProfilingInfo<CL_PROFILING_COMMAND_END>() - rwEventMean.getProfilingInfo<CL_PROFILING_COMMAND_START>();
			elapsedRWMin += rwEventMin.getProfilingInfo<CL_PROFILING_COMMAND_END>() - rwEventMin.getProfilingInfo<CL_PROFILING_COMMAND_START>();
			elapsedRWMax += rwEventMax.getProfilingInfo<CL_PROFILING_COMMAND_END>() - rwEventMax.getProfilingInfo<CL_PROFILING_COMMAND_START>();

			// Adds the current vector's calculated result to its assigned variable and for mean it adds up the sum of all the lists
			sum += meanRes[0];
			minimum = minRes[0] < minimum ? minRes[0] : minimum;
			maximum = maxRes[0] > maximum ? maxRes[0] : maximum;

		}

		// Calculates the mean of the entire dataset
		mean = sum / tempuratureVector.size();

		// vector containing the data to push to the kernel
		std::vector<double> stdDevVec;

		// vector containing the results of each kernel's operations
		std::vector<double> stdDevRes(1024);

		// StdDev buffershot af
		cl::Buffer bufferStdDevIn(context, CL_MEM_READ_WRITE, workGroupSizeOf);
		cl::Buffer bufferMean(context, CL_MEM_READ_WRITE, sizeof(double));
		cl::Buffer bufferStdDevOut(context, CL_MEM_READ_WRITE, workGroupSizeOf);

		double stdDev = 0;
		double sumStdDev = 0;

		// Stores the amount of time each kernel took to calculate the standard deviation of the dataset
		cl_ulong elapsedRunStdDev = 0; cl_ulong elapsedRWStdDev = 0;

		// Stores memory transfer time
		cl::Event runEventStdDev; cl::Event rwEventStdDev;

		// used to work out standard deviation, done seperately as mean found in the previous loop required for this
		for (int i = 0; i < tempVectorsVec.size(); i++) {
			stdDevVec = tempVectorsVec[i];

			// Writes to buffers
			queue.enqueueWriteBuffer(bufferStdDevIn, CL_TRUE, 0, workGroupSizeOf, &stdDevVec[0]);
			queue.enqueueWriteBuffer(bufferMean, CL_TRUE, 0, sizeof(double), &mean);
			queue.enqueueWriteBuffer(bufferStdDevOut, CL_TRUE, 0, workGroupSizeOf, &stdDevRes[0]);

			// Passes parameters to the kernels
			kernel_stdDev.setArg(0, bufferStdDevIn);
			kernel_stdDev.setArg(1, bufferMean);
			kernel_stdDev.setArg(2, workGroupSizeOf, nullptr);
			kernel_stdDev.setArg(3, bufferStdDevOut);

			// kernel, offset, Global size, local size
			// Work group size must be multiple of Global Size
			queue.enqueueNDRangeKernel(kernel_stdDev, cl::NullRange, cl::NDRange(maxWorkGroupSize), cl::NDRange(maxWorkGroupSize), NULL, &runEventStdDev);

			// Waits for the kernels to finish before moving on
			runEventStdDev.wait();

			// Calculates how long each kernel took to run and adds that to a vaiable which tracks each kernels overall runtime
			elapsedRunStdDev += runEventStdDev.getProfilingInfo<CL_PROFILING_COMMAND_END>() - runEventStdDev.getProfilingInfo<CL_PROFILING_COMMAND_START>();

			// Reads the results of the kernel from the buffer
			queue.enqueueReadBuffer(bufferStdDevOut, CL_TRUE, 0, workGroupSizeOf, &stdDevRes[0], NULL, &rwEventStdDev);

			// Waits for the kernel to finish before moving on - blocking
			rwEventStdDev.wait();

			// Calculates how long each kernel took to write to and read the buffer overall
			elapsedRWStdDev += rwEventStdDev.getProfilingInfo<CL_PROFILING_COMMAND_END>() - rwEventStdDev.getProfilingInfo<CL_PROFILING_COMMAND_START>();

			// Adds up all the results calculated why the kernel t
			for (int j = 0; j < stdDevVec.size(); j++) {
				sumStdDev += stdDevRes[j];
			}

		}

		// Calculates standard deviation of 
		stdDev = sqrt((sumStdDev / (tempuratureVector.size() - 1)));
		//cl::flush;

		std::cout << "Mean: " << sum / tempuratureVector.size() << std::endl;
		std::cout << "Minimum: " << minimum << std::endl;
		std::cout << "Maximum: " << maximum << std::endl;
		std::cout << "Standard Deviation: " << stdDev << std::endl;
		std::cout << std::endl;
		std::cout << "Total time taken for addReduce kernel to calculate the mean: " << elapsedRunMean << " ns" << std::endl;
		std::cout << "Total time taken for minimum kernel to calculate the minimum: " << elapsedRunMin << " ns" << std::endl;
		std::cout << "Total time taken for maximum kernel to calculate the maximum: " << elapsedRunMax << " ns" << std::endl;
		std::cout << "Total time taken for stdDev kernel to calculate the stadrdard deviation: " << elapsedRunStdDev << " ns" << std::endl;
		std::cout << std::endl;
		std::cout << "Average time taken per addReduce kernel: " << elapsedRunMean / tempVectorsVec.size() << " ns" << std::endl;
		std::cout << "Average time taken per minimum kernel: " << elapsedRunMin / tempVectorsVec.size() << " ns" << std::endl;
		std::cout << "Average time taken per maximum kernel: " << elapsedRunMax / tempVectorsVec.size() << " ns" << std::endl;
		std::cout << "Average time taken per stdDev kernel: " << elapsedRunStdDev / tempVectorsVec.size() << " ns" << std::endl;
		std::cout << std::endl;
		std::cout << "Total memory transfer for addReduce kernel: " << elapsedRWMean << " kb" << std::endl;
		std::cout << "Total memory transfer for minimum kernel: " << elapsedRWMin << " kb" << std::endl;
		std::cout << "Total memory transfer for maximum kernel: " << elapsedRWMax << " kb" << std::endl;
		std::cout << "Total memory transfer for stdDev kernel: " << elapsedRWStdDev << " kb" << std::endl;
		std::cout << std::endl;

		// Calculates how long program took to 
		auto end = chrono::high_resolution_clock::now();
		double timeTaken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
		timeTaken *= 1e-9;

		std::cout << "\nTotal program execution time: " << fixed << timeTaken << " seconds.\n";
		std::cout << "Please press enter to close the program...";
		std::getchar();
	}
	catch (cl::Error err) {
		std::cerr << "ERROR: " << err.what() << ", " << getErrorString(err.err()) << std::endl;
	}
	return 0;
}