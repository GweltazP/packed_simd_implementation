// Copyright 2018 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
//
// Author: Florian Zaruba <zarubaf@iis.ee.ethz.ch>
//
// Description: Multiplication Unit with one pipeline register
//              This unit relies on retiming features of the synthesizer
//


module multiplier_zpsf
  import ariane_pkg::*;
#(
    parameter config_pkg::cva6_cfg_t CVA6Cfg = config_pkg::cva6_cfg_empty
) (
    input  logic                             clk_i,
    input  logic                             rst_ni,
    input  logic         [TRANS_ID_BITS-1:0] trans_id_i,
    input  logic                             mult_valid_i,
    input  fu_op                             operation_i,
    input  riscv::xlen_t                     operand_a_i,
    input  riscv::xlen_t                     operand_b_i,
    input  riscv::xlen_t                     operand_c_i,       // the third operand for SMAQA
    input  riscv::xlen_t                     operand_d_i,       // fourth operand for SMAQA64 (rs1 + 1)
    input  riscv::xlen_t                     operand_e_i,       // fifth operand for SMAQA64 (rs2 + 1)
    output riscv::xlen_t                     result_o,
    // output riscv::xlen_t                     result_simd_o,     // 2nd half of the result [31:0] // CHANGED: added for SIMD
    output logic                             overflow_o,        // CHANGED: added for SIMD
    // output logic                             is_64bits_o,       // CHANGED: added for SIMD
    output logic                             mult_valid_o,
    output logic                             mult_ready_o,
    output logic         [TRANS_ID_BITS-1:0] mult_trans_id_o
);
  // Carry-less multiplication
  logic [riscv::XLEN-1:0]
      clmul_q, clmul_d, clmulr_q, clmulr_d, operand_a, operand_b, operand_a_rev, operand_b_rev;
  logic clmul_rmode, clmul_hmode;

  if (ariane_pkg::BITMANIP) begin : gen_bitmanip
    // checking for clmul_rmode and clmul_hmode
    assign clmul_rmode = (operation_i == CLMULR);
    assign clmul_hmode = (operation_i == CLMULH);

    // operand_a and b reverse generator
    for (genvar i = 0; i < riscv::XLEN; i++) begin
      assign operand_a_rev[i] = operand_a_i[(riscv::XLEN-1)-i];
      assign operand_b_rev[i] = operand_b_i[(riscv::XLEN-1)-i];
    end

    // operand_a and operand_b selection
    assign operand_a = (clmul_rmode | clmul_hmode) ? operand_a_rev : operand_a_i;
    assign operand_b = (clmul_rmode | clmul_hmode) ? operand_b_rev : operand_b_i;

    // implementation
    always_comb begin
      clmul_d = '0;
      for (int i = 0; i <= riscv::XLEN; i++) begin
        clmul_d = (|((operand_b >> i) & 1)) ? clmul_d ^ (operand_a << i) : clmul_d;
      end
    end

    // clmulr + clmulh result generator
    for (genvar i = 0; i < riscv::XLEN; i++) begin
      assign clmulr_d[i] = clmul_d[(riscv::XLEN-1)-i];
    end
  end

  // Pipeline register
  logic [TRANS_ID_BITS-1:0] trans_id_q;
  logic                     mult_valid_q;
  fu_op operator_d, operator_q;
  logic [riscv::XLEN*2-1:0] mult_result_d, mult_result_q;

  // SIMD Pipeline register
  logic [riscv::XLEN*2-1:0] simd_mult_result_q, simd_mult_result_d;
  logic [riscv::XLEN*2-1:0] simd_sat_mult_result_q, simd_sat_mult_result_d;
  logic [riscv::XLEN:0] simd_smaqa_result_d, simd_smaqa_result_q;
  logic overflow_d, overflow_q;

  // control registers
  logic sign_a, sign_b;
  logic saturated;      //CHANGED: added for SIMD
  logic simd_16bit;     //CHANGED: added for SIMD
  logic crossed;        //CHANGED: added for SIMD
  logic mult_valid;

  // control signals
  assign mult_valid_o = mult_valid_q;
  assign mult_trans_id_o = trans_id_q;
  assign mult_ready_o = 1'b1;
  assign mult_valid      = mult_valid_i && (operation_i inside {MUL, MULH, MULHU, MULHSU, MULW, CLMUL, CLMULH, CLMULR,
                                                                SMUL16, SMULX16, UMUL16, UMULX16, KHM16, KHMX16,     //CHANGED: added for SIMD
                                                                SMUL8, SMULX8, UMUL8, UMULX8, KHM8, KHMX8});        //CHANGED: added for SIMD

  // Sign Select MUX
  always_comb begin
    sign_a = 1'b0;
    sign_b = 1'b0;
    saturated = 1'b0;         //CHANGED: added for SIMD
    simd_16bit = 1'b0;        //CHANGED: added for SIMD
    crossed = 1'b0;           //CHANGED: added for SIMD

    //CHANGED: added SIMD multiplication instruction

    // 16bits SIMD multiplication
    if (operation_i == SMUL16 | operation_i == SMULX16 | operation_i == UMUL16 | operation_i == UMULX16) begin
        simd_16bit = 1'b1;
    // 8bits SIMD multiplication
    end else begin
        simd_16bit = 1'b0;
    // saturated multiplication
    end if (operation_i == KHM16 | operation_i == KHMX16 | operation_i == KHM8 | operation_i == KHMX8) begin
        saturated = 1'b1;
    // unsaturated multiplication
    end else begin
        saturated = 1'b0;
    // crossed multiplication
    end if (operation_i == SMULX16 | operation_i == UMULX16 | operation_i == KHMX16 | 
            operation_i == SMULX8 | operation_i == UMULX8 | operation_i == KHMX8) begin
        crossed = 1'b1;
    // uncrossed multiplication
    end else begin
        crossed = 1'b0;
    // signed multiplication
    end if (operation_i == MULH | 
            operation_i == SMUL16 | operation_i == SMULX16 | operation_i == KHM16 | operation_i == KHMX16 | 
            operation_i == SMUL8 | operation_i == SMULX8 | operation_i == KHM8 | operation_i == KHMX8) begin
      sign_a = 1'b1;
      sign_b = 1'b1;
    // signed - unsigned multiplication
    end else if (operation_i == MULHSU) begin
      sign_a = 1'b1;
    // unsigned multiplication
    end else begin
      sign_a = 1'b0;
      sign_b = 1'b0;
    end
  end


  // single stage version
  assign mult_result_d = $signed(
      {operand_a_i[riscv::XLEN-1] & sign_a, operand_a_i}
  ) * $signed(
      {operand_b_i[riscv::XLEN-1] & sign_b, operand_b_i}
  );


    //CHANGED: added for SIMD 16 bits instruction
    // 16bits SIMD Multpilication
    
    always_comb begin
        if (simd_16bit) begin
            if(crossed) begin
                if (saturated && (operand_a_i[31:16] == 16'h8000) && (operand_b_i[15:0] == 16'h8000)) begin
                    // Y[63:32] saturatured
                    simd_mult_result_d[63:32] = 32'h00007FFF;
                    overflow_d = 1'b1;
                end
                else begin
                    // Y[63:32] = A[31:16] * B[15:0]
                    simd_mult_result_d[63:32] = $signed(
                        {operand_a_i[31] & sign_a, operand_a_i[31:16]}
                    ) * $signed(
                        {operand_b_i[15] & sign_b, operand_b_i[15:0]}
                    );
                end
                if (saturated && (operand_a_i[15:0] == 16'h8000) && (operand_b_i[31:16] == 16'h8000)) begin
                    // Y[31:0] saturatured
                    simd_mult_result_d[31:0] = 32'h00007FFF;
                    overflow_d = 1'b1;
                end
                else begin
                    // Y[31:0]  = A[15:0] * B[31:16]
                    simd_mult_result_d[31:0]  = $signed(
                        {operand_a_i[15] & sign_a, operand_a_i[15:0]}
                    ) * $signed(
                        {operand_b_i[31] & sign_b, operand_b_i[31:16]}
                    );
                end
            end 
            else begin
                if (saturated && (operand_a_i[31:16] == 16'h8000) && (operand_b_i[31:16] == 16'h8000)) begin
                    // Y[63:32] saturatured
                    simd_mult_result_d[63:32] = 32'h00007FFF;
                    overflow_d = 1'b1;
                end
                else begin
                    // Y[63:32] = A[31:16] * B[31:16]
                    simd_mult_result_d[63:32] = $signed(
                        {operand_a_i[31] & sign_a, operand_a_i[31:16]}
                    ) * $signed(
                        {operand_b_i[31] & sign_b, operand_b_i[31:16]}
                    );
                end
                if (saturated && (operand_a_i[15:0] == 16'h8000) && (operand_b_i[15:0] == 16'h8000)) begin
                    // Y[31:0] saturatured
                    simd_mult_result_d[31:0] = 32'h00007FFF;
                    overflow_d = 1'b1;
                end
                else begin
                    // Y[31:0]  = A[15:0] * B[15:0]
                    simd_mult_result_d[31:0]  = $signed(
                        {operand_a_i[15] & sign_a, operand_a_i[15:0]}
                    ) * $signed(
                        {operand_b_i[15] & sign_b, operand_b_i[15:0]}
                    );
                end 
            end
            simd_sat_mult_result_d[31:16] = simd_mult_result_d[63:48];
            simd_sat_mult_result_d[15:0] = simd_mult_result_d[31:16]; 
        end
        
        //CHANGED: added for SIMD 8 bits instruction
        // 8bits SIMD Multpilication
        if (~simd_16bit) begin
            if(crossed) begin
                if (saturated && (operand_a_i[31:24] == 8'h80) && (operand_b_i[23:16] == 8'h80)) begin
                    // Y[63:48] saturatured
                    simd_mult_result_d[63:48] = 16'h007F;
                    overflow_d = 1'b1;
                end
                else begin
                    // Y[63:48] = A[31:24] * B[23:16]
                    simd_mult_result_d[63:48] = $signed(
                    {operand_a_i[31] & sign_a, operand_a_i[31:24]}
                    ) * $signed(
                    {operand_b_i[23] & sign_b, operand_b_i[23:16]}
                    ); 
                end
                if (saturated && (operand_a_i[23:16] == 8'h80) && (operand_b_i[31:24] == 8'h80)) begin
                    // Y[47:32] saturatured
                    simd_mult_result_d[47:32] = 16'h007F;
                    overflow_d = 1'b1;
                end
                else begin
                    // Y[47:32] = A[23:16] * B[31:24]
                    simd_mult_result_d[47:32] = $signed(
                    {operand_a_i[23] & sign_a, operand_a_i[23:16]}
                    ) * $signed(
                    {operand_b_i[31] & sign_b, operand_b_i[31:24]}
                    );
                end        
                if (saturated && (operand_a_i[15:8] == 8'h80) && (operand_b_i[7:0] == 8'h80)) begin
                    // Y[31:16] saturatured
                    simd_mult_result_d[31:16] = 16'h007F;
                    overflow_d = 1'b1;
                end
                else begin
                    // Y[31:16] = A[15:8] * B[7:0]
                    simd_mult_result_d[31:16] = $signed(
                    {operand_a_i[15] & sign_a, operand_a_i[15:8]}
                    ) * $signed(
                    {operand_b_i[7] & sign_b, operand_b_i[7:0]}
                    ); 
                end
                if (saturated && (operand_a_i[7:0] == 8'h80) && (operand_b_i[15:8] == 8'h80)) begin
                    // Y[15:0] saturatured
                    simd_mult_result_d[15:0] = 16'h007F;
                    overflow_d = 1'b1;
                end
                else begin
                    // Y[15:0] = A[7:0] * B[15:8]
                    simd_mult_result_d[15:0] = $signed(
                    {operand_a_i[7] & sign_a, operand_a_i[7:0]}
                    ) * $signed(
                    {operand_b_i[7] & sign_b, operand_b_i[15:8]}
                    );
                end
            end     
            else begin
                if (saturated && (operand_a_i[31:24] == 8'h80) && (operand_b_i[31:24] == 8'h80)) begin
                    // Y[63:48] saturatured
                    simd_mult_result_d[63:48] = 16'h007F;
                    overflow_d = 1'b1;
                end
                else begin
                    // Y[63:48] = A[31:24] * B[31:24] 
                    simd_mult_result_d[63:48] = $signed(
                    {operand_a_i[31] & sign_a, operand_a_i[31:24]}
                    ) * $signed(
                    {operand_b_i[31] & sign_b, operand_b_i[31:24]}
                    ); 
                end
                if (saturated && (operand_a_i[23:16] == 8'h80) && (operand_b_i[23:16] == 8'h80)) begin
                    // Y[47:32] saturatured
                    simd_mult_result_d[47:32] = 16'h007F;
                    overflow_d = 1'b1;
                end
                else begin
                    // Y[47:32] = A[23:16] * B[23:16] 
                    simd_mult_result_d[47:32] = $signed(
                    {operand_a_i[23] & sign_a, operand_a_i[23:16]}
                    ) * $signed(
                    {operand_b_i[23] & sign_b, operand_b_i[23:16]}
                    );
                end        
                if (saturated && (operand_a_i[15:8] == 8'h80) && (operand_b_i[15:8] == 8'h80)) begin
                    // Y[31:16] saturatured
                    simd_mult_result_d[31:16] = 16'h007F;
                    overflow_d = 1'b1;
                end
                else begin
                    // Y[31:16] = A[15:8] * B[15:8] 
                    simd_mult_result_d[31:16] = $signed(
                    {operand_a_i[15] & sign_a, operand_a_i[15:8]}
                    ) * $signed(
                    {operand_b_i[15] & sign_b, operand_b_i[15:8]}
                    ); 
                end
                if (saturated && (operand_a_i[7:0] == 8'h80) && (operand_b_i[7:0] == 8'h80)) begin
                    // Y[15:0] saturatured
                    simd_mult_result_d[15:0] = 16'h007F;
                    overflow_d = 1'b1;
                end
                else begin
                    // Y[15:0] = A[7:0] * B[7:0] 
                    simd_mult_result_d[15:0] = $signed(
                    {operand_a_i[7] & sign_a, operand_a_i[7:0]}
                    ) * $signed(
                    {operand_b_i[7] & sign_b, operand_b_i[7:0]}
                    );
                end 
            end
            simd_sat_mult_result_d[31:24] = simd_mult_result_d[63:56];
            simd_sat_mult_result_d[23:16] = simd_mult_result_d[47:39];
            simd_sat_mult_result_d[15:8]  = simd_mult_result_d[31:24];
            simd_sat_mult_result_d[7:0]   = simd_mult_result_d[15:8];
        end
        end

    // ------------------------
    //MSW 32x32 multiply & Add
    // ------------------------
        logic [riscv::XLEN-1:0] mult_result_msw_d, mult_result_msw_q;
        logic [riscv::XLEN*2-1:0] temp64;
        logic [riscv::XLEN*2:0] temp64_shifted;
        logic [riscv::XLEN-1:0]   temp32;
        logic [riscv::XLEN:0]   temp32_add;
        logic [riscv::XLEN:0]   temp32_sub;
        logic [riscv::XLEN-1:0]   temp32_double;
        logic rounding;
        logic is_mul;
        logic is_mul_add;
        logic is_mul_sub;
        logic is_mul_double;
        logic p_ov;
        logic n_ov;
        logic saturation;
        logic [riscv::XLEN:0] max_val_32;
        logic [riscv::XLEN:0] min_val_32;
        logic [2*riscv::XLEN:0] max_val_64;
        logic [2*riscv::XLEN:0] min_val_64;

        assign max_val_32 = 33'h7FFFFFFF;
        assign min_val_32 = 33'h80000000;
        assign max_val_64 = 65'h7FFFFFFFFFFFFFFF;
        assign min_val_64 = 65'h8000000000000000;

        always_comb begin
            if(operation_i == SMMUL_U || operation_i == KMMAC_U || operation_i == KMMSB_U || operation_i == KWMMUL_U) begin
                rounding = 1'b1;
            end else begin
                rounding = 1'b0;
            end
            if(operation_i == SMMUL || operation_i == SMMUL_U) begin
                is_mul = 1'b1;
                is_mul_add = 1'b0;
                is_mul_sub = 1'b0;
                is_mul_double = 1'b0;
            end else if(operation_i == KMMAC || operation_i == KMMAC_U) begin
                is_mul = 1'b0;
                is_mul_add = 1'b1;
                is_mul_sub = 1'b0;
                is_mul_double = 1'b0;
            end else if(operation_i == KMMSB || operation_i == KMMSB_U) begin
                is_mul = 1'b0;
                is_mul_add = 1'b0;
                is_mul_sub = 1'b1;
                is_mul_double = 1'b0;
            end else if(operation_i == KWMMUL_U || operation_i == KWMMUL) begin
                is_mul = 1'b0;
                is_mul_add = 1'b0;
                is_mul_sub = 1'b0;
                is_mul_double = 1'b1;
            end else begin
                is_mul = 1'b0;
                is_mul_add = 1'b0;
                is_mul_sub = 1'b0;
                is_mul_double = 1'b0;
            end
        
            if(operand_a_i == 31'h80000000 && operand_b_i == 31'h80000000) begin
                saturation = 1'b1;
            end else begin
                saturation = 1'b0;
            end
        end

        assign temp64 = $signed(
            {operand_a_i[riscv::XLEN-1] & sign_a, operand_a_i}
        ) * $signed(
            {operand_b_i[riscv::XLEN-1] & sign_b, operand_b_i}
        );

        assign temp32      = $signed(temp64[2*riscv::XLEN-1:riscv::XLEN]) + $signed({1'b0, rounding * {31'b0, temp64[riscv::XLEN]}});
        assign temp32_add  = $signed(temp32) + $signed(operand_c_i);
        assign temp32_sub  = $signed(temp32) - $signed(operand_c_i);
        assign p_ov =   (is_mul_add && $signed(temp32_add) > $signed(max_val_32)) ||
                        (is_mul_sub && $signed(temp32_sub) > $signed(max_val_32));
        assign n_ov =   (is_mul_add && $signed(temp32_add) < $signed(max_val_32)) ||
                        (is_mul_sub && $signed(temp32_sub) < $signed(max_val_32));
        assign temp64_shifted = temp64 << 1;
        assign temp32_double = $signed(temp64_shifted[2*riscv::XLEN:riscv::XLEN+1]) + $signed(rounding*{31'b0, temp64[riscv::XLEN+1]});



        assign mult_result_msw_d = (is_mul) ? temp32 :
                            (p_ov || saturation) ? 32'h7FFFFFFF :
                            (n_ov) ? 32'h80000000 :
                            (is_mul_add) ? temp32_add[riscv::XLEN:1] :
                            (is_mul_sub) ? temp32_sub[riscv::XLEN:1] :
                            (is_mul_double) ? temp32_double : '0;

        assign overflow_d = (is_mul_add && (p_ov || n_ov)) || (is_mul_sub && (p_ov || n_ov)) || (is_mul_double && saturation);


  assign operator_d = operation_i;

  always_comb begin : p_selmux
    unique case (operator_q)
      MULH, MULHU, MULHSU:                                                  result_o = mult_result_q[riscv::XLEN*2-1:riscv::XLEN];
      MULW:                                                                 result_o = sext32(mult_result_q[31:0]);
      CLMUL:                                                                result_o = clmul_q;
      CLMULH:                                                               result_o = clmulr_q >> 1;
      CLMULR:                                                               result_o = clmulr_q;
    //   SMUL16, SMULX16, UMUL16, UMULX16, SMUL8, SMULX8, UMUL8, UMULX8:       result_o = simd_mult_result_q[2*riscv::XLEN-1:riscv::XLEN],
    //                                                                         result_simd_o = simd_mult_result_q[riscv::XLEN-1:0],
    //                                                                         is_64bits_o = 1'b1;                                 //CHANGED: added 2nd Output
      KHM16, KHMX16, KHM8, KHMX8:   begin
                                                                            result_o = simd_sat_mult_result_q[riscv::XLEN-1:0];
                                                                            overflow_o = overflow_q;
                                    end
                                                                                                        //CHANGED: added overflow flag
      SMMUL, SMMUL_U, KMMAC, KMMAC_U, KMMSB, KMMSB_U, KWMMUL, KWMMUL_U: begin
                                                                            result_o = mult_result_msw_q; //CHANGED: added for SIMD
                                                                            overflow_o = overflow_q;
                                                                        end

      // MUL performs an XLEN-bit×XLEN-bit multiplication and places the lower XLEN bits in the destination register
      
      default:             result_o = mult_result_q[riscv::XLEN-1:0];  // including MUL
    endcase
  end
  if (ariane_pkg::BITMANIP) begin
    always_ff @(posedge clk_i or negedge rst_ni) begin
      if (~rst_ni) begin
        clmul_q  <= '0;
        clmulr_q <= '0;
      end else begin
        clmul_q  <= clmul_d;
        clmulr_q <= clmulr_d;
      end
    end
  end
  // -----------------------
  // Output pipeline register
  // -----------------------
  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (~rst_ni) begin
      mult_valid_q  <= '0;
      trans_id_q    <= '0;
      operator_q    <= MUL;
      mult_result_q <= '0;
      simd_mult_result_q <= '0;
      simd_smaqa_result_q <= '0; 
      simd_sat_mult_result_q <= '0;   //CHANGED: added for SIMD
      overflow_q    <= '0;            //CHANGED: added for SIMD
      mult_result_msw_q <= '0;        //CHANGED: added for SIMD
    end else begin
      // Input silencing
      trans_id_q    <= trans_id_i;
      // Output Register
      mult_valid_q  <= mult_valid;
      operator_q    <= operator_d;
      mult_result_q <= mult_result_d;
      simd_mult_result_q <= simd_mult_result_d;
      simd_smaqa_result_q <= simd_smaqa_result_d;
      simd_sat_mult_result_q <= simd_sat_mult_result_d; //CHANGED: added for SIMD
      overflow_q    <= overflow_d;                      //CHANGED: added for SIMD
      mult_result_msw_q <= mult_result_msw_d;           //CHANGED: added for SIMD
    end
  end
endmodule
