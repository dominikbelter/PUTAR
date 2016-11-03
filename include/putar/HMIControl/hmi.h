/** @file hmi.h
 *
 * hmi interface
 *
 */

#ifndef _HMI_H_
#define _HMI_H_

#include "../Defs/defs.h"
#include "Utilities/observer.h"
#include <iostream>
#include <string>
#include <vector>
#include <mutex>

namespace putar{
	class Hmi{
		public:					
			virtual void updatePose(const Mat34& pose); 
	};
}

#endif
