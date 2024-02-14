struct Command {
    const char *lang;
    const char *code_file_name;
    const char *code_input_file_name;
    const char *code_output_file_name;
    const char *cwd;
    char command[600];
};

void run_container(struct Command *command);

void build_command(struct Command *lang);
