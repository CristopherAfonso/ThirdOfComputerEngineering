module uc(input wire [5:0] opcode, input wire z, output wire s_inc, s_inm, we3, wez, output wire [2:0] op_alu);
  reg s_inc_r, s_inm_r, we3_r, wez_r;
  reg [2:0] op_alu_r;
  always @(opcode, z)
    begin
      if (opcode[5] == 1'b0) // operacion de la alu
        begin
          op_alu_r = opcode[4:2]; // opcode alu
          s_inc_r = 1;        // se pasa a la siguiente instrucción del PC
          s_inm_r = 0;        // no se carga ningun inmediato
          we3_r = 1;          // se habilita la escritura en el banco de registros
          wez_r = 1;          // se habilita la escritura del flag de cero
        end
      else
        begin 
          if (opcode[4] == 1'b0) // saltos
            begin
              case (opcode[3:0])  
                4'b0000: //J
                  begin
                    op_alu_r = 3'b000; // este opcode es para que la alu no modifique nada
                    s_inc_r = 0;   // no se pasa a la siguiente instrucción del PC porque se hará un salto
                    s_inm_r = 0;   // no se carga un inmediato desde la instruccion
                    we3_r = 0;     // no modificamos el banco de registros
                    wez_r = 0;     // no se modifica el flag de cero
                  end
                4'b0010: //JZ 
                  begin
                    op_alu_r = 3'b000;  // este opcode es para que la alu no modifique nada
                    s_inc_r = ( (z == 1'b1) ? 1'b0 : 1'b1 ); // si z es uno entonces hacemos el salto 
                    s_inm_r = 0;    // no se carga un inmediato desde la instruccion
                    we3_r = 0;      // no modificamos el banco de registros
                    wez_r = 0;      // no se modifica el flag de cero
                  end
                4'b0011: //JNZ
                  begin
                    op_alu_r = 3'b000;  // este opcode es para que la alu no modifique nada
                    s_inc_r = ( (z == 1'b1) ? 1'b1 : 1'b0 ); // si z es cero entonces hacemos el salto
                    s_inm_r = 0;    // no se carga un inmediato desde la instruccion
                    we3_r = 0;      // no modificamos el banco de registros
                    wez_r = 0;      // no se modifica el flag de cero
                  end
					 default: //no cambiar nada
					   begin	
				        op_alu_r = 3'b000;
                    s_inc_r = 1'b0;    
                    s_inm_r = 1'b0;     
                    we3_r = 1'b0;    
                    wez_r = 1'b0;         
						end
              endcase
            end
          else // LI - carga inmediata
            begin
              op_alu_r = 3'b000; // este opcode es para que la alu no modifique nada
              s_inc_r = 1;       // se pasa a la siguiente instrucción del PC
              s_inm_r = 1;       // se carga un inmediato desde la instruccion
              we3_r = 1;         // se habilita la escritura en el banco de registros
              wez_r = 0;         // no se habilita la escritura en el flag de cero
            end
        end
    end
	 assign s_inc = s_inc_r;
	 assign s_inm = s_inm_r;
	 assign we3 = we3_r;
	 assign wez = wez_r;
    assign op_alu = op_alu_r;
endmodule
