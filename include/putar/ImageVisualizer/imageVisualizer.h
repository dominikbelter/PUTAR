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


            virtual cv::Mat (UpdateImage(cv::Mat mask, cv::Mat depthMask, cv::Mat RGB, cv::Mat depthImg)) = 0;
			
			// Virtual destructor
			virtual ~ImageVisualizer(){}
	};
}

#endif
