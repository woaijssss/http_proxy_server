/*
 * WHPSHttpStatusCode.h
 *
 *  Created on: Oct 8, 2019
 *      Author: wenhan
 */

#ifndef __WHPSHTTPSTATUSCODE_H__
#define __WHPSHTTPSTATUSCODE_H__

#include "WHPSStatus.h"

class WHPSHttpStatusCode final: public WHPSStatus
{
public:
        WHPSHttpStatusCode() = delete;

        virtual ~WHPSHttpStatusCode() = delete;
};

#endif /* __WHPSHTTPSTATUSCODE_H__ */
