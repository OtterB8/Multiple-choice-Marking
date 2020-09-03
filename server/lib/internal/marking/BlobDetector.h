#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

#define Pi 3.14159265

using namespace std;
using namespace cv;

const int hx[9] = { 1, 1, 1, 0, 0, -1, -1, -1, 0 };
const int hy[9] = { 1, 0, -1, 1, -1, 1, 0, -1, 0 };
const double sqrt2 = sqrt(2);
const int l_img = 10;
const double l_sigma = 2;
const double R = 22.6274;

class BlobDetector
{
public:
	BlobDetector() {}
	~BlobDetector() {}
	
	bool LoadImage(const char* filename)
	{
		image = imread(filename, IMREAD_COLOR);

		if (!image.data)
			return false;

		row = image.rows;
		col = image.cols;

		return true;
	}

	Mat ShowImage(vector<char> answer, vector<pair<Point, pair<int, char>>> posOfAnswer)
	{
		for (int i = 0; i < answer.size(); i++)
			for (int j = 0; j < posOfAnswer.size(); j++)
			{
				pair<int, char> temp = posOfAnswer[j].second;
				if (i + 1 == temp.first && toupper(answer[i]) == toupper(temp.second))
				{
					circle(blobImage, posOfAnswer[j].first, R, Scalar(0, 0, 255), 2);
					break;
				}
			}

		return blobImage;
	}

	vector<Point> DetectBlob()
	{
		vector<Point> circles;
		if (!image.data)
			return circles;

		Mat grey, *laplace;
		double sigma[l_img];
		int offset = 1, row = image.rows, col = image.cols;

		blobImage = image.clone();

		cvtColor(image, grey, COLOR_RGB2GRAY);

		double maxP = 0;
		sigma[0] = l_sigma;
		for (int i = 1; i < l_img; i++)
			sigma[i] = sigma[i - 1] * sqrt2;

		laplace = LoG_convolve(grey, maxP, sigma);

		Mat tmp = Mat::zeros(grey.size(), DataType<double>::type);
		for (int r = 5; r < l_img - 1; r++)
			for (int i = offset; i < row - offset; i++)
				for (int j = offset; j < col - offset; j++)
					if (laplace[r].at<double>(i, j) > 0.3 * maxP && checkLocal(i, j, laplace[r], laplace[r - 1], laplace[r + 1])) {
						tmp.at<double>(i, j) = sigma[r];
					}

		for (int i = offset; i < row - offset; i++)
			for (int j = offset; j < col - offset; j++)
				if (tmp.at<double>(i, j) > 0 && tmp.at<double>(i, j)  * sqrt2 >= 22 && tmp.at<double>(i, j)  * sqrt2 <= 23)
					circles.push_back(Point(j, i));

		delete[] laplace;
		return circles;
	}

private:
	Mat image, blobImage;
	int row, col;

	bool checkLocal(int i, int j, Mat img, Mat down, Mat up)
	{
		double v = img.at<double>(i, j);

		if (checkLocalMax(v, i, j, img) && checkLocalMax(v, i, j, down) && checkLocalMax(v, i, j, up))
			return true;

		return false;
	}

	bool checkLocalMax(double v, int i, int j, Mat a)
	{
		int c = 0;
		for (int k = 0; k < 9; k++)
			if (v >= a.at<double>(i + hx[k], j + hy[k]))
				c++;
		return (c == 9);
	}

	Mat* LoG_convolve(Mat img, double& maxP, double* sigma)
	{
		Mat * res = new Mat[l_img], tmp = Mat::zeros(img.size(), DataType<double>::type);
		for (int i = 0; i < img.rows; i++)
			for (int j = 0; j < img.cols; j++)
				tmp.at<double>(i, j) = img.at<uchar>(i, j);

		for (int k = 0; k < l_img; k++) {
			Mat filter_log = LoG(sigma[k]);
			Mat image;
			filter2D(tmp, image, -1, filter_log);
			for (int i = 0; i < image.rows; i++)
				for (int j = 0; j < image.cols; j++) {
					image.at<double>(i, j) = pow(image.at<double>(i, j), 2);
					maxP = max(maxP, image.at<double>(i, j));
				}
			res[k] = image;
		}

		return res;
	}

	Mat LoG(double sigma)
	{
		int k = (int)ceil(sigma * 6);
		k -= k % 2;

		int n = k + 1;

		Mat res = Mat::zeros(Size(n, n), DataType<double>::type);
		Mat x = Mat::zeros(Size(n, 1), DataType<double>::type);
		Mat y = Mat::zeros(Size(1, n), DataType<double>::type);
		Mat x_filter = x.clone(), y_filter = y.clone();

		int p = 0;
		for (int i = -k / 2; i <= k / 2; i++) {
			x.at<double>(0, p) = i;
			y.at<double>(p++, 0) = i;
		}

		for (int i = 0; i < n; i++) {
			x_filter.at<double>(0, i) = exp(-(pow(x.at<double>(0, i), 2) / (2 * pow(sigma, 2))));
			y_filter.at<double>(i, 0) = exp(-(pow(y.at<double>(i, 0), 2) / (2 * pow(sigma, 2))));
		}

		for (int i = 0; i < res.rows; i++)
			for (int j = 0; j < res.cols; j++)
				res.at<double>(i, j) = (-(2 * pow(sigma, 2)) + (pow(x.at<double>(0, i), 2) + pow(y.at<double>(j, 0), 2))) *
				(x_filter.at<double>(0, i) * y_filter.at<double>(j, 0)) * (1 / (2 * Pi * pow(sigma, 4)));

		return res;
	}
};