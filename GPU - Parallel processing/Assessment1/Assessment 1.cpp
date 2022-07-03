// #include <CL/cl2.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include <sstream>
#include <ctime>

#include "Utils.h"

/* TODO
- Pass - report:
-- Mean
-- Min
-- Max
-- Std Dev
- Report memory transfer and execution times to user

- 2:2
-- Attempt at optimisation
-- Report program performance (?)

- 2:1
-- Use well optimised kernels (code executed)
-- Use whole temp values not just floats
-- Report clearly program performance to user (memory used?? )

- 1st
-- Show median, 1st and 3rd quartile
-- Optimisation based on local memory considered
--  Program performance is clearly reported and communicated to the user in detail
*/

/* Summary*/
/*
- Use double
- show mean, min, max, std dev, median, 1st, 3rd quartile
- Show memory transfer, kernel execution, and total program execution times
- When optimising: display execution times for different variants of your algorithm
*/


/*
You should pay careful attention to the correctness of the developed algorithms and
effectiveness of any optimisation strategies. Your code should follow good programming
practices such as clear commenting, proper structure, and use of descriptive names for methods
and variables. You must also clearly explain how to run your code by including a “readme” file
with your submission.
*/

/*
You are also required to provide a short summary of your implementation (max. 300 words),
which should be included as a code comment section above the main function in your host code.
This should briefly describe the implemented functionality, clearly indicate any original
developments in the code (beyond what was provided in the workshops), cite any external
sources used, and detail any optimisation strategies employed. Note that your grade will solely
depend on the quality of your code, so keep this section concise and do not exceed the word
count.
*/

// TODO: point to list and pass address instead
// TODO: delete once finished



// Function declarations
void getBatchEntries(std::vector<int>& tempVec);
double getMin(std::vector<double>& ta);
double getMax(std::vector<double>& ta);

