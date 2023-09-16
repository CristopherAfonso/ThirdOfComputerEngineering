module uc(input wire [5:0] opcode, input wire z, s, o, p, output reg s_inc, s_inm, s_rgj, we3, wez, wes, weo, wep, wed, wext, rws, wsp, wed_ext, wess, we_ram, output reg [1:0] wro);
  always @(opcode, z, s, o, p) begin
	  if (opcode != 6'b000000) begin
      if (opcode[5] == 1'b1) begin
        s_inm = 0;    // no se carga un inmediato desde la instruccion
        s_rgj = (opcode[1:0] == 2'b01) ? 1 : 0; // se carga una dirección inmediata (en la memoria de instrucciones)
        we3 = (opcode[1:0] == 2'b01) ? 1 : 0;   // no modificamos el banco de registros
        wez = 0;      // no se modifica el flag de cero
        wes = 0;      // no se modifica el flag de sign
        weo = 0;      // no se modifica el flag de overflow
        wep = 0;      // no se modifica el flag de parity
        wed = 0;      // no se modifica la memoria de datos
        wext = 0;     // se usa el dato de la memoria interna de datos
        rws = 0;      // no se habilita la escritura de la pila
        wsp = 0;      // no se habilita la escritura del stack pointer
        wro = (opcode[1:0] == 2'b01) ? 2'b01 : 2'b00; // se habilita (o no) la escritura de la alu en el banco de registros
		    wed_ext = 0; // no se escribe en la memoria externa
		    wess = 0;    // no se usara el display 7 seg
		    we_ram = 0;  // no se habilita la SRAM externa
        case (opcode[1:0]) // saltos
          2'b00, 2'b01: begin // J
            s_inc = 0;   // no se pasa a la siguiente instrucción del PC porque se hará un salto
          end
          2'b10: begin //JZ 
            s_inc = ~z; // si z es uno entonces si hacemos el salto
          end
          2'b11: begin //JNZ
            s_inc = z; // si z es cero entonces si hacemos el salto
          end
        endcase          
      end else begin // opcode[5] = 0
        s_inc = 1;          // se pasa a la siguiente instrucción del PC
        s_rgj = 0;          // se cargaría la dirección inmediata (si s_inc fuera cero, que no lo es)
        if (opcode[4] == 1'b0) begin // operacion de la alu
          s_inm = ~opcode[3];                   // si se carga un inmediato
          we3 = 1;                              // se habilita la escritura en el banco de registros
          wez = 1;                              // se habilita la escritura del flag de cero
          wes = (opcode[1] == 1'b1) ? 1 : 0;    // se habilita la escritura del flag de sign
          weo = (opcode[2:1] == 2'b01) ? 1 : 0; // se habilita la escritura del flag de overflow
          wep = 1;                              // se habilita la escritura del flag de parity
          wed = 0;                              // no se modifica la memoria de datos
          wext = 0;                             // se usa el dato de la memoria interna de datos
          rws = 0;                              // no se habilita la escritura de la pila
          wsp = 0;                              // no se habilita la escritura del stack pointer
          wro = 2'b00;                          // se habilita la escritura de la alu en el banco de registros
				  wed_ext = 0;                          // no se escribe en la memoria externa
				  wess = 0;                             // no se usara el display 7 seg
				  we_ram = 0;                           // no se habilita la SRAM externa
        end else begin  // opcode[4] = 1
          if (opcode[3] == 1'b0) begin // operaciones de memoria de datos (LR / SR)
            we3 = ~opcode[2];   // se habilita la escritura en el banco de registros
            s_inm = 0;          // no se carga ningun inmediato
            wez = 0;            // no se habilita la escritura del flag de cero
            wes = 0;            // no se modifica el flag de sign
            weo = 1;            // no se modifica el flag de overflow
            wep = 1;            // no se modifica el flag de parity
            rws = 0;            // no se habilita la escritura de la pila
            wsp = 0;            // no se habilita la escritura del stack pointer
            wess = ((opcode[2:0] == 3'b111) ? 1 : 0);
            wro = 2'b11;        // la entrada al banco de registros es la salida
            if (opcode [1] == 1'b0) begin // mini-memoria interna
              wext = 0;   // se va a memoria interna (opcode[1] == 0) ó memoria externa (opcode[1] == 1)
              wed = opcode[2];    // se habilita la escritura de la memoria de datos si se hace LR, sino (SR) no.       
			        wed_ext = 0; // no se escribe en la memoria externa		
					    we_ram = 0;  // no se habilita la SRAM externa	  
            end else begin // memoria externa, opcode[1] = 1
              if (opcode[0] == 1'b0)	begin
				        wext = 1;   // se va a memoria interna (opcode[1] == 0) ó memoria externa (opcode[1] == 1)
                wed = 0;    // se habilita la escritura de la memoria de datos si se hace LR, sino (SR) no. 
                wed_ext = opcode[2]; // se escribe (o no)en la memoria externa
								we_ram = 1;  // se habilita la SRAM externa
				      end else begin  // opcode[0] = 1
						    wext = 0;    // se va a memoria interna (opcode[1] == 0) ó memoria externa (opcode[1] == 1)
                wed = 0;     // se habilita la escritura de la memoria de datos si se hace LR, sino (SR) no. 
                wed_ext = 0; // se escribe (o no)en la memoria externa
							  we_ram = 0;  // no se habilita la SRAM externa
				      end
            end
          end else begin // operaciones de PUSH/POP a la pila, opcode[3] = 1
            s_inm = 0;              // no se carga ningun inmediato
            we3 = opcode[2];        // se habilita la escritura en el banco de registros
            wez = 0;                // no se habilita la escritura del flag de cero
            wes = 0;                // no se modifica el flag de sign
            weo = 0;                // no se modifica el flag de overflow
            wep = 0;                // no se modifica el flag de parity
            rws = ~opcode[2];       // se habilita la escritura de la pila
            wro = 2'b10;            // la entrada al banco de registros es la salidawep = 0;                // no se modifica el flag de parity
            wext = 0;               // se va a memoria interna (opcode[1] == 0) ó memoria externa (opcode[1] == 1)
            wsp = 1;                // se habilita la escritura del stack pointer
            wed = 0;                // se habilita la escritura de la memoria de datos si se hace LR, sino (SR) no.
					  wed_ext = 0;            // no se escribe en la memoria externa
					  wess = 0;               // no se usara el display 7 seg
					  we_ram = 0;             // no se habilita la SRAM externa
          end
        end
      end
    end else begin
      wess = 0;                             // no se usara el display 7 seg
    end
  end
endmodule
