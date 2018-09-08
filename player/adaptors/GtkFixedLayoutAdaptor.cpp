#include "GtkFixedLayoutAdaptor.hpp"

int GtkFixedLayoutAdaptor::width() const
{
    int width, _;
    m_handler.get_size_request(width, _);
    return width;
}

int GtkFixedLayoutAdaptor::height() const
{
    int _, height;
    m_handler.get_size_request(_, height);
    return height;
}

void GtkFixedLayoutAdaptor::setSize(int width, int height)
{
    m_handler.set_size_request(width, height);
}

void GtkFixedLayoutAdaptor::addChild(IWidgetAdaptor& child, int left, int top)
{
    auto&& handler = getHandler(child);
    m_handler.put(handler, left, top);
}

void GtkFixedLayoutAdaptor::removeChildren()
{
    for(auto&& widget : m_handler.get_children())
    {
        m_handler.remove(*widget);
    }
}

void GtkFixedLayoutAdaptor::show()
{
    m_handler.show();
}

Gtk::Fixed* GtkFixedLayoutAdaptor::get()
{
    return &m_handler;
}

void GtkFixedLayoutAdaptor::apply(AdaptorVisitor& visitor)
{
    visitor.visit(*this);
}
