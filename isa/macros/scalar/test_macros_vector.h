// See LICENSE for license details.

#ifndef __TEST_MACROS_VECTOR_H
#define __TEST_MACROS_VECTOR_H

#-----------------------------------------------------------------------
# Helper macros
#-----------------------------------------------------------------------

# VSEW temporarily hard-coded to 32 bits
#define RVTEST_VSET vsetivli x31, 1, e32, tu, mu;
#define __riscv_vsew 32
#define __riscv_double_vsew 64
#define VSEW_MASK_BITS 0x00000000ffffffff
#define DOUBLE_VSEW_MASK_BITS 0xffffffffffffffff
#define VSET_VSEW vsetivli x31, 1, e32, tu, mu;
#define VSET_VSEW_4AVL vsetivli x31, 4, e32, tu, mu;
#define VSET_DOUBLE_VSEW vsetivli x31, 1, e64, tu, mu;

// #define RVTEST_VSET vsetivli x31, 1, e64, tu, mu;
// #define __riscv_vsew 64
// #define __riscv_double_vsew 128
// #define VSEW_MASK_BITS 0xffffffffffffffff
// #define DOUBLE_VSEW_MASK_BITS 0xffffffffffffffff
// #define VSET_VSEW vsetivli x31, 1, e64, tu, mu;
// #define VSET_VSEW_4AVL vsetivli x31, 4, e64, tu, mu;
// #define VSET_DOUBLE_VSEW vsetivli x31, 1, e128, tu, mu;

#define MASK_VSEW(x)        ((x) & ((1 << (__riscv_vsew - 1) << 1) - 1))
#define MASK_EEW(x, eew)    ((x) & ((1 << (eew - 1) << 1) - 1))
#define MASK_DOUBLE_VSEW(x) ((x) & ((1 << ((__riscv_vsew * 2) - 1) << 1) - 1))
#define MASK_HALF_VSEW(x)   ((x) & ((1 << ((__riscv_vsew / 2) - 1) << 1) - 1))
#define MASK_QUART_VSEW(x)  ((x) & ((1 << ((__riscv_vsew / 4) - 1) << 1) - 1))
#define MASK_EIGHTH_VSEW(x) ((x) & ((1 << ((__riscv_vsew / 8) - 1) << 1) - 1))
#define MASK_XLEN(x)        ((x) & ((1 << (__riscv_xlen - 1) << 1) - 1))
#define MASK_BITS(eew)      ((-1 << (64 - eew)) >> (64 - eew))
#define MK_EEW(eew_num) e##eew_num
#define MK_VLE_INST(eew_num) vle##eew_num.v
#define MK_VSE_INST(eew_num) vse##eew_num.v

#define SEXT_IMM(x)            ((x) | (-(((x) >> 4) & 1) << 4))
#define SEXT_HALF_TO_VSEW(x)   ((x) | (-(((x) >> ((__riscv_vsew / 2) - 1)) & 1) << ((__riscv_vsew / 2) - 1)))
#define SEXT_QUART_TO_VSEW(x)  ((x) | (-(((x) >> ((__riscv_vsew / 4) - 1)) & 1) << ((__riscv_vsew / 4) - 1)))
#define SEXT_EIGHTH_TO_VSEW(x) ((x) | (-(((x) >> ((__riscv_vsew / 8) - 1)) & 1) << ((__riscv_vsew / 8) - 1)))
#define SEXT_DOUBLE_VSEW(x)    ((x) | (-(((x) >> ((__riscv_vsew * 2) - 1)) & 1) << (__riscv_vsew - 1)))
#define ZEXT_VSEW(x)           ((x) | (x) >> (__riscv_vsew - 1))
#define ZEXT_DOUBLE_VSEW(x)    ((x) | (x) >> ((__riscv_vsew * 2) - 1))

# Compare only low VSEW-bits of v14 and correctval
#define VMVXS_AND_MASK_VSEW( targetreg, testreg ) \
    vmv.x.s targetreg, testreg; \
    li x2, VSEW_MASK_BITS; \
    and targetreg, targetreg, x2; \

#define VMVXS_AND_MASK_EEW( targetreg, testreg, eew ) \
    vmv.x.s targetreg, testreg; \
    li x2, MASK_BITS(eew); \
    and targetreg, targetreg, x2; \

#define VMVXS_AND_MASK_DOUBLEVSEW( targetreg, testreg ) \
    vmv.x.s targetreg, testreg; \
    li x2, DOUBLE_VSEW_MASK_BITS; \
    and targetreg, targetreg, x2; \

#define TEST_CASE( testnum, testreg, correctval, code... ) \
test_ ## testnum: \
    code; \
    li x7, MASK_VSEW(correctval); \
    li TESTNUM, testnum; \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    bne x14, x7, fail;

#define TEST_CASE_MEM( testnum, testreg, correctval, eewmem, code... ) \
test_ ## testnum: \
    code; \
    li x7, MASK_VSEW(correctval); \
    li TESTNUM, testnum; \
    VMVXS_AND_MASK_EEW( x14, testreg, eewmem ) \
    bne x14, x7, fail;

#define TEST_CASE_FP_FLAG( testnum, testreg, correctval, flags, code... ) \
test_ ## testnum: \
    code; \
    li x7, MASK_VSEW(correctval); \
    li TESTNUM, testnum; \
    fsflags a1, x0; \
    li a2, flags; \
    bne a1, a2, fail; \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    bne x14, x7, fail;

#define TEST_CASE_W( testnum, testreg, correctval, code... ) \
test_ ## testnum: \
    code; \
    li x7, MASK_DOUBLE_VSEW(correctval); \
    li TESTNUM, testnum; \
    VSET_DOUBLE_VSEW \
    VMVXS_AND_MASK_DOUBLEVSEW(x14, testreg) \
    VSET_VSEW \
    bne x14, x7, fail;

#define TEST_CASE_MASK( testnum, testreg, correctval, code... ) \
test_ ## testnum: \
    code; \
    li x7, correctval; \
    li TESTNUM, testnum; \
    vpopc.m x14, testreg; \
    bne x14, x7, fail;

#define TEST_CASE_MASK_4VL( testnum, testreg, correctval, code... ) \
test_ ## testnum: \
    code; \
    li x7, correctval; \
    li TESTNUM, testnum; \
    VSET_VSEW_4AVL \
    vpopc.m x14, testreg; \
    VSET_VSEW \
    bne x14, x7, fail;

#define TEST_CASE_AVG_VV( testnum, inst, testreg, correctval00, correctval01, correctval10, correctval11, code... ) \
test_ ## testnum: \
    code; \
    li TESTNUM, testnum; \
    csrwi vxrm, 0; \
    inst v14, v2, v1; \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    li x7, MASK_VSEW(correctval00); \
    bne x14, x7, fail; \
    csrwi vxrm, 1; \
    inst v14, v2, v1; \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    li x7, MASK_VSEW(correctval01); \
    bne x14, x7, fail; \
    csrwi vxrm, 2; \
    inst v14, v2, v1; \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    li x7, MASK_VSEW(correctval10); \
    bne x14, x7, fail; \
    csrwi vxrm, 3; \
    inst v14, v2, v1; \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    li x7, MASK_VSEW(correctval11); \
    bne x14, x7, fail;

#define TEST_CASE_AVG_VX( testnum, inst, testreg, correctval00, correctval01, correctval10, correctval11, code... ) \
test_ ## testnum: \
    code; \
    li TESTNUM, testnum; \
    csrwi vxrm, 0; \
    inst v14, v2, x1; \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    li x7, MASK_VSEW(correctval00); \
    bne x14, x7, fail; \
    csrwi vxrm, 1; \
    inst v14, v2, x1; \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    li x7, MASK_VSEW(correctval01); \
    bne x14, x7, fail; \
    csrwi vxrm, 2; \
    inst v14, v2, x1; \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    li x7, MASK_VSEW(correctval10); \
    bne x14, x7, fail; \
    csrwi vxrm, 3; \
    inst v14, v2, x1; \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    li x7, MASK_VSEW(correctval11); \
    bne x14, x7, fail;

