// Local vec size is as big as local work group is
kernel void addReduce(global double* gVec, local double* lVec, global double* outVec) {
	int id = get_global_id(0);
	int lid = get_local_id(0); // ID of work item in work group
	int s = get_local_size(0); // size of workgroup

	// Transfer global memory to local
	lVec[lid] = gVec[id];

	// Allows all memory items to finish copying over to local memory
	barrier(CLK_LOCAL_MEM_FENCE);

	// Dont use % as less effective
	// Binary shift to the right by 1, same as i being divide by to each time but much faster
	// e.g. i = 8, i /2 = 4, i / 2 = 2 etc
	// e.g. i = 8 = 1000, >>1 then i = 4 = 0100 etc
	for (int i = s/2; i > 0; i >>= 1 ) {
		if (lid < i) { //. Makes sure I doesnt go below current ID
			lVec[lid] += lVec[lid + i];
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	
	// If at start of array, send out
	if (lid == 0) {
		outVec[id] = lVec[0];// Gets work group's ID
	}

}

// Finds the minimum value in the vector passed
kernel void minimum(global double* gVec, local double* lVec, global double* outVec) {
	int id = get_global_id(0);
	int lid = get_local_id(0); // ID of work item in work group
	int s = get_local_size(0); // size of workgroup

	// Transfer global memory to local
	lVec[lid] = gVec[id]; 


	// Allows all memory items to finish copying over to local memory
	barrier(CLK_LOCAL_MEM_FENCE);

	// Doesnt use % as much slower
	for (int i = s/2; i > 0; i >>= 1 ) {
		if (lid < i && (lVec[lid] > lVec[lid + i])) {
			lVec[lid] = lVec[lid + i];
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	
	// If at start of array, send out
	if (lid == 0) {
		outVec[id] = lVec[0];// Gets work group's ID
	}
}

// Finds the maximum value in the vector passed
kernel void maximum(global double* gVec, local double* lVec, global double* outVec) {
	int id = get_global_id(0);
	int lid = get_local_id(0); // ID of work item in work group
	int s = get_local_size(0); // size of workgroup

	// Transfer global memory to local
	lVec[lid] = gVec[id]; 

	// Allows all memory items to finish copying over to local memory
	barrier(CLK_LOCAL_MEM_FENCE);

	// Dont use % as slower
	for (int i = s/2; i > 0; i >>= 1 ) {
		if (lid < i && (lVec[lid] < lVec[lid + i])) {
			lVec[lid] = lVec[lid + i];
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	
	// If at start of array, send out
	if (lid == 0) {
		outVec[get_group_id(0)] = lVec[0];// Gets work group's ID
	}
}

// Calculares sum of x minus the mean squared
kernel void stdDev(global double* gVec, global const double* xMean, local double* lVec, global double* outSum) {
	int id = get_global_id(0);
	int lid = get_local_id(0);
	int s = get_local_size(0); // size of workgroup
	double private temp;
	double private mean;

	// Transfer global memory to local
	lVec[lid] = gVec[id];
	mean = xMean[0]; // Transfers mean to private memory as faster
	barrier(CLK_LOCAL_MEM_FENCE); // waits for the transfer from global to local memory to complete

	temp = lVec[lid];
	outSum[id] = (temp - mean) * (temp - mean); 

}
