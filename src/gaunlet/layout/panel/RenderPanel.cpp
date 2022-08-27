#include "gaunlet/layout/panel/RenderPanel.h"

namespace gaunlet::Layout {

    void RenderPanel::onUpdate(gaunlet::Core::TimeStep) {
        m_scene.render(m_renderMode, m_camera, m_framebuffer);
    }

}