#define TEST_CASE_AVG_VI( testnum, inst, testreg, correctval00, correctval01, correctval10, correctval11, val2, code... ) \
test_ ## testnum: \
    code; \
    li TESTNUM, testnum; \
    csrwi vxrm, 0; \
    inst v14, v2, (val2); \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    li x7, MASK_VSEW(correctval00); \
    bne x14, x7, fail; \
    csrwi vxrm, 1; \
    inst v14, v2, (val2); \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    li x7, MASK_VSEW(correctval01); \
    bne x14, x7, fail; \
    csrwi vxrm, 2; \
    inst v14, v2, (val2); \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    li x7, MASK_VSEW(correctval10); \
    bne x14, x7, fail; \
    csrwi vxrm, 3; \
    inst v14, v2, (val2); \
    VMVXS_AND_MASK_VSEW( x14, testreg ) \
    li x7, MASK_VSEW(correctval11); \
    bne x14, x7, fail;

#define TEST_CASE_LOAD( testnum, testreg, eew, correctval1, correctval2, code... ) \
test_ ## testnum: \
    code; \
    li x7, MASK_EEW(correctval1, eew); \
    li TESTNUM, testnum; \
    vsetivli x31, 1, MK_EEW(eew), tu, mu; \
    VMVXS_AND_MASK_EEW( x14, testreg, eew ) \
    VSET_VSEW \
    bne x14, x7, fail; \
    vsetivli x31, 4, MK_EEW(eew), tu, mu; \
    vslidedown.vi v16, testreg, 1; \
    VSET_VSEW \
    li x7, MASK_EEW(correctval2, eew); \
    li TESTNUM, testnum; \
    vsetivli x31, 1, MK_EEW(eew), tu, mu; \
    VMVXS_AND_MASK_EEW( x14, v16, eew ) \
    VSET_VSEW \
    bne x14, x7, fail;

# For simplicity, all vlseg/vsseg test use 3 fields
#define TEST_CASE_VLSEG3( testnum, testreg, eew, correctval1, correctval2, correctval3, code... ) \
test_ ## testnum: \
    code; \
    li x7, MASK_EEW(correctval1, eew); \
    li x8, MASK_EEW(correctval2, eew); \
    li x9, MASK_EEW(correctval3, eew); \
    li TESTNUM, testnum; \
    vsetivli x31, 1, MK_EEW(eew), tu, mu; \
    VMVXS_AND_MASK_EEW( x14, testreg, eew ) \
    VMVXS_AND_MASK_EEW( x15, v15, eew ) \
    VMVXS_AND_MASK_EEW( x16, v16, eew ) \
    VSET_VSEW \
    bne x14, x7, fail; \
    bne x15, x8, fail; \
    bne x16, x9, fail;

# For simplicity, all vlre/vsre test use 2 fields
#define TEST_CASE_VLRE( testnum, eew, correctval1, correctval2, code... ) \
test_ ## testnum: \
    code; \
    li x7, MASK_EEW(correctval1, eew); \
    li x8, MASK_EEW(correctval2, eew); \
    li TESTNUM, testnum; \
    vsetivli x31, 1, MK_EEW(eew), tu, mu; \
    VMVXS_AND_MASK_EEW( x14, v16, eew ) \
    VMVXS_AND_MASK_EEW( x15, v17, eew ) \
    VSET_VSEW \
    bne x14, x7, fail; \
    bne x15, x8, fail; \

# Load from `correctval_addr_reg` to v15 as correctval_vec, compare each element with `testreg`; vl should be set before calling `TEST_CASE_LOOP()`
#define TEST_CASE_LOOP( testnum, testreg, correctval_addr_reg, code...) \
test_ ## testnum: \
    code; \
    csrr x31, vstart; \
    csrr x30, vl; \
    vle32.v v15, (correctval_addr_reg); \
    li TESTNUM, testnum; \
1:  VMVXS_AND_MASK_VSEW( x14, testreg ) \
    VMVXS_AND_MASK_VSEW( x7, v15 ) \
    bne x14, x7, fail; \
    addi x31, x31, 1; \
    vslidedown.vi testreg, testreg, 1; \
    vslidedown.vi v15, v15, 1; \
    bne x31, x30, 1b; \
    VSET_VSEW; 

#define TEST_CASE_LOOP_64( testnum, testreg, correctval_addr_reg, code...) \
test_ ## testnum: \
    code; \
    csrr x31, vstart; \
    csrr x30, vl; \
    vle64.v v15, (correctval_addr_reg); \
    li TESTNUM, testnum; \
1:  VMVXS_AND_MASK_VSEW( x14, testreg ) \
    VMVXS_AND_MASK_VSEW( x7, v15 ) \
    bne x14, x7, fail; \
    addi x31, x31, 1; \
    vslidedown.vi testreg, testreg, 1; \
    vslidedown.vi v15, v15, 1; \
    bne x31, x30, 1b; \
    VSET_VSEW; 

#define TEST_CASE_FP( testnum, testreg, flags, result, val1, val2, code... ) \
test_ ## testnum: \
  li x7, 0; \
  vmv.v.x v14, x7; \
  li  TESTNUM, testnum; \
  la  a0, test_ ## testnum ## _data ;\
  code; \
  vfmv.f.s f3, testreg; \
  feq.s a0, f2, f3; \
  li a3, 1; \
  bne a0, a3, fail; \
  fsflags a1, x0; \
  li a2, flags; \
  bne a1, a2, fail; \
  .pushsection .data; \
  .align 2; \
  test_ ## testnum ## _data: \
  .float val1; \
  .float val2; \
  .result; \
  .popsection

#define TEST_W_CASE_FP( testnum, testreg, flags, result, val1, val2, code... ) \
test_ ## testnum: \
  li x7, 0; \
  vmv.v.x v14, x7; \
  li  TESTNUM, testnum; \
  la  a0, test_ ## testnum ## _data ;\
  code; \
  VSET_DOUBLE_VSEW \
  vfmv.f.s f3, testreg; \
  VSET_VSEW \
  feq.d a0, f2, f3; \
  li a3, 1; \
  bne a0, a3, fail; \
  fsflags a1, x0; \
  li a2, flags; \
  bne a1, a2, fail; \
  .pushsection .data; \
  .align 2; \
  test_ ## testnum ## _data: \
  .float val1; \
  .float val2; \
  .double result; \
  .popsection

# Use feq.d to compare correctval(f2) computed by fadd.d and answer(f3) computed by vfwadd
#define TEST_CASE_W_FP( testnum, testreg, flags, val1, val2, code... ) \
test_ ## testnum: \
  li x7, 0; \
  VSET_DOUBLE_VSEW \
  vmv.v.x v14, x7; \
  VSET_VSEW \
  li  TESTNUM, testnum; \
  la  a0, test_ ## testnum ## _data ;\
  code; \
  VSET_DOUBLE_VSEW \
  vfmv.f.s f3, testreg; \
  VSET_VSEW \
  feq.d a0, f2, f3; \
  li a3, 1; \
  bne a0, a3, fail; \
  fsflags a1, x0; \
  li a2, flags; \
  bne a1, a2, fail; \
  .pushsection .data; \
  .align 2; \
  test_ ## testnum ## _data: \
  .float val1; \
  .float val2; \
  .popsection

