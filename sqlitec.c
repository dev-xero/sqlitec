#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Define constants */
#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 225

/* Define an enum for meta command results */
typedef enum
{
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED
} MetaCommandResult;

/* State enums */
typedef enum
{
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED
} PrepareResult;

/* Statement type enums */
typedef enum
{
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

/* Define a struct for rows */
typedef struct
{
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
} Row;

/* Define a struct for the default input buffer */
typedef struct
{
    char *buffer;
    size_t buffer_length;
    size_t input_length;
} InputBuffer;

/* Define a struct for statement information */
typedef struct
{
    StatementType type;
    Row row_to_insert; // only used for insertions
} Statement;

/* Instantiate a new input buffer */
InputBuffer *new_input_buffer()
{
    InputBuffer *input_buffer = (InputBuffer *)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;
}

/* Prints a prompt to the user */
void print_prompt() { printf("db > "); }

/* Reads input */
void read_input(InputBuffer *input_buffer)
{
    size_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

    if (bytes_read <= 0)
    {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    // Ignore trailing newlines
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

/* Closes input buffer */
void close_input_buffer(InputBuffer *input_buffer)
{
    free(input_buffer->buffer);
    free(input_buffer);
}

/* Performs a meta-command */
MetaCommandResult do_meta_command(InputBuffer *input_buffer)
{
    if (strcmp(input_buffer->buffer, ".exit") == 0)
        exit(EXIT_SUCCESS);
    else
        return META_COMMAND_UNRECOGNIZED;
}

/* Prepares SQL statement */
PrepareResult prepare_statement(InputBuffer *input_buffer, Statement *statement)
{
    if (strncmp(input_buffer->buffer, "insert", 6) == 0)
    { // Use strncmp() since insert will be followed by data
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(
            input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert.id),
            statement->row_to_insert.username,
            statement->row_to_insert.email);
        if (args_assigned < 3)
        {
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0)
    {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED;
}

/* Executes statements */
void execute_statement(Statement *statement)
{
    switch (statement->type)
    {
    case (STATEMENT_INSERT):
        printf("Insert statement handled here.\n");
        break;
    case (STATEMENT_SELECT):
        printf("Select statement handled here.\n");
        break;
    }
}

/** SQLiteC main */
int main(int argc, char *argv[])
{
    InputBuffer *input_buffer = new_input_buffer();

    while (true)
    {
        print_prompt();
        read_input(input_buffer);

        // meta commands, (dot commands) are handled here
        if (input_buffer->buffer[0] == '.')
        {
            switch (do_meta_command(input_buffer))
            {
            case (META_COMMAND_SUCCESS):
                continue;
            case (META_COMMAND_UNRECOGNIZED):
                printf("Unrecognized command '%s'\n", input_buffer->buffer);
                continue;
            }
        }

        // SQL statements are handled here
        Statement statement;
        switch (prepare_statement(input_buffer, &statement))
        {
        case (PREPARE_SUCCESS):
            break;
        case (PREPARE_UNRECOGNIZED):
            printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
            continue;
        }

        execute_statement(&statement);
        printf("Executed.\n");
    }
}