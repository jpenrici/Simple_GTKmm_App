/*
 * Reference:
 *    https://www.gtkmm.org
 */

#ifndef GTKMM_APP_H
#define GTKMM_APP_H

#include <gtkmm.h>

class GUIApp : public Gtk::ApplicationWindow {

    public:
        GUIApp(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
        virtual ~GUIApp();

    protected:
        Glib::RefPtr<Gtk::Builder> m_refGlade;
};

#endif // GTKMM_APP_H
