//
// Created by cybik on 22-02-15.
//


#include <cstdio>
#include <cstdlib>
#include <resources/SASSProcess.h>
#include "sass/context.h"

#include <sysexits.h>
#include <cstring>
#include <string>

#define BUFSIZE 512
std::string SASSProcess(std::string input) {

    struct Sass_Options* options = sass_make_options();
    sass_option_set_output_style(options, SASS_STYLE_EXPANDED);
    sass_option_set_precision(options, 10);
    sass_option_set_is_indented_syntax_src(options, true);

    struct Sass_Data_Context* ctx;
    //char buffer[BUFSIZE];
    //size_t size = 1;
    char *source_string = (char*)malloc(sizeof(char) * (input.size()+1));

    if(source_string == NULL) {
        printf("Allocation failed");
        return "";
    }

    source_string[0] = '\0';
    strcat(source_string, input.c_str());

    ctx = sass_make_data_context(source_string);
    struct Sass_Context* ctx_out = sass_data_context_get_context(ctx);
    sass_data_context_set_options(ctx, options);
    sass_compile_data_context(ctx);
    int status = sass_context_get_error_status(ctx_out);
    std::string output;
    if(status != 0) {
        output = sass_context_get_error_message(ctx_out);
    } else {
        output = sass_context_get_output_string(ctx_out);
    }
    sass_delete_data_context(ctx);
    return (status>0?"":output);
}