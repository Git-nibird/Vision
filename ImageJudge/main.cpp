#include<opencv2\opencv.hpp>
#include<iostream>
#include<string>
#include<cmath>
#include<ctime>
using namespace cv;
using namespace std;

double getMSE(const Mat& test, const Mat& reference)//MSE=sum((test-eval)^2)/(m*n)
{
	assert(test.rows == reference.rows && test.cols == reference.cols);
	assert(test.channels() == 1 && reference.channels() == 1);

	Mat _test, _reference, diff;
	test.convertTo(_test, CV_32FC1);
	reference.convertTo(_reference, CV_32FC1);
	absdiff(_test, _reference, diff);
	Mat multiple = diff.mul(diff);
	Scalar t = mean(multiple);
	return t.val[0];
}

double getPSNR(const Mat& test, double mse)//PSNR=10log(max(test)/mse)
{
	const uchar* ix;
	int temp = 0;
	for (int i = 0; i < test.rows; ++i)
	{
		ix = test.ptr<uchar>(i);
		for (int j = 0; j < test.cols; ++j)
		{
			if (ix[j] > temp)
			{
				temp = ix[j];
			}
		}
	}
	return 10 * log10(temp*temp / (mse+0.000001)); //add a small number avoiding the divider to be zero
}

double getSSIM(const Mat& test, const Mat& reference)//SSIM=(2*mean_test*mean_ref+c1)*(2*cov_test_ref+c2)/
                //     ((mean_test^2+mean_ref^2+c1)*(sd_test^2+sd_ref^2+c2))
{
	const double C1 = 6.5025, C2 = 58.5525;

	assert(test.rows == reference.rows && test.cols == reference.cols);
	assert(test.channels() == 1 && reference.channels() == 1);


	Mat _test, _reference;
	test.convertTo(_test, CV_32FC1);
	reference.convertTo(_reference, CV_32FC1);
	Mat multiple = _test.mul(_reference);
	Scalar mean_test, mean_ref, mean_mul;

	mean_test = mean(_test);
	mean_ref = mean(_reference);
	mean_mul = mean(multiple);

	double cov = mean_mul.val[0] - mean_test.val[0] * mean_ref.val[0];
	double temp1 = (2 * mean_test.val[0] * mean_ref.val[0] + C1)*(2 * cov + C2);
	
	Mat test2 = _test.mul(_test);
	Mat ref2 = _reference.mul(_reference);
	Scalar mean_test2, mean_ref2;

	mean_test2 = mean(test2);
	mean_ref2 = mean(ref2);

	double temp2 = (mean_test.val[0] * mean_test.val[0] + mean_ref.val[0] * mean_ref.val[0] + C1)*(mean_test2.val[0] - mean_test.val[0] * mean_test.val[0] + mean_ref2.val[0] - mean_ref.val[0] * mean_ref.val[0] + C2);
	return temp1 / temp2;
}

double getCV(const Mat& test)//mean(Laplacian(iamge))
{
	Mat temp(test.rows, test.cols, CV_8UC1);
	if (test.channels() != 1)
	{
		cout << "ERROR: the tested or reference image should be gray style!" << endl;
		return -1;
	}
	Laplacian(test, temp, test.depth());
	Scalar t = mean(temp);
	return t.val[0];
}

double getNR(const Mat& test)//nr=mean|deNoise-noise|/mean(deNoise)
{
	Mat deNoise, diff;
	//GaussianBlur(test, deNoise)
	//three kinds of noise: Gaussian, Spotted, Salt and Pepper, how to decide which kind it is?
	medianBlur(test, deNoise, 3);
	absdiff(deNoise, test, diff);
	Scalar t1 = mean(diff);
	Scalar t2 = mean(deNoise);
	return t1.val[0] / t2.val[0];
}

void createMatrix(double **&matrix, Size size)
{
	matrix = new double*[size.height];
	for (int i = 0; i < size.height; ++i)
	{
		matrix[i] = new double[size.width]();
	}
}

void releaseMatrix(double **&matrix, Size size)
{
	for (int i = 0; i < size.height; ++i)
	{
		delete[] matrix[i];
	}
	delete [] matrix;
}