#define TEST_CASE_N_FP( testnum, testreg, flags, result, val1, val2, code... ) \
test_ ## testnum: \
  li x7, 0; \
  vmv.v.x v14, x7; \
  li  TESTNUM, testnum; \
  la  a0, test_ ## testnum ## _data ;\
  code; \
  vfmv.f.s f3, testreg; \
  feq.s a0, f2, f3; \
  li a3, 1; \
  bne a0, a3, fail; \
  fsflags a1, x0; \
  li a2, flags; \
  bne a1, a2, fail; \
  .pushsection .data; \
  .align 2; \
  test_ ## testnum ## _data: \
  .double val1; \
  .float result; \
  .float 0; \
  .popsection

# Use feq.d to compare correctval(f2) computed by fadd.d and answer(f3) computed by vfwadd
#define TEST_CASE_WVWF_FP( testnum, testreg, flags, val1, val2, code... ) \
test_ ## testnum: \
  li x7, 0; \
  vmv.v.x v14, x7; \
  li  TESTNUM, testnum; \
  la  a0, test_ ## testnum ## _data ;\
  code; \
  VSET_DOUBLE_VSEW \
  vfmv.f.s f3, testreg; \
  VSET_VSEW \
  feq.d a0, f2, f3; \
  li a3, 1; \
  bne a0, a3, fail; \
  fsflags a1, x0; \
  li a2, flags; \
  bne a1, a2, fail; \
  .pushsection .data; \
  .align 2; \
  test_ ## testnum ## _data: \
  .double val1; \
  .float val2; \
  .float 0; \
  .popsection

#define TEST_CASE_MASK_FP( testnum, testreg, flags, result, val1, val2, code... ) \
test_ ## testnum: \
  li x7, 0; \
  vmv.v.x v14, x7; \
  li  TESTNUM, testnum; \
  la  a0, test_ ## testnum ## _data ;\
  code; \
  li x7, result; \
  vpopc.m x14, testreg; \
  bne x14, x7, fail; \
  fsflags a1, x0; \
  li a2, flags; \
  bne a1, a2, fail; \
  .pushsection .data; \
  .align 2; \
  test_ ## testnum ## _data: \
  .float val1; \
  .float val2; \
  .int result; \
  .popsection

#define TEST_CASE_FP_INT( testnum, testreg, flags, correctval_eew, correctval, val, code... ) \
test_ ## testnum: \
  li x7, 0; \
  vmv.v.x v14, x7; \
  li  TESTNUM, testnum; \
  la  a0, test_ ## testnum ## _data ;\
  code; \
  li x7, MASK_EEW(correctval, correctval_eew); \
  li TESTNUM, testnum; \
  vsetivli x31, 1, MK_EEW(correctval_eew), tu, mu; \
  VMVXS_AND_MASK_EEW( x14, testreg, correctval_eew ) \
  VSET_VSEW \
  bne x14, x7, fail; \
  fsflags a1, x0; \
  li a2, flags; \
  bne a1, a2, fail; \
  .pushsection .data; \
  .align 2; \
  test_ ## testnum ## _data: \
  .float val; \
  .float 0; \
  .popsection

#define TEST_CASE_N_FP_INT( testnum, testreg, flags, correctval_eew, correctval, val, code... ) \
test_ ## testnum: \
  li x7, 0; \
  vmv.v.x v14, x7; \
  li  TESTNUM, testnum; \
  la  a0, test_ ## testnum ## _data ;\
  code; \
  li x7, MASK_EEW(correctval, correctval_eew); \
  li TESTNUM, testnum; \
  vsetivli x31, 1, MK_EEW(correctval_eew), tu, mu; \
  VMVXS_AND_MASK_EEW( x14, testreg, correctval_eew ) \
  VSET_VSEW \
  bne x14, x7, fail; \
  fsflags a1, x0; \
  li a2, flags; \
  bne a1, a2, fail; \
  .pushsection .data; \
  .align 2; \
  test_ ## testnum ## _data: \
  .double val; \
  .popsection

#define TEST_CASE_INT_FP( testnum, testreg, flags, result, val, code... ) \
test_ ## testnum: \
  li x7, 0; \
  vmv.v.x v14, x7; \
  li  TESTNUM, testnum; \
  la  a0, test_ ## testnum ## _data ;\
  code; \
  vfmv.f.s f3, testreg; \
  feq.s a0, f2, f3; \
  li a3, 1; \
  bne a0, a3, fail; \
  fsflags a1, x0; \
  li a2, flags; \
  bne a1, a2, fail; \
  .pushsection .data; \
  .align 2; \
  test_ ## testnum ## _data: \
  .float result; \
  .float 0; \
  .popsection

#define TEST_W_CASE_INT_FP( testnum, testreg, flags, result, val, code... ) \
test_ ## testnum: \
  li x7, 0; \
  vmv.v.x v14, x7; \
  li  TESTNUM, testnum; \
  la  a0, test_ ## testnum ## _data ;\
  code; \
  VSET_DOUBLE_VSEW \
  vfmv.f.s f3, testreg; \
  VSET_VSEW \
  feq.d a0, f2, f3; \
  li a3, 1; \
  bne a0, a3, fail; \
  fsflags a1, x0; \
  li a2, flags; \
  bne a1, a2, fail; \
  .pushsection .data; \
  .align 2; \
  test_ ## testnum ## _data: \
  .double result; \
  .popsection

#-----------------------------------------------------------------------
# Tests for instructions with vector-vector operand
#-----------------------------------------------------------------------

#define TEST_VV_OP( testnum, inst, result, val2, val1 ) \
  TEST_CASE( testnum, v14, result, \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v2, x7; \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    inst v14, v2, v1; \
  )

#define TEST_AVG_VV_OP( testnum, inst, result00, result01, result10, result11, val2, val1 ) \
  TEST_CASE_AVG_VV( testnum, inst, v14, result00, result01, result10, result11, \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v2, x7; \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
  )

#define TEST_ADC_VV_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE( testnum, v14, result, \
    li x7, 0; \
    vmv.v.x v7, x7; \
    vmsne.vi v0, v7, 0; \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v2, x7; \
    inst v14, v1, v2, v0; \
  )

#define TEST_W_AVG_WV_OP( testnum, inst, result00, result01, result10, result11, val2, val1 ) \
  TEST_CASE_AVG_VV( testnum, inst, v14, result00, result01, result10, result11, \
    li x7, MASK_DOUBLE_VSEW(val2); \
    VSET_DOUBLE_VSEW \
    vmv.v.x v2, x7; \
    VSET_VSEW \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
  )

#define TEST_VV_OP_WITH_INIT( testnum, inst, result, val1, val2 ) \
  TEST_CASE( testnum, v14, result, \
    li x7, 0; \
    vmv.v.x v14, x7; \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v2, x7; \
    inst v14, v1, v2; \
  )

#define TEST_VX_OP( testnum, inst, result, val2, val1 ) \
  TEST_CASE( testnum, v14, result, \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v2, x7; \
    li x1, MASK_XLEN(val1); \
    inst v14, v2, x1; \
  )

#define TEST_AVG_VX_OP( testnum, inst, result00, result01, result10, result11, val1, val2 ) \
  TEST_CASE_AVG_VX( testnum, inst, v14, result00, result01, result10, result11, \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v2, x7; \
    li x1, MASK_XLEN(val2); \
  )

# For VX instruction that order of oprands is 'vd, rs1, vs2'(rs-vs), val1-rs1, val2-vs2
#define TEST_VX_OP_RV( testnum, inst, result, val1, val2 ) \
  TEST_CASE( testnum, v14, result, \
    li x7, 0; \
    vmv.v.x v14, x7; \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v1, x7; \
    li x1, MASK_XLEN(val1); \
    inst v14, x1, v1; \
  )

