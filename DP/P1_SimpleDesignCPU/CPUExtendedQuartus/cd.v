module cd(input wire clk, reset, s_inc, s_inm, s_rgj, we3, wez, wes, weo, wep, wed, wext, rws, wsp, wed_ext, wess, input wire [1:0] wro, output wire z, s, o, p, output wire [5:0] opcode, output wire [15:0] dir_mem_ex, inout wire [15:0] data_bus, output wire[6:0] out_d0, out_d1, out_d2, out_d3);
//Camino de datos de instrucciones de un solo ciclo

  wire [31:0] mem_out;
  wire [15:0] result_alu, op2_alu, reg1_out, reg2_out, stack_out, data_mem_in_out, mux_to_banc_reg_out, mem_save_out, mux_data_out_to_mux4, data_mem_in_ex;
  wire [10:0] desp_mem_data_out;
  wire [9:0] pc_out, mux_to_pc, sum_to_mux_jump, mux_addr_out;
  wire [2:0] mux_op_alu_out;
  wire alu_zero, out_flag_zero;
  wire alu_sign, out_flag_sign;
  wire alu_overflow, out_flag_overflow;
  wire alu_parity, out_flag_parity;

  registro #(10) pc(clk, reset, mux_to_pc, pc_out);
  memprog mem(clk, pc_out, mem_out);
  sum sumador(10'b0000000001, pc_out, sum_to_mux_jump);
  mux2 #(10) mux_jump(mux_addr_out, sum_to_mux_jump, s_inc, mux_to_pc);
  mux2 #(10) mux_dir_or_reg(mem_out[9:0], reg2_out[9:0], s_rgj, mux_addr_out);


  alu alu0(reg1_out, op2_alu, mux_op_alu_out, result_alu, alu_zero, alu_sign, alu_overflow, alu_parity);
  mux2 #(3) mux_op_alu(opcode[2:0], 3'b010, opcode[4], mux_op_alu_out);
  ffd flag_zero(clk, reset, alu_zero, wez, out_flag_zero);
  ffd flag_sign(clk, reset, alu_sign, wes, out_flag_sign);
  ffd flag_overflow(clk, reset, alu_overflow, weo, out_flag_overflow);
  ffd flag_parity(clk, reset, alu_parity, wep, out_flag_parity);
  regfile banc_reg(clk, we3, mem_out[20:16], mem_out[4:0], mem_out[9:5], mem_out[25:21], mux_to_banc_reg_out, reg1_out, reg2_out, mem_save_out);
  mux2 #(16) mux_rd2_or_const(reg2_out, mem_out[15:0], s_inm, op2_alu);
  mux4 #(16) mux_to_banc_reg(result_alu, {6'b000000, sum_to_mux_jump}, stack_out, mux_data_out_to_mux4, wro, mux_to_banc_reg_out);
  mux2 #(16) mux_mem_ex_or_mem_in(data_mem_in_out, data_mem_in_ex, wext, mux_data_out_to_mux4);
  
  
  datafile data_mem(clk, wed, result_alu[10:0], mem_save_out, data_mem_in_out);
  extmemmod extmem(wed_ext, mem_save_out, data_mem_in_ex, data_bus);
  sevensegments sevenseg(clk, reset, wess, mem_save_out[6:0], reg2_out[1:0], out_d0, out_d1, out_d2, out_d3);
  stackfile stack(clk, rws, wsp, reg2_out, stack_out);

  assign z = out_flag_zero;
  assign s = out_flag_sign;
  assign o = out_flag_overflow;
  assign p = out_flag_parity;
  assign opcode = mem_out[31:26];
  assign dir_mem_ex = result_alu;

  //assign data_mem_out_ex = mem_save_out;

endmodule
