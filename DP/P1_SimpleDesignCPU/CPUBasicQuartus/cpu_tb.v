`timescale 1 ns / 10 ps

module cpu_tb;


reg clk, reset;
wire [5:0] opcode;
integer i;

initial
begin // for para iterar sobre todos los registros siguiendo la ruta real hasta ellos
  //for (i = 0; i < 16; ++i) $dumpvars(0, cpu_tb.micpu.data_path.banc_reg.regb[i]);
end

// generación de reloj clk
always //siempre activo, no hay condición de activación
begin
  clk = 1'b1;
  #30;
  clk = 1'b0;
  #30;
end

// instanciación del procesador
CPU micpu(clk, reset, opcode);

initial
begin
  $dumpfile("cpu_tb.vcd");
  $dumpvars;
  reset = 0;
  #10; 
  reset = 1;  //a partir del flanco de subida del reset empieza el funcionamiento normal
  #10;
  reset = 0;  //bajamos el reset 
end

initial
begin

  #(11*60);  //Esperamos 9 ciclos o 9 instrucciones
  $finish;
end

endmodule
