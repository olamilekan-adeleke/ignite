#pragma once

#include <memory>
#include "basic/ui_component.hpp"

class StatefulComponent : public UIComponent {
 public:
  StatefulComponent() : isDirty_(false), needToRedraw_(true) {}
  virtual ~StatefulComponent() = default;

  virtual std::shared_ptr<UIComponent> body() = 0;

 protected:
  bool isDirty() const;

  void markDirty();

  std::shared_ptr<UIComponent> getChild();

  virtual bool processChildTaps(const UITapEvent &event) override;

  void layout(UISize size) override;
  void draw(SkCanvas *canvas) override;

  const std::vector<std::shared_ptr<UIComponent>> &children() const override;

  std::string toString(int indent = 0) const override;

 private:
  bool isDirty_;
  bool needToRedraw_;
  std::shared_ptr<UIComponent> cachedBody_;
  mutable std::vector<std::shared_ptr<UIComponent>> statefulChildren_;
};
