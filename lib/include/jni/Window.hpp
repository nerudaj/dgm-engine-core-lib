#pragma once

#include "types/SemanticTypes.hpp"

namespace jni
{
    class [[nodiscard]] Window final {
    public:
        OperationResult enableImmesiveStickyMode() const;
    };
}