//auto
void getBatchEntries(std::vector<double>& tempVec) {

	std::ifstream readFile;
	readFile.open("temp_lincolnshire_datasets/temp_lincolnshire.txt");

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

	// std::cout << ListPlatformsDevices() << std::endl;

	// Gets tempuratures list (currently just the 1) TODO: pass index to start from?
	std::vector<double> tempuratureVector;
	getBatchEntries(tempuratureVector);

	// ===
	int platform_id = 0; int device_id = 0;

	try {
		//Part 2 - host operations
		//2.1 Select computing devices
		cl::Context context = GetContext(platform_id, device_id);

		//display the selected device
		std::cout << "Runinng on " << GetPlatformName(platform_id) << ", " << GetDeviceName(platform_id, device_id) << std::endl;

		//create a queue to which we will push commands for the device
		cl::CommandQueue queue(context, CL_QUEUE_PROFILING_ENABLE);

		//2.2 Load & build the device code
		cl::Program::Sources sources;

		AddSources(sources, "kernels/my_kernels.cl");

		cl::Program program(context, sources);

		//build and debug the kernel code
		try {
			program.build();
		}
		catch (const cl::Error& err) {
			std::cout << "Build Status: " << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			std::cout << "Build Options:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			std::cout << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			throw err;
		}

		int localSize = 2048;

		//std::vector<int> A = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -10, 11, 12, 13, 14, 15, 1, 2, 3, 4, 5, -1, 9, 1 , 12, 3, 5, 16, 7, 4, 16, 12 };
		std::vector<double> A = tempuratureVector;

		// size_t inputSize = A.size() * sizeof(int);//size in bytes
		//size_t nr_groups = inputElements / localSize; // -> number of lists to make

		//int startIndex = localSize;
		int index = localSize;

		std::vector<double> tempMean = { A.begin(), A.begin() + localSize }; // 0 - 10
		std::vector<double> tempMin = { A.begin(), A.begin() + localSize }; // 0 - 10
		std::vector<double> tempMax = { A.begin(), A.begin() + localSize }; // 0 - 10

		std::vector<double> tempMeanRes(localSize);
		std::vector<double> tempMinRes(localSize);
		std::vector<double> tempMaxRes(localSize);

		size_t inputElements = tempMean.size();//number of input elements
		size_t vectorSize = tempMean.size() * sizeof(double);

		std::cout << "A size = " << A.size() << std::endl;

		// int is 2^31 ~2 billbion so big enough for ds
		// If less than 1024 entries left, do by hand
		
		cl::Event startEvent;
		cl_ulong elapsed = 0;

		cl::Kernel kernel_add = cl::Kernel(program, "scan_add");

		//device - buffers
		cl::Buffer bufferMean(context, CL_MEM_READ_WRITE, vectorSize);
		//cl::Buffer bufferMin(context, CL_MEM_READ_WRITE, vectorSize);
		//cl::Buffer bufferMax(context, CL_MEM_READ_WRITE, vectorSize);
		cl::Buffer bufferMeanRes(context, CL_MEM_READ_WRITE, vectorSize);
		//cl::Buffer bufferMinRes(context, CL_MEM_READ_WRITE, vectorSize);
		//cl::Buffer bufferMaxRes(context, CL_MEM_READ_WRITE, vectorSize);
		std::cout << "Running kernel...\n";
		// ========= TODO: make more lists then run them all at once
		for (index = localSize; index + localSize <= A.size(); index += localSize) {
			tempMeanRes = { A.begin() + index, A.begin() + index + localSize };
			//tempMinRes = { A.begin() + index, A.begin() + index + localSize };
			//tempMaxRes = { A.begin() + index, A.begin() + index + localSize };

			//4.1 copy array A to and initialise other arrays on device memory
			queue.enqueueWriteBuffer(bufferMean, CL_TRUE, 0, vectorSize, &tempMean[0]);
			//queue.enqueueWriteBuffer(bufferMin, CL_TRUE, 0, vectorSize, &tempMin[0]);
			//queue.enqueueWriteBuffer(bufferMax, CL_TRUE, 0, vectorSize, &tempMax[0]);

			queue.enqueueWriteBuffer(bufferMeanRes, CL_TRUE, 0, vectorSize, &tempMeanRes[0]);
			//queue.enqueueWriteBuffer(bufferMinRes, CL_TRUE, 0, vectorSize, &tempMinRes[0]);
			//queue.enqueueWriteBuffer(bufferMaxRes, CL_TRUE, 0, vectorSize, &tempMaxRes[0]);

			kernel_add.setArg(0, bufferMean);
			kernel_add.setArg(1, bufferMeanRes);
			kernel_add.setArg(2, workGroupSizeOf, nullptr);
			kernel_add.setArg(3, workGroupSizeOf, nullptr);

			//cl::Kernel kernel_minimum = cl::Kernel(program, "minimum");
			//kernel_minimum.setArg(0, bufferMin);
			//kernel_minimum.setArg(1, bufferMinRes);

			//cl::Kernel kernel_maximum = cl::Kernel(program, "maximum");
			//kernel_maximum.setArg(0, bufferMax);
			//kernel_maximum.setArg(1, bufferMaxRes);

			//call all kernels in a sequence
			queue.enqueueNDRangeKernel(kernel_add, cl::NullRange, cl::NDRange(inputElements), cl::NullRange, NULL, &startEvent);
			//queue.enqueueNDRangeKernel(kernel_minimum, cl::NullRange, cl::NDRange(inputElements), cl::NullRange);
			//queue.enqueueNDRangeKernel(kernel_maximum, cl::NullRange, cl::NDRange(inputElements), cl::NullRange);

			startEvent.wait();
			elapsed += startEvent.getProfilingInfo<CL_PROFILING_COMMAND_END>() - startEvent.getProfilingInfo<CL_PROFILING_COMMAND_START>();

			//4.3 Copy the result from device to host
			queue.enqueueReadBuffer(bufferMean, CL_TRUE, 0, vectorSize, &tempMean[0]);
			//queue.enqueueReadBuffer(bufferMin, CL_TRUE, 0, vectorSize, &tempMin[0]);
			//queue.enqueueReadBuffer(bufferMax, CL_TRUE, 0, vectorSize, &tempMax[0]);
			queue.enqueueReadBuffer(bufferMeanRes, CL_TRUE, 0, vectorSize, &tempMeanRes[0]);
			//queue.enqueueReadBuffer(bufferMinRes, CL_TRUE, 0, vectorSize, &tempMinRes[0]);
			//queue.enqueueReadBuffer(bufferMaxRes, CL_TRUE, 0, vectorSize, &tempMaxRes[0]);

			tempMean = tempMeanRes;
			//tempMin = tempMinRes;
			//tempMax = tempMaxRes;
		}

		// Joins the added up values with what's left to add up
		tempMean.insert(tempMean.end(), A.begin() + index, A.end());
		//tempMin.insert(tempMin.end(), A.begin() + index, A.end());
		//tempMax.insert(tempMax.end(), A.begin() + index, A.end());

		double long sum = 0;
		for (int i = 0; i < tempMean.size(); i++) {
			sum += tempMean[i];
		}

		double mean = sum / A.size();

		time_t endTime;

		std::cout << "end: " << time(&endTime) << std::endl;

		
		double min = DBL_MAX;

		for (int i = 0; i < tempMin.size(); i++) {
			if (tempMin[i] < min) {
				min = tempMin[i];
			}
		}


		double max = DBL_MIN;

		for (int i = 0; i < tempMax.size(); i++) {
			if (tempMax[i] > max) {
				max = tempMax[i];
			}
		}


		std::cout << "Mean: " << mean << std::endl;
		std::cout << "Time taken to calculate mean of " << A.size() << " entries: " << elapsed << std::endl; // 72 seconds

		std::cout << "Min: " << min << std::endl;
		std::cout << "Max: " << max << std::endl;
		

		// Std Dev

		index = localSize;

		/*
		double mean = 9.25; /////////////////////

		std::vector<double> tempX1;
		std::vector<double> tempAdder(localSize, 0); // Functions as second list to add values to for add kernel

		for (index = localSize; index + localSize <= A.size(); index += localSize) {

			tempX1 = { A.begin() + index, A.begin() + index + localSize };

			//device - buffers
			cl::Buffer bufferTempX1(context, CL_MEM_READ_WRITE, vectorSize);
			cl::Buffer bufferTempAdder(context, CL_MEM_READ_WRITE, vectorSize);
			cl::Buffer bufferMeanVal(context, CL_MEM_READ_WRITE, sizeof(double));

			//4.1 copy array A to and initialise other arrays on device memory
			queue.enqueueWriteBuffer(bufferTempX1, CL_TRUE, 0, vectorSize, &tempX1[0]);
			queue.enqueueWriteBuffer(bufferMeanVal, CL_TRUE, 0, vectorSize, &mean);

			cl::Kernel kernel_XMinusMeanSqd = cl::Kernel(program, "XMinusMeanSqd");
			kernel_XMinusMeanSqd.setArg(0, bufferTempX1);
			kernel_XMinusMeanSqd.setArg(1, bufferMeanVal);

			//call all kernels in a sequence
			queue.enqueueNDRangeKernel(kernel_XMinusMeanSqd, cl::NullRange, cl::NDRange(inputElements), cl::NullRange);

			//4.3 Copy the result from device to host
			queue.enqueueReadBuffer(bufferTempX1, CL_TRUE, 0, vectorSize, &tempX1[0]);


			cl::Kernel kernel_add = cl::Kernel(program, "add");
			kernel_add.setArg(0, bufferTempX1);
			kernel_add.setArg(1, bufferTempAdder);

			queue.enqueueNDRangeKernel(kernel_add, cl::NullRange, cl::NDRange(inputElements), cl::NullRange);
			queue.enqueueReadBuffer(bufferTempAdder, CL_TRUE, 0, vectorSize, &tempAdder[0]);

			tempX1 = tempAdder;

			break;
		}

		std::cout << "tempAdder: " << tempAdder << std::endl;
		std::cout << "tempX1: " << tempX1 << std::endl;

		std::vector<double> rest = { A.begin() + index, A.end() };

		for (int i = 0; i < rest.size(); i++) {
			rest[i] = (rest[i] - mean) * (rest[i] - mean);
		}

		tempX1.insert(tempX1.end(), rest.begin() + index, rest.end());

		double long sum = 0;

		for (int i = 0; i < tempX1.size(); i++) {
			sum += tempMean[i];
		}

		// HERE
		double stdDev = sqrt(sum / (A.size() - 1));

		std::cout << "Standard Deviation: " << stdDev << std::endl;
		*/

	}
	catch (cl::Error err) {
		std::cerr << "ERROR: " << err.what() << ", " << getErrorString(err.err()) << std::endl;
	}

	return 0;
}
