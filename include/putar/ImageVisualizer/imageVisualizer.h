#ifndef _IMAGEVISUALIZER_H_
#define _IMAGEVISUALIZER_H_

#include "../Defs/defs.h"
#include "Utilities/observer.h"
#include <iostream>
#include <string>

namespace putar{
	class ImageVisualizer{
		public:
		
			ImageVisualizer() {}
			
			// update the Cloud
			virtual void updateCloud(cv::Mat RGBD) = 0;
			
			// update Mask
			virtual void updateImage(cv::Mat Mask) = 0; 
			
			// Virtual destructor
			virtual ~ImageVisualizer(){}
	};
}

#endif
