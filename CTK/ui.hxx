/*
 * Copyright(c) 2018, Markus Schmidt <schmidt@boomshop.net>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3.0 of the
 * License, or (at your option) any later version.
 *
 * CTK is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with CTK; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CTK_UI_HXX
#define CTK_UI_HXX

#include "pugl/pugl.h"

#include <cairo/cairo.h>
#include <unistd.h>
#include <stdio.h>
#include <list>
#include <unordered_map>

#include "container.hxx"
#include "style.hxx"

namespace CTK {

class UI : public CTK::Container, public CTK::Style
{
public:
    const char* type = "ui";
    
    UI (PuglNativeWindow parent = 0, const char* windowName = "CTK");
    virtual ~UI();
    
    /// when used as a UI plugin, created by a host, this function should be
    /// called repeatedly at ~30 fps to handle events and redraw if needed.
    int idle ();

    /// when UI is running standalone, call this function to run the UI. When
    /// the function returns, the main window has been closed.
    virtual int run ();

    /// call this to recieve the LV2 widget handle
    PuglNativeWindow getNativeWindow () { return puglGetNativeWindow(view); }
    
    const char* title;
    
    void close () { quit = true; }
    void requestExpose(CTK::Widget* w);
    void addEvent(CTK::Widget *widget, CTK::EventType type, int (*callback)(CTK::Widget*, const void*, void*), void *data = NULL);
    void removeEvent(CTK::Widget *widget, CTK::EventType type, int (*callback)(CTK::Widget*, const void*, void*));
    
    virtual void rescale (float scale);
    
protected:
    PuglView* view;
    std::list<CTK::Widget*> queue;
    std::list<CTK::GenericEventMeta*> events[CTK::GENERIC_EVENT_TYPE_SIZE];
    bool quit;
    
    virtual void resize ();
    virtual void recalc ();
    virtual void expose ();
    
    void resizeWindow (PuglView* view, int w, int h);

    void handleExpose (const PuglEventExpose _event);
    void handleConfigure (const PuglEventConfigure _event);
    void handleEvent (const CTK::Event* event);
    void internalEvent (const CTK::Event* event);
    
    static void onEvent (PuglView* _view, const PuglEvent* _event) {
        UI* ui = (UI*)puglGetHandle(_view);
        ui->handleEvent((CTK::Event*)_event);
    }
    
    void requestExpose () { if (view) puglPostRedisplay(view); }
    void addToQueue (CTK::Widget* w);
};

}; // namespace CTK

#endif // CTK_UI_HXX
