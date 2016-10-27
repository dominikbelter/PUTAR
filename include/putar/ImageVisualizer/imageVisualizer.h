#ifndef _IMAGEVISUALIZER_H_
#define _IMAGEVISUALIZER_H_

#include "../Defs/defs.h"
#include "Utilities/observer.h"
#include <iostream>
#include <string>
#include <vector>
#include <mutex>

namaspace putar{
	class imageVisualizer{
		public:
					
			virtual void updateCloud(cv::Mat RGBD) = 0;
			virtual void updateImage(cv::Mat Mask) = 0; 
	};
}

#endif