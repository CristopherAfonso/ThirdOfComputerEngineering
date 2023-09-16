module alu(input wire [15:0] a, b,
           input wire [2:0] op_alu,
           output wire [15:0] y,
           output wire zero, sign, overflow, parity);

reg [16:0] s;
		   
always @(a, b, op_alu)
begin
  case (op_alu)              
    3'b000: s = a;     //nada
    3'b001: s = ~a;    //complemento a 1
    3'b010: s = a + b; //suma
    3'b011: s = a - b; //resta
    3'b100: s = a & b; //AND
    3'b101: s = a | b; //OR
    3'b110: s = -a;    //negación OP1
    3'b111: s = -b;    //negación OP2
	default: s = 'bx;    //desconocido en cualquier otro caso (x � z), por si se modifica el c�digo
  endcase
end

assign y = s[15:0];

//Calculo del flag de cero
assign zero = ~(|y);   //operador de reducci�n |y hace la or de los bits del vector 'y' y devuelve 1 bit resultado	   
assign sign = y[15];   //señal que nos dice si el resultado es negativo, señal a 1 si es negativo
assign overflow = s[16] ^ s[15]; //calcula el bit de overflow
assign parity = ~(^y); //calcula el bit de paridad par
endmodule
