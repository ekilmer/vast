// RUN: vast-cc --from-source %s | vast-opt --vast-hl-lower-types | FileCheck %s
// REQUIRES: type-lowering

// CHECK-LABEL: func @main() -> i32
int main()
{
    // CHECK: hl.var "x" : !hl.lvalue<i1> =  {
    // CHECK:   [[V1:%[0-9]+]] = hl.const #hl.bool<false> : i1
    // CHECK:   hl.value.yield [[V1]] : i1
    // CHECK: }
    bool x = false;

    // CHECK: hl.var "y" : !hl.lvalue<i8> =  {
    // CHECK:   [[V2:%[0-9]+]] = hl.const #hl.integer<72> : i8
    // CHECK:   hl.value.yield [[V2]] : i8
    // CHECK: }
    char y = 'H';


    // CHECK: hl.var "ia" : !hl.lvalue<memref<3xi32>> =  {
    // CHECK:   [[V3:%[0-9]+]] = hl.const #hl.integer<0> : i32
    // CHECK:   [[V4:%[0-9]+]] = hl.const #hl.integer<1> : i32
    // CHECK:   [[V5:%[0-9]+]] = hl.const #hl.integer<2> : i32
    // CHECK:   [[V6:%[0-9]+]] = hl.initlist [[V3]], [[V4]], [[V5]] : (i32, i32, i32) -> memref<3xi32>
    // CHECK:   hl.value.yield [[V6]] : memref<3xi32>
    // CHECK: }
    int ia[3] = { 0, 1, 2 };


    // CHECK: hl.var "ll" : !hl.lvalue<i64> =  {
    // CHECK:   [[V7:%[0-9]+]] = hl.const #hl.integer<142> : i32
    // CHECK:   [[V8:%[0-9]+]] = hl.implicit_cast [[V7]] IntegralCast : i32 -> i64
    // CHECK:   hl.value.yield [[V8]] : i64
    // CHECK: }
    long long ll = 142;



    // CHECK: hl.var "ld" : !hl.lvalue<f128> =  {
    // CHECK:   [[V9:%[0-9]+]] = hl.constant.float 91.019999999999996 : f64
    // CHECK:   [[V10:%[0-9]+]] = hl.implicit_cast [[V9]] FloatingCast : f64 -> f128
    // CHECK:   hl.value.yield [[V10]] : f128
    // CHECK: }
    long double ld = 91.02;

    // CHECK: hl.var "fa" : !hl.lvalue<memref<3xf32>> =  {
    // CHECK:   [[V11:%[0-9]+]] = hl.constant.float 0.000000e+00 : f64
    // CHECK:   [[V12:%[0-9]+]] = hl.implicit_cast [[V11]] FloatingCast : f64 -> f32
    // CHECK:   [[V13:%[0-9]+]] = hl.constant.float 5.000000e-01 : f64
    // CHECK:   [[V14:%[0-9]+]] = hl.implicit_cast [[V13]] FloatingCast : f64 -> f32
    // CHECK:   [[V15:%[0-9]+]] = hl.constant.float 1.000000e+00 : f64
    // CHECK:   [[V16:%[0-9]+]] = hl.implicit_cast [[V15]] FloatingCast : f64 -> f32
    // CHECK:   [[V17:%[0-9]+]] = hl.initlist [[V12]], [[V14]], [[V16]] : (f32, f32, f32) -> memref<3xf32>
    // CHECK:   hl.value.yield [[V17]] : memref<3xf32>
    // CHECK: }
    float fa[3] = { 0.0, 0.5, 1.0 };
}