#define TEST_W_AVG_WX_OP( testnum, inst, result00, result01, result10, result11, val2, val1 ) \
  TEST_CASE_AVG_VX( testnum, inst, v14, result00, result01, result10, result11, \
    li x7, MASK_DOUBLE_VSEW(val2); \
    VSET_DOUBLE_VSEW \
    vmv.v.x v2, x7; \
    VSET_VSEW \
    li x1, MASK_VSEW(val1); \
  )

#define TEST_ADC_VX_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE( testnum, v14, result, \
    li x7, 0; \
    vmv.v.x v7, x7; \
    vmsne.vi v0, v7, 0; \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    li x1, MASK_VSEW(val2); \
    inst v14, v1, x1, v0; \
  )

#define TEST_VI_OP( testnum, inst, result, val2, val1 ) \
  TEST_CASE( testnum, v14, result, \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v2, x7; \
    inst v14, v2, SEXT_IMM(val1); \
  )

#define TEST_AVG_VI_OP( testnum, inst, result00, result01, result10, result11, val1, val2 ) \
  TEST_CASE_AVG_VI( testnum, inst, v14, result00, result01, result10, result11, val2, \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v2, x7; \
  )

#define TEST_W_AVG_WI_OP( testnum, inst, result00, result01, result10, result11, val1, val2 ) \
  TEST_CASE_AVG_VI( testnum, inst, v14, result00, result01, result10, result11, val2, \
    li x7, MASK_DOUBLE_VSEW(val1); \
    VSET_DOUBLE_VSEW \
    vmv.v.x v2, x7; \
    VSET_VSEW \
  )

#define TEST_W_VV_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_W( testnum, v14, result, \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v2, x7; \
    inst v14, v1, v2; \
  )

#define TEST_W_VV_OP_WITH_INIT( testnum, inst, result, val1, val2 ) \
  TEST_CASE_W( testnum, v14, result, \
    li x7, 0; \
    VSET_DOUBLE_VSEW \
    vmv.v.x v14, x7; \
    VSET_VSEW \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v2, x7; \
    inst v14, v1, v2; \
  )

#define TEST_W_VX_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_W( testnum, v14, result, \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    li x1, MASK_XLEN(val2); \
    inst v14, v1, x1; \
  )

# For VX instruction that order of oprands is 'vd, rs1, vs2'(rs-vs), val1-rs1, val2-vs2
#define TEST_W_VX_OP_RV( testnum, inst, result, val1, val2 ) \
  TEST_CASE_W( testnum, v14, result, \
    li x7, 0; \
    VSET_DOUBLE_VSEW \
    vmv.v.x v14, x7; \
    VSET_VSEW \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v1, x7; \
    li x1, MASK_XLEN(val1); \
    inst v14, x1, v1; \
  )

#define TEST_W_VI_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_W( testnum, v14, result, \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    inst v14, v1, SEXT_IMM(val2); \
  )

#define TEST_ADC_VI_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE( testnum, v14, result, \
    li x7, 0; \
    vmv.v.x v7, x7; \
    vmsne.vi v0, v7, 0; \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    inst v14, v1, SEXT_IMM(val2), v0; \
  )

#define TEST_N_VV_OP( testnum, inst, result, val2, val1 ) \
  TEST_CASE( testnum, v14, MASK_VSEW(result), \
    li x7, SEXT_DOUBLE_VSEW(val2); \
    vmv.v.x v2, x7; \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    inst v14, v2, v1; \
  )

#define TEST_N_VX_OP( testnum, inst, result, val2, val1 ) \
  TEST_CASE( testnum, v14, MASK_VSEW(result), \
    li x7, SEXT_DOUBLE_VSEW(val2); \
    vmv.v.x v2, x7; \
    li x1, MASK_VSEW(val1); \
    inst v14, v2, x1; \
  )

#define TEST_N_VI_OP( testnum, inst, result, val2, val1 ) \
  TEST_CASE( testnum, v14, MASK_VSEW(result), \
    li x7, SEXT_DOUBLE_VSEW(val2); \
    vmv.v.x v2, x7; \
    inst v14, v2, SEXT_IMM(val1); \
  )

#define TEST_WVU_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE( testnum, v14, result, \
    li x7, ZEXT_DOUBLE_VSEW(val1); \
    vmv.v.x v1, x7; \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v2, x7; \
    inst v14, v1, v2; \
  )

#define TEST_WXU_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE( testnum, v14, result, \
    li x7, ZEXT_DOUBLE_VSEW(val1); \
    vmv.v.x v1, x7; \
    li x1, MASK_XLEN(val2); \
    inst v14, v1, x1; \
  )

#define TEST_WIU_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE( testnum, v14, result, \
    li x7, ZEXT_DOUBLE_VSEW(val1); \
    vmv.v.x v1, x7; \
    inst v14, v1, SEXT_IMM(val2); \
  )

#define TEST_W_WV_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_W( testnum, v14, result, \
    li x7, SEXT_DOUBLE_VSEW(val1); \
    VSET_DOUBLE_VSEW \
    vmv.v.x v2, x7; \
    VSET_VSEW \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v1, x7; \
    inst v14, v2, v1; \
  )

#define TEST_W_WX_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_W( testnum, v14, result, \
    li x7, SEXT_DOUBLE_VSEW(val1); \
    VSET_DOUBLE_VSEW \
    vmv.v.x v2, x7; \
    VSET_VSEW \
    li x1, MASK_XLEN(val2); \
    inst v14, v2, x1; \
  )

#define TEST_W_WI_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_W( testnum, v14, result, \
    li x7, SEXT_DOUBLE_VSEW(val1); \
    VSET_DOUBLE_VSEW \
    vmv.v.x v2, x7; \
    VSET_VSEW \
    inst v14, v2, SEXT_IMM(val2); \
  )

#define TEST_W_WVU_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_W( testnum, v14, result, \
    li x7, ZEXT_DOUBLE_VSEW(val1); \
    VSET_DOUBLE_VSEW \
    vmv.v.x v2, x7; \
    VSET_VSEW \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v1, x7; \
    inst v14, v2, v1; \
  )

#define TEST_W_WXU_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_W( testnum, v14, result, \
    li x7, ZEXT_DOUBLE_VSEW(val1); \
    VSET_DOUBLE_VSEW \
    vmv.v.x v2, x7; \
    VSET_VSEW \
    li x1, MASK_XLEN(val2); \
    inst v14, v2, x1; \
  )

#define TEST_W_WIU_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_W( testnum, v14, result, \
    li x7, ZEXT_DOUBLE_VSEW(val1); \
    vmv.v.x v1, x7; \
    inst v14, v1, SEXT_IMM(val2); \
  )


#define TEST_VZEXT2_OP( testnum, val1 ) \
  TEST_CASE( testnum, v14, ZEXT_VSEW(MASK_HALF_VSEW(val1)), \
    li x7, MASK_HALF_VSEW(val1); \
    vmv.v.x v1, x7; \
    vzext.vf2 v14, v1; \
  )

#define TEST_VSEXT2_OP( testnum, val1 ) \
  TEST_CASE( testnum, v14, SEXT_HALF_TO_VSEW(MASK_HALF_VSEW(val1)), \
    li x7, MASK_HALF_VSEW(val1); \
    vmv.v.x v1, x7; \
    vsext.vf2 v14, v1; \
  )

#define TEST_VZEXT4_OP( testnum, val1 ) \
  TEST_CASE( testnum, v14, ZEXT_VSEW(MASK_QUART_VSEW(val1)), \
    li x7, MASK_QUART_VSEW(val1); \
    vmv.v.x v1, x7; \
    vzext.vf4 v14, v1; \
  )

