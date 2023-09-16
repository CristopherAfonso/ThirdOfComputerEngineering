// (C) 2001-2013 Altera Corporation. All rights reserved.
// Your use of Altera Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License Subscription 
// Agreement, Altera MegaCore Function License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the applicable 
// agreement for further details.


// $Id: //acds/rel/12.1sp1/ip/merlin/altera_merlin_router/altera_merlin_router.sv.terp#1 $
// $Revision: #1 $
// $Date: 2012/10/10 $
// $Author: swbranch $

// -------------------------------------------------------
// Merlin Router
//
// Asserts the appropriate one-hot encoded channel based on 
// either (a) the address or (b) the dest id. The DECODER_TYPE
// parameter controls this behaviour. 0 means address decoder,
// 1 means dest id decoder.
//
// In the case of (a), it also sets the destination id.
// -------------------------------------------------------

`timescale 1 ns / 1 ns

module first_nios2_system_addr_router_default_decode
  #(
     parameter DEFAULT_CHANNEL = 7,
               DEFAULT_DESTID = 7 
   )
  (output [89 - 86 : 0] default_destination_id,
   output [9-1 : 0] default_src_channel
  );

  assign default_destination_id = 
    DEFAULT_DESTID[89 - 86 : 0];
  generate begin : default_decode
    if (DEFAULT_CHANNEL == -1)
      assign default_src_channel = '0;
    else
      assign default_src_channel = 9'b1 << DEFAULT_CHANNEL;
  end
  endgenerate

endmodule


module first_nios2_system_addr_router
(
    // -------------------
    // Clock & Reset
    // -------------------
    input clk,
    input reset,

    // -------------------
    // Command Sink (Input)
    // -------------------
    input                       sink_valid,
    input  [100-1 : 0]    sink_data,
    input                       sink_startofpacket,
    input                       sink_endofpacket,
    output                      sink_ready,

    // -------------------
    // Command Source (Output)
    // -------------------
    output                          src_valid,
    output reg [100-1    : 0] src_data,
    output reg [9-1 : 0] src_channel,
    output                          src_startofpacket,
    output                          src_endofpacket,
    input                           src_ready
);

    // -------------------------------------------------------
    // Local parameters and variables
    // -------------------------------------------------------
    localparam PKT_ADDR_H = 60;
    localparam PKT_ADDR_L = 36;
    localparam PKT_DEST_ID_H = 89;
    localparam PKT_DEST_ID_L = 86;
    localparam ST_DATA_W = 100;
    localparam ST_CHANNEL_W = 9;
    localparam DECODER_TYPE = 0;

    localparam PKT_TRANS_WRITE = 63;
    localparam PKT_TRANS_READ  = 64;

    localparam PKT_ADDR_W = PKT_ADDR_H-PKT_ADDR_L + 1;
    localparam PKT_DEST_ID_W = PKT_DEST_ID_H-PKT_DEST_ID_L + 1;




    // -------------------------------------------------------
    // Figure out the number of bits to mask off for each slave span
    // during address decoding
    // -------------------------------------------------------
    localparam PAD0 = log2ceil(64'h1000000 - 64'h800000);
    localparam PAD1 = log2ceil(64'h1100000 - 64'h1080000);
    localparam PAD2 = log2ceil(64'h1101000 - 64'h1100000);
    localparam PAD3 = log2ceil(64'h1103000 - 64'h1102800);
    localparam PAD4 = log2ceil(64'h1103020 - 64'h1103000);
    localparam PAD5 = log2ceil(64'h1103040 - 64'h1103020);
    localparam PAD6 = log2ceil(64'h1103050 - 64'h1103040);
    localparam PAD7 = log2ceil(64'h1103058 - 64'h1103050);
    localparam PAD8 = log2ceil(64'h1103060 - 64'h1103058);
    // -------------------------------------------------------
    // Work out which address bits are significant based on the
    // address range of the slaves. If the required width is too
    // large or too small, we use the address field width instead.
    // -------------------------------------------------------
    localparam ADDR_RANGE = 64'h1103060;
    localparam RANGE_ADDR_WIDTH = log2ceil(ADDR_RANGE);
    localparam OPTIMIZED_ADDR_H = (RANGE_ADDR_WIDTH > PKT_ADDR_W) ||
                                  (RANGE_ADDR_WIDTH == 0) ?
                                        PKT_ADDR_H :
                                        PKT_ADDR_L + RANGE_ADDR_WIDTH - 1;
    localparam RG = RANGE_ADDR_WIDTH-1;

      wire [PKT_ADDR_W-1 : 0] address = sink_data[OPTIMIZED_ADDR_H : PKT_ADDR_L];

    // -------------------------------------------------------
    // Pass almost everything through, untouched
    // -------------------------------------------------------
    assign sink_ready        = src_ready;
    assign src_valid         = sink_valid;
    assign src_startofpacket = sink_startofpacket;
    assign src_endofpacket   = sink_endofpacket;

    wire [PKT_DEST_ID_W-1:0] default_destid;
    wire [9-1 : 0] default_src_channel;




    first_nios2_system_addr_router_default_decode the_default_decode(
      .default_destination_id (default_destid),
      .default_src_channel (default_src_channel)
    );

    always @* begin
        src_data    = sink_data;
        src_channel = default_src_channel;

        src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = default_destid;
        // --------------------------------------------------
        // Address Decoder
        // Sets the channel and destination ID based on the address
        // --------------------------------------------------

        // ( 0x800000 .. 0x1000000 )
        if ( {address[RG:PAD0],{PAD0{1'b0}}} == 25'h800000 ) begin
            src_channel = 9'b010000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 7;
        end

        // ( 0x1080000 .. 0x1100000 )
        if ( {address[RG:PAD1],{PAD1{1'b0}}} == 25'h1080000 ) begin
            src_channel = 9'b001000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 6;
        end

        // ( 0x1100000 .. 0x1101000 )
        if ( {address[RG:PAD2],{PAD2{1'b0}}} == 25'h1100000 ) begin
            src_channel = 9'b100000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 8;
        end

        // ( 0x1102800 .. 0x1103000 )
        if ( {address[RG:PAD3],{PAD3{1'b0}}} == 25'h1102800 ) begin
            src_channel = 9'b000000001;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 0;
        end

        // ( 0x1103000 .. 0x1103020 )
        if ( {address[RG:PAD4],{PAD4{1'b0}}} == 25'h1103000 ) begin
            src_channel = 9'b000100000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 5;
        end

        // ( 0x1103020 .. 0x1103040 )
        if ( {address[RG:PAD5],{PAD5{1'b0}}} == 25'h1103020 ) begin
            src_channel = 9'b000000100;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 2;
        end

        // ( 0x1103040 .. 0x1103050 )
        if ( {address[RG:PAD6],{PAD6{1'b0}}} == 25'h1103040 ) begin
            src_channel = 9'b000001000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 3;
        end

        // ( 0x1103050 .. 0x1103058 )
        if ( {address[RG:PAD7],{PAD7{1'b0}}} == 25'h1103050 ) begin
            src_channel = 9'b000010000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 4;
        end

        // ( 0x1103058 .. 0x1103060 )
        if ( {address[RG:PAD8],{PAD8{1'b0}}} == 25'h1103058 ) begin
            src_channel = 9'b000000010;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 1;
        end

end


    // --------------------------------------------------
    // Ceil(log2()) function
    // --------------------------------------------------
    function integer log2ceil;
        input reg[65:0] val;
        reg [65:0] i;

        begin
            i = 1;
            log2ceil = 0;

            while (i < val) begin
                log2ceil = log2ceil + 1;
                i = i << 1;
            end
        end
    endfunction

endmodule


