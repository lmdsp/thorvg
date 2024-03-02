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

#include "tvgLottieJSEngine.h"

/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/

LottieObject* _find(LottieObject* target, const char* id)
{
    if (target->name && !strcmp(target->name, id)) return target;

    if (target->type != LottieObject::Type::Group && target->type != LottieObject::Type::Layer) return nullptr;

    //target has children, find recursively.
    auto group = static_cast<LottieGroup*>(target);

    for (auto c = group->children.begin(); c < group->children.end(); ++c) {
        if (auto ret = _find(*c, id)) return ret;
    }

    return nullptr;
}


/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

bool LottieJSEngine::interpret(LottieComposition* comp, LottieExpression* exp)
{
    TVGLOG("LOTTIE", "Interpret Expression");

    printf("%s\n", exp->src);

    auto p = exp->src;

    while (*p != '\0') {
        ++p;
    }

    auto shape1 = _find(exp->root, "Shape 1");
    if (!shape1) return false;

    auto path1 = _find(shape1, "Path 1");
    if (!path1) return false;

    //path overriding
    auto path = static_cast<LottiePath*>(path1);
    auto target = static_cast<LottiePathSet*>(exp->prop);
    *target = path->pathset;

    return true;
}