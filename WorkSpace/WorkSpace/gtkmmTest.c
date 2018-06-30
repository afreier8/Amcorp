glade and OpenGL in same window

#include <gtkmm.h>
#include <gtkglmm.h>

class GLWidget : public Gtk::GL::DrawingArea {
    public:
    GLWidget(Glib::RefPtr<Gdk::GL::Config> glconfig)
        : Gtk::GL::DrawingArea(glconfig) {}
    ~GLWidget() {}
    virtual bool on_expose_event(GdkEventExpose* event);
};

bool GLWidget::on_expose_event(GdkEventExpose* event)
{
    Glib::RefPtr<Gdk::GL::Drawable> d = get_gl_drawable();
    d->gl_begin(get_gl_context());

    // make this as complex as you need
    glClear(GL_COLOR_BUFFER_BIT);

    d->swap_buffers();
    d->gl_end();
    return true;
}

int main(int argc, char **argv)
{
    Gtk::Main kit(argc, argv);
    Gtk::GL::init(argc, argv);

    Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("ui.glade");

    Gtk::Window* mainWindow;
    Gtk::Alignment* container;

    builder->get_widget("mainWindow", mainWindow);
    builder->get_widget("Box", container);

    if (mainWindow == NULL || container == NULL) {
        g_critical("Gtk Builder failed to load mainWindow and/or container !\n");
        return -1;
    }

    Glib::RefPtr<Gdk::GL::Config> glconfig;

    glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGBA | Gdk::GL::MODE_DOUBLE);
    if (!glconfig)
        glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB);
    if (!glconfig) {
        g_critical("Cannot create OpenGL-capable config\n");
        return -1;
    }

    GLWidget drawingArea(glconfig);

    drawingArea.set_size_request(640, 480);
    drawingArea.show();
    container->add(drawingArea);

    kit.run(*mainWindow);

    return 0;
}
