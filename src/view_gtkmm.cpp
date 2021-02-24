/*
 * Reference:
 *    https://www.gtkmm.org
 */

#include "view_gtkmm.hpp"

GUIApp::GUIApp(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
: Gtk::ApplicationWindow(cobject), m_refGlade(refGlade)
{
}

GUIApp::~GUIApp()
{
}
