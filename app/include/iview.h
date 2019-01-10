#ifndef IVIEW_H
#define IVIEW_H

#include "imodel.h"

class IView {

public:
    virtual void setModel(const IModel&) = 0;
    virtual void update() = 0;
    virtual int exec() = 0;
    virtual ~IView() = default;
};

#endif // IVIEW_H
