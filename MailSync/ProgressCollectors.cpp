//
//  ProgressCollectors.cpp
//  MailSync
//
//  Created by Ben Gotow on 7/5/17.
//  Copyright © 2017 Foundry 376. All rights reserved.
//
//  Use of this file is subject to the terms and conditions defined
//  in 'LICENSE.md', which is part of the Mizo Mail-Sync package.
//

#include "ProgressCollectors.hpp"

using namespace std;
using namespace chrono;

static void maybeReportProgress(Task * task, MailStore * store, unsigned int current,
                                unsigned int maximum, unsigned int * lastReportedPercent,
                                steady_clock::time_point * lastSaveTime) {
    if (task == nullptr || store == nullptr || maximum == 0) {
        return;
    }

    unsigned int percent = (unsigned int)(((uint64_t)current * 100) / maximum);
    auto now = steady_clock::now();
    auto elapsedMs = duration_cast<milliseconds>(now - *lastSaveTime).count();

    // Throttle: every 5% or ~4 times per second, always allow 0% and 100%.
    if (percent != 0 && percent != 100 &&
        percent < *lastReportedPercent + 5 && elapsedMs < 250) {
        return;
    }
    if (percent == *lastReportedPercent && percent != 100) {
        return;
    }

    *lastReportedPercent = percent;
    *lastSaveTime = now;
    task->data()["progress"] = percent;
    store->save(task);
}

IMAPProgress::IMAPProgress(Task * task, MailStore * store)
: _task(task), _store(store), _lastReportedPercent(0), _lastSaveTime(steady_clock::now()) {
}

void IMAPProgress::bodyProgress(IMAPSession * session, unsigned int current, unsigned int maximum) {
    maybeReportProgress(_task, _store, current, maximum, &_lastReportedPercent, &_lastSaveTime);
}

void IMAPProgress::itemsProgress(IMAPSession * session, unsigned int current, unsigned int maximum) {
}

SMTPProgress::SMTPProgress(Task * task, MailStore * store)
: _task(task), _store(store), _lastReportedPercent(0), _lastSaveTime(steady_clock::now()) {
}

void SMTPProgress::bodyProgress(SMTPSession * session, unsigned int current, unsigned int maximum) {
    maybeReportProgress(_task, _store, current, maximum, &_lastReportedPercent, &_lastSaveTime);
}
