// RUN: vast-cc --ccopts -xc --from-source %s | vast-opt --vast-hl-lower-types --vast-hl-structs-to-tuples --vast-hl-to-ll | FileCheck %s
// REQUIRES: to-ll

// CHECK: llvm.func @count([[ARG:%arg[0-9]+]]: i32)
void count(int arg)
{
    // store argument
    // CHECK: [[C:%[0-9]+]] = llvm.mlir.constant(1 : index) : i64
    // CHECK: [[ADDR:%[0-9]+]] = llvm.alloca [[C]] x i32 : (i64) -> !llvm.ptr<i32>
    // CHECK: llvm.store [[ARG]], [[ADDR]] : !llvm.ptr<i32>

    // CHECK: [[V0:%[0-9]+]] = llvm.mlir.constant(1 : index) : i64
    // CHECK: [[V1:%[0-9]+]] = llvm.alloca [[V0]] x i32 : (i64) -> !llvm.ptr<i32>
    // CHECK: [[V2:%[0-9]+]] = llvm.mlir.constant(15 : ui8) : i32
    // CHECK: llvm.store [[V2]], [[V1]] : !llvm.ptr<i32>
    // CHECK: [[V3:%[0-9]+]] = llvm.mlir.constant(1 : index) : i64
    // CHECK: [[V4:%[0-9]+]] = llvm.alloca [[V3]] x i32 : (i64) -> !llvm.ptr<i32>
    // CHECK: [[V5:%[0-9]+]] = llvm.load [[V1]] : !llvm.ptr<i32>
    // CHECK: llvm.store [[V5]], [[V4]] : !llvm.ptr<i32>

    unsigned int c = 15, iter = c;
    // CHECK: llvm.return
}