double getSMD2(const Mat& test)//SMD2=sum(|test(x,y)-test(x+1,y)|*|test(x,y)-test(x,y+1)|)
{
	double temp = 0;
	const uchar* ix;
	const uchar* ix1;
	for (int i = 0; i < test.rows-1; ++i)
	{
		ix = test.ptr<uchar>(i);
		ix1 = test.ptr<uchar>(i + 1);
		for (int j = 0; j < test.cols-1; ++j)
		{
			temp += abs(ix[j] - ix1[j])*abs(ix[j] - ix[j + 1]);
		}
	}
	return temp / (test.rows * test.cols);
}

double **fwt97(double** matrix, int width, int height)
{
	//9 / 7 Coefficients:
	double a1 = -1.586134342;
	double a2 = -0.05298011854;
	double a3 = 0.8829110762;
	double a4 = 0.4435068522;

	//Scale coeff:
	double k1 = 0.81289306611596146; // 1 / 1.230174104914
	double k2 = 0.61508705245700002; // 1.230174104914 / 2

	for (int col = 0; col < width; ++col)
	{
		//Predict 1. y1
		for (int row = 1; row < height - 1; row += 2)//������
		{
			matrix[row][col] += a1 * (matrix[row - 1][col] + matrix[row + 1][col]);
		}
		matrix[height - 1][col] += 2 * a1 * matrix[height - 2][col];

		//Update 1. y0
		for (int row = 2; row < height; row += 2)//ż����
		{
			matrix[row][col] += a2 * (matrix[row - 1][col] + matrix[row + 1][col]);//����ע�ⲻҪԽ��
		}
		matrix[0][col] += 2 * a2 * matrix[1][col];
		
		//Predict 2.
		for (int row = 1; row < height - 1; row += 2)//������
		{
			matrix[row][col] += a3 * (matrix[row - 1][col] + matrix[row + 1][col]);
		}
		matrix[height - 1][col] += 2 * a3 * matrix[height - 2][col];
		
		//Updata 2.
		for (int row = 2; row < height; row += 2)//ż����
		{
			matrix[row][col] += a4 * (matrix[row - 1][col] + matrix[row + 1][col]);//
		}
		matrix[0][col] += 2 * a4 * matrix[1][col];
	}

	double **temp;
	createMatrix(temp, Size(width, height));
	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			if (row % 2 == 0)
				temp[col][row / 2] = k1 * matrix[row][col];
			else
				temp[col][row / 2 + height / 2] = k2 * matrix[row][col];
		}
	}
	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			matrix[row][col] = temp[row][col];
		}
	}

	releaseMatrix(temp, Size(width, height));

	return matrix;
}

Mat fwt97_2d(Mat image, int nlevels)
{
	int iWidth = image.rows, iHeight = image.cols;
	double **matrix;
	createMatrix(matrix, image.size());
	
	//convert mat to 2d matrix
	const uchar *ix;
	for (int row = 0; row < iHeight; ++row)
	{
		ix = image.ptr<uchar>(row);
		for (int col = 0; col < iWidth; ++col)
		{
			matrix[row][col] = double(uchar(ix[col]));
		}
	}

	int width = iWidth, height = iHeight;
	//do the wavelet decompose
	for (int i = 0; i < nlevels; ++i)
	{
		matrix = fwt97(matrix, width, height);
		matrix = fwt97(matrix, width, height);
		width /= 2;
		height /= 2; 
	}

#ifdef SHOW_WAVELET
	Mat im1(image.size(), CV_8UC1);
	for (int row = 0; row < iHeight; ++row)
	{
		for (int col = 0; col < iWidth; ++col)
		{
			if (matrix[row][col] < 0)
				im1.at<uchar>(row, col) = 0;
			else if (matrix[row][col] > 255)
				im1.at<uchar>(row, col) = 255;
			else
				im1.at<uchar>(row, col) = uchar(matrix[row][col]);
		}
	}
	imshow("97wavelet", im1);
#endif
	//multiple the CSF coefficient with different frequence band
	double csf[4] = { 2.16, 2.87, 3.16, 2.56 };
	for (int i = 0; i < nlevels; ++i)
	{
		int tHeight = 0, tWidth = 0;
		for (int row = tHeight; row < height; ++row)
		{
			for (int col = tWidth ; col < width; ++col)
			{
				matrix[row][col] = csf[i] * matrix[row][col];
			}
		}
		tWidth = width;
		tHeight = height;
		width *= 2;
		height *= 2;
	}
	
	Mat im(image.size(), CV_64FC1);
	for (int row = 0; row < iHeight; ++row)
	{
		double * dm = im.ptr<double>(row);
		for (int col = 0; col < iWidth; ++col)
		{
			dm[col] = matrix[row][col];
		}
	}
	releaseMatrix(matrix, image.size());
	return im;
}

