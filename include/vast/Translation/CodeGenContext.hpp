// Copyright (c) 2022-present, Trail of Bits, Inc.

#pragma once

#include "vast/Util/Warnings.hpp"

VAST_RELAX_WARNINGS
#include <clang/AST/ASTContext.h>
#include <llvm/ADT/ScopedHashTable.h>
#include <mlir/IR/MLIRContext.h>
#include <mlir/IR/Value.h>
#include <mlir/Support/LogicalResult.h>
VAST_UNRELAX_WARNINGS

#include "vast/Dialect/HighLevel/HighLevelDialect.hpp"
#include "vast/Dialect/HighLevel/HighLevelOps.hpp"
#include "vast/Dialect/HighLevel/HighLevelAttributes.hpp"
#include "vast/Dialect/HighLevel/HighLevelTypes.hpp"
#include "vast/Util/Functions.hpp"
#include "vast/Util/ScopeTable.hpp"
#include "vast/Util/Common.hpp"

#include <variant>

namespace vast::hl
{
    struct CodeGenContext {
        MContext &mctx;
        AContext &actx;
        OwningModuleRef &mod;

        dl::DataLayoutBlueprint dl;

        CodeGenContext(MContext &mctx, AContext &actx, OwningModuleRef &mod)
            : mctx(mctx)
            , actx(actx)
            , mod(mod)
        {}

        using VarTable = ScopedValueTable< const clang::VarDecl *, Value >;
        VarTable vars;

        using TypeDefTable = ScopedValueTable< const clang::TypedefDecl *, TypeDefOp >;
        TypeDefTable typedefs;

        using TypeDeclTable = ScopedValueTable< const clang::TypeDecl *, TypeDeclOp >;
        TypeDeclTable typedecls;

        using FuncDeclTable = ScopedValueTable< const clang::FunctionDecl *, mlir::func::FuncOp >;
        FuncDeclTable funcdecls;

        using EnumDecls = ScopedValueTable< const clang::EnumDecl *, EnumDeclOp >;
        EnumDecls enumdecls;

        using EnumConstants = ScopedValueTable< const clang::EnumConstantDecl *, EnumConstantOp >;
        EnumConstants enumconsts;

        using LabelTable = ScopedValueTable< const clang::LabelDecl*, LabelDeclOp >;
        LabelTable labels;

        size_t anonymous_count = 0;
        llvm::DenseMap< const clang::TagDecl *, std::string > tag_names;

        std::string get_decl_name(const clang::NamedDecl *decl) {
            if (decl->getIdentifier())
                return decl->getName().str();
            return "anonymous[" + std::to_string(decl->getID()) + "]";
        }

        std::string get_namespaced_for_decl_name(const clang::TagDecl *decl) {
            // gather contexts
            std::vector< const clang::DeclContext * > dctxs;
            for (const auto *dctx = decl->getDeclContext(); dctx; dctx = dctx->getParent()) {
                dctxs.push_back(dctx);
            }

            std::string name;
            for (const auto *dctx : llvm::reverse(dctxs)) {
                if (llvm::isa< clang::TranslationUnitDecl >(dctx))
                    continue;

                if (llvm::isa< clang::FunctionDecl >(dctx))
                    continue;

                if (const auto *d = llvm::dyn_cast< clang::TagDecl >(dctx)) {
                    name += get_decl_name(d);
                } else {
                    VAST_UNREACHABLE("unknown decl context: {0}", dctx->getDeclKindName());
                }

                name += "::";
            }

            return name;
        }

        std::string get_namespaced_decl_name(const clang::TagDecl *decl) {
            return get_namespaced_for_decl_name(decl) + get_decl_name(decl);
        }

        llvm::StringRef decl_name(const clang::TagDecl *decl) {
            if (tag_names.count(decl)) {
                return tag_names[decl];
            }

            auto name = get_namespaced_decl_name(decl);
            auto [it, _] = tag_names.try_emplace(decl, name);
            return it->second;
        }

        const dl::DataLayoutBlueprint &data_layout() const { return dl; }
        dl::DataLayoutBlueprint &data_layout() { return dl; }

        mlir::Region &getBodyRegion() { return mod->getBodyRegion(); }

        auto error(llvm::Twine msg) { return mod->emitError(msg); }

        template< typename Table, typename Token, typename ValueType = typename Table::ValueType >
        ValueType symbol(Table &table, Token token, llvm::Twine msg, bool with_error = true) {
            if (auto val = table.lookup(token))
                return val;
            if (with_error)
                error(msg);
            return nullptr;
        }

        mlir::func::FuncOp lookup_function(const clang::FunctionDecl *decl, bool with_error = true) {
            return symbol(funcdecls, decl, "error: undeclared function '" + decl->getName() + "'", with_error);
        }

        //
        // Integer Attribute Constants
        //
        template< typename T >
        Type bitwidth_type() { return mlir::IntegerType::get(&mctx, bits< T >()); }

        template< typename T >
        mlir::IntegerAttr interger_attr(T v) { return mlir::IntegerAttr::get(bitwidth_type< T >(), v); }

        mlir::IntegerAttr  u8(uint8_t  v) { return interger_attr(v); }
        mlir::IntegerAttr u16(uint16_t v) { return interger_attr(v); }
        mlir::IntegerAttr u32(uint32_t v) { return interger_attr(v); }
        mlir::IntegerAttr u64(uint64_t v) { return interger_attr(v); }

        mlir::IntegerAttr  i8(int8_t  v) { return interger_attr(v); }
        mlir::IntegerAttr i16(int16_t v) { return interger_attr(v); }
        mlir::IntegerAttr i32(int32_t v) { return interger_attr(v); }
        mlir::IntegerAttr i64(int64_t v) { return interger_attr(v); }
    };
} // namespace vast::hl
