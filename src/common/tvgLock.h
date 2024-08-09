/*
 * Copyright (c) 2024 the ThorVG project. All rights reserved.

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _TVG_LOCK_H_
#define _TVG_LOCK_H_

#ifdef THORVG_THREAD_SUPPORT

#include <mutex>
#include "tvgTaskScheduler.h"

namespace tvg {

    struct Key
    {
        std::mutex mtx;
    };

    struct ScopedLock
    {
        Key* key = nullptr;

        ScopedLock(Key& k)
        {
            if (TaskScheduler::threads() > 0) {
                k.mtx.lock();
                key = &k;
            }
        }

        ~ScopedLock()
        {
            if (TaskScheduler::threads() > 0) {
                key->mtx.unlock();
            }
        }
    };

}

#else //THORVG_THREAD_SUPPORT

namespace tvg {

    struct Key {};

    struct ScopedLock
    {
        ScopedLock(Key& key) {}
    };

}

#endif //THORVG_THREAD_SUPPORT

#endif //_TVG_LOCK_H_