double getHVSNR(const Mat &test, const Mat &reference, int nlevels)
{
	Mat _test(test.size(), CV_64FC1), _ref(reference.size(), CV_64FC1);
	_test = fwt97_2d(test, nlevels);
	_ref = fwt97_2d(reference, nlevels);

	//Minkovski nonlinear summation
	Mat diff(test.size(), CV_64FC1), powImg(test.size(), CV_64FC1);
	absdiff(_test, _ref, diff);
	pow(diff, 4, powImg);
	
	double temp = 0;
	temp = mean(diff).val[0];
	temp = pow(temp, 1. / 4);

	return 10 * log10(255*255 / (temp+0.000001)); //add a small number avoiding the divider to be zero
}

int main()
{
	double mse;
	double psnr;
	double ssim;
	double contour_volume;
	double nr;
	double smd2;
	double hvsnr;
	string testName = "test_512.png", refName = "test_512.png";

	Mat test = imread(testName, CV_LOAD_IMAGE_GRAYSCALE);
	if (test.empty())
	{
		cout << "ERROR: can not load the tested image!" << endl;
		return -1;
	}

	Mat reference = imread(refName, CV_LOAD_IMAGE_GRAYSCALE);
	if (reference.empty())
	{
		cout << "ERROR: can not load the reference image!" << endl;
		return -1;
	}
	try
	{
		clock_t start_t, end_t;
		start_t = clock();
		mse = getMSE(test, reference);
		end_t = clock();
		cout << "the mean square error of the image is " << mse << "\nthe time is " << end_t - start_t << "ms" << endl;

		start_t = clock();
		psnr = getPSNR(test, mse);
		end_t = clock();
		cout << "the peak signal to noise ratio is " << psnr << "dB\nthe time is " << end_t - start_t << "ms" << endl;

		start_t = clock() / CLOCKS_PER_SEC;
		ssim = getSSIM(test, reference);
		end_t = clock() / CLOCKS_PER_SEC;
		cout << "the structural similarity index is " << ssim << "\nthe processed time is " << (end_t - start_t) * 1000 << "ms" << endl;

		start_t = clock();
		contour_volume = getCV(test);
		end_t = clock();
		cout << "the contour volume is " << contour_volume << "\nthe processed time is " << end_t - start_t << "ms" << endl;

		start_t = clock();
		nr = getNR(test);
		end_t = clock();
		cout << "the nosie rate is " << nr << "\nthe processed time is " << end_t - start_t << "ms" << endl;

		start_t = clock();
		smd2 = getSMD2(test);
		end_t = clock();
		cout << "the definition is " << smd2 << "\nthe processed time is " << end_t - start_t << "ms" << endl;

		start_t = clock();
		hvsnr = getHVSNR(test, reference, 4);
		end_t = clock();
		cout << "the human vision system noise rate is " << hvsnr << "\nthe processed time is " << end_t - start_t << "ms" << endl;

		imshow(testName, test);
		imshow(refName, reference);
		waitKey(0);
	}
	catch (cv::Exception &e)
	{ 
		const char* err_msg = e.what();
		cout << "Exception caught " << err_msg << endl;
	}
	

	return 0;
}

