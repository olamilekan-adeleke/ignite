
#include "ui_component.hpp"
#include "size.hpp"
#include "ui_alignment.hpp"
#include <memory>

struct SizedParam {
  UISize size = {-1, -1};
  UIAlignment align = UIAlignment::TopLeft;
  std::shared_ptr<UIComponent> child = nullptr;
};

class Sized : public UIComponent {
public:
  Sized(const SizedParam &param = {});

  void layout(float parentWidth, float parentHeight);
  void draw(SkCanvas *canvas);

private:
  UISize size_;
  UIAlignment align_;
  std::shared_ptr<UIComponent> child_;
};
