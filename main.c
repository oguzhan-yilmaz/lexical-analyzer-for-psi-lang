#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int reverse_search(char*, char, int);
int is_identifier(char*);
char* bracketer(char*, char, char, int);
int is_keyword(char*);
int is_operator(char*);
int combine_and_check_for_operator(char, char);
char* get_bracket_name(char);
char fpeek(FILE *);
char* char_to_str(char);
char* two_char_to_str(char, char);


int main(){
    char ch, ch2, buffer[15] = {0} ;
    char* str_temp = NULL;
    int i = 0, index = 0;

    FILE* f_ptr, *f_out_ptr;
    f_ptr = fopen("code.psi","r");
    f_out_ptr = fopen("code.lex", "w");

    // check if file is actually opened
    if(f_ptr == NULL || f_out_ptr == NULL){
        printf("Error while opening the file\n");
        exit(1);
    }
    // main loop to read the file
    while((ch = fgetc(f_ptr)) != EOF){
        // INT CONST
        if(isdigit(ch) && (isspace(fpeek(f_ptr)) || isdigit(fpeek(f_ptr)) )){
            str_temp = (char*) malloc(11);
            i = 0;
            str_temp[i] = ch;
            while((ch = fgetc(f_ptr)) != EOF && i <= 10){
                str_temp[++i] = ch;
                if(!isdigit(fpeek(f_ptr))){
                    str_temp[++i] = '\0';
                    break;
                }
            }
            if(i > 10){
                fprintf(f_out_ptr, "ERROR: INTEGER CONSTANT is longer than 10 characters.\n");
            }else if(ch == EOF){
                fprintf(f_out_ptr, "ERROR: END OF FILE.\n");
            }

            fprintf(f_out_ptr, "IntConst(%s)\n", str_temp);
            free(str_temp);
        }
        else if(isalnum(ch) || ch == '_'){
            // IDENTIFIER OR KEYWORD
            buffer[index++] = ch;

            if(isalnum(fpeek(f_ptr))){
            }else if (fpeek(f_ptr) == '_') {
            }else{
                //fgetc(f_ptr); // waste the whitespace
                buffer[index] = '\0';
                index = 0;

                if(is_keyword(buffer))
                    fprintf(f_out_ptr, "Keyword(%s)\n", buffer);
                i = -10;
                if((i = is_identifier(buffer))==1)
                    fprintf(f_out_ptr, "Identifier(%s)\n", strlwr(buffer));
                if(i == -1)
                    fprintf(f_out_ptr, "ERROR: <%s> is too long\n", buffer);
                if(i == -2)
                    fprintf(f_out_ptr, "ERROR: <%s> is not starting with alphabetic character\n", buffer);
                if(i == -3)
                    fprintf(f_out_ptr, "ERROR: <%s> is contains non-alphanumeric characters\n", buffer);
            }
        }
        // IF CH IS NOT ALPHANUMERIC
        else{

            // STRING CONST
            if (ch == '"'){
                str_temp = (char*) malloc(200);
                i = 0;
                while((ch = fgetc(f_ptr)) != EOF){
                    str_temp[i++] = ch;
                    if(fpeek(f_ptr) == '"') {
                        fgetc(f_ptr); // waste the end quote
                        str_temp[i] = '\0';
                        break;
                    }
                }

                if(ch == EOF){
                    fprintf(f_out_ptr, "ERROR: String not closed\n");
                }
                fprintf(f_out_ptr, "StringConst(%s)\n", str_temp);
                free(str_temp);
            }
            // BRACKET
            else if(get_bracket_name(ch) != NULL) {
                fprintf(f_out_ptr, "%s\n", get_bracket_name(ch));
            }
            // COMMENT
            else if(ch == '/' && fpeek(f_ptr) == '*'){

                printf("comment enter\n");
                fgetc(f_ptr); // waste the asterisk
                while((ch = fgetc(f_ptr)) != EOF){
                    if(ch == '*' && fpeek(f_ptr) == '/'){
                        fgetc(f_ptr); // waste the slash
                        break;
                    }
                }
                if(ch == EOF){
                    //RAISE AN ERROR comment not closed
                    fprintf(f_out_ptr, "ERROR: Comment not closed\n");
                }
                printf("comment end\n");
            }
            // OPERATOR
            else if(combine_and_check_for_operator(ch, fpeek(f_ptr))){
                ch2 = fgetc(f_ptr); // waste one char
                fprintf(f_out_ptr, "Operator(%c%c)\n", ch, ch2);

            }
            else if(is_operator(char_to_str(ch))){
                fprintf(f_out_ptr, "Operator(%c)\n", ch);
            }
            // END OF LINE
            else if(ch == ';'){
                fprintf(f_out_ptr, "EndOfLine\n");
            }
        }
    }
    fclose(f_ptr);
    fclose(f_out_ptr);
    return 0;
}

