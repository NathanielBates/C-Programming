// CDA-3103 Project
// Nathaniel Bates
// December 2, 2016

#include "spimcore.h"

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero) {

	switch(ALUControl){

		case '0':
			*ALUresult = A + B;		// Add A and B and set total to ALUresult.
			break;

		case '1':
			*ALUresult = A - B;		// Subract B from A and set amount to ALUresult
			break;

		case '2':
			if((int)A < (int)B)		// Signed Integers
            *ALUresult = 1;		// ALUresult is set to 1 if A < B.
      	else							// ALUresult is set to 0 otherwise.
			   *ALUresult = 0;
			break;

		case '3':
         if(A < B)
            *ALUresult = 1;		// ALUresult is set to 1 if A < B.
         else 							// ALUresult is set to 0 otherwise.
			   *ALUresult = 0;
			break;

		case '4':
			*ALUresult = A & B;		// Bitwise AND operator on A and B.
			break;						// ALUresult is set to that value.

		case '5':
			*ALUresult = A | B;		// Bitwise OR operator on A and B.
			break;              		// ALUresult is set to that value.

		case '6':
		   *ALUresult = B << 16;	// Left-Shift B by 16 bits.
		   break;						// ALUresult is set to that value.

		case '7':
		   *ALUresult = ~A;  		// ALUresult set equal to the negation of A.
	}

	if(*ALUresult == 0)				// If ALUresult is equal to 0, then Zero = 1.
	   *Zero = '1';					// Otherwise Zero = '0'.
	else
	   *Zero = '0';

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction) {

	// Program counter is byte addressed, so we put the data from
	// memory into the instruction.

	if(PC % 4 == 0)
		*instruction = Mem[PC >> 2];
	else
		return 1;

	return 0;
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec) {

// Instruction is parsed and the corresponding values are set in the correct fields.

	*op     = (instruction & 0xfc000000) >> 26;	//11111100000000000000000000000000
	*r1     = (instruction & 0x03e00000) >> 21;	//00000011111000000000000000000000
	*r2     = (instruction & 0x001f0000) >> 16;	//00000000000111110000000000000000
	*r3     = (instruction & 0x0000f800) >> 11;	//00000000000000001111100000000000
	*funct  =  instruction & 0x0000003f; 	   	//00000000000000000000000000111111
	*offset =  instruction & 0x0000ffff; 	   	//00000000000000001111111111111111
	*jsec   =  instruction & 0x03ffffff; 	   	//00000011111111111111111111111111

}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls) {

	switch(op){
    // R type
    case 0:
        controls->RegDst = '1';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '7';
        controls->MemWrite = '0';
        controls->ALUSrc = '0';
        controls->RegWrite = '1';
    break;

    // loadUpperI
    case 15:
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '6';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
    break;

    // BranchOneq
    case 4:
        controls->RegDst = '2';
        controls->Jump = '0';
        controls->Branch = '1';
        controls->MemRead = '0';
        controls->MemtoReg = '2';
        controls->ALUOp = '1';
        controls->MemWrite = '0';
        controls->ALUSrc = '2';
        controls->RegWrite = '0';
    break;

    // SetLessTI
    case 10:
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '2';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
    break;

    // SetLessTIU
    case 11:
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '3';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
    break;

    // Load word
    case 35:
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '1';
        controls->MemtoReg = '1';
        controls->ALUOp = '0';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
    break;

    // Store word
    case 43:
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '0';
        controls->MemWrite = '1';
        controls->ALUSrc = '1';
        controls->RegWrite = '0';
    break;

    // j type
    case 2:
        controls->RegDst = '2';
        controls->Jump = '1';
        controls->Branch = '2';
        controls->MemRead = '0';
        controls->MemtoReg = '2';
        controls->ALUOp = '0';
        controls->MemWrite = '0';
        controls->ALUSrc = '2';
        controls->RegWrite = '0';
    break;

    // AddImmediate
    case 8:
        controls->RegDst = '0';
        controls->Jump = '0';
        controls->Branch = '0';
        controls->MemRead = '0';
        controls->MemtoReg = '0';
        controls->ALUOp = '0';
        controls->MemWrite = '0';
        controls->ALUSrc = '1';
        controls->RegWrite = '1';
    break;

    default:
        return 1;
    }

	 return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2) {

// Read the registers addressed by r1 and r2, and write the values to data1
// and data2.

	*data1 = Reg[r1];
	*data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value) {

// Combine negated values to offset values.
// Create the 32 bit version that is properly signed extended.
// If the value is positve the upper 16 bits should have 1's, which indicates negative.
// If the value is negative the value should have 0's, which indicates positive.

	unsigned negative = offset >> 15;

	if(negative == 1)
      *extended_value = 0xffff0000 | offset;
	else
      *extended_value = 0x0000ffff & offset;

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero) {

	if(ALUSrc == '1')
	   data2 = extended_value;

	if(ALUOp == '7')
   {
   	switch(funct){

			// AND
        	case 36:
            ALUOp = '4';
            break;

         // ADD
			case 32:
            ALUOp = '0';
            break;

         // SUBTRACT
			case 34:
            ALUOp = '1';
            break;

         // OR
        	case 37:
            ALUOp = '5';
            break;

			// Set Less Than Signed
			case 42:
            ALUOp = '2';
            break;

			// Set Less Than unsigned
			case 43:
            ALUOp = '3';
            break;

        	// SHIFT
		  	case 6:
            ALUOp = '6';
            break;

        	// NOR
		  	case 39:
            ALUOp = '7';
            break;

			// If halt condition occurs, 1 is returned.
			default:
            return 1;
        }

		  // Send data to the ALU function for processing.
		  ALU(data1, data2, ALUOp, ALUresult, Zero);
    }

    else
       ALU(data1, data2, ALUOp, ALUresult, Zero);

	return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem) {

// Reading the memory and putting into the memory address divided by 4 because memory is
// byte addressed. Memdata is pointing to the ALUresult divided by 4.
	if(MemRead  == '1')
	{
		if(ALUresult % 4 == 0)
			*memdata = Mem[ALUresult >> 2];
		else
			return 1;
    }

// Write data2 into the memory of the ALUresult address.
// If a halt occurs, we return 1.
	if(MemWrite == '1')
	{
		if( ALUresult % 4 == 0)
			Mem[ALUresult >> 2] = data2;
		else
			return 1;
    }

	return 0;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg) {

	// If we need to write in the register, we will put the memory data into
	// the register addressed by r2 or r3.
	if(RegWrite == '1')
		if(MemtoReg == '1')
			Reg[r2] = memdata;
		else if(MemtoReg == '0')
			if(RegDst == '1')
				Reg[r3] = ALUresult;
			else
				Reg[r2] = ALUresult;

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC) {

	// Update the program counter.
	*PC += 4;

	// If branching and Zero is asserted, we add the extended value times 4.
	if(Zero == '1' && Branch == '1')
		*PC += (extended_value << 2);

	// If jump is assertive, multiply it by 4, concatenate it with the program counter.
	if(Jump == '1')
        *PC = (*PC & 0xf0000000) | (jsec << 2);

}
