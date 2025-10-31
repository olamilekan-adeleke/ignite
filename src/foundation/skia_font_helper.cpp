#include "foundation/skia_font_helper.hpp"

#include <include/core/SkFontMgr.h>

#if defined(SK_BUILD_FOR_MAC)
#include <include/ports/SkFontMgr_mac_ct.h>
#elif defined(SK_BUILD_FOR_LINUX)
#include <include/ports/SkFontMgr_direct.h>
#elif defined(SK_BUILD_FOR_WIN)
#include <include/ports/SkFontMgr_win_dw.h>
#endif

sk_sp<SkFontMgr> CreatePlatformFontMgr() {
#if defined(SK_BUILD_FOR_MAC)
  return SkFontMgr_New_CoreText(nullptr);
#elif defined(SK_BUILD_FOR_LINUX)
  return SkFontMgr_New_Direct();
#elif defined(SK_BUILD_FOR_WIN)
  return SkFontMgr_New_DirectWrite();
#else
  return SkFontMgr::RefEmpty();
#endif
}
