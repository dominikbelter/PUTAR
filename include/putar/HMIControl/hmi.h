/** @file hmi.h
 *
 * hmi interface
 *
 */
#ifndef _OBJLOADER_H_
#define _OBJLOADER_H_

#include "../Defs/defs.h"
#include "Utilities/observer.h"
#include <iostream>
#include <string>
#include <vector>
#include <mutex>

namaspace putar{
	class Hmi{
		public:					
			virtual void updatePose(const Mat34& pose); 
	};
}