#define TEST_VSEXT4_OP( testnum, val1 ) \
  TEST_CASE( testnum, v14, SEXT_QUART_TO_VSEW(MASK_QUART_VSEW(val1)), \
    li x7, MASK_QUART_VSEW(val1); \
    vmv.v.x v1, x7; \
    vsext.vf4 v14, v1; \
  )

#define TEST_VZEXT8_OP( testnum, val1 ) \
  TEST_CASE( testnum, v14, ZEXT_VSEW(MASK_EIGHTH_VSEW(val1)), \
    li x7, MASK_EIGHTH_VSEW(val1); \
    vmv.v.x v1, x7; \
    vzext.vf8 v14, v1; \
  )


#define TEST_VSEXT8_OP( testnum, val1 ) \
  TEST_CASE( testnum, v14, SEXT_EIGHTH_TO_VSEW(MASK_EIGHTH_VSEW(val1)), \
    li x7, MASK_EIGHTH_VSEW(val1); \
    vmv.v.x v1, x7; \
    vsext.vf8 v14, v1; \
  )

#define TEST_FP_1OPERAND_OP( testnum, inst, flags, result, val ) \
  TEST_CASE_FP( testnum, v14, flags, float result, val, 0, \
    flw f0, 0(a0); \
    vfmv.s.f v1, f0; \
    flw f2, 8(a0); \
    inst v14, v1; \
  )

#define TEST_W_FP_1OPERAND_OP( testnum, inst, flags, result, val ) \
  TEST_W_CASE_FP( testnum, v14, flags, result, val, 0, \
    flw f0, 0(a0); \
    vfmv.s.f v1, f0; \
    fld f2, 8(a0); \
    inst v14, v1; \
  )

#define TEST_N_FP_1OPERAND_OP( testnum, inst, flags, result, val ) \
  TEST_CASE_N_FP( testnum, v14, flags, result, val, 0, \
    fld f0, 0(a0); \
    VSET_DOUBLE_VSEW \
    vfmv.s.f v2, f0; \
    VSET_VSEW \
    flw f2, 8(a0); \
    inst v14, v2; \
  )

#define TEST_FP_HEX_1OPERAND_OP( testnum, inst, flags, result, val ) \
  TEST_CASE_FP_FLAG( testnum, v14, result, flags, \
    li x7, MASK_VSEW(val); \
    vmv.v.x v1, x7; \
    inst v14, v1; \
  )

#define TEST_VVM_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_MASK( testnum, v14, result, \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v2, x7; \
    inst v14, v1, v2; \
  )

#define TEST_VXM_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_MASK( testnum, v14, result, \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    li x1, MASK_XLEN(val2); \
    inst v14, v1, x1; \
  )

#define TEST_VIM_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_MASK( testnum, v14, result, \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    inst v14, v1, SEXT_IMM(val2); \
  )

#define TEST_ADC_VVM_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_MASK( testnum, v14, result, \
    li x7, 0; \
    vmv.v.x v7, x7; \
    vmsne.vi v0, v7, 0; \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    li x7, MASK_VSEW(val2); \
    vmv.v.x v2, x7; \
    inst v14, v1, v2, v0; \
  )

#define TEST_ADC_VXM_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_MASK( testnum, v14, result, \
    li x7, 0; \
    vmv.v.x v7, x7; \
    vmsne.vi v0, v7, 0; \
    li x7, MASK_VSEW(val1); \
    vmv.v.x v1, x7; \
    li x1, MASK_VSEW(val2); \
    inst v14, v1, x1, v0; \
  )

#define TEST_ADC_VIM_OP( testnum, inst, result, val1, val2 ) \
  TEST_CASE_MASK( testnum, v14, result, \
    li x7, 0; \
    vmv.v.x v7, x7; \
    vmsne.vi v0, v7, 0; \
    li x7, MASK_VSEW(val1); \
    inst v14, v1, SEXT_IMM(val2), v0; \
  )

#define TEST_FP_VVM_OP( testnum, inst, flags, result, val1, val2 ) \
  TEST_CASE_MASK_FP( testnum, v14, flags, result, val1, val2,     \
    flw f0, 0(a0); \
    flw f1, 4(a0); \
    vfmv.s.f v1, f0; \
    vfmv.s.f v2, f1; \
    flw f2, 8(a0); \
    inst v14, v1, v2; \
  )

#define TEST_FP_VFM_OP( testnum, inst, flags, result, val1, val2 ) \
  TEST_CASE_MASK_FP( testnum, v14, flags, result, val1, val2, \
    flw f0, 0(a0); \
    flw f1, 4(a0); \
    vfmv.s.f v1, f0; \
    flw f2, 8(a0); \
    inst v14, v1, f1; \
  )

#define TEST_VLSE_OP( testnum, inst, eew, result1, result2, stride, base ) \
  TEST_CASE_LOAD( testnum, v14, eew, result1, result2, \
    la  x1, base; \
    li  x2, stride; \
    vsetivli x31, 4, MK_EEW(eew), tu, mu; \
    inst v14, (x1), x2; \
    VSET_VSEW \
  )

#define TEST_VLXEI_OP( testnum, inst, index_eew, result1, result2, base_data, base_index ) \
  TEST_CASE_LOAD( testnum, v14, __riscv_vsew, result1, result2, \
    la  x1, base_data; \
    la  x6, base_index; \
    vsetivli x31, 4, MK_EEW(index_eew), tu, mu; \
    MK_VLE_INST(index_eew) v2, (x6); \
    VSET_VSEW_4AVL \
    inst v14, (x1), v2; \
    VSET_VSEW \
  )

#define TEST_VLE_OP( testnum, inst, eew, result1, result2, base ) \
  TEST_CASE_LOAD( testnum, v14, eew, result1, result2, \
    la  x1, base; \
    vsetivli x31, 4, MK_EEW(eew), tu, mu; \
    inst v14, (x1); \
    VSET_VSEW \
  )

#define TEST_VLEFF_OP( testnum, inst, eew, result1, result2, base ) \
  TEST_CASE_LOAD( testnum, v14, eew, result1, result2, \
    la  x1, base; \
    vsetivli x31, 4, MK_EEW(eew), tu, mu; \
    inst v14, (x1); \
    csrr x30, vl; \
    bne x30, x31, fail; \
  )

#define TEST_VLSEG3_OP( testnum, inst, eew, result1, result2, result3, base ) \
  TEST_CASE_VLSEG3( testnum, v14, eew, result1, result2, result3,  \
    la  x1, base; \
    inst v14, (x1); \
  )

#define TEST_VLSEG1_OP( testnum, inst, eew, result, base ) \
  TEST_CASE_MEM( testnum, v14, result, eew,  \
    la  x1, base; \
    inst v14, (x1); \
  )

#define TEST_VLRE2_OP( testnum, inst, eew, result1, result2, base ) \
  TEST_CASE_VLRE( testnum, eew, result1, result2,  \
    la  x1, base; \
    inst v16, (x1); \
  )

#define TEST_VLRE1_OP( testnum, inst, eew, result, base ) \
  TEST_CASE( testnum, v16, result,  \
    la  x1, base; \
    inst v16, (x1); \
  )

#define TEST_VLSSEG3_OP( testnum, inst, eew, result1, result2, result3, stride, base ) \
  TEST_CASE_VLSEG3( testnum, v14, eew, result1, result2, result3,  \
    la  x1, base; \
    li  x2, stride; \
    inst v14, (x1), x2; \
  )

