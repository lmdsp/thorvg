/*
 * Copyright (c) 2023 - 2024 the ThorVG project. All rights reserved.

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

#include "Example.h"

/************************************************************************/
/* ThorVG Drawing Contents                                              */
/************************************************************************/

struct UserExample : tvgexam::Example
{
    bool content(tvg::Canvas* canvas, uint32_t w, uint32_t h) override
    {
        if (!canvas) return false;

        //Image source
        ifstream file(EXAMPLE_DIR"/image/rawimage_200x300.raw", ios::binary);
        if (!file.is_open()) return false;
        auto data = (uint32_t*) malloc(sizeof(uint32_t) * (200 * 300));
        file.read(reinterpret_cast<char*>(data), sizeof (uint32_t) * 200 * 300);
        file.close();

        //background
        auto bg = tvg::Shape::gen();
        bg->appendRect(0, 0, 625, h);
        bg->fill(50, 50, 50);
        canvas->push(std::move(bg));

        {
            //Shape + Shape Mask Add
            auto shape = tvg::Shape::gen();
            shape->appendCircle(125, 100, 150, 150);
            shape->fill(255, 255, 255);

            auto mask = tvg::Shape::gen();
            mask->appendCircle(125, 100, 50, 50);
            mask->fill(255, 255, 255);

            auto add = tvg::Shape::gen();
            add->appendCircle(175, 100, 50, 50);
            add->fill(255, 255, 255);
            mask->composite(std::move(add), tvg::CompositeMethod::AddMask);
            shape->composite(std::move(mask), tvg::CompositeMethod::AlphaMask);
            canvas->push(std::move(shape));

            //Shape + Shape Mask Subtract
            auto shape2 = tvg::Shape::gen();
            shape2->appendCircle(375, 100, 150, 150);
            shape2->fill(255, 255, 255, 255);

            auto mask2 = tvg::Shape::gen();
            mask2->appendCircle(375, 100, 50, 50);
            mask2->fill(255, 255, 255, 127);

            auto sub = tvg::Shape::gen();
            sub->appendCircle(400, 100, 50, 50);
            sub->fill(255, 255, 255);
            mask2->composite(std::move(sub), tvg::CompositeMethod::SubtractMask);
            shape2->composite(std::move(mask2), tvg::CompositeMethod::AlphaMask);
            canvas->push(std::move(shape2));

            //Shape + Shape Mask Intersect
            auto shape3 = tvg::Shape::gen();
            shape3->appendCircle(625, 100, 50, 50);
            shape3->fill(255, 255, 255, 127);

            auto mask3 = tvg::Shape::gen();
            mask3->appendCircle(625, 100, 50, 50);
            mask3->fill(255, 255, 255, 127);

            auto inter = tvg::Shape::gen();
            inter->appendCircle(650, 100, 50, 50);
            inter->fill(255, 255, 255);
            mask3->composite(std::move(inter), tvg::CompositeMethod::IntersectMask);
            shape3->composite(std::move(mask3), tvg::CompositeMethod::AlphaMask);
            canvas->push(std::move(shape3));

            //Shape + Shape Mask Difference
            auto shape4 = tvg::Shape::gen();
            shape4->appendCircle(875, 100, 150, 150);
            shape4->fill(255, 255, 255);

            auto mask4 = tvg::Shape::gen();
            mask4->appendCircle(875, 100, 50, 50);
            mask4->fill(255, 255, 255);

            auto diff = tvg::Shape::gen();
            diff->appendCircle(900, 100, 50, 50);
            diff->fill(255, 255, 255);
            mask4->composite(std::move(diff), tvg::CompositeMethod::DifferenceMask);
            shape4->composite(std::move(mask4), tvg::CompositeMethod::AlphaMask);
            canvas->push(std::move(shape4));
        }
        {
            //Shape + Shape Mask Add
            auto shape = tvg::Shape::gen();
            shape->appendCircle(125, 300, 100, 100);
            shape->fill(255, 255, 255);

            auto mask = tvg::Shape::gen();
            mask->appendCircle(125, 300, 50, 50);
            mask->fill(255, 255, 255);

            auto add = tvg::Shape::gen();
            add->appendCircle(175, 300, 50, 50);
            add->fill(255, 255, 255);
            mask->composite(std::move(add), tvg::CompositeMethod::AddMask);
            shape->composite(std::move(mask), tvg::CompositeMethod::InvAlphaMask);
            canvas->push(std::move(shape));

            //Shape + Shape Mask Subtract
            auto shape2 = tvg::Shape::gen();
            shape2->appendCircle(375, 300, 100, 100);
            shape2->fill(255, 255, 255, 255);

            auto mask2 = tvg::Shape::gen();
            mask2->appendCircle(375, 300, 50, 50);
            mask2->fill(255, 255, 255, 127);

            auto sub = tvg::Shape::gen();
            sub->appendCircle(400, 300, 50, 50);
            sub->fill(255, 255, 255);
            mask2->composite(std::move(sub), tvg::CompositeMethod::SubtractMask);
            shape2->composite(std::move(mask2), tvg::CompositeMethod::InvAlphaMask);
            canvas->push(std::move(shape2));

            //Shape + Shape Mask Intersect
            auto shape3 = tvg::Shape::gen();
            shape3->appendCircle(625, 300, 100, 100);
            shape3->fill(255, 255, 255, 127);

            auto mask3 = tvg::Shape::gen();
            mask3->appendCircle(625, 300, 50, 50);
            mask3->fill(255, 255, 255, 127);

            auto inter = tvg::Shape::gen();
            inter->appendCircle(650, 300, 50, 50);
            inter->fill(255, 255, 255);
            mask3->composite(std::move(inter), tvg::CompositeMethod::IntersectMask);
            shape3->composite(std::move(mask3), tvg::CompositeMethod::InvAlphaMask);
            canvas->push(std::move(shape3));

            //Shape + Shape Mask Difference
            auto shape4 = tvg::Shape::gen();
            shape4->appendCircle(875, 300, 100, 100);
            shape4->fill(255, 255, 255);

            auto mask4 = tvg::Shape::gen();
            mask4->appendCircle(875, 300, 50, 50);
            mask4->fill(255, 255, 255);

            auto diff = tvg::Shape::gen();
            diff->appendCircle(900, 300, 50, 50);
            diff->fill(255, 255, 255);
            mask4->composite(std::move(diff), tvg::CompositeMethod::DifferenceMask);
            shape4->composite(std::move(mask4), tvg::CompositeMethod::InvAlphaMask);
            canvas->push(std::move(shape4));
        }
        {
            //Rect + Rect Mask Add
            auto shape = tvg::Shape::gen();
            shape->appendRect(75, 450, 150, 150);
            shape->fill(255, 255, 255);

            auto mask = tvg::Shape::gen();
            mask->appendRect(75, 500, 100, 100);
            mask->fill(255, 255, 255);

            auto add = tvg::Shape::gen();
            add->appendRect(125, 450, 100, 100);
            add->fill(255, 255, 255);
            mask->composite(std::move(add), tvg::CompositeMethod::AddMask);
            shape->composite(std::move(mask), tvg::CompositeMethod::AlphaMask);
            canvas->push(std::move(shape));

            //Rect + Rect Mask Subtract
            auto shape2 = tvg::Shape::gen();
            shape2->appendRect(325, 450, 150, 150);
            shape2->fill(255, 255, 255);

            auto mask2 = tvg::Shape::gen();
            mask2->appendRect(325, 500, 100, 100);
            mask2->fill(255, 255, 255, 127);

            auto sub = tvg::Shape::gen();
            sub->appendRect(375, 450, 100, 100);
            sub->fill(255, 255, 255);
            mask2->composite(std::move(sub), tvg::CompositeMethod::SubtractMask);
            shape2->composite(std::move(mask2), tvg::CompositeMethod::AlphaMask);
            canvas->push(std::move(shape2));

            //Rect + Rect Mask Intersect
            auto shape3 = tvg::Shape::gen();
            shape3->appendRect(575, 450, 150, 150);
            shape3->fill(255, 255, 255);

            auto mask3 = tvg::Shape::gen();
            mask3->appendRect(575, 500, 100, 100);
            mask3->fill(255, 255, 255, 127);

            auto inter = tvg::Shape::gen();
            inter->appendRect(625, 450, 100, 100);
            inter->fill(255, 255, 255);
            mask3->composite(std::move(inter), tvg::CompositeMethod::IntersectMask);
            shape3->composite(std::move(mask3), tvg::CompositeMethod::AlphaMask);
            canvas->push(std::move(shape3));

            //Rect + Rect Mask Difference
            auto shape4 = tvg::Shape::gen();
            shape4->appendRect(825, 450, 150, 150);
            shape4->fill(255, 255, 255);

            auto mask4 = tvg::Shape::gen();
            mask4->appendRect(825, 500, 100, 100);
            mask4->fill(255, 255, 255);

            auto diff = tvg::Shape::gen();
            diff->appendRect(875, 450, 100, 100);
            diff->fill(255, 255, 255);
            mask4->composite(std::move(diff), tvg::CompositeMethod::DifferenceMask);
            shape4->composite(std::move(mask4), tvg::CompositeMethod::AlphaMask);
            canvas->push(std::move(shape4));
        }

        {
            //Transformed Image + Shape Mask Add
            auto image = tvg::Picture::gen();
            if (!tvgexam::verify(image->load(data, 200, 300, true))) return false;
            image->translate(150, 650);
            image->scale(0.5f);
            image->rotate(45);

            auto mask = tvg::Shape::gen();
            mask->appendCircle(125, 700, 50, 50);
            mask->fill(255, 255, 255);

            auto add = tvg::Shape::gen();
            add->appendCircle(150, 750, 50, 50);
            add->fill(255, 255, 255);
            mask->composite(std::move(add), tvg::CompositeMethod::AddMask);
            image->composite(std::move(mask), tvg::CompositeMethod::AlphaMask);
            canvas->push(std::move(image));

            //Transformed Image + Shape Mask Subtract
            auto image2 = tvg::Picture::gen();
            if (!tvgexam::verify(image2->load(data, 200, 300, true))) return false;
            image2->translate(400, 650);
            image2->scale(0.5f);
            image2->rotate(45);

            auto mask2 = tvg::Shape::gen();
            mask2->appendCircle(375, 700, 50, 50);
            mask2->fill(255, 255, 255, 127);

            auto sub = tvg::Shape::gen();
            sub->appendCircle(400, 750, 50, 50);
            sub->fill(255, 255, 255);
            mask2->composite(std::move(sub), tvg::CompositeMethod::SubtractMask);
            image2->composite(std::move(mask2), tvg::CompositeMethod::AlphaMask);
            canvas->push(std::move(image2));

            //Transformed Image + Shape Mask Intersect
            auto image3 = tvg::Picture::gen();
            if (!tvgexam::verify(image3->load(data, 200, 300, true))) return false;
            image3->translate(650, 650);
            image3->scale(0.5f);
            image3->rotate(45);

            auto mask3 = tvg::Shape::gen();
            mask3->appendCircle(625, 700, 50, 50);
            mask3->fill(255, 255, 255, 127);

            auto inter = tvg::Shape::gen();
            inter->appendCircle(650, 750, 50, 50);
            inter->fill(255, 255, 255, 127);
            mask3->composite(std::move(inter), tvg::CompositeMethod::IntersectMask);
            image3->composite(std::move(mask3), tvg::CompositeMethod::AlphaMask);
            canvas->push(std::move(image3));

            //Transformed Image + Shape Mask Difference
            auto image4 = tvg::Picture::gen();
            if (!tvgexam::verify(image4->load(data, 200, 300, true))) return false;
            image4->translate(900, 650);
            image4->scale(0.5f);
            image4->rotate(45);

            auto mask4 = tvg::Shape::gen();
            mask4->appendCircle(875, 700, 50, 50);
            mask4->fill(255, 255, 255);

            auto diff = tvg::Shape::gen();
            diff->appendCircle(900, 750, 50, 50);
            diff->fill(255, 255, 255);
            mask4->composite(std::move(diff), tvg::CompositeMethod::DifferenceMask);
            image4->composite(std::move(mask4), tvg::CompositeMethod::AlphaMask);
            canvas->push(std::move(image4));
        }
        {
            //Transformed Image + Shape Mask Add
            auto image = tvg::Picture::gen();
            if (!tvgexam::verify(image->load(data, 200, 300, true))) return false;
            image->translate(150, 850);
            image->scale(0.5f);
            image->rotate(45);

            auto mask = tvg::Shape::gen();
            mask->appendCircle(125, 900, 50, 50);
            mask->fill(255, 255, 255);

            auto add = tvg::Shape::gen();
            add->appendCircle(150, 950, 50, 50);
            add->fill(255, 255, 255);
            mask->composite(std::move(add), tvg::CompositeMethod::AddMask);
            image->composite(std::move(mask), tvg::CompositeMethod::InvAlphaMask);
            canvas->push(std::move(image));

            //Transformed Image + Shape Mask Subtract
            auto image2 = tvg::Picture::gen();
            if (!tvgexam::verify(image2->load(data, 200, 300, true))) return false;
            image2->translate(400, 850);
            image2->scale(0.5f);
            image2->rotate(45);

            auto mask2 = tvg::Shape::gen();
            mask2->appendCircle(375, 900, 50, 50);
            mask2->fill(255, 255, 255, 127);

            auto sub = tvg::Shape::gen();
            sub->appendCircle(400, 950, 50, 50);
            sub->fill(255, 255, 255);
            mask2->composite(std::move(sub), tvg::CompositeMethod::SubtractMask);
            image2->composite(std::move(mask2), tvg::CompositeMethod::InvAlphaMask);
            canvas->push(std::move(image2));

            //Transformed Image + Shape Mask Intersect
            auto image3 = tvg::Picture::gen();
            if (!tvgexam::verify(image3->load(data, 200, 300, true))) return false;
            image3->translate(650, 850);
            image3->scale(0.5f);
            image3->rotate(45);

            auto mask3 = tvg::Shape::gen();
            mask3->appendCircle(625, 900, 50, 50);
            mask3->fill(255, 255, 255, 127);

            auto inter = tvg::Shape::gen();
            inter->appendCircle(650, 950, 50, 50);
            inter->fill(255, 255, 255, 127);
            mask3->composite(std::move(inter), tvg::CompositeMethod::IntersectMask);
            image3->composite(std::move(mask3), tvg::CompositeMethod::InvAlphaMask);
            canvas->push(std::move(image3));

            //Transformed Image + Shape Mask Difference
            auto image4 = tvg::Picture::gen();
            if (!tvgexam::verify(image4->load(data, 200, 300, true))) return false;
            image4->translate(900, 850);
            image4->scale(0.5f);
            image4->rotate(45);

            auto mask4 = tvg::Shape::gen();
            mask4->appendCircle(875, 900, 50, 50);
            mask4->fill(255, 255, 255);

            auto diff = tvg::Shape::gen();
            diff->appendCircle(900, 950, 50, 50);
            diff->fill(255, 255, 255);
            mask4->composite(std::move(diff), tvg::CompositeMethod::DifferenceMask);
            image4->composite(std::move(mask4), tvg::CompositeMethod::InvAlphaMask);
            canvas->push(std::move(image4));
        }

        free(data);

        return true;
    }
};


/************************************************************************/
/* Entry Point                                                          */
/************************************************************************/

int main(int argc, char **argv)
{
    return tvgexam::main(new UserExample, argc, argv, 1024, 1024);
}