#pragma once

#define TOTAL_SIZE 1024
#define IP 0
#define REG1 1
#define REG2 2
#define REG3 3
#define REG4 4
#define REG5 5
#define REG6 6
#define LOCAL_STACK_ADDRESS 7
#define CODE_START 8
#define STRING_VAR_START TOTAL_SIZE / 2
#define STACK_START TOTAL_SIZE - 1
#define GLOBAL_VAR_START TOTAL_SIZE - 500

#define SUM 32001
#define DIFF 32002

#define VARIABLE 32010
#define MOVE 32011

#define PRINT 32020
#define SCAN_REG 32021
#define PRINT_STR 32022

#define IF_EQUAL 32040
#define IF_MORE 32041

#define GOTO 32050
#define GOTO_ADDRESS 32051

#define PUT_VALUE 32060
#define POP 32061
#define LOOK 32062
#define LOOK_ADDRESS 32063
#define PUT_FROM_ADDRESS 32064

#define STOP 32070
