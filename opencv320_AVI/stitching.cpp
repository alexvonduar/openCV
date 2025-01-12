/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include <iostream>
#include <fstream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"

using namespace std;
using namespace cv;

bool try_use_gpu = false;
Stitcher::Mode mode = Stitcher::PANORAMA; //Stitcher::SCANS; //Stitcher::PANORAMA;
vector<String> img_names;
vector<Mat> imgs;
string result_name = "imgStichingResult.jpg";

void printUsage();
int parseCmdArgs(int argc, char** argv);

#define ENABLE_LOG 1
#define LOG(msg) std::cout << msg
#define LOGLN(msg) std::cout << msg << std::endl

int main(int argc, char* argv[])
{
	int retval = parseCmdArgs(argc, argv);
	if (retval) return -1;

	//img_names.push_back("D:\\VisualStudioProjects\\videostitchings\\opencv_extra\\testdata\\stitching\\boat1.jpg");
	//img_names.push_back("D:\\VisualStudioProjects\\videostitchings\\opencv_extra\\testdata\\stitching\\boat2.jpg");
	//img_names.push_back("D:\\VisualStudioProjects\\videostitchings\\opencv_extra\\testdata\\stitching\\boat3.jpg");
	//img_names.push_back("D:\\VisualStudioProjects\\videostitchings\\opencv_extra\\testdata\\stitching\\boat4.jpg");
	//img_names.push_back("D:\\VisualStudioProjects\\videostitchings\\opencv_extra\\testdata\\stitching\\boat5.jpg");
	//img_names.push_back("D:\\VisualStudioProjects\\videostitchings\\opencv_extra\\testdata\\stitching\\boat6.jpg");


	//img_names.push_back("C:\\Users\\Junyue\\WebDownload\\LiveSnapshot\\F1.bmp");
	//img_names.push_back("C:\\Users\\Junyue\\WebDownload\\LiveSnapshot\\EL1.bmp");
	//img_names.push_back("C:\\Users\\Junyue\\WebDownload\\LiveSnapshot\\ER1.bmp");
	//img_names.push_back("C:\\Users\\Junyue\\WebDownload\\LiveSnapshot\\E1.bmp");
	//img_names.push_back("C:\\Users\\Junyue\\WebDownload\\LiveSnapshot\\F1.bmp");
	//img_names.push_back("C:\\Users\\Junyue\\WebDownload\\LiveSnapshot\\E1.bmp");
	//img_names.push_back("C:\\Users\\Junyue\\WebDownload\\LiveSnapshot\\F1.bmp");
	//img_names.push_back("C:\\Users\\Junyue\\WebDownload\\LiveSnapshot\\E1.bmp");
	//img_names.push_back("C:\\Users\\Junyue\\WebDownload\\LiveSnapshot\\F1.bmp");
	//img_names.push_back("C:\\Users\\Junyue\\WebDownload\\LiveSnapshot\\E2.bmp");
	//img_names.push_back("C:\\Users\\Junyue\\WebDownload\\LiveSnapshot\\F2.bmp");
	img_names.push_back("C:\\Users\\lugang\\WebDownload\\LiveSnapshot\\A1.bmp");
	img_names.push_back("C:\\Users\\lugang\\WebDownload\\LiveSnapshot\\A2.bmp");


#if ENABLE_LOG
	int64 t = getTickCount();
#endif

	LOGLN("Loading images");

	for (int i = 0; i < img_names.size(); i++){
		Mat img = imread(img_names[i]);
		if (img.empty())
		{
			cout << "Can't read image '" << img_names[i] << "'\n";
			return -1;
		}
		imgs.push_back(img);
	}



	LOGLN("Loaded images, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");
	t = getTickCount();

	Mat pano;
	Ptr<Stitcher> stitcher = Stitcher::create(mode, try_use_gpu);
	stitcher->setPanoConfidenceThresh(0.01f);
	stitcher->setWarper(makePtr<cv::PlaneWarper>());
	Stitcher::Status status = stitcher->stitch(imgs, pano);

	if (status != Stitcher::OK)
	{
		cout << "Can't stitch images, error code = " << int(status) << endl;
		return -1;
	}

	LOGLN("Pairwise matching, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");

	imwrite(result_name, pano);
	imshow("result", pano);

	waitKey(8000);

	return 0;
}


void printUsage()
{
	cout <<
		"Images stitcher.\n\n"
		"stitching img1 img2 [...imgN]\n\n"
		"Flags:\n"
		"  --try_use_gpu (yes|no)\n"
		"      Try to use GPU. The default value is 'no'. All default values\n"
		"      are for CPU mode.\n"
		"  --mode (panorama|scans)\n"
		"      Determines configuration of stitcher. The default is 'panorama',\n"
		"      mode suitable for creating photo panoramas. Option 'scans' is suitable\n"
		"      for stitching materials under affine transformation, such as scans.\n"
		"  --output <result_img>\n"
		"      The default is 'result.jpg'.\n";
}


int parseCmdArgs(int argc, char** argv)
{
	/*
	if (argc == 1)
	{
	printUsage();
	return -1;
	}
	*/
	for (int i = 1; i < argc; ++i)
	{
		if (string(argv[i]) == "--help" || string(argv[i]) == "/?")
		{
			printUsage();
			return -1;
		}
		else if (string(argv[i]) == "--try_use_gpu")
		{
			if (string(argv[i + 1]) == "no")
				try_use_gpu = false;
			else if (string(argv[i + 1]) == "yes")
				try_use_gpu = true;
			else
			{
				cout << "Bad --try_use_gpu flag value\n";
				return -1;
			}
			i++;
		}
		else if (string(argv[i]) == "--output")
		{
			result_name = argv[i + 1];
			i++;
		}
		else if (string(argv[i]) == "--mode")
		{
			if (string(argv[i + 1]) == "panorama")
				mode = Stitcher::PANORAMA;
			else if (string(argv[i + 1]) == "scans")
				mode = Stitcher::SCANS;
			else
			{
				cout << "Bad --mode flag value\n";
				return -1;
			}
			i++;
		}
		else
		{
			Mat img = imread(argv[i]);
			if (img.empty())
			{
				cout << "Can't read image '" << argv[i] << "'\n";
				return -1;
			}
			imgs.push_back(img);
		}
	}
	return 0;
}
