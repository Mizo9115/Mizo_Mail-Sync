//
//  ProgressCollectors.hpp
//  MailSync
//
//  Created by Ben Gotow on 7/5/17.
//  Copyright © 2017 Foundry 376. All rights reserved.
//
//  Use of this file is subject to the terms and conditions defined
//  in 'LICENSE.md', which is part of the Mizo Mail-Sync package.
//

#ifndef ProgressCollectors_hpp
#define ProgressCollectors_hpp

#include <stdio.h>
#include <chrono>
#include <MailCore/MailCore.h>

#include "Task.hpp"
#include "MailStore.hpp"

using namespace mailcore;

class IMAPProgress : public IMAPProgressCallback {
    Task * _task;
    MailStore * _store;
    unsigned int _lastReportedPercent;
    std::chrono::steady_clock::time_point _lastSaveTime;

public:
    IMAPProgress(Task * task = nullptr, MailStore * store = nullptr);
    void bodyProgress(IMAPSession * session, unsigned int current, unsigned int maximum);
    void itemsProgress(IMAPSession * session, unsigned int current, unsigned int maximum);
};

class SMTPProgress : public SMTPProgressCallback {
    Task * _task;
    MailStore * _store;
    unsigned int _lastReportedPercent;
    std::chrono::steady_clock::time_point _lastSaveTime;

public:
    SMTPProgress(Task * task = nullptr, MailStore * store = nullptr);
    void bodyProgress(SMTPSession * session, unsigned int current, unsigned int maximum);
};


#endif /* ProgressCollectors_hpp */
