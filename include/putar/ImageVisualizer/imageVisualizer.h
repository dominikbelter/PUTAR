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
            virtual void updateFrame(cv::Mat RGBD, cv::Mat depthImg) = 0;
			
			// update Mask
            virtual void updateMask(cv::Mat Mask) = 0;
			
			// Virtual destructor
			virtual ~ImageVisualizer(){}
	};
}

#endif
