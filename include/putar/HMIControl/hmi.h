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

namespace putar{
	class Hmi{
        public:

            Hmi() {}

			virtual void updatePose(const Mat34& pose); 

            virtual ~Hmi(){}
	};
}

#endif
