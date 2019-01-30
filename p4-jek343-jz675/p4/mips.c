#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "hashtable.h"
#include "mips.h"

/************** BEGIN HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ***************/
const int R_TYPE = 0;
const int I_TYPE = 1;
const int MEM_TYPE = 2;
const int UNKNOWN_TYPE = 3;

/**
 * Return the type of instruction for the given operation
 * Available options are R_TYPE, I_TYPE, MEM_TYPE, UNKNOWN_TYPE
 */
int get_op_type(char *op) {
    const char *r_type_op[] = { "addu", "subu", "and", "or", "xor", "nor", "slt", "sltu" , "movn", "movz", "sllv", "srlv", "srav" };
    const char *i_type_op[] = { "addiu", "andi", "ori", "xori", "slti", "sltiu", "sll", "srl", "sra", "lui" };
    const char *mem_type_op[] = { "lw", "lb", "lbu", "sw", "sb" };
    for (int i = 0; i < (int) (sizeof(r_type_op) / sizeof(char *)); i++) {
        if (strcmp(r_type_op[i], op) == 0) {
            return R_TYPE;
        }
    }
    for (int i = 0; i < (int) (sizeof(i_type_op) / sizeof(char *)); i++) {
        if (strcmp(i_type_op[i], op) == 0) {
            return I_TYPE;
        }
    }
    for (int i = 0; i < (int) (sizeof(mem_type_op) / sizeof(char *)); i++) {
        if (strcmp(mem_type_op[i], op) == 0) {
            return MEM_TYPE;
        }
    }
    return UNKNOWN_TYPE;
}
/*************** END HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ****************/

registers_t *registers;
// TODO: create any additional variables to store the state of the interpreter
hashtable_t *mem; 


void init(registers_t *starting_registers) {
    registers = starting_registers;
    // TODO: initialize any additional variables needed for state
    mem = ht_init(512); 
}

// TODO: create any necessary helper functions


//TODO: write hex to int function
int HexToDec(char* hex) { //can input 0xabcdef
    int dec = (int)strtol(hex, NULL, 0); 
    return dec; 
}

//this method chooses whether the value is an integer or hex and returns value
int HexOrDecToVal(char *input) {
    if (strstr(input, "0x")==NULL){ //if decimal 
        return atoi(input); 
    } else {
	return HexToDec(input);  
    }
}

//this method parses registers only
int regParse(char *r){
    strsep(&r, "$"); //remove $ 
    return atoi(r); 
}

void step(char *instruction) {
    // Extracts and returns the substring before the first space character,
    // by replacing the space character with a null-terminator.
    // `instruction` now points to the next character after the space
    // See `man strsep` for how this library function works
    char *op = strsep(&instruction, " ");
    // Uses the provided helper function to determine the type of instruction
    int op_type = get_op_type(op);
    // Skip this instruction if it is not in our supported set of instructions
    if (op_type == UNKNOWN_TYPE) {
        return;
    }
      
    // TODO: write logic for evaluating instruction on current interpreter state
 
    char *r1 = strsep(&instruction, ","); 
    int reg1 = regParse(r1); 


    char *r2;
    int val2; 
   
    char *r3;
    int val3;

     
    //TODO: parse using atoi and strtok
    if (op_type == R_TYPE) {
 	//r2 value (r2 is register)
    	r2 = strsep(&instruction, ",");
        val2 = registers->r[regParse(r2)];
	//r3 value (r3 is register)
	val3 = registers->r[regParse(instruction)];
    } else if (op_type ==I_TYPE) {
	//r2 value (r2 is register)
	if (strstr(op, "lui") == NULL) {
	    r2 = strsep(&instruction,","); 
	    val2 = registers->r[regParse(r2)];
	}
//      r2 = strsep(&instruction, ","); 
//	val2 = registers->r[regParse(r2)];

	//r3 value (r3 is dec or hex)
	//printf(instruction);
	//printf("\n"); 
	val3 = HexOrDecToVal(instruction); 

    } else if (op_type == MEM_TYPE){
     	//r2 value (r2 is dec or hex)
	r2 = strsep(&instruction, "("); 
	val2 = HexOrDecToVal(r2); 
	//r3 value (r3 is register) 
	r3 = strsep(&instruction, ")"); 
	val3 = registers->r[regParse(r3)]; 
    }


    //TODO make an if for all 20 mips commands
    if (strstr(op, "add") != NULL){
	registers->r[reg1] = val2 + val3; 
    }else if(strstr(op, "and") != NULL){
	printf("and: reg1 = %d, val2 = %d, val3 = %d", reg1, val2, val3);
	registers->r[reg1] = (val2 & val3);
    }else if(strstr(op, "xor") != NULL){
	registers->r[reg1] = (val2 ^ val3);
    }else if(strstr(op, "or") != NULL){ //ordering matters
	registers->r[reg1] = (val2 | val3);
    }else if(strstr(op, "slt") != NULL){
	registers->r[reg1] = (val2 < val3);
    }else if (strstr(op, "subu") != NULL){
	registers->r[reg1] = val2-val3;
    }else if(strstr(op, "movz") != NULL){
	if(val3 == 0)
	    registers->r[reg1] = val2;{ 
	}
    }else if (strstr(op, "sll") != NULL){
	val2 = (unsigned int) val2; 
	registers->r[reg1] = (val2 << val3);
    }else if (strstr(op, "sra") != NULL){
	registers->r[reg1] = (val2 >> val3);
    }else if (strstr(op, "lui") != NULL){
	val3 = (unsigned int) val3; 
	registers->r[reg1] = (val3 << 16);
    }else if (strstr(op, "lw") != NULL){
	int location = val2+val3;
	registers->r[reg1] = ht_get(mem, location);  
    }else if (strstr(op, "lbu") != NULL) {
	int byte = val2%4; 
	int location = val2+val3 - byte;
	int LS = (3-byte)*8;
	int RS = 24; 
	unsigned int tempVal = ht_get(mem, location);
	tempVal = (tempVal <<LS);
	registers->r[reg1] = (tempVal >> RS); 
    }else if (strstr(op, "lb") != NULL){  //lb 
	int byte = val2%4;
	int location = val2 + val3 - byte; 
	int LS = (3-byte)*8; //logical shift left this is little endian...
	int RS = 24; //logical right shift ammount
	
 	int tempVal = ht_get(mem, location); 
	tempVal = (tempVal << LS); 
	registers->r[reg1] = (tempVal >> RS);  
    }else if (strstr(op, "sw") != NULL){
	int value = registers->r[reg1];
	int location = 4*val2+val3; 
	ht_add(mem, location, value);
    }else if (strstr(op, "sb") != NULL){
	int byte = val2%4; 
	int location = val2 + val3 - byte; 
	unsigned int Store = registers->r[reg1];
	int ff = 255; //this is int for 0x000000ff
	int LS = byte*8; 
	ff = ~(ff << LS); 	

	Store = (Store << 24); //isolates last 2 hex

	int RS = (3-byte)*8;  //this is little endian
	Store = (Store >> RS); //little endian shift	

	int tempVal = ht_get(mem, location); 
	tempVal = (tempVal & ff); 
	tempVal = (tempVal | Store); //this is converted

	ht_add(mem, location, tempVal); 
    }
}


