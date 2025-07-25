/*
MADE BY ME
INSPIRED BY BASIC
07/25/2025
NUL IS A PROGRAMMING LANGUAGE THAT HAS NO USE YET
IT MEANS "NO USE LANGUAGE"

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 256
#define MAX_VARS 100

typedef struct {
    int number;
    char code[MAX_LINE_LENGTH];
} Line;

typedef struct {
    char name[32];
    int value;
} Variable;

Line program[MAX_LINES];
int line_count = 0;

Variable vars[MAX_VARS];
int var_count = 0;

void trim(char *str) {
    char temp[MAX_LINE_LENGTH];
    int i = 0, j = 0;
    while (str[i]) {
        if (!isspace(str[i]) || (i > 0 && !isspace(str[i - 1])))
            temp[j++] = str[i];
        i++;
    }
    temp[j] = '\0';
    strcpy(str, temp);
}

int get_var_index(char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0)
            return i;
    }
    if (var_count >= MAX_VARS) {
        printf("VARIABLE ERROR\n");
        exit(1);
    }
    strcpy(vars[var_count].name, name);
    vars[var_count].value = 0;
    return var_count++;
}

int eval(char *expr) {
    int val = 0;
    char op = 0;
    char token[32];
    int pos = 0, tok_pos = 0;

    while (expr[pos]) {
        if (isdigit(expr[pos]) || isalpha(expr[pos])) {
            tok_pos = 0;
            while (isalnum(expr[pos]))
                token[tok_pos++] = expr[pos++];
            token[tok_pos] = '\0';

            int value;
            if (isalpha(token[0])) {
                int idx = get_var_index(token);
                value = vars[idx].value;
            } else {
                value = atoi(token);
            }

            if (!op)
                val = value;
            else {
                if (op == '+') val += value;
                else if (op == '-') val -= value;
                else if (op == '*') val *= value;
                else if (op == '/') val /= value;
                op = 0;
            }
        } else if (strchr("+-*/", expr[pos])) {
            op = expr[pos++];
        } else {
            pos++;
        }
    }
    return val;
}

void insert_line(int number, char *code) {
    for (int i = 0; i < line_count; i++) {
        if (program[i].number == number) {
            strcpy(program[i].code, code);
            return;
        }
    }
    if (line_count >= MAX_LINES) {
        printf("LINE ERROR\n");
        return;
    }
    program[line_count].number = number;
    strcpy(program[line_count].code, code);
    line_count++;

    for (int i = 0; i < line_count - 1; i++) {
        for (int j = i + 1; j < line_count; j++) {
            if (program[i].number > program[j].number) {
                Line temp = program[i];
                program[i] = program[j];
                program[j] = temp;
            }
        }
    }
}

int find_line_index(int number) {
    for (int i = 0; i < line_count; i++) {
        if (program[i].number == number)
            return i;
    }
    return -1;
}

void clear_program() {
    line_count = 0;
    printf("DONE\n");
}

int execute_line(char *line);

void run_program() {
    int pc = 0;
    while (pc < line_count) {
        int jump_to = execute_line(program[pc].code);
        if (jump_to == -1) {
            pc++;
        } else {
            int idx = find_line_index(jump_to);
            if (idx == -1) {
                printf("LINE ERROR\n");
                return;
            }
            pc = idx;
        }
    }
}

int execute_line(char *line) {
    char cmd[10], rest[MAX_LINE_LENGTH];
    sscanf(line, "%s %[^\n]", cmd, rest);

    if (strcmp(cmd, "LET") == 0) {
        char varname[32], expr[MAX_LINE_LENGTH];
        sscanf(rest, "%s = %[^\n]", varname, expr);
        int idx = get_var_index(varname);
        vars[idx].value = eval(expr);
    } else if (strcmp(cmd, "PRINT") == 0) {
        trim(rest);
        if (rest[0] == '"') {
            rest[strlen(rest) - 1] = '\0';
            printf("%s\n", rest + 1);
        } else {
            printf("%d\n", eval(rest));
        }
    } else if (strcmp(cmd, "INPUT") == 0) {
        char varname[32];
        sscanf(rest, "%s", varname);
        int idx = get_var_index(varname);
        printf("? ");
        scanf("%d", &vars[idx].value);
    } else if (strcmp(cmd, "GOTO") == 0) {
        return atoi(rest);
    } else if (strcmp(cmd, "END") == 0) {
        exit(0);
    } else {
        printf("ERROR\n");
    }
    return -1;
}

int main() {
    char line[MAX_LINE_LENGTH];

    printf("===== NUL VERSION 1.0 =====\n");

    while (1) {
        printf("] ");
        fgets(line, MAX_LINE_LENGTH, stdin);
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) continue;

        if (isdigit(line[0])) {
            int lineno;
            char code[MAX_LINE_LENGTH];
            sscanf(line, "%d %[^\n]", &lineno, code);
            insert_line(lineno, code);
        } else {
            if (strcmp(line, "RUN") == 0) {
                run_program();
            } else if (strcmp(line, "END") == 0) {
                break;
            } else if (strcmp(line, "CLEAR") == 0) {
                clear_program();
            } else {
                execute_line(line);
            }
        }
    }

    return 0;
}