#define TEST_VLSSEG1_OP( testnum, inst, eew, result, stride, base ) \
  TEST_CASE( testnum, v14, result,  \
    la  x1, base; \
    li  x2, stride; \
    inst v14, (x1), x2; \
  )

#define TEST_VLXSEG3_OP( testnum, inst, index_eew, result1, result2, result3, base_data, base_index ) \
  TEST_CASE_VLSEG3( testnum, v14, __riscv_vsew, result1, result2, result3,  \
    la  x1, base_data; \
    la  x6, base_index; \
    MK_VLE_INST(index_eew) v2, (x6); \
    inst v14, (x1), v2; \
  )

#define TEST_VLXSEG1_OP( testnum, inst, index_eew, result, base_data, base_index ) \
  TEST_CASE( testnum, v14, result,  \
    la  x1, base_data; \
    la  x6, base_index; \
    MK_VLE_INST(index_eew) v2, (x6); \
    inst v14, (x1), v2; \
  )

#define TEST_VSSEG3_OP( testnum, load_inst, store_inst, eew, result1, result2, result3, base ) \
  TEST_CASE_VLSEG3( testnum, v14, eew, result1, result2, result3,  \
    la  x1, base; \
    li x7, MASK_EEW(result1, eew); \
    li x8, MASK_EEW(result2, eew); \
    li x9, MASK_EEW(result3, eew); \
    vsetivli x31, 1, MK_EEW(eew), tu, mu; \
    vmv.v.x v1, x7; \
    vmv.v.x v2, x8; \
    vmv.v.x v3, x9; \
    VSET_VSEW \
    store_inst v1, (x1); \
    load_inst v14, (x1); \
  )


#define TEST_VSSEG1_OP( testnum, load_inst, store_inst, eew, result, base ) \
  TEST_CASE_MEM( testnum, v14, result, eew,  \
    la  x1, base; \
    li x7, MASK_EEW(result, eew); \
    vsetivli x31, 1, MK_EEW(eew), tu, mu; \
    vmv.v.x v1, x7; \
    VSET_VSEW \
    store_inst v1, (x1); \
    load_inst v14, (x1); \
  )

#define TEST_VSSSEG3_OP( testnum, load_inst, store_inst, eew, result1, result2, result3, stride, base ) \
  TEST_CASE_VLSEG3( testnum, v14, eew, result1, result2, result3,  \
    la  x1, base; \
    li  x2, stride; \
    li x7, MASK_EEW(result1, eew); \
    li x8, MASK_EEW(result2, eew); \
    li x9, MASK_EEW(result3, eew); \
    vsetivli x31, 1, MK_EEW(eew), tu, mu; \
    vmv.v.x v1, x7; \
    vmv.v.x v2, x8; \
    vmv.v.x v3, x9; \
    VSET_VSEW \
    store_inst v1, (x1), x2; \
    load_inst v14, (x1), x2; \
  )

#define TEST_VSSSEG1_OP( testnum, load_inst, store_inst, eew, result, stride, base ) \
  TEST_CASE_MEM( testnum, v14, result, eew,  \
    la  x1, base; \
    li  x2, stride; \
    li x7, MASK_EEW(result, eew); \
    vsetivli x31, 1, MK_EEW(eew), tu, mu; \
    vmv.v.x v1, x7; \
    VSET_VSEW \
    store_inst v1, (x1), x2; \
    load_inst v14, (x1), x2; \
  )

#define TEST_VSXSEG3_OP( testnum, load_inst, store_inst, index_eew, result1, result2, result3, base_data, base_index ) \
  TEST_CASE_VLSEG3( testnum, v14, __riscv_vsew, result1, result2, result3,  \
    la  x1, base_data; \
    la  x6, base_index; \
    MK_VLE_INST(index_eew) v5, (x6); \
    li x7, MASK_VSEW(result1); \
    li x8, MASK_VSEW(result2); \
    li x9, MASK_VSEW(result3); \
    vmv.v.x v1, x7; \
    vmv.v.x v2, x8; \
    vmv.v.x v3, x9; \
    store_inst v1, (x1), v5; \
    load_inst v14, (x1), v5; \
  )

#define TEST_VSXSEG1_OP( testnum, load_inst, store_inst, index_eew, result, base_data, base_index ) \
  TEST_CASE( testnum, v14, result,  \
    la  x1, base_data; \
    la  x6, base_index; \
    MK_VLE_INST(index_eew) v5, (x6); \
    li x7, MASK_VSEW(result); \
    vmv.v.x v1, x7; \
    store_inst v1, (x1), v5; \
    load_inst v14, (x1), v5; \
  )

#define TEST_VSSE_OP( testnum, load_inst, store_inst, eew, result, stride, base ) \
  TEST_CASE_MEM( testnum, v14, result, eew, \
    la  x1, base; \
    li  x2, stride; \
    li  x3, result; \
    vsetivli x31, 1, MK_EEW(eew), tu, mu; \
    vmv.v.x v1, x3; \
    VSET_VSEW \
    store_inst v1, (x1), x2; \
    load_inst v14, (x1), x2; \
  )

#define TEST_VSE_OP( testnum, load_inst, store_inst, eew, result, base ) \
  TEST_CASE_MEM( testnum, v14, result, eew, \
    la  x1, base; \
    li  x3, result; \
    vsetivli x31, 1, MK_EEW(eew), tu, mu; \
    vmv.v.x v1, x3; \
    VSET_VSEW \
    store_inst v1, (x1); \
    load_inst v14, (x1); \
  )

#define TEST_VSXEI_OP( testnum, load_inst, store_inst, index_eew, result, base_data, base_index ) \
  TEST_CASE( testnum, v14, result, \
    la  x1, base_data; \
    la  x6, base_index; \
    MK_VLE_INST(index_eew) v2, (x6); \
    li  x3, result; \
    vmv.v.x v1, x3; \
    store_inst v1, (x1), v2; \
    load_inst v14, (x1), v2; \
  )

#define TEST_VSRE2_OP( testnum, load_inst, store_inst, eew, result1, result2, base ) \
  TEST_CASE_VLRE( testnum, eew, result1, result2,  \
    la  x1, base; \
    li x7, MASK_EEW(result1, eew); \
    li x8, MASK_EEW(result2, eew); \
    vsetivli x31, 1, MK_EEW(eew), tu, mu; \
    vmv.v.x v8, x7; \
    vmv.v.x v9, x8; \
    VSET_VSEW \
    store_inst v8, (x1); \
    load_inst v16, (x1); \
  )

#define TEST_VSRE1_OP( testnum, load_inst, store_inst, eew, result, base ) \
  TEST_CASE( testnum, v16, result,  \
    la  x1, base; \
    li x7, MASK_EEW(result, eew); \
    vsetivli x31, 1, MK_EEW(eew), tu, mu; \
    vmv.v.x v8, x7; \
    VSET_VSEW \
    store_inst v8, (x1); \
    load_inst v16, (x1); \
  )

#define TEST_FP_VV_OP( testnum, inst, flags, result, val1, val2 ) \
  TEST_CASE_FP( testnum, v14, flags, float result, val1, val2,     \
    flw f0, 0(a0); \
    flw f1, 4(a0); \
    vfmv.s.f v1, f0; \
    vfmv.s.f v2, f1; \
    flw f2, 8(a0); \
    inst v14, v1, v2; \
  )

#define TEST_FP_VF_OP( testnum, inst, flags, result, val1, val2 ) \
  TEST_CASE_FP( testnum, v14, flags, float result, val1, val2,     \
    flw f0, 0(a0); \
    flw f1, 4(a0); \
    vfmv.s.f v1, f0; \
    flw f2, 8(a0); \
    inst v14, v1, f1; \
  )

