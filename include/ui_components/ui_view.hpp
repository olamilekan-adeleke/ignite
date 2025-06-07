
#include "color.hpp"
#include "ui_component.hpp"
#include "ui_components/ui_component.hpp"
#include "ui_edge_insets.hpp"

struct ViewParams {
  UIEdgeInsets insets = {0.0f, 0.0f, 0.0f, 0.0f};
  UIEdgeInsets margin = {0.0f, 0.0f, 0.0f, 0.0f};
  Color backgroundColor = Color::White();
  float borderRadius = 0.0f;
  bool antiAlias = false;
  std::shared_ptr<UIComponent> child = nullptr;
};

class View : public UIComponent {
public:
  View(const ViewParams &params = ViewParams());

  View(const View &) = delete;
  View &operator=(const View &) = delete;

  void layout(float parentWidth, float parentHeight) override;
  void draw(SkCanvas *canvas) override;

protected:
  ViewParams params_;
};
