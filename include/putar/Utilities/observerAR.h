
#ifndef OBSERVERAR_H_
#define OBSERVERAR_H_

#include "Defs/defs.h"
#include <vector>
#include <list>
class ObserverAR
{
public:
    virtual void update(const putar::Mat34& objectState) = 0;
    virtual void updateCamera(const putar::Mat34& cameraState) = 0;
};

class SubjectAR
{
    //Lets keep a track of all the observers we observe
    std::vector<ObserverAR*> list;

public:
    void attach(ObserverAR *observer);
    void detach(ObserverAR *observer);
    void notify(const putar::Mat34& objectState);
    void notifyCamera(const putar::Mat34& cameraState);
};

#endif // OBSERVERAR_H_