#define TEST_FP_VF_OP_AFTER_VMSEQ( testnum, flags, result, val1, val2, vmseqop1, vmseqop2 ) \
  TEST_CASE_FP( testnum, v14, flags, float result, val1, val2,     \
    li x7, MASK_VSEW(vmseqop1); \
    vmv.v.x v1, x7; \
    vmseq.vi v0, v1, vmseqop2; \
    flw f0, 0(a0); \
    flw f1, 4(a0); \
    vfmv.s.f v1, f0; \
    flw f2, 8(a0); \
    vfmerge.vfm v14, v1, f1, v0; \
  )

# For VF instruction that order of oprands is 'vd, rs1, vs2'(rs-vs), val1-rs1, val2-vs2
#define TEST_FP_VF_OP_RV( testnum, inst, flags, result, val1, val2 ) \
  TEST_CASE_FP( testnum, v14, flags, float result, val1, val2,     \
    flw f0, 0(a0); \
    flw f1, 4(a0); \
    vfmv.s.f v1, f0; \
    flw f2, 8(a0); \
    inst v14, f1, v1; \
  )

# Correctval is computed by fadd.d((widen)f0, (widen)f1) -> f2
#define TEST_W_FP_VV_OP( testnum, inst, finst, flags, val1, val2 ) \
  TEST_CASE_W_FP( testnum, v14, flags, val1, val2,     \
    flw f0, 0(a0); \
    flw f1, 4(a0); \
    vfmv.s.f v1, f0; \
    vfmv.s.f v2, f1; \
    fcvt.d.s f0, f0; \
    fcvt.d.s f1, f1; \
    finst f2, f0, f1; \
    inst v14, v1, v2; \
  )

#define TEST_W_FP_VV_OP_NEGRESULT( testnum, inst, finst, flags, val1, val2 ) \
  TEST_CASE_W_FP( testnum, v14, flags, val1, val2,     \
    flw f0, 0(a0); \
    flw f1, 4(a0); \
    vfmv.s.f v1, f0; \
    vfmv.s.f v2, f1; \
    fcvt.d.s f0, f0; \
    fcvt.d.s f1, f1; \
    finst f2, f0, f1; \
    fneg.d f2, f2; \
    inst v14, v1, v2; \
  )

# Correctval is computed by fadd.d((widen)f0, (widen)f4) -> f2
#define TEST_W_FP_VF_OP( testnum, inst, finst, flags, val1, val2 ) \
  TEST_CASE_W_FP( testnum, v14, flags, val1, val2,     \
    flw f0, 0(a0); \
    flw f1, 4(a0); \
    flw f4, 4(a0); \
    vfmv.s.f v1, f0; \
    fcvt.d.s f0, f0; \
    fcvt.d.s f4, f4; \
    finst f2, f0, f4; \
    inst v14, v1, f1; \
  )

#define TEST_W_FP_VF_OP_RV( testnum, inst, finst, flags, val1, val2 ) \
  TEST_CASE_W_FP( testnum, v14, flags, val1, val2,     \
    flw f0, 0(a0); \
    flw f1, 4(a0); \
    flw f4, 4(a0); \
    vfmv.s.f v1, f0; \
    fcvt.d.s f0, f0; \
    fcvt.d.s f4, f4; \
    finst f2, f0, f4; \
    inst v14, f1, v1; \
  )

#define TEST_W_FP_VF_OP_RV_NEGRESULT( testnum, inst, finst, flags, val1, val2 ) \
  TEST_CASE_W_FP( testnum, v14, flags, val1, val2,     \
    flw f0, 0(a0); \
    flw f1, 4(a0); \
    flw f4, 4(a0); \
    vfmv.s.f v1, f0; \
    fcvt.d.s f0, f0; \
    fcvt.d.s f4, f4; \
    finst f2, f0, f4; \
    fneg.d f2, f2; \
    inst v14, f1, v1; \
  )

# Operand order: inst val1, val2
#define TEST_W_FP_WV_OP( testnum, inst, finst, flags, val1, val2 ) \
  TEST_CASE_WVWF_FP( testnum, v14, flags, val1, val2,  \
    fld f0, 0(a0); \
    flw f1, 8(a0); \
    flw f4, 8(a0); \
    VSET_DOUBLE_VSEW \
    vfmv.s.f v2, f0; \
    VSET_VSEW \
    vfmv.s.f v1, f1; \
    fcvt.d.s f4, f4; \
    finst f2, f0, f4; \
    inst v14, v2, v1; \
  )

# For Widen-Floating instruction that order of oprands is 'vd(2*SEW), vs2(SEW), vs1(2*SEW)'
#define TEST_W_FP_WV_OP_DS( testnum, inst, finst, flags, val1, val2 ) \
  TEST_CASE_WVWF_FP( testnum, v14, flags, val1, val2,  \
    fld f0, 0(a0); \
    flw f1, 8(a0); \
    flw f4, 8(a0); \
    VSET_DOUBLE_VSEW \
    vfmv.s.f v1, f0; \
    VSET_VSEW \
    vfmv.s.f v2, f1; \
    fcvt.d.s f4, f4; \
    finst f2, f0, f4; \
    inst v14, v2, v1; \
  )

# For Widen-Floating instruction that order of oprands is 'vd(2*SEW), vs2(2*SEW), vs1(SEW)'
#define TEST_W_FP_WF_OP( testnum, inst, finst, flags, val1, val2 ) \
  TEST_CASE_WVWF_FP( testnum, v14, flags, val1, val2,  \
    fld f0, 0(a0); \
    flw f1, 8(a0); \
    flw f4, 8(a0); \
    VSET_DOUBLE_VSEW \
    vfmv.s.f v2, f0; \
    VSET_VSEW \
    fcvt.d.s f4, f4; \
    finst f2, f0, f4; \
    inst v14, v2, f1; \
  )

#define TEST_FP_INT_OP( testnum, inst, flags, result, val ) \
  TEST_CASE_FP_INT( testnum, v14, flags, __riscv_vsew, result, val,   \
    flw f0, 0(a0); \
    vfmv.s.f v1, f0; \
    inst v14, v1; \
  )

#define TEST_W_FP_INT_OP( testnum, inst, flags, result, val ) \
  TEST_CASE_FP_INT( testnum, v14, flags, __riscv_double_vsew, result, val,   \
    flw f0, 0(a0); \
    vfmv.s.f v1, f0; \
    inst v14, v1; \
  )

#define TEST_N_FP_INT_OP( testnum, inst, flags, result, val ) \
  TEST_CASE_N_FP_INT( testnum, v14, flags, __riscv_vsew, result, val,   \
    fld f0, 0(a0); \
    VSET_DOUBLE_VSEW \
    vfmv.s.f v2, f0; \
    VSET_VSEW \
    inst v14, v2; \
  )

#define TEST_INT_FP_OP( testnum, inst, flags, result, val ) \
  TEST_CASE_INT_FP( testnum, v14, flags, result, val,   \
    li x7, MASK_VSEW(val); \
    vmv.v.x v1, x7; \
    flw f2, 0(a0); \
    inst v14, v1; \
  )

#define TEST_W_INT_FP_OP( testnum, inst, flags, result, val ) \
  TEST_W_CASE_INT_FP( testnum, v14, flags, result, val,   \
    li x7, MASK_VSEW(val); \
    vmv.v.x v1, x7; \
    fld f2, 0(a0); \
    inst v14, v1; \
  )

#define TEST_N_INT_FP_OP( testnum, inst, flags, result, val ) \
  TEST_CASE_INT_FP( testnum, v14, flags, result, val,  \
    li x7, MASK_DOUBLE_VSEW(val); \
    VSET_DOUBLE_VSEW \
    vmv.v.x v2, x7; \
    VSET_VSEW \
    flw f2, 0(a0); \
    inst v14, v2; \
  )

