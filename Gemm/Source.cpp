#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

void valueCheck(vector<float>& valueCheckInput, int input_n, int input_c, int input_h, int input_w, int offset = 0) {
	if (offset == 1) { input_n = 1; }

	int temp1 = input_w * input_h * input_c;
	for (int ⁠n_idx = 0; ⁠n_idx < input_n; ⁠n_idx++)
	{
		int temp2 = ⁠n_idx * temp1;
		for (int ⁠c_idx = 0; ⁠c_idx < input_c; ⁠c_idx++)
		{
			int temp3 = ⁠c_idx * input_w * input_h + temp2;
			for (int ⁠h_idx = 0; ⁠h_idx < input_h; ⁠h_idx++)
			{
				int temp4 = ⁠h_idx * input_w + temp3;
				cout << "  ";
				for (int w_idx = 0; w_idx < input_w; w_idx++)
				{
					int g_idx = w_idx + temp4;

					//cout.setf(ios::fixed);
					//cout.precision(6);
					cout << setw(8) << valueCheckInput[g_idx] << " ";
				}cout << endl;
			}cout << endl; cout << endl;
		}cout << endl;
	}cout << endl;
}


int main()
{
	int batchSize = 1;
	int input_n = batchSize;
	int input_c = 3;
	int input_h = 4;
	int input_w = 4;

	vector<float> inputImage(input_n * input_c * input_h * input_w);


	float count = 1.f;
	int temp1 = input_c * input_h * input_w;
	for (int ⁠n_idx = 0; ⁠n_idx < input_n; ⁠n_idx++)
	{
		int temp2 = ⁠n_idx * temp1;
		for (int ⁠c_idx = 0; ⁠c_idx < input_c; ⁠c_idx++)
		{
			int temp3 = ⁠c_idx * input_h * input_w + temp2;
			for (int ⁠h_idx = 0; ⁠h_idx < input_h; ⁠h_idx++)
			{
				int temp4 = ⁠h_idx * input_w + temp3;
				for (int w_idx = 0; w_idx < input_w; w_idx++)
				{
					int g_idx = w_idx + temp4;
					inputImage[g_idx] = count;
					count++;
				}
			}
		}
	}

	int kernelWidth = 2;
	int kernelHeight = 2;
	int stride = 1;
	int paddingSize = 0;
	int output_c = 4;

	int W_out = (input_w - kernelWidth + 2 * paddingSize) / stride + 1;
	int H_out = (input_h - kernelHeight + 2 * paddingSize) / stride + 1;

	//im2col

	vector<float> resultIm2col(kernelWidth * kernelHeight * input_c * H_out * W_out); // 4 * 3 * 3 * 3 = 12 * 9

	for (int ⁠c_idx = 0; ⁠c_idx < input_c; ⁠c_idx++) {
		for (int colStride = 0; colStride < H_out; colStride++) {// - > 
			for (int rowStride = 0; rowStride < W_out; rowStride++) {
				for (int y = 0; y < kernelHeight; y++) { // 
					for (int x = 0; x < kernelWidth; x++) { // 		
						int ⁠g_idx_R = ⁠c_idx * 36 + colStride + rowStride * 3 + x * 9 + y * 18;
						int g_idx_I = ⁠c_idx * 16 + colStride + rowStride * 4 + x + y * 4;
						resultIm2col[⁠g_idx_R] = inputImage[g_idx_I];
					}
				}
			}
		}
	}


	cout << endl;
	cout << "<im2col Result>" << endl;

	for (int i = 0; i < 12; i++) {
		int temp = i * 9;
		for (int j = 0; j < 9; j++) {

			int g_idx = temp + j;
			cout << setw(3) << resultIm2col[g_idx];

		}cout << endl;
	}cout << endl; cout << endl;

	//
	vector<float> kernel(output_c * input_c * kernelWidth * kernelHeight);
	float count2 = 1.f;
	int temp_1 = input_c * kernelWidth * kernelHeight;
	for (int ⁠n_idx = 0; ⁠n_idx < output_c; ⁠n_idx++)
	{
		int temp_2 = ⁠n_idx * temp_1;
		for (int ⁠c_idx = 0; ⁠c_idx < input_c; ⁠c_idx++)
		{
			int temp_3 = ⁠c_idx * kernelWidth * kernelHeight + temp_2;
			for (int ⁠h_idx = 0; ⁠h_idx < kernelHeight; ⁠h_idx++)
			{
				int temp_4 = ⁠h_idx * kernelWidth + temp_3;
				for (int w_idx = 0; w_idx < kernelWidth; w_idx++)
				{
					int g_idx = w_idx + temp_4;
					kernel[g_idx] = count2;
					count2++;
				}
			}
		}
	}



	valueCheck(kernel, 4, 3, 2, 2);


	// Reshaped kernel
	cout << "Reshaped kernel" << endl;

	for (int ⁠oc_idx = 0; ⁠oc_idx < output_c; ⁠oc_idx++) {// 4
		int temp = ⁠oc_idx * 12;
		for (int ⁠r_idx = 0; ⁠r_idx < input_c * kernelHeight * kernelWidth; ⁠r_idx++) {// 12
			int g_idx = ⁠r_idx + temp;
			cout << setw(3) << kernel[g_idx];
		}cout << endl;
	}cout << endl; cout << endl;


	vector<float> midResult(output_c  * W_out * H_out);

	for (int o = 0; o < output_c; o++) {

		int temp1 = o * kernelHeight * kernelWidth * input_c;

		for (int i = 0; i < 1; ++i) {

			for (int j = 0; j < W_out * H_out; ++j)
			{
				int sum = 0;

				int j_idx = j + o * W_out * H_out;

				for (int k = 0; k < kernelHeight * kernelWidth * input_c; ++k)
				{
					sum += kernel[temp1 + k] * resultIm2col[k *  W_out * H_out + j];
				}

				midResult[j_idx] = (int)sum;
			}
		}
	}
	

	cout << "midResult" << endl;
	for (int ⁠oc_idx = 0; ⁠oc_idx < output_c; ⁠oc_idx++) {
		for (int h = 0; h < 3; h++) {
			for (int w = 0; w < 3; w++) {

				int gid = ⁠oc_idx * 9 + h * 3 + w;
				cout << setw(10) << midResult[gid];

			}cout << endl;
		}cout << endl; cout << endl;
	}cout << endl; cout << endl;

	cout << "끝" << endl;

	return 0;
}
