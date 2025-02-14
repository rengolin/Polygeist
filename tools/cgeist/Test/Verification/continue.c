// RUN: cgeist %s --function=* -S | FileCheck %s

int get();
void other();

int checkCmdLineFlag(const int argc) {
  int bFound = 0;

    for (int i = 1; i < argc; i++) {
      if (get()) {
        bFound = 1;
        continue;
      }
      other();
    }

  return bFound;
}

// CHECK:   func.func @checkCmdLineFlag(%[[arg0:.+]]: i32) -> i32
// CHECK-DAG:     %[[c1:.+]] = arith.constant 1 : index
// CHECK-DAG:     %[[c1_i32:.+]] = arith.constant 1 : i32
// CHECK-DAG:     %[[c0_i32:.+]] = arith.constant 0 : i32
// CHECK-NEXT:     %[[V0:.+]] = arith.index_cast %[[arg0]] : i32 to index
// CHECK-NEXT:     %[[V1:.+]] = scf.for %[[arg1:.+]] = %[[c1]] to %[[V0]] step %[[c1:.+]] iter_args(%[[arg2:.+]] = %[[c0_i32]]) -> (i32) {
// CHECK-NEXT:       %[[V2:.+]] = func.call @get() : () -> i32
// CHECK-NEXT:       %[[V3:.+]] = arith.cmpi ne, %[[V2]], %[[c0_i32]] : i32
// CHECK-NEXT:       %[[V4:.+]] = arith.select %[[V3]], %[[c1_i32]], %[[arg2]] : i32
// CHECK-NEXT:       scf.if %[[V3]] {
// CHECK-NEXT:       } else {
// CHECK-NEXT:         call @other() : () -> ()
// CHECK-NEXT:       }
// CHECK-NEXT:       scf.yield %[[V4]] : i32
// CHECK-NEXT:     }
// CHECK-NEXT:     return %[[V1]] : i32
// CHECK-NEXT:   }
