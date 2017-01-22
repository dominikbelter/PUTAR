//CPP File
#include "Utilities/observerAR.h"
#include <algorithm>
#include <iostream>
using namespace std;
void SubjectAR::attach(ObserverAR *observer){
    list.push_back(observer);
}
void SubjectAR::detach(ObserverAR *observer){
    list.erase(std::remove(list.begin(), list.end(), observer), list.end());
}

void SubjectAR::notify(const putar::Mat34& objectState){
    for(vector<ObserverAR*>::const_iterator iter = list.begin(); iter != list.end(); ++iter)
    {
        if(*iter != 0) {
            (*iter)->update(objectState);
        }
    }
}

void SubjectAR::notifyCamera(const putar::Mat34& cameraState){
    for(vector<ObserverAR*>::const_iterator iter = list.begin(); iter != list.end(); ++iter)
    {
        if(*iter != 0) {
            (*iter)->updateCamera(cameraState);
        }
    }
}
