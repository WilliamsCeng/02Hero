############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
set_directive_unroll "addRoundKey/addRoundKey_label6"
set_directive_unroll "leftLoop4int/leftLoop4int_label3"
set_directive_unroll "T/T_label4"
set_directive_unroll "subBytes/subBytes_label7"
set_directive_loop_tripcount -max 16 "deAes_return/deAes_return_label24"
set_directive_loop_tripcount -max 16 "aes_return/aes_return_label23"
set_directive_resource -core RAM_2P_BRAM "getNumFromSBox" S
set_directive_resource -core RAM_2P_BRAM "getNumFromS1Box" S2
set_directive_unroll "convertToIntArray/convertToIntArray_label0"
set_directive_unroll "convertToIntArray/convertToIntArray_label1"
set_directive_array_partition -type complete -dim 1 "aes_return" pArray
set_directive_unroll "addRoundKey/addRoundKey_label0"
set_directive_pipeline "aes_return/aes_return_label23"
set_directive_array_partition -type complete -dim 1 "T" numArray
set_directive_array_partition -type complete -dim 1 "leftLoop4int" temp
set_directive_pipeline -rewind "extendKey/extendKey_label0"
set_directive_array_partition -type complete -dim 1 "extendKey" w
set_directive_array_partition -type complete -dim 1 "addRoundKey" warray
set_directive_unroll "leftLoop4int/leftLoop4int_label2"
set_directive_array_partition -type complete -dim 1 "shiftRows" rowTwo
set_directive_array_partition -type complete -dim 1 "shiftRows" rowThree
set_directive_array_partition -type complete -dim 1 "shiftRows" rowFour
set_directive_dataflow "shiftRows"
set_directive_unroll "shiftRows/shiftRows_label1"
set_directive_unroll "shiftRows/shiftRows_label8"
set_directive_array_partition -type block -factor 4 -dim 2 "mixColumns" colM
set_directive_array_partition -type complete -dim 1 "mixColumns" tempArray
set_directive_unroll "convertArrayToStr/convertArrayToStr_label11"
set_directive_unroll "convertArrayToStr/convertArrayToStr_label2"
set_directive_array_partition -type complete -dim 1 "deAes_return" cArray
set_directive_array_partition -type complete -dim 1 "deShiftRows" rowTwo
set_directive_array_partition -type complete -dim 1 "deShiftRows" rowThree
set_directive_array_partition -type complete -dim 1 "deShiftRows" rowFour
set_directive_unroll "deShiftRows/deShiftRows_label16"
set_directive_unroll "deShiftRows/deShiftRows_label17"
set_directive_array_partition -type block -factor 4 -dim 2 "deMixColumns" deColM
set_directive_array_partition -type complete -dim 1 "deMixColumns" tempArray
set_directive_pipeline -rewind "mixColumns/mixColumns_label3"
set_directive_unroll "mixColumns/mixColumns_label4"
set_directive_unroll "extendKey/extendKey_label5"
set_directive_unroll "subBytes/subBytes_label0"
set_directive_unroll "deSubBytes/deSubBytes_label1"
set_directive_unroll "deSubBytes/deSubBytes_label13"
set_directive_dataflow "deShiftRows"
set_directive_array_partition -type complete -dim 1 "rightLoop4int" temp
set_directive_unroll "rightLoop4int/rightLoop4int_label14"
set_directive_unroll "rightLoop4int/rightLoop4int_label15"
set_directive_unroll "addRoundTowArray/addRoundTowArray_label2"
set_directive_unroll "addRoundTowArray/addRoundTowArray_label20"
set_directive_array_partition -type complete -dim 1 "deAes_return/deAes_return_label24" wArray
set_directive_array_partition -type complete -dim 1 "getArrayFrom4W" colOne
set_directive_array_partition -type complete -dim 1 "getArrayFrom4W" colTwo
set_directive_array_partition -type complete -dim 1 "getArrayFrom4W" colThree
set_directive_array_partition -type complete -dim 1 "getArrayFrom4W" colFour
set_directive_unroll "getArrayFrom4W/getArrayFrom4W_label21"
set_directive_unroll "mixColumns/mixColumns_label9"
set_directive_unroll "deMixColumns/deMixColumns_label5"
set_directive_unroll "deMixColumns/deMixColumns_label6"
set_directive_unroll "deMixColumns/deMixColumns_label18"
