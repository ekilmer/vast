// Copyright (c) 2022-present, Trail of Bits, Inc.

#pragma once

#include "vast/repl/common.hpp"

namespace vast::repl {

    using owning_module_ref = OwningModuleRef;

    struct state_t {
        explicit state_t(MContext &ctx) : ctx(ctx) {}

        bool exit = false;

        std::optional< std::string > source;

        MContext &ctx;
        owning_module_ref mod;
    };

} // namespace vast::repl
