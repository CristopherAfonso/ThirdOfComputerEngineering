# Diseño de procesadores - Práctica 1
## CODIFICACIÓN DE LAS INSTRUCCIONES (16 bits)
  > Operación ALU -> 4 bits con el primero a 0 
      > Los 3 bits restantes seleccionan entre E=S, negación, suma, resta, AND, OR y paso a Ca2.
       > Tras el opcode se indican, en campos de 4 bits, registro 1, 2 y resultado.
  > Otras operaciones:
     > Carga inmediata de valor en registro -> 4 bits con el primero a 1 y los otros a 100. Se tienen luego 8 bits para la constante y 4 para el registro
     > Saltos: 6 bits con el primero a 1 (después del opcode va una dirección de 10 bits)
        > Incondicional (J) y condicional en base a la bandera de 0 (JNZ y JZ). Indicados por los 4 últimos bits del opcode, permitiendo ampliaciones

## INSTRUCCIONES VERIFICADAS
  > Se ha probado un programa, cargado en memoria, que utiliza los distintos saltos variando la bandera de cero para ver si funcionan correctamente, ya sea con cargas inmediatas de registros o sumas y restas de la ALU.
  
## DIFICULTADES
  > La principal dificultad fue darse cuenta de que el cable del opcode y el de la ALU estaban cruzados en el módulo unificador CPU.
