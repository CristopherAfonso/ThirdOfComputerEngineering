`timescale 1 ns / 10 ps

module pr1_cpu_extendida;


reg clk, reset;
wire write_ext, read_ext, we_ram;
wire [15:0] dir_mem_ex, data_bus;
wire [6:0] out_d0, out_d1, out_d2, out_d3;
integer i;

initial
begin // for para iterar sobre todos los registros siguiendo la ruta real hasta ellos
  for (i = 0; i < 32; i=i+1) $dumpvars(0, pr1_cpu_extendida.pr1_cpu_extendidaa.data_path.banc_reg.regb[i]);
end

// initial
// begin // for para iterar sobre parte de la memoria de datos siguiendo la ruta real hasta ellos
//   for (i = 0; i < 30; ++i) $dumpvars(0, cpu_tb.micpu.data_path.data_mem.mem_data[i]);
// end

// generación de reloj clk
always //siempre activo, no hay condición de activación
begin
  clk = 1'b1;
  #30;
  clk = 1'b0;
  #30;
end

// instanciación del procesador
cpu pr1_cpu_extendidaa(clk, reset, dir_mem_ex, write_ext, read_ext, we_ram, data_bus, out_d0, out_d1, out_d2, out_d3);

initial
begin
  //$dumpfile("cpu_tb.vcd");
  $dumpvars;
  reset = 1;  //a partir del flanco de subida del reset empieza el funcionamiento normal
  #10;
  reset = 0;  //bajamos el reset 
end

initial
begin

  #(512*60);  //Esperamos 25 ciclos/instrucciones
  $finish;
end

endmodule