////-----------------------------------������˵����----------------------------------------------
////	     ��������:������OpenCV���Ž̳�֮�š��������˲�ר������ֵ�˲���˫���˲�  �� ��������Դ��
////	     ��������IDE�汾��Visual Studio 2010
////	   ��������OpenCV�汾�� 2.4.8
////	     2014��4��8�� Create by ǳī
////------------------------------------------------------------------------------------------------
//
////-----------------------------------��ͷ�ļ��������֡�---------------------------------------
////	     ����������������������ͷ�ļ�
////----------------------------------------------------------------------------------------------
//#include <opencv2/core/core.hpp>
//#include<opencv2/highgui/highgui.hpp>
//#include<opencv2/imgproc/imgproc.hpp>
//#include <iostream>
//
////-----------------------------------�������ռ��������֡�---------------------------------------
////	     ����������������ʹ�õ������ռ�
////----------------------------------------------------------------------------------------------- 
//using namespace std;
//using namespace cv;
//
//
//-----------------------------------��ȫ�ֱ����������֡�--------------------------------------
//	     ������ȫ�ֱ�������
//-----------------------------------------------------------------------------------------------
//Mat g_srcImage, g_dstImage1, g_dstImage2, g_dstImage3, g_dstImage4, g_dstImage5;
//int g_nBoxFilterValue = 12;  //�����˲��ں�ֵ
//int g_nMeanBlurValue = 12;  //��ֵ�˲��ں�ֵ
//int g_nGaussianBlurValue =6;  //��˹�˲��ں�ֵ
//int g_nMedianBlurValue = 6;  //��ֵ�˲�����ֵ
//int g_nBilateralFilterValue = 13;  //˫���˲�����ֵ
//
//
////-----------------------------------��ȫ�ֺ����������֡�--------------------------------------
////	     ������ȫ�ֺ�������
////-----------------------------------------------------------------------------------------------
////�켣���ص�����
//static void on_BoxFilter(int, void *);	     //�����˲�
//static void on_MeanBlur(int, void *);	    //��ֵ���˲���
//static void on_GaussianBlur(int, void *);		      //��˹�˲���
//static void on_MedianBlur(int, void *);		 //��ֵ�˲���
//static void on_BilateralFilter(int, void*);		      //˫���˲���
//
//
////-----------------------------------��main( )������--------------------------------------------
////	     ����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ
////-----------------------------------------------------------------------------------------------
//int main()
//{
//	double psnr, cv, mse;
//	clock_t start_t, end_t;
//	system("color 5E");
//
//	//����ԭͼ
//	Mat g_std = imread("Einstein.jpg", 0);
//	g_srcImage = imread("noise.jpg", 0);
//	if (!g_srcImage.data) { printf("Oh��no����ȡsrcImage����~��\n"); return false; }
//
//	//��¡ԭͼ���ĸ�Mat������
//	g_dstImage1 = g_srcImage.clone();
//	g_dstImage2 = g_srcImage.clone();
//	g_dstImage3 = g_srcImage.clone();
//	g_dstImage4 = g_srcImage.clone();
//	g_dstImage5 = g_srcImage.clone();
//
//	//��ʾԭͼ
//	namedWindow("��<0>ԭͼ���ڡ�", 1);
//	imshow("��<0>ԭͼ���ڡ�", g_srcImage);
//
//	mse = getMSE(g_srcImage, g_std);
//	psnr = getPSNR(g_srcImage, mse);
//	cv = getCV(g_srcImage);
//	cout << "ԭͼPSNR= " << psnr << ", CV= " << cv << endl;
//
//	//=================��<1>�����˲���=========================
//	//��������
//	namedWindow("��<1>�����˲���", 1);
//	//�����켣��		
//	start_t = clock();
//	createTrackbar("�ں�ֵ��", "��<1>�����˲���", &g_nBoxFilterValue, 50, on_BoxFilter);
//	on_BoxFilter(g_nBoxFilterValue, 0);
//	end_t = clock();
//	//imshow("��<1>�����˲���", g_dstImage1);
//	//=====================================================
//	mse = getMSE(g_dstImage1, g_std);
//	psnr = getPSNR(g_dstImage1, mse);
//	cv = getCV(g_dstImage1);
//	cout << "�����˲�PSNR= " << psnr << ", CV= " << cv <<", time="<<end_t-start_t<< endl;
//
//	//=================��<2>��ֵ�˲���==========================
//	//��������
//	namedWindow("��<2>��ֵ�˲���", 1);
//	//�����켣��
//	start_t = clock();
//	createTrackbar("�ں�ֵ��", "��<2>��ֵ�˲���", &g_nMeanBlurValue, 50, on_MeanBlur);
//	on_MeanBlur(g_nMeanBlurValue, 0);
//	end_t = clock();
//	//======================================================
//	mse = getMSE(g_dstImage2, g_std);
//	psnr = getPSNR(g_dstImage2, mse);
//	cv = getCV(g_dstImage2);
//	cout << "��ֵ�˲�PSNR= " << psnr << ", CV= " << cv << ", time=" << end_t - start_t << endl;
//
//	//=================��<3>��˹�˲���===========================
//	//��������
//	namedWindow("��<3>��˹�˲���", 1);
//	//�����켣��
//	start_t = clock();
//	createTrackbar("�ں�ֵ��", "��<3>��˹�˲���", &g_nGaussianBlurValue, 50, on_GaussianBlur);
//	on_GaussianBlur(g_nGaussianBlurValue, 0);
//	end_t = clock();
//	//=======================================================
//	mse = getMSE(g_dstImage3, g_std);
//	psnr = getPSNR(g_dstImage3, mse);
//	cv = getCV(g_dstImage3);
//	cout << "��˹�˲�PSNR= " << psnr << ", CV= " << cv << ", time=" << end_t - start_t << endl;
//
//	//=================��<4>��ֵ�˲���===========================
//	//��������
//	namedWindow("��<4>��ֵ�˲���", 1);
//	//�����켣��
//	start_t = clock();
//	createTrackbar("����ֵ��", "��<4>��ֵ�˲���", &g_nMedianBlurValue, 50, on_MedianBlur);
//	on_MedianBlur(g_nMedianBlurValue, 0);
//	end_t = clock();
//	//=======================================================
//	mse = getMSE(g_dstImage4, g_std);
//	psnr = getPSNR(g_dstImage4, mse);
//	cv = getCV(g_dstImage4);
//	cout << "��ֵ�˲�PSNR= " << psnr << ", CV= " << cv << ", time=" << end_t - start_t << endl;
//
//	//=================��<5>˫���˲���===========================
//	//��������
//	namedWindow("��<5>˫���˲���", 1);
//	//�����켣��
//	start_t = clock();
//	createTrackbar("����ֵ��", "��<5>˫���˲���", &g_nBilateralFilterValue, 50, on_BilateralFilter);
//	on_BilateralFilter(g_nBilateralFilterValue, 0);
//	end_t = clock();
//	//=======================================================
//	mse = getMSE(g_dstImage5, g_std);
//	psnr = getPSNR(g_dstImage5, mse);
//	cv = getCV(g_dstImage5);
//	cout << "˫���˲�PSNR= " << psnr << ", CV= " << cv << ", time=" << end_t - start_t << endl;
//
//	//���һЩ������Ϣ
//	cout << endl << "\t�š����ˣ�������������۲�ͼ��Ч��~\n\n"
//		<< "\t���¡�q����ʱ�������˳�~!\n"
//		<< "\n\n\t\t\t\tbyǳī";
//
//	while (char(waitKey(1)) != 'q') {}
//
//	return 0;
//}
//
////-----------------------------��on_BoxFilter( )������------------------------------------
////	     �����������˲������Ļص�����
////-----------------------------------------------------------------------------------------------
//static void on_BoxFilter(int, void *)
//{
//	//�����˲�����
//	boxFilter(g_srcImage, g_dstImage1, -1, Size(g_nBoxFilterValue + 1, g_nBoxFilterValue + 1));
//	//��ʾ����
//	imshow("��<1>�����˲���", g_dstImage1);
//}
//
////-----------------------------��on_MeanBlur( )������------------------------------------
////	     ��������ֵ�˲������Ļص�����
////-----------------------------------------------------------------------------------------------
//static void on_MeanBlur(int, void *)
//{
//	blur(g_srcImage, g_dstImage2, Size(g_nMeanBlurValue + 1, g_nMeanBlurValue + 1), Point(-1, -1));
//	imshow("��<2>��ֵ�˲���", g_dstImage2);
//
//}
//
////-----------------------------��on_GaussianBlur( )������------------------------------------
////	     ��������˹�˲������Ļص�����
////-----------------------------------------------------------------------------------------------
//static void on_GaussianBlur(int, void *)
//{
//	GaussianBlur(g_srcImage, g_dstImage3, Size(g_nGaussianBlurValue * 2 + 1, g_nGaussianBlurValue * 2 + 1), 0, 0);
//	imshow("��<3>��˹�˲���", g_dstImage3);
//}
//
//
////-----------------------------��on_MedianBlur( )������------------------------------------
////	     ��������ֵ�˲������Ļص�����
////-----------------------------------------------------------------------------------------------
//static void on_MedianBlur(int, void *)
//{
//	medianBlur(g_srcImage, g_dstImage4, g_nMedianBlurValue * 2 + 1);
//	imshow("��<4>��ֵ�˲���", g_dstImage4);
//}
//
//
////-----------------------------��on_BilateralFilter( )������------------------------------------
////	     ������˫���˲������Ļص�����
////-----------------------------------------------------------------------------------------------
//static void on_BilateralFilter(int, void *)
//{
//	bilateralFilter(g_srcImage, g_dstImage5, g_nBilateralFilterValue, g_nBilateralFilterValue * 2, g_nBilateralFilterValue / 2);
//	imshow("��<5>˫���˲���", g_dstImage5);
//}