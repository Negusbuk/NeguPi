/****************************************************************************
 **
 **  Copyright (C) 2013, 2014 Andreas Mussgiller
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 **
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <thread>
#include <chrono>
#include <tuple>
#include <cmath>

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <stitching/detail/exposure_compensate.hpp>

typedef std::tuple<int,std::string,double,bool> imageInfo_t;

double getBrightness(const cv::Mat& image)
{
  cv::Mat hsv_image;
  cv::cvtColor(image, hsv_image, CV_BGR2HSV);

  int bins = 256;
  int histSize[] = { bins };
  // Set ranges for histogram bins
  float lranges[] = { 0, 256 };
  const float* ranges[] = { lranges };
  // create matrix for histogram
  cv::Mat hist;
  int channels[] = { 2 };
  double sum = hsv_image.rows * hsv_image.cols;
  double mean = 0;

  cv::calcHist(&hsv_image, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);
  for (int b = 0;b<bins;b++) {
    mean += b * hist.at<float>(b);
  }

  return mean/sum;
}

int main(int argc, char * argv[])
{
  if (argc==1) return -1;

  std::vector<imageInfo_t> imageInfo;

  std::string filelist = argv[1];

  std::ifstream ifile(filelist);
  std::string filename;
  int i = 0;
  while (ifile >> filename) {
    cv::Mat image = cv::imread(filename.c_str(), CV_LOAD_IMAGE_COLOR);

    double brightness = getBrightness(image);

    std::cout << i << std::endl;
    imageInfo.push_back(std::make_tuple(i++, filename, brightness, true));

    //if (i==60) break;
  }

  std::vector<imageInfo_t>::iterator it = imageInfo.begin();
  ++it;
  std::vector<imageInfo_t>::iterator itend = imageInfo.end();
  --itend;

  for (;
       it!=itend;
       ++it) {
    imageInfo_t& info = *it;

    std::vector<imageInfo_t>::iterator itn = it;
    double mean = 0;
    int count = 0;
    for (int i=0;i<5;++i) {
      --itn;
      imageInfo_t& infon = *itn;
      if (std::get<3>(infon)) {
        mean += std::get<2>(infon);
        count++;
      }
      if (itn==imageInfo.begin()) break;
    }
    if (count>0) mean /= count;

    double value = std::get<2>(info);
    double delta = mean-value;
    if (std::fabs(delta)>1.5) {
      std::get<3>(info) = false;
      std::cout << std::get<1>(info) << "\t" << value << "\t" << delta << std::endl;

      std::vector<cv::Mat> images(3);

      {
        itn = it;
        itn--;
        imageInfo_t& infon = *itn;
        images[0] = cv::imread(std::get<1>(infon).c_str(), CV_LOAD_IMAGE_COLOR);
      }

      images[1] = cv::imread(std::get<1>(info).c_str(), CV_LOAD_IMAGE_COLOR);
      std::string filename_org = std::get<1>(info);
      filename_org.erase(filename_org.size()-4, 4);
      filename_org += "_org.jpg";
      std::cout << filename_org << std::endl;
      cv::imwrite(filename_org, images[1]);

      {
        itn = it;
        itn++;
        imageInfo_t& infon = *itn;
        images[2] = cv::imread(std::get<1>(infon).c_str(), CV_LOAD_IMAGE_COLOR);
      }

      cv::Ptr<cv::detail::ExposureCompensator> compensator =
          cv::detail::ExposureCompensator::createDefault(cv::detail::ExposureCompensator::GAIN);

      std::vector<cv::Point> corners(3);
      std::vector<cv::Mat> masks(3);

      for (int i=0;i<3;++i) {
        masks[i].create(images[i].size(), CV_8U);
        masks[i].setTo(cv::Scalar::all(255));

        corners[i] = cv::Point(10, 10);
      }

      compensator->feed(corners, images, masks);

      cv::Mat mask;
      mask.create(images[1].size(), CV_8U);
      mask.setTo(cv::Scalar::all(255));
      compensator->apply(1, corners[1], images[1], mask);

      double bn = getBrightness(images[0]);
      double b = getBrightness(images[1]);
      delta = bn - b;
      //std::cout << bn << "\t" << b << std::endl;

      cv::Mat image_new = images[1] + cv::Scalar(delta, delta, delta);
      //image.convertTo(image_new, -1, mean/value, 0);

      //std::cout << getBrightness(image) << "\t" << getBrightness(image_new) << std::endl;


      cv::imwrite(std::get<1>(info).c_str(), image_new);
    }
  }
  return 0;
}
