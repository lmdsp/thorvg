/*
 * Copyright (c) 2020-2021 Samsung Electronics Co., Ltd. All rights reserved.

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
#include <float.h>
#include <math.h>
#include "tvgMath.h"
#include "tvgPaint.h"

/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/


static bool _compFastTrack(Paint* cmpTarget, const RenderTransform* pTransform, RenderTransform* rTransform, RenderRegion& viewport)
{
    /* Access Shape class by Paint is bad... but it's ok still it's an internal usage. */
    auto shape = static_cast<Shape*>(cmpTarget);

    //Rectangle Candidates?
    const Point* pts;
     if (shape->pathCoords(&pts) != 4) return false;

    if (rTransform) rTransform->update();

    //No Rotation?
    if (pTransform && !mathRightAngle(&pTransform->m)) return false;
    if (rTransform && !mathRightAngle(&rTransform->m)) return false;

    //Perpendicular Rectangle?
    auto pt1 = pts + 0;
    auto pt2 = pts + 1;
    auto pt3 = pts + 2;
    auto pt4 = pts + 3;

    if ((mathEqual(pt1->x, pt2->x) && mathEqual(pt2->y, pt3->y) && mathEqual(pt3->x, pt4->x) && mathEqual(pt1->y, pt4->y)) ||
        (mathEqual(pt2->x, pt3->x) && mathEqual(pt1->y, pt2->y) && mathEqual(pt1->x, pt4->x) && mathEqual(pt3->y, pt4->y))) {

        auto x1 = pt1->x;
        auto y1 = pt1->y;
        auto x2 = pt3->x;
        auto y2 = pt3->y;

        if (rTransform) {
            x1 = x1 * rTransform->m.e11 + rTransform->m.e13;
            y1 = y1 * rTransform->m.e22 + rTransform->m.e23;
            x2 = x2 * rTransform->m.e11 + rTransform->m.e13;
            y2 = y2 * rTransform->m.e22 + rTransform->m.e23;
        }

        if (pTransform) {
            x1 = x1 * pTransform->m.e11 + pTransform->m.e13;
            y1 = y1 * pTransform->m.e22 + pTransform->m.e23;
            x2 = x2 * pTransform->m.e11 + pTransform->m.e13;
            y2 = y2 * pTransform->m.e22 + pTransform->m.e23;
        }

        if (x1 < 0.0f) x1 = 0.0f;
        if (y1 < 0.0f) y1 = 0.0f;

        viewport.x = static_cast<uint32_t>(x1);
        viewport.y = static_cast<uint32_t>(y1);
        viewport.w = static_cast<uint32_t>(x2 - x1 < 0 ? 0 : roundf(x2 - x1 + 0.5f));
        viewport.h = static_cast<uint32_t>(x2 - x1 < 0 ? 0 : roundf(y2 - y1 + 0.5f));

        return true;
    }

    return false;
}


Paint* Paint::Impl::duplicate()
{
    auto ret = smethod->duplicate();
    if (!ret) return nullptr;

    //duplicate Transform
    if (rTransform) {
        ret->pImpl->rTransform = new RenderTransform();
        *ret->pImpl->rTransform = *rTransform;
        ret->pImpl->renderFlag |= RenderUpdateFlag::Transform;
    }

    ret->pImpl->opacity = opacity;

    if (cmpTarget) ret->pImpl->cmpTarget = cmpTarget->duplicate();

    ret->pImpl->cmpMethod = cmpMethod;

    return ret;
}


bool Paint::Impl::rotate(float degree)
{
    if (rTransform) {
        if (mathEqual(degree, rTransform->degree)) return true;
    } else {
        if (mathZero(degree)) return true;
        rTransform = new RenderTransform();
    }
    rTransform->degree = degree;
    if (!rTransform->overriding) renderFlag |= RenderUpdateFlag::Transform;

    return true;
}


