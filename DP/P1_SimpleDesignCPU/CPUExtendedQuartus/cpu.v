module cpu(input wire clk, reset, output wire [17:0] dir_mem_ex, output wire write_ext, read_ext, we_ram, inout wire [15:0] data_bus, output wire[6:0] out_d0, out_d1, out_d2, out_d3);
  wire s_inc, s_inm, s_rgj, we3, wez, wes, weo, wep, wed, wext, rws, wsp, wed_ext, wess, z, s, o, p;
  wire [1:0] wro;
  wire [5:0] opcode;
  wire [15:0] dir_mem_ex2;
  assign read_ext = ~wed_ext;
  assign write_ext = ~wed_ext;
  assign dir_mem_ex = {2'b00, dir_mem_ex2};
  cd data_path(clk, reset, s_inc, s_inm, s_rgj, we3, wez, wes, weo, wep, wed, wext, rws, wsp, wed_ext, wess, wro, z, s, o, p, opcode, dir_mem_ex2, data_bus, out_d0, out_d1, out_d2, out_d3);
  uc control_unit(opcode, z, s, o, p, s_inc, s_inm, s_rgj, we3, wez, wes, weo, wep, wed, wext, rws, wsp, wed_ext, wess, we_ram, wro);
endmodule
