#include "jni/Core.hpp"
#include "jni/Window.hpp"

OperationResult jni::Window::enableImmesiveStickyMode() const
{
    auto&& core = jni::Core::attachCurrentThread();
    JNIEnv* env = core.getEnv();

    jobject window = sf::getNativeActivity()->clazz; // NativeActivity is itself an Activity
    jclass activityClass = env->GetObjectClass(window);

    jmethodID getWindow = env->GetMethodID(activityClass, "getWindow", "()Landroid/view/Window;");
    jobject win = env->CallObjectMethod(window, getWindow);
    if (win == nullptr)
        return OperationResult::Failure;

    jclass windowClass = env->GetObjectClass(win);
    jmethodID getDecorView = env->GetMethodID(windowClass, "getDecorView", "()Landroid/view/View;");
    jobject decorView = env->CallObjectMethod(win, getDecorView);
    if (decorView == nullptr)
        return OperationResult::Failure;

    jclass viewClass = env->GetObjectClass(decorView);
    jmethodID setSystemUiVisibility = env->GetMethodID(viewClass, "setSystemUiVisibility", "(I)V");

    const int flags =
        0x00001000 | // SYSTEM_UI_FLAG_IMMERSIVE_STICKY
        0x00000004 | // SYSTEM_UI_FLAG_FULLSCREEN
        0x00000002 | // SYSTEM_UI_FLAG_HIDE_NAVIGATION
        0x00000100 | // SYSTEM_UI_FLAG_LAYOUT_STABLE
        0x00000400 | // SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
        0x00000200;  // SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION

    env->CallVoidMethod(decorView, setSystemUiVisibility, flags);
    return OperationResult::Success;
}
