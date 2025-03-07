// RUN: vast-cc --from-source %s | vast-opt --vast-hl-lower-types | FileCheck %s
// REQUIRES: type-lowering

// CHECK-LABEL: func @main() -> i32
int main()
{
    // CHECK: hl.var "a" : !hl.lvalue<i32>
    int a;

    // CHECK: hl.var "b" : !hl.lvalue<i32> = {
    // CHECK:   [[V1:%[0-9]+]] = hl.const #hl.integer<1> : i32
    // CHECK:   hl.value.yield [[V1]] : i32
    // CHECK: }
    int b = 1;

    unsigned int ub = 1U;

    // CHECK: hl.var "c" : !hl.lvalue<i32> = {
    // CHECK:   [[V2:%[0-9]+]] = hl.const #hl.integer<1> : i32
    // CHECK:   hl.value.yield [[V2]] : i32
    // CHECK: }
    int c( 1 );

    int ni = -1;

    // CHECK:   [[V4:%[0-9]+]] = hl.implicit_cast [[V5:%[0-9]+]] IntegralCast : i32 -> i64
    long nl = -1;
}
