// RUN: vast-cc --ccopts -xc --from-source %s | FileCheck %s
// RUN: vast-cc --ccopts -xc --from-source %s > %t && vast-opt %t | diff -B %t -

int main() {
    // CHECK: [[N:%[0-9]+]] = hl.var "n" : !hl.lvalue<!hl.int>
    // CHECK: hl.var "p" : !hl.lvalue<!hl.ptr<!hl.int>>
    // CHECK:   [[R:%[0-9]+]] = hl.ref [[N]] : !hl.lvalue<!hl.int>
    // CHECK:   hl.addressof [[R]] : !hl.lvalue<!hl.int> -> !hl.ptr<!hl.int>
    int n = 1, *p = &n;
    // CHECK: hl.var "pv" : !hl.lvalue<!hl.ptr<!hl.void>>
    // CHECK:   BitCast : !hl.ptr<!hl.int> -> !hl.ptr<!hl.void>
    void* pv = p; // int* to void*
    // CHECK: hl.var "p2" : !hl.lvalue<!hl.ptr<!hl.int>>
    // CHECK:   BitCast : !hl.ptr<!hl.void> -> !hl.ptr<!hl.int>
    int* p2 = pv; // void* to int*
}
