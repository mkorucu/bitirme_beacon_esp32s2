#ifndef __LITEJSON
#define __LITEJSON

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

class LiteJSON {
        
    public:
        LiteJSON(uint16_t buffer_size){
            buffer = (char *)malloc(buffer_size+1);
            buffer[0] = '\0';
            is_empty = true;
        };
        ~LiteJSON(){
            free(buffer);
        };
        void add_pair(const char *key, const char *value){
            if(is_empty){
              strcat(buffer,left_brace);
              is_empty = false;
            }
            else{
                strcat(buffer,comma);
            }
            
            strcat(buffer,double_quote);
            strcat(buffer,key);
            strcat(buffer,double_quote);
            
            strcat(buffer,colon);
            
            strcat(buffer,double_quote);
            strcat(buffer,value);
            strcat(buffer,double_quote);
        };
        void add_pair(const char *key, int value){
            if(is_empty){
              strcat(buffer,left_brace);
              is_empty = false;
            }
            else{
                strcat(buffer,comma);
            }
            
            strcat(buffer,double_quote);
            strcat(buffer,key);
            strcat(buffer,double_quote);
            
            strcat(buffer,colon);
            
            uint16_t end = strlen(buffer);
            sprintf(buffer+end,"%d",value);
        };
        void add_pair(const char *key, double value){
            if(is_empty){
              strcat(buffer,left_brace);
              is_empty = false;
            }
            else{
                strcat(buffer,comma);
            }
            
            strcat(buffer,double_quote);
            strcat(buffer,key);
            strcat(buffer,double_quote);
            
            strcat(buffer,colon);
            
            if(isnan(value)){
                value = 0.0;
            }

            uint16_t end = strlen(buffer);
            sprintf(buffer+end,"%.1f",value);
        };
        char* serialize(){
            strcat(buffer,"}");
            return buffer;
        }
        void clear(){
            buffer[0] = '\0';      
            is_empty = true;
        }
    
    private:
        char *buffer;
        bool is_empty;
        
        const char left_brace[2] = "{";
        const char right_brace[2] = "}"; 
        const char double_quote[4] = "\"";
        const char comma[2] = ",";
        const char colon[2] = ":";
        
};

#endif