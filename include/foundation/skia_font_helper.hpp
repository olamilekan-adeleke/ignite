#pragma once

// Forward-declare Skia types (avoids includes)
class SkFontMgr;
template <typename T>
class sk_sp;

// Declare our factory function
sk_sp<SkFontMgr> CreatePlatformFontMgr();
