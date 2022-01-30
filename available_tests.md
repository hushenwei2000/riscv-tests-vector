# Available_Tests

## Chap11: Integer Arithmetic

|                               |            |           |            |          |        |         |          |
| ----------------------------- | ---------- | --------- | ---------- | -------- | ------ | ------- | -------- |
| <font color="green"> vadd | <font color="green"> vsub | <font color="green"> vrsub     | <font color="green"> vmul       | <font color="green"> vwmul    | <font color="green"> vdiv   | <font color="green"> vrem    | <font color="green"> vremu    |
| <font color="green"> vmacc                         | <font color="green"> vmadd      | <font color="green"> vnmsac    | <font color="green"> vnmsub     | <font color="green"> vwmaccu  | <font color="green"> vwmacc | <font color="green"> vwmaccsu | <font color="green"> vwmaccus |
| <font color="green"> vmseq                         | <font color="green"> vmsgt      | <font color="green"> vmsle     | <font color="green"> vmsleu     | <font color="green"> vmslt    | <font color="green"> vmsltu | <font color="green"> vmsne   | <font color="green"> vmax     |
| <font color="green"> vmaxu                         | <font color="green"> vmin       | <font color="green"> vminu     | <font color="green"> vand       | <font color="green"> vor      | <font color="green"> vxor   | <font color="green"> vsll    | <font color="green"> vext     |
| <font color="green"> vmsgtu                        | <font color="green"> vmulh_32   | <font color="green"> vmulhu_32 | <font color="green"> vmulhsu_32 | <font color="green"> vdivu_32 |  <font color="green"> vwadd      |  <font color="green"> vadc    |   <font color="green"> vmadc_32     |
| <font color="green"> vwsub      | <font color="green"> vwaddu   | <font color="green"> vwsubu | <font color="green"> vsrl | <font color="green"> vsra |     |     |      |

- Note1: Currently VSEW=32, if need to test VSEW=64, you should change Makefrag to compile `vmulh_64.S` (vmulh, vmulhsu, vdivu are the same)

## Chap13: Floating-Point
**`Floating-point tests *ONLY* support *VSEW=32* so far.`**
|         |          |          |        |         |         |         |   |
| -----   | -        | -        | -      | -       | -       | -       | - |
| <font color="green"> vfadd   | <font color="green"> vfsub   | <font color="green"> vfmul    | <font color="green"> vfdiv | <font color="green"> vfwadd | <font color="green"> vfwmul | <font color="green"> vfwsub | <font color="green"> vfmax  |
| <font color="green"> vfmacc  | <font color="green"> vfnmacc  | <font color="green"> vfmsac   | <font color="green"> vfnmsac| <font color="green"> vfmadd  | <font color="green"> vfnmadd | <font color="green"> vfmsub  | <font color="green"> vfmin  |
| <font color="green"> vfnmsub | <font color="green"> vfwmacc  | <font color="green"> vfwmsac  |<font color="green"> vfwnmacc| <font color="green"> vfwnmsac | <font color="green"> vfsqrt  | <font color="green"> vfsgnj  | <font color="green"> vmfeq  |
| <font color="green"> vmflt   | <font color="green"> vmfgt    | <font color="green"> vfclass  | <font color="green"> vfmerge| <font color="green"> vfcvt   | <font color="green"> vfwcvt   | <font color="green"> vfrec     |  <font color="green"> vfncvt      |

## Chap7: Load and Store

|       |       |   |   |   |   |   |   |
| ----- | ----- | - | - | - | - | - | - |
| <font color="yellow"> vlse | <font color="yellow">vsse | <font color="yellow">vle  | <font color="green">vse  | <font color="yellow"> vlxei  | <font color="green"> vsxei  | <font color="yellow"> vlseg   | <font color="yellow"> vsseg  |
| <font color="yellow"> vlsseg| <font color="yellow"> vssseg| <font color="green"> vlxseg| <font color="yellow"> vsxseg| <font color="yellow">vlre |  <font color="yellow">vle8ff     |   <font color="yellow">vle16ff  | <font color="yellow">vle32ff        | 
|<font color="yellow">vle64ff||||||||

- Note1: Unfinish: 7.4(vlm.v vsm.v unrecognized opcode)
- Note2: 'Seg' load& store cant pass when EEW=64
- Note3: yellow means that our model can only pass p-test

## Chap14: Vector Reduction Operations

|         |          |          |         |         |   |   |   |
| ------- | -------- | -------- | ------- | ------- | - | - | - |
| <font color="green"> vredand  | <font color="green"> vredmaxu | <font color="green"> vredminu  | <font color="green"> vredsum   | <font color="green"> vredxor    | <font color="green"> vredmax   | <font color="green"> vredmin     | <font color="green"> vredor |
| <font color="green"> vfredmax | <font color="green"> vfredmin | <font color="green"> vfredosum | <font color="green"> vfredusum |  <font color="green"> vwredsum  | <font color="green"> vwredsumu | <font color="green"> vfwredsum*  |        |
- Note1: vfwredusum(unrecognized opcode)

## Chap12: Vector Fixed-Point Arithmetic Instructions

|       |       |   |   |   |   |   |   |
| ----- | ----- | - | - | - | - | - | - |
| <font color="green"> vsadd | <font color="green"> vsaddu | <font color="green"> vssub  | <font color="green"> vssubu  | <font color="green"> vaadd  | <font color="green"> vaaddu  | <font color="green"> vasub   | <font color="green"> vasubu  |
| <font color="green"> vsmul | <font color="green"> vssra     | <font color="green"> vssrl  | <font color="green"> vnclipu  | <font color="green"> vnclip  |   |   |   |

## Chap15: Vector Mask Instructions

|       |       |   |   |   |   |   |   |
| ----- | ----- | - | - | - | - | - | - |
| <font color="yellow"> vmrl | <font color="yellow"> vsfmb | <font color="yellow"> viota| <font color="yellow"> vpopc |   |   |   |   |
|      |       |      |   |   |   |   |   |

## Chap16: Vector Permutation Instructions

|       |       |   |   |   |   |   |   |
| ----- | ----- | - | - | - | - | - | - |
| <font color="green"> vrgather | <font color="yellow"> vcompress | <font color="yellow"> vslide | <font color="yellow"> vmvre | <font color="green"> vmv |   |   |   |
|          |           |        |       |     |   |   |   |
