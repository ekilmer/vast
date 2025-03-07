// RUN: vast-cc --from-source %s | FileCheck %s
// RUN: vast-cc --from-source %s > %t && vast-opt %t | diff -B %t -

// CHECK-LABEL: func @cast_cstyle
void cast_cstyle()
{
    int i = 0;
    // CHECK: [[I:%[0-9]+]] = hl.var "i" : !hl.lvalue<!hl.int>
    short s = (short)i;
    // CHECK: hl.var "s" : !hl.lvalue<!hl.short>
    // CHECK: [[V1:%[0-9]+]] = hl.ref [[I]] : !hl.lvalue<!hl.int>
    // CHECK: [[V2:%[0-9]+]] = hl.implicit_cast [[V1]] LValueToRValue : !hl.lvalue<!hl.int> -> !hl.int
    // CHECK: [[V3:%[0-9]+]] = hl.implicit_cast [[V2]] IntegralCast : !hl.int -> !hl.short
    // CHECK: [[V4:%[0-9]+]] = hl.cstyle_cast [[V3]] NoOp : !hl.short -> !hl.short
    // CHECK: hl.value.yield [[V4]] : !hl.short
}