bool Paint::Impl::scale(float factor)
{
    if (rTransform) {
        if (mathEqual(factor, rTransform->scale)) return true;
    } else {
        if (mathZero(factor)) return true;
        rTransform = new RenderTransform();
    }
    rTransform->scale = factor;
    if (!rTransform->overriding) renderFlag |= RenderUpdateFlag::Transform;

    return true;
}


bool Paint::Impl::translate(float x, float y)
{
    if (rTransform) {
        if (mathEqual(x, rTransform->x) && mathEqual(y, rTransform->y)) return true;
    } else {
        if (mathZero(x) && mathZero(y)) return true;
        rTransform = new RenderTransform();
    }
    rTransform->x = x;
    rTransform->y = y;
    if (!rTransform->overriding) renderFlag |= RenderUpdateFlag::Transform;

    return true;
}


bool Paint::Impl::render(RenderMethod& renderer)
{
    Compositor* cmp = nullptr;

    //OPTIMIZE_ME: Can we replace the simple AlphaMasking with ClipPath?

    /* Note: only ClipPath is processed in update() step.
        Create a composition image. */
    if (cmpTarget && cmpMethod != CompositeMethod::ClipPath && !(cmpTarget->pImpl->ctxFlag & ContextFlag::FastTrack)) {
        auto region = smethod->bounds(renderer);
        if (region.w == 0 || region.h == 0) return false;
        cmp = renderer.target(region);
        renderer.beginComposite(cmp, CompositeMethod::None, 255);
        cmpTarget->pImpl->render(renderer);
    }

    if (cmp) renderer.beginComposite(cmp, cmpMethod, cmpTarget->pImpl->opacity);

    auto ret = smethod->render(renderer);

    if (cmp) renderer.endComposite(cmp);

    return ret;
}


void* Paint::Impl::update(RenderMethod& renderer, const RenderTransform* pTransform, uint32_t opacity, Array<RenderData>& clips, uint32_t pFlag)
{
    if (renderFlag & RenderUpdateFlag::Transform) {
        if (!rTransform) return nullptr;
        if (!rTransform->update()) {
            delete(rTransform);
            rTransform = nullptr;
        }
    }

    /* 1. Composition Pre Processing */
    void *cmpData = nullptr;
    RenderRegion viewport;
    bool cmpFastTrack = false;

    if (cmpTarget) {
        /* If transform has no rotation factors && ClipPath / AlphaMasking is a simple rectangle,
           we can avoid regular ClipPath / AlphaMasking sequence but use viewport for performance */
        auto tryFastTrack = false;
        if (cmpMethod == CompositeMethod::ClipPath) tryFastTrack = true;
        else if (cmpMethod == CompositeMethod::AlphaMask && cmpTarget->identifier() == TVG_CLASS_ID_SHAPE) {
            auto shape = static_cast<Shape*>(cmpTarget);
            uint8_t a;
            shape->fillColor(nullptr, nullptr, nullptr, &a);
            if (a == 255 && shape->opacity() == 255 && !shape->fill()) tryFastTrack = true;
        }
        if (tryFastTrack) {
            RenderRegion viewport2;
            if ((cmpFastTrack = _compFastTrack(cmpTarget, pTransform, cmpTarget->pImpl->rTransform, viewport2))) {
                viewport = renderer.viewport();
                viewport2.intersect(viewport);
                renderer.viewport(viewport2);
                cmpTarget->pImpl->ctxFlag |= ContextFlag::FastTrack;
            } else {
                cmpTarget->pImpl->ctxFlag &= ~ContextFlag::FastTrack;
            }
        }
        if (!cmpFastTrack) {
            cmpData = cmpTarget->pImpl->update(renderer, pTransform, 255, clips, pFlag);
            if (cmpMethod == CompositeMethod::ClipPath) clips.push(cmpData);
        }
    }

    /* 2. Main Update */
    void *edata = nullptr;
    auto newFlag = static_cast<RenderUpdateFlag>(pFlag | renderFlag);
    renderFlag = RenderUpdateFlag::None;
    opacity = (opacity * this->opacity) / 255;

    if (rTransform && pTransform) {
        RenderTransform outTransform(pTransform, rTransform);
        edata = smethod->update(renderer, &outTransform, opacity, clips, newFlag);
    } else {
        auto outTransform = pTransform ? pTransform : rTransform;
        edata = smethod->update(renderer, outTransform, opacity, clips, newFlag);
    }

    /* 3. Composition Post Processing */
    if (cmpFastTrack) renderer.viewport(viewport);
    else if (cmpData && cmpMethod == CompositeMethod::ClipPath) clips.pop();

    return edata;
}