char fpeek(FILE *stream){
    char c;

    c = fgetc(stream);
    ungetc(c, stream);

    return c;
}

char* char_to_str(char chr){
    char* temp = (char*) malloc(2);
    temp[0] = chr;
    temp[1] = '\0';
    return temp;
}

char* two_char_to_str(char chr1, char chr2){
    char* temp = (char*) malloc(sizeof(char) * 3);
    temp[0] = chr1;
    temp[1] = chr2;
    temp[2] = '\0';
    return temp;
}

int combine_and_check_for_operator(char chr1, char chr2){
    char* temp = two_char_to_str(chr1, chr2);
    int rv;
    rv = is_operator(temp);
    free(temp);
    return rv;
}

char* get_bracket_name(char ch){
    if(ch == '('){
        return "LeftPar";
    }else if(ch == ')'){
        return "RightPar";
    }else if(ch == '{'){
        return "LeftCurlyBracket";
    }else if(ch == '}'){
        return "RightCurlyBracket";
    }else{
        return NULL;
    }
}

int reverse_search(char* str, char chr, int offset){
    // returns the last index of a character, if not found returns a negative
    // integer
    int len = strlen(str) - offset;
    int i = len;
    char* p = str;
    for(; i>=0;i--){
        if( *(p+i) == chr){
            return i;
        }
    }
    return -1;
}

char* bracketer(char *str, char srch, char srch_cls, int offset){
    // returns the inside of a bracket, if there are no brackets returns NULL
    // NULL means there are unclosed brackets
    int closing, opening;
    char* result;
    int length = 0;
    closing = reverse_search(str, srch_cls, offset);
    if( closing >= 0 ){
        opening = reverse_search(str, srch, closing);
        if( opening >= 0 ){
            length = closing - opening - 1;
            result = (char*)malloc(length);
            memcpy(result, str + opening + 1, length);
            result[length] = '\0';
        }else{
            return NULL;
        }
    }else{
       return NULL;
    }
    return result;
}

int is_identifier(char* str){
    // returns 1 if str is an identifier else a negative number
    char* p = NULL;
    // must be shorter than 30 characters
    if( strlen(str) > 30){
        return -1;
    }
    // make the str all lowercase
    str = strlwr(str);
    // must start with alphabetic chars
    if( str[0] > 'z' || str[0] < 'a'){
        return -2;
    }
    //must continue with alphanumeric chars or underscore
    for(p = str; *p != '\0'; ++p){
        if(isalnum(*p)){
        }else if (*p == '_') {
        }else{
            return -3;
        }
    }
    return 1;
}

int is_operator(char* str){
    char operators[7][3] = {"+","-","*","/","++","--",":="};
    int i, flag = 0;

    for(i = 0; i < 7; ++i){
        if(strcmp(operators[i], str) == 0){
            flag = 1;
            break;
        }
    }
    return flag;
}

int is_keyword(char* str){
    char keywords[18][10] = {"break","case","char","const","continue","do","else","enum",
                            "float","for","goto","if","int","long","record","return","static","while"};
    int i, flag = 0;

    for(i = 0; i < 18; ++i){
        if(strcmp(keywords[i], str) == 0){
            flag = 1;
            break;
        }
    }
    return flag;
}