#define TEST_VMRL_OP( testnum, inst, sew, result, src1_addr, src2_addr ) \
  TEST_CASE_MASK_4VL( testnum, v14, result, \
    VSET_VSEW_4AVL \
    la  x1, src1_addr; \
    MK_VLE_INST(sew) v5, (x1); \
    la  x1, src2_addr; \
    MK_VLE_INST(sew) v6, (x1); \
    vmseq.vi v1, v5, 1; \
    vmseq.vi v2, v6, 1; \
    inst v14, v1, v2; \
    VSET_VSEW \
  )

#define TEST_VSFMB_OP( testnum, inst, result, src1_addr ) \
  TEST_CASE_MASK_4VL( testnum, v14, result, \
    VSET_VSEW_4AVL \
    la  x1, src1_addr; \
    vle32.v v5, (x1); \
    vmseq.vi v1, v5, 1; \
    inst v14, v1; \
    VSET_VSEW \
  )

#define TEST_VSFMB_OP_64( testnum, inst, result, src1_addr ) \
  TEST_CASE_MASK_4VL( testnum, v14, result, \
    VSET_VSEW_4AVL \
    la  x1, src1_addr; \
    vle64.v v5, (x1); \
    vmseq.vi v1, v5, 1; \
    inst v14, v1; \
    VSET_VSEW \
  )

#define TEST_VIOTA_OP( testnum, inst, result_addr, src1_addr ) \
  TEST_CASE_LOOP( testnum, v14, x7, \
    VSET_VSEW_4AVL \
    la  x1, src1_addr; \
    la  x7, result_addr; \
    vle32.v v5, (x1); \
    vmseq.vi v0, v5, 1; \
    inst v14, v0; \
  )

#define TEST_VID_OP( testnum, inst, result_addr, src1_addr ) \
  TEST_CASE_LOOP( testnum, v0, x7, \
    VSET_VSEW_4AVL \
    la  x1, src1_addr; \
    la  x7, result_addr; \
    vle32.v v5, (x1); \
    vmseq.vi v0, v5, 1; \
    inst v0; \
  )
#define TEST_VID_OP_64( testnum, inst, result_addr, src1_addr ) \
  TEST_CASE_LOOP( testnum, v0, x7, \
    VSET_VSEW_4AVL \
    la  x1, src1_addr; \
    la  x7, result_addr; \
    vle64.v v5, (x1); \
    vmseq.vi v0, v5, 1; \
    inst v0; \
  )

#define TEST_VCOMPRESS_OP( testnum, inst, result_addr, src1_addr, src2_addr, vm_addr ) \
  TEST_CASE_LOOP( testnum, v6, x7, \
    VSET_VSEW_4AVL \
    la  x7, result_addr; \
    la  x1, src1_addr; \
    la  x2, src2_addr; \
    la  x3, vm_addr; \
    vle32.v v5, (x1); \
    vle32.v v6, (x2); \
    vle32.v v0, (x3); \
    inst v6, v5, v0; \
  )

#define TEST_VCOMPRESS_OP_64( testnum, inst, result_addr, src1_addr, src2_addr, vm_addr ) \
  TEST_CASE_LOOP_64( testnum, v6, x7, \
    VSET_VSEW_4AVL \
    la  x7, result_addr; \
    la  x1, src1_addr; \
    la  x2, src2_addr; \
    la  x3, vm_addr; \
    vle64.v v5, (x1); \
    vle64.v v6, (x2); \
    vle64.v v0, (x3); \
    inst v6, v5, v0; \
  )

#define TEST_VPOPC_OP( testnum, inst, result, vm_addr ) \
    VSET_VSEW_4AVL \
    la  x2, vm_addr; \
    vle32.v v14, (x2); \
    li x7, result; \
    li TESTNUM, testnum; \
    inst x14, v14; \
    VSET_VSEW \
    bne x14, x7, fail;

#define TEST_VPOPC_OP_64( testnum, inst, result, vm_addr ) \
    VSET_VSEW_4AVL \
    la  x2, vm_addr; \
    vle64.v v14, (x2); \
    li x7, result; \
    li TESTNUM, testnum; \
    inst x14, v14; \
    VSET_VSEW \
    bne x14, x7, fail;

#define TEST_VMV_OP( testnum, result ) \
    li TESTNUM, testnum; \
    li x7, MASK_VSEW(result); \
    li x8, 0; \
    vmv.s.x v14, x7; \
    VMVXS_AND_MASK_VSEW( x8, v14 ) \
    bne x8, x7, fail;

#define TEST_VFMVS_OP( testnum, base ) \
    li TESTNUM, testnum; \
    la a0, base; \
    flw f7, 0(a0); \
    vfmv.s.f v14, f7; \
    vfmv.f.s f8, v14; \
    fcvt.w.s x8, f8; \
    fcvt.w.s x7, f7; \
    bne x8, x7, fail;

#define TEST_VFMVF_OP( testnum, base ) \
    li TESTNUM, testnum; \
    la a0, base; \
    flw f7, 0(a0); \
    vfmv.v.f v14, f7; \
    vfmv.f.s f8, v14; \
    fcvt.w.s x8, f8; \
    fcvt.w.s x7, f7; \
    bne x8, x7, fail;

#define TEST_VMRE1_OP( testnum, inst, result, base ) \
  TEST_CASE( testnum, v18, result,  \
    la  x1, base; \
    vl8re32.v v8, (x1); \
    inst v18, v8; \
  )

#define TEST_VMRE2_OP( testnum, inst, result, base ) \
  TEST_CASE( testnum, v19, result,  \
    la  x1, base; \
    vl8re32.v v8, (x1); \
    inst v18, v8; \
  )

#define TEST_VMRE4_OP( testnum, inst, result, base ) \
  TEST_CASE( testnum, v15, result,  \
    la  x1, base; \
    vl8re32.v v8, (x1); \
    inst v12, v8; \
  )

#define TEST_VMRE8_OP( testnum, inst, result, base ) \
  TEST_CASE( testnum, v23, result,  \
    la  x1, base; \
    vl8re32.v v8, (x1); \
    inst v16, v8; \
  )

#define TEST_VSLIDE_VX_OP( testnum, inst, result, val2, base ) \
  TEST_CASE( testnum, v14, result, \
    la  x1, base; \
    VSET_VSEW_4AVL \
    vle32.v v1, (x1); \
    li x1, MASK_VSEW(val2); \
    inst v14, v1, x1; \
    VSET_VSEW \
  )

#define TEST_VSLIDE_VI_OP( testnum, inst, result, val2, base ) \
  TEST_CASE( testnum, v14, result, \
    la  x1, base; \
    VSET_VSEW_4AVL \
    vle32.v v1, (x1); \
    inst v14, v1, val2; \
    VSET_VSEW \
  )

#define TEST_VSLIDE_VF_OP( testnum, inst, flags, result, base, freg_base ) \
  TEST_CASE( testnum, v14, result, \
    la a0, freg_base; \
    flw f1, 0(a0); \
    la  x1, base; \
    VSET_VSEW_4AVL \
    vle32.v v1, (x1); \
    inst v14, v1, f1; \
    VSET_VSEW \
  )

#-----------------------------------------------------------------------
# Pass and fail code (assumes test num is in TESTNUM)
#-----------------------------------------------------------------------

#define TEST_PASSFAIL \
        bne x0, TESTNUM, pass; \
fail: \
        RVTEST_FAIL; \
pass: \
        RVTEST_PASS \


#-----------------------------------------------------------------------
# Test data section
#-----------------------------------------------------------------------

#define TEST_DATA

#endif
