//
// Created by Daniel on 25.10.17.
//

#ifndef VCS_CHECKOUTACTION_H
#define VCS_CHECKOUTACTION_H

#include "Action.h"

#include "../VersionControl/Helper.h"

class CheckoutAction : public Action {
public:
    CheckoutAction(const Command &command);

    void execute();

};


#endif //VCS_CHECKOUTACTION_H
