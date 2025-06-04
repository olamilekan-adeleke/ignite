
#include <memory>
#include <vector>

#include "ui_components/ui_component.hpp"

class Row : public UIComponent {
 public:
  Row() = default;

  // Template magic
  template <typename... Children>
  Row(std::shared_ptr<UIComponent> first, Children... rest);

  ~Row() override = default;
  Row(const Row&) = delete;
  Row& operator=(const Row&) = delete;
  Row& addChild(std::shared_ptr<UIComponent> child);
  void layout(float parentWidth, float parentHeight) override;
  void draw(SkCanvas* canvas) override;

 private:
  std::vector<std::shared_ptr<UIComponent>> children_;
};