bool Paint::Impl::bounds(float* x, float* y, float* w, float* h, bool transformed)
{
    Matrix* m = nullptr;

    //Case: No transformed, quick return!
    if (!transformed || !(m = this->transform())) return smethod->bounds(x, y, w, h);

    //Case: Transformed
    auto tx = 0.0f;
    auto ty = 0.0f;
    auto tw = 0.0f;
    auto th = 0.0f;

    auto ret = smethod->bounds(&tx, &ty, &tw, &th);

    //Get vertices
    Point pt[4] = {{tx, ty}, {tx + tw, ty}, {tx + tw, ty + th}, {tx, ty + th}};

    //New bounding box
    auto x1 = FLT_MAX;
    auto y1 = FLT_MAX;
    auto x2 = -FLT_MAX;
    auto y2 = -FLT_MAX;

    //Compute the AABB after transformation
    for (int i = 0; i < 4; i++) {
        mathMultiply(&pt[i], m);

        if (pt[i].x < x1) x1 = pt[i].x;
        if (pt[i].x > x2) x2 = pt[i].x;
        if (pt[i].y < y1) y1 = pt[i].y;
        if (pt[i].y > y2) y2 = pt[i].y;
    }

    if (x) *x = x1;
    if (y) *y = y1;
    if (w) *w = x2 - x1;
    if (h) *h = y2 - y1;

    return ret;
}


/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

Paint :: Paint() : pImpl(new Impl())
{
}


Paint :: ~Paint()
{
    delete(pImpl);
}


Result Paint::rotate(float degree) noexcept
{
    if (pImpl->rotate(degree)) return Result::Success;
    return Result::FailedAllocation;
}


Result Paint::scale(float factor) noexcept
{
    if (pImpl->scale(factor)) return Result::Success;
    return Result::FailedAllocation;
}


Result Paint::translate(float x, float y) noexcept
{
    if (pImpl->translate(x, y)) return Result::Success;
    return Result::FailedAllocation;
}


Result Paint::transform(const Matrix& m) noexcept
{
    if (pImpl->transform(m)) return Result::Success;
    return Result::FailedAllocation;
}


Matrix Paint::transform() noexcept
{
    auto pTransform = pImpl->transform();
    if (pTransform) return *pTransform;
    return {1, 0, 0, 0, 1, 0, 0, 0, 1};
}


TVG_DEPRECATED Result Paint::bounds(float* x, float* y, float* w, float* h) const noexcept
{
    return this->bounds(x, y, w, h, false);
}


Result Paint::bounds(float* x, float* y, float* w, float* h, bool transform) const noexcept
{
    if (pImpl->bounds(x, y, w, h, transform)) return Result::Success;
    return Result::InsufficientCondition;
}


Paint* Paint::duplicate() const noexcept
{
    return pImpl->duplicate();
}


Result Paint::composite(std::unique_ptr<Paint> target, CompositeMethod method) noexcept
{
    auto p = target.release();
    if (pImpl->composite(p, method)) return Result::Success;
    if (p) delete(p);
    return Result::InvalidArguments;
}


CompositeMethod Paint::composite(const Paint** target) const noexcept
{
    if (target) *target = pImpl->cmpTarget;

    return pImpl->cmpMethod;
}


Result Paint::opacity(uint8_t o) noexcept
{
    if (pImpl->opacity == o) return Result::Success;

    pImpl->opacity = o;
    pImpl->renderFlag |= RenderUpdateFlag::Color;

    return Result::Success;
}


uint8_t Paint::opacity() const noexcept
{
    return pImpl->opacity;
}
