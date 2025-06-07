
#include <memory>
#include <vector>

#include "ui_components/ui_component.hpp"

struct RowParams {
  float spacing = 0.0f;
  std::vector<std::shared_ptr<UIComponent>> children;
};

class Row : public UIComponent {
public:
  Row(const RowParams &param = {});

  // Template magic
  // template <typename... Children>
  // Row(std::shared_ptr<UIComponent> first, Children... rest);

  void addChild(std::shared_ptr<UIComponent> child);

  void layout(float parentWidth, float parentHeight) override;
  void draw(SkCanvas *canvas) override;

  const std::vector<std::shared_ptr<UIComponent>> children() override;

private:
  float spacing_;
  std::vector<std::shared_ptr<UIComponent>> children_;
};
