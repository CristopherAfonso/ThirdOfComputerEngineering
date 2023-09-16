//Componentes varios

//Banco de registros de dos outs y una entrada
module regfile(input  wire         clk, 
               input  wire         we3,             //se�al de habilitaci�n de escritura
               input  wire [4:0]   ra1, ra2, rasave, wa3,   //direcciones de regs leidos y reg a escribir
               input  wire [15:0]  wd3, 			      //dato a escribir
               output wire [15:0]  rd1, rd2, rdsave);  //datos leidos

  reg [15:0] regb[0:31]; //memoria de 32 registros de 16 bits de ancho

  initial
  begin
    $readmemb("/home/alumno/Documentos/pr1_processor_design/regfile.dat",regb); // inicializa los registros a valores conocidos
  end  
  
  // El registro 0 siempre es cero
  // se leen dos reg combinacionalmente
  // y la escritura del tercero ocurre en flanco de subida del reloj
  always @(posedge clk)
    if (we3) regb[wa3] <= wd3;	
  
  assign rd1 = (ra1 != 0) ? regb[ra1] : 0;
  assign rd2 = (ra2 != 0) ? regb[ra2] : 0;
  assign rdsave = (rasave != 0) ? regb[rasave] : 0;
endmodule


//Memoria de datos de dos outs y una entrada
module datafile(input  wire           clk, 
                input  wire           wed,           //se�al de habilitaci�n de escritura
                input  wire [10:0]     fa,           //dir. a leer/escribir
                input  wire [15:0]     sw, 			     //dato a escribir
                output wire [15:0]  d_mem);          //datos leidos

  reg [15:0] mem_data[0:511]; //memoria de 2048 dir. de 16 bits de ancho

  initial
  begin
    $readmemb("/home/alumno/Documentos/pr1_processor_design/datafile.dat",mem_data); // inicializa los registros a valores conocidos
  end  

  always @(posedge clk)
    if (wed) mem_data[fa] <= sw;	
  
  assign d_mem = mem_data[fa] % 2048;
endmodule

//modulo sumador  
module sum #(parameter WIDTH = 10)
            (input  wire [WIDTH-1:0] a, b, output wire [WIDTH-1:0] y);
  assign y = a + b;
endmodule

//modulo registro para modelar el PC, cambia en cada flanco de subida de reloj o de reset
module registro #(parameter WIDTH = 8)
              (input wire             clk, reset,
               input wire [WIDTH-1:0] d, 
               output reg [WIDTH-1:0] q);

  always @(posedge clk, posedge reset)
    if (reset) q <= 0;
    else       q <= d;

endmodule

//modulo multiplexor, si s=0 sale d0, s=1 sale d1
module mux2 #(parameter WIDTH = 8)
             (input  wire [WIDTH-1:0] d0, d1, 
              input  wire             s, 
              output wire [WIDTH-1:0] y);

  assign y = s ? d1 : d0; 
endmodule

// Módulo multiplexor de 4 entradas
module mux4 #(parameter WIDTH = 8)
             (input  wire [WIDTH-1:0] d0, d1, d2, d3,
              input  wire [1:0]       s,
              output wire [WIDTH-1:0] y);

  reg [WIDTH-1:0] y_temp;

  always @*
    case (s)
      2'b00: y_temp = d0;
      2'b01: y_temp = d1;
      2'b10: y_temp = d2;
      2'b11: y_temp = d3;
    endcase

  assign y = y_temp;
endmodule


//Biestable para el flag de cero
//Biestable tipo D s�ncrono con reset as�ncrono por flanco y entrada de habilitaci�n de carga
module ffd(input wire clk, reset, d, carga, output reg q);

  always @(posedge clk, posedge reset)
    if (reset)
	    q <= 1'b0;
	  else
	    if (carga)
	      q <= d;

endmodule 

//Pila de la CPU de una out y una entrada
module stackfile(input  wire        clk,
                 input  wire        rws, wsp,     //se�al de habilitaci�n de escritura
                 input  wire [15:0] push_d, 		  //dato a escribir
                 output reg [15:0]  pop_d);       //datos leidos

  reg [15:0] mem_stack[0:127]; //memoria de 1024 dir. de 16 bits de ancho
  reg [9:0] current_sp;

  initial
  begin
    $readmemb("/home/alumno/Documentos/pr1_processor_design/stackfile.dat",mem_stack); // inicializa los registros a valores conocidos
    current_sp = 10'b0000000000;
  end

  always @(negedge clk)
    begin
      if (!rws) pop_d = ((current_sp == 10'b0000000000) ? 0 : mem_stack[(current_sp - 1) % 1024]);
      if (rws) mem_stack[current_sp] = push_d;
      if (wsp) current_sp = ((!rws) ? ((current_sp == 10'b0000000000) ? 0 : (current_sp - 10'b0000000001)) : current_sp + 1);
    end
endmodule

// modulo que contacta con "el exterior"
module extmemmod(input wire rw, input wire [15:0] d_in, output wire [15:0] d_out, inout wire [15:0] mem_cable);
 // reg [15:0] io_out;


  //always @(rw)
	 // begin
	   // io_out <= d_in;
	  //end 
	  
  assign mem_cable = ((rw) ? d_in : 16'hz);
  assign d_out = ((rw) ? 16'b0000000000000000 : mem_cable);
endmodule


module sevensegments(input wire clk, reset, wess, input wire[6:0] in_val, input wire[1:0] in_dir, output reg[6:0] out_d0, out_d1, out_d2, out_d3);
  reg [6:0] out;

  always @(negedge clk) begin
    if (reset) out = 7'b0000000; // todos los leds encendidos
    else
      if (wess) begin
        case (in_val)
          7'b0000000: out = 7'b1111111; // caracter nulo
            
				  
          7'b1000001: out = 7'b0001000; // A
          //7'd98: out = 7'b1100000; // b
          7'b1000011: out = 7'b0110001; // C
          //7'd100: out = 7'b1000010;// d
          //7'd69: out = 7'b0110000; // E
          //7'd70: out = 7'b0111000; // F
          //7'd71: out = 7'b0100001; // G
          //7'd72: out = 7'b1001000; // H
          //7'd73: out = 7'b1001111; // I
				  7'b1001010: out = 7'b1000111; // J
          //7'd76: out = 7'b1110001; // L
          //7'd78: out = 7'b0001001; // N
          //7'd79: out = 7'b0000001; // O
          //7'd80: out = 7'b0011000; // P
          //7'd113: out = 7'b0001100;// q
          //7'd114: out = 7'b1111010;// r
          //7'd83: out = 7'b0100100; // S
          //7'd85: out = 7'b1000001; // U
          //7'd121: out = 7'b1001100;// y
				  //7'd90: out = 7'b0010010; // z
          default: out = 7'b0000000;
        endcase
			 
			  case (in_dir)
			    2'b00: out_d0 = out;
				  2'b01: out_d1 = out;
				  2'b10: out_d2 = out;
				  2'b11: out_d3 = out;
			  endcase
      end
  end
endmodule
